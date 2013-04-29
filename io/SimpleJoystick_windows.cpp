//==============================================================================
// Project  : Grape
// Module   : IO
// File     : SimpleJoystick_windows.cpp
//==============================================================================

#include "SimpleJoystick.h"
#include "Dx8JoystickManager.h"
#include <iostream>
#include <sstream>

namespace Grape
{

/// \class SimpleJoystickP
/// \brief Windows specific private implementation.
///
/// The following implementation uses the DirectInput API (DirectX SDK).

class SimpleJoystickP
{
public:
    SimpleJoystickP(HWND hWnd);
    ~SimpleJoystickP() {}
    bool attach(unsigned int index);
    bool attach(const GUID& guid);
    bool attach(const std::string& name);
    void detach();
    bool isConnected() const { return _jsMan.isDeviceConnected(_instanceGUID); }
    std::string getDeviceName() const { return _instanceName; }
    const DIDEVCAPS& getDeviceCapabilities() const { return _deviceCaps; }
    bool setupDevice();
    bool setDeadZone(int val);
    int getDeadZone() const { return _deadZone; }
    bool readState(DIJOYSTATE& state);
    bool isAxisPresent(int i) const;
    void dispErrMsg(const std::string &func, const std::string &msg);
private:
    static BOOL CALLBACK enumAxesCallBack(const DIDEVICEOBJECTINSTANCE* lpdidoi, void* ptr);
public:
    HWND                    _hWnd;			//!< 'Top level' app window handle (user specified)
    int						_axesRange;		//!< full range for axes (user specified)
    int						_deadZone;		//!< dead zone
    LPDIRECTINPUTDEVICE8	_pDevice;		//!< Device interface
    DIDEVCAPS				_deviceCaps;	//!< Device capabilities
    bool					_axisPresent[8]; //!< available axes
    int						_nSliders;		//!< number of sliders found
    GUID					_instanceGUID;	//!< GUID of active device
    std::string				_instanceName;	//!< Name of the active device
    Dx8JoystickManager&     _jsMan;         //!< singleton manager interface
};

//==============================================================================
SimpleJoystickP::SimpleJoystickP(HWND hWnd)
//==============================================================================
    : _hWnd(hWnd),
      _axesRange(1 << 16),
      _deadZone(0),
      _pDevice(NULL),
      _nSliders(0),
      _instanceGUID(GUID_NULL),
      _jsMan(Dx8JoystickManager::getSingleton())
{
    memset(&_deviceCaps, 0, sizeof(DIDEVCAPS));
    memset(&_axisPresent, 0, 8 * sizeof(bool));
    _jsMan.enumerateDevices(); // initialize DirectInput interface
}

//------------------------------------------------------------------------------
bool SimpleJoystickP::attach(unsigned int index)
//------------------------------------------------------------------------------
{
    return ((_pDevice = _jsMan.attach(index, _instanceGUID)) != NULL);
}

//------------------------------------------------------------------------------
bool SimpleJoystickP::attach(const GUID& guid)
//------------------------------------------------------------------------------
{
    memcpy(&_instanceGUID, &guid, sizeof(GUID));
    return ((_pDevice = _jsMan.attach(guid)) != NULL);
}

//------------------------------------------------------------------------------
bool SimpleJoystickP::attach(const std::string& name)
//------------------------------------------------------------------------------
{
    return ((_pDevice = _jsMan.attach(name, _instanceGUID)) != NULL);
}

//------------------------------------------------------------------------------
void SimpleJoystickP::detach()
//------------------------------------------------------------------------------
{
    _jsMan.detach(_pDevice);
    memset( &_axisPresent, 0, 8 * sizeof(bool));
    memset( &_deviceCaps, 0, sizeof(DIDEVCAPS));
    _instanceGUID = GUID_NULL;
    _instanceName.clear();
    _nSliders = 0;
}

//------------------------------------------------------------------------------
bool SimpleJoystickP::setupDevice()
//------------------------------------------------------------------------------
{
    if( !_pDevice )
    {
        return false;
    }

    // Set data format to "simple joystick". We pass in DIJOYSTATE to GetDeviceState
    // whenever a state update is desired.
    HRESULT hr = _pDevice->SetDataFormat( &c_dfDIJoystick );
    if( FAILED(hr) )
    {
#ifdef _DEBUG
        dispErrMsg("SimpleJoystickP::setupDevice", "FAILED SetDataFormat");
#endif
        return false;
    }

    // set access level
    /// \todo: do we need exclusive access?
    hr = _pDevice->SetCooperativeLevel(_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE );
    if( FAILED(hr) )
    {
#ifdef _DEBUG
        dispErrMsg("SimpleJoystickP::setupDevice", "FAILED SetCooperativeLevel");
#endif
        return false;
    }

    // Get device name
    DIPROPSTRING dipStr;
    dipStr.diph.dwSize = sizeof(DIPROPSTRING);
    dipStr.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipStr.diph.dwHow = DIPH_DEVICE;
    dipStr.diph.dwObj = 0;
    _pDevice->GetProperty(DIPROP_PRODUCTNAME, &dipStr.diph);

    std::wstring wname(dipStr.wsz);
    _instanceName = std::string(wname.begin(), wname.end());

    // Get device properties
    _deviceCaps.dwSize = sizeof(DIDEVCAPS);
    hr = _pDevice->GetCapabilities(&_deviceCaps);
    if( FAILED(hr) )
    {
#ifdef _DEBUG
        dispErrMsg("SimpleJoystickP::setupDevice", "FAILED GetCapabilities");
#endif
        return false;
    }

    // enumerate axes, and callback function gets range of values reported for them
    hr = _pDevice->EnumObjects(SimpleJoystickP::enumAxesCallBack, (void*)(this), DIDFT_AXIS);
    if( FAILED(hr) )
    {
#ifdef _DEBUG
        dispErrMsg("SimpleJoystickP::setupDevice", "FAILED EnumObjects(axes)");
#endif
        return false;
    }

    /*
    // is device autocentering?
    DIPROPDWORD dipW;
    dipW.diph.dwSize = sizeof(DIPROPDWORD);
    dipW.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipW.diph.dwHow = DIPH_DEVICE;
    dipW.diph.dwObj = 0;
    hr = pDevice_->GetProperty(DIPROP_AUTOCENTER, &dipW.diph );
    if( SUCCEEDED(hr) && (dipW.dwData == DIPROPAUTOCENTER_ON) )
        isAutoCentering_ = true;
    else
        isAutoCentering_ = false;
    */

    return true;
}

//------------------------------------------------------------------------------
BOOL CALLBACK SimpleJoystickP::enumAxesCallBack(const DIDEVICEOBJECTINSTANCE* lpdidoi, void* ptr)
//------------------------------------------------------------------------------
{
    if( ptr == NULL )
    {
        return DIENUM_STOP;
    }

    HRESULT hr;
    SimpleJoystickP* pClassObj = (SimpleJoystickP*)(ptr);
    if( pClassObj->_pDevice == NULL )
    {
        return DIENUM_STOP;
    }

    /*
    // Set axis to autocenter - expected to fail for most regular joysticks,
    // might work for force feedback devices.
    DIPROPDWORD dipW;
    dipW.diph.dwSize = sizeof(DIPROPDWORD);
    dipW.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipW.diph.dwHow = DIPH_DEVICE;
    dipW.diph.dwObj = 0;
    dipW.dwData = DIPROPAUTOCENTER_ON;
    hr = pClassObj->pDevice_->SetProperty(DIPROP_AUTOCENTER, &dipW.diph);
                                // this might fail, but we dont care
    */

    // Set range for this axis
    DIPROPRANGE dipRange;
    dipRange.diph.dwSize = sizeof(DIPROPRANGE);
    dipRange.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipRange.diph.dwHow = DIPH_BYID;
    dipRange.diph.dwObj = lpdidoi->dwType;
    dipRange.lMax = (pClassObj->_axesRange/2)-1;
    dipRange.lMin = -(pClassObj->_axesRange/2);

    hr = pClassObj->_pDevice->SetProperty( DIPROP_RANGE, &dipRange.diph );
    if( FAILED(hr) )
    {
        return DIENUM_STOP;
    }

    int axis;
    if( lpdidoi->guidType == GUID_XAxis )
        axis = 0;
    else if( lpdidoi->guidType == GUID_YAxis)
        axis = 1;
    else if( lpdidoi->guidType == GUID_ZAxis)
        axis = 2;
    else if( lpdidoi->guidType == GUID_RxAxis)
        axis = 3;
    else if( lpdidoi->guidType == GUID_RyAxis)
        axis = 4;
    else if( lpdidoi->guidType == GUID_RzAxis)
        axis = 5;
    else if( lpdidoi->guidType == GUID_Slider)
        axis = 6;
    else
        return DIENUM_CONTINUE;

    if( axis == 6 )
    {
        if( pClassObj->_nSliders < 2)
        {
            axis += pClassObj->_nSliders;
            pClassObj->_axisPresent[axis] = true;
            ++pClassObj->_nSliders;
        }
    }
    else
    {
        pClassObj->_axisPresent[axis] = true;
    }

    return DIENUM_CONTINUE;
}

//------------------------------------------------------------------------------
bool SimpleJoystickP::setDeadZone(int val)
//------------------------------------------------------------------------------
{
    if( _pDevice == NULL )
    {
        return false;
    }

    val = abs(val);

    // set the dead zone
    DIPROPDWORD dipW;
    dipW.diph.dwSize = sizeof(DIPROPDWORD);
    dipW.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipW.diph.dwHow = DIPH_DEVICE;
    dipW.diph.dwObj = 0;
    dipW.dwData = (val * 20000)/(_axesRange);
                    // deadzone range is 0 - 10,000, where
                    // 10,000 indicates 100%

    if( FAILED(_pDevice->SetProperty(DIPROP_DEADZONE, &dipW.diph)) )
    {
        return false;
    }

    // did it work?
    _pDevice->GetProperty(DIPROP_DEADZONE, &dipW.diph);
    _deadZone = (_axesRange * dipW.dwData)/20000;
#ifdef _DEBUG
    std::cout << "[SimpleJoystickP::setDeadZone] range = +/-" << (_axesRange>>1)
              << " set= " << val << " get= " << _deadZone << std::endl;
#endif
    return ( abs(_deadZone - val) < (_axesRange/1000) );
}

//------------------------------------------------------------------------------
bool SimpleJoystickP::isAxisPresent(int i) const
//------------------------------------------------------------------------------
{
    if( (i < 0) || (i > 7) ) return false;
    return _axisPresent[i];
}

//------------------------------------------------------------------------------
bool SimpleJoystickP::readState(DIJOYSTATE& state)
//------------------------------------------------------------------------------
{
    if( !_pDevice )
    {
        return false;
    }

    // poll to update
    HRESULT hr = _pDevice->Poll();
    if( FAILED(hr) )
    {
        // re-acquire and try again
        // NOTE: According to DirectX documentation, Acquire() never returns
        // DIERR_INPUTLOST. However the following is based on sample code
        // in the SDK.
        hr = _pDevice->Acquire();
        //while( hr == DIERR_INPUTLOST )
        //{
        //	hr = pDevice_->Acquire();
        //}

        // fatal error
        if ((hr == DIERR_INVALIDPARAM) || (hr == DIERR_NOTINITIALIZED))
        {
            return false;// E_FAIL;
        }

        // If another application has control of this device, wait our turn.
        // NOTE: We shouldn't get this error if we have exclusive access
        if (hr == DIERR_OTHERAPPHASPRIO)
        {
            return false;// S_OK;
        }
    }

    // have device. read state.
    return ( SUCCEEDED(_pDevice->GetDeviceState(sizeof(DIJOYSTATE), &state)) );
}

//-----------------------------------------------------------------------------
void SimpleJoystickP::dispErrMsg(const std::string &func, const std::string &msg)
//-----------------------------------------------------------------------------
{
    if( _hWnd ) // we have a window
    {
        std::wstring wsmsg(msg.begin(), msg.end());
        std::wstring wsfunc(func.begin(), func.end());
        MessageBox(_hWnd, wsmsg.c_str(), wsfunc.c_str(),
                   MB_OK | MB_ICONEXCLAMATION | MB_TASKMODAL);
    }
    else // just console
    {
        std::cerr << "[" << func << "]: " << msg << std::endl << std::flush;
    }

}

//==============================================================================
SimpleJoystick::SimpleJoystick(HWND hWnd)
//==============================================================================
    : _pImpl(new SimpleJoystickP(hWnd) )
{
}

//------------------------------------------------------------------------------
SimpleJoystick::~SimpleJoystick()
//------------------------------------------------------------------------------
{
    disconnect();
    delete _pImpl;
}

//------------------------------------------------------------------------------
bool SimpleJoystick::connect(const std::string& idStr)
//------------------------------------------------------------------------------
{
    disconnect();

    // idStr could be a number, name or guid formatted as string.

    // try to parse 'idStr' as guid
    GUID guid;
    if( stringToGuid(idStr, guid) )
    { // if we are here, 'idStr' must be GUID
        if( !_pImpl->attach(guid) )
        {
            return false;
        }
    }

    // didn't work. try to parse as name
    else if( !_pImpl->attach(idStr) )
    {
        // not name, maybe index
        unsigned int i;
        std::stringstream ss(idStr);
        ss >> i;
        if( !_pImpl->attach(i) )
        {
            return false;
        }
    }

    // We connected to something. Set it up.
    if( !_pImpl->setupDevice() )
    {
        disconnect();
        return false;
    }

    // resize state vectors
    const DIDEVCAPS& caps = _pImpl->getDeviceCapabilities();
    _state.axes.resize(caps.dwAxes, 0);
    _state.buttons.resize(caps.dwButtons, 0);
    _state.pov.resize(caps.dwPOVs, 0);

#ifdef _DEBUG
    std::cout << "[SimpleJoystick::connect] Opened device " << idStr << std::endl
              << "\tName: " << _pImpl->getDeviceName() << std::endl
              << "\tNumber of axes: " << caps.dwAxes << std::endl
              << "\tNumber of buttons: " << caps.dwButtons << std::endl;
#endif

    return true;
}

//------------------------------------------------------------------------------
void SimpleJoystick::disconnect()
//------------------------------------------------------------------------------
{
    _pImpl->detach();

    _state.axes.clear();
    _state.buttons.clear();
    _state.pov.clear();
}

//------------------------------------------------------------------------------
bool SimpleJoystick::isConnected() const
//------------------------------------------------------------------------------
{
    return _pImpl->isConnected();
}

//------------------------------------------------------------------------------
std::string SimpleJoystick::getName() const
//------------------------------------------------------------------------------
{
    return _pImpl->getDeviceName();
}

//------------------------------------------------------------------------------
bool SimpleJoystick::setDeadZone(int val)
//------------------------------------------------------------------------------
{
    return _pImpl->setDeadZone(val);
}

//------------------------------------------------------------------------------
int SimpleJoystick::getDeadZone() const
//------------------------------------------------------------------------------
{
    return _pImpl->getDeadZone();
}

//------------------------------------------------------------------------------
bool SimpleJoystick::update()
//------------------------------------------------------------------------------
{
    DIJOYSTATE jsState;
    if( !_pImpl->readState(jsState) )
    {
        return false;
    }

    // timestamp it
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft); // returns 100ns intervals since 1/1/1601 (UTC)
    ULARGE_INTEGER uli;
    uli.LowPart = ft.dwLowDateTime;
    uli.HighPart = ft.dwHighDateTime;
    _state.ms = uli.QuadPart/10000ULL;

    int j = 0;
    LONG* jAxes = &jsState.lX; // convert DirectX way to my way
    for(int i = 0; i < 8; ++i )
    {
        if( _pImpl->isAxisPresent(i) )
        {
            _state.axes[j] = jAxes[i];
            ++j;
        }
    }

    int nButtons = std::min<int>(_state.buttons.size(), 32);
    for(int i = 0; i < nButtons; ++i)
    {
        _state.buttons[i] = (jsState.rgbButtons[i]& 0x80);
    }

    int nPov = std::min<int>(_state.pov.size(), 4);
    for(int i = 0; i < nPov; ++i)
    {
        _state.pov[i] = (int)(jsState.rgdwPOV[i]);
    }

    return true;
}

} // Grape
