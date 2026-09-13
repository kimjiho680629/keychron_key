/* Copyright 2026 @ kimjiho680629 / Keychron QMK Game Turbo Engine
 *
 * 이 소스 파일은 KBDFans (kimjiho680629/kbdfans_key)의 Game 1 ~ Game 5
 * 초고속 논블로킹 순환 연타 엔진 및 RGB LED 피드백 사양을 Keychron 키보드용으로 구현한 모듈입니다.
 */

#include "game_turbo.h"

static uint8_t  active_game = 0;
static uint8_t  game_step   = 0;
static uint32_t game_timer  = 0;

static const uint16_t seq_345[]   = {KC_3, KC_4, KC_5};
static const uint16_t seq_2345[]  = {KC_2, KC_3, KC_4, KC_5};
static const uint16_t seq_12345[] = {KC_1, KC_2, KC_3, KC_4, KC_5};
static const uint16_t seq_45[]    = {KC_4, KC_5};

uint8_t get_active_game_turbo(void) {
    return active_game;
}

void stop_game_turbo(void) {
    active_game = 0;
    game_step   = 0;
    set_game_turbo_led_color(0);
}

void set_game_turbo_led_color(uint8_t game) {
#if defined(RGB_MATRIX_ENABLE)
    if (game == 0) {
        /* 키 중지 시: 모든 LED 소등(OFF) */
        rgb_matrix_disable_noeeprom();
    } else {
        rgb_matrix_enable_noeeprom();
        rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
        switch (game) {
            case 1: // GAME 1 (345): 주황색 (Orange: H28, S255, V255)
                rgb_matrix_sethsv_noeeprom(28, 255, 255);
                break;
            case 2: // GAME 2 (2345): 브라운색 (Brown: H28, S255, V128)
                rgb_matrix_sethsv_noeeprom(28, 255, 128);
                break;
            case 3: // GAME 3 (12345): 하늘색 (Cyan: H128, S255, V255)
                rgb_matrix_sethsv_noeeprom(128, 255, 255);
                break;
            case 4: // GAME 4 (45): 빨간색 (Red: H0, S255, V255)
                rgb_matrix_sethsv_noeeprom(0, 255, 255);
                break;
            case 5: // GAME 5 (L_MOUSE): 자홍색/마젠타 (Magenta: H213, S255, V255)
                rgb_matrix_sethsv_noeeprom(213, 255, 255);
                break;
        }
    }
#endif
}

void matrix_scan_game_turbo(void) {
    if (active_game == 0) return;

    if (active_game == 5) {
        /* GAME_L_MOUSE: 마우스 좌클릭 (MS_BTN1) 50ms 황금 아이템 줍기 속도 (초당 20회) */
#if defined(MOUSEKEY_ENABLE)
        if (timer_elapsed(game_timer) >= 50) {
            game_timer = timer_read();
            tap_code16(MS_BTN1);
        }
#endif
    } else {
        /* GAME 1~4: 25ms 순환 연타 */
        if (timer_elapsed(game_timer) >= 25) {
            game_timer = timer_read();
            switch (active_game) {
                case 1: // 345 연타
                    tap_code16(seq_345[game_step]);
                    game_step = (game_step + 1) % (sizeof(seq_345) / sizeof(seq_345[0]));
                    break;
                case 2: // 2345 연타
                    tap_code16(seq_2345[game_step]);
                    game_step = (game_step + 1) % (sizeof(seq_2345) / sizeof(seq_2345[0]));
                    break;
                case 3: // 12345 연타
                    tap_code16(seq_12345[game_step]);
                    game_step = (game_step + 1) % (sizeof(seq_12345) / sizeof(seq_12345[0]));
                    break;
                case 4: // 45 연타
                    tap_code16(seq_45[game_step]);
                    game_step = (game_step + 1) % (sizeof(seq_45) / sizeof(seq_45[0]));
                    break;
            }
        }
    }
}

bool process_game_turbo(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) return true;

    switch (keycode) {
        case GAME_345: // 0x7E40 (QK_USER)
        case QK_KB_0:  // 0x7E00 (VIA v3 CUSTOM 탭 1번 버튼 - KBDFans 호환)
        case QK_KB_11: // 0x7E0B (Keychron VIA CUSTOM 탭 12번 버튼 - GM 345)
        case 0x5F80:   // VIA v2 CUSTOM 탭 호환
        case 0x7F00:   // VIA v11 CUSTOM 탭 호환
        case KC_F13:   // VIA SPECIAL 탭 [F13] 호환
            active_game = (active_game == 1) ? 0 : 1;
            game_step   = 0;
            game_timer  = timer_read();
            set_game_turbo_led_color(active_game);
            return false;

        case GAME_2345: // 0x7E41 (QK_USER + 1)
        case QK_KB_1:   // 0x7E01 (VIA v3 CUSTOM 탭 2번 버튼)
        case QK_KB_12:  // 0x7E0C (Keychron VIA CUSTOM 탭 13번 버튼 - GM 2345)
        case 0x5F81:    // VIA v2 CUSTOM 탭 호환
        case 0x7F01:    // VIA v11 CUSTOM 탭 호환
        case KC_F14:    // VIA SPECIAL 탭 [F14] 호환
            active_game = (active_game == 2) ? 0 : 2;
            game_step   = 0;
            game_timer  = timer_read();
            set_game_turbo_led_color(active_game);
            return false;

        case GAME_12345: // 0x7E42 (QK_USER + 2)
        case QK_KB_2:    // 0x7E02 (VIA v3 CUSTOM 탭 3번 버튼)
        case QK_KB_13:   // 0x7E0D (Keychron VIA CUSTOM 탭 14번 버튼 - GM 12345)
        case 0x5F82:     // VIA v2 CUSTOM 탭 호환
        case 0x7F02:     // VIA v11 CUSTOM 탭 호환
        case KC_F15:     // VIA SPECIAL 탭 [F15] 호환
            active_game = (active_game == 3) ? 0 : 3;
            game_step   = 0;
            game_timer  = timer_read();
            set_game_turbo_led_color(active_game);
            return false;

        case GAME_45: // 0x7E43 (QK_USER + 3)
        case QK_KB_3:  // 0x7E03 (VIA v3 CUSTOM 탭 4번 버튼)
        case QK_KB_14: // 0x7E0E (Keychron VIA CUSTOM 탭 15번 버튼 - GM 45)
        case 0x5F83:   // VIA v2 CUSTOM 탭 호환
        case 0x7F03:   // VIA v11 CUSTOM 탭 호환
        case KC_F16:   // VIA SPECIAL 탭 [F16] 호환
            active_game = (active_game == 4) ? 0 : 4;
            game_step   = 0;
            game_timer  = timer_read();
            set_game_turbo_led_color(active_game);
            return false;

        case GAME_L_MOUSE: // 0x7E44 (QK_USER + 4)
        case QK_KB_4:      // 0x7E04 (VIA v3 CUSTOM 탭 5번 버튼)
        case QK_KB_15:     // 0x7E0F (Keychron VIA CUSTOM 탭 16번 버튼 - GM MOUSE)
        case 0x5F84:       // VIA v2 CUSTOM 탭 호환
        case 0x7F04:       // VIA v11 CUSTOM 탭 호환
        case KC_F17:       // VIA SPECIAL 탭 [F17] 호환
            active_game = (active_game == 5) ? 0 : 5;
            game_step   = 0;
            game_timer  = timer_read();
            set_game_turbo_led_color(active_game);
            return false;
    }

    return true;
}
