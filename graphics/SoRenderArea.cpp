//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : SoRenderArea.cpp
//
// Note     :   This file is a derivative work of 'SoViewer' library available at
//              http://code.google.com/p/openinventorviewer/. A BSD-style license
//              applies. See License.txt
//==============================================================================

#include "SoRenderArea.h"
#include <Inventor/SoDB.h>
#include <Inventor/nodekits/SoNodeKit.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/elements/SoGLCacheContextElement.h>

namespace Grape
{

SO_EVENT_SOURCE(SoWheelEvent);

//==============================================================================
void SoRenderArea::init()
//==============================================================================
{
    SoDB::init();
    SoNodeKit::init();
    SoInteraction::init();
}

//------------------------------------------------------------------------------
SoRenderArea::SoRenderArea()
//------------------------------------------------------------------------------
{
    SoRenderArea::init();

    _pWheelEvent = new SoWheelEvent;
    _pKeyboardEvent = new SoKeyboardEvent;
    _pMouseButtonEvent = new SoMouseButtonEvent;
    _pLocation2Event = new SoLocation2Event;
    _pEventManager = new SoEventManager;
    _pRenderManager = new SoRenderManager;
    _pRenderManager->getGLRenderAction()->setCacheContext( SoGLCacheContextElement::getUniqueCacheContext() );
    _pRenderManager->setBackgroundColor(SbColor4f(0.0f, 0.0f, 0.0f, 1.0f));
    _pRenderManager->setRenderCallback(renderCallback, this); // turns on auto-update if callback is valid
}

//------------------------------------------------------------------------------
SoRenderArea::~SoRenderArea()
//------------------------------------------------------------------------------
{
    delete _pRenderManager;
    delete _pEventManager;
    delete _pLocation2Event;
    delete _pMouseButtonEvent;
    delete _pKeyboardEvent;
    delete _pWheelEvent;
}

//------------------------------------------------------------------------------
void SoRenderArea::setSceneGraph(SoNode *pScene)
//------------------------------------------------------------------------------
{
    _pRenderManager->deactivate();
    _pRenderManager->setSceneGraph( pScene );
    _pEventManager->setSceneGraph( pScene );
    _pRenderManager->activate();
    _pRenderManager->scheduleRedraw();
}

//------------------------------------------------------------------------------
void SoRenderArea::setViewportRegion(const SbViewportRegion& region)
//------------------------------------------------------------------------------
{
    getGLRenderAction()->setViewportRegion(region);
    _pEventManager->setViewportRegion( region );
}


//------------------------------------------------------------------------------
void SoRenderArea::setAutoRedraw(SbBool enable)
//------------------------------------------------------------------------------
{
    if( enable )
    {
        _pRenderManager->setRenderCallback(renderCallback, this);
    }
    else
    {
        _pRenderManager->setRenderCallback(NULL, NULL);
    }
}

//------------------------------------------------------------------------------
void SoRenderArea::soKeyPressEvent( SoKeyboardEvent *pEvent)
//------------------------------------------------------------------------------
{
    _pEventManager->processEvent( pEvent );
}

//------------------------------------------------------------------------------
void SoRenderArea::soKeyReleaseEvent( SoKeyboardEvent *pEvent)
//------------------------------------------------------------------------------
{
    _pEventManager->processEvent( pEvent );
}

//------------------------------------------------------------------------------
void SoRenderArea::soMouseMoveEvent( SoLocation2Event *pEvent )
//------------------------------------------------------------------------------
{
    _pEventManager->processEvent( pEvent );
}

//------------------------------------------------------------------------------
void SoRenderArea::soMousePressEvent( SoMouseButtonEvent *pEvent )
//------------------------------------------------------------------------------
{
    _pEventManager->processEvent( pEvent );
}

//------------------------------------------------------------------------------
void SoRenderArea::soMouseReleaseEvent( SoMouseButtonEvent *pEvent )
//------------------------------------------------------------------------------
{
    _pEventManager->processEvent( pEvent );
}

//------------------------------------------------------------------------------
void SoRenderArea::soWheelEvent( SoWheelEvent *pEvent )
//------------------------------------------------------------------------------
{
    _pEventManager->processEvent( pEvent );
}

//------------------------------------------------------------------------------
void SoRenderArea::soResizeEvent(int width, int height)
//------------------------------------------------------------------------------
{
    SbVec2s size((short)width, (short) height);
    SbViewportRegion region(size);

    _pRenderManager->setWindowSize(size);
    _pRenderManager->setSize(size);
    _pEventManager->setSize(size);
    _pRenderManager->setViewportRegion(region);
    _pEventManager->setViewportRegion(region);
    _pRenderManager->scheduleRedraw();
}

//------------------------------------------------------------------------------
void SoRenderArea::soPaintEvent()
//------------------------------------------------------------------------------
{
    _pRenderManager->render(/*clearWindow*/ 1, /*clearZBuffer*/ 1);
}

//------------------------------------------------------------------------------
void SoRenderArea::renderCallback(void *pUserData, SoRenderManager*)
//------------------------------------------------------------------------------
{
    ((SoRenderArea*)pUserData)->soRenderCallback();
}

} // Grape
