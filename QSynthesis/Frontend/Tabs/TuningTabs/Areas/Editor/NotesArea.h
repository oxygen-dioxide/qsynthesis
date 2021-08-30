#ifndef NOTESAREA_H
#define NOTESAREA_H

#include <QGraphicsScene>
#include <QGraphicsSceneMoveEvent>
#include <QRubberBand>
#include <QWidget>

#include "../../Graphics/GraphicsLifter.h"
#include "../../Graphics/GraphicsNote.h"
#include "../../Graphics/GraphicsPoint.h"
#include "../../Graphics/GraphicsRubber.h"
#include "Graphics/GraphicsLineEdit.h"
#include "Note/QLinkNote.h"
#include "Plugin/PluginTempData.h"
#include "QUtils.h"
#include "Render/RenderArgs.h"
#include "Utils/ItemList.h"

class EditorInterface;
class TuningGroup;
class NoteOperation;
class NotesScrollArea;

class NotesArea : public QGraphicsScene {
    Q_OBJECT

    Q_PROPERTY(QColor timeLineColor READ timeLineColor WRITE setTimeLineColor NOTIFY colorChanged)
    Q_PROPERTY(
        QColor sectionLineColor READ sectionLineColor WRITE setSectionLineColor NOTIFY colorChanged)
    Q_PROPERTY(
        QColor pitchLineColor READ pitchLineColor WRITE setPitchLineColor NOTIFY colorChanged)
    Q_PROPERTY(
        QColor levelLineColor READ levelLineColor WRITE setLevelLineColor NOTIFY colorChanged)
    Q_PROPERTY(QColor backLowColor READ backLowColor WRITE setBackLowColor NOTIFY colorChanged)
    Q_PROPERTY(QColor backHighColor READ backHighColor WRITE setBackHighColor NOTIFY colorChanged)
public:
    explicit NotesArea(EditorInterface *editor, NotesScrollArea *parent = nullptr);
    virtual ~NotesArea();

    TuningGroup *ptrs() const;

    NotesScrollArea *view() const;

    ItemList<GraphicsNote> NotesList;

    const double noteZIndex = 1;
    const double pointZIndex = 2;
    const double linesZIndex = 3;
    const double vibratoZIndex = 4;
    const double lyricEditZIndex = 5;
    const double rubberBandZIndex = 6;
    const double playHeadZIndex = 7;

public:
    QColor timeLineColor() const;
    void setTimeLineColor(const QColor &timeLineColor);

    QColor sectionLineColor() const;
    void setSectionLineColor(const QColor &sectionLineColor);

    QColor pitchLineColor() const;
    void setPitchLineColor(const QColor &pitchLineColor);

    QColor levelLineColor() const;
    void setLevelLineColor(const QColor &levelLineColor);

    QColor backLowColor() const;
    void setBackLowColor(const QColor &backLowColor);

    QColor backHighColor() const;
    void setBackHighColor(const QColor &backHighColor);

private:
    TuningGroup *m_ptrs;

    NotesScrollArea *m_view;

    bool m_moving;

    GraphicsDragger *m_drawingItem;
    QPoint m_renderRange;

    double m_globalTempo;

    bool m_notesEnabled;
    bool m_pitchesEnabled;
    bool m_envelopesEnabled;

    QColor m_timeLineColor;
    QColor m_sectionLineColor;
    QColor m_pitchLineColor;
    QColor m_levelLineColor;
    QColor m_backLowColor;
    QColor m_backHighColor;

public:
    void saveOperation(NoteOperation *n, QString desc = "");  // Out
    bool handleOperation(NoteOperation *o, bool undo = true); // In

    void executeOperation(NoteOperation *n, bool undo);

    // Call
public:
    void statusCall();
    void lengthCall();

    double globalTempo() const;
    void setGlobalTempo(double globalTempo);

public:
    void adjustSize();
    void updateBackground();

    void centralizeVision(GraphicsNote *p); // Move vision to fit note
    void centralizeVision(int index);

    void showOnStage(QGraphicsRectItem *w, bool right = false);
    void showOnCenter(QGraphicsRectItem *w);

    void selectNote(GraphicsNote *p);

    void moveToStart();
    void moveToEnd();

public:
    void adjustNotes(QPoint range = QPoint(-1, -1));
    void adjustNoteComponents(QPoint range = QPoint(-1, -1));

    QPoint selectContinuously(bool selectAll = false);

    // Actions
public:
    void handleDelete();

    void inputNotes(const QList<QLinkNote> &notes);
    void pasteNotes(const QList<QLinkNote> &notes);

    void preparePlugin(PluginTempData &ns, bool all);
    void receivePlugin(const PluginTempData &ns, QPoint orgRegion);

    QList<RenderArgs> prepareRender();

    void removeSelectedNotes();
    void removeSelectedPoints();

    void shiftSelectedNotes(int offset);
    void modifySelectedNotes(const NoteProperties &orgValues, const NoteProperties &newValues);

    void insertLyrics(const QStringList &lyrics);
    void replaceSingleLyric(const QString &lyric, GraphicsNote *p);
    void replaceSelectedLyrics(const QStringList &lyrics, bool ignoreRest);

    void insertRest();
    void removeRestLessThan(int len);
    void modifySelectedTempo(double tempo);

    void modifySelectedEnvelope(Qs::EnvelopeBatch method);

private:
    GraphicsNote *insertNote(int index, const QLinkNote &note);
    GraphicsNote *appendNote(const QLinkNote &note); // *!*

    GraphicsNote *insertNotes(const QList<int> &indexs, const QList<QLinkNote> &notes); // *!*

    void removeNote(int index);
    void removeNote(GraphicsNote *p);

    void removeNotes(const QList<int> &indexs); // *!*
    void clearNotes();                          // *!*
    void removeAllNotes();                      // *!*

    // Variate Move
private:
    void moveNotes(QPoint range, int movement);

    // Const
public:
    double zeroLine() const;

    QPoint continuousSelection() const;
    int totalLength() const;

    QList<QLinkNote> allNotes() const;
    QList<QLinkNote> selectedNotes() const;

    QList<RenderArgs> allRenderArgs() const;

    QList<NoteProperties> selectedProperties() const;

    // Draw
public:
    GraphicsNote *insertNoteFromCursor(QPointF pos);
    void prepareDrawNote(QGraphicsSceneMouseEvent *event);

    GraphicsPoint *insertPointFromCursor(QPointF pos);
    void prepareDrawPoint(QGraphicsSceneMouseEvent *event);

    void removePointFromCursor(GraphicsPoint *p);
    void changePointFromCursor(GraphicsPoint *p);

    void addPortamento(GraphicsNote *p);
    void removePortamento(GraphicsNote *p);

    void addVibrato(GraphicsNote *p);
    void removeVibrato(GraphicsNote *p);

    // Move
public:
    void prepareMove(GraphicsNote *p);
    void refreshMove(GraphicsNote *p);

    void endMove(GraphicsNote *p);
    void afterDraw(GraphicsNote *p);

    // Points Slots
    void prepareMove(GraphicsPoint *p);
    void refreshMove(GraphicsPoint *p, QPointF pos);

    void endMove(GraphicsPoint *p);
    void afterDraw(GraphicsPoint *p);

    void afterModifyVibrato(GraphicsNote *p);

    void afterModifyLifters(GraphicsLifter::Props prop);

    // States
public:
    bool isMoving() const;
    bool isSelecting() const;
    bool isLyricEditing() const;
    bool isPlaying() const;

    void setNotesEnabled(bool enabled);
    void setPitchesEnabled(bool enabled);
    void setEnvelopesEnabled(bool enabled);

    // Update
public:
    void updateNoteTickAfter(int index);
    void updateNoteTimeAfter(int index);
    int updateNoteTempoAfter(int index, bool br = false); // Return the next tempo-edited index

    void updateNotesStatus(QPoint range = QPoint(-1, -1));

    // Assist
public:
    double tempoAt(int index) const;
    bool getGenonSettings(QString lrc, QGenonSettings *genon, int noteNum) const;

    QPointF convertValueToPosition(int tick, int noteNum) const;
    QRectF convertValueToGeometry(int tick, int noteNum, int length) const;

    QPair<int, int> convertPositionToValue(QPointF pos) const;
    double convertWidthToLength(int width) const;

    int findNoteAtPos(double x) const;
    int findNoteAtTick(int x) const;

    QRectF viewportRect() const;

    // Core
private:
    GraphicsNote *createNoteCore(const QLinkNote &note);
    void removeNoteCore(GraphicsNote *p);

    void centralizeVisionCore(int y);

    void savePointsCore(const ItemList<ScopeHandler> &list, bool mode2);
    void saveScatterCore(const ItemList<Mode2Handler> &list, bool vbr);

    QList<RenderArgs> getRenderArgsCore(QPoint range) const;

    // Lyric
private:
    GraphicsLineEdit *m_lyricEdit;
    GraphicsNote *m_curEditNote;

    void initLyricModules();

    void setCurEditNote(GraphicsNote *p);

    void editFinish();
    void editNextLyric();

    void switchLyric();
    void abandonLyric();

public:
    void editNoteLyric(GraphicsNote *pNote);
    GraphicsNote *curEditNote() const;

    // Select
private:
    GraphicsRubber *m_selector; // Frame Selector

    void initSelectModules();

    void startSelecting(bool vertical = false);
    void duringSelecting();
    void stopSelecting();

    void analyseSelect();

    // Play
private:
    QGraphicsRectItem *playHead;
    int playToNote;
    qint64 playToPosition;

    void initPlayModules();
    void adjustPlayHead();

public:
    void advancePlaying(qint64 position);
    void stopPlaying();

    // Events
private:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    void handleSceneRectChanged(const QRectF &rect);

signals:
    void colorChanged();
};

#endif // NOTESAREA_H
