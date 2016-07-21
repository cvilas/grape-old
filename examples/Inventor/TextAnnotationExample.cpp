//==============================================================================
// File     :   TextAnnotationExample.h
// Brief    :   Example program for SoScreenTextAnnotation class.
//==============================================================================

#include "graphics/SoSceneViewer.h"
#include "graphics/SoScreenTextAnnotation.h"

#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoFontStyle.h>
#include <Inventor/engines/SoElapsedTime.h>
#include <Inventor/nodes/SoRotationXYZ.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoText2.h>

#include <QApplication>

using namespace grape;

//==============================================================================  
int main(int argc, char** argv)
//==============================================================================  
{
	QApplication app(argc, argv);
	SoQtg::init(); // must be called before using QtGraphics library

	// Scenegraph root
	SoSeparator* pRoot = new SoSeparator;
	pRoot->ref();

	// The 3D scene - a rotating cube
	SoSeparator* p3dScene = new SoSeparator;
	SoMaterial* pMaterial = new SoMaterial;
	SoRotationXYZ* pRotation = new SoRotationXYZ;
	p3dScene->addChild(pRotation);
	p3dScene->addChild(pMaterial);
	p3dScene->addChild(new SoCube);
	pRoot->addChild(p3dScene);

	pMaterial->transparency.setValue(0.3);
	pRotation->axis = SoRotationXYZ::X;
	SoElapsedTime* pCounter = new SoElapsedTime;
	pRotation->angle.connectFrom(&pCounter->timeOut);

	// The 2D text annotation node
	SoScreenTextAnnotation* pAnnotation = new SoScreenTextAnnotation;
	pAnnotation->setName("sample_annotation");
	pRoot->addChild(pAnnotation);

	// setup annotation text..
	//.. fields work just the same way as any Open-Inventor node 
	pAnnotation->position.setValue(0, 0);

	//... Fields within parts of this nodekit can either be set as follows...
	pAnnotation->set("material", "diffuseColor 1.0 0 0");
	pAnnotation->set("font", "size 20.0");

	//... or a pointer to the part node could be obtained and manipulated 
	SoFont* pFont = (SoFont*)pAnnotation->getPart("font", true);
	pFont->name.setValue("Courier New");

	SoText2* pText = (SoText2*)pAnnotation->getPart("text", true);
	pText->string.setValue("Annotation text");

	SoSceneViewer viewer;
	viewer.setBackgroundColor(QColor(173, 178, 204, 255));
	viewer.setSceneGraph(pRoot);
	viewer.resize(320, 240);
	viewer.show();
	int val = app.exec();
	pRoot->unref();

	return val;
}
