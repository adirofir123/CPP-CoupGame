// adirofir123@gmail.com

#pragma once
#include "../Player.hpp"
#include "../exceptions.hpp"

struct Judge : Player
{
    Judge(Game &g, const std::string &name)
        : Player(g, name) {}

    std::string role() const override { return "Judge"; }

    // Undo (block a bribe): cancel target's last Bribe of 4 coins
    void undo(Player &target) override
    {
        if (game_.turn() != name_)
            throw IllegalAction("Not " + name_ + "'s turn");
        if (!target.isActive())
            throw IllegalAction("Target not active");

        // Attempt to undo their last Bribe
        if (!game_.undoBribe(&target))
        {
            throw IllegalAction("No bribe to undo from " + target.name());
        }

        // Do not refund coins — bribe is canceled but coins are lost
        game_.nextTurn();
    }
};
