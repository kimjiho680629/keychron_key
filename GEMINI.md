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
  - `q2/ansi_encoder/keymaps/game_turbo`:
    - CapsLock 위치: 단독 한/영 전환키(`KC_HAEN`)
    - Layer 0 (Mac) & Layer 1 (Windows): 노브 회전=볼륨 업/다운(`KC_VOLU`, `KC_VOLD`), 클릭=`KC_MUTE`
    - Layer 2 (Mac Fn) & Layer 3 (Win Fn): 노브 회전=RGB 밝기 조절(`RM_VALU`, `RM_VALD`), 클릭=`RM_TOGG`
    - Layer 4 (Fn2 / `_FN3`): F1~F12, W=`GM 345` (`QK_KB_0`), E=`GM MOUSE` (`QK_KB_4`), C/V=매크로0/1, 노브=볼륨/음소거 명시적 고정
  - `q2_max/ansi_encoder/keymaps/game_turbo`
  - `q2_he/ansi_encoder/keymaps/game_turbo`

## 4. 로터리 인코더(Knob) 하드웨어 드라이버 및 EEPROM 관리
- **QMK 빌드 필수 규칙**:
  - QMK 빌드 파이프라인(`builddefs/common_features.mk`) 구조상 `ENCODER_MAP_ENABLE`만 켜져 있으면 하위 드라이버가 빌드되지 않고 탈락함.
  - 반드시 **`rules.mk`에 `ENCODER_ENABLE = yes`를 명시적으로 선언**해야 MCU 하드웨어 Quadrature 인터럽트 드라이버와 VIA 동적 인코더 API(`dynamic_keymap_get_encoder`, `dynamic_keymap_set_encoder`)가 바이너리에 정상 포함됨.
- **EEPROM Dynamic Keymap 인코더 동기화**:
  - QMK VIA는 펌웨어 재플래싱 시 기존 EEPROM을 덮어쓰지 않고 보존하므로, 드라이버를 새로 활성화한 직후에는 EEPROM 상의 인코더 영역에 미초기화 데이터가 남아있을 수 있음.
  - 전용 복구 스크립트 `./fix_encoder_eeprom.py`를 실행하여 Raw HID를 통해 모든 레이어의 인코더 볼륨/RGB 매핑을 1초 만에 즉시 동기화 가능.

## 5. VIA 웹앱 및 직설적 키보드 정의 JSON 사양
- **VIA 키보드 정의(Definition) JSON**:
  - 파일 위치:
    - `/home/kjh/Downloads/VIA_디자인탭_Keychron_Q2_정의.json` (직설적 한글 파일명)
    - `/home/kjh/Projects/Gemini_Job/Keychron/VIA_디자인탭_Keychron_Q2_정의.json`
    - `/home/kjh/Projects/Gemini_Job/Keychron/keychron_q2_via_definition.json`
    - `qmk_firmware/keyboards/keychron/q2/via_json/q2_ansi_encoder.json`
  - **CUSTOM(사용자 지정) 탭 전면 최적화**: Keychron 순정 단축키 대신 **Game 1~5(`GM 345`, `GM 2345`, `GM 12345`, `GM 45`, `GM MOUSE`)를 0~4번 인덱스에 단독 배치**하여, 사용자 지정 탭을 열었을 때 첫 줄에 5대 핵심 게임키가 큼직하게 렌더링되도록 구현.
  - **로터리 인코더(Knob) UI 완벽 결속**: 우측 상단 노브 키에 `"0,14\n\n\n\n\n\n\n\n\ne0"` (스위치 클릭 + Rotary Encoder 0 바인딩)을 적용하여 VIA 웹앱에서 노브 회전 매핑 슬롯 활성화.
  - **무경고(Zero Warning) 초경량 구조**: 경고를 유발하는 구형 조명 메뉴를 제거하고 `"keycodes": ["qmk_rgb_matrix_keycodes"]`로 표준화하여 디자인 탭 업로드 시 어떠한 에러나 노란 경고도 없이 100% 정상 로드.
- **키맵 기본 내장**:
  - 펌웨어 기본 키맵(Layer 4 / Fn2 레이어)의 W=`QK_KB_0`(`GM 345`), E=`QK_KB_4`(`GM MOUSE`)로 기본 하드코딩되어 있어, 별도의 키맵 백업 JSON 복원 작업 없이 정의 파일 로드만으로 즉시 사용 가능.
  - 구버전 키맵 백업 파일들은 다운로드 폴더 내 `keymap_backup/` 폴더로 격리 보관.

## 6. 주요 명령어 및 스크립트
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
- **로터리 인코더(Knob) EEPROM 자동 동기화**:
  ```bash
  ./fix_encoder_eeprom.py
  ```
- **빌드 캐시 정리**:
  ```bash
  ./build_fw.sh clean
  ```
- **소프트웨어 부트로더 점프 헬퍼**:
  ```bash
  ./enter_bootloader.py
  ```

## 7. 소스코드 구조 및 개발 컨벤션
- **하드웨어 스위치 및 레이어 규칙**:
  - 65%/60% 계열(Q2, V2 등)은 물리 스위치가 Mac 위치일 때 Layer 0, Windows 위치일 때 Layer 1로 전환됨 (`active ? 1 : 0`).
  - Layer 4(`_FN3` 또는 `FN2`)는 물리적 F1~F12 펑션키 전용 레이어로 할당되어 있음.
- **연타 엔진 확장 컨벤션**:
  - 신규 키보드 모델에 연타 엔진 탑재 시 `rules.mk`에 `ENCODER_ENABLE = yes`, `include keyboards/keychron/common/game_turbo/game_turbo.mk`를 포함하고, `keymap.c`의 `process_record_user`, `matrix_scan_user`, `rgb_matrix_indicators_user`에서 각각 Game Turbo 핸들러를 호출할 것.
