#include "../../../Forms/EditorForm.h"
#include "../../../Scrolls/NotesScrollArea.h"
#include "../../../TuningGroup.h"
#include "../NotesArea.h"
#include "mainwindow.h"

void NotesArea::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItem *curItem = itemAt(event->scenePos(), QTransform());

    if (isLyricEditing() && curItem != m_lyricEdit) {
        editFinish();
    }

    if (curItem || qDragOut.dragging) {
        return GraphicsArea::mousePressEvent(event);
    } else {
        Qt::KeyboardModifiers c = QApplication::keyboardModifiers();
        if (c == qConfig->notes.sceneDragging) {
            m_moving = true;
            m_view->setDragMode(QGraphicsView::ScrollHandDrag);
        } else {
            if (c != qConfig->notes.reserveSelect) {
                qDragOut.removeAll();
            }
            if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton) {
                m_selecting = true;
            }
        }
    }
}

void NotesArea::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (m_selecting && !m_selector->active()) {
        if (event->buttons() & Qt::LeftButton) {
            startSelecting();
        } else if (event->buttons() & Qt::RightButton) {
            startSelecting(true);
        }
    }
    return GraphicsArea::mouseMoveEvent(event);
}

void NotesArea::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (m_moving) {
        m_moving = false;
        m_view->setDragMode(QGraphicsView::NoDrag);
    }
    m_selecting = false;
    GraphicsArea::mouseReleaseEvent(event);
    statusCall();
}

void NotesArea::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItem *curItem = itemAt(event->scenePos(), QTransform());
    if (curItem) {
        GraphicsDragger *obj;
        if (m_notesEnabled || ((obj = static_cast<GraphicsDragger *>(curItem)) &&
                               obj->element() != GraphicsDragger::Note)) {
            return GraphicsArea::mousePressEvent(event); // Pass to lower
        }
    }
    if (event->button() == Qt::LeftButton) {
        if (isPlaying()) {
            return;
        }
        // Left Double Click
        if (m_notesEnabled) {
            prepareDrawNote(event);
        } else if (m_pitchesEnabled || m_envelopesEnabled) {
            prepareDrawPoint(event);
        }
    } else if (event->button() == Qt::RightButton) {
        if (isPlaying()) {
            jumpPlaying(event->scenePos().x());
        }
    }
}

void NotesArea::leaveEvent(QEvent *event) {
    if (m_drawingItem) {
        switch (m_drawingItem->element()) {
        case GraphicsDragger::Note: {
            GraphicsNote *p = static_cast<GraphicsNote *>(m_drawingItem);
            p->afterRelease();
            break;
        }
        case GraphicsDragger::Point: {
            GraphicsPoint *p = static_cast<GraphicsPoint *>(m_drawingItem);
            p->afterRelease();
            break;
        }
        default:
            break;
        }
    }
}

bool NotesArea::eventFilter(QObject *obj, QEvent *event) {
    if (obj == m_lyricEdit && AppAssistant::keyIsDown(event)) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        int key = keyEvent->key();
        if (key == Qt::Key_Tab) {
            switchLyric();
            return true;
        } else if (key == Qt::Key_Escape) {
            abandonLyric();
            return true;
        } else if (key == Qt::Key_Enter || key == Qt::Key_Return) {
            editFinish();
            return true;
        }
    } else if (obj == this) {
        if (event->type() == AppAssistant::keyIsDown(event)) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            int key = keyEvent->key();
            if (key == Qt::Key_Tab) {
                qDragOut.removeAll();
            }
        } else if (event->type() == QEvent::Leave) {
            leaveEvent(event);
            if (event->isAccepted()) {
                return true;
            }
        }
    }
    return GraphicsArea::eventFilter(obj, event);
}
