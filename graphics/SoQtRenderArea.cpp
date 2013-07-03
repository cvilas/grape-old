//==============================================================================
// Project  : Grape
// Module   : Graphics
// File     : SoQtRenderArea.cpp
//
// Note     :   This file is a derivative work of 'SoViewer' library available at
//              http://code.google.com/p/openinventorviewer/. A BSD-style license
//              applies. See License.txt
//==============================================================================

#include "SoQtRenderArea.h"
#include <QTimerEvent>
#include <QWheelEvent>
#include <QtOpenGL>

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

namespace Grape
{

//==============================================================================
SoQtRenderArea::SoQtRenderArea(QObject* pParent)
//==============================================================================
: QGraphicsScene(pParent), SoRenderArea()
{
    commonInit();
}

//-------------------------------------------------------------------------------
SoQtRenderArea::~SoQtRenderArea()
//-------------------------------------------------------------------------------
{
}

//-------------------------------------------------------------------------------
void SoQtRenderArea::soRenderCallback()
//-------------------------------------------------------------------------------
{
    update();
}

//-----------------------------------------------------------------------------
void SoQtRenderArea::drawBackground(QPainter* pPainter, const QRectF&)
//-----------------------------------------------------------------------------
{
    enum QPaintEngine::Type painterType = pPainter->paintEngine()->type();
    if ( (painterType != QPaintEngine::OpenGL)
#if QT_VERSION >= 0x040600
        && (painterType != QPaintEngine::OpenGL2)
#endif
        )
    {
        qWarning("[SoQtRenderArea::drawBackground] Need a QGLWidget to be set as viewport on the graphics view");
        return;
    }

    saveGlState();

    //---- Render Inventor SceneGraph ------

    // Qt may have changed opengl attributes. Reset them for Coin Scenegraph traversal
    // ** Warning** I am not sure whether this is an efficient way to reclaim state, but
    // it seems to do the job for now.
    reinitialize();

    // Set blending attributes to default values
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    soPaintEvent(); // Draw the scene!
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_MULTISAMPLE);

    restoreGlState();
}

//-------------------------------------------------------------------------------
void SoQtRenderArea::resize(int w, int h)
//-------------------------------------------------------------------------------
{
    soResizeEvent(w, h);
}

//-------------------------------------------------------------------------------
void SoQtRenderArea::keyPressEvent(QKeyEvent* pEvent)
//-------------------------------------------------------------------------------
{
    QGraphicsScene::keyPressEvent(pEvent);
    if (pEvent->isAccepted())
        return;

    _pKeyboardEvent->setTime(SbTime(_time.elapsed() / 1000.0));
    _pKeyboardEvent->setKey( translateKey(pEvent) );
    _pKeyboardEvent->setState(SoButtonEvent::DOWN);
    _pKeyboardEvent->setShiftDown(pEvent->modifiers() & Qt::ShiftModifier);
    _pKeyboardEvent->setCtrlDown(pEvent->modifiers() & Qt::ControlModifier);
    _pKeyboardEvent->setAltDown(pEvent->modifiers() & Qt::AltModifier);
    soKeyPressEvent( _pKeyboardEvent );
}

//-------------------------------------------------------------------------------
void SoQtRenderArea::keyReleaseEvent(QKeyEvent* pEvent)
//-------------------------------------------------------------------------------
{
    QGraphicsScene::keyReleaseEvent(pEvent);
    if (pEvent->isAccepted())
        return;

    _pKeyboardEvent->setTime(SbTime(_time.elapsed() / 1000.0));
    _pKeyboardEvent->setKey( translateKey(pEvent) );
    _pKeyboardEvent->setState(SoButtonEvent::UP);
    _pKeyboardEvent->setShiftDown(pEvent->modifiers() & Qt::ShiftModifier);
    _pKeyboardEvent->setCtrlDown(pEvent->modifiers() & Qt::ControlModifier);
    _pKeyboardEvent->setAltDown(pEvent->modifiers() & Qt::AltModifier);
    soKeyReleaseEvent( _pKeyboardEvent );
}

//-------------------------------------------------------------------------------
void SoQtRenderArea::mouseMoveEvent(QGraphicsSceneMouseEvent* pEvent)
//-------------------------------------------------------------------------------
{
    QGraphicsScene::mouseMoveEvent(pEvent);
    if (pEvent->isAccepted())
        return;

    _pLocation2Event->setTime(SbTime(_time.elapsed() / 1000.0));
    QPointF pos = pEvent->scenePos();
    _pLocation2Event->setPosition(SbVec2s(pos.x(), height() - 1 - pos.y()));
    _pLocation2Event->setShiftDown(pEvent->modifiers() & Qt::ShiftModifier);
    _pLocation2Event->setCtrlDown(pEvent->modifiers() & Qt::ControlModifier);
    _pLocation2Event->setAltDown(pEvent->modifiers() & Qt::AltModifier);
    soMouseMoveEvent( _pLocation2Event );
}

//-------------------------------------------------------------------------------
void SoQtRenderArea::mousePressEvent(QGraphicsSceneMouseEvent* pEvent)
//-------------------------------------------------------------------------------
{
    QGraphicsScene::mousePressEvent(pEvent);
    if (pEvent->isAccepted())
        return;

    switch( pEvent->button() )
    {
    case Qt::LeftButton:
        _pMouseButtonEvent->setButton(SoMouseButtonEvent::BUTTON1);
        break;
    case Qt::MidButton:
        _pMouseButtonEvent->setButton(SoMouseButtonEvent::BUTTON2);
        break;
    case Qt::RightButton:
        _pMouseButtonEvent->setButton(SoMouseButtonEvent::BUTTON3);
        break;
    default:
        _pMouseButtonEvent->setButton(SoMouseButtonEvent::ANY);
        break;
    }
    _pMouseButtonEvent->setTime(SbTime(_time.elapsed() / 1000.0));
    _pMouseButtonEvent->setState(SoButtonEvent::DOWN);
    QPointF pos = pEvent->scenePos();
    _pMouseButtonEvent->setPosition(SbVec2s(pos.x(), height() - 1 - pos.y()));
    _pMouseButtonEvent->setShiftDown(pEvent->modifiers() & Qt::ShiftModifier);
    _pMouseButtonEvent->setCtrlDown(pEvent->modifiers() & Qt::ControlModifier);
    _pMouseButtonEvent->setAltDown(pEvent->modifiers() & Qt::AltModifier);
    soMousePressEvent( _pMouseButtonEvent );
}

//-------------------------------------------------------------------------------
void SoQtRenderArea::mouseReleaseEvent(QGraphicsSceneMouseEvent* pEvent)
//-------------------------------------------------------------------------------
{
    QGraphicsScene::mouseReleaseEvent(pEvent);
    if (pEvent->isAccepted())
        return;

    switch( pEvent->button() )
    {
    case Qt::LeftButton:
        _pMouseButtonEvent->setButton(SoMouseButtonEvent::BUTTON1);
        break;
    case Qt::MidButton:
        _pMouseButtonEvent->setButton(SoMouseButtonEvent::BUTTON2);
        break;
    case Qt::RightButton:
        _pMouseButtonEvent->setButton(SoMouseButtonEvent::BUTTON3);
        break;
    default:
        _pMouseButtonEvent->setButton(SoMouseButtonEvent::ANY);
        break;
    }
    _pMouseButtonEvent->setTime(SbTime(_time.elapsed() / 1000.0));
    _pMouseButtonEvent->setState(SoButtonEvent::UP);
    QPointF pos = pEvent->scenePos();
    _pMouseButtonEvent->setPosition(SbVec2s(pos.x(), height() - 1 - pos.y()));
    _pMouseButtonEvent->setShiftDown(pEvent->modifiers() & Qt::ShiftModifier);
    _pMouseButtonEvent->setCtrlDown(pEvent->modifiers() & Qt::ControlModifier);
    _pMouseButtonEvent->setAltDown(pEvent->modifiers() & Qt::AltModifier);
    soMouseReleaseEvent( _pMouseButtonEvent );
}

//-------------------------------------------------------------------------------
void SoQtRenderArea::wheelEvent(QGraphicsSceneWheelEvent* pEvent)
//-------------------------------------------------------------------------------
{
    QGraphicsScene::wheelEvent(pEvent);
    if (pEvent->isAccepted())
        return;

    _pWheelEvent->setTime(SbTime(_time.elapsed() / 1000.0));
    QPointF pos = pEvent->scenePos();
    _pWheelEvent->setPosition(SbVec2s(pos.x(), height() - 1 - pos.y()));
    _pWheelEvent->setShiftDown(pEvent->modifiers() & Qt::ShiftModifier);
    _pWheelEvent->setCtrlDown(pEvent->modifiers() & Qt::ControlModifier);
    _pWheelEvent->setAltDown(pEvent->modifiers() & Qt::AltModifier);
    _pWheelEvent->setDelta(pEvent->delta());
    soWheelEvent( _pWheelEvent );
}

//-------------------------------------------------------------------------------
void SoQtRenderArea::timerEvent( QTimerEvent* pEvent )
//-------------------------------------------------------------------------------
{
    if( pEvent->timerId() == _timerSensorId )
    {
        SoDB::getSensorManager()->processTimerQueue();

        // Timer sensors are set up to trigger at specific, absolute times.
        SbTime nextTimerSensor;
        if( SoDB::getSensorManager()->isTimerSensorPending(nextTimerSensor) )
        {
            int interval = (nextTimerSensor - SbTime::getTimeOfDay()).getMsecValue();
            killTimer(_timerSensorId);
            _timerSensorId = startTimer( (interval <= 0) ? 1 :  interval);
        }
    }

    else if( pEvent->timerId() == _delaySensorId )
    {
        // Delay sensors trigger when the application is otherwise idle.
        SoDB::getSensorManager()->processTimerQueue();
        SoDB::getSensorManager()->processDelayQueue(true);
    }

    else
    {
        QGraphicsScene::timerEvent(pEvent);
    }
}

//-------------------------------------------------------------------------------
void SoQtRenderArea::commonInit()
//-------------------------------------------------------------------------------
{
    _timerSensorId = QObject::startTimer( 1 );
    _delaySensorId = QObject::startTimer( 0 );
    _time.start();
}

//-----------------------------------------------------------------------------
void SoQtRenderArea::saveGlState()
//-----------------------------------------------------------------------------
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
}

//-----------------------------------------------------------------------------
void SoQtRenderArea::restoreGlState()
//-----------------------------------------------------------------------------
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glPopAttrib();
}

//-------------------------------------------------------------------------------
SoKeyboardEvent::Key SoQtRenderArea::translateKey( QKeyEvent * pEvent )
//-------------------------------------------------------------------------------
{
    SoKeyboardEvent::Key ev = SoKeyboardEvent::ANY;

    switch( pEvent->key() )
    {
    case Qt::Key_Escape: ev = SoKeyboardEvent::ESCAPE; break;
    case Qt::Key_Tab: ev = SoKeyboardEvent::TAB; break;
    case Qt::Key_Backspace: ev = SoKeyboardEvent::BACKSPACE; break;
    case Qt::Key_Return: ev = SoKeyboardEvent::RETURN; break;
    case Qt::Key_Enter: ev = SoKeyboardEvent::ENTER; break;
    case Qt::Key_Insert: ev = SoKeyboardEvent::INSERT; break;
        // Avoid problem with Microsoft Visual C++ Win32 API headers (yes,
        // they actually #define DELETE in their WINNT.H header file).
#ifdef DELETE
    case Qt::Key_Delete: ev = SoKeyboardEvent::KEY_DELETE; break;
#else
    case Qt::Key_Delete: ev = SoKeyboardEvent::DELETE; break;
#endif
    case Qt::Key_Pause: ev = SoKeyboardEvent::PAUSE; break;
    case Qt::Key_Print: ev = SoKeyboardEvent::PRINT; break;
    case Qt::Key_Home: ev = SoKeyboardEvent::HOME; break;
    case Qt::Key_End: ev = SoKeyboardEvent::END; break;
    case Qt::Key_Left: ev = SoKeyboardEvent::LEFT_ARROW; break;
    case Qt::Key_Up: ev = SoKeyboardEvent::UP_ARROW; break;
    case Qt::Key_Right: ev = SoKeyboardEvent::RIGHT_ARROW; break;
    case Qt::Key_Down: ev = SoKeyboardEvent::DOWN_ARROW; break;
    case Qt::Key_PageUp: ev = SoKeyboardEvent::PAGE_UP; break;
    case Qt::Key_PageDown: ev = SoKeyboardEvent::PAGE_DOWN; break;
    case Qt::Key_Shift: ev = SoKeyboardEvent::LEFT_SHIFT; break;
    case Qt::Key_Control: ev = SoKeyboardEvent::LEFT_CONTROL; break;
    case Qt::Key_Meta: ev = SoKeyboardEvent::LEFT_ALT; break;
    case Qt::Key_Alt: ev = SoKeyboardEvent::LEFT_ALT; break;
    case Qt::Key_CapsLock: ev = SoKeyboardEvent::CAPS_LOCK; break;
    case Qt::Key_NumLock: ev = SoKeyboardEvent::NUM_LOCK; break;
    case Qt::Key_ScrollLock: ev = SoKeyboardEvent::SCROLL_LOCK; break;
    case Qt::Key_F1: ev = SoKeyboardEvent::F1; break;
    case Qt::Key_F2: ev = SoKeyboardEvent::F2; break;
    case Qt::Key_F3: ev = SoKeyboardEvent::F3; break;
    case Qt::Key_F4: ev = SoKeyboardEvent::F4; break;
    case Qt::Key_F5: ev = SoKeyboardEvent::F5; break;
    case Qt::Key_F6: ev = SoKeyboardEvent::F6; break;
    case Qt::Key_F7: ev = SoKeyboardEvent::F7; break;
    case Qt::Key_F8: ev = SoKeyboardEvent::F8; break;
    case Qt::Key_F9: ev = SoKeyboardEvent::F9; break;
    case Qt::Key_F10: ev = SoKeyboardEvent::F10; break;
    case Qt::Key_F11: ev = SoKeyboardEvent::F11; break;
    case Qt::Key_F12: ev = SoKeyboardEvent::F12; break;
    case Qt::Key_Space: ev = SoKeyboardEvent::SPACE; break;
    case Qt::Key_Exclam: ev = SoKeyboardEvent::NUMBER_1; break;
    case Qt::Key_QuoteDbl: ev = SoKeyboardEvent::APOSTROPHE; break;
    case Qt::Key_NumberSign: ev = SoKeyboardEvent::NUMBER_3; break;
    case Qt::Key_Dollar: ev = SoKeyboardEvent::NUMBER_4; break;
    case Qt::Key_Percent: ev = SoKeyboardEvent::NUMBER_5; break;
    case Qt::Key_Ampersand: ev = SoKeyboardEvent::NUMBER_6; break;
    case Qt::Key_Apostrophe: ev = SoKeyboardEvent::APOSTROPHE; break;
    case Qt::Key_ParenLeft: ev = SoKeyboardEvent::NUMBER_9; break;
    case Qt::Key_ParenRight: ev = SoKeyboardEvent::NUMBER_0; break;
    case Qt::Key_Asterisk: ev = SoKeyboardEvent::NUMBER_8; break;
    case Qt::Key_Plus: ev = SoKeyboardEvent::PAD_ADD; break;
    case Qt::Key_Minus: ev = SoKeyboardEvent::PAD_SUBTRACT; break;
    case Qt::Key_Comma: ev = SoKeyboardEvent::COMMA; break;
    case Qt::Key_Period: ev = SoKeyboardEvent::PERIOD; break;
    case Qt::Key_Slash: ev = SoKeyboardEvent::SLASH; break;
    case Qt::Key_0: ev = SoKeyboardEvent::NUMBER_0; break;
    case Qt::Key_1: ev = SoKeyboardEvent::NUMBER_1; break;
    case Qt::Key_2: ev = SoKeyboardEvent::NUMBER_2; break;
    case Qt::Key_3: ev = SoKeyboardEvent::NUMBER_3; break;
    case Qt::Key_4: ev = SoKeyboardEvent::NUMBER_4; break;
    case Qt::Key_5: ev = SoKeyboardEvent::NUMBER_5; break;
    case Qt::Key_6: ev = SoKeyboardEvent::NUMBER_6; break;
    case Qt::Key_7: ev = SoKeyboardEvent::NUMBER_7; break;
    case Qt::Key_8: ev = SoKeyboardEvent::NUMBER_8; break;
    case Qt::Key_9: ev = SoKeyboardEvent::NUMBER_9; break;
    case Qt::Key_Colon: ev = SoKeyboardEvent::SEMICOLON; break;
    case Qt::Key_Semicolon: ev = SoKeyboardEvent::SEMICOLON; break;
    case Qt::Key_Less: ev = SoKeyboardEvent::COMMA; break;
    case Qt::Key_Equal: ev = SoKeyboardEvent::EQUAL; break;
    case Qt::Key_Greater: ev = SoKeyboardEvent::PERIOD; break;
    case Qt::Key_Question: ev = SoKeyboardEvent::BACKSLASH; break;
    case Qt::Key_At: ev = SoKeyboardEvent::NUMBER_2; break;
    case Qt::Key_A: ev = SoKeyboardEvent::A; break;
    case Qt::Key_B: ev = SoKeyboardEvent::B; break;
    case Qt::Key_C: ev = SoKeyboardEvent::C; break;
    case Qt::Key_D: ev = SoKeyboardEvent::D; break;
    case Qt::Key_E: ev = SoKeyboardEvent::E; break;
    case Qt::Key_F: ev = SoKeyboardEvent::F; break;
    case Qt::Key_G: ev = SoKeyboardEvent::G; break;
    case Qt::Key_H: ev = SoKeyboardEvent::H; break;
    case Qt::Key_I: ev = SoKeyboardEvent::I; break;
    case Qt::Key_J: ev = SoKeyboardEvent::J; break;
    case Qt::Key_K: ev = SoKeyboardEvent::K; break;
    case Qt::Key_L: ev = SoKeyboardEvent::L; break;
    case Qt::Key_M: ev = SoKeyboardEvent::M; break;
    case Qt::Key_N: ev = SoKeyboardEvent::N; break;
    case Qt::Key_O: ev = SoKeyboardEvent::O; break;
    case Qt::Key_P: ev = SoKeyboardEvent::P; break;
    case Qt::Key_Q: ev = SoKeyboardEvent::Q; break;
    case Qt::Key_R: ev = SoKeyboardEvent::R; break;
    case Qt::Key_S: ev = SoKeyboardEvent::S; break;
    case Qt::Key_T: ev = SoKeyboardEvent::T; break;
    case Qt::Key_U: ev = SoKeyboardEvent::U; break;
    case Qt::Key_V: ev = SoKeyboardEvent::V; break;
    case Qt::Key_W: ev = SoKeyboardEvent::W; break;
    case Qt::Key_X: ev = SoKeyboardEvent::X; break;
    case Qt::Key_Y: ev = SoKeyboardEvent::Y; break;
    case Qt::Key_Z: ev = SoKeyboardEvent::Z; break;
    case Qt::Key_BracketLeft: ev = SoKeyboardEvent::BRACKETLEFT; break;
    case Qt::Key_Backslash: ev = SoKeyboardEvent::BACKSLASH; break;
    case Qt::Key_BracketRight: ev = SoKeyboardEvent::BRACKETRIGHT; break;
    case Qt::Key_AsciiCircum: ev = SoKeyboardEvent::NUMBER_7; break;
    case Qt::Key_Underscore: ev = SoKeyboardEvent::MINUS; break;
    case Qt::Key_BraceLeft: ev = SoKeyboardEvent::BRACKETLEFT; break;
    case Qt::Key_Bar: ev = SoKeyboardEvent::BACKSLASH; break;
    case Qt::Key_BraceRight: ev = SoKeyboardEvent::BRACKETRIGHT; break;
    case Qt::Key_AsciiTilde: ev = SoKeyboardEvent::GRAVE; break;
    case Qt::Key_unknown: ev = SoKeyboardEvent::ANY; break;
    }
    return ev;
}

} // Grape
