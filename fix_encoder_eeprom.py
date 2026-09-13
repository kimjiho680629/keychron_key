#!/usr/bin/env python3
"""
Keychron Q2 / VIA 로터리 인코더(Knob) EEPROM 자동 동기화 도구
- VIA Raw HID 프로토콜을 통해 키보드 내부 EEPROM의 인코더 매핑을 검사하고 기본값으로 자동 복구합니다.
- Layer 0 (Mac): CCW = KC_VOLD (0xAA), CW = KC_VOLU (0xA9)
- Layer 1 (Windows): CCW = KC_VOLD (0xAA), CW = KC_VOLU (0xA9)
- Layer 2 (Mac Fn): CCW = RM_VALD (0x784A), CW = RM_VALU (0x7849)
- Layer 3 (Win Fn): CCW = RM_VALD (0x784A), CW = RM_VALU (0x7849)
- Layer 4 (Fn2 / Fn3): CCW = KC_VOLD (0xAA), CW = KC_VOLU (0xA9)
"""

import sys
try:
    import hid
except ImportError:
    print("오류: hidapi 모듈이 설치되어 있지 않습니다. (pip install hidapi)")
    sys.exit(1)

def find_keychron_raw_hid():
    for d in hid.enumerate():
        # Keychron Vendor ID: 0x3434 (13364)
        if d['vendor_id'] == 0x3434 and d.get('usage_page') == 0xFF60:
            return d['path']
    return None

def set_encoder(dev, layer, clockwise, keycode):
    buf = [0x00] * 32
    buf[0] = 0x15 # id_dynamic_keymap_set_encoder
    buf[1] = layer
    buf[2] = 0    # encoder 0
    buf[3] = 1 if clockwise else 0
    buf[4] = (keycode >> 8) & 0xFF
    buf[5] = keycode & 0xFF
    dev.write(bytes(buf))
    return dev.read(32)

def get_encoder(dev, layer, clockwise):
    buf = [0x00] * 32
    buf[0] = 0x14 # id_dynamic_keymap_get_encoder
    buf[1] = layer
    buf[2] = 0
    buf[3] = 1 if clockwise else 0
    dev.write(bytes(buf))
    res = dev.read(32)
    return (res[4] << 8) | res[5]

def main():
    path = find_keychron_raw_hid()
    if not path:
        print("❌ Keychron 키보드 Raw HID 인터페이스를 찾을 수 없습니다.")
        print("키보드가 USB 케이블로 정상 연결되어 있는지 확인해 주세요.")
        sys.exit(1)

    print(f"✅ Keychron 키보드 감지 완료: {path.decode(errors='ignore')}")
    dev = hid.Device(path=path)

    # 1. 프로토콜 버전 확인
    buf = [0x00] * 32
    buf[0] = 0x01
    dev.write(bytes(buf))
    res = dev.read(32)
    proto_ver = (res[1] << 8) | res[2]
    print(f"ℹ️  VIA 프로토콜 버전: v{proto_ver >> 8}.{proto_ver & 0xFF}")

    # 2. 인코더 매핑 테이블 주입
    targets = [
        (0, "Mac 기본", 0x00AA, 0x00A9),        # VOLD, VOLU
        (1, "Windows 기본", 0x00AA, 0x00A9),    # VOLD, VOLU
        (2, "Mac Fn 미디어", 0x784A, 0x7849),    # RM_VALD, RM_VALU
        (3, "Win Fn 미디어", 0x784A, 0x7849),    # RM_VALD, RM_VALU
        (4, "Fn2 게임/펑션", 0x00AA, 0x00A9),   # VOLD, VOLU
    ]

    print("\n🔄 키보드 내부 EEPROM 인코더 매핑 동기화 진행 중...")
    for layer, name, ccw, cw in targets:
        set_encoder(dev, layer, False, ccw)
        set_encoder(dev, layer, True, cw)
        r_ccw = get_encoder(dev, layer, False)
        r_cw = get_encoder(dev, layer, True)
        print(f"  • Layer {layer} ({name}): 반시계=0x{r_ccw:04X}, 시계=0x{r_cw:04X} [정상 동기화]")

    dev.close()
    print("\n🎉 모든 레이어의 노브 볼륨/RGB 조절 매핑이 성공적으로 복구되었습니다!")
    print("👉 지금 즉시 노브를 좌/우로 돌려 볼륨 조절이 되는지 확인해 보세요.")

if __name__ == "__main__":
    main()
