#pragma once

#include <Arduino.h>
#include "AirSystem.h"

enum class UIScreen {
    TIRES,
    AIR_SYSTEM,
    ACCESSORIES
};

enum class UIState {
    NORMAL,
    TIRE_SELECTED,
    EDITING_TARGET,
    RUNNING,
    MANUAL_DUMP
};

class UIController {
public:
    UIController(AirSystem& airSystem);

    void begin();
    void update();

    // Individual tire selection
    void toggleTire(TireIndex tire);

    // Group selection shortcuts
    void selectFront();
    void selectRear();
    void selectAll();

    void clearTireSelection();

    // Selection information
    bool hasTireSelection() const;
    bool isTireSelected(TireIndex tire) const;

    // Manual dump
    void dumpPressed();
    void dumpReleased();

    // Encoder
    void encoderRotate(int direction);
    void encoderPress();
    void encoderLongPress();

    // Navigation
    void swipeLeft();
    void swipeRight();

    // UI getters
    UIScreen getScreen() const;
    UIState getState() const;
    float getEditingTarget() const;

private:
    AirSystem& air;

    UIScreen screen = UIScreen::TIRES;
    UIState state = UIState::NORMAL;

    float editingTarget = 35.0f;

    void startTargetOperation();

    bool frontSelected() const;
    bool rearSelected() const;
    bool allSelected() const;
};