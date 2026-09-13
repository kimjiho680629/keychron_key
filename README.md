# Keychron 키보드 QMK 펌웨어 & Game Turbo 특수 연타 엔진

Keychron(키크론) 기계식 키보드를 위한 고성능 QMK 커스텀 펌웨어 및 **Game 1~5 초고속 논블로킹 순환 연타 엔진 & 하드웨어 레벨 실시간 RGB 피드백 시스템** 올인원 배포판입니다.  
[kimjiho680629/kbdfans_key](https://github.com/kimjiho680629/kbdfans_key)의 검증된 연타 엔진 사양을 Keychron STM32/ARM 하드웨어에 완벽 이식하고 최신 VIA v3 스펙에 맞춰 최적화하였습니다.

---

## 🌟 주요 특징 및 기술적 개선

- **초고속 논블로킹(Non-blocking) 타이머 엔진**: 일반 `wait_ms()` 기반 매크로와 달리 키보드 전체 입력 반응성을 1ms도 저하시키지 않고 백그라운드에서 매끄럽게 동작합니다.
- **QMK 표준 인디케이터(`rgb_matrix_indicators_user`) 기반 절대 색상 고정**:
  - 기존 일회성 HSV 설정 시 QMK 렌더링 파이프라인이 EEPROM의 기본값(빨간색)으로 덮어쓰던 버그를 완벽하게 해결하였습니다.
  - 프레임 플러시 직전 하드웨어 PWM 레벨에서 색상을 직접 주입하여 **주황색, 브라운, 하늘색, 빨간색, 자홍색** 고유 색상을 100% 무결하게 유지합니다.
- **토글(Toggle) 구동 방식**: 1회 누르면 즉시 연타 및 LED ON ➔ 다시 누르면 즉시 정지 및 완전 소등(OFF).
- **초경량 노-워닝(Zero Warning) VIA v3 정의 파일 탑재**:
  - 사용자 지정(`CUSTOM`) 탭 클릭 시 불필요한 맥 단축키 없이 **Game 1~5 연타키 5종이 첫 줄에 큼직하게 단독 렌더링**됩니다.
  - 경고를 유발하는 구형 조명 메뉴를 제거하고 `"qmk_rgb_matrix_keycodes"` 표준을 적용하여 **디자인 탭에서 단 1개의 경고/에러도 없이 깔끔하게 로드**됩니다.
- **Keychron 순정 기능 100% 보존**: 볼륨 노브, 한영키(`KC_HAEN`), Mac/Windows 하드웨어 토글 스위치가 순정 그대로 정상 작동합니다.

---

## 🎮 Game Turbo 연타 엔진 상세 사양

모든 게임키는 **토글(Toggle)** 방식으로 작동하며, 시작 시 지정된 고유 색상 LED가 점등되고 정지 시 소등됩니다.

| 게임키 명칭 | VIA CUSTOM 탭 버튼 | VIA 키코드 (신규 표준) | Any 직접입력 코드 | 대체 단축키 | 연타 동작 및 주기 | LED 피드백 색상 |
| :--- | :---: | :---: | :---: | :---: | :--- | :---: |
| **Game 1** (`GAME_345`) | **`[ GM 345 ]`** | `0x7E00` (`QK_KB_0`) | `0x7E40` (`QK_USER`) | `[ F13 ]` | `3 ➔ 4 ➔ 5` (25ms 순환 연타) | 🟧 **주황색 (RGB: 255, 120, 0)** |
| **Game 2** (`GAME_2345`) | **`[ GM 2345 ]`** | `0x7E01` (`QK_KB_1`) | `0x7E41` (`QK_USER+1`) | `[ F14 ]` | `2 ➔ 3 ➔ 4 ➔ 5` (25ms 순환 연타) | 🟫 **브라운 / 앰버 (RGB: 140, 50, 0)** |
| **Game 3** (`GAME_12345`) | **`[ GM 12345 ]`** | `0x7E02` (`QK_KB_2`) | `0x7E42` (`QK_USER+2`) | `[ F15 ]` | `1 ➔ 2 ➔ 3 ➔ 4 ➔ 5` (25ms 순환 연타) | 🩵 **하늘색 (RGB: 0, 220, 255)** |
| **Game 4** (`GAME_45`) | **`[ GM 45 ]`** | `0x7E03` (`QK_KB_3`) | `0x7E43` (`QK_USER+3`) | `[ F16 ]` | `4 ➔ 5` (25ms 순환 연타) | 🔴 **빨간색 (RGB: 255, 0, 0)** |
| **Game 5** (`GAME_L_MOUSE`) | **`[ GM MOUSE ]`** | `0x7E04` (`QK_KB_4`) | `0x7E44` (`QK_USER+4`) | `[ F17 ]` | **마우스 좌클릭(`MS_BTN1`) 50ms 초고속 연타** | 🟣 **자홍색 / 마젠타 (RGB: 255, 0, 200)** |

*(구버전 키코드인 `QK_KB_11` ~ `QK_KB_15`도 완벽히 하위 호환됩니다.)*

---

## 📁 저장소 구조

```text
├── keyboards/keychron/
│   ├── common/game_turbo/                  # 공통 Game Turbo 엔진 모듈
│   │   ├── game_turbo.h                    # 키코드 상수 및 API 선언
│   │   ├── game_turbo.c                    # 논블로킹 타이머 및 실시간 RGB 인디케이터
│   │   └── game_turbo.mk                   # 소스 빌드 플래그
│   ├── q2/ansi_encoder/keymaps/game_turbo/ # Keychron Q2 Knob 전용 키맵 (Layer 4 W=GM 345, E=GM MOUSE 기본 탑재)
│   ├── q2_max/ansi_encoder/keymaps/game_turbo/ # Q2 Max 무선 전용 키맵
│   └── q2_he/ansi_encoder/keymaps/game_turbo/  # Q2 HE 자석축 전용 키맵
├── VIA_디자인탭_Keychron_Q2_정의.json       # 직설적 파일명의 초경량 무경고 VIA 정의 JSON
├── keychron_q2_via_definition.json         # 영문 호환 정의 JSON
├── keychron_q2_game_turbo_via.json         # 기존 빌드 스크립트 호환 정의 JSON
├── keychron_q2_ansi_encoder_game_turbo.bin # 즉시 플래싱 가능한 최신 펌웨어 바이너리
├── build_fw.sh                             # DFU 자동 감지 플래싱 & 컴파일 헬퍼
├── enter_bootloader.py                     # 소프트웨어 부트로더 점프 파이썬 유틸
├── setup_env.sh                            # 툴체인 및 udev 권한 원클릭 셋업
├── README.md                               # 종합 사용 설명서
└── GEMINI.md                               # 프로젝트 개발 및 기술 명세서
```

---

## ⚡ 펌웨어 플래싱 (업로드) 가이드

사전 빌드된 최신 바이너리(`keychron_q2_ansi_encoder_game_turbo.bin`)를 키보드에 바로 주입할 수 있습니다.

### 1단계: 키보드를 DFU 부트로더 모드로 진입

아래 방법 중 하나를 선택합니다:

1. **단축키 부팅 진입 (가장 추천 ⭐)**:
   - USB 케이블을 키보드에서 분리합니다.
   - 키보드의 **`Space` + `B`** 키를 동시에 꾹 누른 상태에서 USB 케이블을 다시 연결합니다.
2. **소프트웨어 명령어 진입**:
   ```bash
   ./enter_bootloader.py
   ```
3. **물리 리셋 버튼 진입**:
   - 스페이스바 키캡을 분리하면 스위치 좌측에 작은 물리 리셋 버튼이 있습니다. 케이블이 연결된 상태에서 2초간 누릅니다.

> **연결 확인**: `dfu-util -l` 실행 시 `0483:df11` (STM32 부트로더) 장치가 감지되면 정상입니다.

---

### 2단계: 펌웨어 플래싱 실행

#### 방법 A: 스마트 헬퍼 스크립트 사용 (가장 편리 ⭐)
부트로더 연결을 자동 감지한 뒤 고속 플래싱하고 키보드를 재부팅합니다:
```bash
./build_fw.sh flash keychron/q2/ansi_encoder game_turbo
```

#### 방법 B: dfu-util 직접 플래싱
```bash
dfu-util -a 0 -d 0483:df11 --dfuse-address 0x08000000:leave -D keychron_q2_ansi_encoder_game_turbo.bin
```

---

## 🎨 VIA 웹앱([usevia.app](https://usevia.app)) 키 매핑 가이드

1. **디자인(DESIGN) 탭에서 정의 로드**:
   - 상단 톱니바퀴(`SETTINGS`) ➔ **`Show Design tab`** 스위치 활성화
   - 상단 **`DESIGN` (브러시 아이콘)** 탭으로 이동
   - **"Load Draft Definition (임시 정의 불러오기)"** 상자에 아래 파일을 드래그 앤 드롭:
     - 📁 **`VIA_디자인탭_Keychron_Q2_정의.json`** *(또는 `keychron_q2_via_definition.json`)*
   - **결과**: 노란 경고나 에러 없이 하단에 `Keychron Q2 ANSI Knob`가 즉시 100% 정상 등록됩니다.

2. **사용자 지정(CUSTOM) 탭에서 키 매핑**:
   - 상단 **`CONFIGURE` (키보드 아이콘)** 탭으로 이동
   - 하단 카테고리에서 **`CUSTOM` (사용자 지정)** 탭 클릭
   - 화면 첫 줄에 나타나는 **`[ GM 345 ]`**, **`[ GM 2345 ]`**, **`[ GM 12345 ]`**, **`[ GM 45 ]`**, **`[ GM MOUSE ]`** 중 원하는 키를 자판의 원하는 위치(예: W, E 등)로 자유롭게 드래그하여 배치합니다.

> 💡 **알아두기**: 최신 펌웨어에는 이미 5번째 레이어(Fn2 / Layer 4)에 `W=GM 345`, `E=GM MOUSE`가 기본값으로 내장되어 있으므로 별도의 키맵 백업 JSON을 불러올 필요가 없습니다!

---

## 🛠️ 소스코드 직접 빌드 방법 (개발자용)

```bash
# 1. 아치 리눅스 컴파일러 환경 셋업 (최초 1회)
./setup_env.sh

# 2. 컴파일
./build_fw.sh compile keychron/q2/ansi_encoder game_turbo
```

---

## 📜 라이선스
본 프로젝트는 [QMK Firmware](https://github.com/qmk/qmk_firmware) 라이선스에 따라 **GPL-2.0-or-later**를 준수합니다.
