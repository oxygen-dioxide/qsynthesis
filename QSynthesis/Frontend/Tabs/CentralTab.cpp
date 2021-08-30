#include "CentralTab.h"
#include "TabWidget.h"
#include "mainwindow.h"

CentralTab::CentralTab(TabWidget *parent) : BaseTab(parent) {
    m_filename = "";
    m_fixedname = "";
    m_type = Qs::NoTab;
    edited = false;
}

CentralTab::~CentralTab() {
    qDebug() << "[Destruct]"
             << "CentralTab";
}

CentralTab::TabType CentralTab::tabType() const {
    return m_type;
}

void CentralTab::undo() {
}

void CentralTab::redo() {
}

void CentralTab::selectAll() {
}

void CentralTab::deselect() {
}

void CentralTab::reset() {
}

void CentralTab::play() {
}

void CentralTab::replay() {
}

void CentralTab::stop() {
}

bool CentralTab::isEdited() const {
    return edited;
}

void CentralTab::setEdited(bool value) {
    edited = value;
    updateTabName();
    updateMenuCore();
}

void CentralTab::updateMenuRoot() {
}

void CentralTab::updateStatusRoot() {
}

void CentralTab::updateMenuCore() {
}

bool CentralTab::load() {
    return false;
}

bool CentralTab::save() {
    return false;
}

bool CentralTab::saveAs(const QString &filename) {
    return false;
}

bool CentralTab::restore() {
    return false;
}

void CentralTab::awake() {
    updateMenuRoot();
    updateStatusRoot();
}

void CentralTab::sleep() {
}

void CentralTab::change() {
    updateMenuCore();
}

QString CentralTab::filename() const {
    return m_filename;
}

void CentralTab::setFilename(const QString &value) {
    m_filename = value;
}

QString CentralTab::fixedname() const {
    return m_fixedname;
}

void CentralTab::setFixedname(const QString &value) {
    m_fixedname = value;
}

void CentralTab::setTabName(const QString &value) {
    QString tabName = (edited ? UntitledPrefix : "") + value;
    BaseTab::setTabName(tabName);
}

void CentralTab::updateTabName() {
    setTabName(m_fixedname);
}