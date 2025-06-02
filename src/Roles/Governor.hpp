// adirofir123@gmail.com

#pragma once
#include "../Player.hpp"
#include "../exceptions.hpp"

struct Governor : Player
{
    Governor(Game &g, const std::string &name)
        : Player(g, name) {}

    std::string role() const override { return "Governor"; }

    // Tax: gain 3 coins (recorded for possible undo)
    void tax() override
    {
        if (game_.turn() != name_)
            throw IllegalAction("Not " + name_ + "'s turn");
        changeCoins(+3);
        game_.recordTax(this, 3);
        game_.nextTurn();
    }

    // Undo (block tax): cancel the target's last Tax of 3 coins
    void undo(Player &target) override
    {
        if (game_.turn() != name_)
            throw IllegalAction("Not " + name_ + "'s turn");
        if (!target.isActive())
            throw IllegalAction("Target not active");

        // Attempt to undo their last Tax
        if (!game_.undoTax(&target))
        {
            throw IllegalAction("No tax to undo from " + target.name());
        }

        // Reverse the 3 coins that target gained
        stealCoins(target, 3);
        game_.nextTurn();
    }
};
