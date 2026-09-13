/* Copyright 2026 @ kimjiho680629 / Keychron Q2 HE Game Turbo Keymap
 *
 * Game 1 ~ Game 5 초고속 논블로킹 순환 연타 엔진 및 RGB LED 피드백 지원
 */

#include QMK_KEYBOARD_H
#include "keychron_common.h"
#include "game_turbo.h"

enum layers {
    MAC_BASE,
    MAC_FN1,
    WIN_BASE,
    WIN_FN1,
    FN2,
};

#define FN1_MAC MO(MAC_FN1)
#define FN1_WIN MO(WIN_FN1)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [MAC_BASE] = LAYOUT_ansi_66(
        KC_ESC,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,            KC_MUTE,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,            KC_DEL,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,             KC_HOME,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,  KC_UP,
        KC_LCTL,  KC_LOPTN, KC_LCMMD,                               KC_SPC,                                 KC_RCMMD, FN1_MAC,  MO(FN2),  KC_LEFT,  KC_DOWN,  KC_RGHT),

    /* Mac FN1 레이어: Fn1 + 1~5 키로 Game 3, 2, 1, 4, 5 (마우스) 즉시 실행 */
    [MAC_FN1] = LAYOUT_ansi_66(
        KC_GRV,   GAME_12345, GAME_2345, GAME_345, GAME_45,  GAME_L_MOUSE, UG_VALU,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  _______,            UG_TOGG,
        _______,  BT_HST1,    BT_HST2,   BT_HST3,  P2P4G,    _______,      _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
        UG_TOGG,  UG_NEXT,    UG_VALU,   UG_HUEU,  UG_SATU,  UG_SPDU,      _______,  _______,  _______,  _______,  _______,  _______,            _______,            KC_END,
        _______,              UG_PREV,   UG_VALD,  UG_HUED,  UG_SATD,      UG_SPDD,  _______,  _______,  _______,  _______,  _______,            _______,  _______,
        _______,  _______,    _______,                                      _______,                                _______,  _______,  _______,  _______,  _______,  _______),

    [WIN_BASE] = LAYOUT_ansi_66(
        KC_ESC,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,            KC_MUTE,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,            KC_DEL,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,             KC_HOME,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,  KC_UP,
        KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                                 KC_RALT,  FN1_WIN,  MO(FN2),  KC_LEFT,  KC_DOWN,  KC_RGHT),

    /* Win FN1 레이어: Fn1 + 1~5 키로 Game 3, 2, 1, 4, 5 (마우스) 즉시 실행 */
    [WIN_FN1] = LAYOUT_ansi_66(
        KC_GRV,   GAME_12345, GAME_2345, GAME_345, GAME_45,  GAME_L_MOUSE, UG_VALU,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  _______,            UG_TOGG,
        _______,  BT_HST1,    BT_HST2,   BT_HST3,  P2P4G,    _______,      _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
        UG_TOGG,  UG_NEXT,    UG_VALU,   UG_HUEU,  UG_SATU,  UG_SPDU,      _______,  _______,  _______,  _______,  _______,  _______,            _______,            KC_END,
        _______,              UG_PREV,   UG_VALD,  UG_HUED,  UG_SATD,      UG_SPDD,  _______,  _______,  _______,  _______,  _______,            _______,  _______,
        _______,  _______,    _______,                                      _______,                                _______,  _______,  _______,  _______,  _______,  _______),

    /* F1~F12 공통 펑션키 레이어 */
    [FN2] = LAYOUT_ansi_66(
        _______,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   _______,            _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,            _______,
        _______,            _______,  _______,  _______,  _______,  BAT_LVL,  _______,  _______,  _______,  _______,  _______,            _______,  _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  _______,  _______)
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [MAC_BASE] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [MAC_FN1]  = { ENCODER_CCW_CW(UG_VALD, UG_VALU) },
    [WIN_BASE] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [WIN_FN1]  = { ENCODER_CCW_CW(UG_VALD, UG_VALU) },
    [FN2]      = { ENCODER_CCW_CW(_______, _______) },
};
#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_game_turbo(keycode, record)) {
        return false;
    }
    return true;
}

void matrix_scan_user(void) {
    matrix_scan_game_turbo();
}
