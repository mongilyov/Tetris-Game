// COPYRIGHT Монгилёв Андрей
// Москва, 2024 год

#ifndef INCLUDE_MAP_HPP_
#define INCLUDE_MAP_HPP_

#include <vector>
#include <random>

#include <SFML/Graphics.hpp>

#include "GameInfo.hpp"
#include "Tetromino.hpp"


constexpr int GRID_HEIGHT     = 20;
constexpr int GRID_WIDTH      = 10;
constexpr short WHITE_TEXTURE = 0;
constexpr short BLUE_TEXTURE  = 1;
constexpr short RED_TEXTURE   = 2;

class Map : public sf::Drawable {
public:
  Map(GameInfo& gI) : gameInfo(gI) {
    map.reserve(GRID_HEIGHT);
    tiles.reserve(GRID_HEIGHT);
    for (size_t i = 0; i < GRID_HEIGHT; ++i) {
      map.emplace_back(std::vector<bool>(GRID_WIDTH));
      tiles.emplace_back(std::vector<Tile>(GRID_WIDTH));
    }
    
    textures.resize(3);
    textures[WHITE_TEXTURE].loadFromFile("whiteTile.png");
    textures[BLUE_TEXTURE].loadFromFile("blueTile.png");
    textures[RED_TEXTURE].loadFromFile("redTile.png");

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    generator = std::default_random_engine(seed);
    distribution = std::uniform_int_distribution<int>(0, 6);
  }
  Map(const Map&) = delete;
  Map& operator=(const Map&) = delete;
  Map(Map&&) = delete;
  Map& operator=(Map&&) = delete;

  constexpr void start() {
    if (!gameInfo.getIsStart()) {
      return;
    }
    gameInfo.start();
    predictNewPiece();
    selectNewPiece();
  }

  constexpr void restart() {
    if (gameInfo.getIsStart()) {
      return;
    }
    gameInfo.restart();
    clear();
    predictNewPiece();
    selectNewPiece();
  }

  bool update() {
    if (gameInfo.getIsStart() || gameInfo.getIsRestart()) {
      return false;
    }
    if (!checkMove(fallingPiece->getDownwardsMove())) {
      fixPiece();
      checkFilledRows();
      return selectNewPiece(); // selectNewPiece() --> bool
    }
    fallingPiece->moveDownwards();
    return true;
  }
  
  constexpr void pieceDownwardsMove() {
    if (
    gameInfo.getIsStart() ||
    gameInfo.getIsRestart() ||
    !checkMove(fallingPiece->getRightMove())) {
      return;
    }
    fallingPiece->moveDownwards();
  }

  constexpr void pieceRightMove() {
    if (
    gameInfo.getIsStart() || 
    gameInfo.getIsRestart() ||
   !checkMove(fallingPiece->getRightMove()))
      {
      return;
    }
    fallingPiece->moveRight();
  }
  
  constexpr void pieceLeftMove() {
    if (
    gameInfo.getIsStart() ||
    gameInfo.getIsRestart() || 
    !checkMove(fallingPiece->getLeftMove())) {
      return;
    }
    fallingPiece->moveLeft(); 
  }
  
  constexpr void pieceRotateClockwise() {
    if (
    gameInfo.getIsStart() ||
    gameInfo.getIsRestart() ||
    !checkMove(fallingPiece->getClockwiseCoords())) {
      return;
    }
    fallingPiece->rotateClockwise();
  }
  
  constexpr void pieceRotateCounterClockwise() {
    if (
    gameInfo.getIsStart() ||
    gameInfo.getIsRestart() ||
    !checkMove(fallingPiece->getCounterClockwiseCoords())) {
      return;
    }
    fallingPiece->rotateCounterClockwise();
  }

  constexpr void predictNewPiece() {
    auto textureIndex = distribution(generator) % 3;

    switch (distribution(generator)) {
      case 0:
        gameInfo.makePiece<I__Tetromino>(
          textures[textureIndex], textureIndex);
        break;
      case 1: 
        gameInfo.makePiece<J__Tetromino>(
          textures[textureIndex], textureIndex);
        break;
      case 2:
        gameInfo.makePiece<L__Tetromino>(
          textures[textureIndex], textureIndex);
        break;
      case 3:
        gameInfo.makePiece<O__Tetromino>(
          textures[textureIndex], textureIndex);
        break;
      case 4:
        gameInfo.makePiece<S__Tetromino>(
          textures[textureIndex], textureIndex);
        break;
      case 5:
        gameInfo.makePiece<T__Tetromino>(
          textures[textureIndex], textureIndex);
        break;
      case 6:
        gameInfo.makePiece<Z__Tetromino>(
          textures[textureIndex], textureIndex);
        break;
    }
  }
  
  constexpr bool selectNewPiece() {
    fallingPiece = gameInfo.handOverPiece();
    predictNewPiece();
    return checkMove(fallingPiece->getCoords());
  }
  
  constexpr void checkFilledRows() {
    int deletedRowsCounter = 0;
    for (auto iter = map.begin(); iter != map.end();) {
      if (std::ranges::all_of(*iter, [](auto tile) { return tile == true; })) { // ==?
        tiles.erase(tiles.begin() + std::distance(map.begin(), iter));
        iter = map.erase(iter);
        ++deletedRowsCounter;
      } else {
        ++iter;
      }
    }

    if (!deletedRowsCounter) {
      return;
    }

    gameInfo.addScore(deletedRowsCounter);

    // TEMPORARY DECISION! redesign fully
    for (; deletedRowsCounter > 0; --deletedRowsCounter) {
      map.emplace(map.begin(), std::vector<bool>(GRID_WIDTH));
      tiles.emplace(tiles.begin(), std::vector<Tile>(GRID_WIDTH));
    }
    
    for (size_t i = 0; i < GRID_HEIGHT; ++i) {
      for (size_t j = 0; j < GRID_WIDTH; ++j) {
        tiles[i][j].setPosition(j, i);
      }
    }
    
  }

private:
  GameInfo& gameInfo;
  std::vector<std::vector<bool> > map;
  std::vector<std::vector<Tile> > tiles;   // ???
  std::vector<sf::Texture> textures;
  std::unique_ptr<Tetromino> fallingPiece;

  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution;

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (const auto& row : tiles) {
      for (const auto& tile : row) {
        target.draw(tile, states);
      }
    }
    if (fallingPiece == nullptr) {
      return;
    }
    target.draw(*fallingPiece, states);
  }
  
  constexpr bool checkMove(std::vector<std::tuple<int, int> >&& pos) {
    for (auto& coord : pos) {
      auto x = std::get<0>(coord);
      auto y = std::get<1>(coord) > 0 ? std::get<1>(coord) : 0;

      if (y == GRID_HEIGHT || x == GRID_WIDTH || x == -1 || map[y][x]) {
        return false;
      }
    }
    return true;
  }

  constexpr void fixPiece() {
    for (auto& coord : fallingPiece->getCoords()) {
      auto x = std::get<0>(coord);
      auto y = std::get<1>(coord);
      map[y][x] = 1;
      tiles[y][x] = Tile(textures[fallingPiece->getTextureIndex()], x, y);
    }
  }
  
  constexpr void clear() {
    map.clear();
    tiles.clear();
    map.reserve(GRID_HEIGHT);
    tiles.reserve(GRID_HEIGHT);
    for (size_t i = 0; i < GRID_HEIGHT; ++i) {
      map.emplace_back(std::vector<bool>(GRID_WIDTH));
      tiles.emplace_back(std::vector<Tile>(GRID_WIDTH));
    } 
    gameInfo.clearScore();
  }
};

#endif // INCLUDE_MAP_HPP_