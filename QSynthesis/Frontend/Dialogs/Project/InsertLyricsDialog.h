#ifndef INSERTLYRICSDIALOG_H
#define INSERTLYRICSDIALOG_H

#include <QCheckBox>
#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>

#include "BaseDialog.h"
#include "Controls/Group/TextControl.h"
#include "QUtils.h"

class InsertLyricsDialog : public BaseDialog {
    Q_OBJECT
public:
    explicit InsertLyricsDialog(QStringList &lyrics, QWidget *parent = nullptr);
    ~InsertLyricsDialog();

    int exec() override;

    bool ignoreRest() const;
    bool replaceLyrics() const;
    bool separateIntoChar() const;

private:
    QStringList &orgLyrics;

    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonLayout;

    QPushButton *btnReset, *btnCancel, *btnOK;

    TextControl *tcLyrics;
    QCheckBox *cbIgnore, *cbReplace, *cbSplit;

    void init();

    void setDefaultLyrics();

    void onResetClicked();
    void onOKClicked();
    void onCancelClicked();

private:
    static int findNextQuote(const QString &s, int n);
    static int findNextSpace(const QString &s, int n);

    static QStringList splitBySpace(const QString &content);
    static QStringList splitByNone(const QString &content);

    static QString correctLyricBySpace(const QString &lrc);
    static QString correctLyricByLength(const QString &lrc);
};

#endif // INSERTLYRICSDIALOG_H
