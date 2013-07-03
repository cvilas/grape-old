//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : SoSceneViewer.h
// Brief    : Viewer widget for Open-Inventor scenegraph
//==============================================================================

#ifndef GRAPE_SOSCENEVIEWER_H
#define GRAPE_SOSCENEVIEWER_H

#include "SoQtGraphicsScene.h"
#include <QGraphicsView>
#include <QMap>

class QGLWidget;

namespace Grape
{

/// \class SoSceneViewer
/// \ingroup graphics
/// \brief Viewer for OpenInventor scenegraphs.
///
/// The SoSceneViewer class provides a visual appearance to SoQtGraphicsScene
/// object, and allows overlaying standard Qt 2D widgets on top of an OpenGL scene.
///
/// Example program:
/// \include Inventor/SimpleAnimationExample.cpp
class GRAPEGRAPHICS_DLL_API SoSceneViewer : public QGraphicsView
{
    Q_OBJECT
public:

    /// The constructor initializes the viewer.
    /// \param pParent	Pointer to the parent widget.
    explicit SoSceneViewer(QWidget* pParent = 0);

    /// Destructor frees any allocated resources
    virtual ~SoSceneViewer();

    /// Sets the background color for this window. Default is black (0,0,0).
    /// \param color (input) Background color.
    inline void setBackgroundColor(const QColor& color);

    /// Convenience function to add an Open Inventor scene graph to display on the viewer.
    /// \param pScene	(input) Root node of the user specified scene graph.
    void setSceneGraph(SoNode* pScene);

    /// Obtain a pointer to OpenGL rendering surface for user manipulation of
    /// cameras, etc.
    /// \return Pointer to render area.
    SoQtGraphicsScene* getRenderArea() const { return _pViewArea; }

    //-------- On Screen Qt widget overlays ---------

    /// Add an on-screen-display item. The position, size and transparency of
    /// the widget can be set as follows:
    /// \code
    ///	QWidget* pItem;
    ///	pItem->setPalette(Qt::transparent);
    ///	pItem->move(10, 10);
    ///	pItem->resize(100, 100);
    ///	pItem->setWindowOpacity(0.6);
    /// \endcode
    /// \param id		A unique id used to reference the item in other methods (input).
    /// \param pItem	Pointer to the OSD item. Ensure that this pointer remains valid
    ///				until removeOSDItem() is called.
    void addOSDItem(const QString& id, QWidget* pItem);

    /// remove a previously added on-screen-display item from the scene.
    /// \param id	The identifier specified for the widget during addOSDItem() (input).
    void removeOSDItem(const QString& id);

protected:
private:
    virtual void resizeEvent(QResizeEvent* pPainter);
    /// Handle window resize.

private:
    QGLWidget*						_pViewPort;
    SoQtGraphicsScene*				_pViewArea;	/// Rendering surface
    QMap<QString, QGraphicsItem*>	_osdItems;		/// OSD objects
}; // SoSceneViewer

//==============================================================================
void SoSceneViewer::setBackgroundColor(const QColor& color)
//==============================================================================
{
    _pViewArea->setBackgroundColor(SbColor4f(color.redF(), color.greenF(), color.blueF(), color.alphaF()));
}

} // Grape

#endif // GRAPE_SOSCENEVIEWER_H
