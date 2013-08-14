//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : CrosshairAnnotation.h
// Brief    : Crosshair annotations on QImages
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

#ifndef GRAPE_CROSSHAIRANNOTATION_H
#define GRAPE_CROSSHAIRANNOTATION_H

#include "Annotation.h"
#include <QPen>

namespace Grape
{

/// \class CrosshairAnnotation
/// \ingroup graphics
/// \brief Crosshair annotation object. Used with ImageAnnotator.
///
/// This is a specialised class used for placing a crosshair annotations on a QImage.
/// The crosshair is placed at the center of the specified bounding box.
///
/// Example program:
/// See example for ImageAnnotator class
class GRAPEGRAPHICS_DLL_API CrosshairAnnotation : public Annotation
{
    Q_OBJECT
public:
    // ----- user set fields -----
    ISignalerFieldT<int>	thickness;	//!< The thickness of lines in pixels

public:
    CrosshairAnnotation(QObject *parent = 0);
    virtual ~CrosshairAnnotation();

private:
    void process(QPainter& painter);

private slots:
    void updatePosition();
    void updateThickness() { _pen.setWidth(thickness.value()); }
private:
    QPen	_pen;
    QLine*	_pLines;
}; // CrosshairAnnotation


} // Grape

#endif // GRAPE_CROSSHAIRANNOTATION_H
