#include "game/RenderSystem.h"
#include "game/NGCSystem.h"
#include "game/macros.h"
#include "dolphin.h"
#include "dolphin/dvd.h"
#include "dolphin/vi/vifuncs.h"

bool UnknownRenderBool;

void NGCSystem::DriveStatus(u8 unk1, int unk2) {}

void NGCSystem::InitializeSystems() {
    OSInit();
    DVDInit();
    VIInit();

    gRenderSystem.InitRenderMode(NULL);
    gRenderSystem.InitFramebuffers();

    gRenderSystem.InitGX();
    gRenderSystem.DrawFirstFramebuffer();
    gRenderSystem.Unknown();

    VISetBlack(TRUE);
    VIFlush();
    VIWaitForRetrace();
}

u32 NGCSystem::GetTicks(int tick_resolution) {
    OSTick tick = OSGetTick();
    switch (tick_resolution) {
    case 1000:
        return OSTicksToMilliseconds(tick);
    case 1:
        return OSTicksToSeconds(tick);
    case 1000000:
        return OSTicksToMicroseconds(tick);
    default:
        DEBUGASSERTLINE(397, 0);
        return 0;
    }
}
