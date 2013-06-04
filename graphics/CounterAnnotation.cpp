//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : CounterAnnotation.cpp
//==============================================================================

#include "CounterAnnotation.h"

namespace Grape
{

//==============================================================================
CounterAnnotation::CounterAnnotation()
//==============================================================================
: TextAnnotation(), step(1), fieldWidth(0), fillChar(QLatin1Char(' '))
{
    count = 0;
}

//------------------------------------------------------------------------------
CounterAnnotation::~CounterAnnotation()
//------------------------------------------------------------------------------
{
}

//------------------------------------------------------------------------------
void CounterAnnotation::process(QPainter& painter)
//------------------------------------------------------------------------------
{
    TextAnnotation::text = QString("%1").arg(count.value(), fieldWidth, 10, fillChar);
    count = count.value() + 1;
    TextAnnotation::process(painter);
}


} //Grape
