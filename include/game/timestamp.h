#define MAX_TIME 1072800000

class GameTimer {
    int value;

  public:
    static void UpdateTicker(int delta_milliseconds);
    void SetTimeout(int delta_milliseconds);
    static void IncrementPauseCounter(void);
    static void DecrementPauseCounter(void);
    void SetRandomTimeout(int low_miliseconds, int high_miliseconds);
    bool elapsed();
    int GetRemainingTime();
    int GetElapsedTime();
    bool valid();
};

class RealtimeTimer {
    int value;

  public:
    void SetTimeout(int delta_milliseconds);
    void SetRandomTimeout(int low_milliseconds, int high_milliseconds);
    bool elapsed();
    int GetRemainingTime();
    int GetElapsedTime();
    bool valid();
};
