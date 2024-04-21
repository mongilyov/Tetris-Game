// COPYRIGHT Монгилёв Андрей
// Москва, 2024 год

#ifndef INCLUDE_COMMANDER_HPP_
#define INCLUDE_COMMANDER_HPP_

#include <SFML/Window/Keyboard.hpp>

#include <functional>
#include <map>

#include "Map.hpp"

class Button {
public:
	Button() = default;
	Button(bool repeat, std::function<void(Map&)> func) : isRepeatable(repeat), 
	functionToExecute(func), pressed(false), toBeExecuted(false) {}
	Button(const Button&) = default;
	Button& operator=(const Button&) = default;

	constexpr void operator()(Map& map) {
		if (pressed && isRepeatable) {
			functionToExecute(map);
			return;
		}
		if (functionToExecute == nullptr || !toBeExecuted) {
			return;
		}
		toBeExecuted = false;

		functionToExecute(map);
	}
	
	inline void pressButton() {
		if (!pressed) {
			pressed = true;
			toBeExecuted = true;
		}
	}
	
	inline void releaseButton() {
		pressed = false;
		toBeExecuted = false;
	}

private:
	bool pressed = false;
	bool isRepeatable;
	bool toBeExecuted = false;
	std::function<void(Map&)> functionToExecute;
};

class Commander {
public:
	Commander(Map& map, GameInfo& gI);

	constexpr void buttonWasPressed(sf::Keyboard::Key& key) {
		buttons[key].pressButton();
	}
	
	constexpr void buttonWasReleased(sf::Keyboard::Key& key) {
		buttons[key].releaseButton();
	}

	void giveOrders();
	
	constexpr int gameMode() {
		return gameInfo.getIsStart() || gameInfo.getIsRestart() ? 0 : 1;
	}
	
	constexpr void waitForRestart() {
		gameInfo.setRestart();
	}
private:
	std::map<sf::Keyboard::Key, Button> buttons;
	Map& map;
	GameInfo& gameInfo;
};

#endif // INCLUDE_COMMANDER_HPP_