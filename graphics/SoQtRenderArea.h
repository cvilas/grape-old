//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : SoQtRenderArea.h
// Brief    : Surface for rendering Open-Inventor scenegraphs
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
