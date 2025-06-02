// main_gui.cpp

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include <string>

#include "src/Game.hpp"
#include "src/Player.hpp"
#include "src/Roles/Governor.hpp"
#include "src/Roles/Spy.hpp"
#include "src/Roles/Baron.hpp"
#include "src/Roles/General.hpp"
#include "src/Roles/Judge.hpp"
#include "src/Roles/Merchant.hpp"

enum class ActionMode
{
    None,
    Arrest,
    Sanction,
    Coup,
    GovBlockTax,
    JudgeCancelBribe,
    SpyPeek,
    BaronInvest,
    GeneralBlockCoup
};

int main()
{
    Game game;
    std::vector<Player *> players = {
        new Governor(game, "Alice"),
        new Spy(game, "Bob"),
        new Baron(game, "Carol"),
        new General(game, "Dave"),
        new Judge(game, "Eve"),
        new Merchant(game, "Frank")};

    sf::RenderWindow window(sf::VideoMode(900, 700), "Coup‐Style Game GUI");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        std::cerr << "Failed to load font\n";
        return 1;
    }

    sf::Text errorText("", font, 20);
    errorText.setFillColor(sf::Color::Red);
    errorText.setPosition(10, 10);
    sf::Clock errorClock;
    bool showError = false;

    struct Button
    {
        sf::RectangleShape box;
        sf::Text label;
        ActionMode mode;
    };
    std::vector<Button> buttons;

    const std::vector<std::pair<std::string, ActionMode>> commonActions = {
        {"Gather", ActionMode::None},
        {"Tax", ActionMode::None},
        {"Bribe", ActionMode::None},
        {"Arrest", ActionMode::Arrest},
        {"Sanction", ActionMode::Sanction},
        {"Coup", ActionMode::Coup}};

    for (size_t i = 0; i < commonActions.size(); ++i)
    {
        Button b;
        b.box.setSize({140, 40});
        b.box.setFillColor(sf::Color(100, 100, 100));
        b.box.setOutlineColor(sf::Color::White);
        b.box.setOutlineThickness(2);
        b.box.setPosition(10 + i * 150, 620);

        b.label.setFont(font);
        b.label.setString(commonActions[i].first);
        b.label.setCharacterSize(18);
        b.label.setFillColor(sf::Color::White);
        b.label.setPosition(b.box.getPosition().x + 10, b.box.getPosition().y + 8);

        b.mode = commonActions[i].second;
        buttons.push_back(b);
    }

    std::vector<Button> perkButtons;
    const std::vector<std::pair<std::string, ActionMode>> rolePerks = {
        {"Block Tax (Gov)", ActionMode::GovBlockTax},
        {"Cancel Bribe (Judge)", ActionMode::JudgeCancelBribe},
        {"Peek Coins (Spy)", ActionMode::SpyPeek},
        {"Invest (Baron)", ActionMode::BaronInvest},
        {"Block Coup (General)", ActionMode::GeneralBlockCoup}};

    for (size_t i = 0; i < rolePerks.size(); ++i)
    {
        Button b;
        b.box.setSize({180, 40});
        b.box.setFillColor(sf::Color(80, 80, 120));
        b.box.setOutlineColor(sf::Color::White);
        b.box.setOutlineThickness(2);
        b.box.setPosition(10 + i * 190, 560);

        b.label.setFont(font);
        b.label.setString(rolePerks[i].first);
        b.label.setCharacterSize(18);
        b.label.setFillColor(sf::Color::White);
        b.label.setPosition(b.box.getPosition().x + 10, b.box.getPosition().y + 8);

        b.mode = rolePerks[i].second;
        perkButtons.push_back(b);
    }

    ActionMode currentMode = ActionMode::None;

    auto getCurrentPlayerPtr = [&]() -> Player *
    {
        for (auto *p : players)
            if (p->name() == game.turn())
                return p;
        return nullptr;
    };

    while (window.isOpen())
    {
        sf::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
                window.close();

            if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f click((float)e.mouseButton.x, (float)e.mouseButton.y);

                // 1) Action buttons (gather, tax, bribe, etc)
                bool buttonHandled = false;
                for (auto &btn : buttons)
                {
                    if (btn.box.getGlobalBounds().contains(click))
                    {
                        Player *actor = getCurrentPlayerPtr();
                        if (!actor)
                        {
                            errorText.setString("No current player");
                            showError = true;
                            errorClock.restart();
                            break;
                        }

                        std::string action = btn.label.getString();
                        try
                        {
                            if (action == "Gather")
                                actor->gather();
                            else if (action == "Tax")
                                actor->tax();
                            else if (action == "Bribe")
                                actor->bribe();
                            else
                                currentMode = btn.mode; // Arrest, Sanction, Coup, etc.
                        }
                        catch (std::exception &ex)
                        {
                            errorText.setString(ex.what());
                            showError = true;
                            errorClock.restart();
                        }

                        buttonHandled = true;
                        break;
                    }
                }

                if (buttonHandled)
                    continue;

                // 2) Perk buttons
                for (auto &pbtn : perkButtons)
                {
                    if (pbtn.box.getGlobalBounds().contains(click))
                    {
                        Player *actor = getCurrentPlayerPtr();
                        if (!actor)
                        {
                            errorText.setString("No current player");
                            showError = true;
                            errorClock.restart();
                            break;
                        }

                        try
                        {
                            if (pbtn.mode == ActionMode::GovBlockTax ||
                                pbtn.mode == ActionMode::JudgeCancelBribe)
                            {
                                // For these, user must click a target player card below (handled in next section)
                                currentMode = pbtn.mode;
                            }
                            else if (pbtn.mode == ActionMode::SpyPeek)
                            {
                                currentMode = ActionMode::SpyPeek;
                            }
                            else if (pbtn.mode == ActionMode::BaronInvest)
                            {
                                actor->invest();
                            }
                            else if (pbtn.mode == ActionMode::GeneralBlockCoup)
                            {
                                currentMode = ActionMode::GeneralBlockCoup;
                            }
                        }
                        catch (std::exception &ex)
                        {
                            errorText.setString(ex.what());
                            showError = true;
                            errorClock.restart();
                        }

                        buttonHandled = true;
                        break;
                    }
                }

                if (buttonHandled)
                    continue;

                // 3) Player cards for target selection (for Arrest/Sanction/Coup/Perk)
                std::vector<Player *> activePlayers;
                for (auto *p : players)
                    if (p->isActive())
                        activePlayers.push_back(p);

                for (size_t i = 0; i < activePlayers.size(); ++i)
                {
                    float y = 30 + i * 50;
                    sf::FloatRect cardRect(20, y, 860, 40);
                    if (cardRect.contains(click))
                    {
                        Player *actor = getCurrentPlayerPtr();
                        Player *target = activePlayers[i];
                        if (!actor)
                        {
                            errorText.setString("No current player");
                            showError = true;
                            errorClock.restart();
                            break;
                        }
                        if (target->name() == actor->name())
                        {
                            // Cannot target yourself!
                            errorText.setString("Cannot target yourself!");
                            showError = true;
                            errorClock.restart();
                            currentMode = ActionMode::None;
                            break;
                        }

                        try
                        {
                            // Only execute on click if a mode is set
                            if (currentMode == ActionMode::Arrest)
                                actor->arrest(*target);
                            else if (currentMode == ActionMode::Sanction)
                                actor->sanction(*target);
                            else if (currentMode == ActionMode::Coup)
                                actor->coup(*target);
                            else if (currentMode == ActionMode::GovBlockTax ||
                                     currentMode == ActionMode::JudgeCancelBribe)
                                actor->undo(*target);
                            else if (currentMode == ActionMode::SpyPeek)
                            {
                                int c = actor->peekCoins(*target);
                                errorText.setString(target->name() + " has " + std::to_string(c) + " coins");
                                showError = true;
                                errorClock.restart();
                            }
                            else if (currentMode == ActionMode::GeneralBlockCoup)
                                actor->undo(*target);
                        }
                        catch (std::exception &ex)
                        {
                            errorText.setString(ex.what());
                            showError = true;
                            errorClock.restart();
                        }
                        // Always exit special mode after action (success or error)
                        currentMode = ActionMode::None;
                        break;
                    }
                }
            } // end MouseButtonPressed
        } // end event loop

        if (showError && errorClock.getElapsedTime().asSeconds() > 2.f)
        {
            showError = false;
            errorText.setString("");
        }

        window.clear(sf::Color(40, 40, 40));

        // ---------------------------
        // Draw the player “cards”
        // ---------------------------
        std::vector<Player *> activePlayers;
        for (auto *p : players)
            if (p->isActive())
                activePlayers.push_back(p);

        for (size_t i = 0; i < activePlayers.size(); ++i)
        {
            float y = 30 + i * 50;

            sf::RectangleShape card({860, 40});
            card.setPosition(20, y);
            if (game.turn() == activePlayers[i]->name())
                card.setFillColor(sf::Color(100, 100, 200));
            else
                card.setFillColor(sf::Color(60, 60, 60));
            card.setOutlineColor(sf::Color::White);
            card.setOutlineThickness(2);
            window.draw(card);

            std::string line = activePlayers[i]->name() + " (" + activePlayers[i]->role() + "): " + std::to_string(activePlayers[i]->coins()) + " coins";
            if (activePlayers[i]->isSanctioned())
                line += "  [SANCTIONED]";
            sf::Text txt(line, font, 18);
            txt.setFillColor(activePlayers[i]->isSanctioned() ? sf::Color::Red : sf::Color::White);
            txt.setPosition(30, y + 8);
            window.draw(txt);
        }

        // Draw common action buttons
        for (auto &b : buttons)
        {
            Player *actor = getCurrentPlayerPtr();
            bool canAct = (actor && actor->name() == game.turn());
            bool sanctioned = (actor && actor->isSanctioned());
            bool mustCoup = (actor && actor->coins() >= 10);

            std::string label = b.label.getString();
            bool enabled = false;

            if (canAct && !mustCoup)
            {
                if (label == "Gather")
                    enabled = !sanctioned;
                else if (label == "Tax")
                    enabled = !sanctioned;
                else if (label == "Bribe")
                    enabled = (actor->coins() >= 4);
                else if (label == "Arrest")
                    enabled = true;
                else if (label == "Sanction")
                    enabled = (actor->coins() >= 3);
                else if (label == "Coup")
                    enabled = (actor->coins() >= 7);
            }

            b.box.setFillColor(enabled ? sf::Color(100, 100, 100) : sf::Color(50, 50, 50));
            b.label.setFillColor(enabled ? sf::Color::White : sf::Color(120, 120, 120));
            window.draw(b.box);
            window.draw(b.label);
        }

        // Draw role-specific perk
        Player *current = getCurrentPlayerPtr();
        if (current && current->name() == game.turn())
        {
            std::string role = current->role();
            for (auto &pbtn : perkButtons)
            {
                bool show = false, enabled = false;
                if (role == "Governor" && pbtn.mode == ActionMode::GovBlockTax)
                {
                    show = true;
                    enabled = true;
                }
                else if (role == "Judge" && pbtn.mode == ActionMode::JudgeCancelBribe)
                {
                    show = true;
                    enabled = true;
                }
                else if (role == "Spy" && pbtn.mode == ActionMode::SpyPeek)
                {
                    show = true;
                    enabled = true;
                }
                else if (role == "Baron" && pbtn.mode == ActionMode::BaronInvest)
                {
                    show = true;
                    enabled = (current->coins() >= 3);
                }
                else if (role == "General" && pbtn.mode == ActionMode::GeneralBlockCoup)
                {
                    show = true;
                    bool foundPending = false;
                    for (auto &pc : game.getPendingCoups())
                    {
                        if (!pc.blocked && pc.attacker->isActive())
                        {
                            foundPending = true;
                            break;
                        }
                    }
                    enabled = (foundPending && current->coins() >= 5);
                }

                if (show)
                {
                    pbtn.box.setFillColor(enabled ? sf::Color(80, 80, 120) : sf::Color(40, 40, 60));
                    pbtn.label.setFillColor(enabled ? sf::Color::White : sf::Color(120, 120, 120));
                    window.draw(pbtn.box);
                    window.draw(pbtn.label);
                }
            }
        }

        // Draw mode indicator
        sf::Text modeLabel("Mode: " +
                               std::string(
                                   (currentMode == ActionMode::None) ? "Normal" : (currentMode == ActionMode::Arrest) ? "Arrest"
                                                                              : (currentMode == ActionMode::Sanction) ? "Sanction"
                                                                              : (currentMode == ActionMode::Coup)     ? "Coup"
                                                                                                                      : "Perk"),
                           font, 18);
        modeLabel.setFillColor(sf::Color::Cyan);
        modeLabel.setPosition(10, 580);
        window.draw(modeLabel);

        // Display winner if game is over
        try
        {
            std::string winner = game.winner();
            sf::Text winnerText(" Winner: " + winner, font, 36);
            winnerText.setFillColor(sf::Color::Green);
            winnerText.setStyle(sf::Text::Bold);
            winnerText.setPosition(250, 300);
            window.draw(winnerText);
        }
        catch (const std::exception &)
        {
            // Game not over, no winner
        }

        if (showError && errorClock.getElapsedTime().asSeconds() > 2.f)
        {
            showError = false;
            errorText.setString("");
        }

        if (showError)
            window.draw(errorText);

        window.display();
    }

    for (auto *p : players)
        delete p;
    return 0;
}
