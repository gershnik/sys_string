//THIS FILE IS GENERATED. PLEASE DO NOT EDIT DIRECTLY

//
// Copyright 2024 Eugene Gershnik
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://github.com/gershnik/sys_string/blob/master/LICENSE
//

//÷ 000D ÷ 000D ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u000D\u000D", {U"\u000D", U"\u000D"});
//÷ 000D ÷ 0308 ÷ 000D ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u000D\u0308\u000D", {U"\u000D", U"\u0308", U"\u000D"});
//÷ 000D × 000A ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) × [3.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u000D\u000A", {U"\u000D\u000A"});
//÷ 000D ÷ 0308 ÷ 000A ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u000D\u0308\u000A", {U"\u000D", U"\u0308", U"\u000A"});
//÷ 000D ÷ 0001 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u000D\u0001", {U"\u000D", U"\u0001"});
//÷ 000D ÷ 0308 ÷ 0001 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u000D\u0308\u0001", {U"\u000D", U"\u0308", U"\u0001"});
//÷ 000D ÷ 094D ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [0.3]
check_graphemes(U"\u000D\u094D", {U"\u000D", U"\u094D"});
//÷ 000D ÷ 0308 × 094D ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [0.3]
check_graphemes(U"\u000D\u0308\u094D", {U"\u000D", U"\u0308\u094D"});
//÷ 000D ÷ 0300 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\u000D\u0300", {U"\u000D", U"\u0300"});
//÷ 000D ÷ 0308 × 0300 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\u000D\u0308\u0300", {U"\u000D", U"\u0308\u0300"});
//÷ 000D ÷ 200C ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [0.3]
check_graphemes(U"\u000D\u200C", {U"\u000D", U"\u200C"});
//÷ 000D ÷ 0308 × 200C ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [0.3]
check_graphemes(U"\u000D\u0308\u200C", {U"\u000D", U"\u0308\u200C"});
//÷ 000D ÷ 200D ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] ZERO WIDTH JOINER (ZWJ) ÷ [0.3]
check_graphemes(U"\u000D\u200D", {U"\u000D", U"\u200D"});
//÷ 000D ÷ 0308 × 200D ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [0.3]
check_graphemes(U"\u000D\u0308\u200D", {U"\u000D", U"\u0308\u200D"});
//÷ 000D ÷ 1F1E6 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u000D\U0001F1E6", {U"\u000D", U"\U0001F1E6"});
//÷ 000D ÷ 0308 ÷ 1F1E6 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u000D\u0308\U0001F1E6", {U"\u000D", U"\u0308", U"\U0001F1E6"});
//÷ 000D ÷ 06DD ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] ARABIC END OF AYAH (Prepend) ÷ [0.3]
check_graphemes(U"\u000D\u06DD", {U"\u000D", U"\u06DD"});
//÷ 000D ÷ 0308 ÷ 06DD ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] ARABIC END OF AYAH (Prepend) ÷ [0.3]
check_graphemes(U"\u000D\u0308\u06DD", {U"\u000D", U"\u0308", U"\u06DD"});
//÷ 000D ÷ 0903 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u000D\u0903", {U"\u000D", U"\u0903"});
//÷ 000D ÷ 0308 × 0903 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u000D\u0308\u0903", {U"\u000D", U"\u0308\u0903"});
//÷ 000D ÷ 1100 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u000D\u1100", {U"\u000D", U"\u1100"});
//÷ 000D ÷ 0308 ÷ 1100 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u000D\u0308\u1100", {U"\u000D", U"\u0308", U"\u1100"});
//÷ 000D ÷ 1160 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u000D\u1160", {U"\u000D", U"\u1160"});
//÷ 000D ÷ 0308 ÷ 1160 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u000D\u0308\u1160", {U"\u000D", U"\u0308", U"\u1160"});
//÷ 000D ÷ 11A8 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u000D\u11A8", {U"\u000D", U"\u11A8"});
//÷ 000D ÷ 0308 ÷ 11A8 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u000D\u0308\u11A8", {U"\u000D", U"\u0308", U"\u11A8"});
//÷ 000D ÷ AC00 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u000D\uAC00", {U"\u000D", U"\uAC00"});
//÷ 000D ÷ 0308 ÷ AC00 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u000D\u0308\uAC00", {U"\u000D", U"\u0308", U"\uAC00"});
//÷ 000D ÷ AC01 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u000D\uAC01", {U"\u000D", U"\uAC01"});
//÷ 000D ÷ 0308 ÷ AC01 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u000D\u0308\uAC01", {U"\u000D", U"\u0308", U"\uAC01"});
//÷ 000D ÷ 0915 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u000D\u0915", {U"\u000D", U"\u0915"});
//÷ 000D ÷ 0308 ÷ 0915 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u000D\u0308\u0915", {U"\u000D", U"\u0308", U"\u0915"});
//÷ 000D ÷ 00A9 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] COPYRIGHT SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\u000D\u00A9", {U"\u000D", U"\u00A9"});
//÷ 000D ÷ 0308 ÷ 00A9 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] COPYRIGHT SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\u000D\u0308\u00A9", {U"\u000D", U"\u0308", U"\u00A9"});
//÷ 000D ÷ 0020 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] SPACE (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u000D\u0020", {U"\u000D", U"\u0020"});
//÷ 000D ÷ 0308 ÷ 0020 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] SPACE (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u000D\u0308\u0020", {U"\u000D", U"\u0308", U"\u0020"});
//÷ 000D ÷ 0378 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u000D\u0378", {U"\u000D", U"\u0378"});
//÷ 000D ÷ 0308 ÷ 0378 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u000D\u0308\u0378", {U"\u000D", U"\u0308", U"\u0378"});
//÷ 000A ÷ 000D ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u000A\u000D", {U"\u000A", U"\u000D"});
//÷ 000A ÷ 0308 ÷ 000D ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u000A\u0308\u000D", {U"\u000A", U"\u0308", U"\u000D"});
//÷ 000A ÷ 000A ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u000A\u000A", {U"\u000A", U"\u000A"});
//÷ 000A ÷ 0308 ÷ 000A ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u000A\u0308\u000A", {U"\u000A", U"\u0308", U"\u000A"});
//÷ 000A ÷ 0001 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u000A\u0001", {U"\u000A", U"\u0001"});
//÷ 000A ÷ 0308 ÷ 0001 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u000A\u0308\u0001", {U"\u000A", U"\u0308", U"\u0001"});
//÷ 000A ÷ 094D ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [0.3]
check_graphemes(U"\u000A\u094D", {U"\u000A", U"\u094D"});
//÷ 000A ÷ 0308 × 094D ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [0.3]
check_graphemes(U"\u000A\u0308\u094D", {U"\u000A", U"\u0308\u094D"});
//÷ 000A ÷ 0300 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\u000A\u0300", {U"\u000A", U"\u0300"});
//÷ 000A ÷ 0308 × 0300 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\u000A\u0308\u0300", {U"\u000A", U"\u0308\u0300"});
//÷ 000A ÷ 200C ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [0.3]
check_graphemes(U"\u000A\u200C", {U"\u000A", U"\u200C"});
//÷ 000A ÷ 0308 × 200C ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [0.3]
check_graphemes(U"\u000A\u0308\u200C", {U"\u000A", U"\u0308\u200C"});
//÷ 000A ÷ 200D ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] ZERO WIDTH JOINER (ZWJ) ÷ [0.3]
check_graphemes(U"\u000A\u200D", {U"\u000A", U"\u200D"});
//÷ 000A ÷ 0308 × 200D ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [0.3]
check_graphemes(U"\u000A\u0308\u200D", {U"\u000A", U"\u0308\u200D"});
//÷ 000A ÷ 1F1E6 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u000A\U0001F1E6", {U"\u000A", U"\U0001F1E6"});
//÷ 000A ÷ 0308 ÷ 1F1E6 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u000A\u0308\U0001F1E6", {U"\u000A", U"\u0308", U"\U0001F1E6"});
//÷ 000A ÷ 06DD ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] ARABIC END OF AYAH (Prepend) ÷ [0.3]
check_graphemes(U"\u000A\u06DD", {U"\u000A", U"\u06DD"});
//÷ 000A ÷ 0308 ÷ 06DD ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] ARABIC END OF AYAH (Prepend) ÷ [0.3]
check_graphemes(U"\u000A\u0308\u06DD", {U"\u000A", U"\u0308", U"\u06DD"});
//÷ 000A ÷ 0903 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u000A\u0903", {U"\u000A", U"\u0903"});
//÷ 000A ÷ 0308 × 0903 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u000A\u0308\u0903", {U"\u000A", U"\u0308\u0903"});
//÷ 000A ÷ 1100 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u000A\u1100", {U"\u000A", U"\u1100"});
//÷ 000A ÷ 0308 ÷ 1100 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u000A\u0308\u1100", {U"\u000A", U"\u0308", U"\u1100"});
//÷ 000A ÷ 1160 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u000A\u1160", {U"\u000A", U"\u1160"});
//÷ 000A ÷ 0308 ÷ 1160 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u000A\u0308\u1160", {U"\u000A", U"\u0308", U"\u1160"});
//÷ 000A ÷ 11A8 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u000A\u11A8", {U"\u000A", U"\u11A8"});
//÷ 000A ÷ 0308 ÷ 11A8 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u000A\u0308\u11A8", {U"\u000A", U"\u0308", U"\u11A8"});
//÷ 000A ÷ AC00 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u000A\uAC00", {U"\u000A", U"\uAC00"});
//÷ 000A ÷ 0308 ÷ AC00 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u000A\u0308\uAC00", {U"\u000A", U"\u0308", U"\uAC00"});
//÷ 000A ÷ AC01 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u000A\uAC01", {U"\u000A", U"\uAC01"});
//÷ 000A ÷ 0308 ÷ AC01 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u000A\u0308\uAC01", {U"\u000A", U"\u0308", U"\uAC01"});
//÷ 000A ÷ 0915 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u000A\u0915", {U"\u000A", U"\u0915"});
//÷ 000A ÷ 0308 ÷ 0915 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u000A\u0308\u0915", {U"\u000A", U"\u0308", U"\u0915"});
//÷ 000A ÷ 00A9 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] COPYRIGHT SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\u000A\u00A9", {U"\u000A", U"\u00A9"});
//÷ 000A ÷ 0308 ÷ 00A9 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] COPYRIGHT SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\u000A\u0308\u00A9", {U"\u000A", U"\u0308", U"\u00A9"});
//÷ 000A ÷ 0020 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] SPACE (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u000A\u0020", {U"\u000A", U"\u0020"});
//÷ 000A ÷ 0308 ÷ 0020 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] SPACE (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u000A\u0308\u0020", {U"\u000A", U"\u0308", U"\u0020"});
//÷ 000A ÷ 0378 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u000A\u0378", {U"\u000A", U"\u0378"});
//÷ 000A ÷ 0308 ÷ 0378 ÷
//÷ [0.2] <LINE FEED (LF)> (LF) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u000A\u0308\u0378", {U"\u000A", U"\u0308", U"\u0378"});
//÷ 0001 ÷ 000D ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u0001\u000D", {U"\u0001", U"\u000D"});
//÷ 0001 ÷ 0308 ÷ 000D ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u0001\u0308\u000D", {U"\u0001", U"\u0308", U"\u000D"});
//÷ 0001 ÷ 000A ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u0001\u000A", {U"\u0001", U"\u000A"});
//÷ 0001 ÷ 0308 ÷ 000A ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u0001\u0308\u000A", {U"\u0001", U"\u0308", U"\u000A"});
//÷ 0001 ÷ 0001 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u0001\u0001", {U"\u0001", U"\u0001"});
//÷ 0001 ÷ 0308 ÷ 0001 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u0001\u0308\u0001", {U"\u0001", U"\u0308", U"\u0001"});
//÷ 0001 ÷ 094D ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [0.3]
check_graphemes(U"\u0001\u094D", {U"\u0001", U"\u094D"});
//÷ 0001 ÷ 0308 × 094D ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [0.3]
check_graphemes(U"\u0001\u0308\u094D", {U"\u0001", U"\u0308\u094D"});
//÷ 0001 ÷ 0300 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\u0001\u0300", {U"\u0001", U"\u0300"});
//÷ 0001 ÷ 0308 × 0300 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\u0001\u0308\u0300", {U"\u0001", U"\u0308\u0300"});
//÷ 0001 ÷ 200C ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [0.3]
check_graphemes(U"\u0001\u200C", {U"\u0001", U"\u200C"});
//÷ 0001 ÷ 0308 × 200C ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [0.3]
check_graphemes(U"\u0001\u0308\u200C", {U"\u0001", U"\u0308\u200C"});
//÷ 0001 ÷ 200D ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] ZERO WIDTH JOINER (ZWJ) ÷ [0.3]
check_graphemes(U"\u0001\u200D", {U"\u0001", U"\u200D"});
//÷ 0001 ÷ 0308 × 200D ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [0.3]
check_graphemes(U"\u0001\u0308\u200D", {U"\u0001", U"\u0308\u200D"});
//÷ 0001 ÷ 1F1E6 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u0001\U0001F1E6", {U"\u0001", U"\U0001F1E6"});
//÷ 0001 ÷ 0308 ÷ 1F1E6 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u0001\u0308\U0001F1E6", {U"\u0001", U"\u0308", U"\U0001F1E6"});
//÷ 0001 ÷ 06DD ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] ARABIC END OF AYAH (Prepend) ÷ [0.3]
check_graphemes(U"\u0001\u06DD", {U"\u0001", U"\u06DD"});
//÷ 0001 ÷ 0308 ÷ 06DD ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] ARABIC END OF AYAH (Prepend) ÷ [0.3]
check_graphemes(U"\u0001\u0308\u06DD", {U"\u0001", U"\u0308", U"\u06DD"});
//÷ 0001 ÷ 0903 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u0001\u0903", {U"\u0001", U"\u0903"});
//÷ 0001 ÷ 0308 × 0903 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u0001\u0308\u0903", {U"\u0001", U"\u0308\u0903"});
//÷ 0001 ÷ 1100 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u0001\u1100", {U"\u0001", U"\u1100"});
//÷ 0001 ÷ 0308 ÷ 1100 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u0001\u0308\u1100", {U"\u0001", U"\u0308", U"\u1100"});
//÷ 0001 ÷ 1160 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u0001\u1160", {U"\u0001", U"\u1160"});
//÷ 0001 ÷ 0308 ÷ 1160 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u0001\u0308\u1160", {U"\u0001", U"\u0308", U"\u1160"});
//÷ 0001 ÷ 11A8 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u0001\u11A8", {U"\u0001", U"\u11A8"});
//÷ 0001 ÷ 0308 ÷ 11A8 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u0001\u0308\u11A8", {U"\u0001", U"\u0308", U"\u11A8"});
//÷ 0001 ÷ AC00 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u0001\uAC00", {U"\u0001", U"\uAC00"});
//÷ 0001 ÷ 0308 ÷ AC00 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u0001\u0308\uAC00", {U"\u0001", U"\u0308", U"\uAC00"});
//÷ 0001 ÷ AC01 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u0001\uAC01", {U"\u0001", U"\uAC01"});
//÷ 0001 ÷ 0308 ÷ AC01 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u0001\u0308\uAC01", {U"\u0001", U"\u0308", U"\uAC01"});
//÷ 0001 ÷ 0915 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u0001\u0915", {U"\u0001", U"\u0915"});
//÷ 0001 ÷ 0308 ÷ 0915 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u0001\u0308\u0915", {U"\u0001", U"\u0308", U"\u0915"});
//÷ 0001 ÷ 00A9 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] COPYRIGHT SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\u0001\u00A9", {U"\u0001", U"\u00A9"});
//÷ 0001 ÷ 0308 ÷ 00A9 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] COPYRIGHT SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\u0001\u0308\u00A9", {U"\u0001", U"\u0308", U"\u00A9"});
//÷ 0001 ÷ 0020 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] SPACE (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u0001\u0020", {U"\u0001", U"\u0020"});
//÷ 0001 ÷ 0308 ÷ 0020 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] SPACE (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u0001\u0308\u0020", {U"\u0001", U"\u0308", U"\u0020"});
//÷ 0001 ÷ 0378 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u0001\u0378", {U"\u0001", U"\u0378"});
//÷ 0001 ÷ 0308 ÷ 0378 ÷
//÷ [0.2] <START OF HEADING> (Control) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u0001\u0308\u0378", {U"\u0001", U"\u0308", U"\u0378"});
//÷ 094D ÷ 000D ÷
//÷ [0.2] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u094D\u000D", {U"\u094D", U"\u000D"});
//÷ 094D × 0308 ÷ 000D ÷
//÷ [0.2] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u094D\u0308\u000D", {U"\u094D\u0308", U"\u000D"});
//÷ 094D ÷ 000A ÷
//÷ [0.2] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u094D\u000A", {U"\u094D", U"\u000A"});
//÷ 094D × 0308 ÷ 000A ÷
//÷ [0.2] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u094D\u0308\u000A", {U"\u094D\u0308", U"\u000A"});
//÷ 094D ÷ 0001 ÷
//÷ [0.2] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u094D\u0001", {U"\u094D", U"\u0001"});
//÷ 094D × 0308 ÷ 0001 ÷
//÷ [0.2] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u094D\u0308\u0001", {U"\u094D\u0308", U"\u0001"});
//÷ 094D × 094D ÷
//÷ [0.2] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [0.3]
check_graphemes(U"\u094D\u094D", {U"\u094D\u094D"});
//÷ 094D × 0308 × 094D ÷
//÷ [0.2] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [0.3]
check_graphemes(U"\u094D\u0308\u094D", {U"\u094D\u0308\u094D"});
//÷ 094D × 0300 ÷
//÷ [0.2] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) × [9.0] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\u094D\u0300", {U"\u094D\u0300"});
//÷ 094D × 0308 × 0300 ÷
//÷ [0.2] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\u094D\u0308\u0300", {U"\u094D\u0308\u0300"});
//÷ 094D × 200C ÷
//÷ [0.2] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) × [9.0] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [0.3]
check_graphemes(U"\u094D\u200C", {U"\u094D\u200C"});
//÷ 094D × 0308 × 200C ÷
//÷ [0.2] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [0.3]
check_graphemes(U"\u094D\u0308\u200C", {U"\u094D\u0308\u200C"});
//÷ 094D × 200D ÷
//÷ [0.2] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [0.3]
check_graphemes(U"\u094D\u200D", {U"\u094D\u200D"});
//÷ 094D × 0308 × 200D ÷
//÷ [0.2] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [0.3]
check_graphemes(U"\u094D\u0308\u200D", {U"\u094D\u0308\u200D"});
//÷ 094D ÷ 1F1E6 ÷
//÷ [0.2] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u094D\U0001F1E6", {U"\u094D", U"\U0001F1E6"});
//÷ 094D × 0308 ÷ 1F1E6 ÷
//÷ [0.2] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u094D\u0308\U0001F1E6", {U"\u094D\u0308", U"\U0001F1E6"});
//÷ 094D ÷ 06DD ÷
//÷ [0.2] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [999.0] ARABIC END OF AYAH (Prepend) ÷ [0.3]
check_graphemes(U"\u094D\u06DD", {U"\u094D", U"\u06DD"});
//÷ 094D × 0308 ÷ 06DD ÷
//÷ [0.2] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] ARABIC END OF AYAH (Prepend) ÷ [0.3]
check_graphemes(U"\u094D\u0308\u06DD", {U"\u094D\u0308", U"\u06DD"});
//÷ 094D × 0903 ÷
//÷ [0.2] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u094D\u0903", {U"\u094D\u0903"});
//÷ 094D × 0308 × 0903 ÷
//÷ [0.2] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u094D\u0308\u0903", {U"\u094D\u0308\u0903"});
//÷ 094D ÷ 1100 ÷
//÷ [0.2] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u094D\u1100", {U"\u094D", U"\u1100"});
//÷ 094D × 0308 ÷ 1100 ÷
//÷ [0.2] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u094D\u0308\u1100", {U"\u094D\u0308", U"\u1100"});
//÷ 094D ÷ 1160 ÷
//÷ [0.2] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u094D\u1160", {U"\u094D", U"\u1160"});
//÷ 094D × 0308 ÷ 1160 ÷
//÷ [0.2] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u094D\u0308\u1160", {U"\u094D\u0308", U"\u1160"});
//÷ 094D ÷ 11A8 ÷
//÷ [0.2] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u094D\u11A8", {U"\u094D", U"\u11A8"});
//÷ 094D × 0308 ÷ 11A8 ÷
//÷ [0.2] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u094D\u0308\u11A8", {U"\u094D\u0308", U"\u11A8"});
//÷ 094D ÷ AC00 ÷
//÷ [0.2] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u094D\uAC00", {U"\u094D", U"\uAC00"});
//÷ 094D × 0308 ÷ AC00 ÷
//÷ [0.2] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u094D\u0308\uAC00", {U"\u094D\u0308", U"\uAC00"});
//÷ 094D ÷ AC01 ÷
//÷ [0.2] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u094D\uAC01", {U"\u094D", U"\uAC01"});
//÷ 094D × 0308 ÷ AC01 ÷
//÷ [0.2] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u094D\u0308\uAC01", {U"\u094D\u0308", U"\uAC01"});
//÷ 094D ÷ 0915 ÷
//÷ [0.2] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [999.0] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u094D\u0915", {U"\u094D", U"\u0915"});
//÷ 094D × 0308 ÷ 0915 ÷
//÷ [0.2] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u094D\u0308\u0915", {U"\u094D\u0308", U"\u0915"});
//÷ 094D ÷ 00A9 ÷
//÷ [0.2] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [999.0] COPYRIGHT SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\u094D\u00A9", {U"\u094D", U"\u00A9"});
//÷ 094D × 0308 ÷ 00A9 ÷
//÷ [0.2] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] COPYRIGHT SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\u094D\u0308\u00A9", {U"\u094D\u0308", U"\u00A9"});
//÷ 094D ÷ 0020 ÷
//÷ [0.2] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [999.0] SPACE (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u094D\u0020", {U"\u094D", U"\u0020"});
//÷ 094D × 0308 ÷ 0020 ÷
//÷ [0.2] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] SPACE (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u094D\u0308\u0020", {U"\u094D\u0308", U"\u0020"});
//÷ 094D ÷ 0378 ÷
//÷ [0.2] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [999.0] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u094D\u0378", {U"\u094D", U"\u0378"});
//÷ 094D × 0308 ÷ 0378 ÷
//÷ [0.2] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u094D\u0308\u0378", {U"\u094D\u0308", U"\u0378"});
//÷ 0300 ÷ 000D ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u0300\u000D", {U"\u0300", U"\u000D"});
//÷ 0300 × 0308 ÷ 000D ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u0300\u0308\u000D", {U"\u0300\u0308", U"\u000D"});
//÷ 0300 ÷ 000A ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u0300\u000A", {U"\u0300", U"\u000A"});
//÷ 0300 × 0308 ÷ 000A ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u0300\u0308\u000A", {U"\u0300\u0308", U"\u000A"});
//÷ 0300 ÷ 0001 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u0300\u0001", {U"\u0300", U"\u0001"});
//÷ 0300 × 0308 ÷ 0001 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u0300\u0308\u0001", {U"\u0300\u0308", U"\u0001"});
//÷ 0300 × 094D ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [0.3]
check_graphemes(U"\u0300\u094D", {U"\u0300\u094D"});
//÷ 0300 × 0308 × 094D ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [0.3]
check_graphemes(U"\u0300\u0308\u094D", {U"\u0300\u0308\u094D"});
//÷ 0300 × 0300 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) × [9.0] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\u0300\u0300", {U"\u0300\u0300"});
//÷ 0300 × 0308 × 0300 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\u0300\u0308\u0300", {U"\u0300\u0308\u0300"});
//÷ 0300 × 200C ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) × [9.0] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [0.3]
check_graphemes(U"\u0300\u200C", {U"\u0300\u200C"});
//÷ 0300 × 0308 × 200C ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [0.3]
check_graphemes(U"\u0300\u0308\u200C", {U"\u0300\u0308\u200C"});
//÷ 0300 × 200D ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [0.3]
check_graphemes(U"\u0300\u200D", {U"\u0300\u200D"});
//÷ 0300 × 0308 × 200D ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [0.3]
check_graphemes(U"\u0300\u0308\u200D", {U"\u0300\u0308\u200D"});
//÷ 0300 ÷ 1F1E6 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u0300\U0001F1E6", {U"\u0300", U"\U0001F1E6"});
//÷ 0300 × 0308 ÷ 1F1E6 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u0300\u0308\U0001F1E6", {U"\u0300\u0308", U"\U0001F1E6"});
//÷ 0300 ÷ 06DD ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] ARABIC END OF AYAH (Prepend) ÷ [0.3]
check_graphemes(U"\u0300\u06DD", {U"\u0300", U"\u06DD"});
//÷ 0300 × 0308 ÷ 06DD ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] ARABIC END OF AYAH (Prepend) ÷ [0.3]
check_graphemes(U"\u0300\u0308\u06DD", {U"\u0300\u0308", U"\u06DD"});
//÷ 0300 × 0903 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u0300\u0903", {U"\u0300\u0903"});
//÷ 0300 × 0308 × 0903 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u0300\u0308\u0903", {U"\u0300\u0308\u0903"});
//÷ 0300 ÷ 1100 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u0300\u1100", {U"\u0300", U"\u1100"});
//÷ 0300 × 0308 ÷ 1100 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u0300\u0308\u1100", {U"\u0300\u0308", U"\u1100"});
//÷ 0300 ÷ 1160 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u0300\u1160", {U"\u0300", U"\u1160"});
//÷ 0300 × 0308 ÷ 1160 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u0300\u0308\u1160", {U"\u0300\u0308", U"\u1160"});
//÷ 0300 ÷ 11A8 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u0300\u11A8", {U"\u0300", U"\u11A8"});
//÷ 0300 × 0308 ÷ 11A8 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u0300\u0308\u11A8", {U"\u0300\u0308", U"\u11A8"});
//÷ 0300 ÷ AC00 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u0300\uAC00", {U"\u0300", U"\uAC00"});
//÷ 0300 × 0308 ÷ AC00 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u0300\u0308\uAC00", {U"\u0300\u0308", U"\uAC00"});
//÷ 0300 ÷ AC01 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u0300\uAC01", {U"\u0300", U"\uAC01"});
//÷ 0300 × 0308 ÷ AC01 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u0300\u0308\uAC01", {U"\u0300\u0308", U"\uAC01"});
//÷ 0300 ÷ 0915 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u0300\u0915", {U"\u0300", U"\u0915"});
//÷ 0300 × 0308 ÷ 0915 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u0300\u0308\u0915", {U"\u0300\u0308", U"\u0915"});
//÷ 0300 ÷ 00A9 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] COPYRIGHT SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\u0300\u00A9", {U"\u0300", U"\u00A9"});
//÷ 0300 × 0308 ÷ 00A9 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] COPYRIGHT SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\u0300\u0308\u00A9", {U"\u0300\u0308", U"\u00A9"});
//÷ 0300 ÷ 0020 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] SPACE (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u0300\u0020", {U"\u0300", U"\u0020"});
//÷ 0300 × 0308 ÷ 0020 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] SPACE (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u0300\u0308\u0020", {U"\u0300\u0308", U"\u0020"});
//÷ 0300 ÷ 0378 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u0300\u0378", {U"\u0300", U"\u0378"});
//÷ 0300 × 0308 ÷ 0378 ÷
//÷ [0.2] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u0300\u0308\u0378", {U"\u0300\u0308", U"\u0378"});
//÷ 200C ÷ 000D ÷
//÷ [0.2] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u200C\u000D", {U"\u200C", U"\u000D"});
//÷ 200C × 0308 ÷ 000D ÷
//÷ [0.2] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u200C\u0308\u000D", {U"\u200C\u0308", U"\u000D"});
//÷ 200C ÷ 000A ÷
//÷ [0.2] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u200C\u000A", {U"\u200C", U"\u000A"});
//÷ 200C × 0308 ÷ 000A ÷
//÷ [0.2] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u200C\u0308\u000A", {U"\u200C\u0308", U"\u000A"});
//÷ 200C ÷ 0001 ÷
//÷ [0.2] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u200C\u0001", {U"\u200C", U"\u0001"});
//÷ 200C × 0308 ÷ 0001 ÷
//÷ [0.2] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u200C\u0308\u0001", {U"\u200C\u0308", U"\u0001"});
//÷ 200C × 094D ÷
//÷ [0.2] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [0.3]
check_graphemes(U"\u200C\u094D", {U"\u200C\u094D"});
//÷ 200C × 0308 × 094D ÷
//÷ [0.2] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [0.3]
check_graphemes(U"\u200C\u0308\u094D", {U"\u200C\u0308\u094D"});
//÷ 200C × 0300 ÷
//÷ [0.2] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) × [9.0] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\u200C\u0300", {U"\u200C\u0300"});
//÷ 200C × 0308 × 0300 ÷
//÷ [0.2] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\u200C\u0308\u0300", {U"\u200C\u0308\u0300"});
//÷ 200C × 200C ÷
//÷ [0.2] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) × [9.0] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [0.3]
check_graphemes(U"\u200C\u200C", {U"\u200C\u200C"});
//÷ 200C × 0308 × 200C ÷
//÷ [0.2] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [0.3]
check_graphemes(U"\u200C\u0308\u200C", {U"\u200C\u0308\u200C"});
//÷ 200C × 200D ÷
//÷ [0.2] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [0.3]
check_graphemes(U"\u200C\u200D", {U"\u200C\u200D"});
//÷ 200C × 0308 × 200D ÷
//÷ [0.2] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [0.3]
check_graphemes(U"\u200C\u0308\u200D", {U"\u200C\u0308\u200D"});
//÷ 200C ÷ 1F1E6 ÷
//÷ [0.2] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u200C\U0001F1E6", {U"\u200C", U"\U0001F1E6"});
//÷ 200C × 0308 ÷ 1F1E6 ÷
//÷ [0.2] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u200C\u0308\U0001F1E6", {U"\u200C\u0308", U"\U0001F1E6"});
//÷ 200C ÷ 06DD ÷
//÷ [0.2] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [999.0] ARABIC END OF AYAH (Prepend) ÷ [0.3]
check_graphemes(U"\u200C\u06DD", {U"\u200C", U"\u06DD"});
//÷ 200C × 0308 ÷ 06DD ÷
//÷ [0.2] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] ARABIC END OF AYAH (Prepend) ÷ [0.3]
check_graphemes(U"\u200C\u0308\u06DD", {U"\u200C\u0308", U"\u06DD"});
//÷ 200C × 0903 ÷
//÷ [0.2] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u200C\u0903", {U"\u200C\u0903"});
//÷ 200C × 0308 × 0903 ÷
//÷ [0.2] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u200C\u0308\u0903", {U"\u200C\u0308\u0903"});
//÷ 200C ÷ 1100 ÷
//÷ [0.2] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u200C\u1100", {U"\u200C", U"\u1100"});
//÷ 200C × 0308 ÷ 1100 ÷
//÷ [0.2] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u200C\u0308\u1100", {U"\u200C\u0308", U"\u1100"});
//÷ 200C ÷ 1160 ÷
//÷ [0.2] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u200C\u1160", {U"\u200C", U"\u1160"});
//÷ 200C × 0308 ÷ 1160 ÷
//÷ [0.2] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u200C\u0308\u1160", {U"\u200C\u0308", U"\u1160"});
//÷ 200C ÷ 11A8 ÷
//÷ [0.2] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u200C\u11A8", {U"\u200C", U"\u11A8"});
//÷ 200C × 0308 ÷ 11A8 ÷
//÷ [0.2] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u200C\u0308\u11A8", {U"\u200C\u0308", U"\u11A8"});
//÷ 200C ÷ AC00 ÷
//÷ [0.2] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u200C\uAC00", {U"\u200C", U"\uAC00"});
//÷ 200C × 0308 ÷ AC00 ÷
//÷ [0.2] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u200C\u0308\uAC00", {U"\u200C\u0308", U"\uAC00"});
//÷ 200C ÷ AC01 ÷
//÷ [0.2] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u200C\uAC01", {U"\u200C", U"\uAC01"});
//÷ 200C × 0308 ÷ AC01 ÷
//÷ [0.2] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u200C\u0308\uAC01", {U"\u200C\u0308", U"\uAC01"});
//÷ 200C ÷ 0915 ÷
//÷ [0.2] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [999.0] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u200C\u0915", {U"\u200C", U"\u0915"});
//÷ 200C × 0308 ÷ 0915 ÷
//÷ [0.2] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u200C\u0308\u0915", {U"\u200C\u0308", U"\u0915"});
//÷ 200C ÷ 00A9 ÷
//÷ [0.2] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [999.0] COPYRIGHT SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\u200C\u00A9", {U"\u200C", U"\u00A9"});
//÷ 200C × 0308 ÷ 00A9 ÷
//÷ [0.2] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] COPYRIGHT SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\u200C\u0308\u00A9", {U"\u200C\u0308", U"\u00A9"});
//÷ 200C ÷ 0020 ÷
//÷ [0.2] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [999.0] SPACE (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u200C\u0020", {U"\u200C", U"\u0020"});
//÷ 200C × 0308 ÷ 0020 ÷
//÷ [0.2] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] SPACE (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u200C\u0308\u0020", {U"\u200C\u0308", U"\u0020"});
//÷ 200C ÷ 0378 ÷
//÷ [0.2] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [999.0] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u200C\u0378", {U"\u200C", U"\u0378"});
//÷ 200C × 0308 ÷ 0378 ÷
//÷ [0.2] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u200C\u0308\u0378", {U"\u200C\u0308", U"\u0378"});
//÷ 200D ÷ 000D ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u200D\u000D", {U"\u200D", U"\u000D"});
//÷ 200D × 0308 ÷ 000D ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u200D\u0308\u000D", {U"\u200D\u0308", U"\u000D"});
//÷ 200D ÷ 000A ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u200D\u000A", {U"\u200D", U"\u000A"});
//÷ 200D × 0308 ÷ 000A ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u200D\u0308\u000A", {U"\u200D\u0308", U"\u000A"});
//÷ 200D ÷ 0001 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u200D\u0001", {U"\u200D", U"\u0001"});
//÷ 200D × 0308 ÷ 0001 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u200D\u0308\u0001", {U"\u200D\u0308", U"\u0001"});
//÷ 200D × 094D ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [0.3]
check_graphemes(U"\u200D\u094D", {U"\u200D\u094D"});
//÷ 200D × 0308 × 094D ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [0.3]
check_graphemes(U"\u200D\u0308\u094D", {U"\u200D\u0308\u094D"});
//÷ 200D × 0300 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ) × [9.0] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\u200D\u0300", {U"\u200D\u0300"});
//÷ 200D × 0308 × 0300 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\u200D\u0308\u0300", {U"\u200D\u0308\u0300"});
//÷ 200D × 200C ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ) × [9.0] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [0.3]
check_graphemes(U"\u200D\u200C", {U"\u200D\u200C"});
//÷ 200D × 0308 × 200C ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [0.3]
check_graphemes(U"\u200D\u0308\u200C", {U"\u200D\u0308\u200C"});
//÷ 200D × 200D ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [0.3]
check_graphemes(U"\u200D\u200D", {U"\u200D\u200D"});
//÷ 200D × 0308 × 200D ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [0.3]
check_graphemes(U"\u200D\u0308\u200D", {U"\u200D\u0308\u200D"});
//÷ 200D ÷ 1F1E6 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u200D\U0001F1E6", {U"\u200D", U"\U0001F1E6"});
//÷ 200D × 0308 ÷ 1F1E6 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u200D\u0308\U0001F1E6", {U"\u200D\u0308", U"\U0001F1E6"});
//÷ 200D ÷ 06DD ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ) ÷ [999.0] ARABIC END OF AYAH (Prepend) ÷ [0.3]
check_graphemes(U"\u200D\u06DD", {U"\u200D", U"\u06DD"});
//÷ 200D × 0308 ÷ 06DD ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] ARABIC END OF AYAH (Prepend) ÷ [0.3]
check_graphemes(U"\u200D\u0308\u06DD", {U"\u200D\u0308", U"\u06DD"});
//÷ 200D × 0903 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u200D\u0903", {U"\u200D\u0903"});
//÷ 200D × 0308 × 0903 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u200D\u0308\u0903", {U"\u200D\u0308\u0903"});
//÷ 200D ÷ 1100 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u200D\u1100", {U"\u200D", U"\u1100"});
//÷ 200D × 0308 ÷ 1100 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u200D\u0308\u1100", {U"\u200D\u0308", U"\u1100"});
//÷ 200D ÷ 1160 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u200D\u1160", {U"\u200D", U"\u1160"});
//÷ 200D × 0308 ÷ 1160 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u200D\u0308\u1160", {U"\u200D\u0308", U"\u1160"});
//÷ 200D ÷ 11A8 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u200D\u11A8", {U"\u200D", U"\u11A8"});
//÷ 200D × 0308 ÷ 11A8 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u200D\u0308\u11A8", {U"\u200D\u0308", U"\u11A8"});
//÷ 200D ÷ AC00 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u200D\uAC00", {U"\u200D", U"\uAC00"});
//÷ 200D × 0308 ÷ AC00 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u200D\u0308\uAC00", {U"\u200D\u0308", U"\uAC00"});
//÷ 200D ÷ AC01 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u200D\uAC01", {U"\u200D", U"\uAC01"});
//÷ 200D × 0308 ÷ AC01 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u200D\u0308\uAC01", {U"\u200D\u0308", U"\uAC01"});
//÷ 200D ÷ 0915 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ) ÷ [999.0] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u200D\u0915", {U"\u200D", U"\u0915"});
//÷ 200D × 0308 ÷ 0915 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u200D\u0308\u0915", {U"\u200D\u0308", U"\u0915"});
//÷ 200D ÷ 00A9 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ) ÷ [999.0] COPYRIGHT SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\u200D\u00A9", {U"\u200D", U"\u00A9"});
//÷ 200D × 0308 ÷ 00A9 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] COPYRIGHT SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\u200D\u0308\u00A9", {U"\u200D\u0308", U"\u00A9"});
//÷ 200D ÷ 0020 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ) ÷ [999.0] SPACE (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u200D\u0020", {U"\u200D", U"\u0020"});
//÷ 200D × 0308 ÷ 0020 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] SPACE (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u200D\u0308\u0020", {U"\u200D\u0308", U"\u0020"});
//÷ 200D ÷ 0378 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ) ÷ [999.0] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u200D\u0378", {U"\u200D", U"\u0378"});
//÷ 200D × 0308 ÷ 0378 ÷
//÷ [0.2] ZERO WIDTH JOINER (ZWJ) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u200D\u0308\u0378", {U"\u200D\u0308", U"\u0378"});
//÷ 1F1E6 ÷ 000D ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u000D", {U"\U0001F1E6", U"\u000D"});
//÷ 1F1E6 × 0308 ÷ 000D ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0308\u000D", {U"\U0001F1E6\u0308", U"\u000D"});
//÷ 1F1E6 ÷ 000A ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u000A", {U"\U0001F1E6", U"\u000A"});
//÷ 1F1E6 × 0308 ÷ 000A ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0308\u000A", {U"\U0001F1E6\u0308", U"\u000A"});
//÷ 1F1E6 ÷ 0001 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0001", {U"\U0001F1E6", U"\u0001"});
//÷ 1F1E6 × 0308 ÷ 0001 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0308\u0001", {U"\U0001F1E6\u0308", U"\u0001"});
//÷ 1F1E6 × 094D ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u094D", {U"\U0001F1E6\u094D"});
//÷ 1F1E6 × 0308 × 094D ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0308\u094D", {U"\U0001F1E6\u0308\u094D"});
//÷ 1F1E6 × 0300 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0300", {U"\U0001F1E6\u0300"});
//÷ 1F1E6 × 0308 × 0300 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0308\u0300", {U"\U0001F1E6\u0308\u0300"});
//÷ 1F1E6 × 200C ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u200C", {U"\U0001F1E6\u200C"});
//÷ 1F1E6 × 0308 × 200C ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0308\u200C", {U"\U0001F1E6\u0308\u200C"});
//÷ 1F1E6 × 200D ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u200D", {U"\U0001F1E6\u200D"});
//÷ 1F1E6 × 0308 × 200D ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0308\u200D", {U"\U0001F1E6\u0308\u200D"});
//÷ 1F1E6 × 1F1E6 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [12.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\U0001F1E6\U0001F1E6", {U"\U0001F1E6\U0001F1E6"});
//÷ 1F1E6 × 0308 ÷ 1F1E6 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0308\U0001F1E6", {U"\U0001F1E6\u0308", U"\U0001F1E6"});
//÷ 1F1E6 ÷ 06DD ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [999.0] ARABIC END OF AYAH (Prepend) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u06DD", {U"\U0001F1E6", U"\u06DD"});
//÷ 1F1E6 × 0308 ÷ 06DD ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] ARABIC END OF AYAH (Prepend) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0308\u06DD", {U"\U0001F1E6\u0308", U"\u06DD"});
//÷ 1F1E6 × 0903 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0903", {U"\U0001F1E6\u0903"});
//÷ 1F1E6 × 0308 × 0903 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0308\u0903", {U"\U0001F1E6\u0308\u0903"});
//÷ 1F1E6 ÷ 1100 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u1100", {U"\U0001F1E6", U"\u1100"});
//÷ 1F1E6 × 0308 ÷ 1100 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0308\u1100", {U"\U0001F1E6\u0308", U"\u1100"});
//÷ 1F1E6 ÷ 1160 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u1160", {U"\U0001F1E6", U"\u1160"});
//÷ 1F1E6 × 0308 ÷ 1160 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0308\u1160", {U"\U0001F1E6\u0308", U"\u1160"});
//÷ 1F1E6 ÷ 11A8 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u11A8", {U"\U0001F1E6", U"\u11A8"});
//÷ 1F1E6 × 0308 ÷ 11A8 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0308\u11A8", {U"\U0001F1E6\u0308", U"\u11A8"});
//÷ 1F1E6 ÷ AC00 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\U0001F1E6\uAC00", {U"\U0001F1E6", U"\uAC00"});
//÷ 1F1E6 × 0308 ÷ AC00 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0308\uAC00", {U"\U0001F1E6\u0308", U"\uAC00"});
//÷ 1F1E6 ÷ AC01 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\U0001F1E6\uAC01", {U"\U0001F1E6", U"\uAC01"});
//÷ 1F1E6 × 0308 ÷ AC01 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0308\uAC01", {U"\U0001F1E6\u0308", U"\uAC01"});
//÷ 1F1E6 ÷ 0915 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [999.0] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0915", {U"\U0001F1E6", U"\u0915"});
//÷ 1F1E6 × 0308 ÷ 0915 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0308\u0915", {U"\U0001F1E6\u0308", U"\u0915"});
//÷ 1F1E6 ÷ 00A9 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [999.0] COPYRIGHT SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u00A9", {U"\U0001F1E6", U"\u00A9"});
//÷ 1F1E6 × 0308 ÷ 00A9 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] COPYRIGHT SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0308\u00A9", {U"\U0001F1E6\u0308", U"\u00A9"});
//÷ 1F1E6 ÷ 0020 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [999.0] SPACE (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0020", {U"\U0001F1E6", U"\u0020"});
//÷ 1F1E6 × 0308 ÷ 0020 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] SPACE (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0308\u0020", {U"\U0001F1E6\u0308", U"\u0020"});
//÷ 1F1E6 ÷ 0378 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [999.0] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0378", {U"\U0001F1E6", U"\u0378"});
//÷ 1F1E6 × 0308 ÷ 0378 ÷
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\U0001F1E6\u0308\u0378", {U"\U0001F1E6\u0308", U"\u0378"});
//÷ 06DD ÷ 000D ÷
//÷ [0.2] ARABIC END OF AYAH (Prepend) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u06DD\u000D", {U"\u06DD", U"\u000D"});
//÷ 06DD × 0308 ÷ 000D ÷
//÷ [0.2] ARABIC END OF AYAH (Prepend) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u06DD\u0308\u000D", {U"\u06DD\u0308", U"\u000D"});
//÷ 06DD ÷ 000A ÷
//÷ [0.2] ARABIC END OF AYAH (Prepend) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u06DD\u000A", {U"\u06DD", U"\u000A"});
//÷ 06DD × 0308 ÷ 000A ÷
//÷ [0.2] ARABIC END OF AYAH (Prepend) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u06DD\u0308\u000A", {U"\u06DD\u0308", U"\u000A"});
//÷ 06DD ÷ 0001 ÷
//÷ [0.2] ARABIC END OF AYAH (Prepend) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u06DD\u0001", {U"\u06DD", U"\u0001"});
//÷ 06DD × 0308 ÷ 0001 ÷
//÷ [0.2] ARABIC END OF AYAH (Prepend) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u06DD\u0308\u0001", {U"\u06DD\u0308", U"\u0001"});
//÷ 06DD × 094D ÷
//÷ [0.2] ARABIC END OF AYAH (Prepend) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [0.3]
check_graphemes(U"\u06DD\u094D", {U"\u06DD\u094D"});
//÷ 06DD × 0308 × 094D ÷
//÷ [0.2] ARABIC END OF AYAH (Prepend) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [0.3]
check_graphemes(U"\u06DD\u0308\u094D", {U"\u06DD\u0308\u094D"});
//÷ 06DD × 0300 ÷
//÷ [0.2] ARABIC END OF AYAH (Prepend) × [9.0] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\u06DD\u0300", {U"\u06DD\u0300"});
//÷ 06DD × 0308 × 0300 ÷
//÷ [0.2] ARABIC END OF AYAH (Prepend) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\u06DD\u0308\u0300", {U"\u06DD\u0308\u0300"});
//÷ 06DD × 200C ÷
//÷ [0.2] ARABIC END OF AYAH (Prepend) × [9.0] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [0.3]
check_graphemes(U"\u06DD\u200C", {U"\u06DD\u200C"});
//÷ 06DD × 0308 × 200C ÷
//÷ [0.2] ARABIC END OF AYAH (Prepend) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [0.3]
check_graphemes(U"\u06DD\u0308\u200C", {U"\u06DD\u0308\u200C"});
//÷ 06DD × 200D ÷
//÷ [0.2] ARABIC END OF AYAH (Prepend) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [0.3]
check_graphemes(U"\u06DD\u200D", {U"\u06DD\u200D"});
//÷ 06DD × 0308 × 200D ÷
//÷ [0.2] ARABIC END OF AYAH (Prepend) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [0.3]
check_graphemes(U"\u06DD\u0308\u200D", {U"\u06DD\u0308\u200D"});
//÷ 06DD × 1F1E6 ÷
//÷ [0.2] ARABIC END OF AYAH (Prepend) × [9.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u06DD\U0001F1E6", {U"\u06DD\U0001F1E6"});
//÷ 06DD × 0308 ÷ 1F1E6 ÷
//÷ [0.2] ARABIC END OF AYAH (Prepend) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u06DD\u0308\U0001F1E6", {U"\u06DD\u0308", U"\U0001F1E6"});
//÷ 06DD × 06DD ÷
//÷ [0.2] ARABIC END OF AYAH (Prepend) × [9.2] ARABIC END OF AYAH (Prepend) ÷ [0.3]
check_graphemes(U"\u06DD\u06DD", {U"\u06DD\u06DD"});
//÷ 06DD × 0308 ÷ 06DD ÷
//÷ [0.2] ARABIC END OF AYAH (Prepend) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] ARABIC END OF AYAH (Prepend) ÷ [0.3]
check_graphemes(U"\u06DD\u0308\u06DD", {U"\u06DD\u0308", U"\u06DD"});
//÷ 06DD × 0903 ÷
//÷ [0.2] ARABIC END OF AYAH (Prepend) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u06DD\u0903", {U"\u06DD\u0903"});
//÷ 06DD × 0308 × 0903 ÷
//÷ [0.2] ARABIC END OF AYAH (Prepend) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u06DD\u0308\u0903", {U"\u06DD\u0308\u0903"});
//÷ 06DD × 1100 ÷
//÷ [0.2] ARABIC END OF AYAH (Prepend) × [9.2] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u06DD\u1100", {U"\u06DD\u1100"});
//÷ 06DD × 0308 ÷ 1100 ÷
//÷ [0.2] ARABIC END OF AYAH (Prepend) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u06DD\u0308\u1100", {U"\u06DD\u0308", U"\u1100"});
//÷ 06DD × 1160 ÷
//÷ [0.2] ARABIC END OF AYAH (Prepend) × [9.2] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u06DD\u1160", {U"\u06DD\u1160"});
//÷ 06DD × 0308 ÷ 1160 ÷
//÷ [0.2] ARABIC END OF AYAH (Prepend) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u06DD\u0308\u1160", {U"\u06DD\u0308", U"\u1160"});
//÷ 06DD × 11A8 ÷
//÷ [0.2] ARABIC END OF AYAH (Prepend) × [9.2] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u06DD\u11A8", {U"\u06DD\u11A8"});
//÷ 06DD × 0308 ÷ 11A8 ÷
//÷ [0.2] ARABIC END OF AYAH (Prepend) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u06DD\u0308\u11A8", {U"\u06DD\u0308", U"\u11A8"});
//÷ 06DD × AC00 ÷
//÷ [0.2] ARABIC END OF AYAH (Prepend) × [9.2] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u06DD\uAC00", {U"\u06DD\uAC00"});
//÷ 06DD × 0308 ÷ AC00 ÷
//÷ [0.2] ARABIC END OF AYAH (Prepend) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u06DD\u0308\uAC00", {U"\u06DD\u0308", U"\uAC00"});
//÷ 06DD × AC01 ÷
//÷ [0.2] ARABIC END OF AYAH (Prepend) × [9.2] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u06DD\uAC01", {U"\u06DD\uAC01"});
//÷ 06DD × 0308 ÷ AC01 ÷
//÷ [0.2] ARABIC END OF AYAH (Prepend) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u06DD\u0308\uAC01", {U"\u06DD\u0308", U"\uAC01"});
//÷ 06DD × 0915 ÷
//÷ [0.2] ARABIC END OF AYAH (Prepend) × [9.2] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u06DD\u0915", {U"\u06DD\u0915"});
//÷ 06DD × 0308 ÷ 0915 ÷
//÷ [0.2] ARABIC END OF AYAH (Prepend) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u06DD\u0308\u0915", {U"\u06DD\u0308", U"\u0915"});
//÷ 06DD × 00A9 ÷
//÷ [0.2] ARABIC END OF AYAH (Prepend) × [9.2] COPYRIGHT SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\u06DD\u00A9", {U"\u06DD\u00A9"});
//÷ 06DD × 0308 ÷ 00A9 ÷
//÷ [0.2] ARABIC END OF AYAH (Prepend) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] COPYRIGHT SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\u06DD\u0308\u00A9", {U"\u06DD\u0308", U"\u00A9"});
//÷ 06DD × 0020 ÷
//÷ [0.2] ARABIC END OF AYAH (Prepend) × [9.2] SPACE (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u06DD\u0020", {U"\u06DD\u0020"});
//÷ 06DD × 0308 ÷ 0020 ÷
//÷ [0.2] ARABIC END OF AYAH (Prepend) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] SPACE (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u06DD\u0308\u0020", {U"\u06DD\u0308", U"\u0020"});
//÷ 06DD × 0378 ÷
//÷ [0.2] ARABIC END OF AYAH (Prepend) × [9.2] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u06DD\u0378", {U"\u06DD\u0378"});
//÷ 06DD × 0308 ÷ 0378 ÷
//÷ [0.2] ARABIC END OF AYAH (Prepend) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u06DD\u0308\u0378", {U"\u06DD\u0308", U"\u0378"});
//÷ 0903 ÷ 000D ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u0903\u000D", {U"\u0903", U"\u000D"});
//÷ 0903 × 0308 ÷ 000D ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u0903\u0308\u000D", {U"\u0903\u0308", U"\u000D"});
//÷ 0903 ÷ 000A ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u0903\u000A", {U"\u0903", U"\u000A"});
//÷ 0903 × 0308 ÷ 000A ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u0903\u0308\u000A", {U"\u0903\u0308", U"\u000A"});
//÷ 0903 ÷ 0001 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u0903\u0001", {U"\u0903", U"\u0001"});
//÷ 0903 × 0308 ÷ 0001 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u0903\u0308\u0001", {U"\u0903\u0308", U"\u0001"});
//÷ 0903 × 094D ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [0.3]
check_graphemes(U"\u0903\u094D", {U"\u0903\u094D"});
//÷ 0903 × 0308 × 094D ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [0.3]
check_graphemes(U"\u0903\u0308\u094D", {U"\u0903\u0308\u094D"});
//÷ 0903 × 0300 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\u0903\u0300", {U"\u0903\u0300"});
//÷ 0903 × 0308 × 0300 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\u0903\u0308\u0300", {U"\u0903\u0308\u0300"});
//÷ 0903 × 200C ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [0.3]
check_graphemes(U"\u0903\u200C", {U"\u0903\u200C"});
//÷ 0903 × 0308 × 200C ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [0.3]
check_graphemes(U"\u0903\u0308\u200C", {U"\u0903\u0308\u200C"});
//÷ 0903 × 200D ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [0.3]
check_graphemes(U"\u0903\u200D", {U"\u0903\u200D"});
//÷ 0903 × 0308 × 200D ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [0.3]
check_graphemes(U"\u0903\u0308\u200D", {U"\u0903\u0308\u200D"});
//÷ 0903 ÷ 1F1E6 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u0903\U0001F1E6", {U"\u0903", U"\U0001F1E6"});
//÷ 0903 × 0308 ÷ 1F1E6 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u0903\u0308\U0001F1E6", {U"\u0903\u0308", U"\U0001F1E6"});
//÷ 0903 ÷ 06DD ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [999.0] ARABIC END OF AYAH (Prepend) ÷ [0.3]
check_graphemes(U"\u0903\u06DD", {U"\u0903", U"\u06DD"});
//÷ 0903 × 0308 ÷ 06DD ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] ARABIC END OF AYAH (Prepend) ÷ [0.3]
check_graphemes(U"\u0903\u0308\u06DD", {U"\u0903\u0308", U"\u06DD"});
//÷ 0903 × 0903 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u0903\u0903", {U"\u0903\u0903"});
//÷ 0903 × 0308 × 0903 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u0903\u0308\u0903", {U"\u0903\u0308\u0903"});
//÷ 0903 ÷ 1100 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u0903\u1100", {U"\u0903", U"\u1100"});
//÷ 0903 × 0308 ÷ 1100 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u0903\u0308\u1100", {U"\u0903\u0308", U"\u1100"});
//÷ 0903 ÷ 1160 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u0903\u1160", {U"\u0903", U"\u1160"});
//÷ 0903 × 0308 ÷ 1160 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u0903\u0308\u1160", {U"\u0903\u0308", U"\u1160"});
//÷ 0903 ÷ 11A8 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u0903\u11A8", {U"\u0903", U"\u11A8"});
//÷ 0903 × 0308 ÷ 11A8 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u0903\u0308\u11A8", {U"\u0903\u0308", U"\u11A8"});
//÷ 0903 ÷ AC00 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u0903\uAC00", {U"\u0903", U"\uAC00"});
//÷ 0903 × 0308 ÷ AC00 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u0903\u0308\uAC00", {U"\u0903\u0308", U"\uAC00"});
//÷ 0903 ÷ AC01 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u0903\uAC01", {U"\u0903", U"\uAC01"});
//÷ 0903 × 0308 ÷ AC01 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u0903\u0308\uAC01", {U"\u0903\u0308", U"\uAC01"});
//÷ 0903 ÷ 0915 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [999.0] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u0903\u0915", {U"\u0903", U"\u0915"});
//÷ 0903 × 0308 ÷ 0915 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u0903\u0308\u0915", {U"\u0903\u0308", U"\u0915"});
//÷ 0903 ÷ 00A9 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [999.0] COPYRIGHT SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\u0903\u00A9", {U"\u0903", U"\u00A9"});
//÷ 0903 × 0308 ÷ 00A9 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] COPYRIGHT SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\u0903\u0308\u00A9", {U"\u0903\u0308", U"\u00A9"});
//÷ 0903 ÷ 0020 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [999.0] SPACE (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u0903\u0020", {U"\u0903", U"\u0020"});
//÷ 0903 × 0308 ÷ 0020 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] SPACE (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u0903\u0308\u0020", {U"\u0903\u0308", U"\u0020"});
//÷ 0903 ÷ 0378 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [999.0] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u0903\u0378", {U"\u0903", U"\u0378"});
//÷ 0903 × 0308 ÷ 0378 ÷
//÷ [0.2] DEVANAGARI SIGN VISARGA (SpacingMark) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u0903\u0308\u0378", {U"\u0903\u0308", U"\u0378"});
//÷ 1100 ÷ 000D ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u1100\u000D", {U"\u1100", U"\u000D"});
//÷ 1100 × 0308 ÷ 000D ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u1100\u0308\u000D", {U"\u1100\u0308", U"\u000D"});
//÷ 1100 ÷ 000A ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u1100\u000A", {U"\u1100", U"\u000A"});
//÷ 1100 × 0308 ÷ 000A ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u1100\u0308\u000A", {U"\u1100\u0308", U"\u000A"});
//÷ 1100 ÷ 0001 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u1100\u0001", {U"\u1100", U"\u0001"});
//÷ 1100 × 0308 ÷ 0001 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u1100\u0308\u0001", {U"\u1100\u0308", U"\u0001"});
//÷ 1100 × 094D ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [0.3]
check_graphemes(U"\u1100\u094D", {U"\u1100\u094D"});
//÷ 1100 × 0308 × 094D ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [0.3]
check_graphemes(U"\u1100\u0308\u094D", {U"\u1100\u0308\u094D"});
//÷ 1100 × 0300 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\u1100\u0300", {U"\u1100\u0300"});
//÷ 1100 × 0308 × 0300 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\u1100\u0308\u0300", {U"\u1100\u0308\u0300"});
//÷ 1100 × 200C ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [0.3]
check_graphemes(U"\u1100\u200C", {U"\u1100\u200C"});
//÷ 1100 × 0308 × 200C ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [0.3]
check_graphemes(U"\u1100\u0308\u200C", {U"\u1100\u0308\u200C"});
//÷ 1100 × 200D ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [0.3]
check_graphemes(U"\u1100\u200D", {U"\u1100\u200D"});
//÷ 1100 × 0308 × 200D ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [0.3]
check_graphemes(U"\u1100\u0308\u200D", {U"\u1100\u0308\u200D"});
//÷ 1100 ÷ 1F1E6 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u1100\U0001F1E6", {U"\u1100", U"\U0001F1E6"});
//÷ 1100 × 0308 ÷ 1F1E6 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u1100\u0308\U0001F1E6", {U"\u1100\u0308", U"\U0001F1E6"});
//÷ 1100 ÷ 06DD ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) ÷ [999.0] ARABIC END OF AYAH (Prepend) ÷ [0.3]
check_graphemes(U"\u1100\u06DD", {U"\u1100", U"\u06DD"});
//÷ 1100 × 0308 ÷ 06DD ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] ARABIC END OF AYAH (Prepend) ÷ [0.3]
check_graphemes(U"\u1100\u0308\u06DD", {U"\u1100\u0308", U"\u06DD"});
//÷ 1100 × 0903 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u1100\u0903", {U"\u1100\u0903"});
//÷ 1100 × 0308 × 0903 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u1100\u0308\u0903", {U"\u1100\u0308\u0903"});
//÷ 1100 × 1100 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [6.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u1100\u1100", {U"\u1100\u1100"});
//÷ 1100 × 0308 ÷ 1100 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u1100\u0308\u1100", {U"\u1100\u0308", U"\u1100"});
//÷ 1100 × 1160 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [6.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u1100\u1160", {U"\u1100\u1160"});
//÷ 1100 × 0308 ÷ 1160 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u1100\u0308\u1160", {U"\u1100\u0308", U"\u1160"});
//÷ 1100 ÷ 11A8 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u1100\u11A8", {U"\u1100", U"\u11A8"});
//÷ 1100 × 0308 ÷ 11A8 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u1100\u0308\u11A8", {U"\u1100\u0308", U"\u11A8"});
//÷ 1100 × AC00 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [6.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u1100\uAC00", {U"\u1100\uAC00"});
//÷ 1100 × 0308 ÷ AC00 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u1100\u0308\uAC00", {U"\u1100\u0308", U"\uAC00"});
//÷ 1100 × AC01 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [6.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u1100\uAC01", {U"\u1100\uAC01"});
//÷ 1100 × 0308 ÷ AC01 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u1100\u0308\uAC01", {U"\u1100\u0308", U"\uAC01"});
//÷ 1100 ÷ 0915 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) ÷ [999.0] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u1100\u0915", {U"\u1100", U"\u0915"});
//÷ 1100 × 0308 ÷ 0915 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u1100\u0308\u0915", {U"\u1100\u0308", U"\u0915"});
//÷ 1100 ÷ 00A9 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) ÷ [999.0] COPYRIGHT SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\u1100\u00A9", {U"\u1100", U"\u00A9"});
//÷ 1100 × 0308 ÷ 00A9 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] COPYRIGHT SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\u1100\u0308\u00A9", {U"\u1100\u0308", U"\u00A9"});
//÷ 1100 ÷ 0020 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) ÷ [999.0] SPACE (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u1100\u0020", {U"\u1100", U"\u0020"});
//÷ 1100 × 0308 ÷ 0020 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] SPACE (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u1100\u0308\u0020", {U"\u1100\u0308", U"\u0020"});
//÷ 1100 ÷ 0378 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) ÷ [999.0] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u1100\u0378", {U"\u1100", U"\u0378"});
//÷ 1100 × 0308 ÷ 0378 ÷
//÷ [0.2] HANGUL CHOSEONG KIYEOK (L) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u1100\u0308\u0378", {U"\u1100\u0308", U"\u0378"});
//÷ 1160 ÷ 000D ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u1160\u000D", {U"\u1160", U"\u000D"});
//÷ 1160 × 0308 ÷ 000D ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u1160\u0308\u000D", {U"\u1160\u0308", U"\u000D"});
//÷ 1160 ÷ 000A ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u1160\u000A", {U"\u1160", U"\u000A"});
//÷ 1160 × 0308 ÷ 000A ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u1160\u0308\u000A", {U"\u1160\u0308", U"\u000A"});
//÷ 1160 ÷ 0001 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u1160\u0001", {U"\u1160", U"\u0001"});
//÷ 1160 × 0308 ÷ 0001 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u1160\u0308\u0001", {U"\u1160\u0308", U"\u0001"});
//÷ 1160 × 094D ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [0.3]
check_graphemes(U"\u1160\u094D", {U"\u1160\u094D"});
//÷ 1160 × 0308 × 094D ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [0.3]
check_graphemes(U"\u1160\u0308\u094D", {U"\u1160\u0308\u094D"});
//÷ 1160 × 0300 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\u1160\u0300", {U"\u1160\u0300"});
//÷ 1160 × 0308 × 0300 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\u1160\u0308\u0300", {U"\u1160\u0308\u0300"});
//÷ 1160 × 200C ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [0.3]
check_graphemes(U"\u1160\u200C", {U"\u1160\u200C"});
//÷ 1160 × 0308 × 200C ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [0.3]
check_graphemes(U"\u1160\u0308\u200C", {U"\u1160\u0308\u200C"});
//÷ 1160 × 200D ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [0.3]
check_graphemes(U"\u1160\u200D", {U"\u1160\u200D"});
//÷ 1160 × 0308 × 200D ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [0.3]
check_graphemes(U"\u1160\u0308\u200D", {U"\u1160\u0308\u200D"});
//÷ 1160 ÷ 1F1E6 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u1160\U0001F1E6", {U"\u1160", U"\U0001F1E6"});
//÷ 1160 × 0308 ÷ 1F1E6 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u1160\u0308\U0001F1E6", {U"\u1160\u0308", U"\U0001F1E6"});
//÷ 1160 ÷ 06DD ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) ÷ [999.0] ARABIC END OF AYAH (Prepend) ÷ [0.3]
check_graphemes(U"\u1160\u06DD", {U"\u1160", U"\u06DD"});
//÷ 1160 × 0308 ÷ 06DD ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] ARABIC END OF AYAH (Prepend) ÷ [0.3]
check_graphemes(U"\u1160\u0308\u06DD", {U"\u1160\u0308", U"\u06DD"});
//÷ 1160 × 0903 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u1160\u0903", {U"\u1160\u0903"});
//÷ 1160 × 0308 × 0903 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u1160\u0308\u0903", {U"\u1160\u0308\u0903"});
//÷ 1160 ÷ 1100 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u1160\u1100", {U"\u1160", U"\u1100"});
//÷ 1160 × 0308 ÷ 1100 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u1160\u0308\u1100", {U"\u1160\u0308", U"\u1100"});
//÷ 1160 × 1160 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [7.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u1160\u1160", {U"\u1160\u1160"});
//÷ 1160 × 0308 ÷ 1160 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u1160\u0308\u1160", {U"\u1160\u0308", U"\u1160"});
//÷ 1160 × 11A8 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [7.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u1160\u11A8", {U"\u1160\u11A8"});
//÷ 1160 × 0308 ÷ 11A8 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u1160\u0308\u11A8", {U"\u1160\u0308", U"\u11A8"});
//÷ 1160 ÷ AC00 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u1160\uAC00", {U"\u1160", U"\uAC00"});
//÷ 1160 × 0308 ÷ AC00 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u1160\u0308\uAC00", {U"\u1160\u0308", U"\uAC00"});
//÷ 1160 ÷ AC01 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u1160\uAC01", {U"\u1160", U"\uAC01"});
//÷ 1160 × 0308 ÷ AC01 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u1160\u0308\uAC01", {U"\u1160\u0308", U"\uAC01"});
//÷ 1160 ÷ 0915 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) ÷ [999.0] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u1160\u0915", {U"\u1160", U"\u0915"});
//÷ 1160 × 0308 ÷ 0915 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u1160\u0308\u0915", {U"\u1160\u0308", U"\u0915"});
//÷ 1160 ÷ 00A9 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) ÷ [999.0] COPYRIGHT SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\u1160\u00A9", {U"\u1160", U"\u00A9"});
//÷ 1160 × 0308 ÷ 00A9 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] COPYRIGHT SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\u1160\u0308\u00A9", {U"\u1160\u0308", U"\u00A9"});
//÷ 1160 ÷ 0020 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) ÷ [999.0] SPACE (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u1160\u0020", {U"\u1160", U"\u0020"});
//÷ 1160 × 0308 ÷ 0020 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] SPACE (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u1160\u0308\u0020", {U"\u1160\u0308", U"\u0020"});
//÷ 1160 ÷ 0378 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) ÷ [999.0] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u1160\u0378", {U"\u1160", U"\u0378"});
//÷ 1160 × 0308 ÷ 0378 ÷
//÷ [0.2] HANGUL JUNGSEONG FILLER (V) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u1160\u0308\u0378", {U"\u1160\u0308", U"\u0378"});
//÷ 11A8 ÷ 000D ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u11A8\u000D", {U"\u11A8", U"\u000D"});
//÷ 11A8 × 0308 ÷ 000D ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u11A8\u0308\u000D", {U"\u11A8\u0308", U"\u000D"});
//÷ 11A8 ÷ 000A ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u11A8\u000A", {U"\u11A8", U"\u000A"});
//÷ 11A8 × 0308 ÷ 000A ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u11A8\u0308\u000A", {U"\u11A8\u0308", U"\u000A"});
//÷ 11A8 ÷ 0001 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u11A8\u0001", {U"\u11A8", U"\u0001"});
//÷ 11A8 × 0308 ÷ 0001 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u11A8\u0308\u0001", {U"\u11A8\u0308", U"\u0001"});
//÷ 11A8 × 094D ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [0.3]
check_graphemes(U"\u11A8\u094D", {U"\u11A8\u094D"});
//÷ 11A8 × 0308 × 094D ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [0.3]
check_graphemes(U"\u11A8\u0308\u094D", {U"\u11A8\u0308\u094D"});
//÷ 11A8 × 0300 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\u11A8\u0300", {U"\u11A8\u0300"});
//÷ 11A8 × 0308 × 0300 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\u11A8\u0308\u0300", {U"\u11A8\u0308\u0300"});
//÷ 11A8 × 200C ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [0.3]
check_graphemes(U"\u11A8\u200C", {U"\u11A8\u200C"});
//÷ 11A8 × 0308 × 200C ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [0.3]
check_graphemes(U"\u11A8\u0308\u200C", {U"\u11A8\u0308\u200C"});
//÷ 11A8 × 200D ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [0.3]
check_graphemes(U"\u11A8\u200D", {U"\u11A8\u200D"});
//÷ 11A8 × 0308 × 200D ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [0.3]
check_graphemes(U"\u11A8\u0308\u200D", {U"\u11A8\u0308\u200D"});
//÷ 11A8 ÷ 1F1E6 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u11A8\U0001F1E6", {U"\u11A8", U"\U0001F1E6"});
//÷ 11A8 × 0308 ÷ 1F1E6 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u11A8\u0308\U0001F1E6", {U"\u11A8\u0308", U"\U0001F1E6"});
//÷ 11A8 ÷ 06DD ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) ÷ [999.0] ARABIC END OF AYAH (Prepend) ÷ [0.3]
check_graphemes(U"\u11A8\u06DD", {U"\u11A8", U"\u06DD"});
//÷ 11A8 × 0308 ÷ 06DD ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] ARABIC END OF AYAH (Prepend) ÷ [0.3]
check_graphemes(U"\u11A8\u0308\u06DD", {U"\u11A8\u0308", U"\u06DD"});
//÷ 11A8 × 0903 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u11A8\u0903", {U"\u11A8\u0903"});
//÷ 11A8 × 0308 × 0903 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u11A8\u0308\u0903", {U"\u11A8\u0308\u0903"});
//÷ 11A8 ÷ 1100 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u11A8\u1100", {U"\u11A8", U"\u1100"});
//÷ 11A8 × 0308 ÷ 1100 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u11A8\u0308\u1100", {U"\u11A8\u0308", U"\u1100"});
//÷ 11A8 ÷ 1160 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u11A8\u1160", {U"\u11A8", U"\u1160"});
//÷ 11A8 × 0308 ÷ 1160 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u11A8\u0308\u1160", {U"\u11A8\u0308", U"\u1160"});
//÷ 11A8 × 11A8 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [8.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u11A8\u11A8", {U"\u11A8\u11A8"});
//÷ 11A8 × 0308 ÷ 11A8 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u11A8\u0308\u11A8", {U"\u11A8\u0308", U"\u11A8"});
//÷ 11A8 ÷ AC00 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u11A8\uAC00", {U"\u11A8", U"\uAC00"});
//÷ 11A8 × 0308 ÷ AC00 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u11A8\u0308\uAC00", {U"\u11A8\u0308", U"\uAC00"});
//÷ 11A8 ÷ AC01 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u11A8\uAC01", {U"\u11A8", U"\uAC01"});
//÷ 11A8 × 0308 ÷ AC01 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u11A8\u0308\uAC01", {U"\u11A8\u0308", U"\uAC01"});
//÷ 11A8 ÷ 0915 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) ÷ [999.0] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u11A8\u0915", {U"\u11A8", U"\u0915"});
//÷ 11A8 × 0308 ÷ 0915 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u11A8\u0308\u0915", {U"\u11A8\u0308", U"\u0915"});
//÷ 11A8 ÷ 00A9 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) ÷ [999.0] COPYRIGHT SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\u11A8\u00A9", {U"\u11A8", U"\u00A9"});
//÷ 11A8 × 0308 ÷ 00A9 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] COPYRIGHT SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\u11A8\u0308\u00A9", {U"\u11A8\u0308", U"\u00A9"});
//÷ 11A8 ÷ 0020 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) ÷ [999.0] SPACE (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u11A8\u0020", {U"\u11A8", U"\u0020"});
//÷ 11A8 × 0308 ÷ 0020 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] SPACE (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u11A8\u0308\u0020", {U"\u11A8\u0308", U"\u0020"});
//÷ 11A8 ÷ 0378 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) ÷ [999.0] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u11A8\u0378", {U"\u11A8", U"\u0378"});
//÷ 11A8 × 0308 ÷ 0378 ÷
//÷ [0.2] HANGUL JONGSEONG KIYEOK (T) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u11A8\u0308\u0378", {U"\u11A8\u0308", U"\u0378"});
//÷ AC00 ÷ 000D ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\uAC00\u000D", {U"\uAC00", U"\u000D"});
//÷ AC00 × 0308 ÷ 000D ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\uAC00\u0308\u000D", {U"\uAC00\u0308", U"\u000D"});
//÷ AC00 ÷ 000A ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\uAC00\u000A", {U"\uAC00", U"\u000A"});
//÷ AC00 × 0308 ÷ 000A ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\uAC00\u0308\u000A", {U"\uAC00\u0308", U"\u000A"});
//÷ AC00 ÷ 0001 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\uAC00\u0001", {U"\uAC00", U"\u0001"});
//÷ AC00 × 0308 ÷ 0001 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\uAC00\u0308\u0001", {U"\uAC00\u0308", U"\u0001"});
//÷ AC00 × 094D ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [0.3]
check_graphemes(U"\uAC00\u094D", {U"\uAC00\u094D"});
//÷ AC00 × 0308 × 094D ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [0.3]
check_graphemes(U"\uAC00\u0308\u094D", {U"\uAC00\u0308\u094D"});
//÷ AC00 × 0300 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\uAC00\u0300", {U"\uAC00\u0300"});
//÷ AC00 × 0308 × 0300 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\uAC00\u0308\u0300", {U"\uAC00\u0308\u0300"});
//÷ AC00 × 200C ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [0.3]
check_graphemes(U"\uAC00\u200C", {U"\uAC00\u200C"});
//÷ AC00 × 0308 × 200C ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [0.3]
check_graphemes(U"\uAC00\u0308\u200C", {U"\uAC00\u0308\u200C"});
//÷ AC00 × 200D ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [0.3]
check_graphemes(U"\uAC00\u200D", {U"\uAC00\u200D"});
//÷ AC00 × 0308 × 200D ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [0.3]
check_graphemes(U"\uAC00\u0308\u200D", {U"\uAC00\u0308\u200D"});
//÷ AC00 ÷ 1F1E6 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\uAC00\U0001F1E6", {U"\uAC00", U"\U0001F1E6"});
//÷ AC00 × 0308 ÷ 1F1E6 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\uAC00\u0308\U0001F1E6", {U"\uAC00\u0308", U"\U0001F1E6"});
//÷ AC00 ÷ 06DD ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) ÷ [999.0] ARABIC END OF AYAH (Prepend) ÷ [0.3]
check_graphemes(U"\uAC00\u06DD", {U"\uAC00", U"\u06DD"});
//÷ AC00 × 0308 ÷ 06DD ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] ARABIC END OF AYAH (Prepend) ÷ [0.3]
check_graphemes(U"\uAC00\u0308\u06DD", {U"\uAC00\u0308", U"\u06DD"});
//÷ AC00 × 0903 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\uAC00\u0903", {U"\uAC00\u0903"});
//÷ AC00 × 0308 × 0903 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\uAC00\u0308\u0903", {U"\uAC00\u0308\u0903"});
//÷ AC00 ÷ 1100 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\uAC00\u1100", {U"\uAC00", U"\u1100"});
//÷ AC00 × 0308 ÷ 1100 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\uAC00\u0308\u1100", {U"\uAC00\u0308", U"\u1100"});
//÷ AC00 × 1160 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [7.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\uAC00\u1160", {U"\uAC00\u1160"});
//÷ AC00 × 0308 ÷ 1160 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\uAC00\u0308\u1160", {U"\uAC00\u0308", U"\u1160"});
//÷ AC00 × 11A8 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [7.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\uAC00\u11A8", {U"\uAC00\u11A8"});
//÷ AC00 × 0308 ÷ 11A8 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\uAC00\u0308\u11A8", {U"\uAC00\u0308", U"\u11A8"});
//÷ AC00 ÷ AC00 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\uAC00\uAC00", {U"\uAC00", U"\uAC00"});
//÷ AC00 × 0308 ÷ AC00 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\uAC00\u0308\uAC00", {U"\uAC00\u0308", U"\uAC00"});
//÷ AC00 ÷ AC01 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\uAC00\uAC01", {U"\uAC00", U"\uAC01"});
//÷ AC00 × 0308 ÷ AC01 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\uAC00\u0308\uAC01", {U"\uAC00\u0308", U"\uAC01"});
//÷ AC00 ÷ 0915 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) ÷ [999.0] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\uAC00\u0915", {U"\uAC00", U"\u0915"});
//÷ AC00 × 0308 ÷ 0915 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\uAC00\u0308\u0915", {U"\uAC00\u0308", U"\u0915"});
//÷ AC00 ÷ 00A9 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) ÷ [999.0] COPYRIGHT SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\uAC00\u00A9", {U"\uAC00", U"\u00A9"});
//÷ AC00 × 0308 ÷ 00A9 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] COPYRIGHT SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\uAC00\u0308\u00A9", {U"\uAC00\u0308", U"\u00A9"});
//÷ AC00 ÷ 0020 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) ÷ [999.0] SPACE (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\uAC00\u0020", {U"\uAC00", U"\u0020"});
//÷ AC00 × 0308 ÷ 0020 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] SPACE (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\uAC00\u0308\u0020", {U"\uAC00\u0308", U"\u0020"});
//÷ AC00 ÷ 0378 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) ÷ [999.0] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\uAC00\u0378", {U"\uAC00", U"\u0378"});
//÷ AC00 × 0308 ÷ 0378 ÷
//÷ [0.2] HANGUL SYLLABLE GA (LV) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\uAC00\u0308\u0378", {U"\uAC00\u0308", U"\u0378"});
//÷ AC01 ÷ 000D ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\uAC01\u000D", {U"\uAC01", U"\u000D"});
//÷ AC01 × 0308 ÷ 000D ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\uAC01\u0308\u000D", {U"\uAC01\u0308", U"\u000D"});
//÷ AC01 ÷ 000A ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\uAC01\u000A", {U"\uAC01", U"\u000A"});
//÷ AC01 × 0308 ÷ 000A ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\uAC01\u0308\u000A", {U"\uAC01\u0308", U"\u000A"});
//÷ AC01 ÷ 0001 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\uAC01\u0001", {U"\uAC01", U"\u0001"});
//÷ AC01 × 0308 ÷ 0001 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\uAC01\u0308\u0001", {U"\uAC01\u0308", U"\u0001"});
//÷ AC01 × 094D ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [0.3]
check_graphemes(U"\uAC01\u094D", {U"\uAC01\u094D"});
//÷ AC01 × 0308 × 094D ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [0.3]
check_graphemes(U"\uAC01\u0308\u094D", {U"\uAC01\u0308\u094D"});
//÷ AC01 × 0300 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\uAC01\u0300", {U"\uAC01\u0300"});
//÷ AC01 × 0308 × 0300 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\uAC01\u0308\u0300", {U"\uAC01\u0308\u0300"});
//÷ AC01 × 200C ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [0.3]
check_graphemes(U"\uAC01\u200C", {U"\uAC01\u200C"});
//÷ AC01 × 0308 × 200C ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [0.3]
check_graphemes(U"\uAC01\u0308\u200C", {U"\uAC01\u0308\u200C"});
//÷ AC01 × 200D ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [0.3]
check_graphemes(U"\uAC01\u200D", {U"\uAC01\u200D"});
//÷ AC01 × 0308 × 200D ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [0.3]
check_graphemes(U"\uAC01\u0308\u200D", {U"\uAC01\u0308\u200D"});
//÷ AC01 ÷ 1F1E6 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\uAC01\U0001F1E6", {U"\uAC01", U"\U0001F1E6"});
//÷ AC01 × 0308 ÷ 1F1E6 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\uAC01\u0308\U0001F1E6", {U"\uAC01\u0308", U"\U0001F1E6"});
//÷ AC01 ÷ 06DD ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) ÷ [999.0] ARABIC END OF AYAH (Prepend) ÷ [0.3]
check_graphemes(U"\uAC01\u06DD", {U"\uAC01", U"\u06DD"});
//÷ AC01 × 0308 ÷ 06DD ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] ARABIC END OF AYAH (Prepend) ÷ [0.3]
check_graphemes(U"\uAC01\u0308\u06DD", {U"\uAC01\u0308", U"\u06DD"});
//÷ AC01 × 0903 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\uAC01\u0903", {U"\uAC01\u0903"});
//÷ AC01 × 0308 × 0903 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\uAC01\u0308\u0903", {U"\uAC01\u0308\u0903"});
//÷ AC01 ÷ 1100 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\uAC01\u1100", {U"\uAC01", U"\u1100"});
//÷ AC01 × 0308 ÷ 1100 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\uAC01\u0308\u1100", {U"\uAC01\u0308", U"\u1100"});
//÷ AC01 ÷ 1160 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\uAC01\u1160", {U"\uAC01", U"\u1160"});
//÷ AC01 × 0308 ÷ 1160 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\uAC01\u0308\u1160", {U"\uAC01\u0308", U"\u1160"});
//÷ AC01 × 11A8 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [8.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\uAC01\u11A8", {U"\uAC01\u11A8"});
//÷ AC01 × 0308 ÷ 11A8 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\uAC01\u0308\u11A8", {U"\uAC01\u0308", U"\u11A8"});
//÷ AC01 ÷ AC00 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\uAC01\uAC00", {U"\uAC01", U"\uAC00"});
//÷ AC01 × 0308 ÷ AC00 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\uAC01\u0308\uAC00", {U"\uAC01\u0308", U"\uAC00"});
//÷ AC01 ÷ AC01 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\uAC01\uAC01", {U"\uAC01", U"\uAC01"});
//÷ AC01 × 0308 ÷ AC01 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\uAC01\u0308\uAC01", {U"\uAC01\u0308", U"\uAC01"});
//÷ AC01 ÷ 0915 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) ÷ [999.0] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\uAC01\u0915", {U"\uAC01", U"\u0915"});
//÷ AC01 × 0308 ÷ 0915 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\uAC01\u0308\u0915", {U"\uAC01\u0308", U"\u0915"});
//÷ AC01 ÷ 00A9 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) ÷ [999.0] COPYRIGHT SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\uAC01\u00A9", {U"\uAC01", U"\u00A9"});
//÷ AC01 × 0308 ÷ 00A9 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] COPYRIGHT SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\uAC01\u0308\u00A9", {U"\uAC01\u0308", U"\u00A9"});
//÷ AC01 ÷ 0020 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) ÷ [999.0] SPACE (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\uAC01\u0020", {U"\uAC01", U"\u0020"});
//÷ AC01 × 0308 ÷ 0020 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] SPACE (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\uAC01\u0308\u0020", {U"\uAC01\u0308", U"\u0020"});
//÷ AC01 ÷ 0378 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) ÷ [999.0] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\uAC01\u0378", {U"\uAC01", U"\u0378"});
//÷ AC01 × 0308 ÷ 0378 ÷
//÷ [0.2] HANGUL SYLLABLE GAG (LVT) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\uAC01\u0308\u0378", {U"\uAC01\u0308", U"\u0378"});
//÷ 0915 ÷ 000D ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u0915\u000D", {U"\u0915", U"\u000D"});
//÷ 0915 × 0308 ÷ 000D ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u0915\u0308\u000D", {U"\u0915\u0308", U"\u000D"});
//÷ 0915 ÷ 000A ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u0915\u000A", {U"\u0915", U"\u000A"});
//÷ 0915 × 0308 ÷ 000A ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u0915\u0308\u000A", {U"\u0915\u0308", U"\u000A"});
//÷ 0915 ÷ 0001 ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u0915\u0001", {U"\u0915", U"\u0001"});
//÷ 0915 × 0308 ÷ 0001 ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u0915\u0308\u0001", {U"\u0915\u0308", U"\u0001"});
//÷ 0915 × 094D ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [0.3]
check_graphemes(U"\u0915\u094D", {U"\u0915\u094D"});
//÷ 0915 × 0308 × 094D ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [0.3]
check_graphemes(U"\u0915\u0308\u094D", {U"\u0915\u0308\u094D"});
//÷ 0915 × 0300 ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) × [9.0] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\u0915\u0300", {U"\u0915\u0300"});
//÷ 0915 × 0308 × 0300 ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\u0915\u0308\u0300", {U"\u0915\u0308\u0300"});
//÷ 0915 × 200C ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) × [9.0] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [0.3]
check_graphemes(U"\u0915\u200C", {U"\u0915\u200C"});
//÷ 0915 × 0308 × 200C ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [0.3]
check_graphemes(U"\u0915\u0308\u200C", {U"\u0915\u0308\u200C"});
//÷ 0915 × 200D ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [0.3]
check_graphemes(U"\u0915\u200D", {U"\u0915\u200D"});
//÷ 0915 × 0308 × 200D ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [0.3]
check_graphemes(U"\u0915\u0308\u200D", {U"\u0915\u0308\u200D"});
//÷ 0915 ÷ 1F1E6 ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u0915\U0001F1E6", {U"\u0915", U"\U0001F1E6"});
//÷ 0915 × 0308 ÷ 1F1E6 ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u0915\u0308\U0001F1E6", {U"\u0915\u0308", U"\U0001F1E6"});
//÷ 0915 ÷ 06DD ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [999.0] ARABIC END OF AYAH (Prepend) ÷ [0.3]
check_graphemes(U"\u0915\u06DD", {U"\u0915", U"\u06DD"});
//÷ 0915 × 0308 ÷ 06DD ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] ARABIC END OF AYAH (Prepend) ÷ [0.3]
check_graphemes(U"\u0915\u0308\u06DD", {U"\u0915\u0308", U"\u06DD"});
//÷ 0915 × 0903 ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u0915\u0903", {U"\u0915\u0903"});
//÷ 0915 × 0308 × 0903 ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u0915\u0308\u0903", {U"\u0915\u0308\u0903"});
//÷ 0915 ÷ 1100 ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u0915\u1100", {U"\u0915", U"\u1100"});
//÷ 0915 × 0308 ÷ 1100 ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u0915\u0308\u1100", {U"\u0915\u0308", U"\u1100"});
//÷ 0915 ÷ 1160 ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u0915\u1160", {U"\u0915", U"\u1160"});
//÷ 0915 × 0308 ÷ 1160 ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u0915\u0308\u1160", {U"\u0915\u0308", U"\u1160"});
//÷ 0915 ÷ 11A8 ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u0915\u11A8", {U"\u0915", U"\u11A8"});
//÷ 0915 × 0308 ÷ 11A8 ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u0915\u0308\u11A8", {U"\u0915\u0308", U"\u11A8"});
//÷ 0915 ÷ AC00 ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u0915\uAC00", {U"\u0915", U"\uAC00"});
//÷ 0915 × 0308 ÷ AC00 ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u0915\u0308\uAC00", {U"\u0915\u0308", U"\uAC00"});
//÷ 0915 ÷ AC01 ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u0915\uAC01", {U"\u0915", U"\uAC01"});
//÷ 0915 × 0308 ÷ AC01 ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u0915\u0308\uAC01", {U"\u0915\u0308", U"\uAC01"});
//÷ 0915 ÷ 0915 ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [999.0] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u0915\u0915", {U"\u0915", U"\u0915"});
//÷ 0915 × 0308 ÷ 0915 ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u0915\u0308\u0915", {U"\u0915\u0308", U"\u0915"});
//÷ 0915 ÷ 00A9 ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [999.0] COPYRIGHT SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\u0915\u00A9", {U"\u0915", U"\u00A9"});
//÷ 0915 × 0308 ÷ 00A9 ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] COPYRIGHT SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\u0915\u0308\u00A9", {U"\u0915\u0308", U"\u00A9"});
//÷ 0915 ÷ 0020 ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [999.0] SPACE (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u0915\u0020", {U"\u0915", U"\u0020"});
//÷ 0915 × 0308 ÷ 0020 ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] SPACE (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u0915\u0308\u0020", {U"\u0915\u0308", U"\u0020"});
//÷ 0915 ÷ 0378 ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [999.0] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u0915\u0378", {U"\u0915", U"\u0378"});
//÷ 0915 × 0308 ÷ 0378 ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u0915\u0308\u0378", {U"\u0915\u0308", U"\u0378"});
//÷ 00A9 ÷ 000D ÷
//÷ [0.2] COPYRIGHT SIGN (ExtPict) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u00A9\u000D", {U"\u00A9", U"\u000D"});
//÷ 00A9 × 0308 ÷ 000D ÷
//÷ [0.2] COPYRIGHT SIGN (ExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u00A9\u0308\u000D", {U"\u00A9\u0308", U"\u000D"});
//÷ 00A9 ÷ 000A ÷
//÷ [0.2] COPYRIGHT SIGN (ExtPict) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u00A9\u000A", {U"\u00A9", U"\u000A"});
//÷ 00A9 × 0308 ÷ 000A ÷
//÷ [0.2] COPYRIGHT SIGN (ExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u00A9\u0308\u000A", {U"\u00A9\u0308", U"\u000A"});
//÷ 00A9 ÷ 0001 ÷
//÷ [0.2] COPYRIGHT SIGN (ExtPict) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u00A9\u0001", {U"\u00A9", U"\u0001"});
//÷ 00A9 × 0308 ÷ 0001 ÷
//÷ [0.2] COPYRIGHT SIGN (ExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u00A9\u0308\u0001", {U"\u00A9\u0308", U"\u0001"});
//÷ 00A9 × 094D ÷
//÷ [0.2] COPYRIGHT SIGN (ExtPict) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [0.3]
check_graphemes(U"\u00A9\u094D", {U"\u00A9\u094D"});
//÷ 00A9 × 0308 × 094D ÷
//÷ [0.2] COPYRIGHT SIGN (ExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [0.3]
check_graphemes(U"\u00A9\u0308\u094D", {U"\u00A9\u0308\u094D"});
//÷ 00A9 × 0300 ÷
//÷ [0.2] COPYRIGHT SIGN (ExtPict) × [9.0] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\u00A9\u0300", {U"\u00A9\u0300"});
//÷ 00A9 × 0308 × 0300 ÷
//÷ [0.2] COPYRIGHT SIGN (ExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\u00A9\u0308\u0300", {U"\u00A9\u0308\u0300"});
//÷ 00A9 × 200C ÷
//÷ [0.2] COPYRIGHT SIGN (ExtPict) × [9.0] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [0.3]
check_graphemes(U"\u00A9\u200C", {U"\u00A9\u200C"});
//÷ 00A9 × 0308 × 200C ÷
//÷ [0.2] COPYRIGHT SIGN (ExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [0.3]
check_graphemes(U"\u00A9\u0308\u200C", {U"\u00A9\u0308\u200C"});
//÷ 00A9 × 200D ÷
//÷ [0.2] COPYRIGHT SIGN (ExtPict) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [0.3]
check_graphemes(U"\u00A9\u200D", {U"\u00A9\u200D"});
//÷ 00A9 × 0308 × 200D ÷
//÷ [0.2] COPYRIGHT SIGN (ExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [0.3]
check_graphemes(U"\u00A9\u0308\u200D", {U"\u00A9\u0308\u200D"});
//÷ 00A9 ÷ 1F1E6 ÷
//÷ [0.2] COPYRIGHT SIGN (ExtPict) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u00A9\U0001F1E6", {U"\u00A9", U"\U0001F1E6"});
//÷ 00A9 × 0308 ÷ 1F1E6 ÷
//÷ [0.2] COPYRIGHT SIGN (ExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u00A9\u0308\U0001F1E6", {U"\u00A9\u0308", U"\U0001F1E6"});
//÷ 00A9 ÷ 06DD ÷
//÷ [0.2] COPYRIGHT SIGN (ExtPict) ÷ [999.0] ARABIC END OF AYAH (Prepend) ÷ [0.3]
check_graphemes(U"\u00A9\u06DD", {U"\u00A9", U"\u06DD"});
//÷ 00A9 × 0308 ÷ 06DD ÷
//÷ [0.2] COPYRIGHT SIGN (ExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] ARABIC END OF AYAH (Prepend) ÷ [0.3]
check_graphemes(U"\u00A9\u0308\u06DD", {U"\u00A9\u0308", U"\u06DD"});
//÷ 00A9 × 0903 ÷
//÷ [0.2] COPYRIGHT SIGN (ExtPict) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u00A9\u0903", {U"\u00A9\u0903"});
//÷ 00A9 × 0308 × 0903 ÷
//÷ [0.2] COPYRIGHT SIGN (ExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u00A9\u0308\u0903", {U"\u00A9\u0308\u0903"});
//÷ 00A9 ÷ 1100 ÷
//÷ [0.2] COPYRIGHT SIGN (ExtPict) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u00A9\u1100", {U"\u00A9", U"\u1100"});
//÷ 00A9 × 0308 ÷ 1100 ÷
//÷ [0.2] COPYRIGHT SIGN (ExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u00A9\u0308\u1100", {U"\u00A9\u0308", U"\u1100"});
//÷ 00A9 ÷ 1160 ÷
//÷ [0.2] COPYRIGHT SIGN (ExtPict) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u00A9\u1160", {U"\u00A9", U"\u1160"});
//÷ 00A9 × 0308 ÷ 1160 ÷
//÷ [0.2] COPYRIGHT SIGN (ExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u00A9\u0308\u1160", {U"\u00A9\u0308", U"\u1160"});
//÷ 00A9 ÷ 11A8 ÷
//÷ [0.2] COPYRIGHT SIGN (ExtPict) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u00A9\u11A8", {U"\u00A9", U"\u11A8"});
//÷ 00A9 × 0308 ÷ 11A8 ÷
//÷ [0.2] COPYRIGHT SIGN (ExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u00A9\u0308\u11A8", {U"\u00A9\u0308", U"\u11A8"});
//÷ 00A9 ÷ AC00 ÷
//÷ [0.2] COPYRIGHT SIGN (ExtPict) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u00A9\uAC00", {U"\u00A9", U"\uAC00"});
//÷ 00A9 × 0308 ÷ AC00 ÷
//÷ [0.2] COPYRIGHT SIGN (ExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u00A9\u0308\uAC00", {U"\u00A9\u0308", U"\uAC00"});
//÷ 00A9 ÷ AC01 ÷
//÷ [0.2] COPYRIGHT SIGN (ExtPict) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u00A9\uAC01", {U"\u00A9", U"\uAC01"});
//÷ 00A9 × 0308 ÷ AC01 ÷
//÷ [0.2] COPYRIGHT SIGN (ExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u00A9\u0308\uAC01", {U"\u00A9\u0308", U"\uAC01"});
//÷ 00A9 ÷ 0915 ÷
//÷ [0.2] COPYRIGHT SIGN (ExtPict) ÷ [999.0] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u00A9\u0915", {U"\u00A9", U"\u0915"});
//÷ 00A9 × 0308 ÷ 0915 ÷
//÷ [0.2] COPYRIGHT SIGN (ExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u00A9\u0308\u0915", {U"\u00A9\u0308", U"\u0915"});
//÷ 00A9 ÷ 00A9 ÷
//÷ [0.2] COPYRIGHT SIGN (ExtPict) ÷ [999.0] COPYRIGHT SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\u00A9\u00A9", {U"\u00A9", U"\u00A9"});
//÷ 00A9 × 0308 ÷ 00A9 ÷
//÷ [0.2] COPYRIGHT SIGN (ExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] COPYRIGHT SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\u00A9\u0308\u00A9", {U"\u00A9\u0308", U"\u00A9"});
//÷ 00A9 ÷ 0020 ÷
//÷ [0.2] COPYRIGHT SIGN (ExtPict) ÷ [999.0] SPACE (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u00A9\u0020", {U"\u00A9", U"\u0020"});
//÷ 00A9 × 0308 ÷ 0020 ÷
//÷ [0.2] COPYRIGHT SIGN (ExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] SPACE (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u00A9\u0308\u0020", {U"\u00A9\u0308", U"\u0020"});
//÷ 00A9 ÷ 0378 ÷
//÷ [0.2] COPYRIGHT SIGN (ExtPict) ÷ [999.0] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u00A9\u0378", {U"\u00A9", U"\u0378"});
//÷ 00A9 × 0308 ÷ 0378 ÷
//÷ [0.2] COPYRIGHT SIGN (ExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u00A9\u0308\u0378", {U"\u00A9\u0308", U"\u0378"});
//÷ 0020 ÷ 000D ÷
//÷ [0.2] SPACE (XXmLinkingConsonantmExtPict) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u0020\u000D", {U"\u0020", U"\u000D"});
//÷ 0020 × 0308 ÷ 000D ÷
//÷ [0.2] SPACE (XXmLinkingConsonantmExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u0020\u0308\u000D", {U"\u0020\u0308", U"\u000D"});
//÷ 0020 ÷ 000A ÷
//÷ [0.2] SPACE (XXmLinkingConsonantmExtPict) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u0020\u000A", {U"\u0020", U"\u000A"});
//÷ 0020 × 0308 ÷ 000A ÷
//÷ [0.2] SPACE (XXmLinkingConsonantmExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u0020\u0308\u000A", {U"\u0020\u0308", U"\u000A"});
//÷ 0020 ÷ 0001 ÷
//÷ [0.2] SPACE (XXmLinkingConsonantmExtPict) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u0020\u0001", {U"\u0020", U"\u0001"});
//÷ 0020 × 0308 ÷ 0001 ÷
//÷ [0.2] SPACE (XXmLinkingConsonantmExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u0020\u0308\u0001", {U"\u0020\u0308", U"\u0001"});
//÷ 0020 × 094D ÷
//÷ [0.2] SPACE (XXmLinkingConsonantmExtPict) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [0.3]
check_graphemes(U"\u0020\u094D", {U"\u0020\u094D"});
//÷ 0020 × 0308 × 094D ÷
//÷ [0.2] SPACE (XXmLinkingConsonantmExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [0.3]
check_graphemes(U"\u0020\u0308\u094D", {U"\u0020\u0308\u094D"});
//÷ 0020 × 0300 ÷
//÷ [0.2] SPACE (XXmLinkingConsonantmExtPict) × [9.0] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\u0020\u0300", {U"\u0020\u0300"});
//÷ 0020 × 0308 × 0300 ÷
//÷ [0.2] SPACE (XXmLinkingConsonantmExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\u0020\u0308\u0300", {U"\u0020\u0308\u0300"});
//÷ 0020 × 200C ÷
//÷ [0.2] SPACE (XXmLinkingConsonantmExtPict) × [9.0] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [0.3]
check_graphemes(U"\u0020\u200C", {U"\u0020\u200C"});
//÷ 0020 × 0308 × 200C ÷
//÷ [0.2] SPACE (XXmLinkingConsonantmExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [0.3]
check_graphemes(U"\u0020\u0308\u200C", {U"\u0020\u0308\u200C"});
//÷ 0020 × 200D ÷
//÷ [0.2] SPACE (XXmLinkingConsonantmExtPict) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [0.3]
check_graphemes(U"\u0020\u200D", {U"\u0020\u200D"});
//÷ 0020 × 0308 × 200D ÷
//÷ [0.2] SPACE (XXmLinkingConsonantmExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [0.3]
check_graphemes(U"\u0020\u0308\u200D", {U"\u0020\u0308\u200D"});
//÷ 0020 ÷ 1F1E6 ÷
//÷ [0.2] SPACE (XXmLinkingConsonantmExtPict) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u0020\U0001F1E6", {U"\u0020", U"\U0001F1E6"});
//÷ 0020 × 0308 ÷ 1F1E6 ÷
//÷ [0.2] SPACE (XXmLinkingConsonantmExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u0020\u0308\U0001F1E6", {U"\u0020\u0308", U"\U0001F1E6"});
//÷ 0020 ÷ 06DD ÷
//÷ [0.2] SPACE (XXmLinkingConsonantmExtPict) ÷ [999.0] ARABIC END OF AYAH (Prepend) ÷ [0.3]
check_graphemes(U"\u0020\u06DD", {U"\u0020", U"\u06DD"});
//÷ 0020 × 0308 ÷ 06DD ÷
//÷ [0.2] SPACE (XXmLinkingConsonantmExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] ARABIC END OF AYAH (Prepend) ÷ [0.3]
check_graphemes(U"\u0020\u0308\u06DD", {U"\u0020\u0308", U"\u06DD"});
//÷ 0020 × 0903 ÷
//÷ [0.2] SPACE (XXmLinkingConsonantmExtPict) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u0020\u0903", {U"\u0020\u0903"});
//÷ 0020 × 0308 × 0903 ÷
//÷ [0.2] SPACE (XXmLinkingConsonantmExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u0020\u0308\u0903", {U"\u0020\u0308\u0903"});
//÷ 0020 ÷ 1100 ÷
//÷ [0.2] SPACE (XXmLinkingConsonantmExtPict) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u0020\u1100", {U"\u0020", U"\u1100"});
//÷ 0020 × 0308 ÷ 1100 ÷
//÷ [0.2] SPACE (XXmLinkingConsonantmExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u0020\u0308\u1100", {U"\u0020\u0308", U"\u1100"});
//÷ 0020 ÷ 1160 ÷
//÷ [0.2] SPACE (XXmLinkingConsonantmExtPict) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u0020\u1160", {U"\u0020", U"\u1160"});
//÷ 0020 × 0308 ÷ 1160 ÷
//÷ [0.2] SPACE (XXmLinkingConsonantmExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u0020\u0308\u1160", {U"\u0020\u0308", U"\u1160"});
//÷ 0020 ÷ 11A8 ÷
//÷ [0.2] SPACE (XXmLinkingConsonantmExtPict) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u0020\u11A8", {U"\u0020", U"\u11A8"});
//÷ 0020 × 0308 ÷ 11A8 ÷
//÷ [0.2] SPACE (XXmLinkingConsonantmExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u0020\u0308\u11A8", {U"\u0020\u0308", U"\u11A8"});
//÷ 0020 ÷ AC00 ÷
//÷ [0.2] SPACE (XXmLinkingConsonantmExtPict) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u0020\uAC00", {U"\u0020", U"\uAC00"});
//÷ 0020 × 0308 ÷ AC00 ÷
//÷ [0.2] SPACE (XXmLinkingConsonantmExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u0020\u0308\uAC00", {U"\u0020\u0308", U"\uAC00"});
//÷ 0020 ÷ AC01 ÷
//÷ [0.2] SPACE (XXmLinkingConsonantmExtPict) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u0020\uAC01", {U"\u0020", U"\uAC01"});
//÷ 0020 × 0308 ÷ AC01 ÷
//÷ [0.2] SPACE (XXmLinkingConsonantmExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u0020\u0308\uAC01", {U"\u0020\u0308", U"\uAC01"});
//÷ 0020 ÷ 0915 ÷
//÷ [0.2] SPACE (XXmLinkingConsonantmExtPict) ÷ [999.0] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u0020\u0915", {U"\u0020", U"\u0915"});
//÷ 0020 × 0308 ÷ 0915 ÷
//÷ [0.2] SPACE (XXmLinkingConsonantmExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u0020\u0308\u0915", {U"\u0020\u0308", U"\u0915"});
//÷ 0020 ÷ 00A9 ÷
//÷ [0.2] SPACE (XXmLinkingConsonantmExtPict) ÷ [999.0] COPYRIGHT SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\u0020\u00A9", {U"\u0020", U"\u00A9"});
//÷ 0020 × 0308 ÷ 00A9 ÷
//÷ [0.2] SPACE (XXmLinkingConsonantmExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] COPYRIGHT SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\u0020\u0308\u00A9", {U"\u0020\u0308", U"\u00A9"});
//÷ 0020 ÷ 0020 ÷
//÷ [0.2] SPACE (XXmLinkingConsonantmExtPict) ÷ [999.0] SPACE (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u0020\u0020", {U"\u0020", U"\u0020"});
//÷ 0020 × 0308 ÷ 0020 ÷
//÷ [0.2] SPACE (XXmLinkingConsonantmExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] SPACE (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u0020\u0308\u0020", {U"\u0020\u0308", U"\u0020"});
//÷ 0020 ÷ 0378 ÷
//÷ [0.2] SPACE (XXmLinkingConsonantmExtPict) ÷ [999.0] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u0020\u0378", {U"\u0020", U"\u0378"});
//÷ 0020 × 0308 ÷ 0378 ÷
//÷ [0.2] SPACE (XXmLinkingConsonantmExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u0020\u0308\u0378", {U"\u0020\u0308", U"\u0378"});
//÷ 0378 ÷ 000D ÷
//÷ [0.2] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u0378\u000D", {U"\u0378", U"\u000D"});
//÷ 0378 × 0308 ÷ 000D ÷
//÷ [0.2] <reserved-0378> (XXmLinkingConsonantmExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <CARRIAGE RETURN (CR)> (CR) ÷ [0.3]
check_graphemes(U"\u0378\u0308\u000D", {U"\u0378\u0308", U"\u000D"});
//÷ 0378 ÷ 000A ÷
//÷ [0.2] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u0378\u000A", {U"\u0378", U"\u000A"});
//÷ 0378 × 0308 ÷ 000A ÷
//÷ [0.2] <reserved-0378> (XXmLinkingConsonantmExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [0.3]
check_graphemes(U"\u0378\u0308\u000A", {U"\u0378\u0308", U"\u000A"});
//÷ 0378 ÷ 0001 ÷
//÷ [0.2] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u0378\u0001", {U"\u0378", U"\u0001"});
//÷ 0378 × 0308 ÷ 0001 ÷
//÷ [0.2] <reserved-0378> (XXmLinkingConsonantmExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [5.0] <START OF HEADING> (Control) ÷ [0.3]
check_graphemes(U"\u0378\u0308\u0001", {U"\u0378\u0308", U"\u0001"});
//÷ 0378 × 094D ÷
//÷ [0.2] <reserved-0378> (XXmLinkingConsonantmExtPict) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [0.3]
check_graphemes(U"\u0378\u094D", {U"\u0378\u094D"});
//÷ 0378 × 0308 × 094D ÷
//÷ [0.2] <reserved-0378> (XXmLinkingConsonantmExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [0.3]
check_graphemes(U"\u0378\u0308\u094D", {U"\u0378\u0308\u094D"});
//÷ 0378 × 0300 ÷
//÷ [0.2] <reserved-0378> (XXmLinkingConsonantmExtPict) × [9.0] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\u0378\u0300", {U"\u0378\u0300"});
//÷ 0378 × 0308 × 0300 ÷
//÷ [0.2] <reserved-0378> (XXmLinkingConsonantmExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] COMBINING GRAVE ACCENT (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\u0378\u0308\u0300", {U"\u0378\u0308\u0300"});
//÷ 0378 × 200C ÷
//÷ [0.2] <reserved-0378> (XXmLinkingConsonantmExtPict) × [9.0] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [0.3]
check_graphemes(U"\u0378\u200C", {U"\u0378\u200C"});
//÷ 0378 × 0308 × 200C ÷
//÷ [0.2] <reserved-0378> (XXmLinkingConsonantmExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] ZERO WIDTH NON-JOINER (ExtendmConjunctLinkermConjunctExtender) ÷ [0.3]
check_graphemes(U"\u0378\u0308\u200C", {U"\u0378\u0308\u200C"});
//÷ 0378 × 200D ÷
//÷ [0.2] <reserved-0378> (XXmLinkingConsonantmExtPict) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [0.3]
check_graphemes(U"\u0378\u200D", {U"\u0378\u200D"});
//÷ 0378 × 0308 × 200D ÷
//÷ [0.2] <reserved-0378> (XXmLinkingConsonantmExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [0.3]
check_graphemes(U"\u0378\u0308\u200D", {U"\u0378\u0308\u200D"});
//÷ 0378 ÷ 1F1E6 ÷
//÷ [0.2] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u0378\U0001F1E6", {U"\u0378", U"\U0001F1E6"});
//÷ 0378 × 0308 ÷ 1F1E6 ÷
//÷ [0.2] <reserved-0378> (XXmLinkingConsonantmExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) ÷ [0.3]
check_graphemes(U"\u0378\u0308\U0001F1E6", {U"\u0378\u0308", U"\U0001F1E6"});
//÷ 0378 ÷ 06DD ÷
//÷ [0.2] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [999.0] ARABIC END OF AYAH (Prepend) ÷ [0.3]
check_graphemes(U"\u0378\u06DD", {U"\u0378", U"\u06DD"});
//÷ 0378 × 0308 ÷ 06DD ÷
//÷ [0.2] <reserved-0378> (XXmLinkingConsonantmExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] ARABIC END OF AYAH (Prepend) ÷ [0.3]
check_graphemes(U"\u0378\u0308\u06DD", {U"\u0378\u0308", U"\u06DD"});
//÷ 0378 × 0903 ÷
//÷ [0.2] <reserved-0378> (XXmLinkingConsonantmExtPict) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u0378\u0903", {U"\u0378\u0903"});
//÷ 0378 × 0308 × 0903 ÷
//÷ [0.2] <reserved-0378> (XXmLinkingConsonantmExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [0.3]
check_graphemes(U"\u0378\u0308\u0903", {U"\u0378\u0308\u0903"});
//÷ 0378 ÷ 1100 ÷
//÷ [0.2] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u0378\u1100", {U"\u0378", U"\u1100"});
//÷ 0378 × 0308 ÷ 1100 ÷
//÷ [0.2] <reserved-0378> (XXmLinkingConsonantmExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL CHOSEONG KIYEOK (L) ÷ [0.3]
check_graphemes(U"\u0378\u0308\u1100", {U"\u0378\u0308", U"\u1100"});
//÷ 0378 ÷ 1160 ÷
//÷ [0.2] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u0378\u1160", {U"\u0378", U"\u1160"});
//÷ 0378 × 0308 ÷ 1160 ÷
//÷ [0.2] <reserved-0378> (XXmLinkingConsonantmExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL JUNGSEONG FILLER (V) ÷ [0.3]
check_graphemes(U"\u0378\u0308\u1160", {U"\u0378\u0308", U"\u1160"});
//÷ 0378 ÷ 11A8 ÷
//÷ [0.2] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u0378\u11A8", {U"\u0378", U"\u11A8"});
//÷ 0378 × 0308 ÷ 11A8 ÷
//÷ [0.2] <reserved-0378> (XXmLinkingConsonantmExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL JONGSEONG KIYEOK (T) ÷ [0.3]
check_graphemes(U"\u0378\u0308\u11A8", {U"\u0378\u0308", U"\u11A8"});
//÷ 0378 ÷ AC00 ÷
//÷ [0.2] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u0378\uAC00", {U"\u0378", U"\uAC00"});
//÷ 0378 × 0308 ÷ AC00 ÷
//÷ [0.2] <reserved-0378> (XXmLinkingConsonantmExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL SYLLABLE GA (LV) ÷ [0.3]
check_graphemes(U"\u0378\u0308\uAC00", {U"\u0378\u0308", U"\uAC00"});
//÷ 0378 ÷ AC01 ÷
//÷ [0.2] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u0378\uAC01", {U"\u0378", U"\uAC01"});
//÷ 0378 × 0308 ÷ AC01 ÷
//÷ [0.2] <reserved-0378> (XXmLinkingConsonantmExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] HANGUL SYLLABLE GAG (LVT) ÷ [0.3]
check_graphemes(U"\u0378\u0308\uAC01", {U"\u0378\u0308", U"\uAC01"});
//÷ 0378 ÷ 0915 ÷
//÷ [0.2] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [999.0] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u0378\u0915", {U"\u0378", U"\u0915"});
//÷ 0378 × 0308 ÷ 0915 ÷
//÷ [0.2] <reserved-0378> (XXmLinkingConsonantmExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u0378\u0308\u0915", {U"\u0378\u0308", U"\u0915"});
//÷ 0378 ÷ 00A9 ÷
//÷ [0.2] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [999.0] COPYRIGHT SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\u0378\u00A9", {U"\u0378", U"\u00A9"});
//÷ 0378 × 0308 ÷ 00A9 ÷
//÷ [0.2] <reserved-0378> (XXmLinkingConsonantmExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] COPYRIGHT SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\u0378\u0308\u00A9", {U"\u0378\u0308", U"\u00A9"});
//÷ 0378 ÷ 0020 ÷
//÷ [0.2] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [999.0] SPACE (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u0378\u0020", {U"\u0378", U"\u0020"});
//÷ 0378 × 0308 ÷ 0020 ÷
//÷ [0.2] <reserved-0378> (XXmLinkingConsonantmExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] SPACE (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u0378\u0308\u0020", {U"\u0378\u0308", U"\u0020"});
//÷ 0378 ÷ 0378 ÷
//÷ [0.2] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [999.0] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u0378\u0378", {U"\u0378", U"\u0378"});
//÷ 0378 × 0308 ÷ 0378 ÷
//÷ [0.2] <reserved-0378> (XXmLinkingConsonantmExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] <reserved-0378> (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u0378\u0308\u0378", {U"\u0378\u0308", U"\u0378"});
//÷ 000D × 000A ÷ 0061 ÷ 000A ÷ 0308 ÷
//÷ [0.2] <CARRIAGE RETURN (CR)> (CR) × [3.0] <LINE FEED (LF)> (LF) ÷ [4.0] LATIN SMALL LETTER A (XXmLinkingConsonantmExtPict) ÷ [5.0] <LINE FEED (LF)> (LF) ÷ [4.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\u000D\u000A\u0061\u000A\u0308", {U"\u000D\u000A", U"\u0061", U"\u000A", U"\u0308"});
//÷ 0061 × 0308 ÷
//÷ [0.2] LATIN SMALL LETTER A (XXmLinkingConsonantmExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\u0061\u0308", {U"\u0061\u0308"});
//÷ 0020 × 200D ÷ 0646 ÷
//÷ [0.2] SPACE (XXmLinkingConsonantmExtPict) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [999.0] ARABIC LETTER NOON (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u0020\u200D\u0646", {U"\u0020\u200D", U"\u0646"});
//÷ 0646 × 200D ÷ 0020 ÷
//÷ [0.2] ARABIC LETTER NOON (XXmLinkingConsonantmExtPict) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [999.0] SPACE (XXmLinkingConsonantmExtPict) ÷ [0.3]
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
//÷ [0.2] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [12.0] REGIONAL INDICATOR SYMBOL LETTER B (RI) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER C (RI) ÷ [999.0] LATIN SMALL LETTER B (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\U0001F1E6\U0001F1E7\U0001F1E8\u0062", {U"\U0001F1E6\U0001F1E7", U"\U0001F1E8", U"\u0062"});
//÷ 0061 ÷ 1F1E6 × 1F1E7 ÷ 1F1E8 ÷ 0062 ÷
//÷ [0.2] LATIN SMALL LETTER A (XXmLinkingConsonantmExtPict) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [13.0] REGIONAL INDICATOR SYMBOL LETTER B (RI) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER C (RI) ÷ [999.0] LATIN SMALL LETTER B (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u0061\U0001F1E6\U0001F1E7\U0001F1E8\u0062", {U"\u0061", U"\U0001F1E6\U0001F1E7", U"\U0001F1E8", U"\u0062"});
//÷ 0061 ÷ 1F1E6 × 1F1E7 × 200D ÷ 1F1E8 ÷ 0062 ÷
//÷ [0.2] LATIN SMALL LETTER A (XXmLinkingConsonantmExtPict) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [13.0] REGIONAL INDICATOR SYMBOL LETTER B (RI) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER C (RI) ÷ [999.0] LATIN SMALL LETTER B (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u0061\U0001F1E6\U0001F1E7\u200D\U0001F1E8\u0062", {U"\u0061", U"\U0001F1E6\U0001F1E7\u200D", U"\U0001F1E8", U"\u0062"});
//÷ 0061 ÷ 1F1E6 × 200D ÷ 1F1E7 × 1F1E8 ÷ 0062 ÷
//÷ [0.2] LATIN SMALL LETTER A (XXmLinkingConsonantmExtPict) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER B (RI) × [13.0] REGIONAL INDICATOR SYMBOL LETTER C (RI) ÷ [999.0] LATIN SMALL LETTER B (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u0061\U0001F1E6\u200D\U0001F1E7\U0001F1E8\u0062", {U"\u0061", U"\U0001F1E6\u200D", U"\U0001F1E7\U0001F1E8", U"\u0062"});
//÷ 0061 ÷ 1F1E6 × 1F1E7 ÷ 1F1E8 × 1F1E9 ÷ 0062 ÷
//÷ [0.2] LATIN SMALL LETTER A (XXmLinkingConsonantmExtPict) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER A (RI) × [13.0] REGIONAL INDICATOR SYMBOL LETTER B (RI) ÷ [999.0] REGIONAL INDICATOR SYMBOL LETTER C (RI) × [13.0] REGIONAL INDICATOR SYMBOL LETTER D (RI) ÷ [999.0] LATIN SMALL LETTER B (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u0061\U0001F1E6\U0001F1E7\U0001F1E8\U0001F1E9\u0062", {U"\u0061", U"\U0001F1E6\U0001F1E7", U"\U0001F1E8\U0001F1E9", U"\u0062"});
//÷ 0061 × 200D ÷
//÷ [0.2] LATIN SMALL LETTER A (XXmLinkingConsonantmExtPict) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [0.3]
check_graphemes(U"\u0061\u200D", {U"\u0061\u200D"});
//÷ 0061 × 0308 ÷ 0062 ÷
//÷ [0.2] LATIN SMALL LETTER A (XXmLinkingConsonantmExtPict) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] LATIN SMALL LETTER B (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u0061\u0308\u0062", {U"\u0061\u0308", U"\u0062"});
//÷ 0061 × 0903 ÷ 0062 ÷
//÷ [0.2] LATIN SMALL LETTER A (XXmLinkingConsonantmExtPict) × [9.1] DEVANAGARI SIGN VISARGA (SpacingMark) ÷ [999.0] LATIN SMALL LETTER B (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u0061\u0903\u0062", {U"\u0061\u0903", U"\u0062"});
//÷ 0061 ÷ 0600 × 0062 ÷
//÷ [0.2] LATIN SMALL LETTER A (XXmLinkingConsonantmExtPict) ÷ [999.0] ARABIC NUMBER SIGN (Prepend) × [9.2] LATIN SMALL LETTER B (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u0061\u0600\u0062", {U"\u0061", U"\u0600\u0062"});
//÷ 1F476 × 1F3FF ÷ 1F476 ÷
//÷ [0.2] BABY (ExtPict) × [9.0] EMOJI MODIFIER FITZPATRICK TYPE-6 (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] BABY (ExtPict) ÷ [0.3]
check_graphemes(U"\U0001F476\U0001F3FF\U0001F476", {U"\U0001F476\U0001F3FF", U"\U0001F476"});
//÷ 0061 × 1F3FF ÷ 1F476 ÷
//÷ [0.2] LATIN SMALL LETTER A (XXmLinkingConsonantmExtPict) × [9.0] EMOJI MODIFIER FITZPATRICK TYPE-6 (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] BABY (ExtPict) ÷ [0.3]
check_graphemes(U"\u0061\U0001F3FF\U0001F476", {U"\u0061\U0001F3FF", U"\U0001F476"});
//÷ 0061 × 1F3FF ÷ 1F476 × 200D × 1F6D1 ÷
//÷ [0.2] LATIN SMALL LETTER A (XXmLinkingConsonantmExtPict) × [9.0] EMOJI MODIFIER FITZPATRICK TYPE-6 (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] BABY (ExtPict) × [9.0] ZERO WIDTH JOINER (ZWJ) × [11.0] OCTAGONAL SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\u0061\U0001F3FF\U0001F476\u200D\U0001F6D1", {U"\u0061\U0001F3FF", U"\U0001F476\u200D\U0001F6D1"});
//÷ 1F476 × 1F3FF × 0308 × 200D × 1F476 × 1F3FF ÷
//÷ [0.2] BABY (ExtPict) × [9.0] EMOJI MODIFIER FITZPATRICK TYPE-6 (Extend_ConjunctExtendermConjunctLinker) × [9.0] COMBINING DIAERESIS (Extend_ConjunctExtendermConjunctLinker) × [9.0] ZERO WIDTH JOINER (ZWJ) × [11.0] BABY (ExtPict) × [9.0] EMOJI MODIFIER FITZPATRICK TYPE-6 (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\U0001F476\U0001F3FF\u0308\u200D\U0001F476\U0001F3FF", {U"\U0001F476\U0001F3FF\u0308\u200D\U0001F476\U0001F3FF"});
//÷ 1F6D1 × 200D × 1F6D1 ÷
//÷ [0.2] OCTAGONAL SIGN (ExtPict) × [9.0] ZERO WIDTH JOINER (ZWJ) × [11.0] OCTAGONAL SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\U0001F6D1\u200D\U0001F6D1", {U"\U0001F6D1\u200D\U0001F6D1"});
//÷ 0061 × 200D ÷ 1F6D1 ÷
//÷ [0.2] LATIN SMALL LETTER A (XXmLinkingConsonantmExtPict) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [999.0] OCTAGONAL SIGN (ExtPict) ÷ [0.3]
check_graphemes(U"\u0061\u200D\U0001F6D1", {U"\u0061\u200D", U"\U0001F6D1"});
//÷ 2701 × 200D ÷ 2701 ÷
//÷ [0.2] UPPER BLADE SCISSORS (XXmLinkingConsonantmExtPict) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [999.0] UPPER BLADE SCISSORS (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u2701\u200D\u2701", {U"\u2701\u200D", U"\u2701"});
//÷ 0061 × 200D ÷ 2701 ÷
//÷ [0.2] LATIN SMALL LETTER A (XXmLinkingConsonantmExtPict) × [9.0] ZERO WIDTH JOINER (ZWJ) ÷ [999.0] UPPER BLADE SCISSORS (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u0061\u200D\u2701", {U"\u0061\u200D", U"\u2701"});
//÷ 0915 ÷ 0924 ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) ÷ [999.0] DEVANAGARI LETTER TA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u0915\u0924", {U"\u0915", U"\u0924"});
//÷ 0915 × 094D × 0924 ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) × [9.3] DEVANAGARI LETTER TA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u0915\u094D\u0924", {U"\u0915\u094D\u0924"});
//÷ 0915 × 094D × 094D × 0924 ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) × [9.3] DEVANAGARI LETTER TA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u0915\u094D\u094D\u0924", {U"\u0915\u094D\u094D\u0924"});
//÷ 0915 × 094D × 200D × 0924 ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) × [9.0] ZERO WIDTH JOINER (ZWJ) × [9.3] DEVANAGARI LETTER TA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u0915\u094D\u200D\u0924", {U"\u0915\u094D\u200D\u0924"});
//÷ 0915 × 093C × 200D × 094D × 0924 ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) × [9.0] DEVANAGARI SIGN NUKTA (Extend_ConjunctExtendermConjunctLinker) × [9.0] ZERO WIDTH JOINER (ZWJ) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) × [9.3] DEVANAGARI LETTER TA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u0915\u093C\u200D\u094D\u0924", {U"\u0915\u093C\u200D\u094D\u0924"});
//÷ 0915 × 093C × 094D × 200D × 0924 ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) × [9.0] DEVANAGARI SIGN NUKTA (Extend_ConjunctExtendermConjunctLinker) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) × [9.0] ZERO WIDTH JOINER (ZWJ) × [9.3] DEVANAGARI LETTER TA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u0915\u093C\u094D\u200D\u0924", {U"\u0915\u093C\u094D\u200D\u0924"});
//÷ 0915 × 094D × 0924 × 094D × 092F ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) × [9.3] DEVANAGARI LETTER TA (LinkingConsonant) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) × [9.3] DEVANAGARI LETTER YA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u0915\u094D\u0924\u094D\u092F", {U"\u0915\u094D\u0924\u094D\u092F"});
//÷ 0915 × 094D ÷ 0061 ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [999.0] LATIN SMALL LETTER A (XXmLinkingConsonantmExtPict) ÷ [0.3]
check_graphemes(U"\u0915\u094D\u0061", {U"\u0915\u094D", U"\u0061"});
//÷ 0061 × 094D ÷ 0924 ÷
//÷ [0.2] LATIN SMALL LETTER A (XXmLinkingConsonantmExtPict) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [999.0] DEVANAGARI LETTER TA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u0061\u094D\u0924", {U"\u0061\u094D", U"\u0924"});
//÷ 003F × 094D ÷ 0924 ÷
//÷ [0.2] QUESTION MARK (XXmLinkingConsonantmExtPict) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) ÷ [999.0] DEVANAGARI LETTER TA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u003F\u094D\u0924", {U"\u003F\u094D", U"\u0924"});
//÷ 0915 × 094D × 094D × 0924 ÷
//÷ [0.2] DEVANAGARI LETTER KA (LinkingConsonant) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) × [9.0] DEVANAGARI SIGN VIRAMA (Extend_ConjunctLinker) × [9.3] DEVANAGARI LETTER TA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u0915\u094D\u094D\u0924", {U"\u0915\u094D\u094D\u0924"});
//÷ 0AB8 × 0AFB × 0ACD × 0AB8 × 0AFB ÷
//÷ [0.2] GUJARATI LETTER SA (LinkingConsonant) × [9.0] GUJARATI SIGN SHADDA (Extend_ConjunctExtendermConjunctLinker) × [9.0] GUJARATI SIGN VIRAMA (Extend_ConjunctLinker) × [9.3] GUJARATI LETTER SA (LinkingConsonant) × [9.0] GUJARATI SIGN SHADDA (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\u0AB8\u0AFB\u0ACD\u0AB8\u0AFB", {U"\u0AB8\u0AFB\u0ACD\u0AB8\u0AFB"});
//÷ 1019 × 1039 × 1018 ÷ 102C × 1037 ÷
//÷ [0.2] MYANMAR LETTER MA (LinkingConsonant) × [9.0] MYANMAR SIGN VIRAMA (Extend_ConjunctLinker) × [9.3] MYANMAR LETTER BHA (LinkingConsonant) ÷ [999.0] MYANMAR VOWEL SIGN AA (XXmLinkingConsonantmExtPict) × [9.0] MYANMAR SIGN DOT BELOW (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\u1019\u1039\u1018\u102C\u1037", {U"\u1019\u1039\u1018", U"\u102C\u1037"});
//÷ 1004 × 103A × 1039 × 1011 × 1039 × 1011 ÷
//÷ [0.2] MYANMAR LETTER NGA (LinkingConsonant) × [9.0] MYANMAR SIGN ASAT (Extend_ConjunctExtendermConjunctLinker) × [9.0] MYANMAR SIGN VIRAMA (Extend_ConjunctLinker) × [9.3] MYANMAR LETTER THA (LinkingConsonant) × [9.0] MYANMAR SIGN VIRAMA (Extend_ConjunctLinker) × [9.3] MYANMAR LETTER THA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u1004\u103A\u1039\u1011\u1039\u1011", {U"\u1004\u103A\u1039\u1011\u1039\u1011"});
//÷ 1B12 × 1B01 ÷ 1B32 × 1B44 × 1B2F ÷ 1B32 × 1B44 × 1B22 × 1B44 × 1B2C ÷ 1B32 × 1B44 × 1B22 × 1B38 ÷
//÷ [0.2] BALINESE LETTER OKARA TEDUNG (XXmLinkingConsonantmExtPict) × [9.0] BALINESE SIGN ULU CANDRA (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] BALINESE LETTER SA (LinkingConsonant) × [9.0] BALINESE ADEG ADEG (Extend_ConjunctLinker) × [9.3] BALINESE LETTER WA (LinkingConsonant) ÷ [999.0] BALINESE LETTER SA (LinkingConsonant) × [9.0] BALINESE ADEG ADEG (Extend_ConjunctLinker) × [9.3] BALINESE LETTER TA (LinkingConsonant) × [9.0] BALINESE ADEG ADEG (Extend_ConjunctLinker) × [9.3] BALINESE LETTER YA (LinkingConsonant) ÷ [999.0] BALINESE LETTER SA (LinkingConsonant) × [9.0] BALINESE ADEG ADEG (Extend_ConjunctLinker) × [9.3] BALINESE LETTER TA (LinkingConsonant) × [9.0] BALINESE VOWEL SIGN SUKU (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\u1B12\u1B01\u1B32\u1B44\u1B2F\u1B32\u1B44\u1B22\u1B44\u1B2C\u1B32\u1B44\u1B22\u1B38", {U"\u1B12\u1B01", U"\u1B32\u1B44\u1B2F", U"\u1B32\u1B44\u1B22\u1B44\u1B2C", U"\u1B32\u1B44\u1B22\u1B38"});
//÷ 179F × 17D2 × 178F × 17D2 × 179A × 17B8 ÷
//÷ [0.2] KHMER LETTER SA (LinkingConsonant) × [9.0] KHMER SIGN COENG (Extend_ConjunctLinker) × [9.3] KHMER LETTER TA (LinkingConsonant) × [9.0] KHMER SIGN COENG (Extend_ConjunctLinker) × [9.3] KHMER LETTER RO (LinkingConsonant) × [9.0] KHMER VOWEL SIGN II (Extend_ConjunctExtendermConjunctLinker) ÷ [0.3]
check_graphemes(U"\u179F\u17D2\u178F\u17D2\u179A\u17B8", {U"\u179F\u17D2\u178F\u17D2\u179A\u17B8"});
//÷ 1B26 ÷ 1B17 × 1B44 × 1B13 ÷
//÷ [0.2] BALINESE LETTER NA (LinkingConsonant) ÷ [999.0] BALINESE LETTER NGA (LinkingConsonant) × [9.0] BALINESE ADEG ADEG (Extend_ConjunctLinker) × [9.3] BALINESE LETTER KA (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u1B26\u1B17\u1B44\u1B13", {U"\u1B26", U"\u1B17\u1B44\u1B13"});
//÷ 1B27 ÷ 1B13 × 1B44 × 1B0B ÷ 1B0B × 1B04 ÷
//÷ [0.2] BALINESE LETTER PA (LinkingConsonant) ÷ [999.0] BALINESE LETTER KA (LinkingConsonant) × [9.0] BALINESE ADEG ADEG (Extend_ConjunctLinker) × [9.3] BALINESE LETTER RA REPA (LinkingConsonant) ÷ [999.0] BALINESE LETTER RA REPA (LinkingConsonant) × [9.1] BALINESE SIGN BISAH (SpacingMark) ÷ [0.3]
check_graphemes(U"\u1B27\u1B13\u1B44\u1B0B\u1B0B\u1B04", {U"\u1B27", U"\u1B13\u1B44\u1B0B", U"\u1B0B\u1B04"});
//÷ 1795 × 17D2 × 17AF ÷ 1798 ÷
//÷ [0.2] KHMER LETTER PHA (LinkingConsonant) × [9.0] KHMER SIGN COENG (Extend_ConjunctLinker) × [9.3] KHMER INDEPENDENT VOWEL QE (LinkingConsonant) ÷ [999.0] KHMER LETTER MO (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u1795\u17D2\u17AF\u1798", {U"\u1795\u17D2\u17AF", U"\u1798"});
//÷ 17A0 × 17D2 × 17AB ÷ 1791 × 17D0 ÷ 1799 ÷
//÷ [0.2] KHMER LETTER HA (LinkingConsonant) × [9.0] KHMER SIGN COENG (Extend_ConjunctLinker) × [9.3] KHMER INDEPENDENT VOWEL RY (LinkingConsonant) ÷ [999.0] KHMER LETTER TO (LinkingConsonant) × [9.0] KHMER SIGN SAMYOK SANNYA (Extend_ConjunctExtendermConjunctLinker) ÷ [999.0] KHMER LETTER YO (LinkingConsonant) ÷ [0.3]
check_graphemes(U"\u17A0\u17D2\u17AB\u1791\u17D0\u1799", {U"\u17A0\u17D2\u17AB", U"\u1791\u17D0", U"\u1799"});


