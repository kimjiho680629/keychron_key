# Keychron 키보드 QMK 펌웨어 & Game Turbo 특수 연타 엔진

Keychron(키크론) 기계식 키보드를 위한 고성능 QMK 커스텀 펌웨어 및 **Game 1~5 초고속 논블로킹 순환 연타 엔진 & 개별 RGB 피드백 시스템** 통합 저장소입니다.  
[kimjiho680629/kbdfans_key](https://github.com/kimjiho680629/kbdfans_key)의 검증된 연타 엔진 사양을 Keychron 하드웨어(STM32/ARM 계열)에 완벽 호환되도록 포팅 및 최적화하였습니다.

---

## 🌟 주요 특징

- **초고속 논블로킹(Non-blocking) 타이머 엔진**: 일반 `wait_ms()` 기반 매크로와 달리 키보드 전체 입력 반응성을 1ms도 저하시키지 않고 백그라운드에서 매끄럽게 동작합니다.
- **토글(Toggle) 구동 방식**: 1회 누르면 즉시 연타 시작 ➔ 다시 누르면 즉시 정지 및 안전한 키 릴리즈.
- **고유 RGB LED 시각 피드백**: 각 연타 모드마다 독립된 고유 색상의 LED가 점등되어 현재 동작 상태를 직관적으로 확인 가능합니다.
- **Keychron 순정 기능 100% 보존**: 볼륨 노브, 한영키(`KC_HAEN`), Mac/Windows 하드웨어 토글 스위치, RGB 백라이트 제어가 모두 순정 그대로 유지됩니다.
- **최신 VIA v3 완벽 호환**: Keychron 순정 단축키 11종(0~10)과 충돌 없는 인덱스 11~15번 독립 키코드 할당 및 `qmk_rgb_matrix` 모듈 최적화로 경고 없이 즉시 키캡 라벨을 렌더링합니다.

---

## 🎮 Game Turbo 연타 엔진 상세 사양

모든 게임키는 **토글(Toggle)** 방식으로 작동하며, 시작 시 지정된 고유 색상 LED가 점등되고 정지 시 소등됩니다.

| 게임키 명칭 | VIA CUSTOM 탭 라벨 | VIA SPECIAL 대체키 | Any 직접입력 코드 | 연타 동작 및 주기 | LED 피드백 색상 |
| :--- | :---: | :---: | :---: | :--- | :---: |
| **Game 1** (`GAME_345`) | **`[ GM 345 ]`** | `[ F13 ]` | `0x7E0B` (`QK_KB_11`) | `3 ➔ 4 ➔ 5` (25ms 순환 연타) | 🟧 **주황색 (Orange)** |
| **Game 2** (`GAME_2345`) | **`[ GM 2345 ]`** | `[ F14 ]` | `0x7E0C` (`QK_KB_12`) | `2 ➔ 3 ➔ 4 ➔ 5` (25ms 순환 연타) | 🟫 **브라운 (Brown)** |
| **Game 3** (`GAME_12345`) | **`[ GM 12345 ]`** | `[ F15 ]` | `0x7E0D` (`QK_KB_13`) | `1 ➔ 2 ➔ 3 ➔ 4 ➔ 5` (25ms 순환 연타) | 🩵 **하늘색 (Cyan)** |
| **Game 4** (`GAME_45`) | **`[ GM 45 ]`** | `[ F16 ]` | `0x7E0E` (`QK_KB_14`) | `4 ➔ 5` (25ms 순환 연타) | 🔴 **빨간색 (Red)** |
| **Game 5** (`GAME_L_MOUSE`) | **`[ GM MOUSE ]`** | `[ F17 ]` | `0x7E0F` (`QK_KB_15`) | **마우스 좌클릭(`MS_BTN1`) 50ms 고속 연타** | 🟣 **자홍색 (Magenta)** |

---

## 📁 저장소 구조

```text
├── keyboards/keychron/
│   ├── common/game_turbo/                  # 공통 Game Turbo 엔진 모듈
│   │   ├── game_turbo.h                    # 키코드 상수 및 API 선언
│   │   ├── game_turbo.c                    # 논블로킹 타이머 및 RGB 피드백 구현체
│   │   └── game_turbo.mk                   # 소스 포함 및 마우스키 빌드 플래그
│   ├── q2/ansi_encoder/keymaps/game_turbo/ # Keychron Q2 Knob 전용 키맵
│   ├── q2_max/ansi_encoder/keymaps/game_turbo/ # Q2 Max 무선 전용 키맵
│   └── q2_he/ansi_encoder/keymaps/game_turbo/  # Q2 HE 자석축 전용 키맵
├── keychron_q2_game_turbo_via.json         # VIA 웹앱용 키보드 정의(Definition) JSON
├── keychron_q2_ansi_encoder_game_turbo.bin # 즉시 플래싱 가능한 사전 빌드 펌웨어
├── build_fw.sh                             # 스마트 빌드 & 자동 감지 플래싱 헬퍼
├── enter_bootloader.py                     # 소프트웨어 부트로더 점프 파이썬 유틸
├── setup_env.sh                            # 툴체인 및 udev 권한 원클릭 셋업
├── README.md                               # 종합 사용 설명서
└── GEMINI.md                               # 프로젝트 개발 및 기술 명세서
```

---

## ⚡ 펌웨어 플래싱 (업로드) 가이드

사전 빌드된 바이너리(`keychron_q2_ansi_encoder_game_turbo.bin`)를 사용하거나 직접 빌드하여 키보드에 업로드할 수 있습니다.

### 1단계: 키보드를 DFU 부트로더 모드로 진입

아래 3가지 방법 중 가장 편한 방법을 선택합니다:

1. **소프트웨어 명령어 진입 (키보드 분해 불필요)**:
   ```bash
   ./enter_bootloader.py
   ```
2. **단축키 부팅 진입 (가장 추천)**:
   - USB 케이블을 키보드에서 분리합니다.
   - 키보드의 **`Space` + `B`** 키를 동시에 꾹 누른 상태에서 USB 케이블을 다시 연결합니다.
3. **물리 리셋 버튼 진입**:
   - 스페이스바 키캡을 분리하면 스위치 좌측 PCB에 작은 물리 리셋 버튼이 있습니다.
   - USB 케이블이 연결된 상태에서 이 버튼을 2초간 누릅니다.

> **DFU 연결 확인**: 터미널에서 `dfu-util -l`을 실행했을 때 `0483:df11` (STM32 부트로더) 장치가 검색되면 정상입니다.

---

### 2단계: 펌웨어 플래싱 실행

#### 방법 A: 스마트 헬퍼 스크립트 사용 (가장 편리)
부트로더 모드가 감지될 때까지 자동 대기 후 고속 플래싱 및 자동 재부팅을 수행합니다:
```bash
./build_fw.sh flash keychron/q2/ansi_encoder game_turbo
```

#### 방법 B: 사전 빌드된 바이너리 직접 플래싱 (dfu-util)
빌드 과정 없이 포함된 바이너리를 곧바로 키보드에 주입합니다:
```bash
dfu-util -a 0 -d 0483:df11 --dfuse-address 0x08000000:leave -D keychron_q2_ansi_encoder_game_turbo.bin
```

---

## 🎨 VIA 웹앱([usevia.app](https://usevia.app)) 키 매핑 및 백업 복원

크롬/엣지/웨일 등 WebHID를 지원하는 브라우저에서 [https://usevia.app](https://usevia.app) 에 접속합니다.

### 1. 키보드 정의(Definition) 로드
1. 상단 메뉴의 **`SETTINGS` (톱니바퀴 아이콘)** ➔ **`Show Design tab`** 스위치를 켭니다.
2. 상단 메뉴에 새로 나타난 **`DESIGN` (브러시 아이콘)** 탭을 클릭합니다.
3. **"Load Draft Definition (임시 정의 불러오기)"** 상자에 저장소의 **`keychron_q2_game_turbo_via.json`** 파일을 드래그 앤 드롭합니다.
   - 키보드 배열이 렌더링되며 하단에 `Keychron Q2 ANSI Knob`가 등록됩니다.

### 2. 키 매핑 방법
상단 **`CONFIGURE` (키보드 아이콘)** 탭으로 이동합니다:
- **방법 1 (CUSTOM 탭)**: 하단 카테고리에서 **`CUSTOM`** 탭을 선택하면 `GM 345`, `GM 2345`, `GM 12345`, `GM 45`, `GM MOUSE` 버튼이 준비되어 있어 원하는 키 위치로 바로 드래그할 수 있습니다.
- **방법 2 (SPECIAL 탭)**: 하단 **`SPECIAL`** 탭에서 `F13` ~ `F17`을 원하는 키 위치에 배치해도 동일하게 작동합니다.

### 3. 백업 키맵 한 번에 복원 (선택 사항)
1. **`CONFIGURE`** 탭 좌측 사이드바 하단의 **`SAVE + LOAD`** 메뉴를 클릭합니다.
2. 우측 **`Load Saved Keymap`** 상자에 저장해 둔 키맵 백업 JSON(예: `Keymap-Keychron Q2 ANSI Knob-GameTurbo.json`)을 업로드합니다.
3. 5번째 레이어(Layer 4 / Fn2)의 `W=GM 345`, `E=GM MOUSE` 등 지정된 매핑이 한 번에 복원됩니다.

---

## 🛠️ 개발 환경 구축 및 소스 컴파일

Arch Linux 환경에서 직접 펌웨어를 수정하고 컴파일하려면:

### 1. 환경 준비 (최초 1회)
```bash
./setup_env.sh
```

### 2. 펌웨어 컴파일
```bash
# Keychron Q2 (유선)
./build_fw.sh compile keychron/q2/ansi_encoder game_turbo

# Keychron Q2 Max (무선)
./build_fw.sh compile keychron/q2_max/ansi_encoder game_turbo

# Keychron Q2 HE (자석축)
./build_fw.sh compile keychron/q2_he/ansi_encoder game_turbo
```
빌드가 완료되면 프로젝트 루트에 `keychron_q2_ansi_encoder_game_turbo.bin`이 생성됩니다.

---

## 📜 라이선스
본 프로젝트는 [QMK Firmware](https://github.com/qmk/qmk_firmware) 라이선스에 따라 **GPL-2.0-or-later**를 준수합니다.
