#ifdef OLED_ENABLE
bool oled_task_user(void) {
    switch (get_highest_layer(layer_state)) {
        case _0:
            oled_write_P(PSTR("0\n"), false);
            break;
        case _1:
            oled_write_P(PSTR(" 1\n"), false);
            break;
        case _2:
            oled_write_P(PSTR("  2\n"), false);
            break;
        case _3:
            oled_write_P(PSTR("  3\n"), false);
            break;
        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
            oled_write_ln_P(PSTR("¯\_(ツ)_/¯"), false);
    }
    return false;
}
#endif