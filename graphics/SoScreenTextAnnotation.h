//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : SoScreenTextAnnotation.h
// Brief    : Text annotations on Open-Inventor scenegraph.
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

#ifndef GRAPE_SOSCREENTEXTANNOTATION_H
#define GRAPE_SOSCREENTEXTANNOTATION_H

#include "SoScreen2DAnnotationKit.h"
#include <Inventor/sensors/SoFieldSensor.h>

namespace grape
{

/// \class SoScreenTextAnnotation
/// \ingroup graphics
/// \brief Screen-aligned text annotations on scenegraphs views.
///
/// The SoScreenTextAnnotation extends SoScreen2DAnnotationKit class to provide screen-
/// aligned text annotations on Open-Inventor scenegraphs. The internal 'parts'
/// structure of this class is as follows:
///
/// \code
///	annotation			(SoSeparator from base class SoScreen2DAnnotationKit)
///		|- font			(SoFont)
///		|- material		(SoMaterial)
///		|- text			(SoText2)
/// \endcode
///
/// See the example program to learn more about using this node.
///
/// Example program:
/// \include Inventor/TextAnnotationExample.cpp
class GRAPEGRAPHICS_DLL_API SoScreenTextAnnotation : public SoScreen2DAnnotationKit
{
    typedef SoScreen2DAnnotationKit inherited;

    SO_KIT_HEADER(SoScreenTextAnnotation);

    SO_KIT_CATALOG_ENTRY_HEADER(font);
    SO_KIT_CATALOG_ENTRY_HEADER(material);
    SO_KIT_CATALOG_ENTRY_HEADER(text);

public:
    SoScreenTextAnnotation();
    static void initClass();

protected:
    virtual ~SoScreenTextAnnotation();
private:
    static void fontSizeChangedCB(void* pData, SoSensor *);
private:
    static bool _isInit;
    SoFieldSensor* _pFontSizeSensor;
}; // SoScreenTextAnnotation

} // grape

#endif // GRAPE_SOSCREENTEXTANNOTATION_H
