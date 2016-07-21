//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : ImageAnnotationViewer.cpp
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

#include "ImageAnnotationViewer.h"
#include "ImageAnnotationViewerP.h"
#include <QPainter>
#include <QMouseEvent>
#include <math.h>
#include <QAction>
#include <QMenu>

#ifndef M_PI
#define M_PI 3.141592654
#endif

namespace grape
{

//==============================================================================
ImageAnnotationViewer::ImageAnnotationViewer(QWidget* pParent, Qt::WindowFlags f)
//==============================================================================
: QWidget(pParent, f), _pAnnotator(0), _mouseOverIndex(-1), _isScaling(false)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    setMouseTracking(true);
    setUserInteractive(true);
}

//-------------------------------------------------------------------------------
ImageAnnotationViewer::~ImageAnnotationViewer()
//-------------------------------------------------------------------------------
{
}

//-------------------------------------------------------------------------------
void ImageAnnotationViewer::setUserInteractive(bool option)
//-------------------------------------------------------------------------------
{
    _isUsrInteractive = option;
    if( option )
    {
        QObject::connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onContextMenuRequest(const QPoint&)));
    }
    else
    {
        QObject::disconnect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onContextMenuRequest(const QPoint&)));
    }
}


//-------------------------------------------------------------------------------
void ImageAnnotationViewer::setImageAnnotator(ImageAnnotator* pAnnotator)
//-------------------------------------------------------------------------------
{
    if( _pAnnotator )
    {
        QObject::disconnect(_pAnnotator, SIGNAL(annotated()), this, SLOT(onAnnotated()));
        QObject::disconnect(_pAnnotator, SIGNAL(annotationAdded()), this, SLOT(onAddAnnotation()));
        QObject::disconnect(_pAnnotator, SIGNAL(annotationRemoved(int)), this, SLOT(onRemoveAnnotation(int)));
    }
    _attributes.clear();

    _pAnnotator = pAnnotator;

    if( !_pAnnotator )
    {
        return;
    }

    // Creates a second list to contain attributes of annotations contained in the
    // list managed by CImageAnnotator.

    int numAnnots = _pAnnotator->getNumAnnotations();
    for(int i = 0; i < numAnnots; ++i )
    {
        onAddAnnotation();
    }
    QObject::connect(_pAnnotator, SIGNAL(annotationAdded()), this, SLOT(onAddAnnotation()));
    QObject::connect(_pAnnotator, SIGNAL(annotationRemoved(int)), this, SLOT(onRemoveAnnotation(int)));
    QObject::connect(_pAnnotator, SIGNAL(annotated()), this, SLOT(onAnnotated()));
}

//-------------------------------------------------------------------------------
void ImageAnnotationViewer::onAddAnnotation()
//-------------------------------------------------------------------------------
{
    AnnotationAttributes attr;
    attr.isLocked = false;
    _attributes.append(attr);
}

//-------------------------------------------------------------------------------
void ImageAnnotationViewer::onRemoveAnnotation( int index )
//-------------------------------------------------------------------------------
{
    if( (index >= 0) && (index < _attributes.size()))
    {
        _attributes.removeAt(index);
    }
}

//-------------------------------------------------------------------------------
void ImageAnnotationViewer::onAnnotated()
//-------------------------------------------------------------------------------
{
    update();
}

//-------------------------------------------------------------------------------
QMenu* ImageAnnotationViewer::createWindowContextMenu()
//-------------------------------------------------------------------------------
{
    QMenu* pContextMenu = new QMenu(this);

    QMenu* pAddMenu = new QMenu;
    pAddMenu->addAction("Text", this, SLOT(onMenuAddText()));
    pAddMenu->addAction("Time Stamp", this, SLOT(onMenuAddTimeStamp()));
    pAddMenu->addAction("Counter", this, SLOT(onMenuAddCounter()));
    pAddMenu->addAction("Cross Hair", this, SLOT(onMenuAddCrosshair()));
    pAddMenu->addAction("Image", this, SLOT(onMenuAddImage()));

    QAction* pAction = pContextMenu->addAction("Add Annotation");
    pAction->setMenu(pAddMenu);

    pAction = pContextMenu->addAction("Scale To Fit");
    pAction->setCheckable(true);
    pAction->setChecked(_isScaling);
    QObject::connect(pAction, SIGNAL(triggered(bool)), this, SLOT(scaleToFit(bool)));

    pAction = pContextMenu->addAction("Enable Rotation");
    pAction->setCheckable(true);
    pAction->setChecked(_pAnnotator->isImageRotationEnabled());
    QObject::connect(pAction, SIGNAL(triggered(bool)), this, SLOT(onRotationEnable(bool)));

    return pContextMenu;
}

//-------------------------------------------------------------------------------
QMenu* ImageAnnotationViewer::createAnnotationContextMenu(int index)
//-------------------------------------------------------------------------------
{
    QMenu* pMenu = new QMenu(this);

    QIdAction* pAction = new QIdAction(index, "Edit", 0);
    pMenu->addAction(pAction);
    QObject::connect(pAction, SIGNAL(triggered(int, bool)), this, SLOT(onMenuEditAnnotation(int, bool)));

    pAction = new QIdAction(index, "Remove", 0);
    pMenu->addAction(pAction);
    QObject::connect(pAction, SIGNAL(triggered(int, bool)), this, SLOT(onMenuRemoveAnnotation(int, bool)));

    pAction = new QIdAction(index, "Lock Position", 0);
    pAction->setCheckable(true);
    if( (index >= 0) && (index < _attributes.size()))
    {
        pAction->setChecked(_attributes[index].isLocked);
    }
    pMenu->addAction(pAction);
    QObject::connect(pAction, SIGNAL(triggered(int, bool)), this, SLOT(onMenuLockAnnotation(int, bool)));

    return pMenu;
}


//-------------------------------------------------------------------------------
void ImageAnnotationViewer::onContextMenuRequest(const QPoint& pos)
//-------------------------------------------------------------------------------
{
    if( !_pAnnotator )
    {
        return;
    }

    // show annotation specific menu if annotation exists here, else
    // show generic menu

    int ind = 0;
    Annotation* pAnnot = _pAnnotator->getAnnotation(mapWindowPosToInputImagePos(pos), ind);
    QMenu* pMenu = 0;
    if( pAnnot )
    {
        pMenu = createAnnotationContextMenu(ind);
    }
    else
    {
        pMenu = createWindowContextMenu();
    }
    pMenu->exec(mapToGlobal(pos));
    delete pMenu;
}

//-------------------------------------------------------------------------------
void ImageAnnotationViewer::onMenuAddText()
//-------------------------------------------------------------------------------
{
    if( !_pAnnotator )
    {
        return;
    }

    TextAnnotation* pAnnot = new TextAnnotation;
    QPointF p = mapWindowPosToInputImagePos(_prevPos);
    pAnnot->text = "Edit this text";
    pAnnot->name = QString("Text (%1, %2)").arg(p.x()).arg(p.y());
    pAnnot->font.setPointSize(15);
    pAnnot->font.setFamily("Arial");
    pAnnot->pen.setColor(Qt::black);
    pAnnot->alignFlags = Qt::AlignCenter | Qt::TextWordWrap;
    pAnnot->boundingBox = QRectF(p.x(), p.y(), _pAnnotator->getOutputImage().width() - p.x(), 10);
    _pAnnotator->addAnnotation(pAnnot, true);
    QDialog* pDialog = new TextAnnotationEditUi(pAnnot, this);
    pDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    pDialog->show();
}

//-------------------------------------------------------------------------------
void ImageAnnotationViewer::onMenuAddTimeStamp()
//-------------------------------------------------------------------------------
{
    if( !_pAnnotator )
    {
        return;
    }

    TimeAnnotation* pAnnot = new TimeAnnotation;
    QPointF p = mapWindowPosToInputImagePos(_prevPos);
    pAnnot->name = QString("Time (%1, %2)").arg(p.x()).arg(p.y());
    //pAnnot->format = "dd MMM yyyy hh:mm:ss.zzz";
    pAnnot->font.setPointSize(15);
    pAnnot->font.setFamily("Arial");
    pAnnot->pen.setColor(Qt::black);
    pAnnot->alignFlags = Qt::AlignLeft | Qt::TextWordWrap;
    pAnnot->boundingBox = QRectF(p.x(), p.y(), _pAnnotator->getOutputImage().width() - p.x(), 10);
    _pAnnotator->addAnnotation(pAnnot, true);
    QDialog* pDialog = new TextAnnotationEditUi(pAnnot, this);
    pDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    pDialog->show();
}

//-------------------------------------------------------------------------------
void ImageAnnotationViewer::onMenuAddCounter()
//-------------------------------------------------------------------------------
{
    if( !_pAnnotator )
    {
        return;
    }

    CounterAnnotation* pAnnot = new CounterAnnotation;
    QPointF p = mapWindowPosToInputImagePos(_prevPos);
    pAnnot->name = QString("Counter (%1, %2)").arg(p.x()).arg(p.y());
    pAnnot->count = 0;
    pAnnot->step = 1;
    pAnnot->fieldWidth = 6;
    pAnnot->fillChar = QChar(' ');
    pAnnot->font.setPointSize(15);
    pAnnot->font.setFamily("Arial");
    pAnnot->pen.setColor(Qt::black);
    pAnnot->alignFlags = Qt::AlignRight;
    pAnnot->boundingBox = QRectF(p.x(), p.y(), _pAnnotator->getOutputImage().width() - p.x(), 10);
    _pAnnotator->addAnnotation(pAnnot, true);
    QDialog* pDialog = new TextAnnotationEditUi(pAnnot, this);
    pDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    pDialog->show();
}

//-------------------------------------------------------------------------------
void ImageAnnotationViewer::onMenuAddCrosshair()
//-------------------------------------------------------------------------------
{
    if( !_pAnnotator )
    {
        return;
    }

    CrosshairAnnotation* pAnnot = new CrosshairAnnotation;
    QPointF p = mapWindowPosToInputImagePos(_prevPos);
    pAnnot->name = QString("Crosshair (%1, %2)").arg(p.x()).arg(p.y());
    pAnnot->thickness = 1;
    pAnnot->boundingBox = QRectF(p.x() - 25, p.y() - 25, 51, 51);
    _pAnnotator->addAnnotation(pAnnot, true);
    QDialog* pDialog = new CrosshairAnnotationEditUi(pAnnot, _pAnnotator->getOutputImage().size(), this);
    pDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    pDialog->show();
}

//-------------------------------------------------------------------------------
void ImageAnnotationViewer::onMenuAddImage()
//-------------------------------------------------------------------------------
{
    if( !_pAnnotator )
    {
        return;
    }

    ImageAnnotation* pAnnot = new ImageAnnotation;
    QPointF p = mapWindowPosToInputImagePos(_prevPos);
    pAnnot->name = QString("Image (%1, %2)").arg(p.x()).arg(p.y());
    pAnnot->image = QImage(":images/ocr200_t.png");
    pAnnot->boundingBox = QRectF(p.x(), p.y(), 200, 100);
    _pAnnotator->addAnnotation(pAnnot, true);
    QDialog* pDialog = new ImageAnnotationSelectUi(pAnnot, this);
    pDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    pDialog->show();
}

//-------------------------------------------------------------------------------
void ImageAnnotationViewer::onMenuEditAnnotation(int index, bool)
//-------------------------------------------------------------------------------
{
    if( !_pAnnotator )
    {
        return;
    }

    Annotation* pAnnot = _pAnnotator->getAnnotation(index);
    if( !pAnnot )
    {
        return;
    }

    QString annotationType = QString(pAnnot->metaObject()->className());
    QDialog* pDialog = NULL;
    if( annotationType == "Grape::CrosshairAnnotation" )
    {
        pDialog = new CrosshairAnnotationEditUi(dynamic_cast<CrosshairAnnotation*>(pAnnot),
            _pAnnotator->getOutputImage().size(),
            this);
    }
    else if( annotationType == "Grape::ImageAnnotation" )
    {
        pDialog = new ImageAnnotationEditUi(dynamic_cast<ImageAnnotation*>(pAnnot), this);
    }
    else if( annotationType == "Grape::TextAnnotation" )
    {
        pDialog = new TextAnnotationEditUi(dynamic_cast<TextAnnotation*>(pAnnot), this);
    }
    else if( annotationType == "Grape::TimeAnnotation" )
    {
        pDialog = new TextAnnotationEditUi(dynamic_cast<TimeAnnotation*>(pAnnot), this);
    }
    else if( annotationType == "Grape::CounterAnnotation" )
    {
        pDialog = new TextAnnotationEditUi(dynamic_cast<CounterAnnotation*>(pAnnot), this);
    }

    if( pDialog )
    {
        pDialog->setAttribute(Qt::WA_DeleteOnClose, true);
        pDialog->show();
    }
}

//-------------------------------------------------------------------------------
void ImageAnnotationViewer::onMenuRemoveAnnotation(int index, bool)
//-------------------------------------------------------------------------------
{
    if( !_pAnnotator )
    {
        return;
    }
    _pAnnotator->removeAnnotation(index);
    if( index == _mouseOverIndex )
    {
        _mouseOverIndex = -1;
    }
}

//-------------------------------------------------------------------------------
void ImageAnnotationViewer::onMenuLockAnnotation(int index, bool checked)
//-------------------------------------------------------------------------------
{
    if( (index >= 0) && (index < _attributes.size()))
    {
        _attributes[index].isLocked = checked;
    }
}

//-------------------------------------------------------------------------------
void ImageAnnotationViewer::mousePressEvent( QMouseEvent* pEvent )
//-------------------------------------------------------------------------------
{
    if( !_isUsrInteractive )
    {
        return pEvent->ignore();
    }
    _prevPos = pEvent->pos();
}

//-------------------------------------------------------------------------------
void ImageAnnotationViewer::mouseReleaseEvent( QMouseEvent* pEvent )
//-------------------------------------------------------------------------------
{
    if( !_isUsrInteractive )
    {
        return pEvent->ignore();
    }
}

//-------------------------------------------------------------------------------
void ImageAnnotationViewer::mouseMoveEvent( QMouseEvent* pEvent)
//-------------------------------------------------------------------------------
{
    if( !_isUsrInteractive )
    {
        return pEvent->ignore();
    }
    if( !_pAnnotator )
    {
        return;
    }

    QPointF pos = pEvent->pos();

    // if left button active, move the current annotation around or rotate image
    if( pEvent->buttons() & Qt::LeftButton )
    {
        // if annotation is active, move it
        if( (_mouseOverIndex >= 0) && (_mouseOverIndex < _attributes.size()) )
        {
            AnnotationAttributes &attr = _attributes[_mouseOverIndex];
            if( !attr.isLocked )
            {
                Annotation* pAnnot = _pAnnotator->getAnnotation(_mouseOverIndex); // find corresponding annotation
                QRectF bb = pAnnot->boundingBox.value();
                QPointF d = pos - _prevPos;
                if ( _isScaling )
                {
                    d /= _scale;
                }
                bb.translate(d); // apply mouse movement
                pAnnot->boundingBox = bb;
            }
        }

        // otherwise rotate the image
        else
        {
            if( _pAnnotator->isImageRotationEnabled() )
            {
                QPointF d;
                int w2 = width()/2;
                int h2 = height()/2;
                d.setX(pos.x() - w2);
                d.setY(pos.y() - h2);
                float deg = _pAnnotator->getImageRotationAngle() + (180.0/M_PI) * atan2(d.y(),d.x());
                d.setX(_prevPos.x() - w2);
                d.setY(_prevPos.y() - h2);
                deg -= (180.0/M_PI) * atan2(d.y(), d.x());
                _pAnnotator->setImageRotationAngle(deg);
            }
        }
    }

    // select the annotation under the mouse
    else
    {
        _pAnnotator->getAnnotation(mapWindowPosToInputImagePos(pos), _mouseOverIndex);
    }

    _prevPos = pos;
    update();

}

//-------------------------------------------------------------------------------
void ImageAnnotationViewer::mouseDoubleClickEvent( QMouseEvent* pEvent )
//-------------------------------------------------------------------------------
{
    if( !_isUsrInteractive )
    {
        return pEvent->ignore();
    }

    if( !_pAnnotator )
    {
        return;
    }

    int index = -1;
    _pAnnotator->getAnnotation(mapWindowPosToInputImagePos(pEvent->pos()), index);
    onMenuEditAnnotation(index, false);
}

//-------------------------------------------------------------------------------
void ImageAnnotationViewer::keyPressEvent( QKeyEvent* pEvent )
//-------------------------------------------------------------------------------
{
    if( !_isUsrInteractive )
    {
        return pEvent->ignore();
    }

    if( !_pAnnotator )
    {
        return;
    }

    switch( pEvent->key() )
    {
    case Qt::Key_Enter :
    case Qt::Key_Return:
        onMenuEditAnnotation(_mouseOverIndex, false);
        break;
    case Qt::Key_Delete :
        onMenuRemoveAnnotation(_mouseOverIndex, false);
        break;
    default:
        break;
    };
}

//-------------------------------------------------------------------------------
void ImageAnnotationViewer::paintEvent( QPaintEvent* )
//-------------------------------------------------------------------------------
{
    if( !_pAnnotator )
    {
        return;
    }

    const QImage& image = _pAnnotator->getOutputImage();

    QPainter painter(this);
    if( _isScaling )
    {
        painter.setRenderHints(QPainter::SmoothPixmapTransform|QPainter::Antialiasing, true);
        painter.scale(_scale, _scale);
    }
    else
    {
        painter.setRenderHints(QPainter::SmoothPixmapTransform|QPainter::Antialiasing, false);
    }
    painter.drawImage(_windowImageoffset, image);

    Annotation* pAnnot = _pAnnotator->getAnnotation(_mouseOverIndex);
    if( !pAnnot )
    {
        return;
    }

    QPen pen(Qt::white);
    pen.setWidth(1);
    painter.setPen(pen);
    painter.setBrush(QColor(127, 127, 127, 50));
    QRectF bb = pAnnot->boundingBox.value();
    bb.translate( _windowImageoffset ); // apply mouse movement
    painter.drawRoundedRect( bb, 5, 5);
}

//-------------------------------------------------------------------------------
void ImageAnnotationViewer::resizeEvent( QResizeEvent* )
//-------------------------------------------------------------------------------
{
    calculateScaleOffset();
}

//-------------------------------------------------------------------------------
void ImageAnnotationViewer::calculateScaleOffset()
//-------------------------------------------------------------------------------
{
    if( !_pAnnotator )
    {
        return;
    }

    const QImage& image = _pAnnotator->getOutputImage();

    qreal imgW = image.width();
    qreal imgH = image.height();
    qreal winW = width();
    qreal winH = height();

    ( (imgW/imgH) > (winW/winH) ) ? (_scale = winW/imgW) : (_scale = winH/imgH);

    if( _isScaling )
    {
        _windowImageoffset = QPoint((width() - (_scale * image.width()))/(2*_scale), (height()- (_scale * image.height()))/(2*_scale));
    }
    else
    {
        _windowImageoffset = QPoint((width() - image.width())/2, (height()- image.height())/2);
    }
}

} // grape
