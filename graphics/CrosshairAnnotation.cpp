//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : CrosshairAnnotation.cpp
//==============================================================================

#include "CrosshairAnnotation.h"
#include <QPainter>

namespace Grape
{

//==============================================================================
CrosshairAnnotation::CrosshairAnnotation(QObject *parent)
//==============================================================================
: Annotation(parent)
{
    _pLines = new QLine [2];
    QObject::connect(&boundingBox, SIGNAL(modified()), this, SLOT(updatePosition()));
    QObject::connect(&thickness, SIGNAL(modified()), this, SLOT(updateThickness()));
    _pen.setColor(Qt::white);
    boundingBox = QRectF(0, 0, 10, 10);
    thickness = 1;
}

//------------------------------------------------------------------------------
CrosshairAnnotation::~CrosshairAnnotation()
//------------------------------------------------------------------------------
{
    delete [] _pLines;
}

//------------------------------------------------------------------------------
void CrosshairAnnotation::updatePosition()
//------------------------------------------------------------------------------
{
    QRectF bb = boundingBox.value();
    const int &x = bb.x();
    const int &y = bb.y();
    const int &w = bb.width();
    const int &h = bb.height();
    _pLines[0].setLine( x, y + h/2, x + w, y + h/2 );
    _pLines[1].setLine( x + w/2, y, x + w/2, y + h );
}

//------------------------------------------------------------------------------
void CrosshairAnnotation::process(QPainter& painter)
//------------------------------------------------------------------------------
{
    painter.setCompositionMode(QPainter::CompositionMode_Exclusion);
    painter.setPen(_pen);
    painter.drawLines(_pLines, 2);
}

} // Grape
