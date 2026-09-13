/* Copyright 2026 @ kimjiho680629 / Keychron Q2 Game Turbo Keymap
 *
 * Game 1 ~ Game 5 초고속 논블로킹 순환 연타 엔진 및 빨간색(Red) 통일 LED 피드백 지원
 * 사용자의 Keymap JSON 커스텀 설정을 영구 기본 키맵으로 통합
 */

#include QMK_KEYBOARD_H
#include "game_turbo.h"

enum layers {
    MAC_BASE,
    WIN_BASE,
    _FN1,
    _FN2,
    _FN3
};

#ifndef KC_HAEN
#    define KC_HAEN KC_LNG1
#endif

#define KC_TASK LGUI(KC_TAB)
#define KC_FLXP LGUI(KC_E)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Layer 0: macOS 기본 레이아웃 (CapsLock 위치: KC_HAEN 한/영키) */
    [MAC_BASE] = LAYOUT_ansi_67(
        KC_ESC,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,  KC_MUTE,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,  KC_DEL,
        KC_HAEN,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,   KC_HOME,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,  KC_UP,
        KC_LCTL,  KC_LOPT,  KC_LCMD,                              KC_SPC,                             KC_RCMD,  MO(_FN1), MO(_FN3), KC_LEFT,  KC_DOWN,  KC_RGHT
    ),

    /* Layer 1: Windows 기본 레이아웃 (CapsLock 위치: KC_HAEN 한/영키) */
    [WIN_BASE] = LAYOUT_ansi_67(
        KC_ESC,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,  KC_MUTE,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,  KC_DEL,
        KC_HAEN,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,   KC_HOME,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,  KC_UP,
        KC_LCTL,  KC_LGUI,  KC_LALT,                              KC_SPC,                             KC_RALT,  MO(_FN2), MO(_FN3), KC_LEFT,  KC_DOWN,  KC_RGHT
    ),

    /* Layer 2: Mac FN1 미디어 및 RGB 조절 레이어 */
    [_FN1] = LAYOUT_ansi_67(
        KC_GRV,   KC_BRID,  KC_BRIU,  KC_MCTL,  KC_LPAD,  RM_VALD,  RM_VALU,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  _______,  RM_TOGG,
        RM_TOGG,  RM_NEXT,  RM_VALU,  RM_HUEU,  RM_SATU,  RM_SPDU,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  RM_PREV,  RM_VALD,  RM_HUED,  RM_SATD,  RM_SPDD,  _______,  _______,  _______,  _______,  _______,  _______,            _______,  _______,
        _______,            _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,  _______,
        _______,  _______,  _______,                              _______,                            _______,  _______,  _______,  _______,  _______,  _______
    ),

    /* Layer 3: Windows FN2 미디어 및 단축키(Task View, File Explorer), RGB 조절 레이어 */
    [_FN2] = LAYOUT_ansi_67(
        KC_GRV,   KC_BRID,  KC_BRIU,  KC_TASK,  KC_FLXP,  RM_VALD,  RM_VALU,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  _______,  RM_TOGG,
        RM_TOGG,  RM_NEXT,  RM_VALU,  RM_HUEU,  RM_SATU,  RM_SPDU,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
        _______,  RM_PREV,  RM_VALD,  RM_HUED,  RM_SATD,  RM_SPDD,  _______,  _______,  _______,  _______,  _______,  _______,            _______,  _______,
        _______,            _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,  _______,
        _______,  _______,  _______,                              _______,                            _______,  _______,  _______,  _______,  _______,  _______
    ),

    /* Layer 4: FN3 레이어 (F1~F12, W=Game 1(GM 345), E=Game 5(GM MOUSE), 매크로0, 매크로1 탑재) */
    [_FN3] = LAYOUT_ansi_67(
        QK_BOOT,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   _______,  _______,
        RM_TOGG,  QK_KB_11, QK_KB_15, RM_HUEU,  RM_SATU,  RM_SPDU,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
        KC_CAPS,  RM_PREV,  RM_VALD,  RM_HUED,  RM_SATD,  RM_SPDD,  _______,  _______,  _______,  _______,  _______,  _______,            _______,  _______,
        _______,            _______,  _______,  QK_MACRO_0, QK_MACRO_1, _______, _______, _______, _______, _______, _______,         _______,  _______,
        _______,  _______,  _______,                              _______,                            _______,  _______,  _______,  _______,  _______,  _______
    )
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [MAC_BASE] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [WIN_BASE] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_FN1]     = { ENCODER_CCW_CW(RM_VALD, RM_VALU) },
    [_FN2]     = { ENCODER_CCW_CW(RM_VALD, RM_VALU) },
    [_FN3]     = { ENCODER_CCW_CW(_______, _______) },
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
