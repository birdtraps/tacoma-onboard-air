#pragma once

#include <Arduino.h>

enum TireIndex {
    LF = 0,
    RF = 1,
    LR = 2,
    RR = 3
};

enum class CornerMode {
    HOLD,
    FILL,
    DUMP
};

enum class SystemMode {
    IDLE,
    RUNNING,
    COMPLETE,
    ERROR
};

struct AirSystemState {
    float tirePressure[4] = {
        35.0f, 35.0f, 35.0f, 35.0f
    };

    float targetPressure = 35.0f;
    float tankPressure = 150.0f;

    bool tireSelected[4] = {
        true, true, true, true
    };

    CornerMode cornerMode[4] = {
        CornerMode::HOLD,
        CornerMode::HOLD,
        CornerMode::HOLD,
        CornerMode::HOLD
    };

    bool compressorOn = false;

    SystemMode mode = SystemMode::IDLE;
};

class AirSystem {
public:
    AirSystemState state;

    void begin();
    void update();

    void setTarget(float psi);
    void increaseTarget();
    void decreaseTarget();

    void start();
    void stop();

    void selectTire(TireIndex tire, bool selected);
    void selectAll();
    void deselectAll();

private:
    unsigned long lastSimulationUpdate = 0;

    static constexpr float PRESSURE_TOLERANCE = 0.2f;

    void updateCornerStates();
    void simulate();
    bool allSelectedTiresAtTarget();
};