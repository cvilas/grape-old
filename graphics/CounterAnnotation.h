//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : CounterAnnotation.h
// Brief    : Counter annotations on QImages
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

#ifndef GRAPE_COUNTERANNOTATION_H
#define GRAPE_COUNTERANNOTATION_H

#include "TextAnnotation.h"

namespace grape
{

/// \class CounterAnnotation
/// \ingroup graphics
/// \brief Counter annotation. Used with ImageAnnotator.
///
/// This is a specialised class used for placing a counter on a QImage. An
/// object of this class is used with ImageAnnotator.
///
/// The CounterAnnotation class is derived from TextAnnotation.
/// On every call to ImageAnnotator::annotate(), an object of this class updates
/// the TextAnnotation::text field to 'count' and increments count by 'step'
/// (Therefore, setting TextAnnotation::text field manually has not effect).
///
/// Example program:
/// See example for ImageAnnotator class
class GRAPEGRAPHICS_DLL_API CounterAnnotation : public TextAnnotation
{
    Q_OBJECT
public:
    ISignalerFieldT<int> count;	//!< Counter value. A call to ImageAnnotator::annotate increments this value by 'step'.
    int step;					//!< Increment step. Every call to CImageAnnotator::annotate increments 'value' by this amount.
    int fieldWidth;				//!< Minimum amount of padding space to be filled with fillChar. Default is 0. A positive value produces right-aligned text, negative value produces left aligned text.
    QChar fillChar;				//!< fill character for padding. Default is ' '.
public:
    CounterAnnotation(QObject *parent = 0);
    virtual ~CounterAnnotation();
protected:
    void process(QPainter& painter);
}; // CounterAnnotation

} // grape

#endif // GRAPE_COUNTERANNOTATION_H
