// This is my custom config file, which will override higher level config files
#pragma once

// Override any existing settings
#undef MOUSEKEY_INTERVAL
#undef BACKLIGHT_BREATHING
#undef BREATHING_PERIOD

// Configure fine grained mouse control
#define MOUSEKEY_INTERVAL 1

// Setup RGB backlight breathing
#define BACKLIGHT_BREATHING
#define BREATHING_PERIOD 10
