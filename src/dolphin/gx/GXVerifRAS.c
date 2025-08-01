// TODO: match

#if DEBUG

#include <stdio.h>

#include <dolphin/gx.h>

#include "__gx.h"

void __GXVerifySU(void)
{
    if (__gxVerif->verifyLevel >= 1 && (u32)GET_REG_FIELD(__gxVerif->rasRegs[32], 11, 12) < 340.0f) {
        sprintf(__gxvDummyStr, __gxvWarnings[2], (u32)GET_REG_FIELD(__gxVerif->rasRegs[32], 11, 12) - 340.0f);
        __gxVerif->cb(1, 2U, __gxvDummyStr);
    }

    if (__gxVerif->verifyLevel >= 1 && (u32)GET_REG_FIELD(__gxVerif->rasRegs[32], 11, 0) < 340.0f) {
        sprintf(__gxvDummyStr, __gxvWarnings[3], (u32)GET_REG_FIELD(__gxVerif->rasRegs[32], 11, 12 /* bug? */) - 340.0f);
        __gxVerif->cb(1, 3U, __gxvDummyStr);
    }

    switch (__gxVerif->rasRegs[67] & 7) {
    case 4:
    case 5:
        if (__gxVerif->verifyLevel >= 1) {
            if ((u32)GET_REG_FIELD(__gxVerif->rasRegs[33], 11, 12) > 1059) {
                sprintf(__gxvDummyStr, __gxvWarnings[4], 1059, "YUV");
                __gxVerif->cb(1, 4U, __gxvDummyStr);
            }
            if ((u32)GET_REG_FIELD(__gxVerif->rasRegs[33], 11, 0) > 915) {
                sprintf(__gxvDummyStr, __gxvWarnings[5], 915, "YUV");
                __gxVerif->cb(1, 5U, __gxvDummyStr);
            }
        }
        break;
    case 0:
    case 1:
    case 3:
        if (__gxVerif->verifyLevel >= 1) {
            if ((u32)GET_REG_FIELD(__gxVerif->rasRegs[33], 11, 12) > 979) {
                sprintf(__gxvDummyStr, __gxvWarnings[4], 979, "RGB");
                __gxVerif->cb(1, 4U, __gxvDummyStr);
            }
            if ((u32)GET_REG_FIELD(__gxVerif->rasRegs[33], 11, 0) > 867) {
                sprintf(__gxvDummyStr, __gxvWarnings[5], 867, "RGB");
                __gxVerif->cb(1, 5U, __gxvDummyStr);
            }
        }
        break;
    case 2:
        if (__gxVerif->verifyLevel >= 1) {
            if ((u32)GET_REG_FIELD(__gxVerif->rasRegs[33], 11, 12) > 979) {
                sprintf(__gxvDummyStr, __gxvWarnings[4], 979, "RGB multisample");
                __gxVerif->cb(1, 4U, __gxvDummyStr);
            }
            if ((u32)GET_REG_FIELD(__gxVerif->rasRegs[33], 11, 0) > 603) {
                sprintf(__gxvDummyStr, __gxvWarnings[5], 603, "RGB multisample");
                __gxVerif->cb(1, 5U, __gxvDummyStr);
            }
        }
        break;
    }
}

void __GXVerifyBUMP(void)
{
    u32 i;
    u32 nBmp;
    u32 nTev;
    u32 nTex;
    u32 matrix;

    nBmp = GET_REG_FIELD(__gxVerif->rasRegs[0], 3, 16);
    nTex = GET_REG_FIELD(__gxVerif->rasRegs[0], 4, 0);
    nTev = GET_REG_FIELD(__gxVerif->rasRegs[0], 4, 10) + 1;

    for (i = 0; i < nTev; i++) {
        matrix = GET_REG_FIELD(__gxVerif->rasRegs[16 + i], 4, 9);
        if (__gxVerif->verifyLevel >= 1) {
            if ((u32)(__gxVerif->rasRegs[16 + i] & 0xFF000000) + 0x01000000 == 0U) {
                sprintf(__gxvDummyStr, __gxvWarnings[7], i);
                __gxVerif->cb(1, 7U, __gxvDummyStr);
            }
            if ((GET_REG_FIELD(__gxVerif->rasRegs[16 + i], 2, 7) != 0 || matrix != 0)
             && GET_REG_FIELD(__gxVerif->rasRegs[16 + i], 2, 0) >= nBmp) {
                sprintf(__gxvDummyStr, __gxvWarnings[8], i);
                __gxVerif->cb(1, 8U, __gxvDummyStr);
            }
            if (matrix != 0) {
                matrix = (matrix & 3) - 1;
                if ((u32)(__gxVerif->rasRegs[(matrix * 3) + 6] & 0xFF000000) + 0x01000000 == 0
                 || (u32)(__gxVerif->rasRegs[(matrix * 3) + 7] & 0xFF000000) + 0x01000000 == 0U
                 || (u32)(__gxVerif->rasRegs[(matrix * 3) + 8] & 0xFF000000) + 0x01000000 == 0U) {
                    sprintf(__gxvDummyStr, __gxvWarnings[9], matrix, i);
                    __gxVerif->cb(1, 9U, __gxvDummyStr);
                }
            }
        }
    }
    if (__gxVerif->verifyLevel >= 1) {
        if (nBmp != 0 && (u32)(__gxVerif->rasRegs[0x27] & 0xFF000000) + 0x01000000 == 0) {
            __gxVerif->cb(1, 0xAU, __gxvWarnings[10]);
        }
        if (nBmp != 0 && (u32)(__gxVerif->rasRegs[0x25] & 0xFF000000) + 0x01000000 == 0) {
            sprintf(__gxvDummyStr, __gxvWarnings[11], 0U, 1);
            __gxVerif->cb(1, 0xBU, __gxvDummyStr);
        }
        if (nBmp > 2U && (u32)(__gxVerif->rasRegs[0x26] & 0xFF000000) + 0x01000000 == 0) {
            sprintf(__gxvDummyStr, __gxvWarnings[11], 2U, 3);
            __gxVerif->cb(1, 0xBU, __gxvDummyStr);
        }
        if (nBmp != 0 && GET_REG_FIELD(__gxVerif->rasRegs[0x27], 3, 3) >= nTex) {
            sprintf(__gxvDummyStr, __gxvWarnings[12], 0U);
            __gxVerif->cb(1, 0xCU, __gxvDummyStr);
        }
        if (nBmp > 1U && GET_REG_FIELD(__gxVerif->rasRegs[0x27], 3, 9) >= nTex) {
            sprintf(__gxvDummyStr, __gxvWarnings[12], 1U);
            __gxVerif->cb(1, 0xCU, __gxvDummyStr);
        }
        if (nBmp > 2U && GET_REG_FIELD(__gxVerif->rasRegs[0x27], 3, 15) >= nTex) {
            sprintf(__gxvDummyStr, __gxvWarnings[12], 2U);
            __gxVerif->cb(1, 0xCU, __gxvDummyStr);
        }
        if (nBmp > 3U && GET_REG_FIELD(__gxVerif->rasRegs[0x27], 3, 21) >= nTex) {
            sprintf(__gxvDummyStr, __gxvWarnings[12], 3U);
            __gxVerif->cb(1, 0xCU, __gxvDummyStr);
        }
        if (nBmp != 0 && GET_REG_FIELD(__gxVerif->rasRegs[0x10], 1, 20)) {
            __gxVerif->cb(1, 0xDU, __gxvWarnings[13]);
        }
        if (nBmp != 0 && GET_REG_FIELD(__gxVerif->rasRegs[0x10], 2, 7) != 0) {
            __gxVerif->cb(1, 0xEU, __gxvWarnings[14]);
        }
        if ((u32)(__gxVerif->rasRegs[0xF] & 0xFF000000) + 0x01000000 == 0 && (nTex != 0 || nBmp != 0)) {
            __gxVerif->cb(1, 0xFU, __gxvWarnings[15]);
        }
    }
}

#define SOMEINDEX(index) (index & 3) + ((index * 8) & ~0x1F)

#define MAX(a, b) ((a) > (b) ? (a) : (b))

void __GXVerifyTEX(void)
{
    u32 i;
    u32 nBmp;
    u32 nTev;
    u32 nTex;
    u32 enabled;
    u32 texId;
    u32 direct[8];
    u32 indirect[8];
    u32 h2;
    u32 w2;
    u32 nlevels;

    nBmp = GET_REG_FIELD(__gxVerif->rasRegs[0], 3, 16);
    nTex = GET_REG_FIELD(__gxVerif->rasRegs[0], 4, 0);
    nTev = GET_REG_FIELD(__gxVerif->rasRegs[0], 4, 10) + 1;

    for (i = 0; i < 8; i++) {
        direct[i] = 0;
        indirect[i] = 0;
    }

    for (i = 0; i < nTev + nBmp; i++) {
        if (i < nTev) {
            if (__gxVerif->verifyLevel >= 1) {
                if ((__gxVerif->rasRegs[(i >> 1U) + 0x28] & 0xFF000000) + 0x01000000 == 0U) {
                    sprintf(__gxvDummyStr, __gxvWarnings[16], i);
                    __gxVerif->cb(1, 16, __gxvDummyStr);
                }
                if (i & 1) {
                    enabled = GET_REG_FIELD(__gxVerif->rasRegs[(i >> 1U) + 0x28], 1, 18);
                    if (enabled && (GET_REG_FIELD(__gxVerif->rasRegs[(i >> 1U) + 0x28], 3, 15) >= nTex)) {
                        sprintf(__gxvDummyStr, __gxvWarnings[17], i);
                        __gxVerif->cb(1, 17, __gxvDummyStr);
                    }
                    texId = GET_REG_FIELD(__gxVerif->rasRegs[(i >> 1U) + 0x28], 3, 12);
                } else {
                    enabled = GET_REG_FIELD(__gxVerif->rasRegs[(i >> 1U) + 0x28], 1, 6);
                    if (enabled && (GET_REG_FIELD(__gxVerif->rasRegs[(i >> 1U) + 0x28], 3, 3) >= nTex)) {
                        sprintf(__gxvDummyStr, __gxvWarnings[17], i);
                        __gxVerif->cb(1, 17, __gxvDummyStr);
                    }
                    texId = GET_REG_FIELD(__gxVerif->rasRegs[(i >> 1U) + 0x28], 3, 0);
                }
                if (enabled && indirect[texId]) {
                    sprintf(__gxvDummyStr, __gxvWarnings[18], i);
                    __gxVerif->cb(1, 18, __gxvDummyStr);
                }
                if (enabled) {
                    direct[texId] = 1;
                }
            }
        } else {
            enabled = 1;
            if ((i - nTev) == 0) {
                texId = GET_REG_FIELD(__gxVerif->rasRegs[0x27], 3, 0);
            } else if ((i - nTev) == 1U) {
                texId = GET_REG_FIELD(__gxVerif->rasRegs[0x27], 3, 6);
            } else if ((i - nTev) == 2U) {
                texId = GET_REG_FIELD(__gxVerif->rasRegs[0x27], 3, 12);
            } else {
                texId = GET_REG_FIELD(__gxVerif->rasRegs[0x27], 3, 18);
            }
            if (__gxVerif->verifyLevel >= 1 && direct[texId]) {
                sprintf(__gxvDummyStr, __gxvWarnings[18], i);
                __gxVerif->cb(1, 18, __gxvDummyStr);
            }
            indirect[texId] = 1;
        }
        if (enabled) {
            if (__gxVerif->verifyLevel >= 1) {
                if ((u32)(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)] & 0xFF000000) + 0x01000000 == 0
                 || (u32)(__gxVerif->rasRegs[0x84 + SOMEINDEX(texId)] & 0xFF000000) + 0x01000000 == 0
                 || (u32)(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)] & 0xFF000000) + 0x01000000 == 0
                 || (u32)(__gxVerif->rasRegs[0x8C + SOMEINDEX(texId)] & 0xFF000000) + 0x01000000 == 0
                 || (u32)(__gxVerif->rasRegs[0x90 + SOMEINDEX(texId)] & 0xFF000000) + 0x01000000 == 0) {
                    sprintf(__gxvDummyStr, __gxvWarnings[19], texId);
                    __gxVerif->cb(1, 19, __gxvDummyStr);
                }
                if (GET_REG_FIELD(__gxVerif->rasRegs[0x8C + SOMEINDEX(texId)], 1, 21) == 0
                 && (u32)(__gxVerif->rasRegs[0x94 + SOMEINDEX(texId)] & 0xFF000000) + 0x01000000 == 0) {
                    sprintf(__gxvDummyStr, __gxvWarnings[20], texId);
                    __gxVerif->cb(1, 20, __gxvDummyStr);
                }
                if (((u32)GET_REG_FIELD(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)], 4, 20) == 8
                  || (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)], 4, 20) == 9
                  || (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)], 4, 20) == 10)
                 && (__gxVerif->rasRegs[0x98 + SOMEINDEX(texId)] & 0xFF000000) + 0x01000000 == 0U) {
                    sprintf(__gxvDummyStr, __gxvWarnings[21], texId);
                    __gxVerif->cb(1, 21, __gxvDummyStr);
                }

                if (GET_REG_FIELD(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)], 10, 0) + 1 == 0) {
                    w2 = 1;
                } else {
                    w2 = 1;
                    while (!(w2 & (GET_REG_FIELD(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)], 10, 0) + 1))) {
                        w2 *= 2;
                    }
                    w2 = (GET_REG_FIELD(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)], 10, 0) + 1) == w2;
                }

                if (GET_REG_FIELD(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)], 10, 10) + 1 == 0) {
                    h2 = 1;
                } else {
                    h2 = 1;
                    while (!(h2 & (GET_REG_FIELD(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)], 10, 10) + 1))) {
                        h2 *= 2;
                    }
                    h2 = (GET_REG_FIELD(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)], 10, 10) + 1) == h2;
                }
                if (GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)] & 0xFFFF, 2, 5) && !w2) {
                    sprintf(__gxvDummyStr, __gxvWarnings[22], "Width", texId);
                    __gxVerif->cb(1, 22, __gxvDummyStr);
                }
                if (GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)] & 0xFFFF, 2, 5) && !h2) {
                    sprintf(__gxvDummyStr, __gxvWarnings[22], "Height", texId);
                    __gxVerif->cb(1, 22, __gxvDummyStr);
                }
                if (GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)], 2, 0) && !w2) {
                    sprintf(__gxvDummyStr, __gxvWarnings[23], "S", texId);
                    __gxVerif->cb(1, 23, __gxvDummyStr);
                }
                if (GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)], 2, 2) && !h2) {
                    sprintf(__gxvDummyStr, __gxvWarnings[23], "T", texId);
                    __gxVerif->cb(1, 23, __gxvDummyStr);
                }
                if (GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)] & 0xFFFF, 2, 5) != 0
                 && ((u32)GET_REG_FIELD(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)], 4, 20) == 8
                  || (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)], 4, 20) == 9
                  || (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)], 4, 20) == 10)
                 && (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)], 3, 5) != 1
                 && (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)], 3, 5) != 5) {
                    sprintf(__gxvDummyStr, __gxvWarnings[24], texId);
                    __gxVerif->cb(1, 24, __gxvDummyStr);
                }
                if ((u32)GET_REG_FIELD(__gxVerif->rasRegs[0x84 + SOMEINDEX(texId)], 8, 0) > (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x84 + SOMEINDEX(texId)], 8, 8)) {
                    sprintf(__gxvDummyStr, __gxvWarnings[25], texId);
                    __gxVerif->cb(1, 25, __gxvDummyStr);
                }
                for (
                  nlevels = 0;
                  (
                    MAX((u32)GET_REG_FIELD(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)], 10, 0) + 1,
                        (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)], 10, 10) + 1) >> nlevels
                  ) != 0;
                  nlevels++) {
                }
                if (GET_REG_FIELD(__gxVerif->rasRegs[0x84 + SOMEINDEX(texId)], 8, 8) > (nlevels - 1) * 16) {
                    sprintf(__gxvDummyStr, __gxvWarnings[26], texId);
                    __gxVerif->cb(1, 26, __gxvDummyStr);
                }
                if (GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)], 1, 21) && GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)], 1, 8)) {
                    sprintf(__gxvDummyStr, __gxvWarnings[27], texId);
                    __gxVerif->cb(1, 27, __gxvDummyStr);
                }
                if (GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)], 2, 19)
                 && (GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)] & 0xFFFF, 2, 5) == 0
                  || (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)], 3, 5) != 6
                  || (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)], 1, 4) != 1
                  || (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)], 4, 20) == 8
                  || (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)], 4, 20) == 9
                  || (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)], 4, 20) == 10
                  || (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)], 1, 8) != 0
                  || (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)], 1, 21))) {
                    sprintf(__gxvDummyStr, __gxvWarnings[28], texId);
                    __gxVerif->cb(1, 28, __gxvDummyStr);
                }
            }
            if (GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)], 1, 18) != 0) {
                if (__gxVerif->verifyLevel >= 1
                 && ((u32)GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)], 3, 5) != 4 || (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)], 1, 4) != 1)) {
                    sprintf(__gxvDummyStr, __gxvWarnings[29], texId);
                    __gxVerif->cb(1, 29, __gxvDummyStr);
                }
                if (__gxVerif->verifyLevel >= 3
                 && (!GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)], 1, 17) || (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)], 4, 20) != 1 || (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)], 2, 19) != 0)) {
                    sprintf(__gxvDummyStr, __gxvWarnings[30], texId);
                    __gxVerif->cb(1, 30, __gxvDummyStr);
                }
            }
            if (GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)], 1, 17) != 0) {
                if (__gxVerif->verifyLevel >= 1
                 && ((u32)GET_REG_FIELD(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)], 4, 20) == 8 || (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)], 4, 20) == 9 || (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x88 + SOMEINDEX(texId)], 4, 20) == 10)) {
                    sprintf(__gxvDummyStr, __gxvWarnings[31], texId);
                    __gxVerif->cb(1, 31, __gxvDummyStr);
                }
                if (__gxVerif->verifyLevel >= 3
                 && (!GET_REG_FIELD(__gxVerif->rasRegs[0x80 + SOMEINDEX(texId)], 1, 18) || 0)) {
                    sprintf(__gxvDummyStr, __gxvWarnings[30], texId);
                    __gxVerif->cb(1, 30, __gxvDummyStr);
                }
            }
        }
    }
}
void __GXVerifyTEV(void)
{
    unsigned long i; // r31
    unsigned long nTev; // r29
    unsigned long nCol; // r28
    unsigned long enabled; // r30
    unsigned long color; // r27
    unsigned long Clh[4]; // r1+0x38
    unsigned long Alh[4]; // r1+0x28
    unsigned long Cwritten[4]; // r1+0x18
    unsigned long Awritten[4]; // r1+0x8

    nTev = GET_REG_FIELD(__gxVerif->rasRegs[0], 4, 10) + 1;
    nCol = GET_REG_FIELD(__gxVerif->rasRegs[0], 3, 4);
nCol;
    for (i = 0; i < 4; i++) {
        Clh[i] = 0;
        Alh[i] = 0;
        Cwritten[i] = 0;
        Awritten[i] = 0;
    }

    for (i = 0; i < nTev; i++) {
        if (__gxVerif->verifyLevel >= 1
         && (((u32) ((__gxVerif->rasRegs[(i * 2) + 0xC0] & 0xFF000000) + 0x01000000) == 0U) || ((u32) ((__gxVerif->rasRegs[(i * 2) + 0xC1] & 0xFF000000) + 0x01000000) == 0U))) {
            sprintf(__gxvDummyStr, __gxvWarnings[32], i);
            __gxVerif->cb(1, 0x20U, __gxvDummyStr);
        }
        if (i & 1) {
            color = GET_REG_FIELD(__gxVerif->rasRegs[(i >> 1U) + 0x28], 3, 19);
        } else {
            color = GET_REG_FIELD(__gxVerif->rasRegs[(i >> 1U) + 0x28], 3, 7);
        }
        if (__gxVerif->verifyLevel >= 2 && ((color == 0 && nCol < 1) || (color == 1 && nCol < 2))) {
            sprintf(__gxvDummyStr, __gxvWarnings[33], i);
            __gxVerif->cb(1, 0x21U, __gxvDummyStr);
        }
        if (i & 1) {
            enabled = GET_REG_FIELD(__gxVerif->rasRegs[(i >> 1U) + 0x28], 1, 18);
        } else {
            enabled = GET_REG_FIELD(__gxVerif->rasRegs[(i >> 1U) + 0x28], 1, 6);
        }
        if (__gxVerif->verifyLevel >= 1) {
            if (!enabled && ((u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 4, 12) == 8 || (u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 4, 12) == 9)) {
                sprintf(__gxvDummyStr, __gxvWarnings[0x22], "A", i);
                __gxVerif->cb(1, 0x22U, __gxvDummyStr);
            }
            if (!enabled && ((u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 4, 8) == 8 || (u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 4, 8) == 9)) {
                sprintf(__gxvDummyStr, __gxvWarnings[0x22], "B", i);
                __gxVerif->cb(1, 0x22U, __gxvDummyStr);
            }
            if (!enabled && ((u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 4, 4) == 8 || (u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 4, 4) == 9)) {
                sprintf(__gxvDummyStr, __gxvWarnings[0x22], "C", i);
                __gxVerif->cb(1, 0x22U, __gxvDummyStr);
            }
            if (!enabled && ((u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 4, 0) == 8 || (u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 4, 0) == 9)) {
                sprintf(__gxvDummyStr, __gxvWarnings[0x22], "D", i);
                __gxVerif->cb(1, 0x22U, __gxvDummyStr);
            }
            if (!enabled && (u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 13) == 4) {
                sprintf(__gxvDummyStr, __gxvWarnings[0x23], "A", i);
                __gxVerif->cb(1, 0x23U, __gxvDummyStr);
            }
            if (!enabled && (u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 10) == 4) {
                sprintf(__gxvDummyStr, __gxvWarnings[0x23], "B", i);
                __gxVerif->cb(1, 0x23U, __gxvDummyStr);
            }
            if (!enabled && (u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 7) == 4) {
                sprintf(__gxvDummyStr, __gxvWarnings[0x23], "C", i);
                __gxVerif->cb(1, 0x23U, __gxvDummyStr);
            }
            if (!enabled && (u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 4) == 4) {
                sprintf(__gxvDummyStr, __gxvWarnings[0x23], "D", i);
                __gxVerif->cb(1, 0x23U, __gxvDummyStr);
            }
            if ((u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 4, 12) <= 7 && ((__gxVerif->rasRegs[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 4, 12) + 0xE1] & 0xFF000000) + 0x01000000) == 0U) {
                if (GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 1, 12) ? !Awritten[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 3, 13)] : !Cwritten[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 3, 13)]) {
                    sprintf(__gxvDummyStr, __gxvWarnings[0x24], "A", i, GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 1, 0xCU) ? "alpha" : "color", (__gxVerif->rasRegs[(i * 2) + 0xC0] >> 0xDU) & 7);
                    __gxVerif->cb(1, 0x24U, __gxvDummyStr);
                }
            }
            if ((u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 4, 8) <= 7 && ((__gxVerif->rasRegs[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 4, 8) + 0xE1] & 0xFF000000) + 0x01000000) == 0U) {
                if (GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 1, 8) ? !Awritten[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 3, 9)] : !Cwritten[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 3, 9)]) {
                    sprintf(__gxvDummyStr, __gxvWarnings[0x24], "B", i, GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 1, 8U) ? "alpha" : "color", (__gxVerif->rasRegs[(i * 2) + 0xC0] >> 9U) & 7);
                    __gxVerif->cb(1, 0x24U, __gxvDummyStr);
                }
            }
            if ((u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 4, 4) <= 7 && ((__gxVerif->rasRegs[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 4, 4) + 0xE1] & 0xFF000000) + 0x01000000) == 0U) {
                if (GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 1, 4) ? !Awritten[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 3, 5)] : !Cwritten[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 3, 5)]) {
                    sprintf(__gxvDummyStr, __gxvWarnings[0x24], "C", i, GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 1, 4U) ? "alpha" : "color", (__gxVerif->rasRegs[(i * 2) + 0xC0] >> 5U) & 7);
                    __gxVerif->cb(1, 0x24U, __gxvDummyStr);
                }
            }
            if ((u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 4, 0) <= 7 && ((__gxVerif->rasRegs[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 4, 0) + 0xE1] & 0xFF000000) + 0x01000000) == 0U) {
                if (GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 1, 0) ? !Awritten[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 3, 1)] : !Cwritten[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 3, 1)]) {
                    sprintf(__gxvDummyStr, __gxvWarnings[0x24], "D", i, (__gxVerif->rasRegs[(i * 2) + 0xC0] & 1) ? "alpha" : "color", (__gxVerif->rasRegs[(i * 2) + 0xC0] >> 1U) & 7);
                    __gxVerif->cb(1, 0x24U, __gxvDummyStr);
                }
            }
            if ((u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 13) <= 3 && ((__gxVerif->rasRegs[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 4, 14) + 0xE0] & 0xFF000000) + 0x01000000) == 0U && Awritten[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 13)] == 0U) {
                sprintf(__gxvDummyStr, __gxvWarnings[0x25], "A", i, GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 13));
                __gxVerif->cb(1, 0x25U, __gxvDummyStr);
            }
            if ((u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 10) <= 3 && ((__gxVerif->rasRegs[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 11) + 0xE0] & 0xFF000000) + 0x01000000) == 0U && Awritten[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 10)] == 0U) {
                sprintf(__gxvDummyStr, __gxvWarnings[0x25], "B", i, GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 10));
                __gxVerif->cb(1, 0x25U, __gxvDummyStr);
            }
            if ((u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 7) <= 3 && ((__gxVerif->rasRegs[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 8) + 0xE0] & 0xFF000000) + 0x01000000) == 0U && Awritten[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 7)] == 0U) {
                sprintf(__gxvDummyStr, __gxvWarnings[0x25], "C", i, GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 7));
                __gxVerif->cb(1, 0x25U, __gxvDummyStr);
            }
            if ((u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 4) <= 3 && ((__gxVerif->rasRegs[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 3) + 0xE0] & 0xFF000000) + 0x01000000) == 0U && Awritten[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 4)] == 0U) {
                sprintf(__gxvDummyStr, __gxvWarnings[0x25], "D", i, GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 4));
                __gxVerif->cb(1, 0x25U, __gxvDummyStr);
            }
        }
        if (__gxVerif->verifyLevel >= 3) {
            if ((u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 4, 12) <= 7) {
                if (GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 1, 12) ? Alh[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 3, 13)] : Clh[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 3, 13)]) {
                    sprintf(__gxvDummyStr, __gxvWarnings[0x26], "A", i, GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 1, 12) ? "alpha" : "color", GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 3, 13));
                    __gxVerif->cb(3, 0x26U, __gxvDummyStr);
                }
            }
            if ((u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 4, 8) <= 7) {
                if (GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 1, 8) ? Alh[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 3, 9)] : Clh[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 3, 9)]) {
                    sprintf(__gxvDummyStr, __gxvWarnings[0x26], "B", i, GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 1, 8) ? "alpha" : "color", GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 3, 9));
                    __gxVerif->cb(3, 0x26U, __gxvDummyStr);
                }
            }
            if ((u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 4, 4) <= 7) {
                if (GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 1, 4) ? Alh[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 3, 5)] : Clh[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 3, 5)]) {
                    sprintf(__gxvDummyStr, __gxvWarnings[0x26], "C", i, GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 1, 4) ? "alpha" : "color", GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 3, 5));
                    __gxVerif->cb(3, 0x26U, __gxvDummyStr);
                }
            }
            if ((u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 13) <= 3 && (u32)Alh[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 13)] != 0) {
                sprintf(__gxvDummyStr, __gxvWarnings[0x27], "A", i, GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 0xDU));
                __gxVerif->cb(3, 0x27U, __gxvDummyStr);
            }
            if ((u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 10) <= 3 && (u32)Alh[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 10)] != 0) {
                sprintf(__gxvDummyStr, __gxvWarnings[0x27], "B", i, ((__gxVerif->rasRegs[(i * 2) + 0xC1] >> 0xAU) & 7));
                __gxVerif->cb(3, 0x27U, __gxvDummyStr);
            }
            if ((u32)GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 7) <= 3 && (u32)Alh[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 7)] != 0) {
                sprintf(__gxvDummyStr, __gxvWarnings[0x27], "C", i, GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 3, 7U));
                __gxVerif->cb(3, 0x27U, __gxvDummyStr);
            }
        }
        Cwritten[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 2, 22)] = 1;
        Awritten[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 2, 22)] = 1;
        Clh[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 2, 22)] = (!GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 2, 0) && !GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC0], 1, 19));
        Alh[GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 2, 22)] = (!GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 2, 0) && !GET_REG_FIELD(__gxVerif->rasRegs[(i * 2) + 0xC1], 1, 19));
    }

    for (i = 0; i < 4; i++) {
        if (Cwritten[i] != 0U) {
            __gxVerif->rasRegs[(i * 2) + 0xE1] = (__gxVerif->rasRegs[(i * 2) + 0xE1] & 0xFFFFFF) | 0xFF000000;
        }
        if (Awritten[i] != 0U) {
            __gxVerif->rasRegs[(i * 2) + 0xE0] = (__gxVerif->rasRegs[(i * 2) + 0xE0] & 0xFFFFFF) | 0xFF000000;
        }
    }
    if (GET_REG_FIELD(__gxVerif->rasRegs[0xF5], 2, 2) && __gxVerif->verifyLevel >= 1) {
        if ((u32) ((__gxVerif->rasRegs[0xF4] & 0xFF000000) + 0x01000000) == 0U) {
            __gxVerif->cb(1, 0x28U, __gxvWarnings[0x28]);
        }
        if (!enabled) {
            __gxVerif->cb(1, 0x29U, __gxvWarnings[0x29]);
        }
    }
    if (__gxVerif->verifyLevel >= 2) {
        if (GET_REG_FIELD(__gxVerif->rasRegs[((nTev - 1) * 2) + 0xC0], 2, 22)) {
            __gxVerif->cb(2, 0x2AU, __gxvWarnings[0x2A]);
        }
        if (GET_REG_FIELD(__gxVerif->rasRegs[((nTev - 1) * 2) + 0xC1], 2, 22)) {
            __gxVerif->cb(2, 0x2BU, __gxvWarnings[0x2B]);
        }
    }
    if (__gxVerif->verifyLevel >= 3) {
        if (!GET_REG_FIELD(__gxVerif->rasRegs[((nTev - 1) * 2) + 0xC1], 2, 0) && !GET_REG_FIELD(__gxVerif->rasRegs[((nTev - 1) * 2) + 0xC0], 1, 19)) {
            __gxVerif->cb(3, 0x2CU, __gxvWarnings[0x2C]);
        }
        if (!GET_REG_FIELD(__gxVerif->rasRegs[((nTev - 1) * 2) + 0xC1], 2, 0) && !GET_REG_FIELD(__gxVerif->rasRegs[((nTev - 1) * 2) + 0xC1], 1, 19)) {
            __gxVerif->cb(3, 0x2DU, __gxvWarnings[0x2D]);
        }
    }
    if (__gxVerif->verifyLevel >= 2 && GET_REG_FIELD(__gxVerif->rasRegs[0x43], 1, 6) && (GET_REG_FIELD(__gxVerif->rasRegs[0xF3], 2, 22) || ((u32) GET_REG_FIELD(__gxVerif->rasRegs[0xF3], 3, 16) != 7) || ((u32) GET_REG_FIELD(__gxVerif->rasRegs[0xF3], 3, 19) != 7))) {
        __gxVerif->cb(2, 0x2EU, __gxvWarnings[0x2E]);
    }
}

void __GXVerifyPE(void)
{
    u32 i;

    if (__gxVerif->verifyLevel >= 1 && GET_REG_FIELD(__gxVerif->rasRegs[0x41], 1, 0) && GET_REG_FIELD(__gxVerif->rasRegs[0x41], 1, 1)) {
        __gxVerif->cb(1, 0x2FU, __gxvWarnings[0x2F]);
    }
    if (__gxVerif->verifyLevel >= 2) {
        if (GET_REG_FIELD(__gxVerif->rasRegs[0], 1, 9) && (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x43], 3, 0) != 2) {
            __gxVerif->cb(2, 0x31U, __gxvWarnings[0x31]);
        }
        if (!GET_REG_FIELD(__gxVerif->rasRegs[0], 1, 9) && (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x43], 3, 0) == 2) {
            __gxVerif->cb(2, 0x32U, __gxvWarnings[0x32]);
        }
    }
    if (__gxVerif->verifyLevel >= 3) {
        for (i = 0; i < 4; i++) {
            if (((u32)GET_REG_FIELD(__gxVerif->rasRegs[i + 1], 4, 4) > GET_REG_FIELD(__gxVerif->rasRegs[i + 1], 4, 12) || (u32)GET_REG_FIELD(__gxVerif->rasRegs[i + 1], 4, 12) > (u32)GET_REG_FIELD(__gxVerif->rasRegs[i + 1], 4, 20))
             && (u32)GET_REG_FIELD(__gxVerif->rasRegs[0x43], 3, 0) == 2) {
                sprintf(__gxvDummyStr, __gxvWarnings[0x33], i);
                __gxVerif->cb(3, 0x33U, __gxvDummyStr);
            }
        }
    }
}

#endif
