//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : SoScreenTextAnnotation.h
// Brief    : Text annotations on Open-Inventor scenegraph.
//==============================================================================

#ifndef GRAPE_SOSCREENTEXTANNOTATION_H
#define GRAPE_SOSCREENTEXTANNOTATION_H

#include "SoScreen2DAnnotationKit.h"
#include <Inventor/sensors/SoFieldSensor.h>

namespace Grape
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

} // Grape

#endif // GRAPE_SOSCREENTEXTANNOTATION_H
