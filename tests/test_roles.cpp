#include "doctest.h"
#include "Game.hpp"
#include "Roles/Governor.hpp"
#include "Roles/Spy.hpp"
#include "Roles/Baron.hpp"
#include "Roles/Judge.hpp"
#include "Roles/Merchant.hpp"
#include "Roles/General.hpp"

TEST_CASE("Governor tax gives 3 coins")
{
    Game g;
    Governor gov(g, "Alice");

    CHECK(gov.coins() == 0);
    gov.tax();
    CHECK(gov.coins() == 3);
}

TEST_CASE("Spy can peek other player's coins")
{
    Game g;
    Spy spy(g, "Bob");
    Governor gov(g, "Alice");

    // Bob acts (gather), then Alice
    spy.gather(); // Bob +1
    gov.gather(); // Alice +1

    CHECK(gov.coins() == 1);

    // Now it's Bob’s turn again:
    int seen = spy.peekCoins(gov);
    CHECK(seen == 1);

    // Confirm peek did NOT advance the turn:
    CHECK(g.turn() == "Bob");
}

TEST_CASE("Baron invest gives +3 net coins")
{
    Game g;
    Baron baron(g, "Baron");

    for (int i = 0; i < 3; ++i)
    {
        // Make sure it's Baron's turn before gathering:
        while (g.turn() != "Baron") g.nextTurn();
        baron.gather(); // 3 gathers
    }
    CHECK(baron.coins() == 3);

    // Ensure it's Baron's turn:
    while (g.turn() != "Baron") g.nextTurn();
    baron.invest(); // -3 +6
    CHECK(baron.coins() == 6);
}


TEST_CASE("Judge undo does NOT refund bribe loss")
{
    Game g;
    Judge judge(g, "Judge");
    Spy spy(g, "Spy");

    // Give each 4 coins via alternating gathers:
    for (int i = 0; i < 4; ++i)
    {
        while (g.turn() != "Judge") g.nextTurn();
        judge.gather(); // Judge +1

        while (g.turn() != "Spy") g.nextTurn();
        spy.gather(); // Spy +1
    }

    // Now give Spy a bribe (costs 4), so Spy goes to 0
    while (g.turn() != "Spy") g.nextTurn();
    spy.bribe();
    CHECK(spy.coins() == 0);

    // Then Judge cancels the bribe—Spy should remain at 0
    while (g.turn() != "Judge") g.nextTurn();
    judge.undo(spy);
    CHECK(spy.coins() == 0);
}


TEST_CASE("Cannot act when not your turn")
{
    Game g;
    Governor gov(g, "Alice");
    Spy spy(g, "Bob");

    // Bob tries to gather when it's still Alice’s turn
    CHECK_THROWS_AS(spy.gather(), const IllegalAction &);
}

TEST_CASE("Governor cannot undo a tax that never happened")
{
    Game g;
    Governor gov(g, "Alice");
    Spy spy(g, "Bob");

    // Each gathers once (to pass the turn)
    gov.gather();
    spy.gather();

    // Governor tries to undo Bob's tax — but Bob never taxed
    CHECK_THROWS_AS(gov.undo(spy), const IllegalAction &);
}

TEST_CASE("Judge cannot undo bribe if no bribe was performed")
{
    Game g;
    Judge judge(g, "Eve");
    Baron baron(g, "Carol");

    judge.gather();
    baron.gather(); // Advance turn
    CHECK_THROWS_AS(judge.undo(baron), const IllegalAction &);
}

TEST_CASE("Baron cannot invest without 3 coins")
{
    Game g;
    Baron baron(g, "Carol");

    baron.gather(); // only 1 coin
    CHECK_THROWS_AS(baron.invest(), const IllegalAction &);
}

TEST_CASE("Player cannot coup with less than 7 coins")
{
    Game g;
    Governor a(g, "Alice");
    Spy b(g, "Bob");

    // Alternate gathers to reach 6 coins each
    for (int i = 0; i < 3; ++i) {
        while (g.turn() != "Alice") g.nextTurn();
        a.gather();
        while (g.turn() != "Bob") g.nextTurn();
        b.gather();
    }
    // Now both have 3 coins. Give Alice 3 more (6 total):
    while (g.turn() != "Alice") g.nextTurn();
    a.gather(); // 4
    while (g.turn() != "Bob") g.nextTurn();
    b.gather();
    while (g.turn() != "Alice") g.nextTurn();
    a.gather(); // 5
    while (g.turn() != "Bob") g.nextTurn();
    b.gather();
    while (g.turn() != "Alice") g.nextTurn();
    a.gather(); // 6

    CHECK(a.coins() == 6);

    // Try to coup with only 6 coins
    CHECK_THROWS_AS(a.coup(b), const IllegalAction &);
}

TEST_CASE("Spy cannot peek out of turn")
{
    Game g;
    Spy spy(g, "Bob");
    Governor gov(g, "Alice");

    while (g.turn() == "Bob")
        g.nextTurn(); // Make sure it's not Spy's turn
    CHECK_THROWS_AS(spy.peekCoins(gov), const IllegalAction &);
}

TEST_CASE("Merchant cannot be arrested if they lack 2 coins")
{
    Game g;
    Merchant merchant(g, "Frank");
    Governor gov(g, "Gov");

    // Give Merchant exactly 1 coin
    merchant.addCoins(1);
    CHECK(merchant.coins() == 1);

    while (g.turn() != "Gov") g.nextTurn();
    CHECK_THROWS_AS(gov.arrest(merchant), const IllegalAction &);
}

