//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : TimeAnnotation.cpp
//==============================================================================

#include "TimeAnnotation.h"
#include <QDateTime>

namespace Grape
{

//==============================================================================
TimeAnnotation::TimeAnnotation()
//==============================================================================
: TextAnnotation(), format("dd MMM yyyy - hh:mm:ss.zzz")
{
}

//------------------------------------------------------------------------------
TimeAnnotation::~TimeAnnotation()
//------------------------------------------------------------------------------
{
}

//------------------------------------------------------------------------------
void TimeAnnotation::process(QPainter& painter)
//------------------------------------------------------------------------------
{
    TextAnnotation::text = QDateTime::currentDateTime().toString(format);
    TextAnnotation::process(painter);
}

} // Grape
