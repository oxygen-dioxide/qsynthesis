#ifndef TYPES_H
#define TYPES_H

#include <QString>

namespace Qs {

    enum ButtonStatus {
        RollOut,
        RollOver,
        RollDown,
    };

    enum CheckStatus {
        Unchecked,
        Checked,
        Uncheckable,
    };

    enum DirRelation {
        SubDir,
        ParentDir,
        SameDir,
    };

    enum MainTabs {
        NoTab,
        Welcome,
        Tuning,
        Folder,
        Keyboard,
        Setting,
    };

    enum VoiceTabs {
        Description,
        PrefixTable,
        OtoTable,
    };

    namespace Panels {

        enum Tracks {
            Overview,
            Configuration,
        };
        enum Editor {
            Note,
            Pitch,
            Envelope,
        };
        enum Params {
            Intensity,
            Modulation,
            Velocity,
        };

    } // namespace Panels

    enum PrefixMap {
        Prefix,
        Suffix,
    };

    enum Scope {
        Mode2,
        Envelopes,
    };

    enum StretchMode {
        NoStretch,
        AbsoluteStretch,
        RelativeStretch,
        PatchStretch,
    };

    enum PianoKeyColor {
        Black,
        White,
    };

    enum NoteStatus {
        Unlisted,
        Listed,
        Rest,
    };

    enum VariableSource { User, Default };

    enum IgnoreSelection { IgnoreNone = 0, IgnoreFront = 1, IgnoreBack = 2, IgnoreSides = 3 };

    enum SpecialPos { Leftmost, Middle, Rightmost };

    enum EnvelopeBatch { P1P4Fade, P2P3Fade, DefaultEnvelope };

    QString noteStatusToString(NoteStatus status);

    QString buttonStatusToString(ButtonStatus status);

} // namespace Qs

#endif // TYPES_H
