//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : SoScreenTextAnnotation.cpp
//
// Copyright (c) 2012, Vilas Chitrakaran
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//    * Neither the name of the copyright holder nor the names of its
//      contributors may be used to endorse or promote products derived from
//      this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR ITS CONTRIBUTORS
// BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
// THE POSSIBILITY OF SUCH DAMAGE.
//==============================================================================


#include "SoScreenTextAnnotation.h"
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/actions/SoGLRenderAction.h>
//#include <Inventor/actions/SoWriteAction.h>

namespace grape
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

} // grape
