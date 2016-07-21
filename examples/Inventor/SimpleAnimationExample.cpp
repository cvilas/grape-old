//==============================================================================
// File     :   SimpleAnimationExample.cpp
// Brief    :   Example program for SoSceneViewer class.
//==============================================================================

#include "graphics/SoSceneViewer.h"
#include <QApplication>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/engines/SoElapsedTime.h>
#include <Inventor/nodes/SoRotationXYZ.h>

using namespace grape;

//==============================================================================  
int main(int argc, char** argv)
//==============================================================================  
{
	QApplication app(argc, argv);
	SoQtg::init(); // must be called before using the Coin library

	SoSeparator* pRoot = new SoSeparator;
	pRoot->ref();

	SoRotationXYZ* pRotation = new SoRotationXYZ;
	pRoot->addChild(pRotation);
	pRoot->addChild(new SoCube);

	pRotation->axis = SoRotationXYZ::X;
	SoElapsedTime* pCounter = new SoElapsedTime;
	pRotation->angle.connectFrom(&pCounter->timeOut);

	SoSceneViewer viewer;
	viewer.setSceneGraph(pRoot);

	viewer.resize(320, 240);
	viewer.show();

	int val = app.exec();
	
	pRoot->unref();
	return val;
}
