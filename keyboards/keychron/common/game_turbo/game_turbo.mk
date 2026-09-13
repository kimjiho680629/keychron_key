# Game Turbo Engine Makefile Include
GAME_TURBO_DIR = $(TOP_DIR)/keyboards/keychron/common/game_turbo

SRC += $(GAME_TURBO_DIR)/game_turbo.c
VPATH += $(GAME_TURBO_DIR)

MOUSEKEY_ENABLE = yes
CFLAGS += -Wno-error=unused-but-set-variable
