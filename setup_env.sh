#!/usr/bin/env bash
# Keychron QMK Firmware Environment Setup Script for Arch Linux
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
QMK_DIR="${SCRIPT_DIR}/qmk_firmware"

echo "=== [1/4] QMK CLI 전역 설정 확인 및 등록 ==="
qmk config user.qmk_home="${QMK_DIR}"
echo "user.qmk_home 이 ${QMK_DIR} 로 설정되었습니다."

echo ""
echo "=== [2/4] 필수 툴체인 및 패키지 확인/설치 안내 ==="
PACKAGES=(arm-none-eabi-gcc arm-none-eabi-newlib arm-none-eabi-binutils dfu-util)
MISSING_PKGS=()

for pkg in "${PACKAGES[@]}"; do
    if ! pacman -Q "${pkg}" &>/dev/null; then
        MISSING_PKGS+=("${pkg}")
    fi
done

if [ ${#MISSING_PKGS[@]} -gt 0 ]; then
    echo "다음 필수 패키지 설치가 필요합니다: ${MISSING_PKGS[*]}"
    echo "아래 명령어를 실행하여 설치해주세요:"
    echo "  sudo pacman -S --needed ${MISSING_PKGS[*]}"
    read -rp "지금 sudo 권한으로 위 패키지들을 설치하시겠습니까? [y/N]: " INSTALL_ANSWER
    if [[ "${INSTALL_ANSWER}" =~ ^[yY]$ ]]; then
        sudo pacman -S --needed "${MISSING_PKGS[@]}"
    else
        echo "패키지 설치를 건너뜁니다. 빌드 전 수동으로 설치해주세요."
    fi
else
    echo "필수 패키지(${PACKAGES[*]})가 이미 설치되어 있습니다."
fi

echo ""
echo "=== [3/4] Linux udev 룰(Flashing 권한) 설정 ==="
UDEV_SOURCE="${QMK_DIR}/util/udev/50-qmk.rules"
UDEV_TARGET="/etc/udev/rules.d/50-qmk.rules"

if [ -f "${UDEV_TARGET}" ]; then
    echo "udev 규칙(${UDEV_TARGET})이 이미 존재합니다."
else
    echo "키보드 부트로더 플래싱 권한을 위한 udev 규칙 복사가 필요합니다."
    read -rp "sudo cp ${UDEV_SOURCE} ${UDEV_TARGET} 명령을 실행하시겠습니까? [y/N]: " UDEV_ANSWER
    if [[ "${UDEV_ANSWER}" =~ ^[yY]$ ]]; then
        sudo cp "${UDEV_SOURCE}" "${UDEV_TARGET}"
        sudo udevadm control --reload-rules
        sudo udevadm trigger
        echo "udev 규칙 적용이 완료되었습니다."
    else
        echo "udev 규칙 설정을 건너뜁니다."
    fi
fi

echo ""
echo "=== [4/4] QMK Git 서브모듈 동기화 ==="
echo "ARM ChibiOS 및 필수 서브모듈을 동기화합니다..."
cd "${QMK_DIR}"
qmk git-submodule

echo ""
echo "=== 환경 설정 완료 ==="
echo "QMK Doctor를 실행하여 최종 상태를 검사합니다:"
qmk doctor -n || true
