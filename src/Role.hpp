// adirofir123@gmail.com

#pragma once

#include <string>
#include "exceptions.hpp"

class Player; // forward declaration

/**
 * Abstract base for all player roles.
 * Provides hooks to customize game actions:
 *  - Tax amount
 *  - Blocking capabilities
 *  - Compensation on sanctions/arrests
 *  - Unique abilities (e.g. Spy peek)
 */
struct Role
{
    virtual ~Role() = default;

    // Display name of the role
    virtual std::string name() const = 0;

    // Tax: default gain is 2 coins
    virtual int onTaxAmount() const { return 2; }
    // Can block another player's tax action?
    virtual bool canBlockTax(const Player &actor) const
    {
        (void)actor;
        return false;
    }

    // Bribe: can this role block a bribe?
    virtual bool canBlockBribe(const Player &actor) const
    {
        (void)actor;

        return false;
    }

    // Arrest: can this role block an arrest?
    virtual bool canBlockArrest(const Player &actor) const
    {
        (void)actor;

        return false;
    }
    // Adjust coins lost when arrested: default lose 1
    virtual int onArrestCoinsLost() const { return 1; }

    // Sanction: can block sanction?
    virtual bool canBlockSanction(const Player &actor) const
    {
        (void)actor;

        return false;
    }
    // Compensation when targeted by sanction: default none
    virtual int onSanctionCompensation() const { return 0; }

    // Coup: can block a coup against this player?
    virtual bool canBlockCoup(const Player &actor) const
    {
        (void)actor;

        return false;
    }

    // Spy ability: peek at target's coins (no default action)
    virtual void onSpyPeek(const Player &target) { (void)target; }
};
