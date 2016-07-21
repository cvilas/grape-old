//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : SoRenderArea.cpp
//
// Note     :   This file is a derivative work of 'SoViewer' library available at
//              http://code.google.com/p/openinventorviewer/.
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

#include "SoRenderArea.h"
#include <Inventor/SoDB.h>
#include <Inventor/nodekits/SoNodeKit.h>
#include <Inventor/SoInteraction.h>
#include <Inventor/elements/SoGLCacheContextElement.h>

namespace grape
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

} // grape
