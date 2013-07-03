//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : SoScreenAnnotationKit.cpp
//==============================================================================

#include "SoScreenAnnotationKit.h"
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoResetTransform.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoViewportRegionElement.h>

namespace Grape
{

SO_KIT_SOURCE(SoScreenAnnotationKit)

bool SoScreenAnnotationKit::_isInit = false;

//------------------------------------------------------------------------------
void SoScreenAnnotationKit::initClass()
//------------------------------------------------------------------------------
{
    if (!_isInit)
    {
        SO_KIT_INIT_CLASS(SoScreenAnnotationKit, SoBaseKit, "SoBaseKit");
        _isInit = true;
    }
}

//------------------------------------------------------------------------------
SoScreenAnnotationKit::SoScreenAnnotationKit()
//------------------------------------------------------------------------------
{
    isBuiltIn = FALSE;

    SO_KIT_CONSTRUCTOR(SoScreenAnnotationKit);

    SO_KIT_ADD_FIELD(on, (TRUE) );

    /// topSeparator			(SoSeparator)
    ///		|- camera			(SoCamera)
    ///		|- annotation		(SoSeparator)
    ///		|- resetTransform	(SoResetTransform)

    // SO_KIT_ADD_CATALOG_ENTRY(name, className, nullByDefault, parentName, rightSiblingName, isPublicPart)
    // SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(name, className, defaultClassName, nullByDefault, parentName,rightName, isPublicPart)
    // Note: we must use "" instead of , , to humour MS VisualC++ 6.
    SO_KIT_ADD_CATALOG_ENTRY(topSeparator, SoSeparator, FALSE, this, "", FALSE);
    SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(camera, SoCamera, SoOrthographicCamera, TRUE, topSeparator, annotation, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(annotation, SoSeparator, FALSE, topSeparator, resetTransform, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(resetTransform, SoResetTransform, FALSE, topSeparator, "", FALSE);

    SO_KIT_INIT_INSTANCE();

    SoResetTransform* pResetTrans = (SoResetTransform*)(this->getAnyPart("resetTransform", TRUE, FALSE, FALSE));
    pResetTrans->whatToReset = SoResetTransform::BBOX; // to avoid annotation geometry effecting viewAll()
}

//------------------------------------------------------------------------------
SoScreenAnnotationKit::~SoScreenAnnotationKit()
//------------------------------------------------------------------------------
{
}

//------------------------------------------------------------------------------
void SoScreenAnnotationKit::GLRender(SoGLRenderAction *pAction)
//------------------------------------------------------------------------------
{
    if( !this->on.getValue() )
    {
        return;
    }
    inherited::GLRender(pAction);
}

} //Grape
