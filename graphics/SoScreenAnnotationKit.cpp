//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : SoScreenAnnotationKit.cpp
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

#include "SoScreenAnnotationKit.h"
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoResetTransform.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoViewportRegionElement.h>

namespace grape
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
