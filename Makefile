COMPILER_DIR = ../lang/src/compiler/
RUNTIME_DIR = ../lang/src/runtime/
SHARED_DIR = ../lang/src/shared/
UTILS_DIR = ../lang/src/utils/
ENGINE_SRC = main.c graphics.c core.c timer.c

INCLUDE = -I$(COMPILER_DIR) -I$(RUNTIME_DIR) -I$(SHARED_DIR) -I$(UTILS_DIR)
CFLAGS = $(INCLUDE) -O3 -std=gnu99 -fgnu89-inline

ifeq ($(OS),Windows_NT)
	# Windows
	LDFLAGS = -lm -lShlwapi -lgravity -lsdl2 -lsdl2_image
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Darwin)
		# MacOS
		LDFLAGS = -lm -lgravity -lsdl2 -lsdl2_image
	else ifeq ($(UNAME_S),NetBSD)
		# NetBSD
		LDFLAGS = -lm -lgravity -lsdl -lsdl2_image
	else ifeq ($(UNAME_S),OpenBSD)
		# OpenBSD
		LDFLAGS = -lm -lgravity -lsdl -lsdl2_image
	else
		# Linux
		LDFLAGS = -lm -lrt -lgravity -lsdl -lsdl2_image
	endif
endif

all: engine

engine:	$(ENGINE_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: all clean engine

clean:
	rm -f engine
