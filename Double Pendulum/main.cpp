

#include <iostream>
#include <cmath>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>

#include "main.h"
#include <vector>


int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 16;
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Double Pendulum", sf::Style::Default, settings);
    sf::Event event;
    int fps = 60;
    window.setFramerateLimit(fps);
    float dt = 1 / fps;

    bool Run = false;
    
    constexpr double  PI = 3.141592653589793238463;
    
    float zerox = window.getSize().x / 2;
    float zeroy = window.getSize().y / 2;

    float x1 = 0;
    float y1 = 0;
    float x2 = 0;
    float y2 = 0;
    float m1 = 0.1;
    float m2 = 0.1;
    float r1 = 150;
    float r2 = 150;
    float a1 = 0;
    float a2 = 0;
    float a1_v = 0;
    float a2_v = 0;
    float a1_a = 0;
    float a2_a = 0;
    float g = 1;
    float deg1 = 0;
    float deg2 = 0;

    float timescale = 1;
    float dampning = 0.99999;

    float width = 3;
    float radius = 20;
    sf::CircleShape circle1(radius);
    sf::CircleShape circle2(radius);
    sf::RectangleShape line1(sf::Vector2f(r1, width));
    sf::RectangleShape line2(sf::Vector2f(r2, width));
    circle1.setOrigin(sf::Vector2f(radius, radius));
    circle2.setOrigin(sf::Vector2f(radius, radius));
    circle1.setFillColor(sf::Color::Green);
    circle2.setFillColor(sf::Color::Green);
    line1.setFillColor(sf::Color::White);
    line2.setFillColor(sf::Color::White);

    std::vector<sf::Vector2f> points;
    sf::Color tracecolor = sf::Color::Color(0, 138, 224, 255);
    int maxsize = 0;

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Space) {
                    if (Run) {
                        Run = false;
                        a1_a = 0;
                        a1_v = 0;
                        a2_a = 0;
                        a2_v = 0;
                    }
                    else {
                        Run = true;
                    }
                }
                if (event.key.code == sf::Keyboard::Right && !Run) {
                    a1 += PI/36;
                }
                if (event.key.code == sf::Keyboard::Left && !Run) {
                    a1 -= PI / 36;
                }
                if (event.key.code == sf::Keyboard::Up && !Run) {
                    a2 += PI / 36;
                }
                if (event.key.code == sf::Keyboard::Down && !Run) {
                    a2 -= PI / 36;
                }
                if (event.key.code == sf::Keyboard::R) {
                    points.clear();
                    a1 = 0;
                    a2 = 0;
                    a1_a = 0;
                    a1_v = 0;
                    a2_a = 0;
                    a2_v = 0;
                    Run = false;
                }
            }
        }
        if (Run) {
            float num1 = -g * (2 * m1 + m2) * sin(a1);
            float num2 = -m2 * g * sin(a1 - 2 * a2);
            float num3 = -2 * sin(a1 - a2) * m2 * (a2_v * a2_v * r2 + a1_v * a1_v * r1 * cos(a1 - a2));
            float den = r1 * (2 * m1 + m2 - m2 * cos(2 * a1 - 2 * a2));
            a1_a = (num1 + num2 + num3) / den;

            num1 = 2 * sin(a1 - a2);
            num2 = a1_v * a1_v * r1 * (m1 + m2) + g * (m1 + m2) * cos(a1);
            num3 = a2_v * a2_v * r2 * m2 * cos(a1 - a2);
            den = r2 * (2 * m1 + m2 - m2 * cos(2 * a1 - 2 * a2));
            a2_a = (num1 * (num2 + num3)) / den;

            a1_v += a1_a * timescale;
            a2_v += a2_a * timescale;
            a1 += a1_v * timescale;
            a2 += a2_v * timescale;
            a1_v *= dampning;
            a2_v *= dampning;
        }
        x1 = zerox + r1 * sin(a1);
        y1 = zeroy + r1 * cos(a1);
        x2 = x1 + r2 * sin(a2);
        y2 = y1 + r2 * cos(a2);

        deg1 = a1 * (180/PI);
        deg2 = a2 * (180/PI);
        if (Run) {
            points.push_back(sf::Vector2f(x2, y2));
        }
        if (points.size() > maxsize && maxsize != 0) {
            points.erase(points.begin());
        }
        window.clear(sf::Color::Black);
        circle1.setPosition(x1, y1);
        circle2.setPosition(x2, y2);
        line1.setPosition(zerox, zeroy);
        line2.setPosition(x1, y1);
        line1.setRotation(90 - deg1);
        line2.setRotation(90 - deg2);

        if (points.size() > 1) {
            for (int i = 0;i < points.size() - 1;i++) {
                sf::Vertex trace[] = {
                    sf::Vertex(points[i], tracecolor),
                    sf::Vertex(points[i + 1], tracecolor)
                };
                window.draw(trace, 2, sf::Lines);
            }
        }

        window.draw(line1);
        window.draw(line2);
        window.draw(circle1);
        window.draw(circle2);
        

        window.display();

        
    }
}
