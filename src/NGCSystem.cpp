#include "game/RenderSystem.h"
#include "game/NGCSystem.h"
#include "dolphin.h"
#include "dolphin/dvd.h"
#include "dolphin/vi/vifuncs.h"

bool UnknownRenderBool;

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
