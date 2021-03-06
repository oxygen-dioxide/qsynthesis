#include "Managers/ColorTheme.h"
#include "QSTabs.h"
#include "mainwindow.h"

void MainWindow::handleNewFile() {
    qDebug() << "[Action]"
             << "New";

    addEmptyTab();
}

void MainWindow::handleImportFile() {
    qDebug() << "[Action]"
             << "Import";

    execImport();
}

void MainWindow::handleOpenFile() {
    qDebug() << "[Action]"
             << "Open File";

    execOpen();
}

void MainWindow::handleOpenFolder() {
    qDebug() << "[Action]"
             << "Open Folder";

    execOpenFolder();
}

void MainWindow::handleSaveFile() {
    qDebug() << "[Action]"
             << "Save";

    CentralTab *tab = currentTab();
    execSave(tab);
}

void MainWindow::handleSaveAsFile() {
    qDebug() << "[Action]"
             << "Save As";

    CentralTab *tab = currentTab();
    if (tab->tabType() == Qs::Tuning) {
        TuningTab *tab1 = qobject_cast<TuningTab *>(tab);
        execSaveAs(tab1);
    }
}

void MainWindow::handleRestoreFile() {
    qDebug() << "[Action]"
             << "Restore";

    CentralTab *tab = currentTab();
    if (tab->tabType() != Qs::Welcome) {
        tab->restore();
    }
}

void MainWindow::handleSwitchFile() {
    qDebug() << "[Action]"
             << "Switch";

    CentralTab *prevTab = previousTab();
    if (prevTab) {
        tabs->setCurrentWidget(prevTab);
    }
}

void MainWindow::handleCloseFile() {
    qDebug() << "[Action]"
             << "Close";

    handleTabCloseRequent(tabs->currentIndex());
}

void MainWindow::handleUndo() {
    qDebug() << "[Action]"
             << "Undo";

    CentralTab *tab = currentTab();
    if (tab->tabType() != Qs::Welcome) {
        tab->undo();
    }
}

void MainWindow::handleRedo() {
    qDebug() << "[Action]"
             << "Redo";

    CentralTab *tab = currentTab();
    if (tab->tabType() != Qs::Welcome) {
        tab->redo();
    }
}

void MainWindow::handleSelectAll() {
    qDebug() << "[Action]"
             << "Select All";

    CentralTab *tab = currentTab();
    if (tab->tabType() != Qs::Welcome) {
        tab->selectAll();
    }
}

void MainWindow::handleDeselect() {
    qDebug() << "[Action]"
             << "Deselect";

    CentralTab *tab = currentTab();
    if (tab->tabType() != Qs::Welcome) {
        tab->deselect();
    }
}

void MainWindow::handleReset() {
    qDebug() << "[Action]"
             << "Reset";

    CentralTab *tab = currentTab();
    if (tab->tabType() != Qs::Welcome) {
        tab->reset();
    }
}

void MainWindow::handlePlayPause() {
    qDebug() << "[Action]"
             << "Play";

    CentralTab *tab = currentTab();
    if (tab->tabType() == Qs::Tuning) {
        TuningTab *tab1 = qobject_cast<TuningTab *>(tab);
        tab1->play();
    } else if (tab->tabType() == Qs::Folder) {
        VoiceBankTab *tab1 = qobject_cast<VoiceBankTab *>(tab);
        tab1->play();
    }
}

void MainWindow::handleReplay() {
    qDebug() << "[Action]"
             << "Replay";

    CentralTab *tab = currentTab();
    if (tab->tabType() == Qs::Tuning) {
        TuningTab *tab1 = qobject_cast<TuningTab *>(tab);
        tab1->replay();
    } else if (tab->tabType() == Qs::Folder) {
        VoiceBankTab *tab1 = qobject_cast<VoiceBankTab *>(tab);
        tab1->replay();
    }
}

void MainWindow::handleStop() {
    qDebug() << "[Action]"
             << "Stop";

    CentralTab *tab = currentTab();
    if (tab->tabType() == Qs::Tuning) {
        TuningTab *tab1 = qobject_cast<TuningTab *>(tab);
        tab1->stop();
    } else if (tab->tabType() == Qs::Folder) {
        VoiceBankTab *tab1 = qobject_cast<VoiceBankTab *>(tab);
        tab1->stop();
    }
}

void MainWindow::handleOpenSettings() {
    addSettingTab();
}

void MainWindow::handleOpenKeyShortcuts() {
    addKeyboardTab();
}

void MainWindow::handleOpenWelcome() {
    addWelcomeTab();
}

void MainWindow::handleInstructions() {
}

void MainWindow::handleCheckUpdate() {
}

void MainWindow::handleAboutApp() {
    QFile file(":/texts/about.html");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }
    QTextStream in(&file);
    in.setCodec(QTextCodec::codecForName("UTF-8"));

    QString text = in.readAll();
    file.close();

    QMessageBox::about(this, tr("About QSynthesis"), text);
}

void MainWindow::handleAboutQt() {
    QMessageBox::aboutQt(this, tr("About Qt"));
}
