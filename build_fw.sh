#!/usr/bin/env bash
# Keychron Firmware Build Helper Script
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
QMK_DIR="${SCRIPT_DIR}/qmk_firmware"

show_usage() {
    cat << 'EOF'
사용법:
  ./build_fw.sh list [검색어]              - 지원되는 키크론 키보드 목록 조회
  ./build_fw.sh compile <키보드> [키맵]    - 펌웨어 컴파일
  ./build_fw.sh flash <키보드> [키맵]      - 펌웨어 컴파일 및 플래싱 대기
  ./build_fw.sh clean                      - 빌드 출력 파일 정리
  ./build_fw.sh branches                   - 사용 가능한 Keychron 공식 브랜치 조회

예시:
  ./build_fw.sh list q1
  ./build_fw.sh compile keychron/q1/rev_0100 via
  ./build_fw.sh compile keychron/q1_max/ansi_encoder via
  ./build_fw.sh flash keychron/v1/ansi via
EOF
}

cmd="${1:-}"

case "${cmd}" in
    list)
        pattern="${2:-}"
        echo "=== 지원되는 Keychron 키보드 목록 ==="
        find "${QMK_DIR}/keyboards/keychron" -maxdepth 3 -name "info.json" | while read -r infopath; do
            dir=$(dirname "${infopath}")
            rel="${dir#${QMK_DIR}/keyboards/}"
            if [ -z "${pattern}" ] || echo "${rel}" | grep -qi "${pattern}"; then
                echo "  ${rel}"
            fi
        done | sort
        ;;

    compile)
        if [ -z "${2:-}" ]; then
            echo "오류: 키보드 타겟을 지정해야 합니다."
            show_usage
            exit 1
        fi
        kb="$2"
        km="${3:-via}"
        echo "컴파일 진행: Keyboard=${kb}, Keymap=${km}"
        qmk compile -kb "${kb}" -km "${km}"

        bin_name=$(echo "${kb}_${km}" | tr '/' '_')
        bin_path="${QMK_DIR}/${bin_name}.bin"
        if [ ! -f "${bin_path}" ]; then
            bin_path=$(find "${QMK_DIR}" -maxdepth 1 -name "*${km}*.bin" -type f | head -n 1)
        fi
        if [ -f "${bin_path}" ]; then
            cp -f "${bin_path}" "${HOME}/Downloads/$(basename "${bin_path}")" 2>/dev/null || true
            cp -f "${bin_path}" "${SCRIPT_DIR}/$(basename "${bin_path}")" 2>/dev/null || true
            echo "최신 바이너리가 프로젝트 루트 및 Downloads 폴더에 복사되었습니다."
        fi
        ;;

    flash)
        if [ -z "${2:-}" ]; then
            echo "오류: 키보드 타겟을 지정해야 합니다."
            show_usage
            exit 1
        fi
        kb="$2"
        km="${3:-via}"
        echo "=========================================================="
        echo "1. 펌웨어 컴파일 진행: Keyboard=${kb}, Keymap=${km}"
        echo "=========================================================="
        qmk compile -kb "${kb}" -km "${km}"

        # 생성된 바이너리 파일 찾기
        bin_name=$(echo "${kb}_${km}" | tr '/' '_')
        bin_path="${QMK_DIR}/${bin_name}.bin"
        if [ ! -f "${bin_path}" ]; then
            # 대체 이름 탐색
            bin_path=$(find "${QMK_DIR}" -maxdepth 1 -name "*${km}*.bin" -type f | head -n 1)
        fi

        if [ -f "${bin_path}" ]; then
            echo "바이너리 확인 완료: ${bin_path} ($(stat -c%s "${bin_path}") 바이트)"
            # Downloads 및 프로젝트 루트에 자동 동기화 복사
            cp -f "${bin_path}" "${HOME}/Downloads/$(basename "${bin_path}")" 2>/dev/null || true
            cp -f "${bin_path}" "${SCRIPT_DIR}/$(basename "${bin_path}")" 2>/dev/null || true
            echo "바이너리가 Downloads 폴더에도 복사되었습니다."
        else
            echo "경고: 빌드된 .bin 파일을 찾지 못했습니다. QMK 기본 플래셔로 전환합니다."
            qmk flash -kb "${kb}" -km "${km}"
            exit 0
        fi

        echo ""
        echo "=========================================================="
        echo "2. 키보드 부트로더(DFU) 감지 및 플래싱 대기"
        echo "=========================================================="
        echo "키보드를 부트로더 모드로 진입시켜 주세요:"
        echo "  👉 [방법 1 - 추천]: 스페이스바 키캡을 분리한 뒤, 가운데 스위치 왼쪽의 작은 원형 리셋 버튼을 1초간 누름"
        echo "  👉 [방법 2]: Fn2 키(스페이스바 오른쪽 두 번째 키)를 누른 상태에서 Esc 키 누름"
        echo "----------------------------------------------------------"

        dfu_found=0
        while [ "${dfu_found}" -eq 0 ]; do
            if dfu-util -l 2>/dev/null | grep -qi "0483:df11"; then
                dfu_found=1
                break
            fi
            printf "."
            sleep 0.5
        done
        echo ""
        echo "🎯 DFU 부트로더 장치 감지 성공 (0483:df11)!"
        echo "펌웨어 플래싱을 시작합니다..."
        sleep 0.5

        if dfu-util -a 0 -d 0483:df11 --dfuse-address 0x08000000:leave -D "${bin_path}"; then
            echo ""
            echo "=========================================================="
            echo "🎉 플래싱이 100% 성공적으로 완료되었습니다!"
            echo "키보드가 정상 재부팅되었습니다."
            echo "=========================================================="
        else
            echo "오류: dfu-util 플래싱 실패. sudo 권한으로 재시도합니다..."
            sudo dfu-util -a 0 -d 0483:df11 --dfuse-address 0x08000000:leave -D "${bin_path}"
        fi
        ;;

    clean)
        echo "빌드 아티팩트 정리 중..."
        qmk clean -a
        ;;

    branches)
        echo "=== 현재 로컬/원격 주요 브랜치 ==="
        cd "${QMK_DIR}"
        git branch -a | grep -E "master|playground|2025q|wireless" || true
        ;;

    *)
        show_usage
        exit 1
        ;;
esac
