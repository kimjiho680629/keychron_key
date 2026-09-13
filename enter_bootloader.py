#!/usr/bin/env python3
"""
Keychron Q2 Software DFU Bootloader Jump Helper
키보드 분해 없이 Esc 키를 일시적으로 부트로더 점프 키(QK_BOOT)로 설정하여
Esc를 누르면 즉시 STM32 DFU 모드로 진입하도록 지원합니다.
"""
import os
import glob
import time
import sys

def find_via_hidraw():
    for dev in sorted(glob.glob('/dev/hidraw*')):
        try:
            # check uevent
            uevent_path = f"/sys/class/hidraw/{os.path.basename(dev)}/device/uevent"
            if os.path.exists(uevent_path):
                with open(uevent_path, 'r') as f:
                    content = f.read()
                    if "3434" in content and "0111" in content and "input2" in content:
                        return dev
        except Exception:
            pass
    return None

def arm_esc_bootloader():
    dev = find_via_hidraw()
    if not dev:
        print("Keychron Q2 VIA Raw HID 장치를 찾을 수 없습니다. (이미 부트로더 모드이거나 미연결)")
        return False

    try:
        fd = os.open(dev, os.O_RDWR)
        # QK_BOOT keycode = 0x7C00 (31744)
        # VIA command: 0x05 (id_dynamic_keymap_set_keycode), layer, row(0), col(0), hi(0x7C), lo(0x00)
        for layer in [0, 1]:
            pkt = bytes([0x00, 0x05, layer, 0x00, 0x00, 0x7C, 0x00] + [0x00]*26)
            os.write(fd, pkt)
            time.sleep(0.05)
        os.close(fd)
        print(f"[{dev}] Esc 키에 부트로더 점프 코드(QK_BOOT) 설정 완료!")
        print("👉 지금 키보드의 [ Esc ] 키를 누르시면 즉시 DFU 모드로 전환됩니다!")
        return True
    except Exception as e:
        print(f"오류 발생: {e}")
        return False

if __name__ == '__main__':
    arm_esc_bootloader()
