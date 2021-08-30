#ifndef FORMSPLITTER_H
#define FORMSPLITTER_H

#include <QSplitter>
#include <QWidget>

#include "../../Forms/BaseForm.h"

class FormSplitter : public QSplitter {
    Q_OBJECT
public:
    explicit FormSplitter(QWidget *parent = nullptr);

    void addForm(BaseForm *form, int stretch = 0);
    void insertForm(int index, BaseForm *form, int stretch = 0);
    void removeForm(BaseForm *form);

    void setTopWidget(QWidget *w, int stretch = 0);
    void removeTopWidget();

    void setStretchFactor(QWidget *w, int stretch);
    void setStretchFactor(int index, int stretch);

    void setMainIndex(int index);
    void setMainForm(BaseForm *form);
    int MainIndex();

private:
    QVector<BaseForm *> forms;
    QWidget *topWidget;
    int mainIndex;

    void onFormVisibilityChanged(bool b);

    void modifyFeatures();
    void modifyStretches();

    QSplitterHandle *createHandle() override;
};

class FormSplitterHandle : public QSplitterHandle {
    Q_OBJECT
public:
    explicit FormSplitterHandle(QSplitter *parent) : QSplitterHandle(Qt::Vertical, parent) {
        QPalette pa;
        QColor color = Qt::transparent;

        // color = Qt::white;
        pa.setColor(QPalette::Background, color);
        setPalette(pa);
    }

private:
};

#endif // FORMSPLITTER_H