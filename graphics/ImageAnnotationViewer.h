//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : ImageAnnotationViewer.h
// Brief    : Viewer widget for QImage overlayed with annotations
//==============================================================================

#ifndef GRAPE_IMAGEANNOTATIONVIEWER_H
#define GRAPE_IMAGEANNOTATIONVIEWER_H

#include "ImageAnnotator.h"
#include <QWidget>

class QMenu;

namespace Grape
{


/// \class ImageAnnotationViewer
/// \brief Interactive viewer for ImageAnnotator.
///
/// ImageAnnotationViewer provides a surface to display the image stream processed
/// by ImageAnnotator. The class also provides a user interactive interface to
/// re-position or modify annotations overlayed on the image.
///
/// Example program:
/// \include 2DGraphics/ImageAnnotationViewerExample.cpp
class GRAPEGRAPHICS_DLL_API ImageAnnotationViewer : public QWidget
{
    Q_OBJECT
public:
    explicit ImageAnnotationViewer(QWidget* pParent = 0, Qt::WindowFlags flgs = 0);
    ///< Constructor. Does a few initialisations. setImageAnnotator() must be called
    ///< before using the object.
    ///< \param annotator	The annotator object to associate the viewer with.
    ///< \param pParent		Pointer to the parent window
    ///< \param flgs		Window flags

    ~ImageAnnotationViewer();
    ///< Destructor. Cleans up.

    void setImageAnnotator(ImageAnnotator* pAnnotator);
    ///< Associates the viewer with a specific annotator.
    ///< \param annotator	(input) The annotator object to associate the viewer with. Can be set to NULL.

    void setUserInteractive(bool option);
    ///< Allow users to interact with the viewer using the mouse.

public slots:
    inline void scaleToFit(bool );

private slots:
    void onContextMenuRequest(const QPoint& pos);
    void onMenuAddText();
    void onMenuAddTimeStamp();
    void onMenuAddCounter();
    void onMenuAddCrosshair();
    void onMenuAddImage();
    void onMenuEditAnnotation( int, bool );
    void onMenuRemoveAnnotation( int, bool );
    void onMenuLockAnnotation( int, bool);
    void onAddAnnotation();
    void onRemoveAnnotation( int );
    void onAnnotated();
    inline void onRotationEnable( bool );

private:
    QMenu* createWindowContextMenu();
    QMenu* createAnnotationContextMenu(int index);
    inline QPointF mapWindowPosToInputImagePos(const QPointF& windowPos);
    void calculateScaleOffset();

    virtual void mousePressEvent( QMouseEvent* );
    virtual void mouseReleaseEvent( QMouseEvent*  );
    virtual void mouseMoveEvent( QMouseEvent* );
    virtual void mouseDoubleClickEvent( QMouseEvent* );
    virtual void keyPressEvent( QKeyEvent* );
    virtual void paintEvent( QPaintEvent* );
    virtual void resizeEvent( QResizeEvent* );

private:
    ImageAnnotator* m_pAnnotator;

    ///< \cond (Doxygen - ignore section between cond and endcond)
    typedef struct AnnotationAttributes
    {
        bool		isLocked;	//!< whether the annotation is locked for motion or not.

        AnnotationAttributes() : isLocked(false)
        {
        }
    }AnnotationAttributes;
    ///< \endcond (Doxygen - ignore section between cond and endcond)

    QList<AnnotationAttributes> m_attributes;

    int		m_mouseOverIndex;		// index of annotation under mouse
    bool	m_isUsrInteractive;

    QPointF	m_prevPos;				// mouse motion tracking variable
    QPoint	m_windowImageoffset;	// offset between the top-left corners of the image and the containing window
    bool    m_isScaling;
    qreal	m_scale;
}; // ImageAnnotationViewer

//------------------------------------------------------------------------------
void ImageAnnotationViewer::scaleToFit(bool option)
//------------------------------------------------------------------------------
{
    m_isScaling = option;
    calculateScaleOffset();
    update();
}

//------------------------------------------------------------------------------
QPointF ImageAnnotationViewer::mapWindowPosToInputImagePos(const QPointF& windowPos)
//------------------------------------------------------------------------------
{
    QPointF pos = windowPos;
    if( m_isScaling )
    {
        pos /= m_scale;
    }
    pos -= m_windowImageoffset;
    return pos;
}

//------------------------------------------------------------------------------
void ImageAnnotationViewer::onRotationEnable(bool option)
//------------------------------------------------------------------------------
{
    if( m_pAnnotator )
    {
        m_pAnnotator->enableImageRotation(option);
        update();
    }
}

} // Grape


#endif // GRAPE_IMAGEANNOTATIONVIEWER_H
