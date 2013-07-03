//==============================================================================
// File     :   SceneViewerExample.h
// Brief    :   Example program for SoSceneViewer class.
//==============================================================================

#include "graphics/SoSceneViewer.h"

using namespace Grape;

//==============================================================================  
class TemperatureOverlay : public QWidget
//==============================================================================  
{
public:
	TemperatureOverlay(QWidget* pParent=NULL, Qt::WindowFlags flags=0);
	virtual ~TemperatureOverlay() {}
private:
	void paintEvent(QPaintEvent* pEvent);
	void draw(QPainter* pPainter);
};

//==============================================================================  
class SceneViewerExample : public QWidget
//==============================================================================  
{
	Q_OBJECT
public:
	SceneViewerExample();
	void setSceneGraph(SoNode* pScene);
	virtual ~SceneViewerExample();
private slots:
	void updateOnTimer();
	void onHomeBtn();
	void onLampBtn();
	void onCamTypBtn();
	void onSnapBtn();
private:
	void resizeEvent(QResizeEvent* pPainter);
	SceneViewerExample(const SceneViewerExample&);
private:
    SoNode*			_pTextOverlay;
    SoSceneViewer*	_pSceneView;
    QTimer*			_pBlinkTimer;
    QWidget*		_pOSD;
    QWidget*		_pGetHomeBtn;
    QWidget*		_pLampBtn;
    QWidget*		_pCamTypBtn;
    QWidget*		_pSnapBtn;
};
