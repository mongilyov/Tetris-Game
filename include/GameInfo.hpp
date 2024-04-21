// COPYRIGHT Монгилёв Андрей
// Москва, 2023 год

#ifndef INCLUDE_GAMEINFO_HPP_
#define INCLUDE_GAMEINFO_HPP_

#include <SFML/Graphics.hpp>

#include <string>

#include "Tetromino.hpp"

constexpr int SCORE_BANNER_HEIGHT = 160;
constexpr int SCORE_BANNER_WIDTH  = 320;
constexpr int INFO_BANNER_HEIGHT  = 640;
constexpr int INFO_BANNER_WIDTH   = 180;

class GameBorder : public sf::Drawable {
public:
  GameBorder() {
    topBorder.setPosition({0, SCORE_BANNER_HEIGHT - 5});
    topBorder.setSize({SCORE_BANNER_WIDTH, 5});
    topBorder.setFillColor(sf::Color::White);
    rightBorder.setPosition({SCORE_BANNER_WIDTH, 155});
    rightBorder.setSize({5, INFO_BANNER_HEIGHT});
    rightBorder.setFillColor(sf::Color::White); 
  }

private:
  sf::RectangleShape topBorder;
  sf::RectangleShape rightBorder;

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(topBorder, states);
    target.draw(rightBorder, states);
  }
};

class ScoreBanner : public sf::Drawable {
public:
  ScoreBanner() {
    scoreText.setString("SCORE ");
    scoreText.setPosition({25, 25});
    scoreText.setCharacterSize(78);
    scoreCounterText.setString("0");
    scoreCounterText.setPosition({300, 25});
    scoreCounterText.setCharacterSize(78);
  }
  
  constexpr void setFont(const sf::Font& font) {
    scoreText.setFont(font);
    scoreCounterText.setFont(font);
  }

  constexpr void setNewScore(int score) {
    scoreCounterText.setString(std::to_string(score));
  }

private:
  sf::Text scoreText;
  sf::Text scoreCounterText;

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(scoreText, states);
    target.draw(scoreCounterText, states);
  }
};

class RightBar : public sf::Drawable {
public:
  RightBar() {}

  template<class T>
  void createNewPiece(const sf::Texture& texture, int index) {
    newPiece = std::make_unique<T>(texture, index);
    newPiece->prepareToShowOnRightBar();
  }

  decltype(auto) handOverPiece() {
    newPiece->prepareToHandOver();
    return std::move(newPiece);
  }

private:
  std::unique_ptr<Tetromino> newPiece;

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (newPiece == nullptr) {
      return;
    }
    target.draw(*newPiece, states);
  }
};

class GameInfo : public sf::Drawable {
public:
  GameInfo(const sf::Font& font) : 
  score(0), 
  isStart(true), 
  isRestart(false) {
    scoreBanner.setFont(font);
    startBanner.setFont(font);
    restartBanner.setFont(font);
    startBanner.setString("PRESS S\nTO START");
    startBanner.setPosition({10, 200});
    startBanner.setCharacterSize(78);
    restartBanner.setString("PRESS R\n    TO\n RESTART");
    restartBanner.setPosition({10, 200});
    restartBanner.setCharacterSize(78);
  }

  void addScore(int filledRows) {
    switch (filledRows) {
      case 1:
        score += 100;
        break;
      
      case 2:
        score += 300;
        break;

      case 3:
        score += 700;
        break;

      case 4: 
        score += 1500;
        break;
    }
    scoreBanner.setNewScore(score);
  }

  template<class T>
  void makePiece(const sf::Texture& texture, int index) {
    rightBar.createNewPiece<T>(texture, index);
  }

  decltype(auto) handOverPiece() {
    return rightBar.handOverPiece();
  }
// todo methods to show
  
  constexpr void start() {
    isStart = false;
    hideStartBanner();
  }
  
  constexpr void restart() {
    isRestart = false;
    hideRestartBanner();
  }
  
  constexpr bool getIsStart() {
    return isStart;
  }

  constexpr bool getIsRestart() {
    return isRestart;
  }

  constexpr void setRestart() {
    isRestart = true;
  }

  constexpr void clearScore() {
    score = 0;
    scoreBanner.setNewScore(score);
  }

private:
  int score;
  GameBorder gameBorder;
  ScoreBanner scoreBanner;
  RightBar rightBar; 
  sf::Text startBanner;
  sf::Text restartBanner;
  bool isStart;
  bool isRestart;

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(gameBorder);
    target.draw(scoreBanner);
    target.draw(rightBar);
    if (isStart) {
      target.draw(startBanner, states);
    }
    if (isRestart) {
      target.draw(restartBanner, states);
    }
  }

  constexpr void hideStartBanner() {
    isStart = false;
  }
  
  constexpr void showRestartBanner() {
    isRestart = true;
  }
  
  constexpr void hideRestartBanner() {
    isRestart = false;
  }
};

#endif // INCLUDE_GAMEINFO_HPP_