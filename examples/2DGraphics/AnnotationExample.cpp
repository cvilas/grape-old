//==============================================================================
// File     :   AnnotationExample.cpp
// Brief    :   Example program for ImageAnnotator class.
//==============================================================================

#include "graphics/ImageAnnotator.h"
#include "graphics/TextAnnotation.h"
#include "graphics/CrosshairAnnotation.h"
#include "graphics/ImageAnnotation.h"

#include <iostream>

#include <QApplication>
#include <QDateTime>

//==============================================================================
int main(int argc, char** argv)
//==============================================================================
{
	QApplication app(argc, argv); // This is required to use QFont.

	const unsigned int OUT_WIDTH = 640;
	const unsigned int OUT_HEIGHT = 480;

	// Input image
	QImage image;
    if( !image.load("/home/vilas/Projects/mine/grape/examples/2DGraphics/images/inspection.bmp") )
	{
		std::cout << "Failed opening input image" << std::endl;
		return -1;
	}

	// create the annotation processor
    Grape::ImageAnnotator annotator;
	annotator.setOutputImageSize(QSize(OUT_WIDTH, OUT_HEIGHT));

	// Add a text annotation
    Grape::TextAnnotation* pTextAnnot = new Grape::TextAnnotation;
	annotator.addAnnotation(pTextAnnot, true);

	// Add an image annotation
    Grape::ImageAnnotation* pImageAnnot = new Grape::ImageAnnotation;
	annotator.addAnnotation(pImageAnnot, true);
	
	// Add a crosshair
    Grape::CrosshairAnnotation* pCrosshair = new Grape::CrosshairAnnotation;
	annotator.addAnnotation(pCrosshair, true);

	// prepare the annotations
	pTextAnnot->name = "comment";
	pTextAnnot->boundingBox = QRect(0, 0, OUT_WIDTH, 20);
	pTextAnnot->font.setPointSize(15);
	pTextAnnot->font.setFamily("Arial");
	pTextAnnot->pen.setColor(Qt::green);
	pTextAnnot->alignFlags = Qt::AlignCenter;
	pTextAnnot->text = QDateTime::currentDateTime().toString();

    pImageAnnot->image = QImage("/home/vilas/Projects/mine/grape/examples/2DGraphics/images/dk_med.png");
	pImageAnnot->name = "logo";
	pImageAnnot->boundingBox = QRect(OUT_WIDTH - 150,
								   OUT_HEIGHT - 75,
								   150,
								   75);

	pCrosshair->name = "crosshair";
	pCrosshair->thickness = 2;
	pCrosshair->boundingBox = QRect(OUT_WIDTH/2, OUT_HEIGHT/2, 20 , 20);

	// annotate the input image
	const QImage& imageOut = annotator.annotate(image);

	// or, you can do this
	// QImage& imageOut = annotator(image);

	// save the annotated image
	if( !imageOut.save("ImageAnnotationExample.png", "PNG") )
	{
		std::cout << "Saving image failed" << std::endl;
		return -1;
	}

	return 0;
}
