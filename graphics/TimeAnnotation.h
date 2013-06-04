//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : TimeAnnotation.h
// Brief    : Time annotations on QImages
//==============================================================================

#ifndef GRAPE_TIMEANNOTATION_H
#define GRAPE_TIMEANNOTATION_H

#include "TextAnnotation.h"

namespace Grape
{

/// \class TimeAnnotation
/// \ingroup graphics
/// \brief Time-stamp annotation. Used with ImageAnnotator.
///
/// This is a specialised class used for placing time-stamps on a QImage. An
/// object of this class is used with ImageAnnotator.
///
/// This is a class derived from TextAnnotation. On every call
/// to ImageAnnotator::annotate(), an object of this class updates
/// TextAnnotation::text field with the current time stamp. (Therefore, setting
/// this field manually has not effect). The time-stamp format is set by the
/// TimeAnnotation::format field which follows the convention for the
/// format used by QDateTime::toString(QString). See documentation for QDateTime.
///
/// Example program:
/// See example for ImageAnnotator class
class GRAPEGRAPHICS_DLL_API TimeAnnotation : public TextAnnotation
{
    Q_OBJECT
public:
    QString format; //!< format string. See documentation for QDateTime::toString(QString). The default is "dd MMM yyyy - hh:mm:ss.zzz".
public:
    TimeAnnotation();
    virtual ~TimeAnnotation();
protected:
    void process(QPainter& painter);
}; // TimeAnnotation

} // Grape

#endif // GRAPE_TIMEANNOTATION_H
