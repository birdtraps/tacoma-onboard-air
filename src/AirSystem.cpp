#include "AirSystem.h"

void AirSystem::begin()
{
    state.mode = SystemMode::IDLE;
}

void AirSystem::update()
{
    if (state.mode == SystemMode::RUNNING) {
        updateCornerStates();
    }

    simulate();
}

void AirSystem::setTarget(float psi)
{
    if (psi < 5.0f)
        psi = 5.0f;

    if (psi > 50.0f)
        psi = 50.0f;

    state.targetPressure = psi;
}

void AirSystem::increaseTarget()
{
    setTarget(state.targetPressure + 1.0f);
}

void AirSystem::decreaseTarget()
{
    setTarget(state.targetPressure - 1.0f);
}

void AirSystem::start()
{
    bool anySelected = false;

    for (int i = 0; i < 4; i++) {
        if (state.tireSelected[i]) {
            anySelected = true;
            break;
        }
    }

    if (!anySelected)
        return;

    state.mode = SystemMode::RUNNING;

    updateCornerStates();
}

void AirSystem::stop()
{
    for (int i = 0; i < 4; i++) {
        state.cornerMode[i] = CornerMode::HOLD;
    }

    state.mode = SystemMode::IDLE;
}

void AirSystem::selectTire(TireIndex tire, bool selected)
{
    state.tireSelected[tire] = selected;

    if (!selected)
        state.cornerMode[tire] = CornerMode::HOLD;
}

void AirSystem::selectAll()
{
    for (int i = 0; i < 4; i++)
        state.tireSelected[i] = true;
}

void AirSystem::deselectAll()
{
    for (int i = 0; i < 4; i++) {
        state.tireSelected[i] = false;
        state.cornerMode[i] = CornerMode::HOLD;
    }
}

void AirSystem::updateCornerStates()
{
    for (int i = 0; i < 4; i++) {

        if (!state.tireSelected[i]) {
            state.cornerMode[i] = CornerMode::HOLD;
            continue;
        }

        float error =
            state.targetPressure - state.tirePressure[i];

        if (error > PRESSURE_TOLERANCE) {
            state.cornerMode[i] = CornerMode::FILL;
        }
        else if (error < -PRESSURE_TOLERANCE) {
            state.cornerMode[i] = CornerMode::DUMP;
        }
        else {
            state.cornerMode[i] = CornerMode::HOLD;
        }
    }

    if (allSelectedTiresAtTarget()) {
        state.mode = SystemMode::COMPLETE;

        for (int i = 0; i < 4; i++)
            state.cornerMode[i] = CornerMode::HOLD;
    }
}

bool AirSystem::allSelectedTiresAtTarget()
{
    for (int i = 0; i < 4; i++) {

        if (!state.tireSelected[i])
            continue;

        float error =
            abs(state.targetPressure - state.tirePressure[i]);

        if (error > PRESSURE_TOLERANCE)
            return false;
    }

    return true;
}

void AirSystem::simulate()
{
    if (millis() - lastSimulationUpdate < 100)
        return;

    lastSimulationUpdate = millis();

    constexpr float FILL_RATE = 0.10f;
    constexpr float DUMP_RATE 