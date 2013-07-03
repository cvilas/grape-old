//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : SoQtOffscreenRenderer.cpp
//==============================================================================

#include "SoQtOffscreenRenderer.h"
#include <Inventor/elements/SoGLCacheContextElement.h>
#include <QGLFormat>

namespace Grape
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

} // Grape
