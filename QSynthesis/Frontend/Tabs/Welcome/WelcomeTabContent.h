#ifndef WELCOMETABCONTENT_H
#define WELCOMETABCONTENT_H

#include <QDateTime>
#include <QDesktopServices>
#include <QLabel>
#include <QLayout>
#include <QListView>
#include <QListWidget>
#include <QPushButton>
#include <QSize>
#include <QWidget>

#include "../Central/CentralTab.h"
#include "Controls/Buttons/SwitchButton.h"
#include "FileListWidget.h"
#include "Global/Methods.h"
#include "QUtils.h"

class WelcomeTab;

// Content of welcoming tab Class
class WelcomeTabContent : public CentralTabContent {
    Q_OBJECT
public:
    explicit WelcomeTabContent(WelcomeTab *tab, QWidget *parent = nullptr);

    void refresh();
    void addItem(const QString &FileName, const QString &LastOpenTime, QString FileLocation);

private:
    WelcomeTab *m_tab;

    QVBoxLayout *mainLayout, *switchLineLayout;
    QHBoxLayout *topLayout, *switchLayout;

    QLabel *lbTitle;
    QLabel *lbEmpty;

    QPushButton *btnNew, *btnOpen;
    FileListWidget *filesList;

    SwitchButton *sBtnFiles, *sBtnFolders, *sBtnTemplates;
    SwitchButtonGroup *sBtnGroup;
    SwitchButtonUnderline *sBtnUnderline;

    void onNewBtnClicked();
    void onOpenBtnClicked();

    void handleSwitched();
    void refreshWidget();

signals:
    void updateTab();
};

#endif // WELCOMETABCONTENT_H
