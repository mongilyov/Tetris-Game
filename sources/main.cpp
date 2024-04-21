// COPYRIGHT Монгилёв Андрей
// Москва, 2024 год

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "Commander.hpp"
#include "GameInfo.hpp"
#include "Map.hpp"

constexpr int HEIGHT = 800;
constexpr int WIDTH = 500;
constexpr int TICKS = 15; // (i think)

int main() {
  sf::RenderWindow window(sf::VideoMode(500, 800), "Tetris (MDS)");
  window.setFramerateLimit(70);

  sf::Music gameMusic;
  gameMusic.openFromFile("tetrisTheme.mp3");
  gameMusic.setLoop(true);
  gameMusic.play();
  
  sf::Font gameFont;
  gameFont.loadFromFile("arcade.ttf");

  GameInfo gameInfo(gameFont);
  Map map(gameInfo);
  Commander commander(map, gameInfo);

  sf::Clock gameClock;

  int tick = 0;

  while (window.isOpen()) {
    auto time = gameClock.restart().asSeconds();
    auto fps = 1.0f / time;

    sf::Event event;

    while (window.pollEvent(event)) {
      switch (event.type) {
        case sf::Event::Closed:
          window.close();
          return 0;
        
        case sf::Event::KeyPressed:
          commander.buttonWasPressed(event.key.code);
          break;
        
        case sf::Event::KeyReleased:
          commander.buttonWasReleased(event.key.code);
          break;
        
        default:
          break;
      }
    }

    commander.giveOrders();
    
    switch (commander.gameMode()) {
      case 0:
        break;
      case 1:
        ++tick;
        if (tick == TICKS) {
          if (!map.update()) {
            commander.waitForRestart();
          }
          tick = 0;
        }
        break;
    }

    
    window.clear();
    window.draw(map);
    window.draw(gameInfo);
    window.display();
  }
}