#include "../../../Scrolls/NotesScrollArea.h"
#include "../../../TuningGroup.h"
#include "../NotesArea.h"

#include <QScrollBar>

bool NotesArea::isSelecting() const {
    return m_selecting;
}

void NotesArea::initSelectModules() {
    m_selector = new GraphicsRubber(this);
    addItem(m_selector);

    m_selector->setZValue(rubberBandZIndex);
    m_selector->setActive(false);

    connect(m_selector, &GraphicsRubber::selecting, this, &NotesArea::duringSelecting);
    connect(m_selector, &GraphicsRubber::selectOver, this, &NotesArea::stopSelecting);
}

void NotesArea::startSelecting(bool vertical) {
    m_selector->setVertical(vertical);
    m_selector->setStartPoint(mousePos());
    m_selector->setActive(true); // Start selecting

    m_selector->grabMouse();
}

void NotesArea::duringSelecting() {
    NotesScrollArea *scroll = view();
    QPoint globalCursor = QCursor::pos();
    QPoint scrollCursor = scroll->viewport()->mapFromGlobal(globalCursor);
    QScrollBar *bar;

    m_selector->setEndPoint(mousePos());

    int value, toValue;
    int offset;
    double ratio = 0.5;
    QRect frameSize(QPoint(20, 20), QPoint(120, 20));

    bar = scroll->horizontalScrollBar();
    value = bar->value();

    if (scrollCursor.x() > scroll->viewport()->width() - frameSize.right()) {
        offset = scrollCursor.x() - (scroll->viewport()->width() - frameSize.right());
        toValue = value + offset * ratio;
        bar->setValue(toValue);
    } else if (scrollCursor.x() < frameSize.left()) {
        offset = scrollCursor.x() - frameSize.left();
        toValue = value + offset * ratio;
        bar->setValue(toValue);
    }

    bar = scroll->verticalScrollBar();
    value = bar->value();

    if (scrollCursor.y() > scroll->viewport()->height() - frameSize.bottom()) {
        offset = scrollCursor.y() - (scroll->viewport()->height() - frameSize.bottom());
        toValue = value + offset * ratio;
        bar->setValue(toValue);
    } else if (scrollCursor.y() < frameSize.top()) {
        offset = scrollCursor.y() - frameSize.top();
        toValue = value + offset * ratio;
        bar->setValue(toValue);
    }
}

void NotesArea::stopSelecting() {
    if (m_selector->active()) {
        m_selector->ungrabMouse();

        analyseSelect();
        m_selector->setActive(false); // Stop selecting
    }
    m_selecting = false;
}

void NotesArea::analyseSelect() {
    int start, end, i, j;

    GraphicsNote *note;
    ScopeHandler *scope;
    GraphicsPoint *point;

    QList<GraphicsNote *> findNotes;
    QList<GraphicsPoint *> findPoints;

    start = 0;
    end = NotesList.size() - 1;

    // Find selected notes' region
    while (start < NotesList.size() &&
           m_selector->x() > NotesList[start]->x() + NotesList[start]->width()) {
        ++start;
    }
    while (end >= 0 && m_selector->x() + m_selector->rect().width() < NotesList[end]->width()) {
        --end;
    }
    if (start > end) {
        return;
    }

    // Add selected notes to temp list
    for (i = start; i <= end; ++i) {
        note = NotesList[i];
        if (m_selector->collidesWithItem(note)) {
            findNotes.push_back(note);
        }
    }

    // Adjust range
    start = (start > 0) ? (start - 1) : start;
    end = (end < NotesList.size() - 1) ? (end + 1) : end;

    if (!m_notesEnabled) {
        // Find points
        for (i = start; i <= end; ++i) {
            note = NotesList[i];
            if (m_pitchesEnabled) {
                scope = note->curves();
            } else {
                scope = note->envelope();
            }
            for (j = 0; j < scope->Points.size(); ++j) {
                point = scope->Points[j];
                if (m_selector->contains(m_selector->mapFromScene(point->pos()))) {
                    findPoints.push_back(point);
                }
            }
        }
    }

    // Select points
    for (i = 0; i < findPoints.size(); ++i) {
        qDragOut.addOne(findPoints[i]);
    }

    // Select notes
    for (i = 0; i < findNotes.size(); ++i) {
        qDragOut.addOne(findNotes[i]);
    }
}
