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

//------------------------------------------------------------------------------
void SoQtg::init()
//------------------------------------------------------------------------------
{
    SoQtg&obj = getSingleton();

    if( obj._isInit )
    {
        return;
    }

    SoRenderArea::init();
    SoScreenAnnotationKit::initClass();
    SoScreen2DAnnotationKit::initClass();
    SoScreenTextAnnotation::initClass();

    obj._isInit = true;
}

} // Grape
