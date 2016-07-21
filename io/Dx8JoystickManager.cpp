//==============================================================================
// Project  : Grape
// Module   : IO
// File     : Dx8JoystickManager.cpp
//
// Copyright (c) 2012, Vilas Chitrakaran
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//    * Neither the name of the copyright holder nor the names of its
//      contributors may be used to endorse or promote products derived from
//      this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR ITS CONTRIBUTORS
// BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
// THE POSSIBILITY OF SUCH DAMAGE.
//==============================================================================

#include "Dx8JoystickManager.h"

namespace grape
{

//==============================================================================
Dx8JoystickManager::Dx8JoystickManager()
//==============================================================================
{
    _pDirectInput = 0;
    _deviceList.clear();
    DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION,
                       IID_IDirectInput8, (void **) &_pDirectInput, NULL);
    enumerateDevices();
}

//------------------------------------------------------------------------------
Dx8JoystickManager::~Dx8JoystickManager()
//------------------------------------------------------------------------------
{
    if( _pDirectInput )
    {
        _pDirectInput->Release();
        _pDirectInput = NULL;
    }
}

//------------------------------------------------------------------------------
bool Dx8JoystickManager::enumerateDevices()
//------------------------------------------------------------------------------
{
    Dx8JoystickManager& manager = getSingleton();
    manager._deviceList.clear();

    if( !(manager._pDirectInput) )
    {
        return false;
    }

    HRESULT hr = manager._pDirectInput->EnumDevices(DI8DEVCLASS_GAMECTRL,
                                                    Dx8JoystickManager::enumDeviceCallBack,
                                                    NULL/*(void*)(&manager)*/,
                                                    DIEDFL_ATTACHEDONLY);

    return (SUCCEEDED(hr) == true);
}

//------------------------------------------------------------------------------
bool Dx8JoystickManager::getDeviceInstance(unsigned int index, DIDEVICEINSTANCE &dinst)
//------------------------------------------------------------------------------
{
    std::vector<DIDEVICEINSTANCE>& devList = getSingleton()._deviceList;
    unsigned int nDevices = devList.size();
    if( index < nDevices )
    {
        memcpy(&dinst, &(devList[index]), sizeof(DIDEVICEINSTANCE));
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
std::string Dx8JoystickManager::getDeviceName(unsigned int index)
//------------------------------------------------------------------------------
{
    std::vector<DIDEVICEINSTANCE>& devList = getSingleton()._deviceList;
    unsigned int nDevices = devList.size();
    if( index < nDevices )
    {
        std::wstring s(devList[index].tszInstanceName);
        return std::string( s.begin(), s.end() );
    }
    return std::string("");
}

//------------------------------------------------------------------------------
const GUID &Dx8JoystickManager::getDeviceGuid(unsigned int index)
//------------------------------------------------------------------------------
{
    std::vector<DIDEVICEINSTANCE>& devList = getSingleton()._deviceList;
    unsigned int nDevices = devList.size();
    if( index < nDevices )
    {
        return devList[index].guidInstance;
    }
    return GUID_NULL;
}

//------------------------------------------------------------------------------
BOOL CALLBACK Dx8JoystickManager::enumDeviceCallBack(const DIDEVICEINSTANCE* lpddi, void* ptr)
//------------------------------------------------------------------------------
{
    ptr = ptr;
    //if( ptr == NULL )
    //{
    //	return DIENUM_STOP;
    //}
    //Dx8JoystickManager* pManager = (Dx8JoystickManager*)(ptr);
    //pManager->_deviceList.push_back(*lpddi);
    std::vector<DIDEVICEINSTANCE>& devList = getSingleton()._deviceList;
    devList.push_back(*lpddi);
    return DIENUM_CONTINUE;
}

//------------------------------------------------------------------------------
LPDIRECTINPUTDEVICE8 Dx8JoystickManager::attach(unsigned int index, GUID &guid)
//------------------------------------------------------------------------------
{
    std::vector<DIDEVICEINSTANCE>& devList = getSingleton()._deviceList;
    LPDIRECTINPUT8 pDI = getSingleton()._pDirectInput;

    if( !(pDI) )
    {
        return NULL;
    }

    unsigned int nDevices = devList.size();
    if( index < nDevices )
    {

        // obtain interface to a device
        LPDIRECTINPUTDEVICE8 pDevice = NULL;
        HRESULT hr = pDI->CreateDevice(devList[index].guidInstance, &pDevice, NULL );
        if( FAILED(hr) )
        {
            return NULL;
        }

        memcpy(&guid, &(devList[index].guidInstance), sizeof(GUID));
        return pDevice;
    }
    return NULL;
}

//------------------------------------------------------------------------------
LPDIRECTINPUTDEVICE8 Dx8JoystickManager::attach(const std::string &name, GUID &guid)
//------------------------------------------------------------------------------
{
    std::vector<DIDEVICEINSTANCE>& devList = getSingleton()._deviceList;
    LPDIRECTINPUT8 pDI = getSingleton()._pDirectInput;

    if( !pDI )
    {
        return NULL;
    }

    // find device in the enumerated list
    std::vector<DIDEVICEINSTANCE>::iterator it = devList.begin();
    LPDIRECTINPUTDEVICE8 pDevice = NULL;
    while( it != devList.end() )
    {
        std::wstring s(it->tszInstanceName);
        if( std::string(s.begin(), s.end()) == name )
        {
            // obtain interface to a device
            HRESULT hr = pDI->CreateDevice(it->guidInstance, &pDevice, NULL );
            if( SUCCEEDED(hr) )
            {
                break;
            }
        }
        ++it;
    }
    if( it == devList.end() )
    {
        return NULL;
    }

    // must have a valid device now
    if( NULL != pDevice )
    {
        memcpy(&guid, &(it->guidInstance), sizeof(GUID));
    }

    return pDevice;
}

//------------------------------------------------------------------------------
LPDIRECTINPUTDEVICE8 Dx8JoystickManager::attach(const GUID& guid)
//------------------------------------------------------------------------------
{
    LPDIRECTINPUT8 pDI = getSingleton()._pDirectInput;

    if( !pDI )
    {
        return NULL;
    }

    // obtain interface to a device
    LPDIRECTINPUTDEVICE8 pDevice = NULL;
    HRESULT hr = pDI->CreateDevice(guid, &pDevice, NULL );
    if( FAILED(hr) )
    {
        return NULL;
    }

    return pDevice;
}

//------------------------------------------------------------------------------
void Dx8JoystickManager::detach(LPDIRECTINPUTDEVICE8 &pDevice)
//------------------------------------------------------------------------------
{
    if( pDevice == NULL )
    {
        return;
    }
    pDevice->Unacquire();
    pDevice->Release();
    pDevice = NULL;
}

} // grape
