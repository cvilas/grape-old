//==============================================================================
// File     :   OffscreenRendererExample.cpp
// Brief    :   Example program for SoQtOffscreenRenderer class.
//==============================================================================

#include "graphics/SoQtOffscreenRenderer.h"
#include "graphics/SoScreenTextAnnotation.h"
#include <QApplication>
#include <Inventor/SoDB.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/engines/SoInterpolateVec3f.h>
#include <Inventor/nodes/SoFontStyle.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoText2.h>
#include <iostream>

using namespace grape;

//==============================================================================  
int main(int argc, char** argv)
//==============================================================================  
{
	QApplication app(argc, argv);
	SoQtg::init(); // must be called before using QtGraphics library

    // The pRoot node
    SoSeparator* pRoot = new SoSeparator;
    pRoot->ref();

	// It is mandatory to have at least one light for the offscreen renderer
    SoDirectionalLight* pLight = new SoDirectionalLight;
    pRoot->addChild(pLight);

    // It is mandatory to have at least one camera for the offscreen renderer
    SoPerspectiveCamera* pCamera = new SoPerspectiveCamera;
    SbRotation cameraRotation = SbRotation::identity();
    cameraRotation *= SbRotation(SbVec3f(1, 0, 0), -0.4f);
    cameraRotation *= SbRotation(SbVec3f(0, 1, 0), 0.4f);
    pCamera->orientation = cameraRotation;
    pRoot->addChild(pCamera);

	SoScreenTextAnnotation* pAnnotation = new SoScreenTextAnnotation;
	pAnnotation->setName("sample_annotation");
	pAnnotation->position.setValue(0, 20);
	pAnnotation->set("material", "diffuseColor 1.0 0 0");
	pAnnotation->set("font", "size 20.0");
	SoFont* pFont = (SoFont*)pAnnotation->getPart("font", true);
	pFont->name.setValue("Courier New");
	SoText2* pText = (SoText2*)pAnnotation->getPart("text", true);
	pText->string.setValue("Annotation text");
	pRoot->addChild(pAnnotation);

    // Something to show... A box
    SoCube* pCube = new SoCube;
    pRoot->addChild(pCube);

    // Set up the two camera positions we want to move the camera between
    SoInterpolateVec3f* pInterpolate = new SoInterpolateVec3f;
    pInterpolate->input0 = SbVec3f(2, 2, 9);
    pInterpolate->input1 = SbVec3f(2, 2, 5);
    pCamera->position.connectFrom(&pInterpolate->output);

    // Set up the offscreen renderer
    SbViewportRegion vpRegion(400, 300);
	QGLFormat fmt = QGLFormat::defaultFormat();
	//fmt.setAccum(true);
	//fmt.setAccumBufferSize(4);
	//fmt.setSampleBuffers(true);
	//fmt.setSamples(4);
	SoQtOffscreenRenderer offscreenRenderer(vpRegion, fmt);
	SoGLRenderAction * pAction = offscreenRenderer.getGLRenderAction();
	if (pAction != NULL)
	{
		pAction->setTransparencyType( SoGLRenderAction::SORTED_OBJECT_BLEND );
		pAction->setSmoothing(true);
		pAction->setPassUpdate(false);
		pAction->setNumPasses(10);
	}

    // How many frames to render for the video
    int frames = 5;
    std::cout << "[OffscreenRendererExample] Writing " << frames << " frames..." << std::endl;

    for (int i = 0; i < frames; i++) 
	{
		//Sleep(1000);

      // Update the camera position
      pInterpolate->alpha = float(i) / (frames - 1);

      // Render the scene
      SbBool ok = offscreenRenderer.render(pRoot);

      // Save the image to disk
      SbString filename = SbString("OffscreenRendererExample-") + (i + 1) + ".bmp";
      if (ok) 
	  {
		  offscreenRenderer.getImage().save(filename.getString(), "bmp");
      } 
	  else 
	  {
		  std::cout << "[OffscreenRendererExample] Error saving image: " << filename.getString() << std::endl;
		  break;
      }
    }

    std::cout << "[OffscreenRendererExample] Done!" << std::endl;

	QApplication::processEvents();

    pRoot->unref();
    return 0;
}
