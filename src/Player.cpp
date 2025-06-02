// adirofir123@gmail.com

#include "Player.hpp"
#include "exceptions.hpp"
#include "Roles/Merchant.hpp"
#include "Roles/Judge.hpp"
#include "Roles/General.hpp"
#include "Roles/Spy.hpp"
#include <algorithm> // for std::min

void Player::changeCoins(int delta)
{
    coins_ += delta;
    if (coins_ < 0)
    {
        coins_ = 0;
    }
}

void Player::stealCoins(Player &target, int amount)
{
    int stolen = std::min(amount, target.coins());
    if (stolen > 0)
    {
        target.changeCoins(-stolen);
        changeCoins(+stolen);
    }
}

void Player::penalize(Player &target, int amount)
{
    int lost = std::min(amount, target.coins());
    if (lost > 0)
    {
        target.changeCoins(-lost);
    }
}

void Player::gather()
{
    if (game_.turn() != name_)
        throw IllegalAction("Not " + name_ + "'s turn");
    if (mustCoup())
        throw IllegalAction(name_ + " has 10 or more coins and must perform a coup");
    if (sanctioned_)
        throw IllegalAction(name_ + " is sanctioned and cannot gather");

    changeCoins(+1);
    sanctioned_ = false;
    if (bribeActionsLeft > 0)
    {
        bribeActionsLeft--;
        if (bribeActionsLeft == 0)
        {
            game_.nextTurn();
        }
        // Stay on player for next bonus action, or pass if that was last
    }
    else
    {
        game_.nextTurn();
    }
}

void Player::tax()
{
    if (game_.turn() != name_)
        throw IllegalAction("Not " + name_ + "'s turn");
    if (mustCoup())
        throw IllegalAction(name_ + " has 10 or more coins and must perform a coup");
    if (sanctioned_)
        throw IllegalAction(name_ + " is sanctioned and cannot tax");

    changeCoins(+2);
    sanctioned_ = false;
    if (bribeActionsLeft > 0)
    {
        bribeActionsLeft--;
        if (bribeActionsLeft == 0)
        {
            game_.nextTurn();
        }
        // Stay on player for next bonus action, or pass if that was last
    }
    else
    {
        game_.nextTurn();
    }
}

void Player::bribe()
{
    if (game_.turn() != name_)
        throw IllegalAction("Not " + name_ + "'s turn");
    if (mustCoup())
        throw IllegalAction(name_ + " has 10 or more coins and must perform a coup");
    if (coins_ < 4)
        throw IllegalAction("Not enough coins to bribe");
    changeCoins(-4);

    // Player gets two extra actions in a row (the bribe doesn't end turn)
    bribeActionsLeft = 2;
    game_.recordBribe(this);
}

void Player::arrest(Player &target)
{
    if (game_.turn() != name_)
        throw IllegalAction("Not " + name_ + "'s turn");
    if (!target.isActive())
        throw IllegalAction("Target not active");
    if (game_.isArrestBlocked(name_))
        throw IllegalAction(name_ + " is blocked from arresting this turn");

    target.onArrested();

    // --- MERCHANT ---
    if (auto *merchant = dynamic_cast<Merchant *>(&target))
    {
        if (merchant->coins() < 2)
            throw IllegalAction("Merchant must have at least 2 coins to be arrested");
        merchant->changeCoins(-2); // Lose 2 to the bank
        // Attacker gains nothing
    }
    // --- GENERAL ---
    else if (auto *general = dynamic_cast<General *>(&target))
    {
        if (general->coins() > 0)
        {
            general->changeCoins(-1); // General loses 1
            changeCoins(+1);          // Attacker gains 1
            general->changeCoins(+1); // General regains 1 (net 0)
        }
        // If general has 0 coins, nothing happens
    }
    // --- DEFAULT CASE ---
    else
    {
        if (target.coins() > 0)
        {
            target.changeCoins(-1);
            changeCoins(+1);
        }
        // If target has 0 coins, nothing happens
    }

    // --- Bribe logic ---
    if (bribeActionsLeft > 0)
    {
        bribeActionsLeft--;
        if (bribeActionsLeft == 0)
            game_.nextTurn();
        // else: stay on player for next bribe action
    }
    else
    {
        game_.nextTurn();
    }
}

void Player::sanction(Player &target)
{
    if (game_.turn() != name_)
        throw IllegalAction("Not " + name_ + "'s turn");
    if (!target.isActive())
        throw IllegalAction("Target not active");
    if (coins_ < 3)
        throw IllegalAction("Not enough coins to apply sanction");

    int penalty = target.sanctionPenalty(); // Default is 0 unless overridden
    if (coins_ < 3 + penalty)
        throw IllegalAction("Not enough coins to sanction this player (additional penalty required)");

    changeCoins(-3 - penalty); // Subtract full cost including penalty
    target.setSanctioned(true);

    if (bribeActionsLeft > 0)
    {
        bribeActionsLeft--;
        if (bribeActionsLeft == 0)
        {
            game_.nextTurn();
        }
        // Otherwise, allow another bonus action
    }
    else
    {
        game_.nextTurn();
    }
}

// Modified coup: record a pending coup instead of immediate elimination
void Player::coup(Player &target)
{
    if (game_.turn() != name_)
        throw IllegalAction("Not " + name_ + "'s turn");
    if (!target.isActive())
        throw IllegalAction("Target not active");
    if (coins_ < 7)
        throw IllegalAction("Need 7 coins to perform a coup");

    // Deduct 7 coins immediately
    changeCoins(-7);

    // Delay elimination until next turn
    game_.recordPendingCoup(this, &target);

    // End turn (or bribe bonus logic)
    if (bribeActionsLeft > 0)
    {
        bribeActionsLeft--;
        if (bribeActionsLeft == 0)
        {
            game_.nextTurn();
        }
        // Otherwise, same player acts again
    }
    else
    {
        game_.nextTurn();
    }
}
