# ─────────────────────────────────────────────────────────────
#  NEBULA HUB — Makefile
#  Requires: OpenOrbis PS4 Toolchain + LibOrbisPkg (PkgTool.Core)
# ─────────────────────────────────────────────────────────────

TITLE       := NEBULA HUB
TITLE_ID    := NEBU00001
APP_VER     := 01.00
PKG_OUT     := nebulahub.pkg

OO_PS4_TOOLCHAIN ?= $(OO_PS4_TOOLCHAIN)

TOOLCHAIN   := $(OO_PS4_TOOLCHAIN)
CC          := $(TOOLCHAIN)/bin/linux/clang
CXX         := $(TOOLCHAIN)/bin/linux/clang++
LD          := $(TOOLCHAIN)/bin/linux/ld.lld
OBJCOPY     := objcopy
PYTHON      := python3

CXXFLAGS    := \
    -target x86_64-pc-freebsd12-elf \
    -fPIC \
    -funwind-tables \
    -std=c++17 \
    -O2 \
    -Wall \
    -Wno-unused-variable \
    -Wno-unused-function \
    -D__ORBIS__ \
    -DUSE_SDL2 \
    -isysroot $(TOOLCHAIN)/include \
    -I$(TOOLCHAIN)/include \
    -Isrc

LDFLAGS     := \
    -m elf_x86_64 \
    --script $(TOOLCHAIN)/link.x \
    --eh-frame-hdr \
    -pie \
    -z max-page-size=0x4000 \
    -z relro \
    -L$(TOOLCHAIN)/lib \
    -lSDL2 \
    -lSDL2_image \
    -lSDL2_ttf \
    -lSceNet_stub \
    -lSceHttp_stub \
    -lSceSsl_stub \
    -lScePad_stub \
    -lSceUserService_stub \
    -lSceSystemService_stub \
    -lSceVideoOut_stub \
    -lScePthread_stub \
    -lkernel_stub \
    -lc \
    -lc++ \
    -lc++abi

# ─── Sources ───
SRCS := \
    src/main.cpp \
    src/db/database.cpp \
    src/net/fetcher.cpp \
    src/input/controller.cpp \
    src/install/installer.cpp \
    src/ui/renderer.cpp \
    src/ui/screens.cpp

OBJS := $(SRCS:.cpp=.o)

# ─── Build targets ───

all: $(PKG_OUT)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

eboot.elf: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

eboot.bin: eboot.elf
	$(PYTHON) $(TOOLCHAIN)/scripts/create-eboot.py \
	    -in eboot.elf \
	    -out eboot.bin \
	    -paid 0x3800000000000011

$(PKG_OUT): eboot.bin
	# Generate param.sfo from descriptor
	$(TOOLCHAIN)/bin/linux/sfo_tool \
	    --create \
	    --title "$(TITLE)" \
	    --title-id "$(TITLE_ID)" \
	    --app-ver "$(APP_VER)" \
	    sce_sys/param.sfo

	# Generate .gp4 project
	$(PYTHON) $(TOOLCHAIN)/scripts/create-gp4.py \
	    -out project.gp4 \
	    --content-id "$(TITLE_ID)-$(TITLE_ID)_00-NEBULAHUB0000001" \
	    --app-path . \
	    --title-id "$(TITLE_ID)"

	# Build PKG
	dotnet $(TOOLCHAIN)/bin/PkgTool.Core.dll \
	    pkg_build project.gp4 . $(PKG_OUT)

	@echo ""
	@echo "✅  $(PKG_OUT) built successfully"
	@echo "    Copy to USB and install via GoldHEN > Package Installer"

clean:
	rm -f $(OBJS) eboot.elf eboot.bin project.gp4 sce_sys/param.sfo $(PKG_OUT)

.PHONY: all clean
