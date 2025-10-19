const float target_fps = 60.0f;

extern float reported_frametime;

void frametime_initialize(void);
void FrameDone(void);
void set_divisor(float divisor);
void increase_frametime_pause_counter(void);
void decrease_frametime_pause_counter(void);
void increase_animation_pause_counter(void);
void decrease_animation_pause_counter(void);
