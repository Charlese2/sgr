#include "dolphin/types.h"

inline u32 CrankyAlign(u32 input, u32 stride) {
    return input & ~(stride - 1);
}

inline u32 CrankyRoundUp(u32 ptr, u32 stride) {
    return (stride + ptr) - 1 & ~(stride -1);
}

inline u32 CrankyRoundUp32(u32 input) {
    return CrankyRoundUp(input, 32);
}

inline u32 CrankyAlign32(u32 input) {
    return CrankyAlign(input, 32);
}
