#ifndef PROJECTSETTINGSDIALOG_H
#define PROJECTSETTINGSDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QValidator>

#include "../Controls/CustomControls.h"
#include "QUtils.h"

class ProjectSettingsDialog : public QDialog {
    Q_OBJECT
public:
    explicit ProjectSettingsDialog(QString &wavtool, QString &outfile, QString &cache,
                                   QWidget *parent = nullptr);
    ~ProjectSettingsDialog();

    int exec() override;

private:
    QString &wavtool, &outfile, &cache;

    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonLayout, *browseLayout;

    QPushButton *btnBrowse, *btnClear;
    QPushButton *btnCancel, *btnOK;

    ComboControl *ccWavtool;
    LineControl *lcOutfile, *lcCache;

    void init();

    void onOKClicked();
    void onCancelClicked();

    void onClearBtnClicked();
    void onBrowseBtnClicked();

    void closeEvent(QCloseEvent *event) override;
};

#endif // PROJECTSETTINGSDIALOG_H
