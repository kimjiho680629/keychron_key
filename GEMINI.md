# Keychron Firmware Project Context & Guidelines

## 1. 프로젝트 개요
- **목적**: Keychron 기계식 키보드(Q, Q Pro/Max, V, V Pro/Max, K Pro/Max 등)의 오픈소스 QMK 펌웨어 소스코드 수정, 커스터마이징, 빌드 및 플래싱 환경
- **특수 기능**: [kimjiho680629/kbdfans_key](https://github.com/kimjiho680629/kbdfans_key) 기반 **Game 1~5 논블로킹 순환 연타 엔진 및 개별 RGB 피드백 시스템** 이식 및 통합
- **저장소 위치**: `/home/kjh/Projects/Gemini_Job/Keychron`
- **QMK 소스코드**: `/home/kjh/Projects/Gemini_Job/Keychron/qmk_firmware` (Keychron 공식 포크, `2025q3` 브랜치 기준)
- **QMK CLI Home**: `qmk config user.qmk_home=/home/kjh/Projects/Gemini_Job/Keychron/qmk_firmware`

## 2. 기술 스택 및 환경
- **운영체제**: Arch Linux (x86_64)
- **펌웨어 프레임워크**: QMK Firmware (Keychron fork, `2025q3`)
- **MCU 타겟**: ARM Cortex-M (STM32F4xx, STM32L4xx 등)
- **크로스 컴파일러 툴체인**:
  - `arm-none-eabi-gcc`
  - `arm-none-eabi-newlib`
  - `arm-none-eabi-binutils`
- **플래싱 도구**: `dfu-util` (직접 DFU 다운로드 지원: `dfu-util -a 0 -d 0483:df11 --dfuse-address 0x08000000:leave -D ...`)
- **CLI 도구**: `qmk` CLI (v1.2.0)

## 3. Game Turbo 특수 연타 엔진 사양
- **엔진 모듈 위치**: `qmk_firmware/keyboards/keychron/common/game_turbo/`
  - `game_turbo.h`: 키코드 상수(`0x7E40`~`0x7E44`, `0x7E0B`~`0x7E0F`, `0x7E00`~`0x7E04`, `F13`~`F17`), 함수 인터페이스 선언
  - `game_turbo.c`: 25ms 순환 연타, 50ms 마우스 좌클릭(`MS_BTN1`) 연타, 원본 사양 고유 RGB LED 색상 피드백
  - `game_turbo.mk`: `SRC += ...`, `MOUSEKEY_ENABLE = yes` 포함 빌드 파일
- **지원 키코드 및 개별 RGB LED 피드백 매핑**:
  - **Game 1 (`GAME_345` / `GM 345`)**: `3->4->5` (25ms 순환) / `0x7E0B` (`QK_KB_11`), `0x7E40` (`QK_USER`), `F13` / 🟧 **주황색 (Orange: HSV 28, 255, 255)**
  - **Game 2 (`GAME_2345` / `GM 2345`)**: `2->3->4->5` (25ms 순환) / `0x7E0C` (`QK_KB_12`), `0x7E41` (`QK_USER+1`), `F14` / 🟫 **브라운 (Brown: HSV 28, 255, 128)**
  - **Game 3 (`GAME_12345` / `GM 12345`)**: `1->2->3->4->5` (25ms 순환) / `0x7E0D` (`QK_KB_13`), `0x7E42` (`QK_USER+2`), `F15` / 🩵 **하늘색 (Cyan: HSV 128, 255, 255)**
  - **Game 4 (`GAME_45` / `GM 45`)**: `4->5` (25ms 순환) / `0x7E0E` (`QK_KB_14`), `0x7E43` (`QK_USER+3`), `F16` / 🔴 **빨간색 (Red: HSV 0, 255, 255)**
  - **Game 5 (`GAME_L_MOUSE` / `GM MOUSE`)**: 마우스 좌클릭 50ms 연타 / `0x7E0F` (`QK_KB_15`), `0x7E44` (`QK_USER+4`), `F17` / 🟣 **자홍색 (Magenta: HSV 213, 255, 255)**
- **토글(Toggle) 방식**: 1회 입력 시 시작(지정 색상 LED ON), 재입력 시 즉시 중지 및 소등(LED OFF, `rgb_matrix_disable_noeeprom()`)
- **키맵 적용 현황**:
  - `q2/ansi_encoder/keymaps/game_turbo`: CapsLock=한영키(`KC_HAEN`), Layer 4 W=`GM 345` (`0x7E0B`), E=`GM MOUSE` (`0x7E0F`), C/V=매크로0/1, 노브=볼륨/RGB
  - `q2_max/ansi_encoder/keymaps/game_turbo`
  - `q2_he/ansi_encoder/keymaps/game_turbo`

## 4. VIA 웹앱 및 키맵 백업 JSON 사양
- **VIA 키보드 정의(Definition) JSON**:
  - 파일 위치: `/home/kjh/Downloads/keychron_q2_game_turbo_via.json`, `/home/kjh/Projects/Gemini_Job/Keychron/keychron_q2_game_turbo_via.json`, `qmk_firmware/keyboards/keychron/q2/via_json/q2_ansi_encoder.json`
  - Keychron 순정 단축키 11종(0~10번)을 그대로 유지하고, 인덱스 11~15번에 Game 1~5(`GM 345`, `GM 2345`, `GM 12345`, `GM 45`, `GM MOUSE`)를 등록하여 인덱스 충돌 방지 및 VIA `CUSTOM` 탭/키캡 라벨 완벽 렌더링 지원.
  - 최신 VIA v3 스펙에 맞춰 `"keycodes": ["qmk_rgb_matrix"]`로 모듈을 명시 지정하여 `qmk_lighting` 호환성 경고 문구 완전 제거.
- **키맵 백업 JSON**:
  - 파일 위치: `/home/kjh/Downloads/Keymap-Keychron Q2 ANSI Knob-GameTurbo.json`, `/home/kjh/Downloads/Keymap-Keychron Q2 ANSI Knob-13-16-41.json`
  - 5번째 레이어(Layer 4 / Fn2 레이어)의 W=`32267` (`QK_KB_11`), E=`32271` (`QK_KB_15`)로 영구 지정.
  - 적용 방법: VIA 상단 `CONFIGURE` ➔ 좌측 사이드바 `SAVE + LOAD` ➔ `Load Saved Keymap`에서 업로드하여 복원.

## 5. 주요 명령어 및 스크립트
- **환경 구축 및 의존성 설치**:
  ```bash
  ./setup_env.sh
  ```
- **키보드 모델 목록 조회**:
  ```bash
  ./build_fw.sh list [모델키워드]
  ```
- **Game Turbo 펌웨어 컴파일**:
  ```bash
  ./build_fw.sh compile keychron/q2/ansi_encoder game_turbo
  ./build_fw.sh compile keychron/q2_max/ansi_encoder game_turbo
  ./build_fw.sh compile keychron/q2_he/ansi_encoder game_turbo
  ```
- **펌웨어 플래싱 (스마트 DFU 자동 감지 및 고속 플래싱)**:
  ```bash
  ./build_fw.sh flash keychron/q2/ansi_encoder game_turbo
  ```
- **빌드 캐시 정리**:
  ```bash
  ./build_fw.sh clean
  ```
- **소프트웨어 부트로더 점프 헬퍼**:
  ```bash
  ./enter_bootloader.py
  ```

## 6. 소스코드 구조 및 개발 컨벤션
- **하드웨어 스위치 및 레이어 규칙**:
  - 65%/60% 계열(Q2, V2 등)은 물리 스위치가 Mac 위치일 때 Layer 0, Windows 위치일 때 Layer 1로 전환됨 (`active ? 1 : 0`).
  - Layer 4(`_FN3` 또는 `FN2`)는 물리적 F1~F12 펑션키 전용 레이어로 할당되어 있음.
- **연타 엔진 확장 컨벤션**:
  - 신규 키보드 모델에 연타 엔진 탑재 시 `rules.mk`에 `include keyboards/keychron/common/game_turbo/game_turbo.mk`를 포함하고, `keymap.c`의 `process_record_user`와 `matrix_scan_user`에서 각각 `process_game_turbo` 및 `matrix_scan_game_turbo`를 호출할 것.
