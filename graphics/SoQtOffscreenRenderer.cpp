//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : SoQtOffscreenRenderer.cpp
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

#include "SoQtOffscreenRenderer.h"
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <QGLFormat>

namespace grape
{

//==============================================================================
static void preRenderCB(void * pUserdata, SoGLRenderAction * pAction)
//==============================================================================
{
    pUserdata = pUserdata;
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    pAction->setRenderingIsRemote(FALSE);
}

//==============================================================================
SoQtOffscreenRenderer::SoQtOffscreenRenderer(const SbViewportRegion &vport, const QGLFormat &fmt)
//==============================================================================
: _glFormat(fmt), _pBuffer(0), _pRenderAction(0), _isLocalAction(FALSE)
{
    init(vport, 0);
}

//------------------------------------------------------------------------------
SoQtOffscreenRenderer::SoQtOffscreenRenderer(SoGLRenderAction* pAction, const QGLFormat &fmt)
//------------------------------------------------------------------------------
: _glFormat(fmt), _pBuffer(0), _pRenderAction(0), _isLocalAction(FALSE)
{
    init(pAction->getViewportRegion(), pAction);
}

//------------------------------------------------------------------------------
void SoQtOffscreenRenderer::init(const SbViewportRegion& vp, SoGLRenderAction* pAction)
//------------------------------------------------------------------------------
{
    _bkColor.setValue(0, 0, 0);
    setViewportRegion(vp);
    if( pAction )
    {
        setGLRenderAction(pAction);
    }
    else
    {
        _pRenderAction = new SoGLRenderAction(vp);
        _isLocalAction = TRUE;
        _pRenderAction->setCacheContext(SoGLCacheContextElement::getUniqueCacheContext());
        _pRenderAction->setTransparencyType(SoGLRenderAction::SORTED_OBJECT_BLEND);
    }
}

//------------------------------------------------------------------------------
SoQtOffscreenRenderer::~SoQtOffscreenRenderer()
//------------------------------------------------------------------------------
{
    if( _pBuffer )
    {
        delete _pBuffer;
    }

    if( _isLocalAction )
    {
        delete _pRenderAction;
    }
}

//------------------------------------------------------------------------------
void SoQtOffscreenRenderer::setViewportRegion(const SbViewportRegion &vport)
//------------------------------------------------------------------------------
{
    _viewPort = vport;
    makeBuffer();
}

//------------------------------------------------------------------------------
void SoQtOffscreenRenderer::setBackgroundColor(const SbColor& color)
//------------------------------------------------------------------------------
{
    _bkColor = color;
}

//------------------------------------------------------------------------------
void SoQtOffscreenRenderer::setGLRenderAction(SoGLRenderAction* pAction)
//------------------------------------------------------------------------------
{
    if( _pRenderAction == pAction )
    {
        return;
    }
    if( _isLocalAction )
    {
        delete _pRenderAction;
    }
    _pRenderAction = pAction;
    _isLocalAction = FALSE;
}

//------------------------------------------------------------------------------
SbBool SoQtOffscreenRenderer::render(SoNode* pScene)
//------------------------------------------------------------------------------
{
    return renderFromBase(pScene);
}

//------------------------------------------------------------------------------
SbBool SoQtOffscreenRenderer::render(SoPath* pPath)
//------------------------------------------------------------------------------
{
    return renderFromBase(pPath);
}

//------------------------------------------------------------------------------
SbBool SoQtOffscreenRenderer::renderFromBase(SoBase* pBase)
//------------------------------------------------------------------------------
{
    _pBuffer->makeCurrent();

    // oldcontext is used to restore the previous context id, in case
    // the render action is not allocated by us.
    const uint32_t oldContext = _pRenderAction->getCacheContext();
    _pRenderAction->setCacheContext(_cacheContext);

    glEnable(GL_DEPTH_TEST);
    glClearColor(_bkColor[0], _bkColor[1], _bkColor[2], 0.0f);

    // needed to clear viewport after glViewport() is called from SoGLRenderAction
    _pRenderAction->addPreRenderCallback(preRenderCB, NULL);
    _pRenderAction->setViewportRegion(_viewPort);

    if( pBase->isOfType(SoNode::getClassTypeId()) )
    {
        _pRenderAction->apply( (SoNode*)pBase );
    }
    else if( pBase->isOfType(SoPath::getClassTypeId()) )
    {
        _pRenderAction->apply( (SoPath*)pBase );
    }
    else
    {
        return FALSE;
    }

    _pRenderAction->removePreRenderCallback(preRenderCB, NULL);

    _pBuffer->doneCurrent();
    _pRenderAction->setCacheContext(oldContext); // restore old

    return TRUE;
}

//------------------------------------------------------------------------------
void SoQtOffscreenRenderer::makeBuffer()
//------------------------------------------------------------------------------
{
    if( _pBuffer )
    {
        delete _pBuffer;
    }
    const SbVec2s size = _viewPort.getWindowSize();
    _pBuffer = new QGLPixelBuffer(size[0], size[1], _glFormat);
    _cacheContext = SoGLCacheContextElement::getUniqueCacheContext();
}

} // grape
