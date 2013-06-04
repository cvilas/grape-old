//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : ImageAnnotationViewer.cpp
//==============================================================================

#include "ImageAnnotationViewer.h"
#include "ImageAnnotationViewerP.h"
#include <QPainter>
#include <QMouseEvent>
#include <math.h>
#include <QAction>
#include <QMenu>

#ifndef M_PI
#define M_PI 3.141592654
#endif

namespace Grape
{

//==============================================================================
ImageAnnotationViewer::ImageAnnotationViewer(QWidget* pParent, Qt::WindowFlags f)
//==============================================================================
: QWidget(pParent, f), m_pAnnotator(0), m_mouseOverIndex(-1), m_isScaling(false)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    setMouseTracking(true);
    setUserInteractive(true);
}

//-------------------------------------------------------------------------------
ImageAnnotationViewer::~ImageAnnotationViewer()
//-------------------------------------------------------------------------------
{
}

//-------------------------------------------------------------------------------
void ImageAnnotationViewer::setUserInteractive(bool option)
//-------------------------------------------------------------------------------
{
    m_isUsrInteractive = option;
    if( option )
    {
        QObject::connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onContextMenuRequest(const QPoint&)));
    }
    else
    {
        QObject::disconnect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onContextMenuRequest(const QPoint&)));
    }
}


//-------------------------------------------------------------------------------
void ImageAnnotationViewer::setImageAnnotator(ImageAnnotator* pAnnotator)
//-------------------------------------------------------------------------------
{
    if( m_pAnnotator )
    {
        QObject::disconnect(m_pAnnotator, SIGNAL(annotated()), this, SLOT(onAnnotated()));
        QObject::disconnect(m_pAnnotator, SIGNAL(annotationAdded()), this, SLOT(onAddAnnotation()));
        QObject::disconnect(m_pAnnotator, SIGNAL(annotationRemoved(int)), this, SLOT(onRemoveAnnotation(int)));
    }
    m_attributes.clear();

    m_pAnnotator = pAnnotator;

    if( !m_pAnnotator )
    {
        return;
    }

    // Creates a second list to contain attributes of annotations contained in the
    // list managed by CImageAnnotator.

    int numAnnots = m_pAnnotator->getNumAnnotations();
    for(int i = 0; i < numAnnots; ++i )
    {
        onAddAnnotation();
    }
    QObject::connect(m_pAnnotator, SIGNAL(annotationAdded()), this, SLOT(onAddAnnotation()));
    QObject::connect(m_pAnnotator, SIGNAL(annotationRemoved(int)), this, SLOT(onRemoveAnnotation(int)));
    QObject::connect(m_pAnnotator, SIGNAL(annotated()), this, SLOT(onAnnotated()));
}

//-------------------------------------------------------------------------------
void ImageAnnotationViewer::onAddAnnotation()
//-------------------------------------------------------------------------------
{
    AnnotationAttributes attr;
    attr.isLocked = false;
    m_attributes.append(attr);
}

//-------------------------------------------------------------------------------
void ImageAnnotationViewer::onRemoveAnnotation( int index )
//-------------------------------------------------------------------------------
{
    if( (index >= 0) && (index < m_attributes.size()))
    {
        m_attributes.removeAt(index);
    }
}

//-------------------------------------------------------------------------------
void ImageAnnotationViewer::onAnnotated()
//-------------------------------------------------------------------------------
{
    update();
}

//-------------------------------------------------------------------------------
QMenu* ImageAnnotationViewer::createWindowContextMenu()
//-------------------------------------------------------------------------------
{
    QMenu* pContextMenu = new QMenu(this);

    QMenu* pAddMenu = new QMenu;
    pAddMenu->addAction("Text", this, SLOT(onMenuAddText()));
    pAddMenu->addAction("Time Stamp", this, SLOT(onMenuAddTimeStamp()));
    pAddMenu->addAction("Counter", this, SLOT(onMenuAddCounter()));
    pAddMenu->addAction("Cross Hair", this, SLOT(onMenuAddCrosshair()));
    pAddMenu->addAction("Image", this, SLOT(onMenuAddImage()));

    QAction* pAction = pContextMenu->addAction("Add Annotation");
    pAction->setMenu(pAddMenu);

    pAction = pContextMenu->addAction("Scale To Fit");
    pAction->setCheckable(true);
    pAction->setChecked(m_isScaling);
    QObject::connect(pAction, SIGNAL(triggered(bool)), this, SLOT(scaleToFit(bool)));

    pAction = pContextMenu->addAction("Enable Rotation");
    pAction->setCheckable(true);
    pAction->setChecked(m_pAnnotator->isImageRotationEnabled());
    QObject::connect(pAction, SIGNAL(triggered(bool)), this, SLOT(onRotationEnable(bool)));

    return pContextMenu;
}

//-------------------------------------------------------------------------------
QMenu* ImageAnnotationViewer::createAnnotationContextMenu(int index)
//-------------------------------------------------------------------------------
{
    QMenu* pMenu = new QMenu(this);

    QIdAction* pAction = new QIdAction(index, "Edit", 0);
    pMenu->addAction(pAction);
    QObject::connect(pAction, SIGNAL(triggered(int, bool)), this, SLOT(onMenuEditAnnotation(int, bool)));

    pAction = new QIdAction(index, "Remove", 0);
    pMenu->addAction(pAction);
    QObject::connect(pAction, SIGNAL(triggered(int, bool)), this, SLOT(onMenuRemoveAnnotation(int, bool)));

    pAction = new QIdAction(index, "Lock Position", 0);
    pAction->setCheckable(true);
    if( (index >= 0) && (index < m_attributes.size()))
    {
        pAction->setChecked(m_attributes[index].isLocked);
    }
    pMenu->addAction(pAction);
    QObject::connect(pAction, SIGNAL(triggered(int, bool)), this, SLOT(onMenuLockAnnotation(int, bool)));

    return pMenu;
}


//-------------------------------------------------------------------------------
void ImageAnnotationViewer::onContextMenuRequest(const QPoint& pos)
//-------------------------------------------------------------------------------
{
    if( !m_pAnnotator )
    {
        return;
    }

    // show annotation specific menu if annotation exists here, else
    // show generic menu

    int ind = 0;
    Annotation* pAnnot = m_pAnnotator->getAnnotation(mapWindowPosToInputImagePos(pos), ind);
    QMenu* pMenu = 0;
    if( pAnnot )
    {
        pMenu = createAnnotationContextMenu(ind);
    }
    else
    {
        pMenu = createWindowContextMenu();
    }
    pMenu->exec(mapToGlobal(pos));
    delete pMenu;
}

//-------------------------------------------------------------------------------
void ImageAnnotationViewer::onMenuAddText()
//-------------------------------------------------------------------------------
{
    if( !m_pAnnotator )
    {
        return;
    }

    TextAnnotation* pAnnot = new TextAnnotation;
    QPointF p = mapWindowPosToInputImagePos(m_prevPos);
    pAnnot->text = "Edit this text";
    pAnnot->name = QString("Text (%1, %2)").arg(p.x()).arg(p.y());
    pAnnot->font.setPointSize(15);
    pAnnot->font.setFamily("Arial");
    pAnnot->pen.setColor(Qt::black);
    pAnnot->alignFlags = Qt::AlignCenter | Qt::TextWordWrap;
    pAnnot->boundingBox = QRectF(p.x(), p.y(), m_pAnnotator->getOutputImage().width() - p.x(), 10);
    m_pAnnotator->addAnnotation(pAnnot, true);
    QDialog* pDialog = new TextAnnotationEditUi(pAnnot, this);
    pDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    pDialog->show();
}

//-------------------------------------------------------------------------------
void ImageAnnotationViewer::onMenuAddTimeStamp()
//-------------------------------------------------------------------------------
{
    if( !m_pAnnotator )
    {
        return;
    }

    TimeAnnotation* pAnnot = new TimeAnnotation;
    QPointF p = mapWindowPosToInputImagePos(m_prevPos);
    pAnnot->name = QString("Time (%1, %2)").arg(p.x()).arg(p.y());
    //pAnnot->format = "dd MMM yyyy hh:mm:ss.zzz";
    pAnnot->font.setPointSize(15);
    pAnnot->font.setFamily("Arial");
    pAnnot->pen.setColor(Qt::black);
    pAnnot->alignFlags = Qt::AlignLeft | Qt::TextWordWrap;
    pAnnot->boundingBox = QRectF(p.x(), p.y(), m_pAnnotator->getOutputImage().width() - p.x(), 10);
    m_pAnnotator->addAnnotation(pAnnot, true);
    QDialog* pDialog = new TextAnnotationEditUi(pAnnot, this);
    pDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    pDialog->show();
}

//-------------------------------------------------------------------------------
void ImageAnnotationViewer::onMenuAddCounter()
//-------------------------------------------------------------------------------
{
    if( !m_pAnnotator )
    {
        return;
    }

    CounterAnnotation* pAnnot = new CounterAnnotation;
    QPointF p = mapWindowPosToInputImagePos(m_prevPos);
    pAnnot->name = QString("Counter (%1, %2)").arg(p.x()).arg(p.y());
    pAnnot->count = 0;
    pAnnot->step = 1;
    pAnnot->fieldWidth = 6;
    pAnnot->fillChar = QChar(' ');
    pAnnot->font.setPointSize(15);
    pAnnot->font.setFamily("Arial");
    pAnnot->pen.setColor(Qt::black);
    pAnnot->alignFlags = Qt::AlignRight;
    pAnnot->boundingBox = QRectF(p.x(), p.y(), m_pAnnotator->getOutputImage().width() - p.x(), 10);
    m_pAnnotator->addAnnotation(pAnnot, true);
    QDialog* pDialog = new TextAnnotationEditUi(pAnnot, this);
    pDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    pDialog->show();
}

//-------------------------------------------------------------------------------
void ImageAnnotationViewer::onMenuAddCrosshair()
//-------------------------------------------------------------------------------
{
    if( !m_pAnnotator )
    {
        return;
    }

    CrosshairAnnotation* pAnnot = new CrosshairAnnotation;
    QPointF p = mapWindowPosToInputImagePos(m_prevPos);
    pAnnot->name = QString("Crosshair (%1, %2)").arg(p.x()).arg(p.y());
    pAnnot->thickness = 1;
    pAnnot->boundingBox = QRectF(p.x() - 25, p.y() - 25, 51, 51);
    m_pAnnotator->addAnnotation(pAnnot, true);
    QDialog* pDialog = new CrosshairAnnotationEditUi(pAnnot, m_pAnnotator->getOutputImage().size(), this);
    pDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    pDialog->show();
}

//-------------------------------------------------------------------------------
void ImageAnnotationViewer::onMenuAddImage()
//-------------------------------------------------------------------------------
{
    if( !m_pAnnotator )
    {
        return;
    }

    ImageAnnotation* pAnnot = new ImageAnnotation;
    QPointF p = mapWindowPosToInputImagePos(m_prevPos);
    pAnnot->name = QString("Image (%1, %2)").arg(p.x()).arg(p.y());
    pAnnot->image = QImage(":images/ocr200_t.png");
    pAnnot->boundingBox = QRectF(p.x(), p.y(), 200, 100);
    m_pAnnotator->addAnnotation(pAnnot, true);
    QDialog* pDialog = new ImageAnnotationSelectUi(pAnnot, this);
    pDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    pDialog->show();
}

//-------------------------------------------------------------------------------
void ImageAnnotationViewer::onMenuEditAnnotation(int index, bool)
//-------------------------------------------------------------------------------
{
    if( !m_pAnnotator )
    {
        return;
    }

    Annotation* pAnnot = m_pAnnotator->getAnnotation(index);
    if( !pAnnot )
    {
        return;
    }

    QString annotationType = QString(pAnnot->metaObject()->className());
    QDialog* pDialog = NULL;
    if( annotationType == "Grape::CrosshairAnnotation" )
    {
        pDialog = new CrosshairAnnotationEditUi(dynamic_cast<CrosshairAnnotation*>(pAnnot),
            m_pAnnotator->getOutputImage().size(),
            this);
    }
    else if( annotationType == "Grape::ImageAnnotation" )
    {
        pDialog = new ImageAnnotationEditUi(dynamic_cast<ImageAnnotation*>(pAnnot), this);
    }
    else if( annotationType == "Grape::TextAnnotation" )
    {
        pDialog = new TextAnnotationEditUi(dynamic_cast<TextAnnotation*>(pAnnot), this);
    }
    else if( annotationType == "Grape::TimeAnnotation" )
    {
        pDialog = new TextAnnotationEditUi(dynamic_cast<TimeAnnotation*>(pAnnot), this);
    }
    else if( annotationType == "Grape::CounterAnnotation" )
    {
        pDialog = new TextAnnotationEditUi(dynamic_cast<CounterAnnotation*>(pAnnot), this);
    }

    if( pDialog )
    {
        pDialog->setAttribute(Qt::WA_DeleteOnClose, true);
        pDialog->show();
    }
}

//-------------------------------------------------------------------------------
void ImageAnnotationViewer::onMenuRemoveAnnotation(int index, bool)
//-------------------------------------------------------------------------------
{
    if( !m_pAnnotator )
    {
        return;
    }
    m_pAnnotator->removeAnnotation(index);
    if( index == m_mouseOverIndex )
    {
        m_mouseOverIndex = -1;
    }
}

//-------------------------------------------------------------------------------
void ImageAnnotationViewer::onMenuLockAnnotation(int index, bool checked)
//-------------------------------------------------------------------------------
{
    if( (index >= 0) && (index < m_attributes.size()))
    {
        m_attributes[index].isLocked = checked;
    }
}

//-------------------------------------------------------------------------------
void ImageAnnotationViewer::mousePressEvent( QMouseEvent* pEvent )
//-------------------------------------------------------------------------------
{
    if( !m_isUsrInteractive )
    {
        return pEvent->ignore();
    }
    m_prevPos = pEvent->pos();
}

//-------------------------------------------------------------------------------
void ImageAnnotationViewer::mouseReleaseEvent( QMouseEvent* pEvent )
//-------------------------------------------------------------------------------
{
    if( !m_isUsrInteractive )
    {
        return pEvent->ignore();
    }
}

//-------------------------------------------------------------------------------
void ImageAnnotationViewer::mouseMoveEvent( QMouseEvent* pEvent)
//-------------------------------------------------------------------------------
{
    if( !m_isUsrInteractive )
    {
        return pEvent->ignore();
    }
    if( !m_pAnnotator )
    {
        return;
    }

    QPointF pos = pEvent->pos();

    // if left button active, move the current annotation around or rotate image
    if( pEvent->buttons() & Qt::LeftButton )
    {
        // if annotation is active, move it
        if( (m_mouseOverIndex >= 0) && (m_mouseOverIndex < m_attributes.size()) )
        {
            AnnotationAttributes &attr = m_attributes[m_mouseOverIndex];
            if( !attr.isLocked )
            {
                Annotation* pAnnot = m_pAnnotator->getAnnotation(m_mouseOverIndex); // find corresponding annotation
                QRectF bb = pAnnot->boundingBox.value();
                QPointF d = pos - m_prevPos;
                if ( m_isScaling )
                {
                    d /= m_scale;
                }
                bb.translate(d); // apply mouse movement
                pAnnot->boundingBox = bb;
            }
        }

        // otherwise rotate the image
        else
        {
            if( m_pAnnotator->isImageRotationEnabled() )
            {
                QPointF d;
                int w2 = width()/2;
                int h2 = height()/2;
                d.setX(pos.x() - w2);
                d.setY(pos.y() - h2);
                float deg = m_pAnnotator->getImageRotationAngle() + (180.0/M_PI) * atan2(d.y(),d.x());
                d.setX(m_prevPos.x() - w2);
                d.setY(m_prevPos.y() - h2);
                deg -= (180.0/M_PI) * atan2(d.y(), d.x());
                m_pAnnotator->setImageRotationAngle(deg);
            }
        }
    }

    // select the annotation under the mouse
    else
    {
        m_pAnnotator->getAnnotation(mapWindowPosToInputImagePos(pos), m_mouseOverIndex);
    }

    m_prevPos = pos;
    update();

}

//-------------------------------------------------------------------------------
void ImageAnnotationViewer::mouseDoubleClickEvent( QMouseEvent* pEvent )
//-------------------------------------------------------------------------------
{
    if( !m_isUsrInteractive )
    {
        return pEvent->ignore();
    }

    if( !m_pAnnotator )
    {
        return;
    }

    int index = -1;
    m_pAnnotator->getAnnotation(mapWindowPosToInputImagePos(pEvent->pos()), index);
    onMenuEditAnnotation(index, false);
}

//-------------------------------------------------------------------------------
void ImageAnnotationViewer::keyPressEvent( QKeyEvent* pEvent )
//-------------------------------------------------------------------------------
{
    if( !m_isUsrInteractive )
    {
        return pEvent->ignore();
    }

    if( !m_pAnnotator )
    {
        return;
    }

    switch( pEvent->key() )
    {
    case Qt::Key_Enter :
    case Qt::Key_Return:
        onMenuEditAnnotation(m_mouseOverIndex, false);
        break;
    case Qt::Key_Delete :
        onMenuRemoveAnnotation(m_mouseOverIndex, false);
        break;
    default:
        break;
    };
}

//-------------------------------------------------------------------------------
void ImageAnnotationViewer::paintEvent( QPaintEvent* )
//-------------------------------------------------------------------------------
{
    if( !m_pAnnotator )
    {
        return;
    }

    const QImage& image = m_pAnnotator->getOutputImage();

    QPainter painter(this);
    if( m_isScaling )
    {
        painter.setRenderHints(QPainter::SmoothPixmapTransform|QPainter::Antialiasing, true);
        painter.scale(m_scale, m_scale);
    }
    else
    {
        painter.setRenderHints(QPainter::SmoothPixmapTransform|QPainter::Antialiasing, false);
    }
    painter.drawImage(m_windowImageoffset, image);

    Annotation* pAnnot = m_pAnnotator->getAnnotation(m_mouseOverIndex);
    if( !pAnnot )
    {
        return;
    }

    QPen pen(Qt::white);
    pen.setWidth(1);
    painter.setPen(pen);
    painter.setBrush(QColor(127, 127, 127, 50));
    QRectF bb = pAnnot->boundingBox.value();
    bb.translate( m_windowImageoffset ); // apply mouse movement
    painter.drawRoundedRect( bb, 5, 5);
}

//-------------------------------------------------------------------------------
void ImageAnnotationViewer::resizeEvent( QResizeEvent* )
//-------------------------------------------------------------------------------
{
    calculateScaleOffset();
}

//-------------------------------------------------------------------------------
void ImageAnnotationViewer::calculateScaleOffset()
//-------------------------------------------------------------------------------
{
    if( !m_pAnnotator )
    {
        return;
    }

    const QImage& image = m_pAnnotator->getOutputImage();

    qreal imgW = image.width();
    qreal imgH = image.height();
    qreal winW = width();
    qreal winH = height();

    ( (imgW/imgH) > (winW/winH) ) ? (m_scale = winW/imgW) : (m_scale = winH/imgH);

    if( m_isScaling )
    {
        m_windowImageoffset = QPoint((width() - (m_scale * image.width()))/(2*m_scale), (height()- (m_scale * image.height()))/(2*m_scale));
    }
    else
    {
        m_windowImageoffset = QPoint((width() - image.width())/2, (height()- image.height())/2);
    }
}

} // Grape
