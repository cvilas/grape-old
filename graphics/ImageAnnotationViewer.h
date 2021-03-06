//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : ImageAnnotationViewer.h
// Brief    : Viewer widget for QImage overlayed with annotations
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

#ifndef GRAPE_IMAGEANNOTATIONVIEWER_H
#define GRAPE_IMAGEANNOTATIONVIEWER_H

#include "ImageAnnotator.h"
#include <QWidget>

class QMenu;

namespace grape
{


/// \class ImageAnnotationViewer
/// \ingroup graphics
/// \brief Interactive viewer for ImageAnnotator.
///
/// ImageAnnotationViewer provides a surface to display the image stream processed
/// by ImageAnnotator. The class also provides a user interactive interface to
/// re-position or modify annotations overlayed on the image.
///
/// Example program:
/// \include 2DGraphics/AnnotationViewerExample.cpp
class GRAPEGRAPHICS_DLL_API ImageAnnotationViewer : public QWidget
{
    Q_OBJECT
public:
    /// Constructor. Does a few initialisations. setImageAnnotator() must be called
    /// before using the object.
    /// \param pParent		Pointer to the parent window
    /// \param flgs		Window flags
    explicit ImageAnnotationViewer(QWidget* pParent = 0, Qt::WindowFlags flgs = 0);

    ~ImageAnnotationViewer();

    /// Associates the viewer with a specific annotator.
    /// \param pAnnotator	(input) The annotator object to associate the viewer with. Can be set to NULL.
    void setImageAnnotator(ImageAnnotator* pAnnotator);

    /// Allow users to interact with the viewer using the mouse.
    void setUserInteractive(bool option);

public slots:
    inline void scaleToFit(bool );

private slots:
    void onContextMenuRequest(const QPoint& pos);
    void onMenuAddText();
    void onMenuAddTimeStamp();
    void onMenuAddCounter();
    void onMenuAddCrosshair();
    void onMenuAddImage();
    void onMenuEditAnnotation( int, bool );
    void onMenuRemoveAnnotation( int, bool );
    void onMenuLockAnnotation( int, bool);
    void onAddAnnotation();
    void onRemoveAnnotation( int );
    void onAnnotated();
    inline void onRotationEnable( bool );

private:
    QMenu* createWindowContextMenu();
    QMenu* createAnnotationContextMenu(int index);
    inline QPointF mapWindowPosToInputImagePos(const QPointF& windowPos);
    void calculateScaleOffset();

    virtual void mousePressEvent( QMouseEvent* );
    virtual void mouseReleaseEvent( QMouseEvent*  );
    virtual void mouseMoveEvent( QMouseEvent* );
    virtual void mouseDoubleClickEvent( QMouseEvent* );
    virtual void keyPressEvent( QKeyEvent* );
    virtual void paintEvent( QPaintEvent* );
    virtual void resizeEvent( QResizeEvent* );

private:
    ImageAnnotator* _pAnnotator;

    ///< \cond (Doxygen - ignore section between cond and endcond)
    typedef struct AnnotationAttributes
    {
        bool		isLocked;	//!< whether the annotation is locked for motion or not.

        AnnotationAttributes() : isLocked(false)
        {
        }
    }AnnotationAttributes;
    ///< \endcond (Doxygen - ignore section between cond and endcond)

    QList<AnnotationAttributes> _attributes;

    int		_mouseOverIndex;		// index of annotation under mouse
    bool	_isUsrInteractive;

    QPointF	_prevPos;				// mouse motion tracking variable
    QPoint	_windowImageoffset;	// offset between the top-left corners of the image and the containing window
    bool    _isScaling;
    qreal	_scale;
}; // ImageAnnotationViewer

//------------------------------------------------------------------------------
void ImageAnnotationViewer::scaleToFit(bool option)
//------------------------------------------------------------------------------
{
    _isScaling = option;
    calculateScaleOffset();
    update();
}

//------------------------------------------------------------------------------
QPointF ImageAnnotationViewer::mapWindowPosToInputImagePos(const QPointF& windowPos)
//------------------------------------------------------------------------------
{
    QPointF pos = windowPos;
    if( _isScaling )
    {
        pos /= _scale;
    }
    pos -= _windowImageoffset;
    return pos;
}

//------------------------------------------------------------------------------
void ImageAnnotationViewer::onRotationEnable(bool option)
//------------------------------------------------------------------------------
{
    if( _pAnnotator )
    {
        _pAnnotator->enableImageRotation(option);
        update();
    }
}

} // grape


#endif // GRAPE_IMAGEANNOTATIONVIEWER_H
