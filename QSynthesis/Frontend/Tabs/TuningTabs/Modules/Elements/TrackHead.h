#ifndef TRACKHEAD_H
#define TRACKHEAD_H

#include <QActionGroup>
#include <QDial>
#include <QFileSystemWatcher>
#include <QLabel>
#include <QMenu>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include "Controls/FixedLineEdit.h"
#include "Directory/VoiceInfo.h"
#include "QUtils.h"
#include "VoiceBank/QOtoReference.h"

class HeadsArea;

class TrackHead : public QWidget {
    Q_OBJECT

    Q_PROPERTY(QColor iconColor READ iconColor WRITE setIconColor NOTIFY iconColorChanged)
public:
    explicit TrackHead(HeadsArea *parent);
    explicit TrackHead(QString res, QString flags, QString name, QString voice, HeadsArea *parent);
    explicit TrackHead(QString flags, QString name, QString voice, HeadsArea *parent);
    explicit TrackHead(QString name, QString voice, HeadsArea *parent);

    void clear();

    int mode() const;
    void setMode(int mode);

    QString flags() const;
    void setFlags(const QString &flags);

    QString name() const;
    void setName(const QString &name);

    QString voiceDir() const;
    void setVoiceDir(const QString &voice);

    QString resampler() const;
    void setResampler(const QString &resampler);

    void updateStatus();

    QColor iconColor() const;
    void setIconColor(const QColor &iconColor);

private:
    HeadsArea *m_parent;

    int m_mode;

    QString m_resampler;
    QString m_flags;
    QString m_name;
    QString m_voice;

    QString m_voiceTitle; // Voice Name Cache

    QColor m_iconColor;

private:
    QHBoxLayout *trackLayout;

    QVBoxLayout *labelLayout;
    QGridLayout *dialLayout;
    QVBoxLayout *buttonLayout;

    QPushButton *btnMute, *btnSolo;
    QDial *panDial, *volDial;

    QLabel *lbPanIcon, *lbVolIcon;

    FixedLineEdit *lbNameFlags;
    QPushButton *btnVoiceRes;

    QMenu *m_menu;

    void init();

    void reloadIcons();

    void onLineEditFinished();
    void onLabelBtnClicked();

    void onVoiceActionTriggered();
    void onBrowseActionTriggered();
    void onRevealActionTriggered();

private:
    VoiceInfo m_voiceInfo;

    void loadVoiceInfo();
    void handleVoiceInfoChanged();

signals:
    void nameChanged(QString orgVal, QString newVal);
    void flagsChanged(QString orgVal, QString newVal);

    void voiceChanged(QString orgVal, QString newVal);
    void resamplerChanged(QString orgVal, QString newVal);

    void iconColorChanged();
};

#endif // TRACKHEAD_H
