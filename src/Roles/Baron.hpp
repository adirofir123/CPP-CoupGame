// adirofir123@gmail.com

#pragma once
#include "../Player.hpp"
#include "exceptions.hpp"

struct Baron : Player
{
  Baron(Game &g, const std::string &name)
      : Player(g, name) {}

  std::string role() const override { return "Baron"; }

  // Invest: pay 3 to gain 6
  void invest() override
  {
    if (game_.turn() != name_)
      throw IllegalAction("Not " + name_ + "'s turn");
    if (coins() < 3)
      throw IllegalAction("Need 3 coins to invest");
    changeCoins(-3);
    changeCoins(+6);
    game_.nextTurn();
  }
};
