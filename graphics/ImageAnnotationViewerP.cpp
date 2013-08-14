//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : ImageAnnotationViewerP.cpp
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

#include "ImageAnnotationViewerP.h"
#include <QtCore/qmath.h>
#include <QColorDialog>
#include <QVBoxLayout>
#include <QFontMetrics>

namespace Grape
{

//==============================================================================
CrosshairAnnotationEditUi::CrosshairAnnotationEditUi( CrosshairAnnotation* pAnnot,
                                                                 const QSize& imageSize,
                                                                 QWidget* pParent)
//==============================================================================
: QDialog(pParent), _pAnnotation(pAnnot), _pUi(new Ui::CrosshairAnnotationEditDialog),
_imageSize(imageSize)
{
    Qt::WindowFlags flags = windowFlags();
    flags &= ~Qt::WindowMinimizeButtonHint;
    flags &= ~Qt::WindowContextHelpButtonHint;
    flags |= (Qt::MSWindowsFixedSizeDialogHint|Qt::WindowStaysOnTopHint);
    setWindowFlags(flags);
    setWindowModality(Qt::WindowModal);
    _pUi->setupUi(this);

    // set current values
    if( pAnnot )
    {
        _pUi->pThickness->setValue(pAnnot->thickness.value());
        _pUi->pWidth->setValue(pAnnot->boundingBox.value().width());
    }

    _pUi->pWidth->setAccelerated(true);
    _pUi->pThickness->setAccelerated(true);

    QObject::connect(_pUi->pThickness, SIGNAL(valueChanged(int)), this, SLOT(onThicknessChanged(int)));
    QObject::connect(_pUi->pWidth, SIGNAL(valueChanged(int)), this, SLOT(onWidthChanged(int)));
    QObject::connect(_pUi->pCenter, SIGNAL(clicked()), this, SLOT(onCentralise()));
}

//------------------------------------------------------------------------------
CrosshairAnnotationEditUi::~CrosshairAnnotationEditUi()
//------------------------------------------------------------------------------
{
    delete _pUi;
}

//------------------------------------------------------------------------------
void CrosshairAnnotationEditUi::onEditId(const QString& t)
//------------------------------------------------------------------------------
{
    if( _pAnnotation )
    {
        _pAnnotation->name = t;
    }
}

//------------------------------------------------------------------------------
void CrosshairAnnotationEditUi::onThicknessChanged(int i)
//------------------------------------------------------------------------------
{
    if( _pAnnotation )
    {
        _pAnnotation->thickness = i;
    }
}

//------------------------------------------------------------------------------
void CrosshairAnnotationEditUi::onWidthChanged(int w)
//------------------------------------------------------------------------------
{
    if( _pAnnotation )
    {
        QRectF bb = _pAnnotation->boundingBox.value();
        QPointF pos = bb.center();
        _pAnnotation->boundingBox = QRectF(pos.x() - w/2.f, pos.y() - w/2.f, w, w);
    }
}


//------------------------------------------------------------------------------
void CrosshairAnnotationEditUi::onCentralise()
//------------------------------------------------------------------------------
{
    if( _pAnnotation )
    {
        qreal w = _pAnnotation->boundingBox.value().width();
        _pAnnotation->boundingBox = QRectF((_imageSize.width() - w)/2.f, (_imageSize.height() - w)/2.f, w, w);
    }
}

//==============================================================================
ImageAnnotationEditUi::ImageAnnotationEditUi( ImageAnnotation* pAnnot,
                                                         QWidget* pParent)
//==============================================================================
: QDialog(pParent), _pAnnotation(pAnnot), _pUi(new Ui::ImageAnnotationEditDialog)
{
    Qt::WindowFlags flags = windowFlags();
    flags &= ~Qt::WindowMinimizeButtonHint;
    flags &= ~Qt::WindowContextHelpButtonHint;
    flags |= (Qt::MSWindowsFixedSizeDialogHint|Qt::WindowStaysOnTopHint);
    setWindowFlags(flags);
    setWindowModality(Qt::WindowModal);
    _pUi->setupUi(this);

    // set current values
    if( pAnnot )
    {
        _pUi->pWidth->setValue(pAnnot->boundingBox.value().width());
        _pUi->pHeight->setValue(pAnnot->boundingBox.value().height());
        _aspect = (float)_pUi->pWidth->value()/_pUi->pHeight->value();
    }
    _pUi->pWidth->setAccelerated(true);
    _pUi->pHeight->setAccelerated(true);

    QObject::connect(_pUi->pHeight, SIGNAL(valueChanged(int)), this, SLOT(onHeightChanged(int)));
    QObject::connect(_pUi->pWidth, SIGNAL(valueChanged(int)), this, SLOT(onWidthChanged(int)));
    QObject::connect(_pUi->pAspect, SIGNAL(stateChanged(int)), this, SLOT(onAspectChanged(int)));
}

//------------------------------------------------------------------------------
ImageAnnotationEditUi::~ImageAnnotationEditUi()
//------------------------------------------------------------------------------
{
    delete _pUi;
}

//------------------------------------------------------------------------------
void ImageAnnotationEditUi::onAspectChanged(int checked)
//------------------------------------------------------------------------------
{
    if( checked == Qt::Checked )
    {
        _aspect = (float)_pUi->pWidth->value()/_pUi->pHeight->value();
    }
}

//------------------------------------------------------------------------------
void ImageAnnotationEditUi::onHeightChanged(int h)
//------------------------------------------------------------------------------
{
    if( _pAnnotation )
    {
        QRectF bb = _pAnnotation->boundingBox.value();
        bb.setHeight(h);
        if( _pUi->pAspect->isChecked() )
        {
            QObject::disconnect(_pUi->pWidth, SIGNAL(valueChanged(int)), this, SLOT(onWidthChanged(int)));
            int w = qCeil(h * _aspect);
            _pUi->pWidth->setValue(w);
            bb.setWidth(w);
            QObject::connect(_pUi->pWidth, SIGNAL(valueChanged(int)), this, SLOT(onWidthChanged(int)));
        }
        _pAnnotation->boundingBox = bb;
    }
}

//------------------------------------------------------------------------------
void ImageAnnotationEditUi::onWidthChanged(int w)
//------------------------------------------------------------------------------
{
    if( _pAnnotation )
    {
        QRectF bb = _pAnnotation->boundingBox.value();
        bb.setWidth(w);
        if( _pUi->pAspect->isChecked() )
        {
            QObject::disconnect(_pUi->pHeight, SIGNAL(valueChanged(int)), this, SLOT(onHeightChanged(int)));
            int h = qCeil(w/_aspect);
            _pUi->pHeight->setValue(h);
            bb.setHeight(h);
            QObject::connect(_pUi->pHeight, SIGNAL(valueChanged(int)), this, SLOT(onHeightChanged(int)));
        }
        _pAnnotation->boundingBox = bb;
    }
}


//==============================================================================
ImageAnnotationSelectUi::ImageAnnotationSelectUi( ImageAnnotation* pAnnot,
                                                             QWidget* pParent)
//==============================================================================
: QDialog(pParent), _pAnnotation(pAnnot), _pUi(new Ui::ImageAnnotationSelectDialog)
{
    Qt::WindowFlags flags = windowFlags();
    flags &= ~Qt::WindowMinimizeButtonHint;
    flags &= ~Qt::WindowContextHelpButtonHint;
    flags |= (/*Qt::MSWindowsFixedSizeDialogHint|*/Qt::WindowStaysOnTopHint);
    setWindowFlags(flags);
    setWindowModality(Qt::WindowModal);
    _pUi->setupUi(this);

    _fsModel.setRootPath(QDir::currentPath());
    _pUi->pDirTreeView->setModel(&_fsModel);
    _pUi->pDirTreeView->scrollTo(_fsModel.index(QDir::currentPath()));
    _pUi->pDirTreeView->resizeColumnToContents(0);

    // Demonstrating look and feel features
    _pUi->pDirTreeView->setAnimated(false);
    _pUi->pDirTreeView->setIndentation(20);
    _pUi->pDirTreeView->setSortingEnabled(true);

    // set current values
    if( pAnnot )
    {
        _pUi->pWidth->setValue(pAnnot->boundingBox.value().width());
        _pUi->pHeight->setValue(pAnnot->boundingBox.value().height());
        _aspect = (float)_pUi->pWidth->value()/_pUi->pHeight->value();
    }
    _pUi->pWidth->setAccelerated(true);
    _pUi->pHeight->setAccelerated(true);

    QObject::connect(_pUi->pDirTreeView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(onItemClicked(const QModelIndex&)));
    QObject::connect(_pUi->pHeight, SIGNAL(valueChanged(int)), this, SLOT(onHeightChanged(int)));
    QObject::connect(_pUi->pWidth, SIGNAL(valueChanged(int)), this, SLOT(onWidthChanged(int)));
    QObject::connect(_pUi->pAspect, SIGNAL(stateChanged(int)), this, SLOT(onAspectChanged(int)));
}

//------------------------------------------------------------------------------
ImageAnnotationSelectUi::~ImageAnnotationSelectUi()
//------------------------------------------------------------------------------
{
    delete _pUi;
}

//------------------------------------------------------------------------------
void ImageAnnotationSelectUi::onItemClicked(const QModelIndex& index)
//------------------------------------------------------------------------------
{
    QString fileName = _fsModel.filePath(index);
    QImage image(fileName);
    if( image.isNull() )
    {
        return;
    }
    _pUi->pPreview->setPixmap( QPixmap::fromImage(image.scaled(_pUi->pPreview->size(), Qt::KeepAspectRatio)) );
    _aspect = (float)image.width()/image.height();
    _pUi->pHeight->setValue(image.height());
    _pUi->pWidth->setValue(image.width());
    if( _pAnnotation )
    {
        _pAnnotation->image = image;
    }
}

//------------------------------------------------------------------------------
void ImageAnnotationSelectUi::onAspectChanged(int checked)
//------------------------------------------------------------------------------
{
    if( checked == Qt::Checked )
    {
        _aspect = (float)_pUi->pWidth->value()/_pUi->pHeight->value();
    }
}

//------------------------------------------------------------------------------
void ImageAnnotationSelectUi::onHeightChanged(int h)
//------------------------------------------------------------------------------
{
    if( _pAnnotation )
    {
        QRectF bb = _pAnnotation->boundingBox.value();
        bb.setHeight(h);
        if( _pUi->pAspect->isChecked() )
        {
            QObject::disconnect(_pUi->pWidth, SIGNAL(valueChanged(int)), this, SLOT(onWidthChanged(int)));
            int w = qCeil(h * _aspect);
            _pUi->pWidth->setValue(w);
            bb.setWidth(w);
            QObject::connect(_pUi->pWidth, SIGNAL(valueChanged(int)), this, SLOT(onWidthChanged(int)));
        }
        _pAnnotation->boundingBox = bb;
    }
}

//------------------------------------------------------------------------------
void ImageAnnotationSelectUi::onWidthChanged(int w)
//------------------------------------------------------------------------------
{
    if( _pAnnotation )
    {
        QRectF bb = _pAnnotation->boundingBox.value();
        bb.setWidth(w);
        if( _pUi->pAspect->isChecked() )
        {
            QObject::disconnect(_pUi->pHeight, SIGNAL(valueChanged(int)), this, SLOT(onHeightChanged(int)));
            int h = qCeil(w/_aspect);
            _pUi->pHeight->setValue(h);
            bb.setHeight(h);
            QObject::connect(_pUi->pHeight, SIGNAL(valueChanged(int)), this, SLOT(onHeightChanged(int)));
        }
        _pAnnotation->boundingBox = bb;
    }
}

//==============================================================================
TextAnnotationAttributesUi::TextAnnotationAttributesUi(TextAnnotation& annot, QWidget* pParent)
: QWidget(pParent), _font(annot.font), _pen(annot.pen), _flags(annot.alignFlags), _pUi(new Ui::TextAnnotationAttributesWidget)
//==============================================================================
{
    _pUi->setupUi(this);

    _pUi->pSize->setAccelerated(true);

    QObject::connect(_pUi->pFont, SIGNAL(currentFontChanged(const QFont&)), this, SLOT(onFontChanged(const QFont&)));
    QObject::connect(_pUi->pSize, SIGNAL(valueChanged(int)), this, SLOT(onSizeChanged(int)));
    QObject::connect(_pUi->pBold, SIGNAL(toggled(bool)), this, SLOT(onBold(bool)));
    QObject::connect(_pUi->pItalic, SIGNAL(toggled(bool)), this, SLOT(onItalic(bool)));
    QObject::connect(_pUi->pColor, SIGNAL(clicked()), this, SLOT(onColorDialog()));
    QObject::connect(_pUi->pLeftAlign, SIGNAL(clicked()), this, SLOT(onLeftAlign()));
    QObject::connect(_pUi->pCenterAlign, SIGNAL(clicked()), this, SLOT(onCenterAlign()));
    QObject::connect(_pUi->pRightAlign, SIGNAL(clicked()), this, SLOT(onRightAlign()));

    if( _flags & Qt::AlignLeft)
    {
        _pUi->pLeftAlign->setChecked(true);
    }
    else if( _flags & Qt::AlignCenter)
    {
        _pUi->pCenterAlign->setChecked(true);
    }
    if( _flags & Qt::AlignRight)
    {
        _pUi->pRightAlign->setChecked(true);
    }
    _pUi->pFont->setCurrentFont(_font);
    _pUi->pSize->setValue(_font.pointSize());
    _pUi->pBold->setChecked(_font.bold());
    _pUi->pItalic->setChecked(_font.italic());

    _pUi->pSelectedColor->setAutoFillBackground(true);
    QPalette p = _pUi->pSelectedColor->palette();
    p.setColor(QPalette::Window, _pen.color());
    _pUi->pSelectedColor->setPalette(p);
}

//------------------------------------------------------------------------------
TextAnnotationAttributesUi::~TextAnnotationAttributesUi()
//------------------------------------------------------------------------------
{
    delete _pUi;
}

//------------------------------------------------------------------------------
void TextAnnotationAttributesUi::onFontChanged(const QFont& f)
//------------------------------------------------------------------------------
{
    _font.setFamily(f.family());
    emit fontPropertiesChanged();
}

//------------------------------------------------------------------------------
void TextAnnotationAttributesUi::onSizeChanged(int s)
//------------------------------------------------------------------------------
{
    _font.setPointSize(s);
    emit fontPropertiesChanged();
}

//------------------------------------------------------------------------------
void TextAnnotationAttributesUi::onBold(bool b)
//------------------------------------------------------------------------------
{
    _font.setBold(b);
    emit fontPropertiesChanged();
}

//------------------------------------------------------------------------------
void TextAnnotationAttributesUi::onItalic(bool b)
//------------------------------------------------------------------------------
{
    _font.setItalic(b);
    emit fontPropertiesChanged();
}

//------------------------------------------------------------------------------
void TextAnnotationAttributesUi::onColorDialog()
//------------------------------------------------------------------------------
{
    QColorDialog* pDialog = new QColorDialog(_pen.color(), this);
    pDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    QObject::connect(pDialog, SIGNAL(colorSelected(const QColor&)), this, SLOT(onColorSelected(const QColor&)));
    pDialog->show();
}

//------------------------------------------------------------------------------
void TextAnnotationAttributesUi::onColorSelected(const QColor& c)
//------------------------------------------------------------------------------
{
    _pen.setColor(c);
    QPalette p = _pUi->pSelectedColor->palette();
    p.setColor(QPalette::Window, _pen.color());
    _pUi->pSelectedColor->setPalette(p);
}

//------------------------------------------------------------------------------
void TextAnnotationAttributesUi::onLeftAlign()
//------------------------------------------------------------------------------
{
    _flags |= Qt::AlignTop;
    _flags &= ~Qt::AlignHCenter;
    _flags &= ~Qt::AlignRight;
    _flags |= Qt::AlignLeft;
    _flags &= ~Qt::AlignJustify;
}

//------------------------------------------------------------------------------
void TextAnnotationAttributesUi::onCenterAlign()
//------------------------------------------------------------------------------
{
    _flags |= Qt::AlignTop;
    _flags |= Qt::AlignHCenter;
    _flags &= ~Qt::AlignRight;
    _flags &= ~Qt::AlignLeft;
    _flags &= ~Qt::AlignJustify;
}

//------------------------------------------------------------------------------
void TextAnnotationAttributesUi::onRightAlign()
//------------------------------------------------------------------------------
{
    _flags |= Qt::AlignTop;
    _flags &= ~Qt::AlignHCenter;
    _flags |= Qt::AlignRight;
    _flags &= ~Qt::AlignLeft;
    _flags &= ~Qt::AlignJustify;
}

//==============================================================================
TextAnnotationEntryUi::TextAnnotationEntryUi( TextAnnotation& annot, QWidget* pParent)
//==============================================================================
: QWidget(pParent), _text(annot.text), _pUi(new Ui::TextAnnotationEntryWidget)
{
    _pUi->setupUi(this);
    _pUi->pText->setPlainText(annot.text);
    _pUi->pText->setWordWrapMode(QTextOption::WordWrap);
    QObject::connect(_pUi->pText, SIGNAL(textChanged()), this, SLOT(onTextChanged()));
}

//------------------------------------------------------------------------------
TextAnnotationEntryUi::~TextAnnotationEntryUi()
//------------------------------------------------------------------------------
{
    delete _pUi;
}

//------------------------------------------------------------------------------
void TextAnnotationEntryUi::onTextChanged()
//------------------------------------------------------------------------------
{
    _text = _pUi->pText->toPlainText();
    emit textChanged();
}

//==============================================================================
TimeAnnotationFormatUi::TimeAnnotationFormatUi( TimeAnnotation& annot, QWidget* pParent)
//==============================================================================
: QWidget(pParent), _format(annot.format), _pUi(new Ui::TimestampAnnotationFormatWidget),
_timeFmt("hh:mm:ss"), _dateFmt("dd MMM yyyy ")
{
    _pUi->setupUi(this);
    parseFormat(annot.format);
    QObject::connect(_pUi->pShowMs, SIGNAL(toggled(bool)), this, SLOT(onShowMs(bool)));
    QObject::connect(_pUi->pShowDate, SIGNAL(toggled(bool)), this, SLOT(onShowDate(bool)));
}

//------------------------------------------------------------------------------
TimeAnnotationFormatUi::~TimeAnnotationFormatUi()
//------------------------------------------------------------------------------
{
    delete _pUi;
}

//------------------------------------------------------------------------------
void TimeAnnotationFormatUi::parseFormat(const QString& fmt)
//------------------------------------------------------------------------------
{
    bool b = fmt.indexOf("zzz") != -1;
    _pUi->pShowMs->setChecked(b);
    _timeFmt = (b)?("hh:mm:ss.zzz"):("hh:mm:ss");

    b = fmt.indexOf("dd") != -1;
    _pUi->pShowDate->setChecked(b);
    _dateFmt = (b)?("dd MMM yyyy "):(" ");

    _format = _dateFmt + _timeFmt;
    emit formatChanged();
}

//------------------------------------------------------------------------------
void TimeAnnotationFormatUi::onShowMs(bool b)
//------------------------------------------------------------------------------
{
    _timeFmt = (b)?("hh:mm:ss.zzz"):("hh:mm:ss");
    _format = _dateFmt + _timeFmt;
    emit formatChanged();
}

//------------------------------------------------------------------------------
void TimeAnnotationFormatUi::onShowDate(bool b)
//------------------------------------------------------------------------------
{
    _dateFmt = (b)?("dd MMM yyyy "):(" ");
    _format = _dateFmt + _timeFmt;
    emit formatChanged();
}

//==============================================================================
CounterAnnotationFormatUi::CounterAnnotationFormatUi( CounterAnnotation* pAnnot, QWidget* pParent)
//==============================================================================
: QWidget(pParent), _pAnnotation(pAnnot), _pUi(new Ui::CounterAnnotationFormatWidget)
{
    _pUi->setupUi(this);

    QObject::connect(_pUi->pReset, SIGNAL(clicked()), this, SLOT(onReset()));
    QObject::connect(_pUi->pFieldWidth, SIGNAL(valueChanged(int)), this, SLOT(onWidthChanged(int)));
    QObject::connect(_pUi->pPadding, SIGNAL(toggled(bool)), this, SLOT(onFillChar(bool)));

    if( pAnnot )
    {
        _pUi->pFieldWidth->setValue(pAnnot->fieldWidth);
        _pUi->pPadding->setChecked(pAnnot->fillChar.unicode() == QChar('0').unicode());
    }
}

//------------------------------------------------------------------------------
CounterAnnotationFormatUi::~CounterAnnotationFormatUi()
//------------------------------------------------------------------------------
{
    delete _pUi;
}

//------------------------------------------------------------------------------
void CounterAnnotationFormatUi::onReset()
//------------------------------------------------------------------------------
{
    if( _pAnnotation )
    {
        _pAnnotation->count = 0;
        emit formatChanged();
    }
}

//------------------------------------------------------------------------------
void CounterAnnotationFormatUi::onWidthChanged(int i)
//------------------------------------------------------------------------------
{
    if( _pAnnotation )
    {
        _pAnnotation->fieldWidth = i;
        emit formatChanged();
    }
}

//------------------------------------------------------------------------------
void CounterAnnotationFormatUi::onFillChar(bool b)
//------------------------------------------------------------------------------
{
    if( _pAnnotation )
    {
        if( b )
        {
            _pAnnotation->fillChar = QLatin1Char( '0' );
        }
        else
        {
            _pAnnotation->fillChar = QLatin1Char( ' ' );
        }
        emit formatChanged();
    }
}

//==============================================================================
TextAnnotationEditUi::TextAnnotationEditUi(TextAnnotation* pAnnot, QWidget* pParent)
//==============================================================================
: QDialog(pParent), _pAnnotation(pAnnot)
{
    Qt::WindowFlags flags = windowFlags();
    flags &= ~Qt::WindowMinimizeButtonHint;
    flags &= ~Qt::WindowContextHelpButtonHint;
    flags |= (/*Qt::MSWindowsFixedSizeDialogHint|*/Qt::WindowStaysOnTopHint);
    setWindowFlags(flags);
    setWindowModality(Qt::WindowModal);

    // text annot
    if( pAnnot )
    {
        QVBoxLayout* pLayout = new QVBoxLayout;
        QString annotType = pAnnot->metaObject()->className();
        if( annotType == "Grape::TextAnnotation" )
        {
            TextAnnotationEntryUi* pEntry = new TextAnnotationEntryUi(*pAnnot, 0);
            QObject::connect(pEntry, SIGNAL(textChanged()), this, SLOT(recalculateBoundingBox()));
            pLayout->addWidget(pEntry);
        }

        else if ( annotType == "Grape::TimeAnnotation" )
        {
            TimeAnnotationFormatUi* pEntry = new TimeAnnotationFormatUi(dynamic_cast<TimeAnnotation&>(*pAnnot), 0);
            QObject::connect(pEntry, SIGNAL(formatChanged()), this, SLOT(recalculateBoundingBox()));
            pLayout->addWidget(pEntry);
        }

        else if ( annotType == "Grape::CounterAnnotation" )
        {
            CounterAnnotationFormatUi* pEntry = new CounterAnnotationFormatUi(dynamic_cast<CounterAnnotation*>(pAnnot), 0);
            QObject::connect(pEntry, SIGNAL(formatChanged()), this, SLOT(recalculateBoundingBox()));
            pLayout->addWidget(pEntry);
        }

        TextAnnotationAttributesUi* pAttr = new TextAnnotationAttributesUi(*pAnnot, 0);
        QObject::connect(pAttr, SIGNAL(fontPropertiesChanged()), this, SLOT(recalculateBoundingBox()));

        pLayout->addWidget(pAttr);
        setLayout(pLayout);
        recalculateBoundingBox();
    }
}

//------------------------------------------------------------------------------
TextAnnotationEditUi::~TextAnnotationEditUi()
//------------------------------------------------------------------------------
{
}

//------------------------------------------------------------------------------
void TextAnnotationEditUi::recalculateBoundingBox()
//------------------------------------------------------------------------------
{
    if( !_pAnnotation )
    {
        return;
    }

    QFontMetrics metrics(_pAnnotation->font);
    QRectF bound;
    QString annotType = _pAnnotation->metaObject()->className();
    if( annotType == "Grape::TextAnnotation" )
    {
        bound = metrics.boundingRect(QRect(0, 0, 10000, 0), _pAnnotation->alignFlags, _pAnnotation->text);
    }
    else if ( annotType == "Grape::TimeAnnotation" )
    {
        TimeAnnotation* pAnnot = dynamic_cast<TimeAnnotation*>(_pAnnotation);
        if( pAnnot )
        {
            bound = metrics.boundingRect(QRect(0, 0, 10000, 0), _pAnnotation->alignFlags, pAnnot->format);
        }
    }
    else if ( annotType == "Grape::CounterAnnotation" )
    {
        CounterAnnotation* pAnnot = dynamic_cast<CounterAnnotation*>(_pAnnotation);
        if( pAnnot )
        {
            bound = metrics.boundingRect(QRect(0, 0, 10000, 0), _pAnnotation->alignFlags, QString("%1").arg(pAnnot->count.value(), pAnnot->fieldWidth, 10, QChar('8')));
        }
    }

    bound.moveTopLeft(_pAnnotation->boundingBox.value().topLeft());
    _pAnnotation->boundingBox = bound;
}

} //Grape
