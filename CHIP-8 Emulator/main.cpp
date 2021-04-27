
//Place Rom in the same folder and name it "ROM".



#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>

#include "main.h"

sf::RenderWindow window(sf::VideoMode(1280, 640), "CHIP-8");
sf::Event event;
int fps = 300;
int hz = 60;
float dt = hz / fps;
int unit = 20;

class CHIP {
public:
    int instruction{};
    unsigned int memory[4096] = {};
    int display[64][32] = { {} };
    int PC{0x200};
    int index{};
    int stack[16] = {};
    int stackpointer{0};
    int delay{};
    int soundtimer{};
    bool keys[16] = {};
    uint8_t registers [16] = {};
    int fonts[80] = { 0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
                    0x20, 0x60, 0x20, 0x20, 0x70, // 1
                    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
                    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
                    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
                    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
                    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
                    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
                    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
                    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
                    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
                    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
                    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
                    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
                    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
                    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
                    };
    void Execute(int instruction) {
        int first = instruction >> 12;
        int second = (instruction >> 8) & 0x0F;
        int third = (instruction >> 4) & 0x00f;
        int fourth = instruction & 0x000f;
        switch (instruction) {
        //Clear Screen
        case 0x00E0:
            memset(display, 0, sizeof display);
            break;
        case 0x00EE:
            stackpointer -= 1;
            PC = stack[stackpointer];
            stack[stackpointer] = 0;
            break;
        }
        switch (first) {
        case 1:
            PC = instruction & 0x0FFF;
            break;
        case 2:
            stack[stackpointer] = PC;
            PC = instruction & 0x0FFF;
            stackpointer += 1;
            break;
        case 3:
            if (registers[second] == (instruction & 0x00FF)) {
                PC += 2;
            }
            break;
        case 4:
            if (registers[second] != (instruction & 0x00FF)) {
                PC += 2;
            }
            break;
        case 5:
            if (registers[second] == registers[third]) {
                PC += 2;
            }
            break;
        case 9:
            if (registers[second] != registers[third]) {
                PC += 2;
            }
            break;
        case 6:
            registers[second] = (instruction & 0x0FF);
            break;
        case 7:
            registers[second] += (instruction & 0x0FF);
            break;
        case 0xA:
            index = (instruction & 0x0FFF);
            break;
        case 0xD:
        {
            int x = registers[second] % 64;
            int y = registers[third] % 32;
            int n = fourth;
            registers[0xF] = 0;
            for (int i = 0;i < n; i++) {
                int sprite = memory[index + i];
                for (int j = 0;j < 8;j++) {
                    int spritepixel = sprite & (0x80 >> j);
                    if (spritepixel) {
                        if (display[x + j][y + i]) {
                            registers[0xF] = 1;
                        }
                        display[x + j][y + i] ^= 1;
                    }
                }
            }
            break;
        }
        case 0x8:
            switch (fourth) {
            case 0:
                registers[second] = registers[third];
                break;
            case 0x1:
                registers[second] = registers[second] | registers[third];
                break;
            case 0x2:
                registers[second] = registers[second] & registers[third];
                break;
            case 0x3:
                registers[second] = registers[second] ^ registers[third];
                break;
            case 0x4:
                registers[second] += registers[third];
                if (registers[second] + registers[third] > 255) {
                    registers[0xF] = 1;
                }
                else {
                    registers[0xF] = 0;
                }
                break;
            case 0x5:
                registers[second] -= registers[third];
                if (registers[second] >= registers[third]) {
                    registers[0xF] = 1;
                }
                else {
                    registers[0xF] = 0;
                }
                break;
            case 0x7:
                registers[second] = registers[third] - registers[second];
                if (registers[second] < registers[third]) {
                    registers[0xF] = 1;
                }
                else {
                    registers[0xF] = 0;
                }
                break;
            case 0x6:
                registers[second] = registers[third];
                registers[0xF] = (registers[second] % 2);
                registers[second] >> 1;
                break;
            case 0xE:
                registers[second] = registers[third];
                int msb = 0;
                int n = registers[second];
                if (!n == 0) {
                    n = n / 2;
                    while (n != 0) {
                        n = n / 2;
                        msb++;
                    }
                }
                msb = (1 << msb);
                registers[0xF] = msb;
                registers[second] << 1;
                break;
            }
            break;
        case 0xB:
            PC = (instruction & 0x0FFF) + registers[0];
            break;
        case 0xC:
            registers[second] = rand() & (instruction & 0x00FF);
            break;
        case 0xE:
            if ((instruction & 0x00FF) == 0x009E) {
                if (keys[registers[second]]) {
                    PC += 2;
                }
                break;
            }
            if ((instruction & 0x00FF) == 0x00A1) {
                if (!keys[registers[second]]) {
                    PC += 2;
                }
                break;
            }
            break;
        case 0xF:
            switch (instruction & 0x00FF) {
            case 0x07:
                registers[second] = delay;
                break;
            case 0x0A:
                if (keys[0]) {
                    registers[second] = 0;
                }
                else if (keys[1]) {
                    registers[second] = 1;
                }
                else if (keys[2]) {
                    registers[second] = 2;
                }
                else if (keys[3]) {
                    registers[second] = 3;
                }
                else if (keys[4]) {
                    registers[second] = 4;
                }
                else if (keys[5]) {
                    registers[second] = 5;
                }
                else if (keys[6]) {
                    registers[second] = 6;
                }
                else if (keys[7]) {
                    registers[second] = 7;
                }
                else if (keys[8]) {
                    registers[second] = 8;
                }
                else if (keys[9]) {
                    registers[second] = 9;
                }
                else if (keys[10]) {
                    registers[second] = 10;
                }
                else if (keys[11]) {
                    registers[second] = 11;
                }
                else if (keys[12]) {
                    registers[second] = 12;
                }
                else if (keys[13]) {
                    registers[second] = 13;
                }
                else if (keys[14]) {
                    registers[second] = 14;
                }
                else if (keys[15]) {
                    registers[second] = 15;
                }
                break;
            case 0x15:
                delay = registers[second];
                break;
            case 0x18:
                soundtimer = registers[second];
                break;
            case 0x1E:
                index += registers[second];
                break;
            case 0x29:
                index = 0x50 + (registers[second] * 5);
                break;
            case 0x33:
            {
                int vx = registers[second];
                memory[index] = vx / 100;
                memory[index + 1] = (vx / 10) % 10;
                memory[index + 2] = vx % 10;
                break;
            }
            case 0x55:
                for (int i = 0;i <= second;i++) {
                    memory[index + i] = registers[i];
                }
                index = index + second + 1;
                break;
            case 0x65:
                for (int i = 0;i <= second;i++) {
                    registers[i] = memory[index + i];
                }
                index = index + second + 1;
                break;
            }
            break;
        }
    }

    void Draw() {
        for (int i = 0;i < 64;i++) {
                for (int j = 0;j < 32;j++) {
                if (display[i][j] == 1) {
                    sf::RectangleShape pixel;
                    pixel.setSize(sf::Vector2f(unit, unit));
                    pixel.setFillColor(sf::Color::White);
                    pixel.setPosition(sf::Vector2f(i * unit, j * unit));
                    window.draw(pixel);
                }
            }
        }
    }
    void Cycle() {
        instruction = memory[PC] << 8 | memory[PC + 1];
        PC += 2;
        Execute(instruction);
        Draw();
    }

};

int main()
{
    CHIP chip;
    
    std::ifstream rom("ROM", std::ios::binary);
    int begin = rom.tellg();
    rom.seekg(0, std::ios::end);
    int end = rom.tellg();
    int size = end - begin;
    rom.seekg(0, std::ios::beg);
    char * buffer = new char [size];
    rom.read(buffer, size);
    for (int i = 0; i < size;i++) {
        chip.memory[i + 0x200] = (unsigned int)(unsigned char)buffer[i];

    }
    for (int i = 0x50; i <= 0x9F;i++) {
        chip.memory[i] = chip.fonts[i - 0x50];
    }
    
    delete[] buffer;
    window.setFramerateLimit(fps);
    window.clear(sf::Color::Black);
    sf::Clock clock;
    clock.restart();
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
       
        if (clock.getElapsedTime().asSeconds() >= dt) {
            if (chip.delay > 0) {
                chip.delay -= 1;
            }
            if (chip.soundtimer > 0) {
                chip.soundtimer -= 1;
            }
            clock.restart();
        }


        chip.keys[0] = sf::Keyboard::isKeyPressed(sf::Keyboard::Num1);
        chip.keys[1] = sf::Keyboard::isKeyPressed(sf::Keyboard::Num2);
        chip.keys[2] = sf::Keyboard::isKeyPressed(sf::Keyboard::Num3);
        chip.keys[3] = sf::Keyboard::isKeyPressed(sf::Keyboard::Num4);
        chip.keys[4] = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
        chip.keys[5] = sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
        chip.keys[6] = sf::Keyboard::isKeyPressed(sf::Keyboard::E);
        chip.keys[7] = sf::Keyboard::isKeyPressed(sf::Keyboard::R);
        chip.keys[8] = sf::Keyboard::isKeyPressed(sf::Keyboard::Q);
        chip.keys[9] = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
        chip.keys[10] = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
        chip.keys[11] = sf::Keyboard::isKeyPressed(sf::Keyboard::F);
        chip.keys[12] = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
        chip.keys[13] = sf::Keyboard::isKeyPressed(sf::Keyboard::X);
        chip.keys[14] = sf::Keyboard::isKeyPressed(sf::Keyboard::C);
        chip.keys[15] = sf::Keyboard::isKeyPressed(sf::Keyboard::V);
        
        window.clear(sf::Color::Black);
        
        chip.Cycle();

        window.display();
    }
}
