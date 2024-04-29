/* Copyright 2022 DOIO
 * Copyright 2022 HorrorTroll <https://github.com/HorrorTroll>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H

// OLED animation
#include "lib/layer_status/layer_status.h"

// I added this so that I could extend the timeout of the OLED display
// It didn't work, so I'm commenting it out 
// #include "quantum.h"
// #include "timer.h"
// static uint32_t oled_dim_timer = 0;
// 
// void matrix_scan_user(void) {
//     if (oled_dim_timer != 0 && timer_elapsed(oled_dim_timer) > 300000) { // 300000 ms = 5 minutes
//         oled_off();
//         oled_dim_timer = 0; // Reset the timer
//     }
// }
// 
// void suspend_power_down_user(void) {
//     #ifdef OLED_ENABLE
//     oled_set_brightness(127); // Set brightness to about 50%
//     oled_dim_timer = timer_read(); // Start the dimming timer
//     #endif
//     // Add other suspend behaviors if needed
// }

enum layer_names {
    _1,
    _2,
    _3,
    _4
};

enum custom_keycodes {
    TOGGLE_HEADING_2 = SAFE_RANGE,
	CONTROL_OR_COPY,
	SHIFT_OR_CUT,
	NOTION_CALLOUT,
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint16_t control_timer;
	static uint16_t shift_timer;
	
	switch (keycode) {
        case TOGGLE_HEADING_2:
            if (record->event.pressed) {
                SEND_STRING("> ## ");
            }
            return false; // Skip all further processing of this key

        case CONTROL_OR_COPY:
            if (record->event.pressed) {
                // Key pressed
                control_timer = timer_read(); // Start the timer
                register_mods(MOD_BIT(KC_LCTL)); // Register Left Control as being held down
            } else {
                // Key released
                unregister_mods(MOD_BIT(KC_LCTL)); // Unregister Left Control
                if (timer_elapsed(control_timer) < TAPPING_TERM) {
                    // If the key was released before the tapping term expires, send Control-C
                    tap_code16(LCTL(KC_C));
                }
            }
            return false; // Skip all further processing of this key

        case SHIFT_OR_CUT:
            if (record->event.pressed) {
                // Key pressed
                shift_timer = timer_read(); // Start the timer
                register_mods(MOD_BIT(KC_LSFT)); // Register Left Shift as being held down
            } else {
                // Key released
                unregister_mods(MOD_BIT(KC_LSFT)); // Unregister Left Shift
                if (timer_elapsed(shift_timer) < TAPPING_TERM) {
                    // If the key was released before the tapping term expires, send Control-X
                    tap_code16(LCTL(KC_X));
                }
            }
            return false; // Skip all further processing of this key

        case NOTION_CALLOUT:
            if (record->event.pressed) {
                SEND_STRING("/callout\n");
            }
            return false; // Skip all further processing of this key
    }
    return true; // Process all other keys normally
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/*
       ┌───┬───┬───┬───┐ ┌───┐
       │   │   │   │   │ │   │
       ├───┼───┼───┼───┤ └───┘     ┌───┐
       │   │   │   │   │           │   │
       ├───┼───┼───┼───┤      ┌───┐└───┘
       │   │   │   │   │      │   │
       ├───┼───┼───┼───┤      └───┘
       │   │   │   │   │
       └───┴───┴───┴───┘
*/
	/* Layer 1 (Windows)                                                                   */
    /*  Column: 1        2                 3                 4              5 (push dial)  */
    [_1] = LAYOUT(
		LALT_T(KC_ESC),  KC_TAB,           LSFT(KC_TAB),     KC_NO,         TO(_4),
		LSG(KC_S),       KC_NO,            RCS(KC_T),        LCTL(KC_T),    TO(_2),
		LCAG(KC_V),      RCS(KC_TAB),      LCTL(KC_W),       LCTL(KC_TAB),  KC_MUTE,
		CONTROL_OR_COPY, SHIFT_OR_CUT,     LGUI(KC_V),       LCTL(KC_V)
	),

    /* Layer 2 (Numpad)                                                                    */
    /*  Column: 1        2                 3                 4              5 (push dial)  */
    [_2] = LAYOUT(
		KC_TRNS,         KC_7,             KC_8,             KC_9,          TO(_1),
		KC_TRNS,         KC_4,             KC_5,             KC_6,          TO(_3),
		KC_TRNS,         KC_1,             KC_2,             KC_3,          KC_TRNS,
		KC_TRNS,         KC_0,             KC_TAB,           KC_ENT
	),

    /* Layer 3 (Notion)                                                                    */
    /*  Column: 1        2                 3                 4              5 (push dial)  */
    [_3] = LAYOUT(
		KC_TRNS,         KC_TAB,           LSFT(KC_TAB),     KC_NO,         TO(_2),
		KC_TRNS,         LALT(KC_K),       LCTL(KC_P),       LALT(KC_J),    TO(_4),
		KC_TRNS,         LCTL(KC_LBRC),    TOGGLE_HEADING_2, LCTL(KC_RBRC), KC_TRNS,
		KC_TRNS,         SHIFT_OR_CUT,     LGUI(KC_V),       LCTL(KC_V)
	),

    /* Layer 4 (RGB Customizations)                                                        */
    /*  Column: 1        2                 3                 4              5 (push dial)  */
    [_4] = LAYOUT(
		KC_TRNS,         KC_TAB,           LSFT(KC_TAB),     KC_NO,         TO(_3),
		RGB_RMOD,        RGB_MOD,          EE_CLR,           QK_BOOT,       TO(_1),
		RGB_SPD,         RGB_SPI,          RGB_SAD,          RGB_SAI,       KC_TRNS,
		RGB_HUD,         RGB_HUI,          RGB_VAD,          RGB_VAI
	),
};

#ifdef OLED_ENABLE
    bool oled_task_user(void) {
        render_layer_status();
        return true;
    }
#endif

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
	/* Rotary Encoders (dials)                                                                                    */
	/*       Top-left                          Top-right                         Bottom                           */
	/* Functions (same for every layer due to use of KC_TRNS on layers 2-4)                                       */
	/*       Mouse left, mouse right           Mouse down, mouse up              Speaker volume down, up          */
    [_1] = { ENCODER_CCW_CW(KC_MS_L, KC_MS_R), ENCODER_CCW_CW(KC_MS_D, KC_MS_U), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_2] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [_3] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [_4] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
};
#endif