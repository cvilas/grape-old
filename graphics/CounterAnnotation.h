//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : CounterAnnotation.h
// Brief    : Counter annotations on QImages
//==============================================================================

#ifndef GRAPE_COUNTERANNOTATION_H
#define GRAPE_COUNTERANNOTATION_H

#include "TextAnnotation.h"

namespace Grape
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
    CounterAnnotation();
    virtual ~CounterAnnotation();
protected:
    void process(QPainter& painter);
}; // CounterAnnotation

} // Grape

#endif // GRAPE_COUNTERANNOTATION_H
