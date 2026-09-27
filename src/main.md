#include <Arduino.h>
#include <Arduino_GFX_Library.h>

Arduino_DataBus *bus = new Arduino_SWSPI(
    GFX_NOT_DEFINED, // DC
    1,               // CS
    46,              // SCK
    0,               // MOSI
    GFX_NOT_DEFINED  // MISO
);

Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
    2,  // DE
    42, // VSYNC
    3,  // HSYNC
    45, // PCLK

    // RGB pins
    4, 41, 5, 40, 6,
    39, 7, 47, 8, 48, 9,
    11, 15, 12, 16, 21,

    // Timing
    1, 10, 8, 50,
    1, 10, 8, 20
);

Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
    480,
    480,
    rgbpanel,
    0,
    true,
    bus,
    GFX_NOT_DEFINED,
    st7701_type5_init_operations,
    sizeof(st7701_type5_init_operations)
);

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println("Tacoma Onboard Air booting...");

    if (!gfx->begin()) {
        Serial.println("Display initialization failed!");
        return;
    }

    gfx->fillScreen(BLACK);

    gfx->setTextColor(WHITE);
    gfx->setTextSize(3);
    gfx->setCursor(115, 210);
    gfx->println("TACOMA AIR");

    gfx->setTextSize(2);
    gfx->setCursor(155, 250);
    gfx->println("BOOT OK");

    Serial.println("Display initialized.");
}

void loop()
{
}