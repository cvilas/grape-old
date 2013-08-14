//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : CrosshairAnnotation.cpp
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

#include "CrosshairAnnotation.h"
#include <QPainter>

namespace Grape
{

//==============================================================================
CrosshairAnnotation::CrosshairAnnotation(QObject *parent)
//==============================================================================
: Annotation(parent)
{
    _pLines = new QLine [2];
    QObject::connect(&boundingBox, SIGNAL(modified()), this, SLOT(updatePosition()));
    QObject::connect(&thickness, SIGNAL(modified()), this, SLOT(updateThickness()));
    _pen.setColor(Qt::white);
    boundingBox = QRectF(0, 0, 10, 10);
    thickness = 1;
}

//------------------------------------------------------------------------------
CrosshairAnnotation::~CrosshairAnnotation()
//------------------------------------------------------------------------------
{
    delete [] _pLines;
}

//------------------------------------------------------------------------------
void CrosshairAnnotation::updatePosition()
//------------------------------------------------------------------------------
{
    QRectF bb = boundingBox.value();
    const int &x = bb.x();
    const int &y = bb.y();
    const int &w = bb.width();
    const int &h = bb.height();
    _pLines[0].setLine( x, y + h/2, x + w, y + h/2 );
    _pLines[1].setLine( x + w/2, y, x + w/2, y + h );
}

//------------------------------------------------------------------------------
void CrosshairAnnotation::process(QPainter& painter)
//------------------------------------------------------------------------------
{
    painter.setCompositionMode(QPainter::CompositionMode_Exclusion);
    painter.setPen(_pen);
    painter.drawLines(_pLines, 2);
}

} // Grape
