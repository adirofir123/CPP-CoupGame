#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Game.hpp"
#include "Roles/Governor.hpp"
#include "exceptions.hpp"
#include "Roles/Spy.hpp"

TEST_CASE("Basic test - game starts empty")
{
    Game g;

    // No players yet:
    CHECK(g.players().empty());

    // Asking for turn or winner when no players exist → exception
    CHECK_THROWS_AS(g.turn(), const IllegalAction &);
    CHECK_THROWS_AS(g.winner(), const IllegalAction &);
}

TEST_CASE("Constructing a single Governor does not throw")
{
    Game g;
    CHECK_NOTHROW(Governor gov(g, "Alice"));
}

TEST_CASE("Adding one player updates state correctly")
{
    Game g;
    Governor gov(g, "Alice");

    auto plist = g.players();
    CHECK(plist.size() == 1);
    CHECK(plist[0] == "Alice");

    CHECK(g.turn() == "Alice");

    std::cout << "[DEBUG] Before winner(): active = " << gov.isActive() << ", coins = " << gov.coins() << "\n";
    CHECK(g.winner() == "Alice");
    std::cout << "[DEBUG] Passed winner()\n";

    g.eliminate("Alice");

    CHECK(g.players().empty());
    CHECK_THROWS_AS(g.turn(), const IllegalAction &);
    CHECK_THROWS_AS(g.winner(), const IllegalAction &);
}
