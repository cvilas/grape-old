//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : SoRenderArea.h
// Brief    : top level interface to Coin Open-Inventor library
//
// Note     :   This file is a derivative work of 'SoViewer' library available at
//              http://code.google.com/p/openinventorviewer/. A BSD-style license
//              applies. See License.txt
//==============================================================================

#ifndef GRAPE_SORENDERAREA_H
#define GRAPE_SORENDERAREA_H

#include "SoQtg.h"
#include <Inventor/nodes/SoNode.h>
#include <Inventor/SoRenderManager.h>
#include <Inventor/SoEventManager.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/events/SoMouseButtonEvent.h>

namespace Grape
{

class SoWheelEvent;

/// \class SoRenderArea
/// \ingroup graphics
/// \brief Top-level interface to Coin Open-Inventor library.
///
/// This is the base class that provides an interface to the Coin Open-Inventor
/// library. Methods for scenegraph management and event handling are implemented
/// here. Application programmers will use only derived classes directly.
///
/// This and it's daughter classes are derivative work of SoViewer library available
/// here: http://code.google.com/p/openinventorviewer/
///
/// Example program:
/// See examples in /examples/Inventor
class GRAPEGRAPHICS_DLL_API SoRenderArea
{
public:
    /// Initialize the Coin system. This needs to be done as the first thing
    /// before you start using the library, or you'll probably see an early crash.
    static void init();

    /// Sets the scene graph to be rendered in the view window.
    /// \param pScene (input) Pointer to open-inventor scene graph.
    virtual void setSceneGraph(SoNode *pScene);

    /// Get the scene graph, including any additional lights and camera that
    /// may be added by derived classes.
    /// \return Pointer to open-inventor scene graph.
    virtual SoNode* getSceneGraph() const { return _pRenderManager->getSceneGraph(); }

    /// Sets the camera to be used.
    virtual void setCamera (SoCamera* pCamera) { _pEventManager->setCamera(pCamera); }

    /// Access the active camera.
    /// \return Pointer to the current camera.
    virtual SoCamera* getCamera() const { return _pEventManager->getCamera(); }

    /// Set viewport region to use for rendering.
    /// \param region (input) View port region.
    void setViewportRegion(const SbViewportRegion& region);

    /// Gets current viewport region in use for rendering.
    /// \return current view port region
    const SbViewportRegion& getViewportRegion() const { return _pEventManager->getViewportRegion(); }

    /// Sets the background color for this window. Default is black (0,0,0).
    /// \param color (input) Background color.
    inline void setBackgroundColor(const SbColor4f& color);

    /// Gets the background color for this window.
    /// \return Background color.
    const SbColor4f& getBackgroundColor() const { return _pRenderManager->getBackgroundColor(); }

    /// Set the action to use for rendering. Overrides any defaukt action.
    /// \param pRA (input) OpenGL rendering action.
    inline void setGLRenderAction(SoGLRenderAction* const pRA);

    /// Access the OpenGL render action.
    /// \return Pointer to render action.
    SoGLRenderAction* getGLRenderAction() const { return _pRenderManager->getGLRenderAction(); }

    /// Set mode of rendering of primitives.
    inline virtual void setRenderMode (const SoRenderManager::RenderMode mode);

    /// Get the render mode for primitives.
    /// \return current mode of rendering of primitives.
    SoRenderManager::RenderMode getRenderMode () const { return _pRenderManager->getRenderMode(); }

    /// Set mode of rendering of stereoscopic images
    inline virtual void setStereoMode (const SoRenderManager::StereoMode mode);

    /// Get the current stereo rendering mode.
    /// \return current mode of rendering stereo.
    SoRenderManager::StereoMode getStereoMode () const{ return _pRenderManager->getStereoMode(); }

    /// Set offset when doing stereo rendering.
    inline virtual void setStereoOffset (const float offset);

    /// Get the stereo rendering offset.
    /// \return current stereo offset.
    float getStereoOffset(void) const { return _pRenderManager->getStereoOffset(); }

    /// Turn anti-aliasing on/off
    /// \param smoothing (input) Set to true to smooth jagged edges. (See SoGLRenderAction::setSmoothing()).
    /// \param numPasses (input) Set the number of rendering passes. Anything greater than the default
    ///				  of 1 will enable antialiasing through the use of OpenGL accumulation buffer.
    inline virtual void setAntialiasing (const SbBool smoothing, const int numPasses);

    /// Returns rendering pass information. See setAntialisasing().
    void getAntialiasing (SbBool &smoothing, int &numPasses) const { _pRenderManager->getAntialiasing(smoothing, numPasses); }

    /// Tell scene manager that double buffering is being used.
    inline virtual void setDoubleBuffer(const SbBool enable);

    /// Check whether double buffering is being used.
    /// \return true if double buffering is used.
    virtual SbBool isDoubleBuffer() const { return _pRenderManager->isDoubleBuffer(); }

    /// Tell the scene manager to enable automatic redraws of the scene upon detecting
    /// changes in the scenegraph. The default is on.
    virtual void setAutoRedraw(SbBool enable);

    /// \return true if the scene manager automatically redraws the scene upon detecting changes in the
    /// scenegraph.
    virtual SbBool isAutoRedraw (void) const { return _pRenderManager->isAutoRedraw(); }

    /*
    /// Sets strategy for adjusting camera clipping plane
    inline virtual void setAutoClipping(SoRenderManager::AutoClippingStrategy autoClipping);

    /// This method returns the current autoclipping strategy.
    SoRenderManager::AutoClippingStrategy getAutoClipping() const { return _pRenderManager->getAutoClipping(); }
    */

    /// Activate rendering and event handling. The default is 'off' until setSceneGraph() is called.
    virtual void activate() { _pRenderManager->activate(); }

    /// Deactivate rendering and event handling.
    virtual void deactivate() { _pRenderManager->deactivate(); }

    /// Re-initialize after parameters affecting the OpenGL context has changed.
    virtual void reinitialize() { _pRenderManager->reinitialize(); }

protected:
    SoRenderArea();
    virtual ~SoRenderArea();

    virtual void soKeyPressEvent( SoKeyboardEvent *pEvent);
    virtual void soKeyReleaseEvent( SoKeyboardEvent *pEvent );
    virtual void soMouseMoveEvent( SoLocation2Event *pEvent );
    virtual void soMousePressEvent( SoMouseButtonEvent *pEvent );
    virtual void soMouseReleaseEvent( SoMouseButtonEvent *pEvent );
    virtual void soWheelEvent( SoWheelEvent *pEvent );
    virtual void soResizeEvent(int width, int height);
    virtual void soPaintEvent();

    virtual void soRenderCallback() = 0;
private:
    static void renderCallback(void *pUserData, SoRenderManager *pManager);

protected:
    SoEventManager*		_pEventManager;
    SoRenderManager*	_pRenderManager;
    SoWheelEvent*		_pWheelEvent;
    SoKeyboardEvent*	_pKeyboardEvent;
    SoMouseButtonEvent*	_pMouseButtonEvent;
    SoLocation2Event*	_pLocation2Event;

}; //SoRenderArea

/// \class SoWheelEvent
/// \ingroup graphics
/// \brief Describes a mouse wheel event
class GRAPEGRAPHICS_DLL_API SoWheelEvent : public SoEvent
{
    SO_EVENT_HEADER();
public:
    SoWheelEvent(){}
    void setDelta(int delta){ _delta = delta; }
    int getDelta() const{ return _delta; }
private:
    int _delta;
};

//==============================================================================
void SoRenderArea::setBackgroundColor(const SbColor4f& color)
//==============================================================================
{
    _pRenderManager->setBackgroundColor(color);
    _pRenderManager->scheduleRedraw();
}

//------------------------------------------------------------------------------
void SoRenderArea::setGLRenderAction(SoGLRenderAction* const pRA)
//------------------------------------------------------------------------------
{
    _pRenderManager->setGLRenderAction(pRA);
    _pRenderManager->scheduleRedraw();
}

//------------------------------------------------------------------------------
void SoRenderArea::setRenderMode (const SoRenderManager::RenderMode mode)
//------------------------------------------------------------------------------
{
    _pRenderManager->setRenderMode(mode);
    _pRenderManager->scheduleRedraw();
}

//------------------------------------------------------------------------------
void SoRenderArea::setStereoMode (const SoRenderManager::StereoMode mode)
//------------------------------------------------------------------------------
{
    _pRenderManager->setStereoMode(mode);
    _pRenderManager->scheduleRedraw();
}

//------------------------------------------------------------------------------
void SoRenderArea::setStereoOffset (const float offset)
//------------------------------------------------------------------------------
{
    _pRenderManager->setStereoOffset(offset);
    _pRenderManager->scheduleRedraw();
}

//------------------------------------------------------------------------------
void SoRenderArea::setAntialiasing (const SbBool smoothing, const int numPasses)
//------------------------------------------------------------------------------
{
    _pRenderManager->setAntialiasing(smoothing, numPasses);
    _pRenderManager->scheduleRedraw();
}

//------------------------------------------------------------------------------
void SoRenderArea::setDoubleBuffer(const SbBool enable)
//------------------------------------------------------------------------------
{
    _pRenderManager->setDoubleBuffer(enable);
    _pRenderManager->scheduleRedraw();
}

/*
//------------------------------------------------------------------------------
void SoRenderArea::setAutoClipping(SoRenderManager::AutoClippingStrategy autoClipping)
//------------------------------------------------------------------------------
{
    _pRenderManager->setAutoClipping(autoClipping);
}
*/

} // Grape

#endif // GRAPE_SORENDERAREA_H
