// COPYRIGHT Монгилёв Андрей
// Москва, 2024 год

#ifndef INCLUDE_TETROMINO_HPP_
#define INCLUDE_TETROMINO_HPP_

#include <tuple>

#include <SFML/Graphics.hpp>

enum class Direction {
  Right,
  Down,
  Left,
  Up,
  Horizontal,
  Vertical,
  O
};

constexpr int TILE_SIZE = 32;          // px
constexpr int SCORE_BANNER_SIZE = 160; // px

class Tile : public sf::Drawable {
public:
  Tile() = default;
  Tile(const sf::Texture& texture, int x, int y) {
    // tileRepr.scale(2, 2);
    tileRepr.setTexture(texture);
    tileRepr.move(TILE_SIZE * x, SCORE_BANNER_SIZE + TILE_SIZE * y);
  }
  virtual ~Tile() = default;

  constexpr void moveDownwards() {
    tileRepr.move(0, TILE_SIZE);
  }

  constexpr void moveRight() {
    tileRepr.move(TILE_SIZE, 0);
  }

  constexpr void moveLeft() {
    tileRepr.move(-TILE_SIZE, 0);
  }
  
  constexpr void setPosition(int x, int y) { // why not sf::Vector2i everywhere............
    tileRepr.setPosition(
      TILE_SIZE * x,
      SCORE_BANNER_SIZE + TILE_SIZE * y
    );
  }

private:
  sf::Sprite tileRepr;

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
      target.draw(tileRepr, states);
  }
};

class Tetromino : public sf::Drawable {
public:
  Tetromino(int index) : globalTextureIndex(index) {
    tiles.reserve(4);
  }
  Tetromino(const Tetromino&) = default;
  Tetromino& operator=(const Tetromino&) = default;
  Tetromino(Tetromino&&) = default;
  Tetromino& operator=(Tetromino&&) = default;

  constexpr std::vector<std::tuple<int, int>> getCoords() {
    std::vector<std::tuple<int, int> > tempVec = {};
    std::ranges::transform(currentCoords, std::back_inserter(tempVec),
    [this](auto& tuple) {
      return std::tuple(std::get<0>(tuple) + std::get<0>(axis),
      std::get<1>(tuple) + std::get<1>(axis));
    });
    return tempVec;
  }
  
  constexpr std::vector<std::tuple<int, int> > getDownwardsMove() {
    std::vector<std::tuple<int, int> > tempVec = {};
    std::ranges::transform(currentCoords, std::back_inserter(tempVec),
    [this](auto& tuple) {
      return std::tuple(std::get<0>(tuple) + std::get<0>(axis),
      std::get<1>(tuple) + std::get<1>(axis) + 1);
    });
    return tempVec;
  }

  constexpr std::vector<std::tuple<int, int> > getRightMove() {
    std::vector<std::tuple<int, int> > tempVec;
    std::ranges::transform(currentCoords, std::back_inserter(tempVec),
    [this](auto& tuple) {
      return std::tuple(std::get<0>(tuple) + std::get<0>(axis) + 1,
      std::get<1>(tuple) + std::get<1>(axis));
    });
    return tempVec;
  }
  
  constexpr std::vector<std::tuple<int, int> > getLeftMove() {
    std::vector<std::tuple<int, int> > tempVec;
    std::ranges::transform(currentCoords, std::back_inserter(tempVec),
    [this](auto& tuple) {
      return std::tuple(std::get<0>(tuple) + std::get<0>(axis) - 1,
      std::get<1>(tuple) + std::get<1>(axis));
    });
    return tempVec;
  }

  constexpr virtual std::vector<std::tuple<int, int> > getClockwiseCoords() = 0;
  
  constexpr virtual std::vector<std::tuple<int, int> > getCounterClockwiseCoords() = 0;

  constexpr void moveDownwards() {
    for (auto& tile : tiles) {
      tile.moveDownwards();
    }
    ++std::get<1>(axis);
  }

  constexpr void moveRight() {
    for (auto& tile : tiles) {
      tile.moveRight();
    }
    ++std::get<0>(axis);
  }

  constexpr void moveLeft() {
    for (auto& tile : tiles) {
      tile.moveLeft();
    }
    --std::get<0>(axis);
  }
  
  constexpr virtual void rotateClockwise() = 0;

  constexpr virtual void rotateCounterClockwise() = 0;

  constexpr void prepareToShowOnRightBar() {
    axis = {13, 4}; // todo
    setPosition();
  }

  constexpr void prepareToHandOver() {
    axis = {5, 0};
    setPosition();
  }
  
  constexpr int getTextureIndex() {
    return globalTextureIndex;
  }

protected:
  std::vector<Tile> tiles;
  std::tuple<int, int> axis = {5, 0};
  std::vector<std::tuple<int, int> > currentCoords;
  Direction direction;
  int globalTextureIndex;

  constexpr void setPosition() { 
    for (size_t i = 0; i < 4; ++i) {
      tiles[i].setPosition(
        std::get<0>(currentCoords[i]) + std::get<0>(axis),
        std::get<1>(currentCoords[i]) + std::get<1>(axis)
      );
    }
  }  
  
  constexpr void createTiles(const sf::Texture& texture) {
    std::ranges::transform(currentCoords, std::back_inserter(tiles),
    [this, &texture](const auto& tuple) {
      return Tile(texture, std::get<0>(tuple) + std::get<0>(axis), std::get<1>(tuple));
    });
  }

  constexpr std::vector<std::tuple<int, int> > getRealCoords(std::vector<std::tuple<int, int> >& coords) {
    std::vector<std::tuple<int, int> > tempVec;
      std::ranges::transform(coords, std::back_inserter(tempVec),
        [this](auto& tuple) {
      return std::tuple(std::get<0>(tuple) + std::get<0>(axis),
        std::get<1>(tuple) + std::get<1>(axis));
    });
    return tempVec;
  }

private:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (const auto& tile : tiles) {
      target.draw(tile, states);
    }
  }
};

class S__Tetromino : public Tetromino {
public:
  S__Tetromino(const sf::Texture& texture, int index) : Tetromino(index) {
    currentCoords = Sh;
    direction = Direction::Horizontal;
    createTiles(texture);
  }

  constexpr std::vector<std::tuple<int, int> > getClockwiseCoords() override {
    if (direction == Direction::Horizontal) {
      return getRealCoords(Sv);
    } else { 
      return getRealCoords(Sh);
    }
  }
 
  constexpr std::vector<std::tuple<int, int> > getCounterClockwiseCoords() override {
    return getClockwiseCoords();
  }
  
  constexpr void rotateClockwise() override {
    if (direction == Direction::Horizontal) {
      currentCoords = Sv;
      direction = Direction::Vertical;
    } else {
      currentCoords = Sh;
      direction = Direction::Horizontal;
    }
    setPosition();
  }
  
  constexpr void rotateCounterClockwise() override {
    rotateClockwise();
  }
  
private:
  std::vector<std::tuple<int, int> > Sv = {
    {1, 0},
    {0, -1},
    {0, 0},
    {1, 1},
  };
  std::vector<std::tuple<int, int> > Sh = {
    {-1, 1},
    {0, 1},
    {0, 0},
    {1, 0}
  };
  
};

class Z__Tetromino : public Tetromino {
public:
  Z__Tetromino(const sf::Texture& texture, int index) : Tetromino(index) {
    currentCoords = Zh;
    direction = Direction::Horizontal;
    createTiles(texture);
  }

  constexpr std::vector<std::tuple<int, int> > getClockwiseCoords() override {
    if (direction == Direction::Horizontal) {
      return getRealCoords(Zv);
    } else { 
      return getRealCoords(Zh);
    }
  }
 
  constexpr std::vector<std::tuple<int, int> > getCounterClockwiseCoords() override {
    return getClockwiseCoords();
  }
  
  constexpr void rotateClockwise() override {
    if (direction == Direction::Horizontal) {
      currentCoords = Zv;
      direction = Direction::Vertical;
    } else {
      currentCoords = Zh;
      direction = Direction::Horizontal;
    }
    setPosition();
  }
  
  constexpr void rotateCounterClockwise() override {
    rotateClockwise();
  }

private:
  std::vector<std::tuple<int, int> > Zv = {
    {1, -1},
    {1, 0},
    {0, 0},
    {0, 1},
  };
  std::vector<std::tuple<int, int> > Zh = {
    {-1, 0},
    {0, 1},
    {0, 0},
    {1, 1}
  };
};

class I__Tetromino : public Tetromino {
public:
  I__Tetromino(const sf::Texture& texture, int index) : Tetromino(index) { 
    currentCoords = Ih;
    direction = Direction::Horizontal;
    createTiles(texture);
  }

  constexpr std::vector<std::tuple<int, int> > getClockwiseCoords() override {
    if (direction == Direction::Horizontal) {
      return getRealCoords(Iv);
    } else { 
      return getRealCoords(Ih);
    }
  }
 
  constexpr std::vector<std::tuple<int, int> > getCounterClockwiseCoords() override {
    return getClockwiseCoords();
  }
  
  constexpr void rotateClockwise() override {
    if (direction == Direction::Horizontal) {
      currentCoords = Iv;
      direction = Direction::Vertical;
    } else {
      currentCoords = Ih;
      direction = Direction::Horizontal;
    }
    setPosition();
  }
  
  constexpr void rotateCounterClockwise() override {
    rotateClockwise();
  }
  
private:
  std::vector<std::tuple<int, int> > Iv = {
    {0, -2},
    {0, -1},
    {0, 0},
    {0, 1},
  };
  std::vector<std::tuple<int, int> > Ih = {
    {-2, 0},
    {-1, 0},
    {0, 0},
    {1, 0}
  };
};

class J__Tetromino : public Tetromino {
public:
  J__Tetromino(const sf::Texture& texture, int index) : Tetromino(index) {
    currentCoords = Jd;
    direction = Direction::Down; 
    createTiles(texture);
  }
  
  constexpr std::vector<std::tuple<int, int> > getClockwiseCoords() override {
    switch (direction) {
      case Direction::Right:
        return getRealCoords(Jd);
        break;
      
      case Direction::Down:
        return getRealCoords(Jl);
        break;
      
      case Direction::Left:
        return getRealCoords(Ju);
        break;
      
      case Direction::Up:
        return getRealCoords(Jr);
        break;
      
      default:
        return {};
        break;
    }
  }
  
  constexpr std::vector<std::tuple<int, int> > getCounterClockwiseCoords() override {
    switch (direction) {
      case Direction::Right:
        return getRealCoords(Ju);
        break;
      
      case Direction::Down:
        return getRealCoords(Jr);
        break;
      
      case Direction::Left:
        return getRealCoords(Jd);
        break;
      
      case Direction::Up:
        return getRealCoords(Jl);
        break;
      
      default:
        return {};
        break;
    }
  }
  
  constexpr void rotateClockwise() override { 
    switch (direction) {
      case Direction::Right:
        currentCoords = Jd;
        direction = Direction::Down;
        break;
      
      case Direction::Down:
        currentCoords = Jl;
        direction = Direction::Left;
        break;
      
      case Direction::Left:
        currentCoords = Ju;
        direction = Direction::Up;
        break;
      
      case Direction::Up:
        currentCoords = Jr;
        direction = Direction::Right;
        break;
      
      default:
        break;
    }
    setPosition();
  }

  constexpr void rotateCounterClockwise() override { 
    switch (direction) {
      case Direction::Right:
        currentCoords = Ju;
        direction = Direction::Up;
        break;
      
      case Direction::Down:
        currentCoords = Jr;
        direction = Direction::Right;
        break;
      
      case Direction::Left:
        currentCoords = Jd;
        direction = Direction::Down;
        break;
      
      case Direction::Up:
        currentCoords = Jl;
        direction = Direction::Left;
        break;
      
      default:
        break;
    }
    setPosition();
  }

private:
  std::vector<std::tuple<int, int> > Jl = {
    {0, -1},
    {0, 0},
    {-1, 1},
    {0, 1}
  };
  std::vector<std::tuple<int, int> > Ju = {
    {-1, -1},
    {0, 0},
    {-1, 0},
    {1, 0}
  };
  std::vector<std::tuple<int, int> > Jr = {
    {0, -1},
    {0, 0},
    {1, -1},
    {0, 1}
  };
  std::vector<std::tuple<int, int> > Jd = {
    {-1, 0},
    {0, 0},
    {1, 1},
    {1, 0}
  };
};

class L__Tetromino : public Tetromino {
public:
  L__Tetromino(const sf::Texture& texture, int index) : Tetromino(index) {
    currentCoords = Ld;
    direction = Direction::Down;
    createTiles(texture);
  }

  constexpr std::vector<std::tuple<int, int> > getClockwiseCoords() override {
    switch (direction) {
      case Direction::Right:
        return getRealCoords(Ld);
        break;
      
      case Direction::Down:
        return getRealCoords(Ll);
        break;
      
      case Direction::Left:
        return getRealCoords(Lu);
        break;
      
      case Direction::Up:
        return getRealCoords(Lr);
        break;
      
      default:
        return {};
        break;
    }
  }
  
  constexpr std::vector<std::tuple<int, int> > getCounterClockwiseCoords() override {
    switch (direction) {
      case Direction::Right:
        return getRealCoords(Lu);
        break;
      
      case Direction::Down:
        return getRealCoords(Lr);
        break;
      
      case Direction::Left:
        return getRealCoords(Ld);
        break;
      
      case Direction::Up:
        return getRealCoords(Ll);
        break;
      
      default:
        return {};
        break;
    }
  }
  
  constexpr void rotateClockwise() override { 
    switch (direction) {
      case Direction::Right:
        currentCoords = Ld;
        direction = Direction::Down;
        break;
      
      case Direction::Down:
        currentCoords = Ll;
        direction = Direction::Left;
        break;
      
      case Direction::Left:
        currentCoords = Lu;
        direction = Direction::Up;
        break;
      
      case Direction::Up:
        currentCoords = Lr;
        direction = Direction::Right;
        break;
      
      default:
        break;
    }
    setPosition();
  }

  constexpr void rotateCounterClockwise() override { 
    switch (direction) {
      case Direction::Right:
        currentCoords = Lu;
        direction = Direction::Up;
        break;
      
      case Direction::Down:
        currentCoords = Lr;
        direction = Direction::Right;
        break;
      
      case Direction::Left:
        currentCoords = Ld;
        direction = Direction::Down;
        break;
      
      case Direction::Up:
        currentCoords = Ll;
        direction = Direction::Left;
        break;
      
      default:
        break;
    }
    setPosition();
  }
private: 
  std::vector<std::tuple<int, int> > Ll = {
    {-1, -1},
    {0, 0},
    {0, -1},
    {0, 1}
  };
  std::vector<std::tuple<int, int> > Lu = {
    {1, -1},
    {0, 0},
    {-1, 0},
    {1, 0}
  };
  std::vector<std::tuple<int, int> > Lr = {
    {0, -1},
    {0, 0},
    {0, 1},
    {1, 1}
  };
  std::vector<std::tuple<int, int> > Ld = {
    {-1, 0},
    {0, 0},
    {1, 0},
    {-1, 1}
  };
};

class T__Tetromino : public Tetromino {
public:
  T__Tetromino(const sf::Texture& texture, int index) : Tetromino(index) {
    currentCoords = Td;
    direction = Direction::Down;
    createTiles(texture);
  }

  constexpr std::vector<std::tuple<int, int> > getClockwiseCoords() override {
    switch (direction) {
      case Direction::Right:
        return getRealCoords(Td);
        break;
      
      case Direction::Down:
        return getRealCoords(Tl);
        break;
      
      case Direction::Left:
        return getRealCoords(Tu);
        break;
      
      case Direction::Up:
        return getRealCoords(Tr);
        break;
      
      default:
        return {};
        break;
    }
  }
  
  constexpr std::vector<std::tuple<int, int> > getCounterClockwiseCoords() override {
    switch (direction) {
      case Direction::Right:
        return getRealCoords(Tu);
        break;
      
      case Direction::Down:
        return getRealCoords(Tr);
        break;
      
      case Direction::Left:
        return getRealCoords(Td);
        break;
      
      case Direction::Up:
        return getRealCoords(Tl);
        break;
      
      default:
        return {};
        break;
    }
  }
  
  constexpr void rotateClockwise() override { 
    switch (direction) {
      case Direction::Right:
        currentCoords = Td;
        direction = Direction::Down;
        break;
      
      case Direction::Down:
        currentCoords = Tl;
        direction = Direction::Left;
        break;
      
      case Direction::Left:
        currentCoords = Tu;
        direction = Direction::Up;
        break;
      
      case Direction::Up:
        currentCoords = Tr;
        direction = Direction::Right;
        break;
      
      default:
        break;
    }
    setPosition();
  }

  constexpr void rotateCounterClockwise() override { 
    switch (direction) {
      case Direction::Right:
        currentCoords = Tu;
        direction = Direction::Up;
        break;
      
      case Direction::Down:
        currentCoords = Tr;
        direction = Direction::Right;
        break;
      
      case Direction::Left:
        currentCoords = Td;
        direction = Direction::Down;
        break;
      
      case Direction::Up:
        currentCoords = Tl;
        direction = Direction::Left;
        break;
      
      default:
        break;
    }
    setPosition();
  }

private:
  std::vector<std::tuple<int, int> > Tl = {
    {0, -1},
    {0, 0},
    {-1, 0},
    {0, 1}
  };
  std::vector<std::tuple<int, int> > Tu = {
    {0, -1},
    {0, 0},
    {-1, 0},
    {1, 0}
  };
  std::vector<std::tuple<int, int> > Tr = {
    {0, -1},
    {0, 0},
    {0, 1},
    {1, 0}
  };
  std::vector<std::tuple<int, int> > Td = {
    {-1, 0},
    {0, 0},
    {1, 0},
    {0, 1}
  };
};

class O__Tetromino : public Tetromino {
public:
  O__Tetromino(const sf::Texture& texture, int index) : Tetromino(index) {
    currentCoords = O;
    direction = Direction::O;
    createTiles(texture);
  }
  
  constexpr std::vector<std::tuple<int, int> > getClockwiseCoords() override {
    return getRealCoords(O);
  }

  constexpr std::vector<std::tuple<int, int> > getCounterClockwiseCoords() override {
    return getRealCoords(O);
  }

  constexpr void rotateClockwise() override {
    return;
  }

  constexpr void rotateCounterClockwise() override {
    return;
  }

private:
  std::vector<std::tuple<int, int> > O = {
    {-1, 0},
    {0, 0},
    {-1, 1},
    {0, 1}
  };
};

#endif // INCLUDE_TETROMINO_HPP_