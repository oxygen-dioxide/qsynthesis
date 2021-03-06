#include "ImageLabel.h"
#include "Controls/Others/TemporaryMenu.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QMimeDatabase>
#include <QMimeType>

ImageLabel::ImageLabel(QWidget *parent) : QLabel(parent) {
    init();
}

ImageLabel::~ImageLabel() {
}

void ImageLabel::init() {
    m_menuButtons = Qt::RightButton;
}

Qt::MouseButtons ImageLabel::menuButtons() const {
    return m_menuButtons;
}

void ImageLabel::setMenuButtons(const Qt::MouseButtons &menuButtons) {
    m_menuButtons = menuButtons;
}

void ImageLabel::mouseReleaseEvent(QMouseEvent *event) {
    if (!(event->button() & m_menuButtons)) {
        return;
    }

    QString revealStr = tr("Show in %1(&S)").arg(fileManagerName);
    QStringList list{revealStr, "", tr("Replace..."), tr("Remove")};
    TemporaryMenu *menu = new TemporaryMenu(list, this);

    int index = menu->start();
    menu->deleteLater();

    switch (index) {
    case 0:
        emit reveal();
        break;
    case 1:
        handleReplace();
        break;
    case 2:
        emit remove();
        break;
    default:
        break;
    }
}

void ImageLabel::handleReplace() {
    QString path = QFileDialog::getOpenFileName(nullptr, tr("Open File"), ".", imageFilterString);
    if (path.isEmpty()) {
        return;
    }

    QMimeDatabase db;
    QMimeType mime = db.mimeTypeForFile(path);
    if (!mime.name().startsWith("image/")) {
        QMessageBox::warning(this, MainTitle, tr("This file is not a valid image file!"));
        return;
    }

    emit replace(path);
}
