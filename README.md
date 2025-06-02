Coup-Style C++ Game (Project 3)
Author: adirofir123@gmail.com

Project Overview
This repository contains a simplified C++ implementation of a “Coup-style” card game.
Players take on one of six roles—each with unique special abilities—and attempt to be the last active player remaining.

Roles implemented:

Governor: Tax action yields 3 coins (versus base 2). Can block (undo) another Governor’s Tax and steal 2 coins.

Spy: Can “peek” at any other player’s coins (reveals their coin count) instead of taking coins.

Baron: “Invest” action: pay 3 coins → instantly gain 6.

General: When arresting, regains +1 extra coin. Can (stub-block) an incoming Coup by paying 5.

Judge: Can undo (block) another player’s Bribe, refunding 4 coins back to them.

Merchant: When performing Arrest, target pays 2 coins to the bank (instead of losing 1 to the arresting player).

Core flow:

At the start of each turn, a player may choose one of:

Gather: +1 coin (blockable by Sanction)

Tax: +2 coins (blockable by Governor, unless Governor themselves)

Bribe: pay 4 coins to take another immediate action

Arrest: choose a target, steal 1 coin, eliminate them

Sanction: pay 3 coins to prevent one target from using Gather/Tax next turn

Coup: pay 7 coins to eliminate a target immediately (blockable by General)

Directory Structure
java
Copy
Edit
Project3/
├── arial.ttf                 ← Font file (TrueType) used by SFML GUI
├── Demo.cpp                  ← Console demo showing sample gameplay
├── main_gui.cpp              ← SFML-based GUI source
├── Makefile                  ← Build/test/gui/valgrind/clean rules
├── README.md                 ← This file
├── src/
│   ├── Game.hpp              ← Manages turn order, elimination, undo‐tracking
│   ├── Game.cpp
│   ├── Player.hpp            ← Base class (gather, tax, bribe, arrest, etc.)
│   ├── Player.cpp
│   ├── Role.hpp              ← Default stub “blocking” interface
│   ├── Role.cpp
│   └── Roles/
│       ├── Governor.hpp      ← Overrides Tax (+3) & undo(Tax)
│       ├── Spy.hpp           ← Overrides peekCoins()
│       ├── Baron.hpp         ← Overrides invest()
│       ├── General.hpp       ← Overrides arrest (regain +1) & stub undo(Coup)
│       ├── Judge.hpp         ← Overrides undo(Bribe) → refund 4
│       └── Merchant.hpp      ← Overrides arrest(target) → target pays 2
└── tests/
    ├── test_game.cpp         ← Tests for Game (turns, eliminate, winner)
    ├── test_player.cpp       ← Tests for Player (gather, tax, bribe, arrest, sanction, coup)
    └── test_roles.cpp        ← Tests for role-specific behaviors & blocking
Build & Run
Prerequisites
C++17 build tools (g++).

doctest: already included under tests/ as a single‐header.

SFML (for GUI) & a TrueType font (arial.ttf).

On WSL (Windows 10) you’ll need an external X server (e.g. VcXsrv or Xming).

On WSL (Windows 11) you can use WSLg (built‐in), no extra X server needed.

1) Clone & Enter
bash
Copy
Edit
git clone <your-repo-URL> Project3
cd Project3
2) Build Demo + Tests (default)
go
Copy
Edit
make
This runs Main (the console demo), then runs test_runner (all unit tests).

Expected output:

less
Copy
Edit
--- Running Main ---
(console demo steps...)
--- Running Tests ---
[doctest] test cases: 11 | 11 passed | 0 failed | 0 skipped
[doctest] Status: SUCCESS!
3) Run Only Tests
bash
Copy
Edit
make test
Compiles and runs the unit tests.

4) Build & Run the GUI
Note: Ensure a working X11/Wayland environment (VcXsrv/Xming on Windows 10, or WSLg on Windows 11). Also place arial.ttf in the project root (same folder as Makefile).

go
Copy
Edit
make gui
Compiles main_gui.cpp with SFML flags (-lsfml-graphics -lsfml-window -lsfml-system).

Immediately launches an SFML window showing:

Left: list of active players (Name : coins), with a blue highlight on the current turn.

Bottom: six buttons: Gather, Tax, Bribe, Arrest, Sanction, Coup.

Click an action button. If it’s a two-step action (Arrest/Sanction/Coup), you then click the target’s name in the list.

5) Memory‐Leak Check with Valgrind
go
Copy
Edit
make valgrind
Rebuilds tests (if needed) and runs under Valgrind.

Expect “All heap blocks were freed — no leaks are possible.”

6) Cleanup
go
Copy
Edit
make clean
Removes executables: Main, test_runner, and gui.

Design Notes
Turn Logic:
Each action method (e.g. gather(), tax(), bribe(), etc.) throws IllegalAction if it isn’t that player’s turn. On success, it calls game_.nextTurn(), which jumps to the next active player in join order.

“Undo” / Blocking is tracked in Game via two containers:

lastTaxes: unordered_map<Player*,int> stores how many coins each player gained with their last tax().

Game::undoTax(player) removes that entry and returns the coin count to be stolen/returned.

lastBribes: unordered_map<Player*,bool> records whether a player has just performed bribe().

Game::undoBribe(player) checks and clears that flag, indicating a valid block for bribe.

Public helper methods in Player for role classes:

addCoins(int) to give a target coins (used by Judge to refund).

stealCoins(Player&,int) to move coins from target → this (used by Governor, General, etc.).

penalize(Player&,int) to remove coins from the target (used by Merchant).

Role-specific overrides live in src/Roles/. Each override checks turn, availability/cost, applies its special effect, and then calls game_.nextTurn().

