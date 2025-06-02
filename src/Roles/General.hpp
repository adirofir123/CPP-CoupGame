// adirofir123@gmail.com

#pragma once
#include "../Player.hpp"
#include "exceptions.hpp"

struct General : Player
{
    General(Game &g, const std::string &name)
        : Player(g, name) {}

    std::string role() const override { return "General"; }

    void onArrested() override
    {
        changeCoins(+1);
    }

    // Block a coup: pay 5 coins and immediately react
    void undo(Player &attacker) override
    {
        if (game_.turn() != name_)
            throw IllegalAction("Not " + name_ + "'s turn");
        if (coins() < 5)
            throw IllegalAction("Need 5 coins to block coup");

        // Attempt to block a pending coup
        if (!game_.blockCoup(attacker.name()))
            throw IllegalAction("No pending coup from " + attacker.name());

        changeCoins(-5); // Pay 5 coins to block the coup
        game_.nextTurn();
    }
};
