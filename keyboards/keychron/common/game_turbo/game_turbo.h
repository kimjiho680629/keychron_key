/* Copyright 2026 @ kimjiho680629 / Keychron QMK Game Turbo Engine
 *
 * 이 헤더 파일은 KBDFans (kimjiho680629/kbdfans_key)의 Game 1 ~ Game 5
 * 초고속 논블로킹 순환 연타 엔진 및 RGB LED 피드백 사양을 Keychron 키보드용으로 이식한 모듈입니다.
 */

#pragma once

#include QMK_KEYBOARD_H

/* Game Turbo 커스텀 키코드 정의 (0x7E40 ~ 0x7E44) */
enum game_turbo_keycodes {
    GAME_345 = QK_USER, // USER00 (0x7E40): 345 순환 연타 (25ms) -> 주황색 (Orange)
    GAME_2345,          // USER01 (0x7E41): 2345 순환 연타 (25ms) -> 브라운 (Brown)
    GAME_12345,         // USER02 (0x7E42): 12345 순환 연타 (25ms) -> 하늘색 (Cyan)
    GAME_45,            // USER03 (0x7E43): 45 순환 연타 (25ms) -> 빨간색 (Red)
    GAME_L_MOUSE,       // USER04 (0x7E44): 마우스 좌클릭 황금 줍기 (50ms) -> 자홍색 (Magenta)
    GAME_TURBO_SAFE_RANGE
};

/* VIA v3 CUSTOM 탭 키코드 상수 정의 */
#ifndef QK_KB_0
#    define QK_KB_0 0x7E00
#endif
#ifndef QK_KB_1
#    define QK_KB_1 0x7E01
#endif
#ifndef QK_KB_2
#    define QK_KB_2 0x7E02
#endif
#ifndef QK_KB_3
#    define QK_KB_3 0x7E03
#endif
#ifndef QK_KB_4
#    define QK_KB_4 0x7E04
#endif

/* Keychron VIA 순정 customKeycodes 뒤에 추가되는 인덱스 (11~15) */
#ifndef QK_KB_11
#    define QK_KB_11 0x7E0B
#endif
#ifndef QK_KB_12
#    define QK_KB_12 0x7E0C
#endif
#ifndef QK_KB_13
#    define QK_KB_13 0x7E0D
#endif
#ifndef QK_KB_14
#    define QK_KB_14 0x7E0E
#endif
#ifndef QK_KB_15
#    define QK_KB_15 0x7E0F
#endif

/* Game Turbo 엔진 인터페이스 함수 */
bool process_game_turbo(uint16_t keycode, keyrecord_t *record);
void matrix_scan_game_turbo(void);
uint8_t get_active_game_turbo(void);
void stop_game_turbo(void);
void set_game_turbo_led_color(uint8_t game);
bool rgb_matrix_indicators_game_turbo(void);
