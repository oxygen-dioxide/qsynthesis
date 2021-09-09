#include "SettingIniData.h"

SettingIniData::SettingIniData() {
    projects.setType(FileNameSet::File);
    folders.setType(FileNameSet::Directory);
    wavtools.setType(FileNameSet::File);
    resamplers.setType(FileNameSet::File);

    clear();
}

void SettingIniData::clear() {
    tool1Path = "";
    tool2Path = "";

    projects.clear();
    folders.clear();
    wavtools.clear();
    resamplers.clear();

    windowRect = NO_RECT;
    windowMaximized = false;

    tracksFormVisibility = true;
    editorFormVisibility = true;
    paramsFormVisibility = true;

    lastQuantize = 1;
    ignoreRestsWhenInsert = true;
    replaceLyricsWhenInsert = true;
    separateCharWhenInsert = false;
}
