// adirofir123@gmail.com

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

class Player; // forward declaration
struct IllegalAction;

// Manages turn order, active players, elimination, winner logic,
// tracking Tax/Bribe for blocking, “block arrest,” and now “pending coups.”
class Game
{
public:
    Game() = default;

    // Called by each Player subclass in its ctor
    void addPlayer(Player *p);

    // Name of current player; throws if no players.
    const std::string &turn() const;

    // All active player names in join order.
    std::vector<std::string> players() const;

    // Eliminates the named player; next turn advances if needed.
    void eliminate(const std::string &name);

    // Advances to the next active player.
    // Also resolves any pending coup for that player before granting the turn.
    void nextTurn();

    // Returns the sole remaining player or throws if >1 remain.
    std::string winner() const;

    // Record that 'p' just took a Tax of 'amount' coins.
    void recordTax(Player *p, int amount);

    // Undo (remove) the most recent Tax record for 'p'.
    // Returns true if there was a Tax to undo.
    bool undoTax(Player *p);

    // Record that 'p' just performed a Bribe.
    void recordBribe(Player *p);

    // Undo (remove) the most recent Bribe record for 'p'.
    // Returns true if there was a Bribe to undo.
    bool undoBribe(Player *p);

    // Spy-related: block a specific player from using arrest
    void blockArrest(const std::string &name);
    bool isArrestBlocked(const std::string &name) const;
    void clearArrestBlock(const std::string &name);

    // Coup‐block related:
    // Record a newly performed (but not yet finalized) coup by attacker against victim.
    // The victim remains “alive” until the attacker’s next turn (unless blocked in the meantime).
    void recordPendingCoup(Player *attacker, Player *victim);

    // Called by a General to cancel (block) a pending coup made by “attackerName.”
    // Returns true if a matching pending coup was found and marked blocked.
    bool blockCoup(const std::string &attackerName);

    // --- PUBLIC: Struct & Getter for pendingCoups ---

    struct PendingCoup
    {
        Player *attacker;
        Player *victim;
        bool blocked;
    };

    // Const reference getter for GUI use
    const std::vector<PendingCoup> &getPendingCoups() const { return pendingCoups; }

private:
    std::vector<Player *> players_;
    size_t currentIndex_{0};

    // For blocking Tax: maps each Player pointer to the last amount gained via Tax
    std::unordered_map<Player *, int> lastTaxes;

    // For blocking Bribe: tracks which Players have just bribed
    std::unordered_set<Player *> lastBribes;

    // For blocking arrest: set of player names who cannot call arrest on their next turn
    std::unordered_set<std::string> blockedFromArrest;

    // All coups that have been performed but not yet finalized
    std::vector<PendingCoup> pendingCoups;
};
