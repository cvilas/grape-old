//==============================================================================
// File     :   MultiScreenExample.cpp
// Brief    :   Example program demonstrating widget placement on multiple screens
//==============================================================================

#include <QApplication>
#include <QDesktopWidget>
#include "ui_screen_number.h"

// Description: This example demonstrates placement of widgets on multiple screens.

//==============================================================================  
class ScreenNumberWidget : public QWidget, private Ui::ScreenNumberWidget
//==============================================================================  
{
public:
	ScreenNumberWidget(const QString& num, QWidget* pParent = 0)
	: QWidget(pParent)
	{
		setupUi(this);
		Ui::ScreenNumberWidget::label->setText(num);
	}
};

//==============================================================================  
int main(int argc, char** argv)
//==============================================================================  
{
	QApplication app(argc, argv);
	
	// get the desktop widget class object to query about screens
	QDesktopWidget* pDesktop = QApplication::desktop();

	// get number of screens
	int screenCount = pDesktop->numScreens();

	// create a widget per screen
	QWidget** ppWidgets = new QWidget* [screenCount];
	for(int i = 0; i < screenCount; ++i )
	{
		ppWidgets[i] = new ScreenNumberWidget(QString("Desktop number %1").arg(i));
		QRect geom = pDesktop->screenGeometry(i);
		ppWidgets[i]->move(geom.x(), geom.y());
		ppWidgets[i]->showFullScreen();
	}

	int ret = app.exec();
	
	// cleanup
	for(int i = 0; i < screenCount; ++i )
	{
		delete ppWidgets[i];
	}
	delete [] ppWidgets;
	return ret;
}
