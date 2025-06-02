// adirofir123@gmail.com

#pragma once
#include "../Player.hpp"
#include "exceptions.hpp"

struct General : Player
{
    General(Game &g, const std::string &name)
        : Player(g, name) {}

    std::string role() const override { return "General"; }

 
    void arrest(Player &target) override
    {
        if (game_.turn() != name_)
            throw IllegalAction("Not " + name_ + "'s turn");
        if (!target.isActive())
            throw IllegalAction("Target not active");

        stealCoins(target, 1);
        game_.nextTurn();
    }

    // Block a coup: pay 5 coins and immediately react
    void undo(Player &attacker) override
    {
        if (game_.turn() != name_)
            throw IllegalAction("Not " + name_ + "'s turn");
        if (coins() < 5)
            throw IllegalAction("Need 5 coins to block coup");

        changeCoins(-5);
        game_.blockArrest(attacker.name());
        game_.nextTurn();
    }
};
