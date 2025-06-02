// adirofir123@gmail.com

#pragma once
#include "../Player.hpp"
#include "../exceptions.hpp"

struct Judge : Player
{
    Judge(Game &g, const std::string &name)
        : Player(g, name) {}

    std::string role() const override { return "Judge"; }

    // Cancels target's bribe; target still loses 4 coins
    void undo(Player &target) override
    {
        if (game_.turn() != name_)
            throw IllegalAction("Not " + name_ + "'s turn");
        if (!target.isActive())
            throw IllegalAction("Target not active");

        if (!game_.undoBribe(&target))
            throw IllegalAction("No bribe to undo from " + target.name());

        // Bribe canceled; coins are lost
        game_.nextTurn();
    }

    // Attacker must pay 1 extra coin to the bank when sanctioning this Judge
    int sanctionPenalty() const override
    {
        return 1;
    }
};
