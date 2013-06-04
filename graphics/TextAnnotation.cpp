//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : TextAnnotation.cpp
//==============================================================================

#include "TextAnnotation.h"
#include <QPainter>
#include <QApplication>

namespace Grape
{

//==============================================================================
TextAnnotation::TextAnnotation(QObject *parent)
//==============================================================================
: Annotation(parent), font(QApplication::font())
{
}

//------------------------------------------------------------------------------
TextAnnotation::~TextAnnotation()
//------------------------------------------------------------------------------
{
}

//------------------------------------------------------------------------------
void TextAnnotation::process(QPainter& painter)
//------------------------------------------------------------------------------
{
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.setPen(pen);
    painter.setFont(font);
    painter.drawText(boundingBox.value(), alignFlags, text, &bestBoundingBox);
}

} // Grape
