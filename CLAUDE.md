# Vita3K-emu Development Guide

## Project Overview

**Vita3K-emu** is a PlayStation Vita emulator fork focused on:
- 1:1 PS Vita hardware accuracy
- Authentic LiveArea UI recreation
- Multi-platform optimization (Desktop, Android, Steam Deck)

## Architecture

### Core Components

| Directory | Purpose |
|-----------|---------|
| `vita3k/cpu/` | ARM CPU emulation via Dynarmic JIT |
| `vita3k/gxm/` | GXM (Vita graphics API) implementation |
| `vita3k/renderer/` | OpenGL & Vulkan rendering backends |
| `vita3k/shader/` | USSE shader recompilation |
| `vita3k/kernel/` | Vita kernel HLE |
| `vita3k/modules/` | 150+ system module implementations |
| `vita3k/gui/` | ImGui + SDL3 user interface |
| `vita3k/mem/` | Memory management |
| `vita3k/audio/` | Audio emulation |
| `vita3k/ngs/` | NGS audio system |
| `external/` | Third-party dependencies (git submodules) |

### Key Technologies

- **Language**: C++20
- **Build**: CMake 3.22+
- **CPU JIT**: Dynarmic (ARM → x86/ARM64)
- **Graphics**: OpenGL 4.3+ / Vulkan 1.0+
- **UI**: Dear ImGui + SDL3
- **Shaders**: GLSL / SPIR-V via SPIRV-Cross

## Building

### Linux (recommended for development)

```bash
# Install dependencies
sudo apt install build-essential cmake ninja-build clang \
    libsdl2-dev libgtk-3-dev libboost-all-dev

# Configure
cmake --preset linux-ninja-clang

# Build
cmake --build build/linux-ninja-clang -j$(nproc)
```

### Windows

```bash
cmake --preset windows-vs2022
cmake --build build/windows-vs2022 --config Release
```

## Coding Conventions

- Follow `.clang-format` for code style
- Use C++20 features where appropriate
- Prefix emulated Vita types with `Sce` (e.g., `SceUID`, `SceSize`)
- Module exports use `EXPORT()` macro
- Use `LOG_*` macros for logging (spdlog)

## Focus Areas for This Fork

### Priority 1: LiveArea UI
Recreate authentic PS Vita home screen experience:
- Bubble icons with app metadata
- Horizontal scrolling with momentum physics
- Page transitions and animations
- System status bar

### Priority 2: Core Accuracy
- Fix vertex explosion bugs
- Improve NGS audio system
- Better game compatibility

### Priority 3: Platform Polish
- Android performance optimization
- Steam Deck controller integration
- Frame pacing improvements

## Useful Files

| File | Description |
|------|-------------|
| `vita3k/gui/src/home_screen.cpp` | Main UI entry point |
| `vita3k/gui/src/game_selector.cpp` | Game library view |
| `vita3k/renderer/src/vulkan/` | Vulkan backend |
| `vita3k/modules/SceGxm/` | GXM graphics module |
| `vita3k/cpu/src/dynarmic_cpu.cpp` | CPU emulation core |

## Testing

```bash
# Run all tests
ctest --test-dir build/linux-ninja-clang

# Run specific test
./build/linux-ninja-clang/bin/vita3k-tests
```

## Git Workflow

- `main` - Stable releases
- `develop` - Integration branch
- `feature/*` - New features
- `fix/*` - Bug fixes
- `ui/livearea` - LiveArea UI development

## Remotes

- `origin` - Your fork (mixelpixx/Vita3K-emu)
- `upstream` - Original project (Vita3K/Vita3K)

```bash
# Sync with upstream
git fetch upstream
git merge upstream/master
```
