#include "game/timestamp.h"
#include "game/macros.h"
#include "dolphin/types.h"

extern "C" {
int rand();
} // TODO: setup headers correctly

int Timestamp_ticker          = 1;
int Timestamp_realtime_ticker = 1;
int Timestamp_paused;

void GameTimer::UpdateTicker(int delta_milliseconds) {
    int remainingTime;
    int remainingRealTime;
    if (!Timestamp_paused) {
        remainingTime = MAX_TIME - Timestamp_ticker;
        if (delta_milliseconds > remainingTime) {
            Timestamp_ticker = delta_milliseconds - remainingTime;
        } else {
            Timestamp_ticker += delta_milliseconds;
        }
    }
    remainingRealTime = MAX_TIME - Timestamp_realtime_ticker;
    if (delta_milliseconds > remainingRealTime) {
        Timestamp_realtime_ticker = delta_milliseconds - remainingRealTime;
    } else {
        Timestamp_realtime_ticker += delta_milliseconds;
    }
}

void GameTimer::IncrementPauseCounter() { Timestamp_paused += 1; }

void GameTimer::DecrementPauseCounter() {
    Timestamp_paused -= 1;
    DEBUGASSERTLINE(54, Timestamp_paused >= 0);
}

void GameTimer::SetTimeout(int delta_milliseconds) {
    int maxTime;
    DEBUGASSERTLINE(65, Timestamp_ticker < MAX_TIME);
    DEBUGASSERTLINE(66, (delta_milliseconds < MAX_TIME / 2) && (delta_milliseconds >= -MAX_TIME / 2));
    if (delta_milliseconds >= 0) {
        maxTime = (MAX_TIME - Timestamp_ticker);
        if (delta_milliseconds > maxTime) {
            value = delta_milliseconds - maxTime;
            return;
        }
        value = Timestamp_ticker + delta_milliseconds;
        return;
    }
    value = Timestamp_ticker + delta_milliseconds;
    if (value < 0) {
        value += MAX_TIME;
    }
}

void GameTimer::SetRandomTimeout(int low_milliseconds, int high_milliseconds) {
    DEBUGASSERTLINE(91, low_milliseconds >= 0);
    DEBUGASSERTLINE(92, high_milliseconds >= 0);
    DEBUGASSERTLINE(93, high_milliseconds >= low_milliseconds);
    SetTimeout(((rand() % (high_milliseconds - low_milliseconds + 1)) + low_milliseconds));
    DEBUGASSERTLINE(96, value != -1);
}

bool GameTimer::elapsed() {
    int remainingTimerTime;
    int timeSinceTimerFinished;
    DEBUGASSERTLINE(102, value < MAX_TIME);
    DEBUGASSERTLINE(103, Timestamp_ticker < MAX_TIME);
    if (value < 0) {
        return false;
    }
    if (value > Timestamp_ticker) {
        remainingTimerTime = value - Timestamp_ticker;
        if (remainingTimerTime > MAX_TIME / 2) {
            return true;
        }
        return false;
    }
    timeSinceTimerFinished = Timestamp_ticker - value;
    if (timeSinceTimerFinished > MAX_TIME / 2) {
        return false;
    }
    return true;
}

int GameTimer::GetRemainingTime() {
    int remainingTimerTime;
    int timeSinceTimerFinished;
    if (value < 0) {
        return MAX_TIME;
    }
    if (value > Timestamp_ticker) {
        remainingTimerTime = value - Timestamp_ticker;
        if (remainingTimerTime > MAX_TIME / 2){
            return -(MAX_TIME - value + Timestamp_ticker);
        }
        return remainingTimerTime;
    }
    timeSinceTimerFinished = Timestamp_ticker - value;
    if (timeSinceTimerFinished > MAX_TIME / 2) {
        return (MAX_TIME - Timestamp_ticker) + value;
    }
    return -(timeSinceTimerFinished);
}

int GameTimer::GetElapsedTime() {
    return -GetRemainingTime();
}

bool GameTimer::valid() {
    if (value < 0) {
        return false;
    }
    return true;
}

void RealtimeTimer::SetTimeout(int delta_milliseconds) {
    int maxTime;
    DEBUGASSERTLINE(178, Timestamp_realtime_ticker < MAX_TIME);
    DEBUGASSERTLINE(179, (delta_milliseconds < MAX_TIME / 2) && (delta_milliseconds >= -MAX_TIME / 2));
    if (delta_milliseconds >= 0) {
        maxTime = (MAX_TIME - Timestamp_realtime_ticker);
        if (delta_milliseconds > maxTime) {
            value = delta_milliseconds - maxTime;
            return;
        }
        value = Timestamp_realtime_ticker + delta_milliseconds;
        return;
    }
    value = Timestamp_realtime_ticker + delta_milliseconds;
    if (value < 0) {
        value += MAX_TIME;
    }
}

void RealtimeTimer::SetRandomTimeout(int low_milliseconds, int high_milliseconds) {
    DEBUGASSERTLINE(204, low_milliseconds >= 0);
    DEBUGASSERTLINE(205, high_milliseconds >= 0);
    SetTimeout(((rand() % (high_milliseconds - low_milliseconds + 1)) + low_milliseconds));
}

bool RealtimeTimer::elapsed() {
        int remainingTimerTime;
    int timeSinceTimerFinished;
    DEBUGASSERTLINE(214, value < MAX_TIME);
    DEBUGASSERTLINE(215, Timestamp_realtime_ticker < MAX_TIME);
    if (value < 0) {
        return false;
    }
    if (value > Timestamp_realtime_ticker) {
        remainingTimerTime = value - Timestamp_realtime_ticker;
        if (remainingTimerTime > MAX_TIME / 2) {
            return true;
        }
        return false;
    }
    timeSinceTimerFinished = Timestamp_realtime_ticker - value;
    if (timeSinceTimerFinished > MAX_TIME / 2) {
        return false;
    }
    return true;
}

int RealtimeTimer::GetRemainingTime() {
    int remainingTimerTime;
    int timeSinceTimerFinished;
    if (value < 0) {
        return MAX_TIME;
    }
    if (value > Timestamp_realtime_ticker) {
        remainingTimerTime = value - Timestamp_realtime_ticker;
        if (remainingTimerTime > MAX_TIME / 2){
            return -(MAX_TIME - value + Timestamp_realtime_ticker);
        }
        return remainingTimerTime;
    }
    timeSinceTimerFinished = Timestamp_realtime_ticker - value;
    if (timeSinceTimerFinished > MAX_TIME / 2) {
        return (MAX_TIME - Timestamp_realtime_ticker) + value;
    }
    return -(timeSinceTimerFinished);
}

int RealtimeTimer::GetElapsedTime() {
    return -GetRemainingTime();
}

bool RealtimeTimer::valid() {
    if (value < 0) {
        return false;
    }
    return true;
}
