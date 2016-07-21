//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : ImageAnnotator.cpp
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

#include "ImageAnnotator.h"
#include <QPainter>

namespace grape
{

//==============================================================================
ImageAnnotator::ImageAnnotator(QObject *parent)
//==============================================================================
: QObject(parent),
  _isRotating(false),
  _angle(0),
  _pPainter(0),
  _fillBkgnd(true),
  _fillColor(0)
{
    alwaysClearBeforeUpdate(true);
}

//------------------------------------------------------------------------------
ImageAnnotator::~ImageAnnotator()
//------------------------------------------------------------------------------
{
    while( _annotations.length() )
    {
        AnnotationProperties prop = _annotations.takeLast();
        if( prop.deleteOnRemove )
        {
            delete prop.pAnnotation;
        }
    }
    if( _pPainter )
    {
        delete _pPainter;
    }
}

//------------------------------------------------------------------------------
void ImageAnnotator::setOutputImageSize(const QSize &size, QImage::Format fmt)
//------------------------------------------------------------------------------
{
    if( size.isEmpty() )
    {
        return;
    }

    bool antiAlias = false;
    bool smooth = false;
    if( _pPainter )
    {
        antiAlias = getAntiAliasing();
        smooth = getSmoothing();
        delete _pPainter;
    }
    _output = QImage(size, fmt);
    _workBuf = QImage(size, fmt);
    _pPainter = new QPainter(&_workBuf);
    setAntiAliasing(antiAlias);
    setSmoothing(smooth);
}

//------------------------------------------------------------------------------
void ImageAnnotator::addAnnotation(Annotation* pAnnotation, bool deleteOnRemove)
//------------------------------------------------------------------------------
{
    AnnotationProperties prop;
    prop.pAnnotation = pAnnotation;
    prop.deleteOnRemove = deleteOnRemove;
    _annotations.append(prop);
    emit annotationAdded();
    QObject::connect( &pAnnotation->boundingBox, SIGNAL(modified()), this, SLOT(onAnnotationMoved()));
}

//------------------------------------------------------------------------------
void ImageAnnotator::onAnnotationMoved()
//------------------------------------------------------------------------------
{
    _fillBkgnd = true;
}

//------------------------------------------------------------------------------
Annotation* ImageAnnotator::getAnnotation(const QPointF& pos, int &index)
//------------------------------------------------------------------------------
{
    index = 0;

    // check for intersection
    QList<AnnotationProperties>::const_iterator itEnd = _annotations.end();
    QList<AnnotationProperties>::const_iterator it = _annotations.begin();
    while( it != itEnd )
    {
        const QRectF& rectBox = it->pAnnotation->boundingBox.value();
        if( rectBox.contains(pos) )
        {
            return it->pAnnotation;
        }
        ++it;
        ++index;
    }
    index = -1;
    return NULL;
}

//------------------------------------------------------------------------------
int ImageAnnotator::getAnnotationIndex(const QString& name)
//------------------------------------------------------------------------------
{
    int j = 0;
    QList<AnnotationProperties>::const_iterator itEnd = _annotations.end();
    QList<AnnotationProperties>::const_iterator it = _annotations.begin();
    while( it != itEnd )
    {
        if( it->pAnnotation->name.value() == name )
        {
            return j;
        }
        ++j;
        ++it;
    }
    return -1;
}

//------------------------------------------------------------------------------
bool ImageAnnotator::removeAnnotation( int index )
//------------------------------------------------------------------------------
{
    if( (index < 0) || (index > (_annotations.size() - 1)) )
    {
        return false;
    }

    AnnotationProperties prop = _annotations.takeAt(index);
    QObject::disconnect( &(prop.pAnnotation->boundingBox), SIGNAL(modified()), this, SLOT(onAnnotationMoved()));
    if( prop.deleteOnRemove )
    {
        delete prop.pAnnotation;
    }
    emit annotationRemoved(index);
    return true;
}

//------------------------------------------------------------------------------
bool ImageAnnotator::removeAnnotation(const QString& name )
//------------------------------------------------------------------------------
{
    return( removeAnnotation(getAnnotationIndex(name)) );
}

//------------------------------------------------------------------------------
QImage& ImageAnnotator::annotate(const QImage &image, enum ScaleMode mode)
//------------------------------------------------------------------------------
{
    // Logic: Draws to internal work buffer first, then blits to output image in
    // one shot at the end.

    if( !_pPainter )
    {
        return _output;
    }

    QRect sourceRect = image.rect();
    QRect outputRect;

    switch(mode)
    {
    case NoResize:
        outputRect = sourceRect;
        break;
    case NoResizeCentered:
        outputRect.setTopLeft( QPoint((_workBuf.width() - sourceRect.width())/2, (_workBuf.height() - sourceRect.height())/2) );
        outputRect.setSize(sourceRect.size());
        break;
    case ResizeToAspect:
    {
        float aspect = (float)sourceRect.width()/(float)sourceRect.height();
        int oh = _workBuf.height();
        int ow = _workBuf.width();
        if( aspect > 1 )
        {
            int h = ow/aspect;
            outputRect.setRect(0, (oh - h)/2, ow, h );
        }
        else
        {
            int w = oh * aspect;
            outputRect.setRect( (ow - w)/2, 0, w, oh );
        }
        break;
    }
    case ResizeToFill:
        outputRect = _workBuf.rect();
        break;
    default:
        break;
    };

    if( _fillBkgnd || _fillBeforeUpdate )
    {
        _workBuf.fill(_fillColor);
        _fillBkgnd = false;
    }

    // copy and scale the input image
    _pPainter->setCompositionMode(QPainter::CompositionMode_SourceOver);
    _pPainter->save();
    if( _isRotating )
    {
        int w2 = _output.width()/2;
        int h2 = _output.height()/2;
        _pPainter->translate(w2, h2);
        _pPainter->rotate(_angle);
        _pPainter->translate(-w2, -h2);
    }
    _pPainter->drawImage(outputRect, image, sourceRect);
    _pPainter->restore();


    //process the overlays
    const int numAnnots = _annotations.size();
    for(int i = 0; i < numAnnots; ++i )
    {
        _pPainter->save();							// save painter state
        _annotations.at(i).pAnnotation->process(*_pPainter);	// paint
        _pPainter->restore();						// restore painter state
    }

    // bit blit
    QPainter painter(&_output);
    painter.drawImage(_output.rect(), _workBuf, _workBuf.rect());

    emit annotated();
    return _output;
}

//------------------------------------------------------------------------------
void ImageAnnotator::setAntiAliasing(bool option)
//------------------------------------------------------------------------------
{
    if( _pPainter )
    {
        _pPainter->setRenderHint(QPainter::Antialiasing, option);
        _pPainter->setRenderHint(QPainter::TextAntialiasing, option);
    }
}

//------------------------------------------------------------------------------
bool ImageAnnotator::getAntiAliasing()
//------------------------------------------------------------------------------
{
    if( !_pPainter )
    {
        return false;
    }
    return ( _pPainter->renderHints() & QPainter::Antialiasing );
}

//------------------------------------------------------------------------------
void ImageAnnotator::setSmoothing(bool option)
//------------------------------------------------------------------------------
{
    if( _pPainter )
    {
        _pPainter->setRenderHint(QPainter::SmoothPixmapTransform, option);
    }
}

//------------------------------------------------------------------------------
bool ImageAnnotator::getSmoothing()
//------------------------------------------------------------------------------
{
    if( !_pPainter )
    {
        return false;
    }
    return ( _pPainter->renderHints() & QPainter::SmoothPixmapTransform );
}


} // grape
