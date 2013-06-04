//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : CrosshairAnnotation.h
// Brief    : Crosshair annotations on QImages
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
    CrosshairAnnotation();
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
