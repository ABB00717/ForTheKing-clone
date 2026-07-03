# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

A C++ console RPG that recreates the game *For The King*. Turn-based combat with dice
mechanics, a fog-of-war world map, equipment/item/skill systems, and dozens of enemies
and bosses. Text is largely Traditional Chinese; comments and commit bodies are too.

## Building and Running

The project builds on **Linux/POSIX with g++** via the `Makefile`. All OS-specific console
work (raw-key input, cursor moves, screen clear, sleep, terminal size) goes through the
`Platform` layer (`Includes/Platform.h` + `Sources/Platform.cpp`), which has a POSIX
implementation (termios/ioctl/ANSI) and a Windows fallback behind `#ifdef _WIN32`.

- `make` — build; the binary is emitted to `build/ForTheKing`.
- `make run` — build and launch. **It runs from inside `build/`** because the game loads
  resources with paths like `../Resources/map.txt` and `../Sources/Display/Dice03.txt`,
  i.e. it expects to run one directory below the project root. Running the binary from the
  repo root will fail to find resources.
- `make clean` — remove `build/`.
- Entry point is `main.cpp` → `Game::MainProcess()`.
- There is **no test framework** in this repo.

Requires a UTF-8, ANSI-capable terminal (the UI is Traditional Chinese with ANSI colors).

**All source files must be UTF-8.** Chinese string literals are used as lookup keys across
translation units (e.g. `SkillTable`/`ItemTable` register a key that `GameInitialize.cpp`
looks up by the same literal). If one file is Big5 and another UTF-8, the byte sequences
differ, the lookup misses, and dereferencing the `find()` result crashes. Keep every file
UTF-8 so the keys match.

### Include-path convention (important)

Headers are included by **basename only** (e.g. `#include <Entity.h>`, `#include <Skill.h>`),
not by relative path. The `Makefile` puts every `Includes/` subdirectory on the include path
(`-I$(shell find Includes -type d)`), so a new header is found automatically once it lives
under `Includes/`. Header includes are **case-sensitive** on Linux — match the real filename
casing exactly (e.g. `Color.h`, not `color.h`).

Some `.cpp` files under `Sources/Gadget/` (`Armor`, `Weapon`, `Contain`, `Passive`) are dead
legacy files that were never in the build; the `Makefile` excludes them via `ORPHANS`.

## Architecture

Source is split into `Includes/` (headers) and `Sources/` (implementations), each mirroring
the same five subsystem folders. `Includes/ConstData.h` holds global enums (SKILL_TYPE,
MODE, terrain codes, the ASCII banner); `Includes/HeaderPack.h` bundles the common std +
windows includes.

- **Control** — game orchestration.
  - `Game` owns the main loop and holds the world's entities as `static` vectors
    (`Game::roles`, `enemys`, `tents`, `stores`, `chests`). It drives movement phases,
    movement-point generation, and battle entry.
  - `WorldMap` is a **namespace** (not a class) of extern globals: the tile `map`, `fog`,
    `renderMap`, per-tile `Rect` structs, and movement/rendering helpers. It is the single
    source of truth for map state.
  - `File` loads Role/Enemy data; `GameInitialize.cpp` wires up initial game state.

- **Entity** — `Entity` is the combat base class (name, `Attribute`, `Skill`, `Equipment`,
  `Dice`, status/buff ticks, faction). `Role` (player, has position/movement/backpack/money)
  and `Enemy` derive from it. `Chest`, `Store`, `Tent`, and `Interactive` are world objects.

- **Gadget** — game systems. Two patterns dominate here:
  - **Command pattern** for skills and items: `SkillCommand`/`ItemCommand` base classes with
    one concrete command per ability (see `SkillCommandSet.h`, `ItemCommandSet.h`). Abilities
    are invoked by string name and executed against `Entity` targets.
  - **Table/registry pattern**: `SkillTable`, `ItemTable`, `EquipmentTable` are classes of
    `static std::map<std::string, ...>` populated once via `Initialize()` at startup. Adding
    a new skill/item/equipment means registering it in the corresponding table plus adding its
    command class.
  - Also: `Attribute`, `Skill` (Active/Passive/Buff), `Equipment` (Weapon/Armor), `Backpack`, `Dice`.

- **Display** — presentation & input. `Displayer`, `UI`, `KeyBoard`, and `Process` (which
  contains the battle/`fightSimulator`, focus and target-selection simulators, world-map view).
  `Color.h` defines ANSI color/style macros.

- **Incident** — `Event` (random events) and `Field` (field encounters) triggered on the map.

State is held largely in static/global registries (`Game::` statics, `WorldMap::` externs,
static Tables) rather than passed around — keep that in mind when tracing data flow.

`Resources/*.txt` (maps, world layouts) and `Sources/Display/*.txt` (dice faces, ASCII art)
are loaded from disk at runtime.

## Project Conventions

These come from `內部規範.md` (the team's internal spec) and are enforced in review:

- **Naming:** camelCase throughout.
- **No externally-linked global variables.** If shared state is needed, expose it only through
  accessor functions; class data members stay private behind getters/setters.
- **Function limits:** ≤150 lines, ≤7 parameters, ≤4 levels of indentation (invert conditions
  or extract helpers instead of nesting deeper). Functions must not read non-`const` globals.
- **One header per source:** a `.cpp` includes only its same-named header. `inline`
  definitions belong in a same-named `.inl` file.
- Every header should carry a comment describing it.

### Comment style (user's global rule)

Write comments in concise English. **Never** use numbered lists (`1.`, `2.`) in comments — use
plain sequential lines. Avoid decorative ASCII (`---`, `[!!!]`). Explain concepts generously
where they aren't obvious, but keep the prose direct.

### Git workflow

- Do large features on a branch named `{Name}-{Status}` where Status ∈ `DEV` (new feature),
  `FIX` (bug fix), `REBUILD` (refactor). Merge-test against `main` before opening a PR.
- Commit messages: `HEADER: short summary`, optionally with a body and footer. Headers:
  `FEAT`, `FIX`, `DOCS`, `PERF`, `TEST`, `STYLE`, `DEPRECATE`, `REFACTOR`, `RELEASE`, `CI`.
- Commit small, frequent changes (~100 lines) rather than one large multi-point commit.

`DLC新增.txt` is a design/backlog note listing planned weapons, armor, accessories, and
skills not yet implemented — a useful reference when adding new content.
