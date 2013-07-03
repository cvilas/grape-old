//==============================================================================
// File     :   SceneViewerExample.cpp
//==============================================================================

#include "SceneViewerExample.h"
#include "graphics/SoScreenTextAnnotation.h"

#include <QPainter>
#include <QTimer>
#include <QDir>
#include <QApplication>
#include <QGraphicsItem>
#include <QPushButton>
#include <QMessageBox>
#include <QFileDialog>

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/SbColor.h>
#include <Inventor/nodes/SoText2.h>


//==============================================================================  
TemperatureOverlay::TemperatureOverlay(QWidget* pParent, Qt::WindowFlags flags) 
//==============================================================================  
: QWidget(pParent, flags) 
{
}

//------------------------------------------------------------------------------  
void TemperatureOverlay::paintEvent(QPaintEvent*)
//------------------------------------------------------------------------------  
{
	QBrush backBrush(QColor(29, 136, 47, 255));
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	int side = qMin(width(), height());
	painter.setViewport((width() - side) / 2, (height() - side) / 2, side, side);
	painter.setWindow(-50, -50, 100, 100);

	painter.setBrush(backBrush);
	QPen pen(backBrush, 1, Qt::SolidLine, Qt::RoundCap);
	painter.setPen(pen);
	painter.drawRoundRect(QRect(-50, -50, 100, 100));
	draw(&painter);
}

//------------------------------------------------------------------------------  
void TemperatureOverlay::draw(QPainter* pPainter)
//------------------------------------------------------------------------------  
{
	QPen pen(Qt::white, 10, Qt::SolidLine, Qt::RoundCap);
	pPainter->setPen(pen);
	pPainter->drawLine(0, -40, 0, +20);    // stem of the icon 
	pen.setWidth(3);
	pPainter->setPen(pen);
	pPainter->drawLine(0, -30, 20, -30);   // 3 horizontal lines
	pPainter->drawLine(0, -20, 20, -20);
	pPainter->drawLine(0, -10, 20, -10);
	pPainter->setBrush(Qt::white);
	pPainter->drawEllipse(-10, 10, 20, 20); // bottom circle

	pPainter->setBrush(Qt::NoBrush);
	pen.setWidth(2);
	pPainter->setPen(pen);
	pPainter->drawEllipse(-10, 35, 5, 5);
	pPainter->drawArc(-1, 37, 10, 10, 60*16, 240*16);
}

//==============================================================================  
SceneViewerExample::SceneViewerExample() 
//==============================================================================  
: QWidget(NULL), _pSceneView(0)
{
	SoQtg::init(); // must be called before using QtGraphics library

	// Add to the scene viewer, which already provides a light and camera.
    _pSceneView = new SoSceneViewer(this);
    _pSceneView->getRenderArea()->setBackgroundColor(SbColor4f(0.68f,0.7f,0.8f, 1.0));
	setWindowTitle("SceneViewerExample");

	// on screen display
    _pOSD = new TemperatureOverlay;
    _pOSD->setPalette(Qt::transparent);
    _pOSD->move(10, 10);
    _pOSD->resize(50, 50);
    _pOSD->setWindowOpacity(0.6);
    _pOSD->setToolTip("Temperature OK");
    _pSceneView->addOSDItem("temperature", _pOSD);

	// home button
    _pGetHomeBtn = new QPushButton;
    _pGetHomeBtn->setPalette(Qt::transparent);
    _pGetHomeBtn->move(65, 10);
    _pGetHomeBtn->resize(50, 50);
    ((QPushButton*)_pGetHomeBtn)->setIcon(QIcon(":/images/home.png"));
    ((QPushButton*)_pGetHomeBtn)->setIconSize(QSize(45, 45));
    _pGetHomeBtn->setWindowOpacity(0.6);
    _pGetHomeBtn->setToolTip("Reset Camera Position");
    _pSceneView->addOSDItem("homeBtn", _pGetHomeBtn);
    QObject::connect(_pGetHomeBtn, SIGNAL(clicked()), this, SLOT(onHomeBtn()));

	// lamp button
    _pLampBtn = new QPushButton;
    _pLampBtn->setPalette(Qt::transparent);
    _pLampBtn->move(120, 10);
    _pLampBtn->resize(50, 50);
    ((QPushButton*)_pLampBtn)->setIcon(QIcon(":/images/bulb.png"));
    ((QPushButton*)_pLampBtn)->setIconSize(QSize(45, 45));
    _pLampBtn->setWindowOpacity(0.6);
    _pLampBtn->setToolTip("Toggle Camera Headlight");
    _pSceneView->addOSDItem("lampBtn", _pLampBtn);
    QObject::connect(_pLampBtn, SIGNAL(clicked()), this, SLOT(onLampBtn()));

	// lamp button
    _pCamTypBtn = new QPushButton;
    _pCamTypBtn->setPalette(Qt::transparent);
    _pCamTypBtn->move(175, 10);
    _pCamTypBtn->resize(50, 50);
    ((QPushButton*)_pCamTypBtn)->setIcon(QIcon(":/images/perProj.png"));
    ((QPushButton*)_pCamTypBtn)->setIconSize(QSize(45, 45));
    _pCamTypBtn->setWindowOpacity(0.6);
    _pCamTypBtn->setToolTip("Toggle Camera Type");
    _pSceneView->addOSDItem("camTypBtn", _pCamTypBtn);
    QObject::connect(_pCamTypBtn, SIGNAL(clicked()), this, SLOT(onCamTypBtn()));

	// snap button
    _pSnapBtn = new QPushButton;
    _pSnapBtn->setPalette(Qt::transparent);
    _pSnapBtn->move(230, 10);
    _pSnapBtn->resize(50, 50);
    ((QPushButton*)_pSnapBtn)->setIcon(QIcon(":/images/pictureCamera.png"));
    ((QPushButton*)_pSnapBtn)->setIconSize(QSize(45, 45));
    _pSnapBtn->setWindowOpacity(0.6);
    _pSnapBtn->setToolTip("Capture this view to file");
    _pSceneView->addOSDItem("snapBtn", _pSnapBtn);
    QObject::connect(_pSnapBtn, SIGNAL(clicked()), this, SLOT(onSnapBtn()));

	// OpgnGL text overlay
    _pTextOverlay = new SoScreenTextAnnotation;
    _pTextOverlay->ref();
    SoScreenTextAnnotation* pAnnot = (SoScreenTextAnnotation*)_pTextOverlay;
	pAnnot->position.setValue(10, 60);
	pAnnot->set("font", "name Arial:Bold");
	pAnnot->set("font", "size 14.0");
	pAnnot->set("material", "diffuseColor 1.0 0 0");
	pAnnot->set("text", "string \"Uninitialised annotation\"");
	pAnnot->setName("top_annotation");
    _pSceneView->getRenderArea()->addAnnotation(pAnnot);

	// OSD blink timer
    _pBlinkTimer = new QTimer;
    connect(_pBlinkTimer, SIGNAL(timeout()), this, SLOT(updateOnTimer()));
    _pBlinkTimer->setInterval(500);
    _pBlinkTimer->start();
}

//------------------------------------------------------------------------------  
void SceneViewerExample::setSceneGraph(SoNode* pScene)
//------------------------------------------------------------------------------  
{
    _pSceneView->setSceneGraph(pScene);
    if( _pSceneView->getRenderArea()->getCameraType() == SoPerspectiveCamera::getClassTypeId() )
	{
        ((QPushButton*)_pCamTypBtn)->setIcon(QIcon(":/images/persProj.png"));
	}
	else
	{
        ((QPushButton*)_pCamTypBtn)->setIcon(QIcon(":/images/orthoProj.png"));
	}
}

//------------------------------------------------------------------------------  
void SceneViewerExample::updateOnTimer()
//------------------------------------------------------------------------------  
{
	static bool state = false;
	state = !state;
	if( state )
	{
        _pOSD->show();
	}
	else
	{
        _pOSD->hide();
	}
    ((SoScreenTextAnnotation*)_pTextOverlay)->on.setValue(state);

	static unsigned int num = 0;
    SoText2* pText = (SoText2*)((SoScreenTextAnnotation*)_pTextOverlay)->getPart("text", 0);
	pText->string.setValue((const char*)((QString("Blink counter %1").arg(num++)).toLatin1()));

	repaint();
	//m_pSceneView->repaint();
}

//------------------------------------------------------------------------------  
void SceneViewerExample::onHomeBtn()
//------------------------------------------------------------------------------  
{
    _pSceneView->getRenderArea()->resetToHomePosition();
}

//------------------------------------------------------------------------------  
void SceneViewerExample::onLampBtn()
//------------------------------------------------------------------------------  
{
	static bool enable = false;
    _pSceneView->getRenderArea()->setHeadlight(enable);
	enable = !enable;
}

//------------------------------------------------------------------------------  
void SceneViewerExample::onCamTypBtn()
//------------------------------------------------------------------------------  
{
    SoQtGraphicsScene* pRenderArea = _pSceneView->getRenderArea();
	SoCamera* pCamera = pRenderArea->getCamera();
	if( !pCamera )
	{
		return;
	}
	
	SoType cameraType = pRenderArea->getCameraType();
	SoType perspectiveType = SoPerspectiveCamera::getClassTypeId();
	SoType orthoType = SoOrthographicCamera::getClassTypeId();

	float focalDist = pCamera->focalDistance.getValue();
	float vfov = (cameraType == perspectiveType) ? 
		(((SoPerspectiveCamera*)pCamera)->heightAngle.getValue() ) : 
		(((SoOrthographicCamera*)pCamera)->height.getValue() );

	pRenderArea->setCameraType( (cameraType == perspectiveType) ? orthoType : perspectiveType );
	
	pCamera = pRenderArea->getCamera();
	cameraType = pRenderArea->getCameraType();
	if( cameraType == orthoType )
	{
		((SoOrthographicCamera*)pCamera)->height.setValue(2 * focalDist * tanf(vfov/2.f));
        ((QPushButton*)_pCamTypBtn)->setIcon(QIcon(":/images/orthoProj.png"));
	}
	else
	{
		((SoPerspectiveCamera*)pCamera)->heightAngle.setValue(M_PI/4.f);//2 * atan2(vfov/2.f, focalDist)); 
        ((QPushButton*)_pCamTypBtn)->setIcon(QIcon(":/images/persProj.png"));
	}
}

//------------------------------------------------------------------------------  
void SceneViewerExample::onSnapBtn()
//------------------------------------------------------------------------------  
{
    QPixmap picture = QPixmap::grabWindow(_pSceneView->winId());

	QString filePath = QFileDialog::getSaveFileName(this, "Save Image File", "untitiled.png", 
		"Images (*.bmp *.jpg *.png)");
	if( filePath.length() )
	{
		picture.save(filePath);
	}
}

//------------------------------------------------------------------------------  
void SceneViewerExample::resizeEvent(QResizeEvent*)
//------------------------------------------------------------------------------  
{
    _pSceneView->resize(width(), height());
}

//------------------------------------------------------------------------------  
SceneViewerExample::~SceneViewerExample()
//------------------------------------------------------------------------------  
{
    _pSceneView->removeOSDItem("temperature");
    _pSceneView->removeOSDItem("homeBtn");
    _pSceneView->removeOSDItem("lampBtn");
    _pSceneView->removeOSDItem("camTypBtn");
    _pSceneView->removeOSDItem("snapBtn");
    _pBlinkTimer->stop();
    _pTextOverlay->unref();
    delete _pOSD;
    delete _pGetHomeBtn;
    delete _pLampBtn;
    delete _pCamTypBtn;
    delete _pSnapBtn;
    delete _pSceneView;
    delete _pBlinkTimer;
}

//==============================================================================  
int main(int argc, char** argv)
//==============================================================================  
{
	QApplication app(argc, argv);

	SceneViewerExample appWindow;

    QString modelPath = ("../examples/Inventor/iv/shadowLightsTest.iv");
	SoNode* pScene = readIvFile(modelPath, "PATH");
	if( pScene == NULL )
	{
		QMessageBox::critical(0, "File not found", QString("Unable to open %1").arg(modelPath));
		return -1;
	}
	pScene->ref();

	appWindow.setSceneGraph(pScene);
	appWindow.resize(640, 480);
	appWindow.show();

	int ret = app.exec();
	pScene->unref();
	return ret;
}
