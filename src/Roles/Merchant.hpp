// adirofir123@gmail.com

#pragma once
#include "../Player.hpp"
#include "../exceptions.hpp"

struct Merchant : Player
{
    Merchant(Game &g, const std::string &name)
        : Player(g, name) {}

    std::string role() const override { return "Merchant"; }

    // Start-of-turn bonus: +1 coin if 3 or more coins
    void startTurn() override
    {
        if (coins() >= 3)
        {
            changeCoins(+1);
        }
    }
};
