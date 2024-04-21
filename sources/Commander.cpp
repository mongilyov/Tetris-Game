#include "Commander.hpp"

Commander::Commander(Map& map, GameInfo& gI) : map(map), gameInfo(gI) {
  buttons[sf::Keyboard::Key::Right] = Button(false, &Map::pieceRightMove);
  buttons[sf::Keyboard::Key::Left]  = Button(false, &Map::pieceLeftMove);
  buttons[sf::Keyboard::Key::Up]    = Button(false, &Map::pieceRotateClockwise);
  buttons[sf::Keyboard::Key::Down]  = Button(true,  &Map::update);
  buttons[sf::Keyboard::Key::Q]     = Button(false, &Map::pieceRotateClockwise);
  buttons[sf::Keyboard::Key::E]     = Button(false, &Map::pieceRotateCounterClockwise);
  buttons[sf::Keyboard::Key::S]     = Button(false, &Map::start);
  buttons[sf::Keyboard::Key::R]     = Button(false, &Map::restart);
}

void Commander::giveOrders() { 
  for (auto& pair : buttons) {
    pair.second(map);
  }
}