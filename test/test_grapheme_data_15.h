//THIS FILE IS GENERATED. PLEASE DO NOT EDIT DIRECTLY

//
// Copyright 2024 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//

//÷ 0020 ÷ 0020 ÷
//÷ [0.2] SPACE (Other) ÷ [999.0] SPACE (Other) ÷ [0.3]
check_graphemes(U"\u0020\u0020", {U"\u0020", U"\u0020"});
//÷ 0020 × 0308 ÷ 0020 ÷
//÷ [0.2] SPACE (Other) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] SPACE (Other) ÷ [0.3]
check_graphemes(U"\u0020\u0308\u0020", {U"\u0020\u0308", U"\u0020"});
//÷ 0020 ÷ 000D ÷
//÷ [0.2] SPACE (Other) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u0020\u000D", {U"\u0020", U"\u000D"});
//÷ 0020 × 0308 ÷ 000D ÷
//÷ [0.2] SPACE (Other) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u0020\u0308\u000D", {U"\u0020\u0308", U"\u000D"});
//÷ 0020 ÷ 000A ÷
//÷ [0.2] SPACE (Other) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u0020\u000A", {U"\u0020", U"\u000A"});
//÷ 0020 × 0308 ÷ 000A ÷
//÷ [0.2] SPACE (Other) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u0020\u0308\u000A", {U"\u0020\u0308", U"\u000A"});
//÷ 0020 ÷ 0001 ÷
//÷ [0.2] SPACE (Other) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u0020\u0001", {U"\u0020", U"\u0001"});
//÷ 0020 × 0308 ÷ 0001 ÷
//÷ [0.2] SPACE (Other) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u0020\u0308\u0001", {U"\u0020\u0308", U"\u0001"});
//÷ 0020 × 034F ÷
//÷ [0.2] SPACE (Other) × [9.0] COMBINING GRAPHEME JOINER (Extend) ÷ [0.3]
check_graphemes(U"\u0020\u034F", {U"\u0020\u034F"});
//÷ 0020 × 0308 × 034F ÷
//÷ [0.2] SPACE (Other) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] COMBINING GRAPHEME JOINER (Extend) ÷ [0.3]
check_graphemes(U"\u0020\u0308\u034F", {U"\u0020\u0308\u034F"});
//÷ 0020 ÷ 1F1E6 ÷
//÷ [0.2] SPACE (Other) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u0020\U0001F1E6", {U"\u0020", U"\U0001F1E6"});
//÷ 0020 × 0308 ÷ 1F1E6 ÷
//÷ [0.2] SPACE (Other) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u0020\u0308\U0001F1E6", {U"\u0020\u0308", U"\U0001F1E6"});
//÷ 0020 ÷ 0600 ÷
//÷ [0.2] SPACE (Other) ÷ [999.0] ARABIC NUMBER SIGN (Prepend) ÷ [0.3]
check_graphemes(U"\u0020\u0600", {U"\u0020", U"\u0600"});
//÷ 0020 × 0308 ÷ 0600 ÷
//÷ [0.2] SPACE (Other) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] ARABIC NUMBER SIGN (Prepend) ÷ [0.3]
check_graphemes(U"\u0020\u0308\u0600", {U"\u0020\u0308", U"\u0600"});
//÷ 0020 × 0903 ÷
//÷ [0.2] SPACE (Other) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u0020\u0903", {U"\u0020\u0903"});
//÷ 0020 × 0308 × 0903 ÷
//÷ [0.2] SPACE (Other) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u0020\u0308\u0903", {U"\u0020\u0308\u0903"});
//÷ 0020 ÷ 1100 ÷
//÷ [0.2] SPACE (Other) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u0020\u1100", {U"\u0020", U"\u1100"});
//÷ 0020 × 0308 ÷ 1100 ÷
//÷ [0.2] SPACE (Other) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u0020\u0308\u1100", {U"\u0020\u0308", U"\u1100"});
//÷ 0020 ÷ 1160 ÷
//÷ [0.2] SPACE (Other) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u0020\u1160", {U"\u0020", U"\u1160"});
//÷ 0020 × 0308 ÷ 1160 ÷
//÷ [0.2] SPACE (Other) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u0020\u0308\u1160", {U"\u0020\u0308", U"\u1160"});
//÷ 0020 ÷ 11A8 ÷
//÷ [0.2] SPACE (Other) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u0020\u11A8", {U"\u0020", U"\u11A8"});
//÷ 0020 × 0308 ÷ 11A8 ÷
//÷ [0.2] SPACE (Other) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u0020\u0308\u11A8", {U"\u0020\u0308", U"\u11A8"});
//÷ 0020 ÷ AC00 ÷
//÷ [0.2] SPACE (Other) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u0020\uAC00", {U"\u0020", U"\uAC00"});
//÷ 0020 × 0308 ÷ AC00 ÷
//÷ [0.2] SPACE (Other) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u0020\u0308\uAC00", {U"\u0020\u0308", U"\uAC00"});
//÷ 0020 ÷ AC01 ÷
//÷ [0.2] SPACE (Other) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u0020\uAC01", {U"\u0020", U"\uAC01"});
//÷ 0020 × 0308 ÷ AC01 ÷
//÷ [0.2] SPACE (Other) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u0020\u0308\uAC01", {U"\u0020\u0308", U"\uAC01"});
//÷ 0020 ÷ 231A ÷
//÷ [0.2] SPACE (Other) ÷ [999.0] WATCH (ExtPict) ÷ [0.3]
check_graphemes(U"\u0020\u231A", {U"\u0020", U"\u231A"});
//÷ 0020 × 0308 ÷ 231A ÷
//÷ [0.2] SPACE (Other) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] WATCH (ExtPict) ÷ [0.3]
check_graphemes(U"\u0020\u0308\u231A", {U"\u0020\u0308", U"\u231A"});
//÷ 0020 × 0300 ÷
//÷ [0.2] SPACE (Other) × [9.0] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u0020\u0300", {U"\u0020\u0300"});
//÷ 0020 × 0308 × 0300 ÷
//÷ [0.2] SPACE (Other) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u0020\u0308\u0300", {U"\u0020\u0308\u0300"});
//÷ 0020 × 200D ÷
//÷ [0.2] SPACE (Other) × [9.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u0020\u200D", {U"\u0020\u200D"});
//÷ 0020 × 0308 × 200D ÷
//÷ [0.2] SPACE (Other) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u0020\u0308\u200D", {U"\u0020\u0308\u200D"});
//÷ 0020 ÷ 0378 ÷
//÷ [0.2] SPACE (Other) ÷ [999.0] <reserved-0378> (Other) ÷ [0.3]
check_graphemes(U"\u0020\u0378", {U"\u0020", U"\u0378"});
//÷ 0020 × 0308 ÷ 0378 ÷
//÷ [0.2] SPACE (Other) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] <reserved-0378> (Other) ÷ [0.3]
check_graphemes(U"\u0020\u0308\u0378", {U"\u0020\u0308", U"\u0378"});
//÷ 000D ÷ 0020 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] SPACE (Other) ÷ [0.3]
check_graphemes(U"\u000D\u0020", {U"\u000D", U"\u0020"});
//÷ 000D ÷ 0308 ÷ 0020 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] SPACE (Other) ÷ [0.3]
check_graphemes(U"\u000D\u0308\u0020", {U"\u000D", U"\u0308", U"\u0020"});
//÷ 000D ÷ 000D ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u000D\u000D", {U"\u000D", U"\u000D"});
//÷ 000D ÷ 0308 ÷ 000D ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u000D\u0308\u000D", {U"\u000D", U"\u0308", U"\u000D"});
//÷ 000D × 000A ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) × [3.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u000D\u000A", {U"\u000D\u000A"});
//÷ 000D ÷ 0308 ÷ 000A ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u000D\u0308\u000A", {U"\u000D", U"\u0308", U"\u000A"});
//÷ 000D ÷ 0001 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u000D\u0001", {U"\u000D", U"\u0001"});
//÷ 000D ÷ 0308 ÷ 0001 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u000D\u0308\u0001", {U"\u000D", U"\u0308", U"\u0001"});
//÷ 000D ÷ 034F ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] COMBINING GRAPHEME JOINER (Extend) ÷ [0.3]
check_graphemes(U"\u000D\u034F", {U"\u000D", U"\u034F"});
//÷ 000D ÷ 0308 × 034F ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] COMBINING GRAPHEME JOINER (Extend) ÷ [0.3]
check_graphemes(U"\u000D\u0308\u034F", {U"\u000D", U"\u0308\u034F"});
//÷ 000D ÷ 1F1E6 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u000D\U0001F1E6", {U"\u000D", U"\U0001F1E6"});
//÷ 000D ÷ 0308 ÷ 1F1E6 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u000D\u0308\U0001F1E6", {U"\u000D", U"\u0308", U"\U0001F1E6"});
//÷ 000D ÷ 0600 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] ARABIC NUMBER SIGN (Prepend) ÷ [0.3]
check_graphemes(U"\u000D\u0600", {U"\u000D", U"\u0600"});
//÷ 000D ÷ 0308 ÷ 0600 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] ARABIC NUMBER SIGN (Prepend) ÷ [0.3]
check_graphemes(U"\u000D\u0308\u0600", {U"\u000D", U"\u0308", U"\u0600"});
//÷ 000D ÷ 0903 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u000D\u0903", {U"\u000D", U"\u0903"});
//÷ 000D ÷ 0308 × 0903 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u000D\u0308\u0903", {U"\u000D", U"\u0308\u0903"});
//÷ 000D ÷ 1100 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u000D\u1100", {U"\u000D", U"\u1100"});
//÷ 000D ÷ 0308 ÷ 1100 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u000D\u0308\u1100", {U"\u000D", U"\u0308", U"\u1100"});
//÷ 000D ÷ 1160 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u000D\u1160", {U"\u000D", U"\u1160"});
//÷ 000D ÷ 0308 ÷ 1160 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u000D\u0308\u1160", {U"\u000D", U"\u0308", U"\u1160"});
//÷ 000D ÷ 11A8 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u000D\u11A8", {U"\u000D", U"\u11A8"});
//÷ 000D ÷ 0308 ÷ 11A8 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u000D\u0308\u11A8", {U"\u000D", U"\u0308", U"\u11A8"});
//÷ 000D ÷ AC00 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u000D\uAC00", {U"\u000D", U"\uAC00"});
//÷ 000D ÷ 0308 ÷ AC00 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u000D\u0308\uAC00", {U"\u000D", U"\u0308", U"\uAC00"});
//÷ 000D ÷ AC01 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u000D\uAC01", {U"\u000D", U"\uAC01"});
//÷ 000D ÷ 0308 ÷ AC01 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u000D\u0308\uAC01", {U"\u000D", U"\u0308", U"\uAC01"});
//÷ 000D ÷ 231A ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] WATCH (ExtPict) ÷ [0.3]
check_graphemes(U"\u000D\u231A", {U"\u000D", U"\u231A"});
//÷ 000D ÷ 0308 ÷ 231A ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] WATCH (ExtPict) ÷ [0.3]
check_graphemes(U"\u000D\u0308\u231A", {U"\u000D", U"\u0308", U"\u231A"});
//÷ 000D ÷ 0300 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u000D\u0300", {U"\u000D", U"\u0300"});
//÷ 000D ÷ 0308 × 0300 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u000D\u0308\u0300", {U"\u000D", U"\u0308\u0300"});
//÷ 000D ÷ 200D ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u000D\u200D", {U"\u000D", U"\u200D"});
//÷ 000D ÷ 0308 × 200D ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u000D\u0308\u200D", {U"\u000D", U"\u0308\u200D"});
//÷ 000D ÷ 0378 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] <reserved-0378> (Other) ÷ [0.3]
check_graphemes(U"\u000D\u0378", {U"\u000D", U"\u0378"});
//÷ 000D ÷ 0308 ÷ 0378 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] <reserved-0378> (Other) ÷ [0.3]
check_graphemes(U"\u000D\u0308\u0378", {U"\u000D", U"\u0308", U"\u0378"});
//÷ 000A ÷ 0020 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] SPACE (Other) ÷ [0.3]
check_graphemes(U"\u000A\u0020", {U"\u000A", U"\u0020"});
//÷ 000A ÷ 0308 ÷ 0020 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] SPACE (Other) ÷ [0.3]
check_graphemes(U"\u000A\u0308\u0020", {U"\u000A", U"\u0308", U"\u0020"});
//÷ 000A ÷ 000D ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u000A\u000D", {U"\u000A", U"\u000D"});
//÷ 000A ÷ 0308 ÷ 000D ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u000A\u0308\u000D", {U"\u000A", U"\u0308", U"\u000D"});
//÷ 000A ÷ 000A ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u000A\u000A", {U"\u000A", U"\u000A"});
//÷ 000A ÷ 0308 ÷ 000A ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u000A\u0308\u000A", {U"\u000A", U"\u0308", U"\u000A"});
//÷ 000A ÷ 0001 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u000A\u0001", {U"\u000A", U"\u0001"});
//÷ 000A ÷ 0308 ÷ 0001 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u000A\u0308\u0001", {U"\u000A", U"\u0308", U"\u0001"});
//÷ 000A ÷ 034F ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] COMBINING GRAPHEME JOINER (Extend) ÷ [0.3]
check_graphemes(U"\u000A\u034F", {U"\u000A", U"\u034F"});
//÷ 000A ÷ 0308 × 034F ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] COMBINING GRAPHEME JOINER (Extend) ÷ [0.3]
check_graphemes(U"\u000A\u0308\u034F", {U"\u000A", U"\u0308\u034F"});
//÷ 000A ÷ 1F1E6 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u000A\U0001F1E6", {U"\u000A", U"\U0001F1E6"});
//÷ 000A ÷ 0308 ÷ 1F1E6 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u000A\u0308\U0001F1E6", {U"\u000A", U"\u0308", U"\U0001F1E6"});
//÷ 000A ÷ 0600 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] ARABIC NUMBER SIGN (Prepend) ÷ [0.3]
check_graphemes(U"\u000A\u0600", {U"\u000A", U"\u0600"});
//÷ 000A ÷ 0308 ÷ 0600 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] ARABIC NUMBER SIGN (Prepend) ÷ [0.3]
check_graphemes(U"\u000A\u0308\u0600", {U"\u000A", U"\u0308", U"\u0600"});
//÷ 000A ÷ 0903 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u000A\u0903", {U"\u000A", U"\u0903"});
//÷ 000A ÷ 0308 × 0903 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u000A\u0308\u0903", {U"\u000A", U"\u0308\u0903"});
//÷ 000A ÷ 1100 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u000A\u1100", {U"\u000A", U"\u1100"});
//÷ 000A ÷ 0308 ÷ 1100 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u000A\u0308\u1100", {U"\u000A", U"\u0308", U"\u1100"});
//÷ 000A ÷ 1160 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u000A\u1160", {U"\u000A", U"\u1160"});
//÷ 000A ÷ 0308 ÷ 1160 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u000A\u0308\u1160", {U"\u000A", U"\u0308", U"\u1160"});
//÷ 000A ÷ 11A8 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u000A\u11A8", {U"\u000A", U"\u11A8"});
//÷ 000A ÷ 0308 ÷ 11A8 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u000A\u0308\u11A8", {U"\u000A", U"\u0308", U"\u11A8"});
//÷ 000A ÷ AC00 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u000A\uAC00", {U"\u000A", U"\uAC00"});
//÷ 000A ÷ 0308 ÷ AC00 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u000A\u0308\uAC00", {U"\u000A", U"\u0308", U"\uAC00"});
//÷ 000A ÷ AC01 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u000A\uAC01", {U"\u000A", U"\uAC01"});
//÷ 000A ÷ 0308 ÷ AC01 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u000A\u0308\uAC01", {U"\u000A", U"\u0308", U"\uAC01"});
//÷ 000A ÷ 231A ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] WATCH (ExtPict) ÷ [0.3]
check_graphemes(U"\u000A\u231A", {U"\u000A", U"\u231A"});
//÷ 000A ÷ 0308 ÷ 231A ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] WATCH (ExtPict) ÷ [0.3]
check_graphemes(U"\u000A\u0308\u231A", {U"\u000A", U"\u0308", U"\u231A"});
//÷ 000A ÷ 0300 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u000A\u0300", {U"\u000A", U"\u0300"});
//÷ 000A ÷ 0308 × 0300 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u000A\u0308\u0300", {U"\u000A", U"\u0308\u0300"});
//÷ 000A ÷ 200D ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u000A\u200D", {U"\u000A", U"\u200D"});
//÷ 000A ÷ 0308 × 200D ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u000A\u0308\u200D", {U"\u000A", U"\u0308\u200D"});
//÷ 000A ÷ 0378 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] <reserved-0378> (Other) ÷ [0.3]
check_graphemes(U"\u000A\u0378", {U"\u000A", U"\u0378"});
//÷ 000A ÷ 0308 ÷ 0378 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] <reserved-0378> (Other) ÷ [0.3]
check_graphemes(U"\u000A\u0308\u0378", {U"\u000A", U"\u0308", U"\u0378"});
//÷ 0001 ÷ 0020 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] SPACE (Other) ÷ [0.3]
check_graphemes(U"\u0001\u0020", {U"\u0001", U"\u0020"});
//÷ 0001 ÷ 0308 ÷ 0020 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] SPACE (Other) ÷ [0.3]
check_graphemes(U"\u0001\u0308\u0020", {U"\u0001", U"\u0308", U"\u0020"});
//÷ 0001 ÷ 000D ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u0001\u000D", {U"\u0001", U"\u000D"});
//÷ 0001 ÷ 0308 ÷ 000D ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u0001\u0308\u000D", {U"\u0001", U"\u0308", U"\u000D"});
//÷ 0001 ÷ 000A ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u0001\u000A", {U"\u0001", U"\u000A"});
//÷ 0001 ÷ 0308 ÷ 000A ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u0001\u0308\u000A", {U"\u0001", U"\u0308", U"\u000A"});
//÷ 0001 ÷ 0001 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u0001\u0001", {U"\u0001", U"\u0001"});
//÷ 0001 ÷ 0308 ÷ 0001 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u0001\u0308\u0001", {U"\u0001", U"\u0308", U"\u0001"});
//÷ 0001 ÷ 034F ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] COMBINING GRAPHEME JOINER (Extend) ÷ [0.3]
check_graphemes(U"\u0001\u034F", {U"\u0001", U"\u034F"});
//÷ 0001 ÷ 0308 × 034F ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] COMBINING GRAPHEME JOINER (Extend) ÷ [0.3]
check_graphemes(U"\u0001\u0308\u034F", {U"\u0001", U"\u0308\u034F"});
//÷ 0001 ÷ 1F1E6 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u0001\U0001F1E6", {U"\u0001", U"\U0001F1E6"});
//÷ 0001 ÷ 0308 ÷ 1F1E6 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u0001\u0308\U0001F1E6", {U"\u0001", U"\u0308", U"\U0001F1E6"});
//÷ 0001 ÷ 0600 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] ARABIC NUMBER SIGN (Prepend) ÷ [0.3]
check_graphemes(U"\u0001\u0600", {U"\u0001", U"\u0600"});
//÷ 0001 ÷ 0308 ÷ 0600 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] ARABIC NUMBER SIGN (Prepend) ÷ [0.3]
check_graphemes(U"\u0001\u0308\u0600", {U"\u0001", U"\u0308", U"\u0600"});
//÷ 0001 ÷ 0903 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u0001\u0903", {U"\u0001", U"\u0903"});
//÷ 0001 ÷ 0308 × 0903 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u0001\u0308\u0903", {U"\u0001", U"\u0308\u0903"});
//÷ 0001 ÷ 1100 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u0001\u1100", {U"\u0001", U"\u1100"});
//÷ 0001 ÷ 0308 ÷ 1100 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u0001\u0308\u1100", {U"\u0001", U"\u0308", U"\u1100"});
//÷ 0001 ÷ 1160 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u0001\u1160", {U"\u0001", U"\u1160"});
//÷ 0001 ÷ 0308 ÷ 1160 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u0001\u0308\u1160", {U"\u0001", U"\u0308", U"\u1160"});
//÷ 0001 ÷ 11A8 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u0001\u11A8", {U"\u0001", U"\u11A8"});
//÷ 0001 ÷ 0308 ÷ 11A8 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u0001\u0308\u11A8", {U"\u0001", U"\u0308", U"\u11A8"});
//÷ 0001 ÷ AC00 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u0001\uAC00", {U"\u0001", U"\uAC00"});
//÷ 0001 ÷ 0308 ÷ AC00 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u0001\u0308\uAC00", {U"\u0001", U"\u0308", U"\uAC00"});
//÷ 0001 ÷ AC01 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u0001\uAC01", {U"\u0001", U"\uAC01"});
//÷ 0001 ÷ 0308 ÷ AC01 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u0001\u0308\uAC01", {U"\u0001", U"\u0308", U"\uAC01"});
//÷ 0001 ÷ 231A ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] WATCH (ExtPict) ÷ [0.3]
check_graphemes(U"\u0001\u231A", {U"\u0001", U"\u231A"});
//÷ 0001 ÷ 0308 ÷ 231A ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] WATCH (ExtPict) ÷ [0.3]
check_graphemes(U"\u0001\u0308\u231A", {U"\u0001", U"\u0308", U"\u231A"});
//÷ 0001 ÷ 0300 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u0001\u0300", {U"\u0001", U"\u0300"});
//÷ 0001 ÷ 0308 × 0300 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u0001\u0308\u0300", {U"\u0001", U"\u0308\u0300"});
//÷ 0001 ÷ 200D ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u0001\u200D", {U"\u0001", U"\u200D"});
//÷ 0001 ÷ 0308 × 200D ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u0001\u0308\u200D", {U"\u0001", U"\u0308\u200D"});
//÷ 0001 ÷ 0378 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] <reserved-0378> (Other) ÷ [0.3]
check_graphemes(U"\u0001\u0378", {U"\u0001", U"\u0378"});
//÷ 0001 ÷ 0308 ÷ 0378 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] <reserved-0378> (Other) ÷ [0.3]
check_graphemes(U"\u0001\u0308\u0378", {U"\u0001", U"\u0308", U"\u0378"});
//÷ 034F ÷ 0020 ÷
//÷ [0.2] COMBINING GRAPHEME JOINER (Extend) ÷ [999.0] SPACE (Other) ÷ [0.3]
check_graphemes(U"\u034F\u0020", {U"\u034F", U"\u0020"});
//÷ 034F × 0308 ÷ 0020 ÷
//÷ [0.2] COMBINING GRAPHEME JOINER (Extend) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] SPACE (Other) ÷ [0.3]
check_graphemes(U"\u034F\u0308\u0020", {U"\u034F\u0308", U"\u0020"});
//÷ 034F ÷ 000D ÷
//÷ [0.2] COMBINING GRAPHEME JOINER (Extend) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u034F\u000D", {U"\u034F", U"\u000D"});
//÷ 034F × 0308 ÷ 000D ÷
//÷ [0.2] COMBINING GRAPHEME JOINER (Extend) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u034F\u0308\u000D", {U"\u034F\u0308", U"\u000D"});
//÷ 034F ÷ 000A ÷
//÷ [0.2] COMBINING GRAPHEME JOINER (Extend) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u034F\u000A", {U"\u034F", U"\u000A"});
//÷ 034F × 0308 ÷ 000A ÷
//÷ [0.2] COMBINING GRAPHEME JOINER (Extend) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u034F\u0308\u000A", {U"\u034F\u0308", U"\u000A"});
//÷ 034F ÷ 0001 ÷
//÷ [0.2] COMBINING GRAPHEME JOINER (Extend) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u034F\u0001", {U"\u034F", U"\u0001"});
//÷ 034F × 0308 ÷ 0001 ÷
//÷ [0.2] COMBINING GRAPHEME JOINER (Extend) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u034F\u0308\u0001", {U"\u034F\u0308", U"\u0001"});
//÷ 034F × 034F ÷
//÷ [0.2] COMBINING GRAPHEME JOINER (Extend) × [9.0] COMBINING GRAPHEME JOINER (Extend) ÷ [0.3]
check_graphemes(U"\u034F\u034F", {U"\u034F\u034F"});
//÷ 034F × 0308 × 034F ÷
//÷ [0.2] COMBINING GRAPHEME JOINER (Extend) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] COMBINING GRAPHEME JOINER (Extend) ÷ [0.3]
check_graphemes(U"\u034F\u0308\u034F", {U"\u034F\u0308\u034F"});
//÷ 034F ÷ 1F1E6 ÷
//÷ [0.2] COMBINING GRAPHEME JOINER (Extend) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u034F\U0001F1E6", {U"\u034F", U"\U0001F1E6"});
//÷ 034F × 0308 ÷ 1F1E6 ÷
//÷ [0.2] COMBINING GRAPHEME JOINER (Extend) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u034F\u0308\U0001F1E6", {U"\u034F\u0308", U"\U0001F1E6"});
//÷ 034F ÷ 0600 ÷
//÷ [0.2] COMBINING GRAPHEME JOINER (Extend) ÷ [999.0] ARABIC NUMBER SIGN (Prepend) ÷ [0.3]
check_graphemes(U"\u034F\u0600", {U"\u034F", U"\u0600"});
//÷ 034F × 0308 ÷ 0600 ÷
//÷ [0.2] COMBINING GRAPHEME JOINER (Extend) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] ARABIC NUMBER SIGN (Prepend) ÷ [0.3]
check_graphemes(U"\u034F\u0308\u0600", {U"\u034F\u0308", U"\u0600"});
//÷ 034F × 0903 ÷
//÷ [0.2] COMBINING GRAPHEME JOINER (Extend) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u034F\u0903", {U"\u034F\u0903"});
//÷ 034F × 0308 × 0903 ÷
//÷ [0.2] COMBINING GRAPHEME JOINER (Extend) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u034F\u0308\u0903", {U"\u034F\u0308\u0903"});
//÷ 034F ÷ 1100 ÷
//÷ [0.2] COMBINING GRAPHEME JOINER (Extend) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u034F\u1100", {U"\u034F", U"\u1100"});
//÷ 034F × 0308 ÷ 1100 ÷
//÷ [0.2] COMBINING GRAPHEME JOINER (Extend) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u034F\u0308\u1100", {U"\u034F\u0308", U"\u1100"});
//÷ 034F ÷ 1160 ÷
//÷ [0.2] COMBINING GRAPHEME JOINER (Extend) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u034F\u1160", {U"\u034F", U"\u1160"});
//÷ 034F × 0308 ÷ 1160 ÷
//÷ [0.2] COMBINING GRAPHEME JOINER (Extend) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u034F\u0308\u1160", {U"\u034F\u0308", U"\u1160"});
//÷ 034F ÷ 11A8 ÷
//÷ [0.2] COMBINING GRAPHEME JOINER (Extend) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u034F\u11A8", {U"\u034F", U"\u11A8"});
//÷ 034F × 0308 ÷ 11A8 ÷
//÷ [0.2] COMBINING GRAPHEME JOINER (Extend) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u034F\u0308\u11A8", {U"\u034F\u0308", U"\u11A8"});
//÷ 034F ÷ AC00 ÷
//÷ [0.2] COMBINING GRAPHEME JOINER (Extend) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u034F\uAC00", {U"\u034F", U"\uAC00"});
//÷ 034F × 0308 ÷ AC00 ÷
//÷ [0.2] COMBINING GRAPHEME JOINER (Extend) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u034F\u0308\uAC00", {U"\u034F\u0308", U"\uAC00"});
//÷ 034F ÷ AC01 ÷
//÷ [0.2] COMBINING GRAPHEME JOINER (Extend) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u034F\uAC01", {U"\u034F", U"\uAC01"});
//÷ 034F × 0308 ÷ AC01 ÷
//÷ [0.2] COMBINING GRAPHEME JOINER (Extend) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u034F\u0308\uAC01", {U"\u034F\u0308", U"\uAC01"});
//÷ 034F ÷ 231A ÷
//÷ [0.2] COMBINING GRAPHEME JOINER (Extend) ÷ [999.0] WATCH (ExtPict) ÷ [0.3]
check_graphemes(U"\u034F\u231A", {U"\u034F", U"\u231A"});
//÷ 034F × 0308 ÷ 231A ÷
//÷ [0.2] COMBINING GRAPHEME JOINER (Extend) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] WATCH (ExtPict) ÷ [0.3]
check_graphemes(U"\u034F\u0308\u231A", {U"\u034F\u0308", U"\u231A"});
//÷ 034F × 0300 ÷
//÷ [0.2] COMBINING GRAPHEME JOINER (Extend) × [9.0] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u034F\u0300", {U"\u034F\u0300"});
//÷ 034F × 0308 × 0300 ÷
//÷ [0.2] COMBINING GRAPHEME JOINER (Extend) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u034F\u0308\u0300", {U"\u034F\u0308\u0300"});
//÷ 034F × 200D ÷
//÷ [0.2] COMBINING GRAPHEME JOINER (Extend) × [9.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u034F\u200D", {U"\u034F\u200D"});
//÷ 034F × 0308 × 200D ÷
//÷ [0.2] COMBINING GRAPHEME JOINER (Extend) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u034F\u0308\u200D", {U"\u034F\u0308\u200D"});
//÷ 034F ÷ 0378 ÷
//÷ [0.2] COMBINING GRAPHEME JOINER (Extend) ÷ [999.0] <reserved-0378> (Other) ÷ [0.3]
check_graphemes(U"\u034F\u0378", {U"\u034F", U"\u0378"});
//÷ 034F × 0308 ÷ 0378 ÷
//÷ [0.2] COMBINING GRAPHEME JOINER (Extend) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] <reserved-0378> (Other) ÷ [0.3]
check_graphemes(U"\u034F\u0308\u0378", {U"\u034F\u0308", U"\u0378"});
//÷ 1F1E6 ÷ 0020 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [999.0] SPACE (Other) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0020", {U"\U0001F1E6", U"\u0020"});
//÷ 1F1E6 × 0308 ÷ 0020 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] SPACE (Other) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0308\u0020", {U"\U0001F1E6\u0308", U"\u0020"});
//÷ 1F1E6 ÷ 000D ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u000D", {U"\U0001F1E6", U"\u000D"});
//÷ 1F1E6 × 0308 ÷ 000D ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0308\u000D", {U"\U0001F1E6\u0308", U"\u000D"});
//÷ 1F1E6 ÷ 000A ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u000A", {U"\U0001F1E6", U"\u000A"});
//÷ 1F1E6 × 0308 ÷ 000A ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0308\u000A", {U"\U0001F1E6\u0308", U"\u000A"});
//÷ 1F1E6 ÷ 0001 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0001", {U"\U0001F1E6", U"\u0001"});
//÷ 1F1E6 × 0308 ÷ 0001 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0308\u0001", {U"\U0001F1E6\u0308", U"\u0001"});
//÷ 1F1E6 × 034F ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] COMBINING GRAPHEME JOINER (Extend) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u034F", {U"\U0001F1E6\u034F"});
//÷ 1F1E6 × 0308 × 034F ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] COMBINING GRAPHEME JOINER (Extend) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0308\u034F", {U"\U0001F1E6\u0308\u034F"});
//÷ 1F1E6 × 1F1E6 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [12.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\U0001F1E6\U0001F1E6", {U"\U0001F1E6\U0001F1E6"});
//÷ 1F1E6 × 0308 ÷ 1F1E6 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0308\U0001F1E6", {U"\U0001F1E6\u0308", U"\U0001F1E6"});
//÷ 1F1E6 ÷ 0600 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [999.0] ARABIC NUMBER SIGN (Prepend) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0600", {U"\U0001F1E6", U"\u0600"});
//÷ 1F1E6 × 0308 ÷ 0600 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] ARABIC NUMBER SIGN (Prepend) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0308\u0600", {U"\U0001F1E6\u0308", U"\u0600"});
//÷ 1F1E6 × 0903 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0903", {U"\U0001F1E6\u0903"});
//÷ 1F1E6 × 0308 × 0903 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0308\u0903", {U"\U0001F1E6\u0308\u0903"});
//÷ 1F1E6 ÷ 1100 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u1100", {U"\U0001F1E6", U"\u1100"});
//÷ 1F1E6 × 0308 ÷ 1100 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0308\u1100", {U"\U0001F1E6\u0308", U"\u1100"});
//÷ 1F1E6 ÷ 1160 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u1160", {U"\U0001F1E6", U"\u1160"});
//÷ 1F1E6 × 0308 ÷ 1160 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0308\u1160", {U"\U0001F1E6\u0308", U"\u1160"});
//÷ 1F1E6 ÷ 11A8 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u11A8", {U"\U0001F1E6", U"\u11A8"});
//÷ 1F1E6 × 0308 ÷ 11A8 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0308\u11A8", {U"\U0001F1E6\u0308", U"\u11A8"});
//÷ 1F1E6 ÷ AC00 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\U0001F1E6\uAC00", {U"\U0001F1E6", U"\uAC00"});
//÷ 1F1E6 × 0308 ÷ AC00 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0308\uAC00", {U"\U0001F1E6\u0308", U"\uAC00"});
//÷ 1F1E6 ÷ AC01 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\U0001F1E6\uAC01", {U"\U0001F1E6", U"\uAC01"});
//÷ 1F1E6 × 0308 ÷ AC01 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0308\uAC01", {U"\U0001F1E6\u0308", U"\uAC01"});
//÷ 1F1E6 ÷ 231A ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [999.0] WATCH (ExtPict) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u231A", {U"\U0001F1E6", U"\u231A"});
//÷ 1F1E6 × 0308 ÷ 231A ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] WATCH (ExtPict) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0308\u231A", {U"\U0001F1E6\u0308", U"\u231A"});
//÷ 1F1E6 × 0300 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0300", {U"\U0001F1E6\u0300"});
//÷ 1F1E6 × 0308 × 0300 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0308\u0300", {U"\U0001F1E6\u0308\u0300"});
//÷ 1F1E6 × 200D ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u200D", {U"\U0001F1E6\u200D"});
//÷ 1F1E6 × 0308 × 200D ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0308\u200D", {U"\U0001F1E6\u0308\u200D"});
//÷ 1F1E6 ÷ 0378 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [999.0] <reserved-0378> (Other) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0378", {U"\U0001F1E6", U"\u0378"});
//÷ 1F1E6 × 0308 ÷ 0378 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] <reserved-0378> (Other) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0308\u0378", {U"\U0001F1E6\u0308", U"\u0378"});
//÷ 0600 × 0020 ÷
//÷ [0.2] ARABIC NUMBER SIGN (Prepend) × [9.2] SPACE (Other) ÷ [0.3]
check_graphemes(U"\u0600\u0020", {U"\u0600\u0020"});
//÷ 0600 × 0308 ÷ 0020 ÷
//÷ [0.2] ARABIC NUMBER SIGN (Prepend) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] SPACE (Other) ÷ [0.3]
check_graphemes(U"\u0600\u0308\u0020", {U"\u0600\u0308", U"\u0020"});
//÷ 0600 ÷ 000D ÷
//÷ [0.2] ARABIC NUMBER SIGN (Prepend) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u0600\u000D", {U"\u0600", U"\u000D"});
//÷ 0600 × 0308 ÷ 000D ÷
//÷ [0.2] ARABIC NUMBER SIGN (Prepend) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u0600\u0308\u000D", {U"\u0600\u0308", U"\u000D"});
//÷ 0600 ÷ 000A ÷
//÷ [0.2] ARABIC NUMBER SIGN (Prepend) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u0600\u000A", {U"\u0600", U"\u000A"});
//÷ 0600 × 0308 ÷ 000A ÷
//÷ [0.2] ARABIC NUMBER SIGN (Prepend) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u0600\u0308\u000A", {U"\u0600\u0308", U"\u000A"});
//÷ 0600 ÷ 0001 ÷
//÷ [0.2] ARABIC NUMBER SIGN (Prepend) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u0600\u0001", {U"\u0600", U"\u0001"});
//÷ 0600 × 0308 ÷ 0001 ÷
//÷ [0.2] ARABIC NUMBER SIGN (Prepend) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u0600\u0308\u0001", {U"\u0600\u0308", U"\u0001"});
//÷ 0600 × 034F ÷
//÷ [0.2] ARABIC NUMBER SIGN (Prepend) × [9.0] COMBINING GRAPHEME JOINER (Extend) ÷ [0.3]
check_graphemes(U"\u0600\u034F", {U"\u0600\u034F"});
//÷ 0600 × 0308 × 034F ÷
//÷ [0.2] ARABIC NUMBER SIGN (Prepend) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] COMBINING GRAPHEME JOINER (Extend) ÷ [0.3]
check_graphemes(U"\u0600\u0308\u034F", {U"\u0600\u0308\u034F"});
//÷ 0600 × 1F1E6 ÷
//÷ [0.2] ARABIC NUMBER SIGN (Prepend) × [9.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u0600\U0001F1E6", {U"\u0600\U0001F1E6"});
//÷ 0600 × 0308 ÷ 1F1E6 ÷
//÷ [0.2] ARABIC NUMBER SIGN (Prepend) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u0600\u0308\U0001F1E6", {U"\u0600\u0308", U"\U0001F1E6"});
//÷ 0600 × 0600 ÷
//÷ [0.2] ARABIC NUMBER SIGN (Prepend) × [9.2] ARABIC NUMBER SIGN (Prepend) ÷ [0.3]
check_graphemes(U"\u0600\u0600", {U"\u0600\u0600"});
//÷ 0600 × 0308 ÷ 0600 ÷
//÷ [0.2] ARABIC NUMBER SIGN (Prepend) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] ARABIC NUMBER SIGN (Prepend) ÷ [0.3]
check_graphemes(U"\u0600\u0308\u0600", {U"\u0600\u0308", U"\u0600"});
//÷ 0600 × 0903 ÷
//÷ [0.2] ARABIC NUMBER SIGN (Prepend) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u0600\u0903", {U"\u0600\u0903"});
//÷ 0600 × 0308 × 0903 ÷
//÷ [0.2] ARABIC NUMBER SIGN (Prepend) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u0600\u0308\u0903", {U"\u0600\u0308\u0903"});
//÷ 0600 × 1100 ÷
//÷ [0.2] ARABIC NUMBER SIGN (Prepend) × [9.2] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u0600\u1100", {U"\u0600\u1100"});
//÷ 0600 × 0308 ÷ 1100 ÷
//÷ [0.2] ARABIC NUMBER SIGN (Prepend) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u0600\u0308\u1100", {U"\u0600\u0308", U"\u1100"});
//÷ 0600 × 1160 ÷
//÷ [0.2] ARABIC NUMBER SIGN (Prepend) × [9.2] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u0600\u1160", {U"\u0600\u1160"});
//÷ 0600 × 0308 ÷ 1160 ÷
//÷ [0.2] ARABIC NUMBER SIGN (Prepend) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u0600\u0308\u1160", {U"\u0600\u0308", U"\u1160"});
//÷ 0600 × 11A8 ÷
//÷ [0.2] ARABIC NUMBER SIGN (Prepend) × [9.2] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u0600\u11A8", {U"\u0600\u11A8"});
//÷ 0600 × 0308 ÷ 11A8 ÷
//÷ [0.2] ARABIC NUMBER SIGN (Prepend) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u0600\u0308\u11A8", {U"\u0600\u0308", U"\u11A8"});
//÷ 0600 × AC00 ÷
//÷ [0.2] ARABIC NUMBER SIGN (Prepend) × [9.2] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u0600\uAC00", {U"\u0600\uAC00"});
//÷ 0600 × 0308 ÷ AC00 ÷
//÷ [0.2] ARABIC NUMBER SIGN (Prepend) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u0600\u0308\uAC00", {U"\u0600\u0308", U"\uAC00"});
//÷ 0600 × AC01 ÷
//÷ [0.2] ARABIC NUMBER SIGN (Prepend) × [9.2] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u0600\uAC01", {U"\u0600\uAC01"});
//÷ 0600 × 0308 ÷ AC01 ÷
//÷ [0.2] ARABIC NUMBER SIGN (Prepend) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u0600\u0308\uAC01", {U"\u0600\u0308", U"\uAC01"});
//÷ 0600 × 231A ÷
//÷ [0.2] ARABIC NUMBER SIGN (Prepend) × [9.2] WATCH (ExtPict) ÷ [0.3]
check_graphemes(U"\u0600\u231A", {U"\u0600\u231A"});
//÷ 0600 × 0308 ÷ 231A ÷
//÷ [0.2] ARABIC NUMBER SIGN (Prepend) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] WATCH (ExtPict) ÷ [0.3]
check_graphemes(U"\u0600\u0308\u231A", {U"\u0600\u0308", U"\u231A"});
//÷ 0600 × 0300 ÷
//÷ [0.2] ARABIC NUMBER SIGN (Prepend) × [9.0] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u0600\u0300", {U"\u0600\u0300"});
//÷ 0600 × 0308 × 0300 ÷
//÷ [0.2] ARABIC NUMBER SIGN (Prepend) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u0600\u0308\u0300", {U"\u0600\u0308\u0300"});
//÷ 0600 × 200D ÷
//÷ [0.2] ARABIC NUMBER SIGN (Prepend) × [9.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u0600\u200D", {U"\u0600\u200D"});
//÷ 0600 × 0308 × 200D ÷
//÷ [0.2] ARABIC NUMBER SIGN (Prepend) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u0600\u0308\u200D", {U"\u0600\u0308\u200D"});
//÷ 0600 × 0378 ÷
//÷ [0.2] ARABIC NUMBER SIGN (Prepend) × [9.2] <reserved-0378> (Other) ÷ [0.3]
check_graphemes(U"\u0600\u0378", {U"\u0600\u0378"});
//÷ 0600 × 0308 ÷ 0378 ÷
//÷ [0.2] ARABIC NUMBER SIGN (Prepend) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] <reserved-0378> (Other) ÷ [0.3]
check_graphemes(U"\u0600\u0308\u0378", {U"\u0600\u0308", U"\u0378"});
//÷ 0903 ÷ 0020 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [999.0] SPACE (Other) ÷ [0.3]
check_graphemes(U"\u0903\u0020", {U"\u0903", U"\u0020"});
//÷ 0903 × 0308 ÷ 0020 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] SPACE (Other) ÷ [0.3]
check_graphemes(U"\u0903\u0308\u0020", {U"\u0903\u0308", U"\u0020"});
//÷ 0903 ÷ 000D ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u0903\u000D", {U"\u0903", U"\u000D"});
//÷ 0903 × 0308 ÷ 000D ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u0903\u0308\u000D", {U"\u0903\u0308", U"\u000D"});
//÷ 0903 ÷ 000A ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u0903\u000A", {U"\u0903", U"\u000A"});
//÷ 0903 × 0308 ÷ 000A ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u0903\u0308\u000A", {U"\u0903\u0308", U"\u000A"});
//÷ 0903 ÷ 0001 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u0903\u0001", {U"\u0903", U"\u0001"});
//÷ 0903 × 0308 ÷ 0001 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u0903\u0308\u0001", {U"\u0903\u0308", U"\u0001"});
//÷ 0903 × 034F ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] COMBINING GRAPHEME JOINER (Extend) ÷ [0.3]
check_graphemes(U"\u0903\u034F", {U"\u0903\u034F"});
//÷ 0903 × 0308 × 034F ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] COMBINING GRAPHEME JOINER (Extend) ÷ [0.3]
check_graphemes(U"\u0903\u0308\u034F", {U"\u0903\u0308\u034F"});
//÷ 0903 ÷ 1F1E6 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u0903\U0001F1E6", {U"\u0903", U"\U0001F1E6"});
//÷ 0903 × 0308 ÷ 1F1E6 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u0903\u0308\U0001F1E6", {U"\u0903\u0308", U"\U0001F1E6"});
//÷ 0903 ÷ 0600 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [999.0] ARABIC NUMBER SIGN (Prepend) ÷ [0.3]
check_graphemes(U"\u0903\u0600", {U"\u0903", U"\u0600"});
//÷ 0903 × 0308 ÷ 0600 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] ARABIC NUMBER SIGN (Prepend) ÷ [0.3]
check_graphemes(U"\u0903\u0308\u0600", {U"\u0903\u0308", U"\u0600"});
//÷ 0903 × 0903 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u0903\u0903", {U"\u0903\u0903"});
//÷ 0903 × 0308 × 0903 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u0903\u0308\u0903", {U"\u0903\u0308\u0903"});
//÷ 0903 ÷ 1100 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u0903\u1100", {U"\u0903", U"\u1100"});
//÷ 0903 × 0308 ÷ 1100 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u0903\u0308\u1100", {U"\u0903\u0308", U"\u1100"});
//÷ 0903 ÷ 1160 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u0903\u1160", {U"\u0903", U"\u1160"});
//÷ 0903 × 0308 ÷ 1160 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u0903\u0308\u1160", {U"\u0903\u0308", U"\u1160"});
//÷ 0903 ÷ 11A8 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u0903\u11A8", {U"\u0903", U"\u11A8"});
//÷ 0903 × 0308 ÷ 11A8 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u0903\u0308\u11A8", {U"\u0903\u0308", U"\u11A8"});
//÷ 0903 ÷ AC00 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u0903\uAC00", {U"\u0903", U"\uAC00"});
//÷ 0903 × 0308 ÷ AC00 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u0903\u0308\uAC00", {U"\u0903\u0308", U"\uAC00"});
//÷ 0903 ÷ AC01 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u0903\uAC01", {U"\u0903", U"\uAC01"});
//÷ 0903 × 0308 ÷ AC01 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u0903\u0308\uAC01", {U"\u0903\u0308", U"\uAC01"});
//÷ 0903 ÷ 231A ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [999.0] WATCH (ExtPict) ÷ [0.3]
check_graphemes(U"\u0903\u231A", {U"\u0903", U"\u231A"});
//÷ 0903 × 0308 ÷ 231A ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] WATCH (ExtPict) ÷ [0.3]
check_graphemes(U"\u0903\u0308\u231A", {U"\u0903\u0308", U"\u231A"});
//÷ 0903 × 0300 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u0903\u0300", {U"\u0903\u0300"});
//÷ 0903 × 0308 × 0300 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u0903\u0308\u0300", {U"\u0903\u0308\u0300"});
//÷ 0903 × 200D ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u0903\u200D", {U"\u0903\u200D"});
//÷ 0903 × 0308 × 200D ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u0903\u0308\u200D", {U"\u0903\u0308\u200D"});
//÷ 0903 ÷ 0378 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [999.0] <reserved-0378> (Other) ÷ [0.3]
check_graphemes(U"\u0903\u0378", {U"\u0903", U"\u0378"});
//÷ 0903 × 0308 ÷ 0378 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] <reserved-0378> (Other) ÷ [0.3]
check_graphemes(U"\u0903\u0308\u0378", {U"\u0903\u0308", U"\u0378"});
//÷ 1100 ÷ 0020 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) ÷ [999.0] SPACE (Other) ÷ [0.3]
check_graphemes(U"\u1100\u0020", {U"\u1100", U"\u0020"});
//÷ 1100 × 0308 ÷ 0020 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] SPACE (Other) ÷ [0.3]
check_graphemes(U"\u1100\u0308\u0020", {U"\u1100\u0308", U"\u0020"});
//÷ 1100 ÷ 000D ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u1100\u000D", {U"\u1100", U"\u000D"});
//÷ 1100 × 0308 ÷ 000D ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u1100\u0308\u000D", {U"\u1100\u0308", U"\u000D"});
//÷ 1100 ÷ 000A ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u1100\u000A", {U"\u1100", U"\u000A"});
//÷ 1100 × 0308 ÷ 000A ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u1100\u0308\u000A", {U"\u1100\u0308", U"\u000A"});
//÷ 1100 ÷ 0001 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u1100\u0001", {U"\u1100", U"\u0001"});
//÷ 1100 × 0308 ÷ 0001 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u1100\u0308\u0001", {U"\u1100\u0308", U"\u0001"});
//÷ 1100 × 034F ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] COMBINING GRAPHEME JOINER (Extend) ÷ [0.3]
check_graphemes(U"\u1100\u034F", {U"\u1100\u034F"});
//÷ 1100 × 0308 × 034F ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] COMBINING GRAPHEME JOINER (Extend) ÷ [0.3]
check_graphemes(U"\u1100\u0308\u034F", {U"\u1100\u0308\u034F"});
//÷ 1100 ÷ 1F1E6 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u1100\U0001F1E6", {U"\u1100", U"\U0001F1E6"});
//÷ 1100 × 0308 ÷ 1F1E6 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u1100\u0308\U0001F1E6", {U"\u1100\u0308", U"\U0001F1E6"});
//÷ 1100 ÷ 0600 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) ÷ [999.0] ARABIC NUMBER SIGN (Prepend) ÷ [0.3]
check_graphemes(U"\u1100\u0600", {U"\u1100", U"\u0600"});
//÷ 1100 × 0308 ÷ 0600 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] ARABIC NUMBER SIGN (Prepend) ÷ [0.3]
check_graphemes(U"\u1100\u0308\u0600", {U"\u1100\u0308", U"\u0600"});
//÷ 1100 × 0903 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u1100\u0903", {U"\u1100\u0903"});
//÷ 1100 × 0308 × 0903 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u1100\u0308\u0903", {U"\u1100\u0308\u0903"});
//÷ 1100 × 1100 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [6.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u1100\u1100", {U"\u1100\u1100"});
//÷ 1100 × 0308 ÷ 1100 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u1100\u0308\u1100", {U"\u1100\u0308", U"\u1100"});
//÷ 1100 × 1160 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [6.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u1100\u1160", {U"\u1100\u1160"});
//÷ 1100 × 0308 ÷ 1160 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u1100\u0308\u1160", {U"\u1100\u0308", U"\u1160"});
//÷ 1100 ÷ 11A8 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u1100\u11A8", {U"\u1100", U"\u11A8"});
//÷ 1100 × 0308 ÷ 11A8 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u1100\u0308\u11A8", {U"\u1100\u0308", U"\u11A8"});
//÷ 1100 × AC00 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [6.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u1100\uAC00", {U"\u1100\uAC00"});
//÷ 1100 × 0308 ÷ AC00 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u1100\u0308\uAC00", {U"\u1100\u0308", U"\uAC00"});
//÷ 1100 × AC01 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [6.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u1100\uAC01", {U"\u1100\uAC01"});
//÷ 1100 × 0308 ÷ AC01 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u1100\u0308\uAC01", {U"\u1100\u0308", U"\uAC01"});
//÷ 1100 ÷ 231A ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) ÷ [999.0] WATCH (ExtPict) ÷ [0.3]
check_graphemes(U"\u1100\u231A", {U"\u1100", U"\u231A"});
//÷ 1100 × 0308 ÷ 231A ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] WATCH (ExtPict) ÷ [0.3]
check_graphemes(U"\u1100\u0308\u231A", {U"\u1100\u0308", U"\u231A"});
//÷ 1100 × 0300 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u1100\u0300", {U"\u1100\u0300"});
//÷ 1100 × 0308 × 0300 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u1100\u0308\u0300", {U"\u1100\u0308\u0300"});
//÷ 1100 × 200D ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u1100\u200D", {U"\u1100\u200D"});
//÷ 1100 × 0308 × 200D ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u1100\u0308\u200D", {U"\u1100\u0308\u200D"});
//÷ 1100 ÷ 0378 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) ÷ [999.0] <reserved-0378> (Other) ÷ [0.3]
check_graphemes(U"\u1100\u0378", {U"\u1100", U"\u0378"});
//÷ 1100 × 0308 ÷ 0378 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] <reserved-0378> (Other) ÷ [0.3]
check_graphemes(U"\u1100\u0308\u0378", {U"\u1100\u0308", U"\u0378"});
//÷ 1160 ÷ 0020 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) ÷ [999.0] SPACE (Other) ÷ [0.3]
check_graphemes(U"\u1160\u0020", {U"\u1160", U"\u0020"});
//÷ 1160 × 0308 ÷ 0020 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] SPACE (Other) ÷ [0.3]
check_graphemes(U"\u1160\u0308\u0020", {U"\u1160\u0308", U"\u0020"});
//÷ 1160 ÷ 000D ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u1160\u000D", {U"\u1160", U"\u000D"});
//÷ 1160 × 0308 ÷ 000D ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u1160\u0308\u000D", {U"\u1160\u0308", U"\u000D"});
//÷ 1160 ÷ 000A ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u1160\u000A", {U"\u1160", U"\u000A"});
//÷ 1160 × 0308 ÷ 000A ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u1160\u0308\u000A", {U"\u1160\u0308", U"\u000A"});
//÷ 1160 ÷ 0001 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u1160\u0001", {U"\u1160", U"\u0001"});
//÷ 1160 × 0308 ÷ 0001 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u1160\u0308\u0001", {U"\u1160\u0308", U"\u0001"});
//÷ 1160 × 034F ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] COMBINING GRAPHEME JOINER (Extend) ÷ [0.3]
check_graphemes(U"\u1160\u034F", {U"\u1160\u034F"});
//÷ 1160 × 0308 × 034F ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] COMBINING GRAPHEME JOINER (Extend) ÷ [0.3]
check_graphemes(U"\u1160\u0308\u034F", {U"\u1160\u0308\u034F"});
//÷ 1160 ÷ 1F1E6 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u1160\U0001F1E6", {U"\u1160", U"\U0001F1E6"});
//÷ 1160 × 0308 ÷ 1F1E6 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u1160\u0308\U0001F1E6", {U"\u1160\u0308", U"\U0001F1E6"});
//÷ 1160 ÷ 0600 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) ÷ [999.0] ARABIC NUMBER SIGN (Prepend) ÷ [0.3]
check_graphemes(U"\u1160\u0600", {U"\u1160", U"\u0600"});
//÷ 1160 × 0308 ÷ 0600 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] ARABIC NUMBER SIGN (Prepend) ÷ [0.3]
check_graphemes(U"\u1160\u0308\u0600", {U"\u1160\u0308", U"\u0600"});
//÷ 1160 × 0903 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u1160\u0903", {U"\u1160\u0903"});
//÷ 1160 × 0308 × 0903 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u1160\u0308\u0903", {U"\u1160\u0308\u0903"});
//÷ 1160 ÷ 1100 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u1160\u1100", {U"\u1160", U"\u1100"});
//÷ 1160 × 0308 ÷ 1100 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u1160\u0308\u1100", {U"\u1160\u0308", U"\u1100"});
//÷ 1160 × 1160 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [7.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u1160\u1160", {U"\u1160\u1160"});
//÷ 1160 × 0308 ÷ 1160 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u1160\u0308\u1160", {U"\u1160\u0308", U"\u1160"});
//÷ 1160 × 11A8 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [7.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u1160\u11A8", {U"\u1160\u11A8"});
//÷ 1160 × 0308 ÷ 11A8 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u1160\u0308\u11A8", {U"\u1160\u0308", U"\u11A8"});
//÷ 1160 ÷ AC00 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u1160\uAC00", {U"\u1160", U"\uAC00"});
//÷ 1160 × 0308 ÷ AC00 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u1160\u0308\uAC00", {U"\u1160\u0308", U"\uAC00"});
//÷ 1160 ÷ AC01 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u1160\uAC01", {U"\u1160", U"\uAC01"});
//÷ 1160 × 0308 ÷ AC01 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u1160\u0308\uAC01", {U"\u1160\u0308", U"\uAC01"});
//÷ 1160 ÷ 231A ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) ÷ [999.0] WATCH (ExtPict) ÷ [0.3]
check_graphemes(U"\u1160\u231A", {U"\u1160", U"\u231A"});
//÷ 1160 × 0308 ÷ 231A ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] WATCH (ExtPict) ÷ [0.3]
check_graphemes(U"\u1160\u0308\u231A", {U"\u1160\u0308", U"\u231A"});
//÷ 1160 × 0300 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u1160\u0300", {U"\u1160\u0300"});
//÷ 1160 × 0308 × 0300 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u1160\u0308\u0300", {U"\u1160\u0308\u0300"});
//÷ 1160 × 200D ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u1160\u200D", {U"\u1160\u200D"});
//÷ 1160 × 0308 × 200D ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u1160\u0308\u200D", {U"\u1160\u0308\u200D"});
//÷ 1160 ÷ 0378 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) ÷ [999.0] <reserved-0378> (Other) ÷ [0.3]
check_graphemes(U"\u1160\u0378", {U"\u1160", U"\u0378"});
//÷ 1160 × 0308 ÷ 0378 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] <reserved-0378> (Other) ÷ [0.3]
check_graphemes(U"\u1160\u0308\u0378", {U"\u1160\u0308", U"\u0378"});
//÷ 11A8 ÷ 0020 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) ÷ [999.0] SPACE (Other) ÷ [0.3]
check_graphemes(U"\u11A8\u0020", {U"\u11A8", U"\u0020"});
//÷ 11A8 × 0308 ÷ 0020 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] SPACE (Other) ÷ [0.3]
check_graphemes(U"\u11A8\u0308\u0020", {U"\u11A8\u0308", U"\u0020"});
//÷ 11A8 ÷ 000D ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u11A8\u000D", {U"\u11A8", U"\u000D"});
//÷ 11A8 × 0308 ÷ 000D ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u11A8\u0308\u000D", {U"\u11A8\u0308", U"\u000D"});
//÷ 11A8 ÷ 000A ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u11A8\u000A", {U"\u11A8", U"\u000A"});
//÷ 11A8 × 0308 ÷ 000A ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u11A8\u0308\u000A", {U"\u11A8\u0308", U"\u000A"});
//÷ 11A8 ÷ 0001 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u11A8\u0001", {U"\u11A8", U"\u0001"});
//÷ 11A8 × 0308 ÷ 0001 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u11A8\u0308\u0001", {U"\u11A8\u0308", U"\u0001"});
//÷ 11A8 × 034F ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] COMBINING GRAPHEME JOINER (Extend) ÷ [0.3]
check_graphemes(U"\u11A8\u034F", {U"\u11A8\u034F"});
//÷ 11A8 × 0308 × 034F ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] COMBINING GRAPHEME JOINER (Extend) ÷ [0.3]
check_graphemes(U"\u11A8\u0308\u034F", {U"\u11A8\u0308\u034F"});
//÷ 11A8 ÷ 1F1E6 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u11A8\U0001F1E6", {U"\u11A8", U"\U0001F1E6"});
//÷ 11A8 × 0308 ÷ 1F1E6 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u11A8\u0308\U0001F1E6", {U"\u11A8\u0308", U"\U0001F1E6"});
//÷ 11A8 ÷ 0600 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) ÷ [999.0] ARABIC NUMBER SIGN (Prepend) ÷ [0.3]
check_graphemes(U"\u11A8\u0600", {U"\u11A8", U"\u0600"});
//÷ 11A8 × 0308 ÷ 0600 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] ARABIC NUMBER SIGN (Prepend) ÷ [0.3]
check_graphemes(U"\u11A8\u0308\u0600", {U"\u11A8\u0308", U"\u0600"});
//÷ 11A8 × 0903 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u11A8\u0903", {U"\u11A8\u0903"});
//÷ 11A8 × 0308 × 0903 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u11A8\u0308\u0903", {U"\u11A8\u0308\u0903"});
//÷ 11A8 ÷ 1100 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u11A8\u1100", {U"\u11A8", U"\u1100"});
//÷ 11A8 × 0308 ÷ 1100 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u11A8\u0308\u1100", {U"\u11A8\u0308", U"\u1100"});
//÷ 11A8 ÷ 1160 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u11A8\u1160", {U"\u11A8", U"\u1160"});
//÷ 11A8 × 0308 ÷ 1160 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u11A8\u0308\u1160", {U"\u11A8\u0308", U"\u1160"});
//÷ 11A8 × 11A8 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [8.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u11A8\u11A8", {U"\u11A8\u11A8"});
//÷ 11A8 × 0308 ÷ 11A8 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u11A8\u0308\u11A8", {U"\u11A8\u0308", U"\u11A8"});
//÷ 11A8 ÷ AC00 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u11A8\uAC00", {U"\u11A8", U"\uAC00"});
//÷ 11A8 × 0308 ÷ AC00 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u11A8\u0308\uAC00", {U"\u11A8\u0308", U"\uAC00"});
//÷ 11A8 ÷ AC01 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u11A8\uAC01", {U"\u11A8", U"\uAC01"});
//÷ 11A8 × 0308 ÷ AC01 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u11A8\u0308\uAC01", {U"\u11A8\u0308", U"\uAC01"});
//÷ 11A8 ÷ 231A ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) ÷ [999.0] WATCH (ExtPict) ÷ [0.3]
check_graphemes(U"\u11A8\u231A", {U"\u11A8", U"\u231A"});
//÷ 11A8 × 0308 ÷ 231A ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] WATCH (ExtPict) ÷ [0.3]
check_graphemes(U"\u11A8\u0308\u231A", {U"\u11A8\u0308", U"\u231A"});
//÷ 11A8 × 0300 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u11A8\u0300", {U"\u11A8\u0300"});
//÷ 11A8 × 0308 × 0300 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u11A8\u0308\u0300", {U"\u11A8\u0308\u0300"});
//÷ 11A8 × 200D ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u11A8\u200D", {U"\u11A8\u200D"});
//÷ 11A8 × 0308 × 200D ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u11A8\u0308\u200D", {U"\u11A8\u0308\u200D"});
//÷ 11A8 ÷ 0378 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) ÷ [999.0] <reserved-0378> (Other) ÷ [0.3]
check_graphemes(U"\u11A8\u0378", {U"\u11A8", U"\u0378"});
//÷ 11A8 × 0308 ÷ 0378 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] <reserved-0378> (Other) ÷ [0.3]
check_graphemes(U"\u11A8\u0308\u0378", {U"\u11A8\u0308", U"\u0378"});
//÷ AC00 ÷ 0020 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) ÷ [999.0] SPACE (Other) ÷ [0.3]
check_graphemes(U"\uAC00\u0020", {U"\uAC00", U"\u0020"});
//÷ AC00 × 0308 ÷ 0020 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] SPACE (Other) ÷ [0.3]
check_graphemes(U"\uAC00\u0308\u0020", {U"\uAC00\u0308", U"\u0020"});
//÷ AC00 ÷ 000D ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\uAC00\u000D", {U"\uAC00", U"\u000D"});
//÷ AC00 × 0308 ÷ 000D ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\uAC00\u0308\u000D", {U"\uAC00\u0308", U"\u000D"});
//÷ AC00 ÷ 000A ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\uAC00\u000A", {U"\uAC00", U"\u000A"});
//÷ AC00 × 0308 ÷ 000A ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\uAC00\u0308\u000A", {U"\uAC00\u0308", U"\u000A"});
//÷ AC00 ÷ 0001 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\uAC00\u0001", {U"\uAC00", U"\u0001"});
//÷ AC00 × 0308 ÷ 0001 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\uAC00\u0308\u0001", {U"\uAC00\u0308", U"\u0001"});
//÷ AC00 × 034F ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] COMBINING GRAPHEME JOINER (Extend) ÷ [0.3]
check_graphemes(U"\uAC00\u034F", {U"\uAC00\u034F"});
//÷ AC00 × 0308 × 034F ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] COMBINING GRAPHEME JOINER (Extend) ÷ [0.3]
check_graphemes(U"\uAC00\u0308\u034F", {U"\uAC00\u0308\u034F"});
//÷ AC00 ÷ 1F1E6 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\uAC00\U0001F1E6", {U"\uAC00", U"\U0001F1E6"});
//÷ AC00 × 0308 ÷ 1F1E6 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\uAC00\u0308\U0001F1E6", {U"\uAC00\u0308", U"\U0001F1E6"});
//÷ AC00 ÷ 0600 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) ÷ [999.0] ARABIC NUMBER SIGN (Prepend) ÷ [0.3]
check_graphemes(U"\uAC00\u0600", {U"\uAC00", U"\u0600"});
//÷ AC00 × 0308 ÷ 0600 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] ARABIC NUMBER SIGN (Prepend) ÷ [0.3]
check_graphemes(U"\uAC00\u0308\u0600", {U"\uAC00\u0308", U"\u0600"});
//÷ AC00 × 0903 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\uAC00\u0903", {U"\uAC00\u0903"});
//÷ AC00 × 0308 × 0903 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\uAC00\u0308\u0903", {U"\uAC00\u0308\u0903"});
//÷ AC00 ÷ 1100 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\uAC00\u1100", {U"\uAC00", U"\u1100"});
//÷ AC00 × 0308 ÷ 1100 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\uAC00\u0308\u1100", {U"\uAC00\u0308", U"\u1100"});
//÷ AC00 × 1160 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [7.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\uAC00\u1160", {U"\uAC00\u1160"});
//÷ AC00 × 0308 ÷ 1160 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\uAC00\u0308\u1160", {U"\uAC00\u0308", U"\u1160"});
//÷ AC00 × 11A8 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [7.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\uAC00\u11A8", {U"\uAC00\u11A8"});
//÷ AC00 × 0308 ÷ 11A8 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\uAC00\u0308\u11A8", {U"\uAC00\u0308", U"\u11A8"});
//÷ AC00 ÷ AC00 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\uAC00\uAC00", {U"\uAC00", U"\uAC00"});
//÷ AC00 × 0308 ÷ AC00 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\uAC00\u0308\uAC00", {U"\uAC00\u0308", U"\uAC00"});
//÷ AC00 ÷ AC01 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\uAC00\uAC01", {U"\uAC00", U"\uAC01"});
//÷ AC00 × 0308 ÷ AC01 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\uAC00\u0308\uAC01", {U"\uAC00\u0308", U"\uAC01"});
//÷ AC00 ÷ 231A ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) ÷ [999.0] WATCH (ExtPict) ÷ [0.3]
check_graphemes(U"\uAC00\u231A", {U"\uAC00", U"\u231A"});
//÷ AC00 × 0308 ÷ 231A ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] WATCH (ExtPict) ÷ [0.3]
check_graphemes(U"\uAC00\u0308\u231A", {U"\uAC00\u0308", U"\u231A"});
//÷ AC00 × 0300 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\uAC00\u0300", {U"\uAC00\u0300"});
//÷ AC00 × 0308 × 0300 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\uAC00\u0308\u0300", {U"\uAC00\u0308\u0300"});
//÷ AC00 × 200D ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\uAC00\u200D", {U"\uAC00\u200D"});
//÷ AC00 × 0308 × 200D ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\uAC00\u0308\u200D", {U"\uAC00\u0308\u200D"});
//÷ AC00 ÷ 0378 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) ÷ [999.0] <reserved-0378> (Other) ÷ [0.3]
check_graphemes(U"\uAC00\u0378", {U"\uAC00", U"\u0378"});
//÷ AC00 × 0308 ÷ 0378 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] <reserved-0378> (Other) ÷ [0.3]
check_graphemes(U"\uAC00\u0308\u0378", {U"\uAC00\u0308", U"\u0378"});
//÷ AC01 ÷ 0020 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) ÷ [999.0] SPACE (Other) ÷ [0.3]
check_graphemes(U"\uAC01\u0020", {U"\uAC01", U"\u0020"});
//÷ AC01 × 0308 ÷ 0020 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] SPACE (Other) ÷ [0.3]
check_graphemes(U"\uAC01\u0308\u0020", {U"\uAC01\u0308", U"\u0020"});
//÷ AC01 ÷ 000D ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\uAC01\u000D", {U"\uAC01", U"\u000D"});
//÷ AC01 × 0308 ÷ 000D ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\uAC01\u0308\u000D", {U"\uAC01\u0308", U"\u000D"});
//÷ AC01 ÷ 000A ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\uAC01\u000A", {U"\uAC01", U"\u000A"});
//÷ AC01 × 0308 ÷ 000A ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\uAC01\u0308\u000A", {U"\uAC01\u0308", U"\u000A"});
//÷ AC01 ÷ 0001 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\uAC01\u0001", {U"\uAC01", U"\u0001"});
//÷ AC01 × 0308 ÷ 0001 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\uAC01\u0308\u0001", {U"\uAC01\u0308", U"\u0001"});
//÷ AC01 × 034F ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] COMBINING GRAPHEME JOINER (Extend) ÷ [0.3]
check_graphemes(U"\uAC01\u034F", {U"\uAC01\u034F"});
//÷ AC01 × 0308 × 034F ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] COMBINING GRAPHEME JOINER (Extend) ÷ [0.3]
check_graphemes(U"\uAC01\u0308\u034F", {U"\uAC01\u0308\u034F"});
//÷ AC01 ÷ 1F1E6 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\uAC01\U0001F1E6", {U"\uAC01", U"\U0001F1E6"});
//÷ AC01 × 0308 ÷ 1F1E6 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\uAC01\u0308\U0001F1E6", {U"\uAC01\u0308", U"\U0001F1E6"});
//÷ AC01 ÷ 0600 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) ÷ [999.0] ARABIC NUMBER SIGN (Prepend) ÷ [0.3]
check_graphemes(U"\uAC01\u0600", {U"\uAC01", U"\u0600"});
//÷ AC01 × 0308 ÷ 0600 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] ARABIC NUMBER SIGN (Prepend) ÷ [0.3]
check_graphemes(U"\uAC01\u0308\u0600", {U"\uAC01\u0308", U"\u0600"});
//÷ AC01 × 0903 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\uAC01\u0903", {U"\uAC01\u0903"});
//÷ AC01 × 0308 × 0903 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\uAC01\u0308\u0903", {U"\uAC01\u0308\u0903"});
//÷ AC01 ÷ 1100 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\uAC01\u1100", {U"\uAC01", U"\u1100"});
//÷ AC01 × 0308 ÷ 1100 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\uAC01\u0308\u1100", {U"\uAC01\u0308", U"\u1100"});
//÷ AC01 ÷ 1160 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\uAC01\u1160", {U"\uAC01", U"\u1160"});
//÷ AC01 × 0308 ÷ 1160 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\uAC01\u0308\u1160", {U"\uAC01\u0308", U"\u1160"});
//÷ AC01 × 11A8 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [8.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\uAC01\u11A8", {U"\uAC01\u11A8"});
//÷ AC01 × 0308 ÷ 11A8 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\uAC01\u0308\u11A8", {U"\uAC01\u0308", U"\u11A8"});
//÷ AC01 ÷ AC00 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\uAC01\uAC00", {U"\uAC01", U"\uAC00"});
//÷ AC01 × 0308 ÷ AC00 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\uAC01\u0308\uAC00", {U"\uAC01\u0308", U"\uAC00"});
//÷ AC01 ÷ AC01 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\uAC01\uAC01", {U"\uAC01", U"\uAC01"});
//÷ AC01 × 0308 ÷ AC01 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\uAC01\u0308\uAC01", {U"\uAC01\u0308", U"\uAC01"});
//÷ AC01 ÷ 231A ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) ÷ [999.0] WATCH (ExtPict) ÷ [0.3]
check_graphemes(U"\uAC01\u231A", {U"\uAC01", U"\u231A"});
//÷ AC01 × 0308 ÷ 231A ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] WATCH (ExtPict) ÷ [0.3]
check_graphemes(U"\uAC01\u0308\u231A", {U"\uAC01\u0308", U"\u231A"});
//÷ AC01 × 0300 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\uAC01\u0300", {U"\uAC01\u0300"});
//÷ AC01 × 0308 × 0300 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\uAC01\u0308\u0300", {U"\uAC01\u0308\u0300"});
//÷ AC01 × 200D ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\uAC01\u200D", {U"\uAC01\u200D"});
//÷ AC01 × 0308 × 200D ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\uAC01\u0308\u200D", {U"\uAC01\u0308\u200D"});
//÷ AC01 ÷ 0378 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) ÷ [999.0] <reserved-0378> (Other) ÷ [0.3]
check_graphemes(U"\uAC01\u0378", {U"\uAC01", U"\u0378"});
//÷ AC01 × 0308 ÷ 0378 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] <reserved-0378> (Other) ÷ [0.3]
check_graphemes(U"\uAC01\u0308\u0378", {U"\uAC01\u0308", U"\u0378"});
//÷ 231A ÷ 0020 ÷
//÷ [0.2] WATCH (ExtPict) ÷ [999.0] SPACE (Other) ÷ [0.3]
check_graphemes(U"\u231A\u0020", {U"\u231A", U"\u0020"});
//÷ 231A × 0308 ÷ 0020 ÷
//÷ [0.2] WATCH (ExtPict) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] SPACE (Other) ÷ [0.3]
check_graphemes(U"\u231A\u0308\u0020", {U"\u231A\u0308", U"\u0020"});
//÷ 231A ÷ 000D ÷
//÷ [0.2] WATCH (ExtPict) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u231A\u000D", {U"\u231A", U"\u000D"});
//÷ 231A × 0308 ÷ 000D ÷
//÷ [0.2] WATCH (ExtPict) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u231A\u0308\u000D", {U"\u231A\u0308", U"\u000D"});
//÷ 231A ÷ 000A ÷
//÷ [0.2] WATCH (ExtPict) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u231A\u000A", {U"\u231A", U"\u000A"});
//÷ 231A × 0308 ÷ 000A ÷
//÷ [0.2] WATCH (ExtPict) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u231A\u0308\u000A", {U"\u231A\u0308", U"\u000A"});
//÷ 231A ÷ 0001 ÷
//÷ [0.2] WATCH (ExtPict) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u231A\u0001", {U"\u231A", U"\u0001"});
//÷ 231A × 0308 ÷ 0001 ÷
//÷ [0.2] WATCH (ExtPict) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u231A\u0308\u0001", {U"\u231A\u0308", U"\u0001"});
//÷ 231A × 034F ÷
//÷ [0.2] WATCH (ExtPict) × [9.0] COMBINING GRAPHEME JOINER (Extend) ÷ [0.3]
check_graphemes(U"\u231A\u034F", {U"\u231A\u034F"});
//÷ 231A × 0308 × 034F ÷
//÷ [0.2] WATCH (ExtPict) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] COMBINING GRAPHEME JOINER (Extend) ÷ [0.3]
check_graphemes(U"\u231A\u0308\u034F", {U"\u231A\u0308\u034F"});
//÷ 231A ÷ 1F1E6 ÷
//÷ [0.2] WATCH (ExtPict) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u231A\U0001F1E6", {U"\u231A", U"\U0001F1E6"});
//÷ 231A × 0308 ÷ 1F1E6 ÷
//÷ [0.2] WATCH (ExtPict) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u231A\u0308\U0001F1E6", {U"\u231A\u0308", U"\U0001F1E6"});
//÷ 231A ÷ 0600 ÷
//÷ [0.2] WATCH (ExtPict) ÷ [999.0] ARABIC NUMBER SIGN (Prepend) ÷ [0.3]
check_graphemes(U"\u231A\u0600", {U"\u231A", U"\u0600"});
//÷ 231A × 0308 ÷ 0600 ÷
//÷ [0.2] WATCH (ExtPict) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] ARABIC NUMBER SIGN (Prepend) ÷ [0.3]
check_graphemes(U"\u231A\u0308\u0600", {U"\u231A\u0308", U"\u0600"});
//÷ 231A × 0903 ÷
//÷ [0.2] WATCH (ExtPict) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u231A\u0903", {U"\u231A\u0903"});
//÷ 231A × 0308 × 0903 ÷
//÷ [0.2] WATCH (ExtPict) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u231A\u0308\u0903", {U"\u231A\u0308\u0903"});
//÷ 231A ÷ 1100 ÷
//÷ [0.2] WATCH (ExtPict) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u231A\u1100", {U"\u231A", U"\u1100"});
//÷ 231A × 0308 ÷ 1100 ÷
//÷ [0.2] WATCH (ExtPict) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u231A\u0308\u1100", {U"\u231A\u0308", U"\u1100"});
//÷ 231A ÷ 1160 ÷
//÷ [0.2] WATCH (ExtPict) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u231A\u1160", {U"\u231A", U"\u1160"});
//÷ 231A × 0308 ÷ 1160 ÷
//÷ [0.2] WATCH (ExtPict) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u231A\u0308\u1160", {U"\u231A\u0308", U"\u1160"});
//÷ 231A ÷ 11A8 ÷
//÷ [0.2] WATCH (ExtPict) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u231A\u11A8", {U"\u231A", U"\u11A8"});
//÷ 231A × 0308 ÷ 11A8 ÷
//÷ [0.2] WATCH (ExtPict) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u231A\u0308\u11A8", {U"\u231A\u0308", U"\u11A8"});
//÷ 231A ÷ AC00 ÷
//÷ [0.2] WATCH (ExtPict) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u231A\uAC00", {U"\u231A", U"\uAC00"});
//÷ 231A × 0308 ÷ AC00 ÷
//÷ [0.2] WATCH (ExtPict) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u231A\u0308\uAC00", {U"\u231A\u0308", U"\uAC00"});
//÷ 231A ÷ AC01 ÷
//÷ [0.2] WATCH (ExtPict) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u231A\uAC01", {U"\u231A", U"\uAC01"});
//÷ 231A × 0308 ÷ AC01 ÷
//÷ [0.2] WATCH (ExtPict) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u231A\u0308\uAC01", {U"\u231A\u0308", U"\uAC01"});
//÷ 231A ÷ 231A ÷
//÷ [0.2] WATCH (ExtPict) ÷ [999.0] WATCH (ExtPict) ÷ [0.3]
check_graphemes(U"\u231A\u231A", {U"\u231A", U"\u231A"});
//÷ 231A × 0308 ÷ 231A ÷
//÷ [0.2] WATCH (ExtPict) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] WATCH (ExtPict) ÷ [0.3]
check_graphemes(U"\u231A\u0308\u231A", {U"\u231A\u0308", U"\u231A"});
//÷ 231A × 0300 ÷
//÷ [0.2] WATCH (ExtPict) × [9.0] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u231A\u0300", {U"\u231A\u0300"});
//÷ 231A × 0308 × 0300 ÷
//÷ [0.2] WATCH (ExtPict) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u231A\u0308\u0300", {U"\u231A\u0308\u0300"});
//÷ 231A × 200D ÷
//÷ [0.2] WATCH (ExtPict) × [9.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u231A\u200D", {U"\u231A\u200D"});
//÷ 231A × 0308 × 200D ÷
//÷ [0.2] WATCH (ExtPict) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u231A\u0308\u200D", {U"\u231A\u0308\u200D"});
//÷ 231A ÷ 0378 ÷
//÷ [0.2] WATCH (ExtPict) ÷ [999.0] <reserved-0378> (Other) ÷ [0.3]
check_graphemes(U"\u231A\u0378", {U"\u231A", U"\u0378"});
//÷ 231A × 0308 ÷ 0378 ÷
//÷ [0.2] WATCH (ExtPict) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] <reserved-0378> (Other) ÷ [0.3]
check_graphemes(U"\u231A\u0308\u0378", {U"\u231A\u0308", U"\u0378"});
//÷ 0300 ÷ 0020 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [999.0] SPACE (Other) ÷ [0.3]
check_graphemes(U"\u0300\u0020", {U"\u0300", U"\u0020"});
//÷ 0300 × 0308 ÷ 0020 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] SPACE (Other) ÷ [0.3]
check_graphemes(U"\u0300\u0308\u0020", {U"\u0300\u0308", U"\u0020"});
//÷ 0300 ÷ 000D ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u0300\u000D", {U"\u0300", U"\u000D"});
//÷ 0300 × 0308 ÷ 000D ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u0300\u0308\u000D", {U"\u0300\u0308", U"\u000D"});
//÷ 0300 ÷ 000A ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u0300\u000A", {U"\u0300", U"\u000A"});
//÷ 0300 × 0308 ÷ 000A ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u0300\u0308\u000A", {U"\u0300\u0308", U"\u000A"});
//÷ 0300 ÷ 0001 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u0300\u0001", {U"\u0300", U"\u0001"});
//÷ 0300 × 0308 ÷ 0001 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u0300\u0308\u0001", {U"\u0300\u0308", U"\u0001"});
//÷ 0300 × 034F ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) × [9.0] COMBINING GRAPHEME JOINER (Extend) ÷ [0.3]
check_graphemes(U"\u0300\u034F", {U"\u0300\u034F"});
//÷ 0300 × 0308 × 034F ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] COMBINING GRAPHEME JOINER (Extend) ÷ [0.3]
check_graphemes(U"\u0300\u0308\u034F", {U"\u0300\u0308\u034F"});
//÷ 0300 ÷ 1F1E6 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u0300\U0001F1E6", {U"\u0300", U"\U0001F1E6"});
//÷ 0300 × 0308 ÷ 1F1E6 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u0300\u0308\U0001F1E6", {U"\u0300\u0308", U"\U0001F1E6"});
//÷ 0300 ÷ 0600 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [999.0] ARABIC NUMBER SIGN (Prepend) ÷ [0.3]
check_graphemes(U"\u0300\u0600", {U"\u0300", U"\u0600"});
//÷ 0300 × 0308 ÷ 0600 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] ARABIC NUMBER SIGN (Prepend) ÷ [0.3]
check_graphemes(U"\u0300\u0308\u0600", {U"\u0300\u0308", U"\u0600"});
//÷ 0300 × 0903 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u0300\u0903", {U"\u0300\u0903"});
//÷ 0300 × 0308 × 0903 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u0300\u0308\u0903", {U"\u0300\u0308\u0903"});
//÷ 0300 ÷ 1100 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u0300\u1100", {U"\u0300", U"\u1100"});
//÷ 0300 × 0308 ÷ 1100 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u0300\u0308\u1100", {U"\u0300\u0308", U"\u1100"});
//÷ 0300 ÷ 1160 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u0300\u1160", {U"\u0300", U"\u1160"});
//÷ 0300 × 0308 ÷ 1160 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u0300\u0308\u1160", {U"\u0300\u0308", U"\u1160"});
//÷ 0300 ÷ 11A8 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u0300\u11A8", {U"\u0300", U"\u11A8"});
//÷ 0300 × 0308 ÷ 11A8 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u0300\u0308\u11A8", {U"\u0300\u0308", U"\u11A8"});
//÷ 0300 ÷ AC00 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u0300\uAC00", {U"\u0300", U"\uAC00"});
//÷ 0300 × 0308 ÷ AC00 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u0300\u0308\uAC00", {U"\u0300\u0308", U"\uAC00"});
//÷ 0300 ÷ AC01 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u0300\uAC01", {U"\u0300", U"\uAC01"});
//÷ 0300 × 0308 ÷ AC01 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u0300\u0308\uAC01", {U"\u0300\u0308", U"\uAC01"});
//÷ 0300 ÷ 231A ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [999.0] WATCH (ExtPict) ÷ [0.3]
check_graphemes(U"\u0300\u231A", {U"\u0300", U"\u231A"});
//÷ 0300 × 0308 ÷ 231A ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] WATCH (ExtPict) ÷ [0.3]
check_graphemes(U"\u0300\u0308\u231A", {U"\u0300\u0308", U"\u231A"});
//÷ 0300 × 0300 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) × [9.0] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u0300\u0300", {U"\u0300\u0300"});
//÷ 0300 × 0308 × 0300 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u0300\u0308\u0300", {U"\u0300\u0308\u0300"});
//÷ 0300 × 200D ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) × [9.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u0300\u200D", {U"\u0300\u200D"});
//÷ 0300 × 0308 × 200D ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u0300\u0308\u200D", {U"\u0300\u0308\u200D"});
//÷ 0300 ÷ 0378 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [999.0] <reserved-0378> (Other) ÷ [0.3]
check_graphemes(U"\u0300\u0378", {U"\u0300", U"\u0378"});
//÷ 0300 × 0308 ÷ 0378 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] <reserved-0378> (Other) ÷ [0.3]
check_graphemes(U"\u0300\u0308\u0378", {U"\u0300\u0308", U"\u0378"});
//÷ 200D ÷ 0020 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [999.0] SPACE (Other) ÷ [0.3]
check_graphemes(U"\u200D\u0020", {U"\u200D", U"\u0020"});
//÷ 200D × 0308 ÷ 0020 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] SPACE (Other) ÷ [0.3]
check_graphemes(U"\u200D\u0308\u0020", {U"\u200D\u0308", U"\u0020"});
//÷ 200D ÷ 000D ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u200D\u000D", {U"\u200D", U"\u000D"});
//÷ 200D × 0308 ÷ 000D ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u200D\u0308\u000D", {U"\u200D\u0308", U"\u000D"});
//÷ 200D ÷ 000A ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u200D\u000A", {U"\u200D", U"\u000A"});
//÷ 200D × 0308 ÷ 000A ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u200D\u0308\u000A", {U"\u200D\u0308", U"\u000A"});
//÷ 200D ÷ 0001 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u200D\u0001", {U"\u200D", U"\u0001"});
//÷ 200D × 0308 ÷ 0001 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u200D\u0308\u0001", {U"\u200D\u0308", U"\u0001"});
//÷ 200D × 034F ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) × [9.0] COMBINING GRAPHEME JOINER (Extend) ÷ [0.3]
check_graphemes(U"\u200D\u034F", {U"\u200D\u034F"});
//÷ 200D × 0308 × 034F ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] COMBINING GRAPHEME JOINER (Extend) ÷ [0.3]
check_graphemes(U"\u200D\u0308\u034F", {U"\u200D\u0308\u034F"});
//÷ 200D ÷ 1F1E6 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u200D\U0001F1E6", {U"\u200D", U"\U0001F1E6"});
//÷ 200D × 0308 ÷ 1F1E6 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u200D\u0308\U0001F1E6", {U"\u200D\u0308", U"\U0001F1E6"});
//÷ 200D ÷ 0600 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [999.0] ARABIC NUMBER SIGN (Prepend) ÷ [0.3]
check_graphemes(U"\u200D\u0600", {U"\u200D", U"\u0600"});
//÷ 200D × 0308 ÷ 0600 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] ARABIC NUMBER SIGN (Prepend) ÷ [0.3]
check_graphemes(U"\u200D\u0308\u0600", {U"\u200D\u0308", U"\u0600"});
//÷ 200D × 0903 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u200D\u0903", {U"\u200D\u0903"});
//÷ 200D × 0308 × 0903 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u200D\u0308\u0903", {U"\u200D\u0308\u0903"});
//÷ 200D ÷ 1100 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u200D\u1100", {U"\u200D", U"\u1100"});
//÷ 200D × 0308 ÷ 1100 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u200D\u0308\u1100", {U"\u200D\u0308", U"\u1100"});
//÷ 200D ÷ 1160 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u200D\u1160", {U"\u200D", U"\u1160"});
//÷ 200D × 0308 ÷ 1160 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u200D\u0308\u1160", {U"\u200D\u0308", U"\u1160"});
//÷ 200D ÷ 11A8 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u200D\u11A8", {U"\u200D", U"\u11A8"});
//÷ 200D × 0308 ÷ 11A8 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u200D\u0308\u11A8", {U"\u200D\u0308", U"\u11A8"});
//÷ 200D ÷ AC00 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u200D\uAC00", {U"\u200D", U"\uAC00"});
//÷ 200D × 0308 ÷ AC00 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u200D\u0308\uAC00", {U"\u200D\u0308", U"\uAC00"});
//÷ 200D ÷ AC01 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u200D\uAC01", {U"\u200D", U"\uAC01"});
//÷ 200D × 0308 ÷ AC01 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u200D\u0308\uAC01", {U"\u200D\u0308", U"\uAC01"});
//÷ 200D ÷ 231A ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [999.0] WATCH (ExtPict) ÷ [0.3]
check_graphemes(U"\u200D\u231A", {U"\u200D", U"\u231A"});
//÷ 200D × 0308 ÷ 231A ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] WATCH (ExtPict) ÷ [0.3]
check_graphemes(U"\u200D\u0308\u231A", {U"\u200D\u0308", U"\u231A"});
//÷ 200D × 0300 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) × [9.0] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u200D\u0300", {U"\u200D\u0300"});
//÷ 200D × 0308 × 0300 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u200D\u0308\u0300", {U"\u200D\u0308\u0300"});
//÷ 200D × 200D ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) × [9.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u200D\u200D", {U"\u200D\u200D"});
//÷ 200D × 0308 × 200D ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u200D\u0308\u200D", {U"\u200D\u0308\u200D"});
//÷ 200D ÷ 0378 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [999.0] <reserved-0378> (Other) ÷ [0.3]
check_graphemes(U"\u200D\u0378", {U"\u200D", U"\u0378"});
//÷ 200D × 0308 ÷ 0378 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] <reserved-0378> (Other) ÷ [0.3]
check_graphemes(U"\u200D\u0308\u0378", {U"\u200D\u0308", U"\u0378"});
//÷ 0378 ÷ 0020 ÷
//÷ [0.2] <reserved-0378> (Other) ÷ [999.0] SPACE (Other) ÷ [0.3]
check_graphemes(U"\u0378\u0020", {U"\u0378", U"\u0020"});
//÷ 0378 × 0308 ÷ 0020 ÷
//÷ [0.2] <reserved-0378> (Other) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] SPACE (Other) ÷ [0.3]
check_graphemes(U"\u0378\u0308\u0020", {U"\u0378\u0308", U"\u0020"});
//÷ 0378 ÷ 000D ÷
//÷ [0.2] <reserved-0378> (Other) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u0378\u000D", {U"\u0378", U"\u000D"});
//÷ 0378 × 0308 ÷ 000D ÷
//÷ [0.2] <reserved-0378> (Other) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u0378\u0308\u000D", {U"\u0378\u0308", U"\u000D"});
//÷ 0378 ÷ 000A ÷
//÷ [0.2] <reserved-0378> (Other) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u0378\u000A", {U"\u0378", U"\u000A"});
//÷ 0378 × 0308 ÷ 000A ÷
//÷ [0.2] <reserved-0378> (Other) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u0378\u0308\u000A", {U"\u0378\u0308", U"\u000A"});
//÷ 0378 ÷ 0001 ÷
//÷ [0.2] <reserved-0378> (Other) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u0378\u0001", {U"\u0378", U"\u0001"});
//÷ 0378 × 0308 ÷ 0001 ÷
//÷ [0.2] <reserved-0378> (Other) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u0378\u0308\u0001", {U"\u0378\u0308", U"\u0001"});
//÷ 0378 × 034F ÷
//÷ [0.2] <reserved-0378> (Other) × [9.0] COMBINING GRAPHEME JOINER (Extend) ÷ [0.3]
check_graphemes(U"\u0378\u034F", {U"\u0378\u034F"});
//÷ 0378 × 0308 × 034F ÷
//÷ [0.2] <reserved-0378> (Other) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] COMBINING GRAPHEME JOINER (Extend) ÷ [0.3]
check_graphemes(U"\u0378\u0308\u034F", {U"\u0378\u0308\u034F"});
//÷ 0378 ÷ 1F1E6 ÷
//÷ [0.2] <reserved-0378> (Other) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u0378\U0001F1E6", {U"\u0378", U"\U0001F1E6"});
//÷ 0378 × 0308 ÷ 1F1E6 ÷
//÷ [0.2] <reserved-0378> (Other) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u0378\u0308\U0001F1E6", {U"\u0378\u0308", U"\U0001F1E6"});
//÷ 0378 ÷ 0600 ÷
//÷ [0.2] <reserved-0378> (Other) ÷ [999.0] ARABIC NUMBER SIGN (Prepend) ÷ [0.3]
check_graphemes(U"\u0378\u0600", {U"\u0378", U"\u0600"});
//÷ 0378 × 0308 ÷ 0600 ÷
//÷ [0.2] <reserved-0378> (Other) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] ARABIC NUMBER SIGN (Prepend) ÷ [0.3]
check_graphemes(U"\u0378\u0308\u0600", {U"\u0378\u0308", U"\u0600"});
//÷ 0378 × 0903 ÷
//÷ [0.2] <reserved-0378> (Other) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u0378\u0903", {U"\u0378\u0903"});
//÷ 0378 × 0308 × 0903 ÷
//÷ [0.2] <reserved-0378> (Other) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u0378\u0308\u0903", {U"\u0378\u0308\u0903"});
//÷ 0378 ÷ 1100 ÷
//÷ [0.2] <reserved-0378> (Other) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u0378\u1100", {U"\u0378", U"\u1100"});
//÷ 0378 × 0308 ÷ 1100 ÷
//÷ [0.2] <reserved-0378> (Other) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u0378\u0308\u1100", {U"\u0378\u0308", U"\u1100"});
//÷ 0378 ÷ 1160 ÷
//÷ [0.2] <reserved-0378> (Other) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u0378\u1160", {U"\u0378", U"\u1160"});
//÷ 0378 × 0308 ÷ 1160 ÷
//÷ [0.2] <reserved-0378> (Other) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u0378\u0308\u1160", {U"\u0378\u0308", U"\u1160"});
//÷ 0378 ÷ 11A8 ÷
//÷ [0.2] <reserved-0378> (Other) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u0378\u11A8", {U"\u0378", U"\u11A8"});
//÷ 0378 × 0308 ÷ 11A8 ÷
//÷ [0.2] <reserved-0378> (Other) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u0378\u0308\u11A8", {U"\u0378\u0308", U"\u11A8"});
//÷ 0378 ÷ AC00 ÷
//÷ [0.2] <reserved-0378> (Other) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u0378\uAC00", {U"\u0378", U"\uAC00"});
//÷ 0378 × 0308 ÷ AC00 ÷
//÷ [0.2] <reserved-0378> (Other) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u0378\u0308\uAC00", {U"\u0378\u0308", U"\uAC00"});
//÷ 0378 ÷ AC01 ÷
//÷ [0.2] <reserved-0378> (Other) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u0378\uAC01", {U"\u0378", U"\uAC01"});
//÷ 0378 × 0308 ÷ AC01 ÷
//÷ [0.2] <reserved-0378> (Other) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u0378\u0308\uAC01", {U"\u0378\u0308", U"\uAC01"});
//÷ 0378 ÷ 231A ÷
//÷ [0.2] <reserved-0378> (Other) ÷ [999.0] WATCH (ExtPict) ÷ [0.3]
check_graphemes(U"\u0378\u231A", {U"\u0378", U"\u231A"});
//÷ 0378 × 0308 ÷ 231A ÷
//÷ [0.2] <reserved-0378> (Other) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] WATCH (ExtPict) ÷ [0.3]
check_graphemes(U"\u0378\u0308\u231A", {U"\u0378\u0308", U"\u231A"});
//÷ 0378 × 0300 ÷
//÷ [0.2] <reserved-0378> (Other) × [9.0] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u0378\u0300", {U"\u0378\u0300"});
//÷ 0378 × 0308 × 0300 ÷
//÷ [0.2] <reserved-0378> (Other) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] COMBINING GRAVE ACCENT (Extend_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u0378\u0308\u0300", {U"\u0378\u0308\u0300"});
//÷ 0378 × 200D ÷
//÷ [0.2] <reserved-0378> (Other) × [9.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u0378\u200D", {U"\u0378\u200D"});
//÷ 0378 × 0308 × 200D ÷
//÷ [0.2] <reserved-0378> (Other) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u0378\u0308\u200D", {U"\u0378\u0308\u200D"});
//÷ 0378 ÷ 0378 ÷
//÷ [0.2] <reserved-0378> (Other) ÷ [999.0] <reserved-0378> (Other) ÷ [0.3]
check_graphemes(U"\u0378\u0378", {U"\u0378", U"\u0378"});
//÷ 0378 × 0308 ÷ 0378 ÷
//÷ [0.2] <reserved-0378> (Other) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] <reserved-0378> (Other) ÷ [0.3]
check_graphemes(U"\u0378\u0308\u0378", {U"\u0378\u0308", U"\u0378"});
//÷ 000D × 000A ÷ 0061 ÷ 000A ÷ 0308 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) × [3.0] <LINE FEED (LF)> (LF) ÷ [4.0] LATIN SMALL LETTER A (Other) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [4.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u000D\u000A\u0061\u000A\u0308", {U"\u000D\u000A", U"\u0061", U"\u000A", U"\u0308"});
//÷ 0061 × 0308 ÷
//÷ [0.2] LATIN SMALL LETTER A (Other) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u0061\u0308", {U"\u0061\u0308"});
//÷ 0020 × 200D ÷ 0646 ÷
//÷ [0.2] SPACE (Other) × [9.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [999.0] ARABIC LETTER NOON (Other) ÷ [0.3]
check_graphemes(U"\u0020\u200D\u0646", {U"\u0020\u200D", U"\u0646"});
//÷ 0646 × 200D ÷ 0020 ÷
//÷ [0.2] ARABIC LETTER NOON (Other) × [9.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [999.0] SPACE (Other) ÷ [0.3]
check_graphemes(U"\u0646\u200D\u0020", {U"\u0646\u200D", U"\u0020"});
//÷ 1100 × 1100 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [6.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u1100\u1100", {U"\u1100\u1100"});
//÷ AC00 × 11A8 ÷ 1100 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [7.0] HANGUL JONGSEONG KIYEOK (T) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\uAC00\u11A8\u1100", {U"\uAC00\u11A8", U"\u1100"});
//÷ AC01 × 11A8 ÷ 1100 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [8.0] HANGUL JONGSEONG KIYEOK (T) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\uAC01\u11A8\u1100", {U"\uAC01\u11A8", U"\u1100"});
//÷ 1F1E6 × 1F1E7 ÷ 1F1E8 ÷ 0062 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [12.0] REGIONAL INDICATOR SYMBOL LETTER B (RI) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER C (RI) ÷ [999.0] LATIN SMALL LETTER B (Other) ÷ [0.3]
check_graphemes(U"\U0001F1E6\U0001F1E7\U0001F1E8\u0062", {U"\U0001F1E6\U0001F1E7", U"\U0001F1E8", U"\u0062"});
//÷ 0061 ÷ 1F1E6 × 1F1E7 ÷ 1F1E8 ÷ 0062 ÷
//÷ [0.2] LATIN SMALL LETTER A (Other) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [13.0] REGIONAL INDICATOR SYMBOL LETTER B (RI) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER C (RI) ÷ [999.0] LATIN SMALL LETTER B (Other) ÷ [0.3]
check_graphemes(U"\u0061\U0001F1E6\U0001F1E7\U0001F1E8\u0062", {U"\u0061", U"\U0001F1E6\U0001F1E7", U"\U0001F1E8", U"\u0062"});
//÷ 0061 ÷ 1F1E6 × 1F1E7 × 200D ÷ 1F1E8 ÷ 0062 ÷
//÷ [0.2] LATIN SMALL LETTER A (Other) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [13.0] REGIONAL INDICATOR SYMBOL LETTER B (RI) × [9.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER C (RI) ÷ [999.0] LATIN SMALL LETTER B (Other) ÷ [0.3]
check_graphemes(U"\u0061\U0001F1E6\U0001F1E7\u200D\U0001F1E8\u0062", {U"\u0061", U"\U0001F1E6\U0001F1E7\u200D", U"\U0001F1E8", U"\u0062"});
//÷ 0061 ÷ 1F1E6 × 200D ÷ 1F1E7 × 1F1E8 ÷ 0062 ÷
//÷ [0.2] LATIN SMALL LETTER A (Other) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER B (RI) × [13.0] REGIONAL INDICATOR SYMBOL LETTER C (RI) ÷ [999.0] LATIN SMALL LETTER B (Other) ÷ [0.3]
check_graphemes(U"\u0061\U0001F1E6\u200D\U0001F1E7\U0001F1E8\u0062", {U"\u0061", U"\U0001F1E6\u200D", U"\U0001F1E7\U0001F1E8", U"\u0062"});
//÷ 0061 ÷ 1F1E6 × 1F1E7 ÷ 1F1E8 × 1F1E9 ÷ 0062 ÷
//÷ [0.2] LATIN SMALL LETTER A (Other) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [13.0] REGIONAL INDICATOR SYMBOL LETTER B (RI) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER C (RI) × [13.0] REGIONAL INDICATOR SYMBOL LETTER D (RI) ÷ [999.0] LATIN SMALL LETTER B (Other) ÷ [0.3]
check_graphemes(U"\u0061\U0001F1E6\U0001F1E7\U0001F1E8\U0001F1E9\u0062", {U"\u0061", U"\U0001F1E6\U0001F1E7", U"\U0001F1E8\U0001F1E9", U"\u0062"});
//÷ 0061 × 200D ÷
//÷ [0.2] LATIN SMALL LETTER A (Other) × [9.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [0.3]
check_graphemes(U"\u0061\u200D", {U"\u0061\u200D"});
//÷ 0061 × 0308 ÷ 0062 ÷
//÷ [0.2] LATIN SMALL LETTER A (Other) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) ÷ [999.0] LATIN SMALL LETTER B (Other) ÷ [0.3]
check_graphemes(U"\u0061\u0308\u0062", {U"\u0061\u0308", U"\u0062"});
//÷ 0061 × 0903 ÷ 0062 ÷
//÷ [0.2] LATIN SMALL LETTER A (Other) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [999.0] LATIN SMALL LETTER B (Other) ÷ [0.3]
check_graphemes(U"\u0061\u0903\u0062", {U"\u0061\u0903", U"\u0062"});
//÷ 0061 ÷ 0600 × 0062 ÷
//÷ [0.2] LATIN SMALL LETTER A (Other) ÷ [999.0] ARABIC NUMBER SIGN (Prepend) × [9.2] LATIN SMALL LETTER B (Other) ÷ [0.3]
check_graphemes(U"\u0061\u0600\u0062", {U"\u0061", U"\u0600\u0062"});
//÷ 1F476 × 1F3FF ÷ 1F476 ÷
//÷ [0.2] BABY (ExtPict) × [9.0] EMOJI MODIFIER FITZPATRICK TYPE-6 (Extend) ÷ [999.0] BABY (ExtPict) ÷ [0.3]
check_graphemes(U"\U0001F476\U0001F3FF\U0001F476", {U"\U0001F476\U0001F3FF", U"\U0001F476"});
//÷ 0061 × 1F3FF ÷ 1F476 ÷
//÷ [0.2] LATIN SMALL LETTER A (Other) × [9.0] EMOJI MODIFIER FITZPATRICK TYPE-6 (Extend) ÷ [999.0] BABY (ExtPict) ÷ [0.3]
check_graphemes(U"\u0061\U0001F3FF\U0001F476", {U"\u0061\U0001F3FF", U"\U0001F476"});
//÷ 0061 × 1F3FF ÷ 1F476 × 200D × 1F6D1 ÷
//÷ [0.2] LATIN SMALL LETTER A (Other) × [9.0] EMOJI MODIFIER FITZPATRICK TYPE-6 (Extend) ÷ [999.0] BABY (ExtPict) × [9.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) × [11.0] OCTAGONAL SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\u0061\U0001F3FF\U0001F476\u200D\U0001F6D1", {U"\u0061\U0001F3FF", U"\U0001F476\u200D\U0001F6D1"});
//÷ 1F476 × 1F3FF × 0308 × 200D × 1F476 × 1F3FF ÷
//÷ [0.2] BABY (ExtPict) × [9.0] EMOJI MODIFIER FITZPATRICK TYPE-6 (Extend) × [9.0] COMBINING DIAERESIS (Extend_ExtCccZwj) × [9.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) × [11.0] BABY (ExtPict) × [9.0] EMOJI MODIFIER FITZPATRICK TYPE-6 (Extend) ÷ [0.3]
check_graphemes(U"\U0001F476\U0001F3FF\u0308\u200D\U0001F476\U0001F3FF", {U"\U0001F476\U0001F3FF\u0308\u200D\U0001F476\U0001F3FF"});
//÷ 1F6D1 × 200D × 1F6D1 ÷
//÷ [0.2] OCTAGONAL SIGN (ExtPict) × [9.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) × [11.0] OCTAGONAL SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\U0001F6D1\u200D\U0001F6D1", {U"\U0001F6D1\u200D\U0001F6D1"});
//÷ 0061 × 200D ÷ 1F6D1 ÷
//÷ [0.2] LATIN SMALL LETTER A (Other) × [9.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [999.0] OCTAGONAL SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\u0061\u200D\U0001F6D1", {U"\u0061\u200D", U"\U0001F6D1"});
//÷ 2701 × 200D × 2701 ÷
//÷ [0.2] UPPER BLADE SCISSORS (Other) × [9.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) × [11.0] UPPER BLADE SCISSORS (Other) ÷ [0.3]
check_graphemes(U"\u2701\u200D\u2701", {U"\u2701\u200D\u2701"});
//÷ 0061 × 200D ÷ 2701 ÷
//÷ [0.2] LATIN SMALL LETTER A (Other) × [9.0] ZERO WIDTH JOINER (ZWJ_ExtCccZwj) ÷ [999.0] UPPER BLADE SCISSORS (Other) ÷ [0.3]
check_graphemes(U"\u0061\u200D\u2701", {U"\u0061\u200D", U"\u2701"});


