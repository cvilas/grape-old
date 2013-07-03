//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : SoSceneViewer.cpp
//==============================================================================


#include "SoSceneViewer.h"
#include <QGraphicsItem>
#include <QGLFormat>
#include <QResizeEvent>

namespace Grape
{

//==============================================================================
SoSceneViewer::SoSceneViewer(QWidget* pParent)
//==============================================================================
: QGraphicsView(pParent),
_pViewArea(0)
{
    QGL::FormatOptions opts;
    opts = QGL::SampleBuffers|QGL::DoubleBuffer|QGL::DepthBuffer|QGL::DirectRendering|
        QGL::Rgba|QGL::AlphaChannel/*|QGL::StereoBuffers*/;
    _pViewPort = new QGLWidget(QGLFormat(opts));
    setViewport( _pViewPort );
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setMouseTracking( true );
    setFocusPolicy(Qt::StrongFocus);

    _pViewArea = new SoQtGraphicsScene(this);
    setScene(_pViewArea);
    setInteractive(true);

    _pViewArea->setDoubleBuffer( _pViewPort->format().doubleBuffer() );
}

//------------------------------------------------------------------------------
SoSceneViewer::~SoSceneViewer()
//------------------------------------------------------------------------------
{
    _osdItems.clear();
    delete _pViewArea;
    delete _pViewPort;
}

//-----------------------------------------------------------------------------
void SoSceneViewer::setSceneGraph(SoNode* pScene)
//-----------------------------------------------------------------------------
{
    if( !pScene )
    {
        return;
    }
    _pViewArea->setSceneGraph(pScene);
}

//-----------------------------------------------------------------------------
void SoSceneViewer::addOSDItem(const QString& id, QWidget* pWidget)
//-----------------------------------------------------------------------------
{
    if( !pWidget )
    {
        return;
    }
    _pViewArea->addWidget(pWidget);
    QPoint pos = mapFromScene(pWidget->pos());
    QGraphicsItem* pItem = this->itemAt(pos);
    if( pItem )
    {
        pItem->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
        pItem->setFlag(QGraphicsItem::ItemIsMovable);
        _osdItems[id] = pItem;
    }
}

//-----------------------------------------------------------------------------
void SoSceneViewer::removeOSDItem(const QString& id)
//-----------------------------------------------------------------------------
{
    QMap<QString, QGraphicsItem*>::iterator it = _osdItems.find(id);
    if ( it == _osdItems.end() )
    {
        return;
    }

    _pViewArea->removeItem(it.value());
    _osdItems.erase(it);
}

//-----------------------------------------------------------------------------
void SoSceneViewer::resizeEvent(QResizeEvent *pEvent)
//-----------------------------------------------------------------------------
{
    if ( scene() )
    {
        scene()->setSceneRect(QRect(QPoint(0, 0), pEvent->size()));
        QGraphicsView::resizeEvent(pEvent);
    }
    _pViewArea->resize(width(), height());
}

} // Grape
