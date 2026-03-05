# Vita3K-emu

[![C/C++ CI](https://github.com/mixelpixx/Vita3K-emu/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/mixelpixx/Vita3K-emu/actions/workflows/c-cpp.yml)
[![Based on Vita3K](https://img.shields.io/badge/based%20on-Vita3K-blue)](https://github.com/Vita3K/Vita3K)

## 🎮 Project Vision

**Vita3K-emu** is a PlayStation Vita emulator fork focused on:
- **1:1 Hardware Accuracy** — Authentic PS Vita behavior
- **LiveArea UI** — Recreating the iconic Vita home screen experience
- **Multi-Platform Polish** — Optimized for Desktop, Android, and Steam Deck

> *Based on the excellent [Vita3K](https://github.com/Vita3K/Vita3K) project*

## ✨ What's Different

| Feature | Status |
|---------|--------|
| Authentic LiveArea home screen | 🔨 In Development |
| Bubble icons with animations | 📋 Planned |
| Native Vita UI recreation | 📋 Planned |
| Enhanced controller support | 📋 Planned |

See our [Development Roadmap](./docs/ROADMAP.md) for the full plan.

## Gallery

|               **Persona 4 Golden** by Atlus                   |                     **A Rose in the Twilight** by Nippon Ichi Software                         |
| :-----------------------------------------------------------: | :--------------------------------------------------------------------------------------------: |
| ![Persona 4 Golden screenshot](./_readme/screenshots/P4G.png) | ![A Rose in the Twilight screenshot](./_readme/screenshots/A%20Rose%20in%20the%20Twilight.png) |

|                  **Alone with You** by Benjamin Rivers                     |                 **VA-11 HALL-A** by Sukeban Games                    |
| :------------------------------------------------------------------------: | :------------------------------------------------------------------: |
| ![Alone with You screenshot](./_readme/screenshots/Alone%20With%20You.png) | ![VA-11 HALL-A screenshot](./_readme/screenshots/VA-11%20HALL-A.png) |

## Compatibility

The emulator runs most homebrew programs and commercial games.

- [Homebrew compatibility](https://vita3k.org/compatibility-homebrew.html)
- [Commercial compatibility](https://vita3k.org/compatibility.html)

## Building

See [`building.md`](./building.md) for build instructions.

### Quick Start (Linux)

```bash
# Clone with submodules
git clone --recursive https://github.com/mixelpixx/Vita3K-emu.git
cd Vita3K-emu

# Build
cmake --preset linux-ninja-clang
cmake --build build/linux-ninja-clang -j$(nproc)
```

## Running

Check the [quickstart guide](https://vita3k.org/quickstart) for system requirements.

**Requirements:**
- Windows: [Visual C++ 2015-2022 Redistributable](https://aka.ms/vs/17/release/vc_redist.x64.exe)
- Linux: xdg-desktop-portal
- Android: [Adreno drivers](https://github.com/K11MCH1/AdrenoToolsDrivers/releases/) (for Adreno GPUs)

## Development

See [`CLAUDE.md`](./CLAUDE.md) for development setup and architecture overview.

### Branch Structure
- `main` — Stable releases
- `develop` — Integration branch
- `ui/livearea` — LiveArea UI development

## License

GPLv2 — See [COPYING.txt](./COPYING.txt)

## Credits

**Upstream Project:**
- [Vita3K Team](https://github.com/Vita3K/Vita3K) — The amazing team behind the original emulator

**Original Thanks:**
Davee, korruptor, Rinnegatamante, ScHlAuChi, Simon Kilroy, TheFlow, xerpi, xyz, Yifan Lu, and many others.

## Support Upstream

The original Vita3K project needs your support!

[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/vita3k)

## Legal

This emulator is not for piracy. Dump games using [NoNpDrm](https://github.com/TheOfficialFloW/NoNpDrm) or [FAGDec](https://github.com/CelesteBlue-dev/PSVita-RE-tools/tree/master/FAGDec/build). Get homebrew from [VitaDB](https://www.rinnegatamante.eu/vitadb/#/).

PlayStation, PlayStation Vita, and PlayStation Network are registered trademarks of Sony Interactive Entertainment Inc.
