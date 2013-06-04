//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : TextAnnotation.h
// Brief    : Textual annotations on QImages
//==============================================================================

#ifndef GRAPE_TEXTANNOTATION_H
#define GRAPE_TEXTANNOTATION_H

#include "Annotation.h"
#include <QFont>
#include <QPen>

namespace Grape
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
    TextAnnotation();
    virtual ~TextAnnotation();
protected:
    void process(QPainter& painter);

}; // TextAnnotation

} // Grape

#endif // GRAPE_TEXTANNOTATION_H
