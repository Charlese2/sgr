#include "game/frametime.h"
#include "game/NGCSystem.h"
#include "game/delay.h"
#include "game/timestamp.h"
#include "game/macros.h"

const int frametime_history_size = 16;

float actual_frametime       = 1.0f / target_fps;
float reported_frametime     = 1.0f / target_fps;
float fps                    = target_fps;
float frametime_divisor      = 1.0f;
float min_reported_frametime = 1.0f / (target_fps * 2);
float max_reported_frametime = 1.0f / (target_fps / 15);

int frames_rendered;
u32 milliseconds_unpaused;
static float min_frametime = min_reported_frametime;
static float max_frametime = max_reported_frametime;
int previous_done_time;
int frametime_history_index = -1;
float total_history_frametime;
bool Frametime_initialized;
int Frametime_paused;
int Animation_timer_paused;

float frametime_history[frametime_history_size];

void frametime_initialize() {
    actual_frametime        = 1.0f / target_fps;
    reported_frametime      = actual_frametime;
    fps                     = target_fps;
    min_reported_frametime  = 1.0f / (target_fps * 2);
    frames_rendered         = 0;
    milliseconds_unpaused   = 0;
    frametime_divisor       = 1.0f;
    max_reported_frametime  = 1.0f / (target_fps / 15);
    previous_done_time      = 0;
    frametime_history_index = -1;
    total_history_frametime = 0.0f;
}

void FrameDone() {

    if (!Frametime_initialized) {
        frametime_initialize();
        Frametime_initialized = true;
    }
    if (Frametime_paused) {
        return;
    }
    int microsecond_ticks = NGCSystem::GetTicks(1000000);
    int delta_time;
    if (microsecond_ticks < previous_done_time) {
        delta_time = microsecond_ticks;
    } else {
        delta_time = microsecond_ticks - previous_done_time;
    }
    if (delta_time < 0) {
        printf("delta_time is negative.\n");
        delta_time = 1000000.0f * reported_frametime;
    }
    previous_done_time = microsecond_ticks;
    actual_frametime   = delta_time / 1000000.0f;
    reported_frametime = actual_frametime;
    if (reported_frametime < min_reported_frametime) {
        float frame_limit_wait_time = min_reported_frametime - reported_frametime;
        Wait(frame_limit_wait_time * 1000.0f);
        reported_frametime = min_reported_frametime;
    } else if (reported_frametime > max_reported_frametime) {
        reported_frametime = max_reported_frametime;
    }
    if (frametime_history_index == -1) {
        for (int index = 0; index < frametime_history_size; index++) {
            frametime_history[index] = 0.0f;
        }
        total_history_frametime = 0.0f;
        frametime_history_index = 0;
    }
    total_history_frametime -= frametime_history[frametime_history_index];
    total_history_frametime += reported_frametime;
    frametime_history[frametime_history_index] = reported_frametime;
    frametime_history_index                    = (frametime_history_index + 1) % 16;
    if (total_history_frametime != 0.0f) {
        if (frames_rendered >= frametime_history_size) {
            fps = 16.0f / total_history_frametime;
        } else {
            fps = frames_rendered / total_history_frametime;
        }
    } else {
        fps = 0.0f;
    }
    reported_frametime /= frametime_divisor;
    frames_rendered++;
    int time = reported_frametime * 1000.0f;
    UpdateTicker(time);
    if (!Animation_timer_paused) {
        milliseconds_unpaused += time;
    }
}

void set_divisor(float divisor) {
    DEBUGASSERTLINE(189, divisor != 0);
    frametime_divisor = divisor;
}

void increase_frametime_pause_counter() { Frametime_paused++; }

void decrease_frametime_pause_counter() {
    Frametime_paused--;
    DEBUGASSERTLINE(263, Frametime_paused >= 0);
    previous_done_time = NGCSystem::GetTicks(1000000);
}

void increase_animation_pause_counter() { Animation_timer_paused++; }

void decrease_animation_pause_counter() {
    Animation_timer_paused--;
    DEBUGASSERTLINE(281, Animation_timer_paused >= 0);
}
