//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : ImageAnnotationViewerP.h
// Brief    : Private header for ImageAnnotationViewer
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

#ifndef GRAPE_IMAGEANNOTATIONVIEWERP_H
#define GRAPE_IMAGEANNOTATIONVIEWERP_H

#include "ImageAnnotation.h"
#include "CrosshairAnnotation.h"
#include "TextAnnotation.h"
#include "CounterAnnotation.h"
#include "TimeAnnotation.h"
#include <QAction>
#include <QFileSystemModel>
#include "ui_crosshair_annotation_edit.h"
#include "ui_image_annotation_edit.h"
#include "ui_image_annotation_select.h"
#include "ui_text_annotation_attributes.h"
#include "ui_text_annotation_entry.h"
#include "ui_timestamp_annotation_format.h"
#include "ui_counter_annotation_format.h"

namespace Grape
{


///< \cond (Doxygen - ignore section between cond and endcond)

//==============================================================================
class QIdAction : public QAction
// QAction with an additional integer ID
//==============================================================================
{
    Q_OBJECT
public:
    QIdAction(int index, const QString& text, QObject* pParent)
        : QAction(text, pParent), _index(index)
    {
        QObject::connect(this, SIGNAL(triggered(bool)), this, SLOT(onTrigger(bool)));
    }
    virtual ~QIdAction() {}
    int getIndex() const { return _index; }
signals:
    void triggered(int index, bool checked = false);
private slots:
    void onTrigger(bool checked) { emit triggered(_index, checked); }
private:
    int _index;
};

//==============================================================================
class CrosshairAnnotationEditUi : public QDialog
// UI for editing cross hair annotations
//==============================================================================
{
    Q_OBJECT
public:
    CrosshairAnnotationEditUi( CrosshairAnnotation* pAnnot, const QSize& imageSize, QWidget* pParent);
    virtual ~CrosshairAnnotationEditUi();
private slots:
    void onEditId(const QString& t);
    void onThicknessChanged(int i);
    void onWidthChanged(int w);
    void onCentralise();
private:
    CrosshairAnnotation* _pAnnotation;
    Ui::CrosshairAnnotationEditDialog* _pUi;
    QSize	_imageSize;
};

//==============================================================================
class ImageAnnotationSelectUi : public QDialog
// UI for editing image overlays
//==============================================================================
{
    Q_OBJECT
public:
    ImageAnnotationSelectUi( ImageAnnotation* pAnnot, QWidget* pParent);
    virtual ~ImageAnnotationSelectUi();
private slots:
    void onItemClicked(const QModelIndex& index);
    void onAspectChanged(int);
    void onHeightChanged(int);
    void onWidthChanged(int);
private:
    QFileSystemModel _fsModel;
    ImageAnnotation* _pAnnotation;
    float _aspect;

    Ui::ImageAnnotationSelectDialog* _pUi;
};

//==============================================================================
class ImageAnnotationEditUi : public QDialog
// UI for editing image overlays
//==============================================================================
{
    Q_OBJECT
public:
    ImageAnnotationEditUi( ImageAnnotation* pAnnot, QWidget* pParent);
    virtual ~ImageAnnotationEditUi();
private slots:
    void onAspectChanged(int);
    void onHeightChanged(int);
    void onWidthChanged(int);
private:
    ImageAnnotation* _pAnnotation;
    float _aspect;
    Ui::ImageAnnotationEditDialog* _pUi;
};

//==============================================================================
class TextAnnotationAttributesUi : public QWidget
// Common attributes for text annotations
//==============================================================================
{
    Q_OBJECT
public:
    QFont&			_font;
    QPen&			_pen;
    int&			_flags;

public:
    TextAnnotationAttributesUi( TextAnnotation& annot, QWidget* pParent);
    virtual ~TextAnnotationAttributesUi();
signals:
    void fontPropertiesChanged();
private slots:
    void onFontChanged(const QFont&);
    void onSizeChanged(int);
    void onBold(bool);
    void onItalic(bool);
    void onColorDialog();
    void onColorSelected(const QColor& c);
    void onLeftAlign();
    void onCenterAlign();
    void onRightAlign();
private:
    Ui::TextAnnotationAttributesWidget* _pUi;
};

//==============================================================================
class TextAnnotationEntryUi : public QWidget
// Text annotation entry
//==============================================================================
{
    Q_OBJECT
public:
    QString&		_text;
public:
    TextAnnotationEntryUi( TextAnnotation& annot, QWidget* pParent);
    virtual ~TextAnnotationEntryUi();
signals:
    void textChanged();
private slots:
    void onTextChanged();
private:
    Ui::TextAnnotationEntryWidget* _pUi;
};

//==============================================================================
class TimeAnnotationFormatUi : public QWidget
// Timestamp annotation format
//==============================================================================
{
    Q_OBJECT
public:
    QString&	_format;
public:
    TimeAnnotationFormatUi( TimeAnnotation& annot, QWidget* pParent);
    virtual ~TimeAnnotationFormatUi();
signals:
    void formatChanged();
private slots:
    void onShowMs(bool);
    void onShowDate(bool);
    void parseFormat(const QString& fmt);
private:
    Ui::TimestampAnnotationFormatWidget* _pUi;
    QString _timeFmt;
    QString _dateFmt;
};

//==============================================================================
class CounterAnnotationFormatUi : public QWidget
// counter annotation format
//==============================================================================
{
    Q_OBJECT
public:
    CounterAnnotationFormatUi( CounterAnnotation* pAnnot, QWidget* pParent);
    virtual ~CounterAnnotationFormatUi();
signals:
    void formatChanged();
private slots:
    void onReset();
    void onWidthChanged(int);
    void onFillChar(bool);
private:
    CounterAnnotation* _pAnnotation;
    Ui::CounterAnnotationFormatWidget* _pUi;
};

//==============================================================================
class TextAnnotationEditUi : public QDialog
// text annotation editor ui
//==============================================================================
{
    Q_OBJECT
public:
    TextAnnotationEditUi(TextAnnotation* pAnnot, QWidget* pParent);
    virtual ~TextAnnotationEditUi();
private slots:
    void recalculateBoundingBox();
private:
    TextAnnotation* _pAnnotation;
};

///< \endcond (Doxygen - ignore section between cond and endcond)


} //Grape

#endif // GRAPE_IMAGEANNOTATIONVIEWERP_H
