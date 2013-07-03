//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : SoScreen2DAnnotationKit.h
// Brief    : 2D annotations on Open-Inventor scenegraph.
//==============================================================================

#ifndef GRAPE_SOSCREEN2DANNOTATIONKIT_H
#define GRAPE_SOSCREEN2DANNOTATIONKIT_H

#include "SoQtg.h"
#include "SoScreenAnnotationKit.h"
#include <Inventor/fields/SoSFVec2s.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoMFString.h>

namespace Grape
{

/// \class SoScreen2DAnnotationKit
/// \ingroup graphics
/// \brief Base node-kit class for screen-aligned 2D annotations on scenegraphs views.
///
/// SoScreen2DAnnotationKit is the base class for all two-dimentional annotations on
/// scenegraphs. Examples of 2D annotations are screen aligned text such as titles,
/// legend, etc and images such as logos. Application programmers will use only
/// the derived classes directly. The class modifies the base class SoScreenAnnotationKit
/// by setting up an orthographic camera for the annotation subgraph.
///
/// The 'annotation' part of the graph is extended as appropriate in the derived
/// classes. For example, for text annotations, a SoText2 part is added to the
/// annotation, along with SoFont and SoMaterial to set text properties.
///
/// \todo
/// - Efficient detection of viewport change for camera relocation
///
/// Example program:
/// See examples for derived classes
class GRAPEGRAPHICS_DLL_API SoScreen2DAnnotationKit : public SoScreenAnnotationKit
{
    typedef SoScreenAnnotationKit inherited;

    SO_KIT_HEADER(SoScreen2DAnnotationKit);

public:
    SoSFVec2s	position;	//!< Position in screen coordinates. Upper left corner is (0,0).

    static void initClass();

protected:
    /// Set by derived classes to the height of the annotation object.
    /// Used by this class to adjust camera position
    int	_annotHeight;

private:
    static bool _isInit;

protected:
    SoScreen2DAnnotationKit();
    virtual ~SoScreen2DAnnotationKit();
    virtual void GLRender(SoGLRenderAction *pAction);
}; // SoScreen2DAnnotationKit

} // Grape

#endif // GRAPE_SOSCREEN2DANNOTATIONKIT_H
