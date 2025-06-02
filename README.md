# Coup-Style Turn-Based Game in C++

A simplified implementation of a **Coup-style turn-based strategy game** written in modern C++17, featuring role-specific abilities, turn management, and a basic GUI using **SFML**.

## 📦 Features

- Full game logic modeled after Coup-like mechanics:
  - Turn-based action system
  - Role-specific abilities: Governor, Spy, Baron, General, Judge, Merchant
  - Sanction, Bribe, Arrest, Coup mechanics
  - Reactions and blocks with delayed logic
  - Custom exceptions for illegal actions
- Enforces all game rules (e.g. mandatory coup at 10+ coins)
- Headless demo mode (`Demo.cpp`)
- **SFML GUI** frontend (`main_gui.cpp`)
- Modular codebase with unit tests
- Memory safe: fully tested with **Valgrind**

## 🔧 Requirements

- C++17 compatible compiler (GCC, Clang, etc.)
- SFML library (graphics, window, system modules)
- GNU Make
- Valgrind (optional, for memory testing)
- Doctest (included in tests)

## 🛠️ Build Instructions

### 1. Clone the Repo

```bash
git clone https://github.com/YOUR_USERNAME/coup-game.git
cd coup-game
```

### 2. Build the Demo Game

```bash
make
./Main
```

### 3. Build the GUI

```bash
make gui
./gui
```

### 4. Run Unit Tests

```bash
make test
```

### 5. Run Valgrind (Memory Leak Check)

```bash
make valgrind      # For core game logic tests
valgrind ./gui     # For GUI (optional, may give noisy output)
```

## 👥 Roles Overview

| Role      | Description                                                       |
|-----------|-------------------------------------------------------------------|
| Governor  | Can tax for 2 coins                                               |
| Spy       | Peeks at others’ coin counts, can bribe                           |
| Baron     | Invests to grow coin count quickly                                |
| General   | Can arrest and block coup if they have 5+ coins                   |
| Judge     | Can block bribes                                                  |
| Merchant  | Gets a bonus coin at the start of their turn                      |

## 🧪 Testing

- `doctest` framework used for unit testing
- Covers core logic, edge cases, illegal moves
- Example output:

```
[doctest] test cases: 16 | 16 passed | 0 failed
[doctest] assertions: 46 | 46 passed | 0 failed
```

## 🗂️ Project Structure

```
.
├── src/                # Core logic: Game, Player, Roles
│   ├── Game.cpp
│   ├── Player.cpp
│   └── Roles/
├── tests/              # Unit tests (Doctest)
│   ├── test_game.cpp
│   ├── test_player.cpp
│   └── test_roles.cpp
├── main_gui.cpp        # SFML GUI frontend
├── Demo.cpp            # CLI-based demo game
├── Makefile
└── README.md
```

## 📌 Notes

- Game ends when only one player remains active
- All illegal actions throw clear exceptions
- GUI shows each turn and action with basic visual feedback

## 📄 License

MIT License — free to use and modify.

## 🙌 Credits

Developed by [Your Name]  
Special thanks to course staff and open-source tools like SFML and Doctest.