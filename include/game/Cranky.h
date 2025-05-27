#include "dolphin/types.h"

inline BOOL CrankyTestAlign(u32 input, u32 stride) { return (input & stride - 1U) == 0; }

inline u32 CrankyAlign(u32 input, u32 stride) { return input & ~(stride - 1); }

inline u32 CrankyRoundUp(u32 input, u32 stride) { return input + (stride - 1) & ~(stride - 1); }

inline u32 CrankyRoundUp32(u32 input) { return CrankyRoundUp(input, 32); }

inline u32 CrankyAlign32(u32 input) { return CrankyAlign(input, 32); }

inline BOOL CrankyTestAlign4(u32 input) { return CrankyTestAlign(input, 4); }

inline BOOL CrankyTestAlign32(u32 input) { return CrankyTestAlign(input, 32); }
