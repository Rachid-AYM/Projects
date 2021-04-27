

#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>

#include "main.h"



int main()
{
    int grid[100][100];
    int nextgrid[100][100];
    float alpha = 100;
    bool evolve = false;
    sf::Color transparent = sf::Color::Color(255, 255, 255, alpha);
    sf::RenderWindow window(sf::VideoMode(600, 600), "Conway's Game of Life");
    sf::Event event;

    window.setFramerateLimit(60);

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Space) {
                    evolve = true;
                }
                else if (event.key.code == sf::Keyboard::R) {
                    for (int i = 0;i < 100;i++) {
                        for (int j = 0;j < 100;j++) {
                            grid[i][j] = 0;
                        }
                    }
                }
            }
        }
        window.clear(sf::Color::Black);

        sf::Vector2i mouse = sf::Mouse::getPosition(window);
       
        sf::RectangleShape rect;
        rect.setPosition(sf::Vector2f(mouse.x - (mouse.x % 6), mouse.y - (mouse.y % 6)));
        rect.setFillColor(transparent);
        rect.setSize(sf::Vector2f(6, 6));
        window.draw(rect);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            int x = (mouse.x - (mouse.x % 6)) / 6;
            int y = (mouse.y - (mouse.y % 6)) / 6;
            if (grid[x][y] == 0) {
                grid[x][y] = 1;
            }
            else {
                grid[x][y] = 0;
            }
        }
        for (int i = 0;i < 100;i++) {
            for (int j = 0;j < 100;j++) {
                if (grid[i][j] == 1) {
                    sf::RectangleShape cell;
                    cell.setPosition(sf::Vector2f(i * 6, j * 6));
                    cell.setFillColor(sf::Color::White);
                    cell.setSize(sf::Vector2f(6, 6));
                    window.draw(cell);
                }
            }
        }
       
        if (evolve) {
            for (int i = 1;i < 99;i++) {
                for (int j = 1;j < 99;j++) {
                    int neighbors = 0;
                    /*for (int k = -1;k < 2;k++) {
                        for (int l = -1;l < 2;l++) {
                            if (grid[i + k][j + l] != grid[i][j]) {
                                if (grid[i + k][j + l] == 1) {
                                    neighbors++;
                                    
                                }
                            }
                        }
                    }*/
                    if (grid[i-1][j] == 1) {
                        neighbors++;
                    }
                    if (grid[i - 1][j-1] == 1) {
                        neighbors++;
                    }if (grid[i - 1][j+1] == 1) {
                        neighbors++;
                    }if (grid[i][j+1] == 1) {
                        neighbors++;
                    }if (grid[i][j-1] == 1) {
                        neighbors++;
                    }if (grid[i + 1][j] == 1) {
                        neighbors++;
                    }if (grid[i + 1][j-1] == 1) {
                        neighbors++;
                    }
                    if (grid[i + 1][j+1] == 1) {
                        neighbors++;
                    }

                    if (grid[i][j] == 1) {
                        if (neighbors == 2 || neighbors == 3) {
                            nextgrid[i][j] = 1;
                        }
                        else {
                            nextgrid[i][j] = 0;
                        }
                    }
                    if (grid[i][j] == 0 && neighbors == 3) {
                        nextgrid[i][j] = 1;
                    }
                    else if (grid[i][j] == 0) {
                        nextgrid[i][j] = 0;
                    }
                    
            
                }    
            }
            for (int i = 0;i < 100;i++) {
                for (int j = 0;j < 100;j++) {
                    grid[i][j] = nextgrid[i][j];
                    nextgrid[i][j] = 0;
                }
            }
            evolve = false;
        }

        window.display();
    }

    return 0;
}
