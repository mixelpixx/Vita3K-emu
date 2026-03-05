# Vita3K-emu Development Roadmap

## Vision

Create the definitive PS Vita emulation experience with authentic LiveArea UI and 1:1 hardware accuracy.

---

## v0.3.0 - LiveArea Foundation

**Theme**: Recreate the PS Vita home screen experience

### Deliverables
- [ ] LiveArea bubble grid with app icons
- [ ] Horizontal page scrolling with momentum
- [ ] Background wallpaper support
- [ ] Basic page indicator dots
- [ ] Touch/mouse input for navigation

### Technical Notes
- Build on existing ImGui infrastructure
- Reference real Vita UI behavior via documentation/videos
- Consider separate rendering path for LiveArea vs in-game

---

## v0.4.0 - LiveArea Polish

**Theme**: Animations and system UI

### Deliverables
- [ ] Bubble press/release animations
- [ ] Page transition effects
- [ ] App launch sequence animation
- [ ] System status bar (time, battery, wifi icons)
- [ ] Settings integration

---

## v0.5.0 - Core Emulation Fixes

**Theme**: Improve game compatibility

### Deliverables
- [ ] Fix vertex explosion regressions
- [ ] NGS audio system improvements
- [ ] Better shader compilation
- [ ] Memory management optimizations

### Target Issues from Upstream
- #3808 - Vertex explosions since commit b4c1335
- #3838 - Missing BGM in games (NGS)
- #2892 - Analog calibration

---

## v0.6.0 - Platform Optimization

**Theme**: Multi-platform polish

### Deliverables
- [ ] Android performance improvements
- [ ] Steam Deck optimizations
- [ ] Controller navigation everywhere
- [ ] Rear touch → gamepad button mapping

---

## v1.0.0 - Stable Release

**Theme**: Production-ready emulator

### Requirements
- LiveArea UI fully functional
- High game compatibility
- Stable across all platforms
- Comprehensive settings/configuration
- Save state support

---

## Research & Future Ideas

### Potential Features
- Network play / ad-hoc emulation
- PS Vita system app recreation (Trophies, Friends, etc.)
- Remote Play compatibility
- Plugin/mod support

### Technical Investigations
- iOS port (when JIT becomes available)
- Switch homebrew port
- Hardware rendering improvements
- Parallel shader compilation

---

## Contributing

See `CLAUDE.md` for development setup and coding guidelines.

### Priority Labels
- 🔴 **Critical** - Blocking issues
- 🟡 **High** - Important for next release
- 🟢 **Normal** - Standard priority
- 🔵 **Low** - Nice to have
