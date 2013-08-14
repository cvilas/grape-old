//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : SoScreenAnnotationKit.h
// Brief    : Screen annotations on Open-Inventor scenegraph
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

#ifndef GRAPE_SOSCREENANNOTATIONKIT_H
#define GRAPE_SOSCREENANNOTATIONKIT_H

#include "SoQtg.h"
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/fields/SoSFVec2s.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoMFString.h>

namespace Grape
{

/// \class SoScreenAnnotationKit
/// \ingroup graphics
/// \brief Base node-kit class for screen-aligned annotations on scenegraphs views.
///
/// SoScreenAnnotationKit is the base class for all annotations placed relative
/// to the screen coordinates of the viewport of Open-Inventor scenegraphs.
/// Examples of 2D annotations are screen aligned text such as titles, legend,
/// etc., and images such as logos. Similarly, a compass ('gnomon') widget
/// typically seen in 3D graphics manipulation programs is an example of a 3D
/// annotation. Application programmers will only use one of the derived classes.
/// The class sets up a subgraph within the nodekit with the following parts:
///
/// \code
/// topSeparator			(SoSeparator)
///		|- camera			(SoCamera)
///		|- annotation		(SoSeparator)
///		|- resetTransform	(SoResetTransform)
/// \endcode
///
/// See derived classes for more information. Note that Open-Inventor natively
/// provides the SoAnnotation class, which should be sufficient for placing
/// annotations relative to scene coordinates (rather than screen coordinates)
///
/// Example program:
/// See examples for derived classes
class GRAPEGRAPHICS_DLL_API SoScreenAnnotationKit : public SoBaseKit
{
    typedef SoBaseKit inherited;

    SO_KIT_HEADER(SoScreenAnnotationKit);

    SO_KIT_CATALOG_ENTRY_HEADER(topSeparator);
    SO_KIT_CATALOG_ENTRY_HEADER(camera);
    SO_KIT_CATALOG_ENTRY_HEADER(annotation);
    SO_KIT_CATALOG_ENTRY_HEADER(resetTransform);

public:
    SoSFBool	on;			//!< Whether visible or not. Default is TRUE.
    static void initClass();

private:
    static bool _isInit;

protected:
    SoScreenAnnotationKit();
    virtual ~SoScreenAnnotationKit();
    virtual void GLRender(SoGLRenderAction *pAction);
}; // SoScreenAnnotationKit

} // Grape

#endif // GRAPE_SOSCREENANNOTATIONKIT_H
