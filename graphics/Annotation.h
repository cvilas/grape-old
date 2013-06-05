//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : Annotation.h
// Brief    : Base class for annotations on QImages
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
