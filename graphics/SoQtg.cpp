//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : SoQtg.cpp
//==============================================================================

#include "SoQtg.h"
#include "SoRenderArea.h"
#include "SoScreenTextAnnotation.h"

namespace Grape
{

//==============================================================================
SoQtg::SoQtg()
//==============================================================================
{
}

//------------------------------------------------------------------------------
void SoQtg::init()
//------------------------------------------------------------------------------
{
    if( SoQtg::_isInit )
    {
        return;
    }

    SoRenderArea::init();
    SoScreenAnnotationKit::initClass();
    SoScreen2DAnnotationKit::initClass();
    SoScreenTextAnnotation::initClass();

    SoQtg::_isInit = true;
}

} // Grape
