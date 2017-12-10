//==============================================================================
// File     :   AnnotationViewerExample.cpp
// Brief    :    Example program for ImageAnnotationViewer class.
//==============================================================================

#include "graphics/ImageAnnotationViewer.h"
#include "graphics/TextAnnotation.h"
#include "graphics/ImageAnnotation.h"
#include "graphics/CrosshairAnnotation.h"

#include <QApplication>
#include <iostream>
#include <QScrollArea>

//==============================================================================
int main(int argc, char** argv)
//==============================================================================
{
	QApplication app(argc, argv);

	const unsigned int OUT_WIDTH = 640;
	const unsigned int OUT_HEIGHT = 480;

	// Input image
	QImage image;
    if( !image.load("../examples/2DGraphics/images/dk_med.png") )
	{
		std::cout << "Failed opening input image" << std::endl;
		return -1;
	}
	image = image.scaled(OUT_WIDTH/2, OUT_HEIGHT/2, Qt::KeepAspectRatio);

	// create the annotation processor
    grape::ImageAnnotator annotator;
	annotator.setAntiAliasing(true);
	annotator.setSmoothing(true);
	annotator.setOutputImageSize(QSize(OUT_WIDTH, OUT_HEIGHT));
	annotator.setFillColor(Qt::gray);

	// Add a text annotation
    grape::TextAnnotation* pTextAnnot = new grape::TextAnnotation;
	annotator.addAnnotation(pTextAnnot, true);

	// Add an image annotation
    grape::ImageAnnotation* pImageAnnot = new grape::ImageAnnotation;
	annotator.addAnnotation(pImageAnnot, true);
	
	// Add a crosshair
    grape::CrosshairAnnotation* pCrosshair = new grape::CrosshairAnnotation;
	annotator.addAnnotation(pCrosshair, true);

	// prepare the annotations
	pTextAnnot->name = "text";
	pTextAnnot->text = "Right click to add/edit/remove annotations";
	pTextAnnot->boundingBox = QRect(0, 0, OUT_WIDTH, 20);
	pTextAnnot->font.setPointSize(10);
	pTextAnnot->font.setFamily("Arial");
	pTextAnnot->pen.setColor(Qt::white);
	pTextAnnot->alignFlags = Qt::AlignLeft;

    pImageAnnot->image = QImage("../examples/2DGraphics/images/dk_med.png");
	pImageAnnot->name = "logo";
	pImageAnnot->boundingBox = QRect(OUT_WIDTH - 150,
								   OUT_HEIGHT - 75,
								   150,
								   75);

	pCrosshair->name = "crosshair";
	pCrosshair->thickness = 2;
	pCrosshair->boundingBox = QRect(OUT_WIDTH/2-10, OUT_HEIGHT/2-10, 21 , 21);

	// Rotate the image...just for fun.
	//annotator.rotate(30);

	// and scale.
	//annotator.scale(0.7f);

	// annotate the input image
    grape::ImageAnnotationViewer viewer;
	viewer.setImageAnnotator(&annotator);
	viewer.setWindowTitle("ImageAnnotationViewerExample");
	viewer.resize(OUT_WIDTH, OUT_HEIGHT);
	viewer.show();
	while( viewer.isVisible() )
	{
        annotator(image, grape::ImageAnnotator::NoResizeCentered);
		app.processEvents();
	}

	return 0;
}
