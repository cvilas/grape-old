//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : SoSceneViewer.cpp
//
// Copyright (c) 2012, Vilas Chitrakaran
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//    * Neither the name of the copyright holder nor the names of its
//      contributors may be used to endorse or promote products derived from
//      this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR ITS CONTRIBUTORS
// BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
// THE POSSIBILITY OF SUCH DAMAGE.
//==============================================================================


#include "SoSceneViewer.h"
#include <QGraphicsItem>
#include <QGLFormat>
#include <QResizeEvent>

namespace grape
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
    /*
    if( !pScene )
    {
        return;
    }*/
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

} // grape
