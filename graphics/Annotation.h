//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : Annotation.h
// Brief    : Base class for annotations on QImages
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

#ifndef GRAPE_ANNOTATION_H
#define GRAPE_ANNOTATION_H

#include "ISignalerField.h"
#include <QString>
#include <QRect>

class QPainter;

namespace Grape
{

/// \class Annotation
/// \ingroup graphics
/// \brief Base class for annotations on QImages. Used with ImageAnnotator.
///
/// ImageAnnotation defines an annotation on a QImage object. Annotations
/// are overlays embedded on the image, and could be a text or a graphic. An
/// object of this class is used with ImageAnnotator. Derived classes implement
/// the process() method containing the specific painting instructions.
///
/// Example program:
/// See example for ImageAnnotator class
class GRAPEGRAPHICS_DLL_API Annotation : public QObject
{
    Q_OBJECT
    friend class ImageAnnotator;

public:

    // ------- user set fields --------
    ISignalerFieldT<QString> name;          //!< unique identifier name
    ISignalerFieldT<QRectF> boundingBox;    //!< annotation bounding box

protected:

    /// This method is called by ImageAnnotator. A derived class implements
    /// painting of the annotation on to the image
    virtual void process(QPainter& painter) = 0;

    explicit Annotation(QObject *parent) : QObject(parent) {}
    virtual ~Annotation() {}
}; // Annotation

} // Grape

#endif // GRAPE_ANNOTATION_H
