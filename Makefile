# === COMPILER SETTINGS ===
CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -g
INCLUDES := -I. -Isrc -Itests

# === SOURCES ===
CORE_SRC := src/Game.cpp src/Player.cpp src/Role.cpp
DEMO_SRC := Demo.cpp $(CORE_SRC)
TEST_SRC := tests/test_game.cpp tests/test_player.cpp tests/test_roles.cpp $(CORE_SRC)
GUI_SRC  := main_gui.cpp $(CORE_SRC)

# === BINARIES ===
DEMO_BIN := Main
TEST_BIN := test_runner
GUI_BIN  := gui

# === TARGETS ===
.PHONY: all Main test gui valgrind clean

# Default: build demo and tests
all: Main test

# === MAIN ===
Main: $(DEMO_SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEMO_SRC) -o $(DEMO_BIN)
	@echo "--- Running Main ---"
	@./$(DEMO_BIN)

# === TEST ===
test: $(TEST_SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(TEST_SRC) -o $(TEST_BIN)
	@echo "--- Running Tests ---"
	@./$(TEST_BIN)

# === GUI ===
gui: $(GUI_SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) main_gui.cpp src/Game.cpp src/Player.cpp src/Role.cpp \
	     -lsfml-graphics -lsfml-window -lsfml-system \
	     -o $(GUI_BIN)
	@echo "--- Running GUI ---"
	@./$(GUI_BIN)

# === VALGRIND ===
valgrind: test
	@echo "--- Running Tests under Valgrind ---"
	valgrind --leak-check=full ./$(TEST_BIN)

# === CLEAN ===
clean:
	rm -f $(DEMO_BIN) $(TEST_BIN) $(GUI_BIN)
