

#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>

#include "main.h"


class Column{
public:
	float width;
	float length;
	float x;
	float y;
	float out;
	sf::Color color = sf::Color::White;
	sf::RectangleShape Draw() {
		sf::RectangleShape col(sf::Vector2f(width, length));
		col.setPosition(sf::Vector2f(x, y));
		col.setOutlineColor(sf::Color::Black);
		col.setFillColor(color);
		return col;
	}
};


int main() {
	sf::RenderWindow window(sf::VideoMode(800, 600), "Insertion Sort Algorithm");
	int numbers[] = { 66, 83, 343, 111, 500, 182, 46, 370, 480, 527, 266, 167, 163, 551, 462, 101, 143, 549, 82, 100, 69, 482, 491, 388, 588, 424, 138, 564, 519, 504, 52, 97, 419, 171, 275, 48, 247, 23, 441, 98, 240, 420, 203, 582, 365, 415, 267, 301, 512, 210, 16, 278, 463, 139, 392, 581, 338, 74, 282, 211, 296, 518, 468, 54, 204, 363, 375, 33, 130, 99, 31, 59, 575, 37, 361, 39, 312, 434, 200, 106, 389, 221, 220, 255, 196, 68, 474, 292, 231, 513, 369, 283, 201, 445, 449, 194, 251, 294, 206, 274 };
	int len = std::end(numbers) - std::begin(numbers);
	int height = 1;
	float unit = 800 / len;
	sf::Time time = sf::milliseconds(10);
	bool done = false;
	window.clear(sf::Color::Black);
	sf::sleep(sf::seconds(5));
	
	for (int k = 0;k < len;k++) {
		Column col;
		col.width = unit - 1;
		col.length = -numbers[k] * height;
		col.x = k * unit;
		col.y = 600;
		window.draw(col.Draw());
		window.display();
	}
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}
			
		window.clear(sf::Color::Black);
		
		if (!done) {
			for (int i = 0;i < len;i++) {

				int j = i;
				
				while (j > 0 && numbers[j] < numbers[j - 1]) {
					
					sf::sleep(time);
					
					for (int k = 0;k < len;k++) {
						if (k != j) {
							Column col;
							col.width = unit - 1;
							col.length = -numbers[k] * height;
							col.x = k * unit;
							col.y = 600;
							window.draw(col.Draw());
							//window.display();
						}
					}
					Column col;
					col.width = unit - 1;
					col.length = -numbers[j] * height;
					col.x = j * unit;
					col.y = 600;
					col.color = sf::Color::Red;
					window.draw(col.Draw());
					window.display();
					
					window.clear(sf::Color::Black);
					int a = numbers[j];
					numbers[j] = numbers[j - 1];
					numbers[j - 1] = a;
					j--;

				}
				
				if (i == len - 1) {
					done = true;
				}
			}
		}
		if (done) {
			for (int k = 0;k < len;k++) {
				Column col;
				col.width = unit - 1;
				col.length = -numbers[k] * height;
				col.x = k * unit;
				col.y = 600;
				col.color = sf::Color::Green;
				window.draw(col.Draw());
			}
		}

		window.display();
	}
	return 0;
}

