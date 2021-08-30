#include "../../../Interfaces/EditorInterface.h"
#include "../../../Interfaces/TracksInterface.h"
#include "../../../Operations/ImplicitOperation.h"
#include "../../../Operations/LengthOperation.h"
#include "../../../Operations/LyricsOperation.h"
#include "../../../Operations/MoveOperation.h"
#include "../../../Operations/PointsOperation.h"
#include "../../../Operations/ScatterOperation.h"
#include "../../../Operations/SequenceOperation.h"
#include "../../../Operations/TempoOperation.h"
#include "../../../TuningGroup.h"
#include "../NotesArea.h"

void NotesArea::saveOperation(NoteOperation *n, QString desc) {
    if (n->differ()) {
        n->setDesc(desc);
        m_ptrs->tab->addHistory(n);
    } else {
        qDebug() << "[NoteOperation]"
                 << "Addition Refused";
        delete n;
    }
}

bool NotesArea::handleOperation(NoteOperation *o, bool undo) {
    executeOperation(o, undo);
    return true;
}

void NotesArea::executeOperation(NoteOperation *n, bool undo) {
    if (n->next() && undo) {
        executeOperation(n->next(), undo);
    }

    NoteOperation::Type type = n->type();

    switch (type) {
    case NoteOperation::Move: {
        MoveOperation *m = static_cast<MoveOperation *>(n);
        int index = m->index(); // leftmost note of selection
        const QList<int> &vc = m->verticals();
        int f = undo ? -1 : 1;

        // If Undo, the movement should be opposite
        int movement = m->movement() * f;

        // If Undo, the left of selection is added by movement
        if (undo) {
            index += m->movement();
        }

        // Change noteNum
        bool verticalMove = false;
        for (int i = 0; i < vc.size(); ++i) {
            int verticalChange = vc[i] * f;
            NotesList[index + i]->Note.noteNum += verticalChange;
            if (verticalChange != 0) {
                verticalMove = true;
            }
        }

        // Move Selection
        QPoint range = QPoint(index, index + vc.size() - 1);

        // Move Notes Core
        moveNotes(range, movement);
        if (verticalMove) {
            int newIndex = index + movement;
            updateNotesStatus(QPoint(newIndex, newIndex + vc.size() - 1));
        }

        // Include Involved Notes
        if (movement > 0) {
            range.ry() += movement;
        } else {
            range.rx() += movement;
        }

        // Include Neighbor Notes
        range.rx()--;
        range.ry()++;

        // Adjust vision
        adjustNotes(range);

        lengthCall();
        break;
    }
    case NoteOperation::Length: {
        LengthOperation *l = static_cast<LengthOperation *>(n);
        const QList<int> &indexs = l->index();
        const QList<int> &orgs = l->origin();
        const QList<int> &news = l->modified();

        for (int i = 0; i < indexs.size(); ++i) {
            // Change Notes Core
            NotesList[indexs.at(i)]->Note.length = undo ? orgs.at(i) : news.at(i);
        }
        updateNoteTickAfter(indexs.front());

        // Adjust vision
        adjustNotes(QPoint(indexs.front(), -1));

        lengthCall();
        break;
    }
    case NoteOperation::Add:
    case NoteOperation::Remove: {
        SequenceOperation *s = static_cast<SequenceOperation *>(n);
        const QList<int> &indexs = s->index();
        const QList<QLinkNote> &notes = s->notes();
        int f = (type == NoteOperation::Add) ? 1 : -1;
        int f2 = undo ? -1 : 1;

        if (f * f2 > 0) {
            // Add
            insertNotes(indexs, notes);
        } else {
            // Remove
            removeNotes(indexs);
        }

        adjustNotes(QPoint(indexs.front(), -1));

        lengthCall();
        break;
    }
    case NoteOperation::Mode2:
    case NoteOperation::Envelope: {
        PointsOperation *m = static_cast<PointsOperation *>(n);
        const QList<int> &indexs = m->index();
        const QList<QList<QControlPoint>> &orgs = m->origin();
        const QList<QList<QControlPoint>> &news = m->modified();

        for (int i = 0; i < indexs.size(); ++i) {
            // Change Notes Core
            if (type == NoteOperation::Mode2) {
                NotesList[indexs.at(i)]->Note.Mode2Pitch = undo ? orgs.at(i) : news.at(i);
                NotesList[indexs.at(i)]->curves()->fromNotePoints();
            } else {
                NotesList[indexs.at(i)]->Note.envelope = undo ? orgs.at(i) : news.at(i);
                NotesList[indexs.at(i)]->envelope()->fromNotePoints();
            }
        }

        // Adjust vision
        adjustNotes(QPoint(indexs.front() - 1, indexs.back() + 1));
        break;
    }
    case NoteOperation::Mode1:
    case NoteOperation::Vibrato: {
        ScatterOperation *m = static_cast<ScatterOperation *>(n);
        const QList<int> &indexs = m->index();
        const QList<QList<double>> &orgs = m->origin();
        const QList<QList<double>> &news = m->modified();

        for (int i = 0; i < indexs.size(); ++i) {
            // Change Notes Core
            if (type == NoteOperation::Vibrato) {
                NotesList[indexs.at(i)]->Note.vibrato = undo ? orgs.at(i) : news.at(i);
                NotesList[indexs.at(i)]->curves()->fromNoteVibrato();
            } else {
                NotesList[indexs.at(i)]->Note.pitches = undo ? orgs.at(i) : news.at(i);
                NotesList[indexs.at(i)]->curves()->fromNotePitches();
            }
        }

        // Adjust vision
        qDebug() << indexs;
        adjustNotes(QPoint(indexs.front() - 1, indexs.back() + 1));
        break;
    }
    case NoteOperation::Lyrics: {
        LyricsOperation *l = static_cast<LyricsOperation *>(n);
        const QList<int> &indexs = l->index();
        const QStringList &orgs = l->origin();
        const QStringList &news = l->modified();

        for (int i = 0; i < indexs.size(); ++i) {
            // Change Notes Core
            NotesList[indexs.at(i)]->Note.lyric = undo ? orgs.at(i) : news.at(i);
        }

        // Adjust vision
        for (int i = 0; i < indexs.size(); ++i) {
            GraphicsNote *p = NotesList.at(indexs[i]);
            p->updateNoteStatus();
        }
        adjustNoteComponents(QPoint(indexs.front() - 1, indexs.back() + 1));
        break;
    }
    case NoteOperation::Intensity:
    case NoteOperation::Modulation:
    case NoteOperation::Velocity:
    case NoteOperation::StartPoint:
    case NoteOperation::PreUtterance:
    case NoteOperation::VoiceOverlap:
    case NoteOperation::Flags: {
        ImplicitOperation *l = static_cast<ImplicitOperation *>(n);
        const QList<int> &indexs = l->index();
        const QStringList &orgs = l->origin();
        const QStringList &news = l->modified();

        if (l->type() == NoteOperation::Intensity) {
            for (int i = 0; i < indexs.size(); ++i) {
                // Change Notes Core
                NotesList[indexs.at(i)]->Note.intensity =
                    undo ? orgs.at(i).toDouble() : news.at(i).toDouble();
            }
        } else if (l->type() == NoteOperation::Modulation) {
            for (int i = 0; i < indexs.size(); ++i) {
                // Change Notes Core
                NotesList[indexs.at(i)]->Note.modulation =
                    undo ? orgs.at(i).toDouble() : news.at(i).toDouble();
            }
        } else if (l->type() == NoteOperation::Velocity) {
            for (int i = 0; i < indexs.size(); ++i) {
                // Change Notes Core
                NotesList[indexs.at(i)]->Note.velocity =
                    undo ? orgs.at(i).toDouble() : news.at(i).toDouble();
            }
        } else if (l->type() == NoteOperation::StartPoint) {
            for (int i = 0; i < indexs.size(); ++i) {
                // Change Notes Core
                NotesList[indexs.at(i)]->Note.stp =
                    undo ? orgs.at(i).toDouble() : news.at(i).toDouble();
            }
        } else if (l->type() == NoteOperation::PreUtterance) {
            for (int i = 0; i < indexs.size(); ++i) {
                // Change Notes Core
                NotesList[indexs.at(i)]->Note.preUttr =
                    undo ? orgs.at(i).toDouble() : news.at(i).toDouble();
            }
        } else if (l->type() == NoteOperation::VoiceOverlap) {
            for (int i = 0; i < indexs.size(); ++i) {
                // Change Notes Core
                NotesList[indexs.at(i)]->Note.overlap =
                    undo ? orgs.at(i).toDouble() : news.at(i).toDouble();
            }
        } else if (l->type() == NoteOperation::Flags) {
            for (int i = 0; i < indexs.size(); ++i) {
                // Change Notes Core
                NotesList[indexs.at(i)]->Note.flags = undo ? orgs.at(i) : news.at(i);
            }
        }

        // Update Core
        for (int i = 0; i < indexs.size(); ++i) {
            GraphicsNote *p = NotesList.at(indexs[i]);
            p->updateCorrectGenon();
            p->updateNeighborCorrectGenons();
            p->lifter()->fromNoteProps();
        }
        updateNoteTickAfter(indexs.front());

        // Adjust vision
        adjustNotes(QPoint(indexs.front() - 1, indexs.back() + 1));
        break;
    }
    case NoteOperation::Tempo: {
        qDebug() << "Tempo";
        TempoOperation *t = static_cast<TempoOperation *>(n);
        int index = t->index();
        if (index < 0) {
            setGlobalTempo(undo ? t->origin() : t->modified());
        } else {
            double tempo = undo ? t->origin() : t->modified();
            bool status = undo ? t->orgStatus() : t->newStatus();
            NotesList[index]->setTempo(tempo);
            NotesList[index]->Note.tempo = status ? tempo : NODEF_DOUBLE;
        }
        updateNoteTempoAfter(index);
        updateNoteTimeAfter(index);

        adjustNoteComponents(QPoint(index - 1, -1));
        break;
    }
    default:
        break;
    }

    if (n->next() && !undo) {
        executeOperation(n->next(), undo);
    }
}

void NotesArea::statusCall() {
    m_ptrs->tab->change();
}

void NotesArea::lengthCall() {
    m_ptrs->editorContent->adjustCanvas(totalLength());
    m_ptrs->tracksContent->adjustDefaultTrack();
}

double NotesArea::globalTempo() const {
    return m_globalTempo;
}

void NotesArea::setGlobalTempo(double globalTempo) {
    m_globalTempo = globalTempo;
}
