#include "game/birds.h"
#include "game/gr_ngc.h"
#include "game/macros.h"
#include "game/camera.h"
#include <stdio.h>

int Num_birds;

bird gBirds[MAX_BIRDS];
char String[16];

void bird::Render() {
    set_alpha_blending_mode(1);
    set_z_mode(4);
    vector3 test;
    for (int i = 0; i < Num_birds; i++) {
        bird *pBird = &gBirds[i];
        if (pBird->unk64 != 3 && pBird->unk64 != 5) {
            pBird->unk10 -= gCamera.unk290;
        }
    }
}

void bird::Unknown() {
    char string[16];
    for (int i = 0; i < Num_birds; i++) {
        sprintf(string, "$%s%i", String, i + 1);

        gBirds[i].unk64 = 0;
        gBirds[i].unk68 = 0.0f;
    }
}

void bird::Unknown2(char *type, int num_birds) {
    DEBUGASSERTLINE(229, num_birds < MAX_BIRDS);
    Num_birds = num_birds;
}

void bird::Unknown3() {
    for (int i = 0; i < Num_birds; i++) {
        bird *pBird = &gBirds[i];
        pBird->unk10 -= gCamera.unk290;
    }
}

void bird::ResetCount() { Num_birds = 0; }

void bird::Unknown4(int birdnum) {
    birdnum--;
    DEBUGASSERTLINE(404, birdnum < Num_birds);
    if (birdnum >= Num_birds) {
        return;
    }
    bird *pBird = &gBirds[birdnum];
    if (pBird->unk64 == 0) {
        pBird->unk64 = 4;
    }
}

void bird::Unknown5(int birdnum, vector3 *unk2) {
    birdnum--;
    DEBUGASSERTLINE(442, birdnum < Num_birds);
    if (birdnum >= Num_birds) {
        return;
    }
    bird *pBird = &gBirds[birdnum];
    if (pBird->unk64 == 4 || pBird->unk64 == 0) {
        pBird->unk64 = 1;
        
    }
}

void bird::Unknown6(int birdnum) {
    birdnum--;
    DEBUGASSERTLINE(466, birdnum < Num_birds);
    if (birdnum >= Num_birds) {
        return;
    }
    bird *pBird  = &gBirds[birdnum];
    pBird->unk64 = 5;
}
