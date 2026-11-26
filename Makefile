# ─────────────────────────────────────────────────────────────
#  NEBULA HUB — Makefile
#  Designed to run INSIDE the official OpenOrbis Docker container:
#    docker run --rm -w /workspace -v "$(pwd)":/workspace \
#      openorbisofficial/toolchain:latest make
#
#  Inside that container, OO_PS4_TOOLCHAIN is already set and
#  clang/clang++/lld are already on PATH with the correct PS4
#  musl libc + libc++ + SDL2 + PS4 stub libraries pre-built.
# ─────────────────────────────────────────────────────────────

TARGET      := x86_64-pc-freebsd12-elf
OO          := $(OO_PS4_TOOLCHAIN)

CXX         := clang++
LD          := clang++

CXXFLAGS := \
    -Iinclude \
    --target=$(TARGET) \
    -fPIC \
    -funwind-tables \
    -std=c++17 \
    -O2 \
    -D__ORBIS__ \
    -D_DEFAULT_SOURCE \
    -D_BSD_SOURCE \
    -D_POSIX_C_SOURCE=200809L \
    -isysroot $(OO) \
    -isystem $(OO)/include/c++/v1 \
    -isystem $(OO)/include \
    -Isrc \
    -rtlib=compiler-rt \
    --unwindlib=libunwind

LDFLAGS := \
    --target=$(TARGET) \
    -fuse-ld=lld \
    -isysroot $(OO) \
    -nostartfiles \
    -nodefaultlibs \
    --unwindlib=libunwind \
    -Wl,--eh-frame-hdr \
    -Wl,-pie \
    -Wl,-z,max-page-size=0x4000 \
    -Wl,-z,relro \
    -Wl,--script=$(OO)/link.x \
    -L$(OO)/lib

# crt1.o/crti.o go first, crtn.o goes last — confirmed to exist in $(OO)/lib
# (crtbegin.o/crtend.o do NOT exist in this toolchain, so we bypass clang's
# automatic GCC-style startup-file insertion entirely with -nostartfiles
# above, and place only the three real files ourselves, in order)
CRT_START := $(OO)/lib/crt1.o $(OO)/lib/crti.o
CRT_END   := $(OO)/lib/crtn.o

LIBS := \
    -lSDL2 \
    -lSDL2_image \
    -lSceNet \
    -lSceNetCtl \
    -lSceAudioOut \
    -lSceHttp \
    -lSceSsl \
    -lScePad \
    -lSceUserService \
    -lSceSystemService \
    -lSceVideoOut \
    -lpthread \
    -lkernel \
    -lc \
    -lc++

SRCS := \
    src/main.cpp \
    src/db/database.cpp \
    src/net/fetcher.cpp \
    src/input/controller.cpp \
    src/install/installer.cpp \
    src/ui/renderer.cpp \
    src/ui/screens.cpp

OBJS := $(SRCS:.cpp=.o)

.PHONY: all clean eboot fself gp4 pkg

all: eboot.bin
	@echo "eboot.bin built — GP4/PKG step handled separately until the real tool syntax is confirmed."

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

eboot.elf: $(OBJS)
	$(LD) $(LDFLAGS) $(CRT_START) $(OBJS) $(LIBS) $(CRT_END) -o $@

eboot.bin: eboot.elf
	$(OO)/bin/linux/create-eboot \
	    -in eboot.elf \
	    -out eboot.oelf \
	    -eboot eboot.bin \
	    -paid 0x3800000000000011 \
	    -ptype fake

clean:
	rm -f $(OBJS) eboot.elf eboot.bin project.gp4
