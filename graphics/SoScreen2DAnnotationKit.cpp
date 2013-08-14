//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : SoScreen2DAnnotationKit.cpp
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

#include "SoScreen2DAnnotationKit.h"
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoResetTransform.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoViewportRegionElement.h>

namespace Grape
{

SO_KIT_SOURCE(SoScreen2DAnnotationKit)

bool SoScreen2DAnnotationKit::_isInit = false;

//------------------------------------------------------------------------------
void SoScreen2DAnnotationKit::initClass()
//------------------------------------------------------------------------------
{
    if (!_isInit)
    {
        SO_KIT_INIT_CLASS(SoScreen2DAnnotationKit, SoScreenAnnotationKit, "SoScreenAnnotationKit");
        _isInit = true;
    }
}

//------------------------------------------------------------------------------
SoScreen2DAnnotationKit::SoScreen2DAnnotationKit()
//------------------------------------------------------------------------------
{
    isBuiltIn = FALSE;
    _annotHeight = 0;

    SO_KIT_CONSTRUCTOR(SoScreen2DAnnotationKit);

    SO_KIT_ADD_FIELD(position, (SbVec2s(0,0)) );

    // SO_KIT_ADD_CATALOG_ENTRY(name, className, nullByDefault, parentName, rightSiblingName, isPublicPart)
    // SO_KIT_CHANGE_ENTRY_TYPE(name, newClassName, newDefaultClassName )
    // Note: we must use "" instead of , , to humour MS VisualC++ 6.
    SO_KIT_CHANGE_ENTRY_TYPE(camera, SoOrthographicCamera, SoOrthographicCamera);

    SO_KIT_INIT_INSTANCE();

    // Set properties for various parts
    SoCamera* pCamera = (SoCamera*)(this->getAnyPart("camera", TRUE, FALSE, FALSE));
    pCamera->viewportMapping.setValue(SoCamera::CROP_VIEWPORT_NO_FRAME);
}

//------------------------------------------------------------------------------
SoScreen2DAnnotationKit::~SoScreen2DAnnotationKit()
//------------------------------------------------------------------------------
{
}

//------------------------------------------------------------------------------
void SoScreen2DAnnotationKit::GLRender(SoGLRenderAction *pAction)
//------------------------------------------------------------------------------
{
    if( !this->on.getValue() )
    {
        return;
    }

    //FIXME 1: The following is best done after detecting a change in viewport via a callback
    SoState *pState = pAction->getState();
    const SbViewportRegion &vpRegion = SoViewportRegionElement::get( pState );

    SoOrthographicCamera* pCamera = (SoOrthographicCamera*)(inherited::getAnyPart("camera", TRUE, FALSE, FALSE));
    const SbVec2s& vpSize = vpRegion.getViewportSizePixels();

    // Change camera field of view to match the viewport
    pCamera->height.setValue(vpSize[1]);
    pCamera->aspectRatio.setValue(vpSize[0]/(float)vpSize[1]);
    float xRadius = 0.5f * (vpSize[0] - 1);
    float yRadius = 0.5f * (vpSize[1] - 1);

    // re-position the annotation
    SbVec2s pos = position.getValue();
    pCamera->position = SbVec3f( xRadius - pos[0], pos[1] + _annotHeight - yRadius, 1 );

    inherited::GLRender(pAction);
}

} // Grape
