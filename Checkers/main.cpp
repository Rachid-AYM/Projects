

#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>

class Board {
public:
    int size = 8;

    void Draw(sf::RenderWindow& window) {
        sf::RectangleShape tile;
        tile.setSize(sf::Vector2f(75.f, 75.f));
        for (int i = 0;i < size;i++) {
            for (int j = 0;j < size;j++) {
                tile.setPosition(sf::Vector2f(75 * i, 75 * j));
                if ((i + j) % 2 == 0) {
                    tile.setFillColor(sf::Color::White);
                }
                else {
                    tile.setFillColor(sf::Color(55, 204, 179, 255));
                }
                window.draw(tile);
            }
        }
    }

    void Highlight(sf::RenderWindow& window, int x, int y) {
        sf::RectangleShape tile;
        tile.setSize(sf::Vector2f(75.f, 75.f));
        tile.setFillColor(sf::Color::Green);
        tile.setPosition(sf::Vector2f(75 * x, 75 * y));
        window.draw(tile);
    }
};

class Piece {
public:
    float radius = 30;
    int x;
    int y;
    bool isAlive;
    bool isKing = false;
    sf::Color color;

    void Draw(sf::RenderWindow& window) {
        if (isAlive) {
            sf::CircleShape shape(radius);
            shape.setFillColor(color);
            if (isKing) {
                shape.setOutlineThickness(3.f);
                shape.setOutlineColor(sf::Color::Yellow);
            }
            shape.setPosition(sf::Vector2f(x * 75 + (75 - radius * 2) / 2, y * 75 + (75 - 2 * radius) / 2));
            window.draw(shape);
        }
    }
};

void Setup(sf::RenderWindow& window, Piece* RedPieces, Piece* WhitePieces) {
    int m = 0;
    for (int i = 0;i < 3;i++) {
        for (int j = (int)!(i%2 & 1);j < 8;j += 2) {
            WhitePieces[m].isAlive = true;
            WhitePieces[m].x = j;
            WhitePieces[m].y = i;
            m++;
        }
        
    }
    m = 0;
    for (int i = 0;i < 3;i++) {
        for (int j = (int)(i % 2 & 1);j < 8;j += 2) {
            RedPieces[m].isAlive = true;
            RedPieces[m].x = j;
            RedPieces[m].y = 7-i;
            m++;
        }

    }
}

Piece* FindPiece(int x, int y, Piece* RedPieces, Piece* WhitePieces) {
    for (int i = 0; i < 12;i++) {
        if (RedPieces[i].x == x && RedPieces[i].y == y) {
            if (RedPieces[i].isAlive) {
                return &RedPieces[i];
            }
        }
        if (WhitePieces[i].x == x && WhitePieces[i].y == y) {
            if (WhitePieces[i].isAlive) {
                return &WhitePieces[i];
            }
        }
    }
    return NULL;
}

void KillPiece(int x, int y, Piece* RedPieces, Piece* WhitePieces, int *turn) {
    FindPiece(x, y, RedPieces, WhitePieces)->isAlive = false;
    *turn = ((*turn == 1) ? 2 : 1);
    return;
}

int MovePiece(int x, int y, Piece* s_Piece, Piece* RedPieces, Piece* WhitePieces, int *turn) {
        if (s_Piece->color == sf::Color::Red || s_Piece->color == sf::Color::White && s_Piece->isKing) {
            if (x == s_Piece->x - 1 && y == s_Piece->y - 1) {
                if (!FindPiece(x, y, RedPieces, WhitePieces)) {
                    *turn = ((*turn == 1) ? 2 : 1);
                    s_Piece->x = x;
                    s_Piece->y = y;
                    return 1;
                }
            }
            if (x == s_Piece->x + 1 && y == s_Piece->y - 1) {
                if (!FindPiece(x, y, RedPieces, WhitePieces)) {
                    *turn = ((*turn == 1) ? 2 : 1);
                    s_Piece->x = x;
                    s_Piece->y = y;
                    return 1;
                }
            }
            if (x == s_Piece->x - 2 && y == s_Piece->y - 2) {
                if (!FindPiece(x, y, RedPieces, WhitePieces) && FindPiece(x+1,y+1,RedPieces, WhitePieces) != NULL && FindPiece(x + 1, y + 1, RedPieces, WhitePieces)->color != s_Piece->color) {
                    *turn = ((*turn == 1) ? 2 : 1);
                    KillPiece(x + 1, y + 1, RedPieces, WhitePieces, turn);
                    s_Piece->x = x;
                    s_Piece->y = y;
                    return 1;
                }
            }
            if (x == s_Piece->x + 2 && y == s_Piece->y - 2) {
                if (!FindPiece(x, y, RedPieces, WhitePieces) && FindPiece(x - 1, y + 1, RedPieces, WhitePieces) != NULL && FindPiece(x - 1, y + 1, RedPieces, WhitePieces)->color != s_Piece->color) {
                    *turn = ((*turn == 1) ? 2 : 1);
                    KillPiece(x - 1, y + 1, RedPieces, WhitePieces, turn);
                    s_Piece->x = x;
                    s_Piece->y = y;
                    return 1;
                }
            }
        }
        if (s_Piece->color == sf::Color::White || s_Piece->color == sf::Color::Red && s_Piece->isKing) {
            if (x == s_Piece->x - 1 && y == s_Piece->y + 1) {
                if (!FindPiece(x, y, RedPieces, WhitePieces)) {
                    *turn = ((*turn == 1) ? 2 : 1);
                    s_Piece->x = x;
                    s_Piece->y = y;
                    return 1;
                }
            }
            if (x == s_Piece->x + 1 && y == s_Piece->y + 1) {
                if (!FindPiece(x, y, RedPieces, WhitePieces)) {
                    *turn = ((*turn == 1) ? 2 : 1);
                    s_Piece->x = x;
                    s_Piece->y = y;
                    return 1;
                }
            }
            if (x == s_Piece->x - 2 && y == s_Piece->y + 2) {
                if (!FindPiece(x, y, RedPieces, WhitePieces) && FindPiece(x + 1, y - 1, RedPieces, WhitePieces) != NULL && FindPiece(x + 1, y - 1, RedPieces, WhitePieces)->color != s_Piece->color) {
                    *turn = ((*turn == 1) ? 2 : 1);
                    s_Piece->x = x;
                    s_Piece->y = y;
                    KillPiece(x + 1, y - 1, RedPieces, WhitePieces, turn);
                    return 1;
                }
            }
            if (x == s_Piece->x + 2 && y == s_Piece->y + 2) {
                if (!FindPiece(x, y, RedPieces, WhitePieces) && FindPiece(x - 1, y - 1, RedPieces, WhitePieces) != NULL && FindPiece(x - 1, y - 1, RedPieces, WhitePieces)->color != s_Piece->color) {
                    *turn = ((*turn == 1) ? 2 : 1);
                    s_Piece->x = x;
                    s_Piece->y = y;
                    KillPiece(x - 1, y - 1, RedPieces, WhitePieces, turn);
                    return 1;
                }
            }
        }
        return 0;
}

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 16.0;
    sf::RenderWindow window(sf::VideoMode(600, 600), "Checkers", sf::Style::Default, settings);
    sf::Event event;
    Board board;
    int grid[8][8];
    Piece RedPieces[12];
    Piece WhitePieces[12];
    bool selected = false;
    Piece* SelectedPiece = NULL;
    int turn = 1;

    for (int i = 0;i < 12;i++) {
        WhitePieces[i].color = sf::Color::White;
        RedPieces[i].color = sf::Color::Red;
    }

    
    Setup(window, RedPieces, WhitePieces);

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    selected = !selected;
                }
            }
        }

        window.clear();

        board.Draw(window);

        if (SelectedPiece != NULL) {
            board.Highlight(window, SelectedPiece->x, SelectedPiece->y);
        }
        
        for (int i = 0;i < 12;i++) {
            WhitePieces[i].Draw(window);
            RedPieces[i].Draw(window);
        }

        if (selected) {
            int x = sf::Mouse::getPosition(window).x / 75;
            int y = sf::Mouse::getPosition(window).y / 75;
            if (FindPiece(x, y, RedPieces, WhitePieces) && (FindPiece(x, y, RedPieces, WhitePieces)->color == sf::Color::Red && turn == 1 || FindPiece(x, y, RedPieces, WhitePieces)->color == sf::Color::White && turn == 2)) {
                if (FindPiece(x, y, RedPieces, WhitePieces) == SelectedPiece) {
                    SelectedPiece = NULL;
                }
                else {
                    SelectedPiece = FindPiece(x, y, RedPieces, WhitePieces);
                }
            
                selected = false;
            }
            else if (SelectedPiece != NULL && MovePiece(x, y, SelectedPiece, RedPieces, WhitePieces, &turn)) {
                selected = false;
                SelectedPiece = NULL;
            }
            selected = false;
        }
        for (int i = 0;i < 12;i++) {
            if (RedPieces[i].y == 0) {
                RedPieces[i].isKing = true;
            }
            if (WhitePieces[i].y == 7) {
                WhitePieces[i].isKing = true;
            }
        }
        

        window.display();
    }
    return 0;
}
