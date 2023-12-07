#include <iostream>
#include <SFML/Graphics.hpp>
#include "game.h"
#include <chrono> 
#include <random>

const float Game::SCENE_WIDTH = 800.0f;
const float Game::SCENE_HEIGHT = 600.0f;
const float Game::PLAYER_START_X = 400.0f;
const float Game::PLAYER_START_Y = 300.0f;
const float Game::RADIUS = 40.0f;
const float Game::SPEED = 150.0f;
const float Game::CHANGE_DIRECTION_EVERY = 2.0f; // Change direction every 2 seconds


Game::Game() : direction(0.f, 0.f) {
    initWindow();
    initBackground();
    initPlayer();
    ghostSpawnInterval = 3.0f; // New ghost every 5 seconds

    // Seed the random engine
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    randomEngine.seed(seed);

    // Random distributions for the initial position
    distX = std::uniform_real_distribution<float>(0.f, SCENE_WIDTH);
    distY = std::uniform_real_distribution<float>(0.f, SCENE_HEIGHT);
    if (!ghostTexture.loadFromFile("./resources/ghost.png")) {
        std::cerr << "Failed to load ghost texture." << std::endl;
        std::exit(1);
    }
    initGhosts(5); 
}

int Game::initWindow() {
    window.create(sf::VideoMode(SCENE_WIDTH, SCENE_HEIGHT), "My 2D game");
    window.setFramerateLimit(120);
    return 0;
}

int Game::initBackground() {
    if (!backgroundTexture.loadFromFile("resources/background.png")) {
        return 1;
    }
    backgroundTexture.setRepeated(true);
    background.setTexture(backgroundTexture);
    background.setTextureRect(sf::IntRect(0, 0, SCENE_WIDTH, SCENE_HEIGHT));
    return 0;
}

int Game::initPlayer() {
    player.setRadius(RADIUS);
    player.setOrigin(RADIUS, RADIUS);
    player.setPosition(PLAYER_START_X, PLAYER_START_Y);
    if (!playerTexture.loadFromFile("resources/pacman.png")) {
        return 1;
    }
    player.setTexture(&playerTexture);
    return 0;
}

void Game::initGhosts(int numberOfGhosts) {
    for (int i = 0; i < numberOfGhosts; ++i) {
        Ghost ghost(ghostTexture);
        ghost.sprite.setPosition(distX(randomEngine), distY(randomEngine));
        ghosts.push_back(ghost);
        ghostsVisible.push_back(true);
    }
}

void Game::updateGhosts(sf::Time delta) {
    static std::uniform_real_distribution<float> dist(-100.0f, 100.0f); // Adjust for a good speed

    for (auto& ghost : ghosts) {
        size_t index = &ghost - &ghosts[0]; // Get index of the current ghost
        if (ghostsVisible[index]) {
            if (ghost.movementClock.getElapsedTime().asSeconds() > Game::CHANGE_DIRECTION_EVERY) {
                ghost.velocity = sf::Vector2f(dist(randomEngine), dist(randomEngine));
                ghost.movementClock.restart();
            }

            ghost.sprite.move(ghost.velocity * delta.asSeconds());

            // Boundary check and update ghost position
            sf::Vector2f pos = ghost.sprite.getPosition();
            sf::FloatRect bounds = ghost.sprite.getGlobalBounds();
            if (pos.x < 0 || pos.x > SCENE_WIDTH - bounds.width) {
                ghost.velocity.x = -ghost.velocity.x;
            }
            if (pos.y < 0 || pos.y > SCENE_HEIGHT - bounds.height) {
                ghost.velocity.y = -ghost.velocity.y;
            }
            ghost.sprite.setPosition(std::max(0.f, std::min(pos.x, SCENE_WIDTH - bounds.width)),
                                     std::max(0.f, std::min(pos.y, SCENE_HEIGHT - bounds.height)));
        }
    }
}


void Game::renderGhosts() {
    for (const auto& ghost : ghosts) {
        size_t index = &ghost - &ghosts[0]; // Get index of the current ghost
        if (ghostsVisible[index]) {
            window.draw(ghost.sprite);
        }
    }
}






void Game::processInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
                case sf::Keyboard::Up:
                    direction.y = -1;
                    break;
                case sf::Keyboard::Down:
                    direction.y = 1;
                    break;
                case sf::Keyboard::Left:
                    direction.x = -1;
                    break;
                case sf::Keyboard::Right:
                    direction.x = 1;
                    break;
                default:
                    break;
            }
        }

        if (event.type == sf::Event::KeyReleased) {
            if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Down)
                direction.y = 0;
            if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right)
                direction.x = 0;
        }
    }
}

void Game::addGhost() {
    Ghost newGhost(ghostTexture);
    newGhost.sprite.setPosition(distX(randomEngine), distY(randomEngine));
    ghosts.push_back(newGhost);
    ghostsVisible.push_back(true);
}



void Game::update(sf::Time delta) {
    // Player movement logic
    float deltaTime = 0.001f * delta.asMilliseconds();
    sf::Vector2f newPosition = player.getPosition() + SPEED * deltaTime * direction;
    newPosition.x = std::max(RADIUS, std::min(newPosition.x, SCENE_WIDTH - RADIUS));
    newPosition.y = std::max(RADIUS, std::min(newPosition.y, SCENE_HEIGHT - RADIUS));
    player.setPosition(newPosition);

    // Ghost collision detection
   for (size_t i = 0; i < ghosts.size(); ++i) {
        if (ghostsVisible[i] && player.getGlobalBounds().intersects(ghosts[i].sprite.getGlobalBounds())) {
            handleCollision(i);
        }
    }

    // Ghost spawning logic
    static sf::Clock ghostSpawnTimer;
    if (ghosts.size() < maxGhosts && ghostSpawnTimer.getElapsedTime().asSeconds() > ghostSpawnInterval) {
        addGhost();
        ghostSpawnTimer.restart();
    }

    // Update ghost positions
    updateGhosts(delta);
}





void Game::handleCollision(size_t ghostIndex) {
    ghostsVisible[ghostIndex] = false;
}

void Game::render() {
    window.clear(sf::Color::White);
    window.draw(background);
    window.draw(player);
    renderGhosts(); // Make sure this function is being called
    window.display();
}





int Game::run() {
    while (window.isOpen()) {
        sf::Time delta = clock.restart();
        processInput();
        update(delta);
        render();
    }
    return 0;
}
