//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : TextAnnotation.h
// Brief    : Textual annotations on QImages
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

#ifndef GRAPE_TEXTANNOTATION_H
#define GRAPE_TEXTANNOTATION_H

#include "Annotation.h"
#include <QFont>
#include <QPen>

namespace grape
{

/// \class TextAnnotation
/// \ingroup graphics
/// \brief Text annotation. Used with ImageAnnotator.
///
/// This is a specialised class used for placing text annotations on a QImage. An
/// object of this class is used with ImageAnnotator.
///
/// Example program:
/// See example for ImageAnnotator class
class GRAPEGRAPHICS_DLL_API TextAnnotation : public Annotation
{
    Q_OBJECT
public:
    // ----- auto updated fields
    QRectF	bestBoundingBox;	//!< After the annotation has been set on the image, this
                                //!< field is set to what the bounding rectangle should be
                                //!< in order to enclose the whole annotation.

    // ----- user set fields -----
    QFont	font;				//!< Font settings
    QPen	pen;				//!< Pen settings
    QString	text;				//!< The annotation text
    int		alignFlags;			//!< Text alignment flags. This is a bitwise OR of the
                                //!< following flags	\n
                                //!< Qt::AlignLeft		\n
                                //!< Qt::AlignRight		\n
                                //!< Qt::AlignHCenter	\n
                                //!< Qt::AlignTop		\n
                                //!< Qt::AlignBottom	\n
                                //!< Qt::AlignVCenter	\n
                                //!< Qt::AlignCenter	\n
                                //!< Qt::TextSingleLine	\n
                                //!< Qt::TextExpandTabs	\n
                                //!< Qt::TextShowMnemonic	\n
                                //!< Qt::TextWordWrap		\n
                                //!< Qt::TextIncludeTrailingSpaces
public:
    TextAnnotation(QObject *parent = 0);
    virtual ~TextAnnotation();
protected:
    void process(QPainter& painter);

}; // TextAnnotation

} // grape

#endif // GRAPE_TEXTANNOTATION_H
