CC = clang
SRCS = $(filter-out main.c,$(foreach file,$(wildcard src/*),$(notdir $(file))))
GAME = game
BUILD_DIR = build_$(CC)
LIB_DIR = lib_$(CC)
OBJS = $(addprefix $(BUILD_DIR)/, $(SRCS:.c=.o))

INSTALL = install
INSTALL_PROGRAM = $(INSTALL)
INSTALL_DATA = cp -r

prefix = /usr/local
exec_prefix = $(prefix)
bindir = $(prefix)/bin
datarootdir = $(prefix)/share
datadir = $(datarootdir)
libdir = $(exec_prefix)/lib

CFLAGS_gcc = -Iinclude -std=c99 -g -Wall -Werror
CFLAGS_clang = -Iinclude -std=c99 -g -Wall -Werror
CFLAGS = $(CFLAGS_$(CC))
LINKER_FLAGS_gcc = -Wl,-z,origin '-Wl,-rpath,$$ORIGIN/$(LIB_DIR)' -L$(LIB_DIR)/ -lm -lcuttle -lGLEW -lSDL2 -lGL -lGLU -lSDL2_image -lSDL2_mixer
LINKER_FLAGS_clang = -Wl,-z,origin '-Wl,-rpath,$$ORIGIN/$(LIB_DIR)' -L$(LIB_DIR)/ -lm -lcuttle -lGLEW -lSDL2 -lGL -lGLU -lSDL2_image -lSDL2_mixer
LINKER_FLAGS = $(LINKER_FLAGS_$(CC))

vpath %.c src

.PHONY: all directories install uninstall clean

all: directories $(GAME)

directories: $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.c
	$(CC) -o $@ -c $(CFLAGS) $<

$(GAME): $(BUILD_DIR)/main.o $(OBJS)
	$(CC) $^ $(LINKER_FLAGS) -o $@
clean:
	rm $(BUILD_DIR)/*.o
