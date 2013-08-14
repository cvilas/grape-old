//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : ImageAnnotator.h
// Brief    : Places annotations on QImages
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

#ifndef GRAPE_IMAGEANNOTATOR_H
#define GRAPE_IMAGEANNOTATOR_H

#include "graphics/Annotation.h"
#include <QImage>
#include <QColor>

namespace Grape
{

/// \class ImageAnnotator
/// \ingroup graphics
/// \brief Base class for placing annotations on a QImage.
///
/// ImageAnnotator is a base class for placing image aligned 2D annotations on
/// QImages. Examples of 2D annotations are text such as titles, legend, etc. and
/// images such as logos. An object of this class creates a new image with the
/// annotations embedded within it.
///
/// This class is most useful where input images can be different sizes but an
/// output stream of fixed size is required, such as for example input buffers
/// to video encoders.
///
/// \todo
/// - Mechanism to save/load overlay information to/from XML file.
///
/// Example program:
/// \include 2DGraphics/AnnotationExample.cpp
class GRAPEGRAPHICS_DLL_API ImageAnnotator : public QObject
{
    Q_OBJECT
    
public:
    enum ScaleMode
    {
        NoResize = 0,		//!< Display actual size, crop bottom-right corner if image is larger than output image.
        NoResizeCentered,	//!< Display actual size and centered, crop edges if image is larger than output image.
        ResizeToAspect,		//!< Resize to output image size, maintain aspect ratio
        ResizeToFill		//!< Stretch image in both dimensions to output image size disregarding aspect ratio.
    };

    /// Constructor. Before using this object, call setOutputImageSize()
    /// to set the desired output dimensions.
    explicit ImageAnnotator(QObject *parent = 0);

    /// Destructor clears allocated resources. Annotations added to this
    /// object are also destroyed.
    ~ImageAnnotator();

    /// Set the dimensions of the output image. Does nothing if the size is invalid.
    /// \param size	(input) Size of the output image.
    /// \param format	(input) Format of the output image.
    void setOutputImageSize(const QSize &size, QImage::Format format = QImage::Format_ARGB32_Premultiplied);


    //-------- Annotation objects ---------

    /// Add an annotation object to the end of an internal list of annotations. Things to note:
    /// - The annotations are applied in the order they were added.
    /// - The annotation objects must exist until they are removed by a call to removeAnnotation().
    /// - Annotations are placed relative to the coordinates of the output image.
    /// The signal annotationAdded() is raised on success.
    /// \param pAnnotation		(input) Pointer to an annotation object.
    /// \param deleteOnRemove	(input) If true, the annotator will take ownership of the annotation and
    /// 						delete the annotation object at the time of removal from the list.
    void addAnnotation(Annotation* pAnnotation, bool deleteOnRemove);

    /// Remove an annotation by index. Indices correspond to the order in which the annotations
    /// were added. The first annotation added to the annotator has index 0. After removal the
    /// annotation will be deleted if the second parameter to addAnnotation() was true when
    /// this annotation was added. The signal annotationRemoved() is raised on success.
    /// \param index	(input) Index of the annotation.
    /// \return		true on successful removal.
    bool removeAnnotation(int index);

    /// Remove an annotation by name. After removal the annotation will be deleted if the
    /// second parameter to addAnnotation() was true when this annotation was added.
    /// The signal annotationRemoved() is raised on success.
    /// \param name	(input) The name field of the annotation (Annotation::name)
    /// \return		True if annotation by the specified name was found and removed, else false.
    bool removeAnnotation(const QString& name);

signals:
    void annotated(); //!< Signal raised on call to annotate().
    void annotationAdded(); //!< Signal raised after a successful call to addAnnotation().

    /// Signal raised on a successful call to removeAnnotation().
    /// \param index Index location at which the annotation was removed.
    void annotationRemoved(int index);

public:
    /// \return the number of annotations in the list.
    int getNumAnnotations() const { return _annotations.count(); }

    /// Get an annotation by index. Indices correspond to the order in which the annotations
    /// were added. The first annotation added to the annotator has index 0.
    /// \return	Pointer to an annotation from the internal list, specified by the
    ///			index. Will return NULL if the index is invalid
    inline Annotation* getAnnotation(int index);

    /// Get the annotation located at the specified position in the image.
    /// \param pos		(input) The location in the coordinates of the output image.
    /// \param index	(output) The index position of the annotation in the internal list.
    ///				Set to -1 if annotation is not found.
    /// \return		Pointer to the annotation if one exists at the location, else NULL.
    Annotation* getAnnotation(const QPointF& pos, int &index);

    /// Get an annotation index. Indices correspond to the order in which the annotations
    /// were added. The first annotation added to the annotator has index 0.
    /// \return		The index at which the annotation by the specified name was found
    ///				in the internal list, else, -1.
    int getAnnotationIndex(const QString& name);

    //-------- Process annotations ---------

    /// Process annotations. The annotations are placed relative to the output image coordinates.
    /// \param image	(input) The input image.
    /// \param mode	(input) Specify how input images are scaled. Default is to
    ///				resize the input to the output image size maintaining aspect ratio.
    /// \return		The output image with annotations embedded.
    QImage& annotate(const QImage& image, enum ImageAnnotator::ScaleMode mode = ImageAnnotator::ResizeToAspect);

    /// Convenience function. Calls annotate().
    QImage& operator()(const QImage& image, enum ScaleMode mode = ImageAnnotator::ResizeToAspect) { return annotate(image, mode); }

    /// \return a reference to the QImage container for the output image.
    const QImage& getOutputImage() const { return _output; }

    //-------- Image rotation ---------

    /// Enable/disable rotation of the input image. Annotations are not rotated.
    inline void enableImageRotation(bool option);

    /// \return true if input image rotation is enabled.
    inline bool isImageRotationEnabled() const;

    /// Rotate the image centrally by the specified angle
    /// \param degrees (input) Rotation angle in degrees (positive for clockwise rotation)
    inline void setImageRotationAngle(float degrees);

    /// \return image rotation angle in degrees.
    inline float getImageRotationAngle() const;

    //-------- Misc. functions ---------

    /// Set the fill color for the background
    /// \param color (input) fill color.
    inline void setFillColor(const QColor &color);

    /// Set the background fill color before update. This causes the output image
    /// to be cleared before updating it with a new image. This may be necessary when
    /// annotate() is called multiple times over the same input image, causing
    /// overwrites on previous annotations in the same locations (visible, for example,
    /// with dynamic text annotations). This option is on by default. Turning it off
    /// may improve performance in some cases.
    /// \param option (input) Set to true to clear the output image before updating.
    void alwaysClearBeforeUpdate(bool option) { _fillBeforeUpdate = option; }

    /// When enabled, this method will anti-alias edges of annotations in the image.
    /// Anti aliasing is disabled by default.
    /// \param option (input) Set to true to enable, and false to disable.
    void setAntiAliasing(bool option);

    /// \return true if antialiasing render is enabled.
    bool getAntiAliasing();

    /// When enabled, this method using a higher quality smoothing algorithm (e.g. bilinear
    /// interpolation) rather than using nearest neighbour when transforming images.
    /// Smoothing is disabled by default.
    /// \param option (input) Set to true to enable, and false to disable.
    void setSmoothing(bool option);

    /// \return true if smoothing option is enabled.
    bool getSmoothing();

private slots:
    void onAnnotationMoved(); //!< Action to take when bounding box has moved.

private:
    bool		_isRotating;
    float		_angle;
    QPainter*	_pPainter;
    QImage		_output;
    QImage		_workBuf; // internal working buffer on which drawing is first done
    bool		_fillBkgnd;
    bool		_fillBeforeUpdate;
    uint		_fillColor;

    /// \cond
    typedef struct AnnotationProperties
    {
        Annotation*	pAnnotation;		//!< annotation object
        qint64      deleteOnRemove:1;	//!< all we need for now. May expand later

        AnnotationProperties()
            :deleteOnRemove(0)
        {}
    }AnnotationProperties;
    ///\endcond

    QList<AnnotationProperties> _annotations;
}; // ImageAnnotator

//-----------------------------------------------------------------------------
Annotation* ImageAnnotator::getAnnotation(int index)
//-----------------------------------------------------------------------------
{
    if( (index < 0) || (index >= _annotations.size()) )
    {
        return NULL;
    }
    return _annotations[index].pAnnotation;
}

//-----------------------------------------------------------------------------
void ImageAnnotator::setFillColor(const QColor &color)
//-----------------------------------------------------------------------------
{
    _fillColor = color.rgb(); _fillBkgnd = true;
}

//-----------------------------------------------------------------------------
void ImageAnnotator::enableImageRotation(bool option)
//-----------------------------------------------------------------------------
{
    _isRotating = option;
}

//-----------------------------------------------------------------------------
bool ImageAnnotator::isImageRotationEnabled() const
//-----------------------------------------------------------------------------
{
    return _isRotating;
}

//-----------------------------------------------------------------------------
void ImageAnnotator::setImageRotationAngle(float degrees)
//-----------------------------------------------------------------------------
{
    _angle = degrees;
}

//-----------------------------------------------------------------------------
float ImageAnnotator::getImageRotationAngle() const
//-----------------------------------------------------------------------------
{
    return _angle;
}

} // Grape

#endif // GRAPE_IMAGEANNOTATOR_H
