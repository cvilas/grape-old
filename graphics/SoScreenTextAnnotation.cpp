//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : SoScreenTextAnnotation.cpp
//==============================================================================


#include "SoScreenTextAnnotation.h"
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/actions/SoGLRenderAction.h>
//#include <Inventor/actions/SoWriteAction.h>

namespace Grape
{

SO_KIT_SOURCE(SoScreenTextAnnotation)

bool SoScreenTextAnnotation::_isInit = false;

//------------------------------------------------------------------------------
void SoScreenTextAnnotation::initClass()
//------------------------------------------------------------------------------
{
    if( !_isInit )
    {
        SO_KIT_INIT_CLASS(SoScreenTextAnnotation, SoScreen2DAnnotationKit, "SoScreen2DAnnotationKit");
        _isInit = true;
    }
}

//------------------------------------------------------------------------------
SoScreenTextAnnotation::SoScreenTextAnnotation()
//------------------------------------------------------------------------------
: SoScreen2DAnnotationKit()
{
    isBuiltIn = FALSE;

    SO_KIT_CONSTRUCTOR(SoScreenTextAnnotation);

    ///	this
    ///		|- annotation		(SoSeparator)
    ///			|- font			(SoFont)
    ///			|- material		(SoMaterial)
    ///			|- text			(SoText2)

    // SO_KIT_ADD_CATALOG_ENTRY(name, className, nullByDefault, parentName, rightSiblingName, isPublicPart)
    // Note: we must use "" instead of , , to humour MS VisualC++ 6.
    SO_KIT_ADD_CATALOG_ENTRY(font, SoFont, FALSE, annotation, material, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(material, SoMaterial, TRUE, annotation, text, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(text, SoText2, FALSE, annotation, "", TRUE);

    SO_KIT_INIT_INSTANCE();

    SoFont* pFont = (SoFont*)(this->getAnyPart("font", TRUE, FALSE, FALSE));
    _pFontSizeSensor = new SoFieldSensor(fontSizeChangedCB, this);
    _pFontSizeSensor->attach(&pFont->size);

    //SoOutput output;
    //output.openFile("SoScreenTextAnnotation.iv");
    //SoWriteAction writeAction(&output);
    //writeAction.apply(this);
    //output.closeFile();
}

//------------------------------------------------------------------------------
SoScreenTextAnnotation::~SoScreenTextAnnotation()
//------------------------------------------------------------------------------
{
    delete _pFontSizeSensor;
}

//------------------------------------------------------------------------------
void SoScreenTextAnnotation::fontSizeChangedCB(void *pData, SoSensor *)
//------------------------------------------------------------------------------
{
    SoScreenTextAnnotation* pObject = (SoScreenTextAnnotation*)pData;
    SoFont* pFont = (SoFont*)(pObject->getAnyPart("font", TRUE, FALSE, FALSE));
    pObject->_annotHeight = pFont->size.getValue();
    pObject->touch();
}

} // Grape
