#include "doctest.h"
#include "Game.hpp"
#include "Roles/Spy.hpp"
#include "Roles/Governor.hpp"
#include "exceptions.hpp"

TEST_CASE("Basic test - single player actions") {
    Game g;
    Spy spy(g, "Bob");

    // Only one player total
    CHECK(g.players().size() == 1);
    CHECK(g.turn() == "Bob");
    CHECK(spy.coins() == 0);
    CHECK(spy.isActive());

    // Gather should succeed (→ coins=1), turn would advance, but no other active players → back to Bob
    spy.gather();
    CHECK(spy.coins() == 1);
    CHECK(g.turn() == "Bob");

    // Tax should succeed (→ +2 → coins=3)
    spy.tax();
    CHECK(spy.coins() == 3);
    CHECK(g.turn() == "Bob");

    // Bribe requires 4 coins: since Bob has only 3, expect exception
    CHECK_THROWS_AS(spy.bribe(), const IllegalAction &);

    // Add one more coin with gather, then bribe (→ pay 4 → coins=0)
    spy.gather();
    CHECK(spy.coins() == 4);
    spy.bribe();
    CHECK(spy.coins() == 0);
}

TEST_CASE("Multi-player turn enforcement") {
    Game g;
    Spy spy(g, "Bob");
    Governor gov(g, "Alice");

    // Bob goes first:
    CHECK(g.turn() == "Bob");
    // Alice tries to gather out of turn → exception
    CHECK_THROWS_AS(gov.gather(), const IllegalAction &);

    // Correct turn: Bob gathers → coins=1, turn->Alice
    spy.gather();
    CHECK(spy.coins() == 1);
    CHECK(g.turn() == "Alice");

    // Now Alice taxes (→ +3 → coins=3), turn->Bob
    gov.tax();
    CHECK(gov.coins() == 3);
    CHECK(g.turn() == "Bob");
}
