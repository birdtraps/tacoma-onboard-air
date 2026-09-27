#include "UIController.h"

UIController::UIController(AirSystem& airSystem)
    : air(airSystem)
{
}

void UIController::begin()
{
    screen = UIScreen::TIRES;
    state = UIState::NORMAL;

    air.deselectAll();

    editingTarget = air.state.targetPressure;
}

void UIController::update()
{
    if (state == UIState::RUNNING &&
        air.state.mode == SystemMode::COMPLETE) {

        clearTireSelection();
        state = UIState::NORMAL;
    }

    if (air.state.mode == SystemMode::ERROR) {
        clearTireSelection();
        state = UIState::NORMAL;
    }
}


// --------------------------------------------------
// INDIVIDUAL TIRE SELECTION
// --------------------------------------------------

void UIController::toggleTire(TireIndex tire)
{
    if (screen != UIScreen::TIRES)
        return;

    if (state == UIState::RUNNING ||
        state == UIState::MANUAL_DUMP)
        return;

    bool newState = !air.state.tireSelected[tire];

    air.selectTire(tire, newState);

    if (hasTireSelection()) {
        state = UIState::TIRE_SELECTED;
        editingTarget = air.state.targetPressure;
    }
    else {
        state = UIState::NORMAL;
    }
}


// --------------------------------------------------
// GROUP SELECTION
// --------------------------------------------------

void UIController::selectFront()
{
    if (screen != UIScreen::TIRES ||
        state == UIState::RUNNING)
        return;

    // If front is already selected by itself,
    // pressing FRONT again clears selection.
    if (frontSelected() &&
        !air.state.tireSelected[LR] &&
        !air.state.tireSelected[RR]) {

        clearTireSelection();
        state = UIState::NORMAL;
        return;
    }

    air.deselectAll();

    air.selectTire(LF, true);
    air.selectTire(RF, true);

    editingTarget = air.state.targetPressure;
    state = UIState::TIRE_SELECTED;
}


void UIController::selectRear()
{
    if (screen != UIScreen::TIRES ||
        state == UIState::RUNNING)
        return;

    if (rearSelected() &&
        !air.state.tireSelected[LF] &&
        !air.state.tireSelected[RF]) {

        clearTireSelection();
        state = UIState::NORMAL;
        return;
    }

    air.deselectAll();

    air.selectTire(LR, true);
    air.selectTire(RR, true);

    editingTarget = air.state.targetPressure;
    state = UIState::TIRE_SELECTED;
}


void UIController::selectAll()
{
    if (screen != UIScreen::TIRES ||
        state == UIState::RUNNING)
        return;

    if (allSelected()) {

        clearTireSelection();
        state = UIState::NORMAL;
        return;
    }

    air.selectAll();

    editingTarget = air.state.targetPressure;
    state = UIState::TIRE_SELECTED;
}


void UIController::clearTireSelection()
{
    air.deselectAll();
}


bool UIController::hasTireSelection() const
{
    return air.state.tireSelected[LF] ||
           air.state.tireSelected[RF] ||
           air.state.tireSelected[LR] ||
           air.state.tireSelected[RR];
}


bool UIController::isTireSelected(TireIndex tire) const
{
    return air.state.tireSelected[tire];
}


bool UIController::frontSelected() const
{
    return air.state.tireSelected[LF] &&
           air.state.tireSelected[RF];
}


bool UIController::rearSelected() const
{
    return air.state.tireSelected[LR] &&
           air.state.tireSelected[RR];
}


bool UIController::allSelected() const
{
    return frontSelected() && rearSelected();
}


// --------------------------------------------------
// ENCODER
// --------------------------------------------------

void UIController::encoderRotate(int direction)
{
    if (!hasTireSelection())
        return;

    if (state != UIState::TIRE_SELECTED &&
        state != UIState::EDITING_TARGET)
        return;

    state = UIState::EDITING_TARGET;

    if (direction > 0)
        editingTarget += 1.0f;

    if (direction < 0)
        editingTarget -= 1.0f;

    if (editingTarget > 50.0f)
        editingTarget = 50.0f;

    if (editingTarget < 5.0f)
        editingTarget = 5.0f;
}


void UIController::encoderPress()
{
    // Short press = CANCEL

    if (state == UIState::TIRE_SELECTED ||
        state == UIState::EDITING_TARGET) {

        clearTireSelection();
        state = UIState::NORMAL;
        return;
    }

    // While running, short press = STOP
    if (state == UIState::RUNNING) {

        air.stop();
        clearTireSelection();
        state = UIState::NORMAL;
    }
}


void UIController::encoderLongPress()
{
    // Long press = ACCEPT / START

    if (state == UIState::TIRE_SELECTED ||
        state == UIState::EDITING_TARGET) {

        startTargetOperation();
    }
}


void UIController::startTargetOperation()
{
    if (!hasTireSelection())
        return;

    air.setTarget(editingTarget);
    air.start();

    state = UIState::RUNNING;
}


// --------------------------------------------------
// MANUAL DUMP
// --------------------------------------------------

void UIController::dumpPressed()
{
    if (screen != UIScreen::TIRES)
        return;

    if (!hasTireSelection())
        return;

    // Placeholder until AirSystem gets manual
    // valve control.
    state = UIState::MANUAL_DUMP;
}


void UIController::dumpReleased()
{
    if (state != UIState::MANUAL_DUMP)
        return;

    // Eventually:
    // air.stopManualDump();

    state = UIState::TIRE_SELECTED;
}


// --------------------------------------------------
// PAG