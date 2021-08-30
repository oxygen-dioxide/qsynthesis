#include "WaveformArea.h"
#include "WaveformScrollArea.h"

#include <cmath>

WaveformArea::WaveformArea(WaveformScrollArea *parent) : QGraphicsScene(parent), m_view(parent) {
    m_index = -1;

    connect(this, &QGraphicsScene::sceneRectChanged, this, &WaveformArea::handleSceneRectChanged);

    btnOffset = new GraphicsOtoHandle(this);
    btnConstant = new GraphicsOtoHandle(this);
    btnBlank = new GraphicsOtoHandle(this);
    btnPreUttr = new GraphicsOtoHandle(this);
    btnOverlap = new GraphicsOtoHandle(this);

    offsetRect = new QGraphicsRectItem();
    constantRect = new QGraphicsRectItem();
    blankRect = new QGraphicsRectItem();

    offsetRect->setPen(Qt::NoPen);
    constantRect->setPen(Qt::NoPen);
    blankRect->setPen(Qt::NoPen);

    addItem(btnOffset);
    addItem(btnConstant);
    addItem(btnBlank);
    addItem(btnPreUttr);
    addItem(btnOverlap);

    addItem(offsetRect);
    addItem(constantRect);
    addItem(blankRect);

    btnOffset->setZValue(handleZIndex);
    btnConstant->setZValue(handleZIndex);
    btnBlank->setZValue(handleZIndex);
    btnPreUttr->setZValue(handleZIndex);
    btnOverlap->setZValue(handleZIndex);

    offsetRect->setZValue(regionZIndex);
    constantRect->setZValue(regionZIndex);
    blankRect->setZValue(regionZIndex);

    connect(btnOffset, &GraphicsOtoHandle::moved, this, &WaveformArea::handleOffsetMove);
    connect(btnConstant, &GraphicsOtoHandle::moved, this, &WaveformArea::handleConstantMove);
    connect(btnBlank, &GraphicsOtoHandle::moved, this, &WaveformArea::handleBlankMove);
    connect(btnPreUttr, &GraphicsOtoHandle::moved, this, &WaveformArea::handlePreUttrMove);
    connect(btnOverlap, &GraphicsOtoHandle::moved, this, &WaveformArea::handleOverlapMove);

    connect(btnOffset, &GraphicsOtoHandle::clicked, this, &WaveformArea::handleOffsetClicked);
    connect(btnConstant, &GraphicsOtoHandle::clicked, this, &WaveformArea::handleConstantClicked);
    connect(btnBlank, &GraphicsOtoHandle::clicked, this, &WaveformArea::handleBlankClicked);
    connect(btnPreUttr, &GraphicsOtoHandle::clicked, this, &WaveformArea::handlePreUttrClicked);
    connect(btnOverlap, &GraphicsOtoHandle::clicked, this, &WaveformArea::handleOverlapClicked);

    setOffsetColor(Qt::lightGray);
    setConstantColor(QColorConstants::Svg::pink);
    setBlankColor(QColorConstants::Svg::lightblue);
    setPreUttrColor(Qt::red);
    setOverlapColor(Qt::green);

    m_waveformColor = QColor(0, 110, 0);

    int alpha = 96;
    setOffsetBackColor(
        QColor(m_offsetColor.red(), m_offsetColor.green(), m_offsetColor.blue(), alpha));
    setConstantBackColor(
        QColor(m_constantColor.red(), m_constantColor.green(), m_constantColor.blue(), alpha));
    setBlankBackColor(QColor(m_blankColor.red(), m_blankColor.green(), m_blankColor.blue(), alpha));

    updateHandles();
}

WaveformArea::~WaveformArea() {
}

void WaveformArea::reset() {
    m_sample.clear();
    m_orgSample.clear();
    m_wavefile.reset();
    m_frqfile.reset();

    updateHandles();
    update();
}

double WaveformArea::duration() const {
    return m_wavefile.duration();
}

void WaveformArea::setWidth(int width) {
    setSceneRect(0, 0, width, sceneRect().height());
}

QColor WaveformArea::offsetColor() const {
    return m_offsetColor;
}

void WaveformArea::setOffsetColor(const QColor &offsetColor) {
    m_offsetColor = offsetColor;
    btnOffset->setLineColor(m_offsetColor);
}

QColor WaveformArea::constantColor() const {
    return m_constantColor;
}

void WaveformArea::setConstantColor(const QColor &constantColor) {
    m_constantColor = constantColor;
    btnConstant->setLineColor(m_constantColor);
}

QColor WaveformArea::blankColor() const {
    return m_blankColor;
}

void WaveformArea::setBlankColor(const QColor &blankColor) {
    m_blankColor = blankColor;
    btnBlank->setLineColor(m_blankColor);
}

QColor WaveformArea::preUttrColor() const {
    return m_preUttrColor;
}

void WaveformArea::setPreUttrColor(const QColor &preUttrColor) {
    m_preUttrColor = preUttrColor;
    btnPreUttr->setLineColor(m_preUttrColor);
}

QColor WaveformArea::overlapColor() const {
    return m_overlapColor;
}

void WaveformArea::setOverlapColor(const QColor &overlapColor) {
    m_overlapColor = overlapColor;
    btnOverlap->setLineColor(m_overlapColor);
}
QColor WaveformArea::offsetBackColor() const {
    return m_offsetBackColor;
}

void WaveformArea::setOffsetBackColor(const QColor &offsetBackColor) {
    m_offsetBackColor = offsetBackColor;
    offsetRect->setBrush(m_offsetBackColor);
}

QColor WaveformArea::constantBackColor() const {
    return m_constantBackColor;
}

void WaveformArea::setConstantBackColor(const QColor &constantBackColor) {
    m_constantBackColor = constantBackColor;
    constantRect->setBrush(m_constantBackColor);
}

QColor WaveformArea::blankBackColor() const {
    return m_blankBackColor;
}

void WaveformArea::setBlankBackColor(const QColor &blankBackColor) {
    m_blankBackColor = blankBackColor;
    blankRect->setBrush(m_blankBackColor);
}

QColor WaveformArea::waveformColor() const {
    return m_waveformColor;
}

void WaveformArea::setWaveformColor(const QColor &waveformColor) {
    m_waveformColor = waveformColor;
    update();
}

int WaveformArea::index() const {
    return m_index;
}

void WaveformArea::setIndex(int index) {
    m_index = index;
}

void WaveformArea::setSample(const QGenonSettings &genon) {
    if (genon.valid() && m_wavefile.load(genon.mFileName)) {
        m_sample = genon;
        m_orgSample = genon;

        m_frqfile.load(genon.frqFile());
    } else {
        m_sample.clear();
        m_orgSample.clear();
        m_wavefile.reset();
        m_frqfile.reset();
    }
    updateHandles();
    update();
}

QGenonSettings WaveformArea::sample() const {
    return m_sample;
}

void WaveformArea::setOffsetByCursor() {
    QPointF pos = cursorPos();

    QPointF orgPos = btnOffset->pos();
    btnOffset->setPosition(pos.x());

    handleOffsetMoveNoLink(btnOffset->pos(), orgPos);
    handleOffsetClicked();
}

void WaveformArea::setConstantByCursor() {
    QPointF pos = cursorPos();

    QPointF orgPos = btnConstant->pos();
    btnConstant->setPosition(pos.x());

    handleConstantMove(btnConstant->pos(), orgPos);
    handleConstantClicked();
}

void WaveformArea::setBlankByCursor() {
    QPointF pos = cursorPos();

    QPointF orgPos = btnBlank->pos();
    btnBlank->setPosition(pos.x());

    handleBlankMove(btnBlank->pos(), orgPos);
    handleBlankClicked();
}

void WaveformArea::setPreUttrByCursor() {
    QPointF pos = cursorPos();

    QPointF orgPos = btnPreUttr->pos();
    btnPreUttr->setPosition(pos.x());

    handlePreUttrMove(btnPreUttr->pos(), orgPos);
    handlePreUttrClicked();
}

void WaveformArea::setOverlapByCursor() {
    QPointF pos = cursorPos();

    QPointF orgPos = btnOverlap->pos();
    btnOverlap->setPosition(pos.x());

    handleOverlapMove(btnOverlap->pos(), orgPos);
    handleOverlapClicked();
}

QPointF WaveformArea::cursorPos() const {
    return m_view->mapToScene(m_view->mapFromGlobal(QCursor::pos()));
}

bool WaveformArea::containsCursor() const {
    return sceneRect().contains(cursorPos());
}

void WaveformArea::updateHandles() {
    double duration = m_wavefile.duration() * 1000;

    if (duration == 0) {
        for (auto it : items()) {
            it->hide();
        }
        return;
    }

    for (auto it : items()) {
        it->show();
    }

    btnOffset->setRegion(sceneRect());
    btnConstant->setRegion(sceneRect());
    btnBlank->setRegion(sceneRect());
    btnPreUttr->setRegion(sceneRect());
    btnOverlap->setRegion(sceneRect());

    btnOffset->setValue(m_orgSample.mOffset / duration);
    btnConstant->setValue((m_orgSample.mConstant + m_orgSample.mOffset) / duration);

    if (m_orgSample.mBlank >= 0) {
        btnBlank->setValue(1.0 - m_orgSample.mBlank / duration);
    } else {
        btnBlank->setValue((-m_orgSample.mBlank + m_orgSample.mOffset) / duration);
    }

    btnPreUttr->setValue((m_orgSample.mPreUtterance + m_orgSample.mOffset) / duration);
    btnOverlap->setValue((m_orgSample.mVoiceOverlap + m_orgSample.mOffset) / duration);

    updateRegionBack();
}

void WaveformArea::updateRegionBack() {
    QRectF offsetRect = sceneRect();
    offsetRect.setRight(btnOffset->position());

    QRectF constantRect = sceneRect();
    constantRect.setLeft(qMin(btnConstant->position(), btnOffset->position()));
    constantRect.setRight(btnConstant->position());

    QRectF blankRect = sceneRect();
    blankRect.setLeft(btnBlank->position());

    this->offsetRect->setRect(offsetRect);
    this->constantRect->setRect(constantRect);
    this->blankRect->setRect(blankRect);
}

void WaveformArea::drawSingleChannel(QPainter &painter, int W, int H) {
    float A = pow(2.0, 8.0 * m_wavefile.bytePerSample() - 1);

    double x = 0, y = double(H) / 2;
    double H2 = H / 2 - 10;

    QRect rect(painter.clipRegion().boundingRect());
    const QVector<short> &data = m_wavefile.data();
    int count = data.size();
    double delta = 0.5;

    x = rect.left();
    while (x < rect.right()) {
        int min = INT_MAX, max = INT_MIN;
        for (int j = x * count / W; j < (x + 1) * count / W; j++) {
            if (j > 0 && j < count) {
                if (data[j] < min) {
                    min = data[j];
                }
                if (data[j] > max) {
                    max = data[j];
                }
            }
        }
        double y1 = y + (max * H2 / A);
        double y2 = y + (min * H2 / A);
        painter.drawLine(QPointF(x, y1), QPointF(x, y2));
        x += delta;
    }
    return;
}

void WaveformArea::drawDoubleChannel(QPainter &painter, int W, int H) {
    float A = pow(2.0, 8.0 * m_wavefile.bytePerSample() - 1);

    double x = 0, y1 = double(H) / 4, y2 = double(H) * 3 / 4;
    double H2 = H / 4 - 10;

    QRect rect(painter.clipRegion().boundingRect());

    const QVector<short> &data = m_wavefile.data();
    const QVector<short> &data2 = m_wavefile.data2();

    int count = data.size();
    double delta = 0.5;

    x = rect.left();
    while (x < rect.right()) {
        int min = INT_MAX, max = INT_MIN;
        int min2 = INT_MAX, max2 = INT_MIN;
        for (int j = x * count / W; j < (x + 1) * count / W; j++) {
            if (j > 0 && j < count) {
                if (data[j] < min) {
                    min = data[j];
                }
                if (data2[j] < min2) {
                    min2 = data2[j];
                }
            }
            if (j > 0 && j < count) {
                if (data[j] > max) {
                    max = data[j];
                }
                if (data2[j] > max2) {
                    max2 = data2[j];
                }
            }
        }
        double y11 = y1 + (max * H2 / A);
        double y12 = y1 + (min * H2 / A);
        painter.drawLine(QPointF(x, y11), QPointF(x, y12));

        double y21 = y2 + (max * H2 / A);
        double y22 = y2 + (min * H2 / A);
        painter.drawLine(QPointF(x, y21), QPointF(x, y22));
        x += delta;
    }
    return;
}

void WaveformArea::handleOffsetMove(QPointF newPos, QPointF orgPos) {
    QGenonSettings genon = m_orgSample;
    QPointF delta = newPos - orgPos;

    btnPreUttr->setPosition(btnPreUttr->position() + delta.x());
    btnOverlap->setPosition(btnOverlap->position() + delta.x());
    btnConstant->setPosition(btnConstant->position() + delta.x());
    if (genon.mBlank < 0) {
        btnBlank->setPosition(btnBlank->position() + delta.x());
    }

    updateRegionBack();

    genon.mOffset = getOffset(genon.mOffset);
    genon.mPreUtterance = getPreUttrance(genon.mPreUtterance, genon.mOffset);
    genon.mVoiceOverlap = getVoiceOverlap(genon.mVoiceOverlap, genon.mOffset);
    genon.mConstant = getConstant(genon.mConstant, genon.mOffset);
    genon.mBlank = getBlank(genon.mBlank, genon.mOffset);

    m_sample = genon;
    emit editing();

    update();
}

void WaveformArea::handleOffsetMoveNoLink(QPointF newPos, QPointF orgPos) {
    QGenonSettings genon = m_orgSample;

    updateRegionBack();

    genon.mOffset = getOffset(genon.mOffset);
    genon.mPreUtterance = getPreUttrance(genon.mPreUtterance, genon.mOffset);
    genon.mVoiceOverlap = getVoiceOverlap(genon.mVoiceOverlap, genon.mOffset);
    genon.mConstant = getConstant(genon.mConstant, genon.mOffset);
    genon.mBlank = getBlank(genon.mBlank, genon.mOffset);

    m_sample = genon;
    emit editing();

    update();
}

void WaveformArea::handleConstantMove(QPointF newPos, QPointF orgPos) {
    QGenonSettings genon = m_orgSample;

    updateRegionBack();

    genon.mConstant = getConstant(genon.mConstant, genon.mOffset);
    m_sample = genon;
    emit editing();

    update();
}

void WaveformArea::handleBlankMove(QPointF newPos, QPointF orgPos) {
    QGenonSettings genon = m_orgSample;

    updateRegionBack();

    genon.mBlank = getBlank(genon.mBlank, genon.mOffset);

    m_sample = genon;
    emit editing();

    update();
}

void WaveformArea::handlePreUttrMove(QPointF newPos, QPointF orgPos) {
    double duration = m_wavefile.duration() * 1000;
    QGenonSettings genon = m_orgSample;

    updateRegionBack();

    genon.mPreUtterance = getPreUttrance(genon.mPreUtterance, genon.mOffset);

    m_sample = genon;
    emit editing();

    update();
}

void WaveformArea::handleOverlapMove(QPointF newPos, QPointF orgPos) {
    double duration = m_wavefile.duration() * 1000;
    QGenonSettings genon = m_orgSample;

    updateRegionBack();

    genon.mVoiceOverlap = getVoiceOverlap(genon.mVoiceOverlap, genon.mOffset);

    m_sample = genon;
    emit editing();

    update();
}

void WaveformArea::handleOffsetClicked() {
    emit editOver(m_orgSample);
    m_orgSample = m_sample;
}

void WaveformArea::handleConstantClicked() {
    emit editOver(m_orgSample);
    m_orgSample = m_sample;
}

void WaveformArea::handleBlankClicked() {
    emit editOver(m_orgSample);
    m_orgSample = m_sample;
}

void WaveformArea::handlePreUttrClicked() {
    emit editOver(m_orgSample);
    m_orgSample = m_sample;
}

void WaveformArea::handleOverlapClicked() {
    emit editOver(m_orgSample);
    m_orgSample = m_sample;
}

double WaveformArea::getOffset(double org) const {
    double duration = m_wavefile.duration() * 1000;
    return btnOffset->value() * duration;
}

double WaveformArea::getPreUttrance(double org, double offset) const {
    double duration = m_wavefile.duration() * 1000;
    return btnPreUttr->value() * duration - offset;
}

double WaveformArea::getVoiceOverlap(double org, double offset) const {
    double duration = m_wavefile.duration() * 1000;
    return btnOverlap->value() * duration - offset;
}

double WaveformArea::getConstant(double org, double offset) const {
    double duration = m_wavefile.duration() * 1000;
    return btnConstant->value() * duration - offset;
}

double WaveformArea::getBlank(double org, double offset) const {
    double duration = m_wavefile.duration() * 1000;
    double temp;
    if (org >= 0) {
        temp = (1 - btnBlank->value()) * duration;
    } else {
        temp = -btnBlank->value() * duration + offset;
        if (temp >= 0) {
            temp = (1 - btnBlank->value()) * duration;
        }
    }
    return temp;
}

void WaveformArea::drawBackground(QPainter *painter, const QRectF &rect) {
    if (m_wavefile.duration() == 0) {
        return;
    }

    int W = width();
    int H = height();

    QRect clipRect = m_view->viewportRect().toRect();
    int sceneWidth = sceneRect().width();
    if (sceneWidth < clipRect.width()) {
        clipRect.setWidth(sceneWidth);
    }
    painter->setClipRect(clipRect);

    QPen pen(m_waveformColor);
    painter->setPen(pen);
    if (m_wavefile.channels() == 1) {
        drawSingleChannel(*painter, W, H);
    } else if (m_wavefile.channels() == 2) {
        drawDoubleChannel(*painter, W, H);
    }
}

void WaveformArea::handleSceneRectChanged(const QRectF &rect) {
    update();
    updateHandles();
}