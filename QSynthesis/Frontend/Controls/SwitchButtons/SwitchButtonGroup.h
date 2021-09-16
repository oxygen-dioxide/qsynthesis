#ifndef SWITCHBUTTONGROUP_H
#define SWITCHBUTTONGROUP_H

#include <QObject>

class SwitchButton;

class SwitchButtonGroup : public QObject {
    Q_OBJECT
public:
    SwitchButtonGroup(QObject *parent);

    void addButton(SwitchButton *btn);
    void removeButton(SwitchButton *btn);

    int currentIndex();
    void setCurrentIndex(int index);

    SwitchButton *currentButton();

private:
    void updateSelected(bool checked);
    void onOneDoubleClicked();

    SwitchButton *m_current;
    QList<SwitchButton *> btns;

signals:
    void switched();
    void oneDoubleClicked();
};

#endif // SWITCHBUTTONGROUP_H
