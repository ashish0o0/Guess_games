// #include <SFML/Graphics.hpp>
// #include <SFML/Window.hpp>
#include <vector>
#include <algorithm>
#include <random>
#include <iostream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

const int GRID_SIZE = 4;
const int CARD_SIZE = 100;
const float TIME_LIMIT = 10.f;
const std::string RESOURCE_PATH = "resources";

// Define a structure for a Card
struct Card {
    sf::RectangleShape shape;
    int id;
    bool faceUp;
    bool matched;
};

void loadCardTextures(std::vector<sf::Texture>& textures) {
    textures.clear();
    for (const auto& entry : fs::directory_iterator(RESOURCE_PATH)) {
        sf::Texture texture;
        if (texture.loadFromFile(entry.path().string())) {
            textures.push_back(texture);
        }
    }
}

void initCards(std::vector<Card>& cards, const std::vector<sf::Texture>& textures, int gridSize) {
    cards.clear();
    int id = 0;

    for (int i = 0; i < gridSize * gridSize; ++i) {
        Card card;
        card.shape.setSize(sf::Vector2f(CARD_SIZE, CARD_SIZE));
        card.shape.setTexture(&textures[id % (textures.size())]);
        card.shape.setFillColor(sf::Color::White);
        card.shape.setOutlineThickness(1);
        card.shape.setOutlineColor(sf::Color::Black);
        card.id = id % (gridSize * gridSize / 2);
        card.faceUp = false;
        card.matched = false;
        cards.push_back(card);
        ++id;
    }

    // Shuffle cards
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cards.begin(), cards.end(), g);

    // Arrange cards in grid
    for (int i = 0; i < gridSize; ++i) {
        for (int j = 0; j < gridSize; ++j) {
            cards[i * gridSize + j].shape.setPosition(j * CARD_SIZE, i * CARD_SIZE);
        }
    }
}

void handleCardFlip(Card& card) {
    if (!card.faceUp && !card.matched) {
        card.faceUp = true;
        card.shape.setFillColor(sf::Color::Yellow); // Flip effect
    }
}

void drawCards(sf::RenderWindow& window, std::vector<Card>& cards) {
    for (auto& card : cards) {
        window.draw(card.shape);
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(GRID_SIZE * CARD_SIZE, GRID_SIZE * CARD_SIZE), "Memory Game");

    // Load textures
    std::vector<sf::Texture> textures;
    loadCardTextures(textures);
    if (textures.empty()) {
        std::cerr << "Error loading card textures!" << std::endl;
        return -1;
    }

    std::vector<Card> cards;
    initCards(cards, textures, GRID_SIZE);

    sf::Clock clock;
    sf::Time elapsed;
    int firstCardIndex = -1;
    int secondCardIndex = -1;
    bool waitingForFlip = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                int col = mousePos.x / CARD_SIZE;
                int row = mousePos.y / CARD_SIZE;
                int index = row * GRID_SIZE + col;

                if (index >= 0 && index < cards.size() && !cards[index].faceUp && !waitingForFlip) {
                    if (firstCardIndex == -1) {
                        firstCardIndex = index;
                        handleCardFlip(cards[firstCardIndex]);
                    } else {
                        secondCardIndex = index;
                        handleCardFlip(cards[secondCardIndex]);

                        // Check for match
                        if (cards[firstCardIndex].id == cards[secondCardIndex].id) {
                            cards[firstCardIndex].matched = true;
                            cards[secondCardIndex].matched = true;
                        } else {
                            waitingForFlip = true;
                        }

                        // Reset indices after checking
                        firstCardIndex = -1;
                        secondCardIndex = -1;
                    }
                }
            }
        }

        // Update the state of the game
        if (waitingForFlip) {
            elapsed = clock.getElapsedTime();
            if (elapsed.asSeconds() >= TIME_LIMIT) {
                if (secondCardIndex != -1) {
                    cards[firstCardIndex].faceUp = false;
                    cards[secondCardIndex].faceUp = false;
                    cards[firstCardIndex].shape.setFillColor(sf::Color::White); // Hide card
                    cards[secondCardIndex].shape.setFillColor(sf::Color::White); // Hide card
                }
                waitingForFlip = false;
                clock.restart();
            }
        }

        // Clear and draw
        window.clear();
        drawCards(window, cards);
        window.display();
    }

    return 0;
}
