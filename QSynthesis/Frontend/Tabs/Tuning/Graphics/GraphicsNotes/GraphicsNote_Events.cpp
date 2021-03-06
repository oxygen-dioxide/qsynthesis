#include "../../Editor/Areas/NotesArea.h"
#include "../../TuningGroup.h"
#include "../GraphicsNote.h"
#include "application.h"

void GraphicsNote::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    qDragIn.filter(m_element); // Deselect all draggers other than note
    afterPress();
    if (event->button() == Qt::RightButton) {
        ungrabMouse();
        openContextMenu();
    }
}

void GraphicsNote::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (!m_movable || m_editor->isPlaying()) {
        return;
    }
    if (event->buttons() & Qt::LeftButton) {
        afterMove(event->scenePos());
    }
}

void GraphicsNote::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (!m_movable || m_editor->isPlaying()) {
        return;
    }
    if (event->button() == Qt::LeftButton) {
        afterRelease();
    }
}

void GraphicsNote::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    if (!m_movable || m_editor->isPlaying()) {
        return;
    }
    if (event->button() == Qt::LeftButton) {
        m_editor->editNoteLyric(this);
    }
}

void GraphicsNote::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
}

void GraphicsNote::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
    if (m_movable) {
        setStretch(event->pos().x() > width() - 5);
    }
}

void GraphicsNote::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    if (m_stretch) {
        setStretch(false);
    }
    if (m_move || m_drawing) {
        afterRelease();
    }
}

void GraphicsNote::focusInEvent(QFocusEvent *event) {
    return GraphicsDragger::focusInEvent(event);
}

void GraphicsNote::focusOutEvent(QFocusEvent *event) {
    return GraphicsDragger::focusOutEvent(event);
}

void GraphicsNote::afterPress() {
    Qt::KeyboardModifiers c = QApplication::keyboardModifiers();
    // Stretch
    if (m_movable) {
        if (m_stretch) {
            qDragIn.removeAll();
            qDragIn.addOne(this);
            // Before stretching
            if (c == qConfig->stretchR) {
                qDragIn.stretching = Qs::RelativeStretch;
            } else {
                qDragIn.stretching = Qs::AbsoluteStretch;
            }
            return;
        }
    }
    bool selected = isSelected();
    // Shift
    if (c == qConfig->selectC) {
        qDragIn.addOne(this);
        m_editor->selectContinuously();
        return;
    }

    // Ctrl
    if (c == qConfig->selectR) {
        // Add or Remove
        if (selected) {
            qDragIn.removeOne(this);
        } else {
            qDragIn.addOne(this);
        }
    } else {
        if (!selected) {
            qDragIn.removeAll();
            qDragIn.addOne(this);
        }
    }
}

void GraphicsNote::afterMove(QPointF pos) {
    // First Move
    if (!m_move) {
        if (!isSelected()) {
            qDragIn.addOne(this);
        }
        if (m_stretch) {
            m_editor->prepareMove(this);

            qDragIn.startDrag(this, pos);
            qDragIn.startInvolve(this, pos);
        } else {
            m_editor->selectContinuously();
            m_editor->prepareMove(this);

            qDragIn.startDrag(this, pos);
            qDragIn.startInvolve(this, pos);
        }
    }

    if (m_move) {
        if (m_stretch) {
            qDragIn.duringDrag(this, pos); // Blank
            qDragIn.duringInvolve(this, pos);

            // Position Changes during a drag procedure
            if (m_sizeChanged) {
                m_editor->refreshMove(this);
            }
        } else {
            qDragIn.duringDrag(this, pos);
            qDragIn.duringInvolve(this, pos);

            // Position Changes during a drag procedure
            if (m_posChanged) {
                m_editor->refreshMove(this);
            }
        }
    }
}

void GraphicsNote::afterRelease() {
    if (m_move || m_drawing) {
        // Release the mouse
        if (m_drawing) {
            // Born
            // qDebug() << "Release mouse from Note";
            this->ungrabMouse();
            setDrawing(false);
            m_editor->afterDraw(this);
        } else {
            // Move
            m_editor->endMove(this);
        }

        if (m_stretch) {
            qDragIn.stopDrag(this); // Blank
            qDragIn.endInvolve(this);
            qDragIn.removeAllT(); // Remove all involved notes
        } else {
            qDragIn.stopDrag(this);
            qDragIn.endInvolve(this);
            qDragIn.removeAllT(); // Remove all involved notes
        }

        // Subsequent Modifications
        adjustComponents();
        if (m_next) {
            m_next->adjustComponents();
        }
    } else {
        m_editor->callForChange();
    }

    qDragIn.stretching = Qs::NoStretch;
}
