//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : SoQtRenderArea.h
// Brief    : Surface for rendering Open-Inventor scenegraphs
//
// Note     :   This file is a derivative work of 'SoViewer' library available at
//              http://code.google.com/p/openinventorviewer/. A BSD-style license
//              applies. See License.txt
//==============================================================================

#ifndef GRAPE_SOQTRENDERAREA_H
#define GRAPE_SOQTRENDERAREA_H

#include "SoRenderArea.h"
#include <Inventor/nodes/SoSeparator.h>
#include <QGraphicsScene>
#include <QTime>

namespace Grape
{

/// \class SoQtRenderArea
/// \ingroup graphics
/// \brief Surface for rendering Open-Inventor scenegraphs.
///
/// This class provides a Qt OpenGL surface to render open-inventor scene graphs
/// and 2D Qt widgets, and forwards Qt events to Open-Inventor. This class is not
/// expected to be of interest to the application programmer; much of the good
/// stuff is in the derived class SoQtGraphicsScene. An object of this class
/// does not have a visual appearence of its own. The application programmer
/// should use the SoSceneViewer class to display the scene.
///
/// Example program:
/// See examples in /examples/Inventor
class GRAPEGRAPHICS_DLL_API SoQtRenderArea : public QGraphicsScene, public SoRenderArea
{
    Q_OBJECT
protected:
    explicit SoQtRenderArea( QObject* pParent = 0 );
    virtual ~SoQtRenderArea();

    virtual void resize( int width, int height );
    virtual void drawBackground(QPainter *pPainter, const QRectF &rect);
    virtual void soRenderCallback();
    virtual void timerEvent( QTimerEvent* pEvent );
    virtual void keyPressEvent( QKeyEvent* pEvent );
    virtual void keyReleaseEvent( QKeyEvent* pEvent );
    virtual void mouseMoveEvent( QGraphicsSceneMouseEvent* pEvent );
    virtual void mousePressEvent( QGraphicsSceneMouseEvent* pEvent );
    virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent* pEvent );
    virtual void wheelEvent( QGraphicsSceneWheelEvent* pEvent );
private:
    void commonInit();
    void saveGlState(); // Push all attributes to protect QPainter against any state changes made by GL specific calls
    void restoreGlState();
    SoKeyboardEvent::Key translateKey( QKeyEvent* pEvent );
private:
    QTime	_time;
    int		_timerSensorId;
    int		_delaySensorId;
}; // SoQtRenderArea

} // Grape

#endif // GRAPE_SOQTRENDERAREA_H
