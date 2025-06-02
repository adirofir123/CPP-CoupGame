# Coup-Style Turn-Based Game in C++

A simplified implementation of a **Coup-style turn-based strategy game** written in modern C++17, featuring role-specific abilities, turn management, and a basic GUI using **SFML**.

## Features

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

## Requirements

- C++17 compatible compiler (GCC, Clang, etc.)
- SFML library (graphics, window, system modules)
- Valgrind (optional, for memory testing)
- Doctest (included in tests)

## Build Instructions

### 1. Clone the Repo

```bash
git clone https://github.com/YOUR_USERNAME/coup-game.git
cd coup-game
```

### 2. Build the Demo Game

```bash
make
```

### 3. Build the GUI

```bash
make gui
```

### 4. Run Unit Tests

```bash
make test
```

### 5. Run Valgrind (Memory Leak Check)

```bash
make valgrind      # For core game logic tests
```

| **Role**     | **Description**                                                                                                                                                                  |
| ------------ | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **Governor** | Gains 3 coins (instead of 2) when using **tax**. Can cancel other players’ **tax** actions.                                                                                      |
| **Spy**      | Can **peek** at others’ coin counts. Can prevent them from using **arrest** on their next turn.                                                                                  |
| **Baron**    | Can **invest** 3 coins to receive 6 coins. If targeted by a **sanction**, receives 1 coin as compensation.                                                                       |
| **General**  | Can pay 5 coins to **block a coup** against himself or another player. If targeted by **arrest**, he gets his coin back.                                                         |
| **Judge**    | Can **cancel a bribe**, causing the other player to lose the 4 coins paid. If targeted by **sanction**, the attacker must pay 1 extra coin to the bank.                          |
| **Merchant** | If they **start their turn with at least 3 coins**, they receive 1 bonus coin. If targeted by **arrest**, they pay 2 coins to the bank instead of giving 1 coin to the attacker. |

## Testing

- `doctest` framework used for unit testing
- Covers core logic, edge cases, illegal moves
- Example output:

```
[doctest] test cases: 16 | 16 passed | 0 failed
[doctest] assertions: 46 | 46 passed | 0 failed
```

## Project Structure

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

## Notes

- Game ends when only one player remains active
- All illegal actions throw clear exceptions
- GUI shows each turn and action with basic visual feedback

