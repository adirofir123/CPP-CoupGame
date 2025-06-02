// adirofir123@gmail.com

#include "Game.hpp"
#include "Player.hpp"
#include "Roles/Governor.hpp"
#include "Roles/Spy.hpp"
#include "Roles/Baron.hpp"
#include "Roles/General.hpp"
#include "Roles/Judge.hpp"
#include "Roles/Merchant.hpp"

#include <iostream>
#include <vector>
using namespace std;

void printState(const Game& game, const vector<Player*>& players) {
    cout << "\n--- Current State ---\n";
    cout << "Turn: " << game.turn() << "\n";
    for (auto* p : players) {
        if (p->isActive()) {
            cout << "  " << p->name() << " [" << p->coins() << "]\n";
        }
    }
    cout << "---------------------\n\n";
}

void advanceTo(Game& game, const string& name) {
    while (game.turn() != name) game.nextTurn();
}

int main() {
    Game game;

    Governor  *alice   = new Governor(game, "Alice");
    Spy       *bob     = new Spy(game, "Bob");
    Baron     *carol   = new Baron(game, "Carol");
    General   *dave    = new General(game, "Dave");
    Judge     *eve     = new Judge(game, "Eve");
    Merchant  *frank   = new Merchant(game, "Frank");

    vector<Player*> players = {alice, bob, carol, dave, eve, frank};

    cout << "=== Demo: Coup-Style Game ===\n";

    // Each player gathers once
    for (int i = 0; i < 6; ++i) {
        advanceTo(game, players[i]->name());
        players[i]->gather();
    }
    printState(game, players);

    // Carol gathers twice and invests
    advanceTo(game, "Carol"); carol->gather();
    advanceTo(game, "Carol"); carol->gather();
    advanceTo(game, "Carol"); cout << "[Baron] Carol invests\n"; carol->invest();
    printState(game, players);

    // Alice taxes
    advanceTo(game, "Alice"); cout << "[Governor] Alice taxes\n"; alice->tax();
    printState(game, players);

    // Bob peeks at Carol
    advanceTo(game, "Bob"); cout << "[Spy] Bob peeks: Carol has " << bob->peekCoins(*carol) << " coins\n";
    printState(game, players);

    // Dave gathers twice, arrests Eve
    advanceTo(game, "Dave"); dave->gather();
    advanceTo(game, "Dave"); dave->gather();
    printState(game, players);
    advanceTo(game, "Dave"); cout << "[General] Dave arrests Eve\n"; dave->arrest(*eve);
    printState(game, players);

    // Carol tries and fails to coup, then gathers and succeeds
    advanceTo(game, "Carol"); cout << "[Baron] Carol attempts coup (should fail)\n";
    try { carol->coup(*frank); } catch (const exception& e) { cout << "  → Error: " << e.what() << "\n"; }
    advanceTo(game, "Carol"); carol->gather();
    printState(game, players);
    advanceTo(game, "Carol"); cout << "[Baron] Carol coups Frank\n"; carol->coup(*frank);
    printState(game, players);

    // Alice taxes again; Dave tries and fails to block
    advanceTo(game, "Alice"); cout << "[Governor] Alice taxes again\n"; alice->tax();
    printState(game, players);
    advanceTo(game, "Dave"); cout << "[General] Dave attempts to block Alice (should fail)\n";
    try { dave->undo(*alice); } catch (const exception& e) { cout << "  → Error: " << e.what() << "\n"; }
    printState(game, players);

    // Bob bribes; Eve blocks
    while (bob->coins() < 4) { advanceTo(game, "Bob"); bob->gather(); }
    advanceTo(game, "Bob"); cout << "[Spy] Bob bribes\n"; bob->bribe();
    printState(game, players);
    advanceTo(game, "Eve"); cout << "[Judge] Eve blocks Bob’s bribe\n"; eve->undo(*bob);
    printState(game, players);

    // Dave coups Alice
    while (dave->coins() < 7) { advanceTo(game, "Dave"); dave->gather(); }
    printState(game, players);
    advanceTo(game, "Dave"); cout << "[General] Dave coups Alice\n"; dave->coup(*alice);
    printState(game, players);

    // Declare winner
    try { cout << "\n*** Winner: " << game.winner() << " ***\n"; }
    catch (const exception& e) { cout << "Game not over: " << e.what() << "\n"; }

    for (auto* p : players) delete p;
    return 0;
}
