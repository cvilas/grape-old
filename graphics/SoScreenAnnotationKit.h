//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : SoScreenAnnotationKit.h
// Brief    : Screen annotations on Open-Inventor scenegraph
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
