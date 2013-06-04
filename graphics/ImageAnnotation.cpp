//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : ImageAnnotation.cpp
//==============================================================================

#include "ImageAnnotation.h"
#include <QPainter>

namespace Grape
{

//==============================================================================
ImageAnnotation::ImageAnnotation()
//==============================================================================
: Annotation()
{
}

//------------------------------------------------------------------------------
ImageAnnotation::~ImageAnnotation()
//------------------------------------------------------------------------------
{
}

//------------------------------------------------------------------------------
void ImageAnnotation::process(QPainter& painter)
//------------------------------------------------------------------------------
{
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(boundingBox.value(), image, image.rect());
}

} // Grape
