#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include <vector>
#include <SFML/System/Clock.hpp>

struct Ghost {
    sf::Sprite sprite;
    sf::Vector2f velocity;
    sf::Clock movementClock;

    Ghost(const sf::Texture& texture) : sprite(texture), velocity(0.f, 0.f) {
        sprite.setScale(0.1f, 0.1f);  // Adjust scale as needed
    }
};

class Game {
public:
    Game();
    int run();
    void addGhost();
    static const float SCENE_WIDTH;
    static const float SCENE_HEIGHT;
    static const float PLAYER_START_X;
    static const float PLAYER_START_Y;
    static const float RADIUS;
    static const float SPEED;
    static const float CHANGE_DIRECTION_EVERY;



private:
    sf::RenderWindow window;
    sf::Sprite background;
    sf::Texture backgroundTexture;
    sf::Texture playerTexture;
    sf::CircleShape player;
    sf::Clock clock;
    sf::Vector2f direction;

    std::vector<Ghost> ghosts;// To store multiple ghosts
    std::vector<bool> ghostsVisible; // To track visibility of each ghost
    sf::Texture ghostTexture;
    std::default_random_engine randomEngine; 
    std::uniform_real_distribution<float> distX, distY; // Distribution for random positions
    sf::Clock spawnClock;
    float ghostSpawnInterval; 
    const int maxGhosts = 10;

    void initGhosts(int numberOfGhosts);
    void updateGhosts(sf::Time delta);
    void renderGhosts();
    void handleCollision(size_t ghostIndex);

    int initWindow();
    int initBackground();
    int initPlayer();
    void processInput();
    void update(sf::Time delta);
    void render();
};
