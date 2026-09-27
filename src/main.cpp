#include <Arduino.h>
#include "AirSystem.h"

AirSystem air;

const char* cornerModeName(CornerMode mode)
{
    switch (mode) {
        case CornerMode::HOLD: return "HOLD";
        case CornerMode::FILL: return "FILL";
        case CornerMode::DUMP: return "DUMP";
    }

    return "?";
}

void printStatus()
{
    const char* tireNames[] = {"LF", "RF", "LR", "RR"};

    Serial.println();
    Serial.println("------- TACOMA AIR -------");

    for (int i = 0; i < 4; i++) {

        Serial.printf(
            "%s: %5.1f PSI   %s\n",
            tireNames[i],
            air.state.tirePressure[i],
            cornerModeName(air.state.cornerMode[i])
        );
    }

    Serial.printf(
        "Target: %.1f PSI\n",
        air.state.targetPressure
    );

    Serial.print("System: ");

    switch (air.state.mode) {

        case SystemMode::IDLE:
            Serial.println("IDLE");
            break;

        case SystemMode::RUNNING:
            Serial.println("RUNNING");
            break;

        case SystemMode::COMPLETE:
            Serial.println("COMPLETE");
            break;

        case SystemMode::ERROR:
            Serial.println("ERROR");
            break;
    }

    Serial.println("-----------------