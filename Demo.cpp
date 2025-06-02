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
#include <memory>

using namespace std;

void printState(const Game &game, const vector<Player *> &players)
{
    cout << "\n--- Current State ---\n";
    try
    {
        cout << "Turn: " << game.turn() << "\n";
    }
    catch (...)
    {
        cout << "Turn: (no active player)\n";
    }

    for (auto *p : players)
    {
        cout << "  " << (p->isActive() ? "[ALIVE] " : "[OUT]   ")
             << p->name() << " [" << p->coins() << "] - " << p->role() << "\n";
    }
    cout << "---------------------\n";
}

void advanceTo(Game &game, const string &name)
{
    while (game.turn() != name)
        game.nextTurn();
}

int main()
{
    Game game;

    Governor *alice = new Governor(game, "Alice");
    Spy *bob = new Spy(game, "Bob");
    Baron *carol = new Baron(game, "Carol");
    General *dave = new General(game, "Dave");
    Judge *eve = new Judge(game, "Eve");
    Merchant *frank = new Merchant(game, "Frank");

    vector<Player *> players = {alice, bob, carol, dave, eve, frank};

    cout << "=== DEMO: COUP-STYLE GAME ===\n";

    // === ROUND 1: Everyone gathers
    cout << "\n=== ROUND 1: Basic Gather ===\n";
    for (auto *p : players)
    {
        advanceTo(game, p->name());
        p->gather();
    }
    printState(game, players);

    // === ROUND 2: Baron invests
    cout << "\n=== ROUND 2: Baron Invests ===\n";
    advanceTo(game, "Carol");
    carol->gather();
    advanceTo(game, "Carol");
    carol->gather();
    advanceTo(game, "Carol");
    carol->invest();
    printState(game, players);

    // === ROUND 3: Governor taxes, Spy peeks
    advanceTo(game, "Alice");
    alice->tax();
    advanceTo(game, "Bob");
    cout << "[Spy] Bob peeks Carol: " << bob->peekCoins(*carol) << " coins\n";
    printState(game, players);

    // === ROUND 4: Arrest & Sanction showcase
    advanceTo(game, "Dave");
    dave->gather();
    advanceTo(game, "Dave");
    dave->gather();
    advanceTo(game, "Dave");
    dave->arrest(*eve); // Arrest Judge
    advanceTo(game, "Carol");
    carol->gather(); // Carol builds toward coup
    printState(game, players);

    // === ROUND 5: Carol coup attempt and follow-up
    cout << "\n=== ROUND 5: Carol attempts coup too early ===\n";
    advanceTo(game, "Carol");
    try
    {
        carol->coup(*frank);
    }
    catch (const exception &e)
    {
        cout << "  → Error: " << e.what() << "\n";
    }

    while (carol->coins() < 7)
    {
        advanceTo(game, "Carol");
        carol->gather();
    }

    advanceTo(game, "Carol");
    if (frank->isActive())
    {
        cout << "[Baron] Carol coups Frank\n";
        carol->coup(*frank);
    }
    else
    {
        cout << "[Baron] Carol tried to coup Frank, but he is already eliminated.\n";
    }
    printState(game, players);

    // === ROUND 6: Bribe + Judge Blocks
    while (bob->coins() < 4)
    {
        advanceTo(game, "Bob");
        bob->gather();
    }
    advanceTo(game, "Bob");
    cout << "[Spy] Bob bribes\n";
    bob->bribe();

    advanceTo(game, "Eve");
    cout << "[Judge] Eve blocks Bob’s bribe\n";
    eve->undo(*bob);
    printState(game, players);

    // === ROUND 7: General coups Governor
    while (dave->coins() < 7)
    {
        advanceTo(game, "Dave");
        dave->gather();
    }
    advanceTo(game, "Dave");
    if (alice->isActive())
    {
        cout << "[General] Dave coups Alice\n";
        dave->coup(*alice);
    }
    else
    {
        cout << "[General] Dave tried to coup Alice, but she is already eliminated.\n";
    }
    printState(game, players);
    // === FINAL SHOWDOWN: Auto-coup until one remains ===
    cout << "\n=== FINAL SHOWDOWN ===\n";
    while (true)
    {
        int aliveCount = 0;
        for (auto *p : players)
            if (p->isActive())
                aliveCount++;
        if (aliveCount <= 1)
            break;

        Player *actor = nullptr;
        for (auto *p : players)
        {
            if (p->isActive() && p->coins() >= 7)
            {
                actor = p;
                break;
            }
        }

        if (!actor)
        {
            // No one has enough coins — force gathering
            for (auto *p : players)
            {
                if (p->isActive())
                {
                    advanceTo(game, p->name());
                    try
                    {
                        p->gather();
                    }
                    catch (...)
                    {
                    }
                }
            }
            continue;
        }

        // Find valid target
        Player *target = nullptr;
        for (auto *p : players)
        {
            if (p->isActive() && p != actor)
            {
                target = p;
                break;
            }
        }

        if (target)
        {
            advanceTo(game, actor->name());
            cout << "[" << actor->role() << "] " << actor->name() << " coups " << target->name() << "\n";
            try
            {
                actor->coup(*target);
            }
            catch (const exception &e)
            {
                cout << "  → Error: " << e.what() << "\n";
            }
            printState(game, players);
        }
    }

    // === WINNER DECLARATION
    cout << "\n=== GAME END ===\n";
    try
    {
        cout << "*** Winner: " << game.winner() << " ***\n";
    }
    catch (const exception &e)
    {
        cout << "Game not over: " << e.what() << "\n";
    }
}
