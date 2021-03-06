#ifndef SELECTSDIALOG_H
#define SELECTSDIALOG_H

#include <QCheckBox>
#include <QDialog>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QValidator>

#include "BaseDialog.h"
#include "QUtils.h"

class SelectsDialog : public BaseDialog {
    Q_OBJECT
public:
    SelectsDialog(const QString &title, const QString &caption, const QStringList &list,
                  QList<bool> &result, bool single, QWidget *parent = nullptr);
    ~SelectsDialog();

    int exec() override;

private:
    QList<bool> &m_result;

    bool m_single;
    QStringList m_options;
    QString m_title, m_caption;

    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonLayout;

    QLabel *lbCaption;
    QVector<QCheckBox *> boxes;

    QPushButton *btnCancel, *btnOK;

    void init();

    void onOKClicked();
    void onCancelClicked();
};

#endif // SELECTSDIALOG_H
