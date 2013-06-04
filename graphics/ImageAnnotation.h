//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : ImageAnnotation.h
// Brief    : Overlay image annotations on QImages
//==============================================================================

#ifndef GRAPE_IMAGEANNOTATION_H
#define GRAPE_IMAGEANNOTATION_H

#include "Annotation.h"
#include <QImage>

namespace Grape
{

/// \class ImageAnnotation
/// \ingroup graphics
/// \brief Image annotation object. Used with ImageAnnotator.
///
/// This is a specialised class used for placing image annotations on a QImage. An
/// object of this class is used with ImageAnnotator.
///
/// Example program:
/// See example for ImageAnnotator class
class GRAPEGRAPHICS_DLL_API ImageAnnotation : public Annotation
{
    Q_OBJECT
public:
    // ----- user set fields -----
    QImage	image;				//!< The annotation image.

public:
    ImageAnnotation(QObject *parent = 0);
    virtual ~ImageAnnotation();
protected:
    void process(QPainter& painter);
}; // ImageAnnotation

} // Grape

#endif // GRAPE_IMAGEANNOTATION_H
