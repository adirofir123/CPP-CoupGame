// adirofir123@gmail.com

#pragma once
#include "../Player.hpp"
#include "exceptions.hpp"

struct Spy : Player
{
    Spy(Game &g, const std::string &name)
        : Player(g, name) {}

    std::string role() const override { return "Spy"; }

    int peekCoins(const Player &target) override
    {
        if (game_.turn() != name_)
            throw IllegalAction("Not " + name_ + "'s turn");

        if (hasPeekedThisTurn)
            throw IllegalAction("Spy can peek only once per turn");

        hasPeekedThisTurn = true;
        return target.coins();
    }

    // Prevent a target player from using arrest on their next turn
    void blockArrest(Player &target)
    {
        if (!target.isActive())
            throw IllegalAction("Target not active");
        game_.blockArrest(target.name());
    }

    void startTurn() override
    {
        hasPeekedThisTurn = false; // Reset peek-per-turn flag
    }
};
