// adirofir123@gmail.com

#include "Game.hpp"
#include "Player.hpp"
#include "exceptions.hpp"
#include <algorithm>
#include <stdexcept>

// Add a player to the game
void Game::addPlayer(Player *p)
{
    players_.push_back(p);
}

// Return the name of the current player
const std::string &Game::turn() const
{
    if (players_.empty())
    {
        throw IllegalAction("No players in game");
    }
    if (!players_[currentIndex_]->isActive())
    {
        throw IllegalAction("Current player is not active");
    }
    return players_[currentIndex_]->name();
}

// Return a list of active players' names
std::vector<std::string> Game::players() const
{
    std::vector<std::string> out;
    for (auto *p : players_)
    {
        if (p->isActive())
            out.push_back(p->name());
    }
    return out;
}

void Game::eliminate(const std::string &name)
{

    for (auto *p : players_)
    {
        if (p->name() == name)
        {
            if (!p->isActive())
                throw IllegalAction("Player '" + name + "' already eliminated");
            p->deactivate();

            // Count active players:
            size_t activeCount = 0;
            for (auto *q : players_)
            {
                if (q->isActive())
                    ++activeCount;
            }

            // If the eliminated player was the current player, advance turn to next active
            if (players_[currentIndex_]->name() == name && activeCount > 0)
            {
                nextTurn();
            }
            return;
        }
    }
    throw IllegalAction("Player '" + name + "' not found");
}

void Game::nextTurn()
{
    if (players_.empty())
        throw IllegalAction("No players to advance turn");

    // Finalize any pending coups by current player
    for (auto it = pendingCoups.begin(); it != pendingCoups.end();)
    {
        if (it->attacker == players_[currentIndex_])
        {
            if (!it->blocked && it->victim->isActive())
            {
                eliminate(it->victim->name());
            }
            it = pendingCoups.erase(it);
        }
        else
        {
            ++it;
        }
    }

    // Advance to next active player
    size_t n = players_.size();
    for (size_t i = 1; i <= n; ++i)
    {
        size_t idx = (currentIndex_ + i) % n;
        if (players_[idx]->isActive())
        {
            currentIndex_ = idx;

            // Reset state for new player
            players_[currentIndex_]->setSanctioned(false);
            clearArrestBlock(players_[currentIndex_]->name());
            players_[currentIndex_]->resetBribeActions();
            players_[currentIndex_]->startTurn(); // Role-specific logic

            return;
        }
    }

    throw IllegalAction("No active players remain");
}

// Return the winner if only one active player remains
std::string Game::winner() const
{
    std::vector<Player *> alive;

    for (auto *p : players_)
    {
        if (p->isActive())
        {
            alive.push_back(p);
        }
    }

    if (alive.size() == 1)
    {
        return alive.front()->name();
    }
    else if (alive.empty())
    {
        throw IllegalAction("No active players remaining.");
    }
    else
    {
        throw IllegalAction("Game still in progress");
    }
}

// Record that a player just took a Tax of 'amount' coins
void Game::recordTax(Player *p, int amount)
{
    lastTaxes[p] = amount;
}

// Undo a player's last Tax
bool Game::undoTax(Player *p)
{
    auto it = lastTaxes.find(p);
    if (it == lastTaxes.end())
        return false;
    lastTaxes.erase(it);
    return true;
}

// Record that a player just performed a Bribe
void Game::recordBribe(Player *p)
{
    lastBribes.insert(p);
}

// Undo a player's last Bribe
bool Game::undoBribe(Player *p)
{
    auto it = lastBribes.find(p);
    if (it == lastBribes.end())
        return false;
    lastBribes.erase(it);
    return true;
}

// Block a specific player from using arrest on their next turn
void Game::blockArrest(const std::string &name)
{
    blockedFromArrest.insert(name);
}

// Check if a player is blocked from arresting
bool Game::isArrestBlocked(const std::string &name) const
{
    return (blockedFromArrest.find(name) != blockedFromArrest.end());
}

// Clear the arrest-block status for a player
void Game::clearArrestBlock(const std::string &name)
{
    blockedFromArrest.erase(name);
}

void Game::recordPendingCoup(Player *attacker, Player *victim)
{
    pendingCoups.push_back({attacker, victim, false});
}

bool Game::blockCoup(const std::string &attackerName)
{
    for (auto &coup : pendingCoups)
    {
        if (!coup.blocked && coup.attacker->name() == attackerName)
        {
            coup.blocked = true;
            return true;
        }
    }
    return false;
}
