//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : SoQtOffscreenRenderer.h
// Brief    : offscreen memory buffer for IV scene graph
//==============================================================================

#ifndef GRAPE_SOQTOFFSCREENRENDERER_H
#define GRAPE_SOQTOFFSCREENRENDERER_H

#include "SoQtg.h"
#include <QtOpenGL/QGLPixelBuffer>
#include <QtOpenGL/QGLFormat>
#include <QImage>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/SbColor.h>
#include <Inventor/SoPath.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/nodes/SoNode.h>

namespace Grape
{

/// \class SoQtOffscreenRenderer
/// \ingroup graphics
/// \brief Renders Open-Inventor scenegraph into offscreen memory buffers.
///
/// This class replicates the functionality in SoOffscreenRenderer in the Coin
/// library, and provides better quality anti-aliasing in rendering. It uses
/// QGLPixelBuffer as the underlying offscreen render buffer.
///
/// Example program:
/// \include Inventor/OffscreenRendererExample.cpp
class GRAPEGRAPHICS_DLL_API SoQtOffscreenRenderer
{
public:
    /// Constructor. Argument is the viewportregion we should use when rendering.
    /// An internal SoGLRenderAction will be constructed. For good antialiasing
    /// results, enable multisampling as follows:
    /// \code
    /// QGLFormat fmt;
    /// int nSamples = 4;
    /// fmt.setSampleBuffers(true);
    /// fmt.setSamples(nSamples);
    /// SoQtOffscreenRenderer renderer(fmt, vport);
    /// \endcode
    explicit SoQtOffscreenRenderer( const SbViewportRegion &vport, const QGLFormat &fmt = QGLFormat::defaultFormat() );

    /// Constructor. Argument is the action we should apply to the scene graph
    /// when rendering the scene. Information about the viewport is extracted from the action.
    explicit SoQtOffscreenRenderer( SoGLRenderAction* pAction, const QGLFormat &fmt = QGLFormat::defaultFormat() );

    /// Destructor.
    ~SoQtOffscreenRenderer();

    /// Returns the format of the pixel buffer. The format may be different from the one
    /// that was requested.
    QGLFormat getGLFormat() const { return _pBuffer->format(); }

    /// Sets the viewport region. This will invalidate the current
    /// buffer, if any. The buffer will not contain valid data until
    /// another call to render() happens.
    void setViewportRegion(const SbViewportRegion &vport);

    /// Returns the viewport region.
    const SbViewportRegion& getViewportRegion() const { return _viewPort; }

    /// Set the background color. The buffer is cleared to this color before rendering.
    void setBackgroundColor(const SbColor& color);

    /// Returns the background color.
    const SbColor& getBackgroundColor() const { return _bkColor; }

    /// Sets the render action. Use this if you have special rendering needs.
    void setGLRenderAction(SoGLRenderAction* pAction);

    /// Returns the rendering action currently used.
    SoGLRenderAction* getGLRenderAction() const { return _pRenderAction; }

    /// Render the scenegraph rooted at pScene into our internal pixel buffer.
    /// Important note: make sure you pass in a scene node pointer which has
    /// both a camera and at least one lightsource below it -- otherwise you
    /// are likely to end up with just a blank or black image buffer.
    /// This mistake is easily made if you use an SoQtOffscreenRenderer on a
    /// scenegraph from one of the standard viewer components, as you will
    /// often just leave the addition of a camera and a headlight light source
    /// to the viewer to set up. This camera and light source are then part
    /// of the viewer's private "super-graph" outside of the scope of the
    /// scenegraph passed in by the application programmer. To make sure the
    /// complete scenegraph (including the viewer's camera and lights) are passed
    /// to this method, you can get the scenegraph root from the viewer's internal
    /// SoSceneManager instance instead of from the viewer's own getSceneGraph() method,
    /// like this:
    ///
    /// \code
    /// SoQtOffscreenRenderer* pRenderer = new SoQtOffscreenRenderer(vpregion);
    /// SoNode* pRoot = pViewer->getSceneManager()->getSceneGraph();
    /// SbBool ok = pRenderer->render(pRoot);
    /// // [then use image buffer in a texture, or write it to file, or whatever]
    /// \endcode
    ///
    /// If you do this and still get a blank buffer, another common problem is to
    /// have a camera which is not actually pointing at the scene geometry you want
    /// a snapshot of. If you suspect that could be the cause of problems on your end,
    /// take a look at SoCamera::pointAt() and SoCamera::viewAll() to see how you can
    /// make a camera node guaranteed to be directed at the scene geometry.
    ///
    /// Yet another common mistake when setting up the camera is to specify values
    /// for the SoCamera::nearDistance and SoCamera::farDistance fields which doesn't
    /// not enclose the full scene. This will result in either just the background color,
    /// or that parts at the front or the back of the scene will not be visible in the rendering.
    SbBool render(SoNode* pScene);

    /// Render the scene specified by path into our internal memory buffer.
    SbBool render(SoPath* pPath);

    /// Returns the offscreen pixel buffer containing rendered
    /// image after call to render().
    QGLPixelBuffer* getBuffer() const { return _pBuffer; }

    /// Return the contents of the pixel buffer as a QImage.
    /// Use this for file I/O
    QImage getImage() { return _pBuffer->toImage(); }

private:
    void init(const SbViewportRegion& vp, SoGLRenderAction* pAction);
    SbBool renderFromBase(SoBase* pBase);
    void makeBuffer();

private:
    QGLFormat			_glFormat;
    QGLPixelBuffer*		_pBuffer;
    uint32_t			_cacheContext;
    SbViewportRegion	_viewPort;
    SbColor				_bkColor;
    SoGLRenderAction*	_pRenderAction;
    SbBool				_isLocalAction;
}; // SoQtOffscreenRenderer

} // Grape

#endif // GRAPE_SOQTOFFSCREENRENDERER_H
