#ifndef TYPES_H
#define TYPES_H

#include <QString>

namespace Qs {

    enum ButtonStatus {
        RollOut,
        RollOver,
        RollDown,
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

    enum NoteProperty {
        Intensity,
        Modulation,
        Velocity,
        Lyric,
        Flags,
        NoteNum,
        Length,
        PreUtterance,
        VoiceOverlap,
        StartPoint,
        Tempo,
        PitchBend,
        PitchType,
        PitchStart,
        Portamento,
        Vibrato,
        Envelope,
    };

    namespace Oto {

        enum Property {
            Filename,
            Alias,
            Offset,
            Constant,
            Blank,
            PreUtterance,
            VoiceOverlap,
        };

    }; // namespace Oto

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
        ReserveStretch,
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

    enum SpecialPos { Leftmost, Middle, Rightmost };

    enum PitchPoint { sType, lineType, rType, jType };

    enum EnvelopeBatch { P1P4Fade, P2P3Fade, DefaultEnvelope };

    QString NoteStatusToString(NoteStatus status);

    QString pitchPointToString(const PitchPoint &oType);
    PitchPoint stringToPitchPoint(const QString &oString);

} // namespace Qs

#endif // TYPES_H