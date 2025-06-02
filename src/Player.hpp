// adirofir123@gmail.com

#pragma once

#include <string>
#include "Game.hpp"
#include "exceptions.hpp"
/**
 * Base class for all Coup game roles.
 * Registers with the Game upon construction and provides
 * common action interfaces and helpers.
 */
class Player
{
public:
    Player(Game &g, const std::string &name)
        : game_(g), name_(name), coins_(0), active_(true)
    {
        game_.addPlayer(this);
    }

    virtual ~Player() = default;

    // Core interface—override in subclasses as needed:
    virtual void gather();
    virtual void tax();
    virtual void bribe();
    virtual void arrest(Player &target);
    virtual void sanction(Player &target);
    virtual void coup(Player &target);

    // Role-specific extensions (no-ops by default):
    virtual void undo(Player &target) { (void)target; }
    virtual void invest() {}
    virtual int peekCoins(const Player &p)
    {
        (void)p;
        return -1;
    }

    // Resets any turn-limited abilities (e.g., Spy peek)
    virtual void startTurn() { hasPeekedThisTurn = false; }

    virtual void onArrested() {}
    virtual int sanctionPenalty() const { return 0; }

    // Role name for GUI/debug
    virtual std::string role() const { return "Player"; }

    // Helpers for roles to adjust coins:
    void stealCoins(Player &target, int amount);
    void penalize(Player &target, int amount);
    void addCoins(int amount) { changeCoins(amount); }

    // Sanction-related API:
    void setSanctioned(bool value) { sanctioned_ = value; }
    bool isSanctioned() const { return sanctioned_; }

    // Turn logic helper:
    bool mustCoup() const { return coins_ >= 10; }

    // Introspection:
    const std::string &name() const { return name_; }
    int coins() const { return coins_; }
    bool isActive() const { return active_; }

    // Called by Game to eliminate player
    void deactivate() { active_ = false; }

    // Reset the bribe turns
    void resetBribeActions() { bribeActionsLeft = 0; }

protected:
    void changeCoins(int delta);

    Game &game_;
    std::string name_;
    int coins_;
    bool active_;
    bool sanctioned_ = false;
    int bribeActionsLeft = 0;
    bool hasPeekedThisTurn = false; // for Spy peek logic
};
