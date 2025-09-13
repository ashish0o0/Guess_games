#include <iostream>
#include <conio.h>
#include <windows.h>
#include <chrono>
using namespace std;
using namespace std::chrono;

bool gameOver;
const int width = 20;
const int height = 17;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;
HANDLE hConsole;
CHAR_INFO consoleBuffer[width * height];

void SetupConsole() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT rect = {0, 0, static_cast<SHORT>(width - 1), static_cast<SHORT>(height - 1)};
    SetConsoleWindowInfo(hConsole, TRUE, &rect);
    COORD bufferSize = {width, height};
    SetConsoleScreenBufferSize(hConsole, bufferSize);
}

void Setup() {
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    nTail = 0;
}

void Draw() {
    for (int i = 0; i < width * height; i++) {
        consoleBuffer[i].Char.AsciiChar = ' ';
        consoleBuffer[i].Attributes = 0x07;
    }

    for (int i = 0; i < width; i++) {
        consoleBuffer[i].Char.AsciiChar = '#';
        consoleBuffer[i].Attributes = 0x07;
        consoleBuffer[(height - 1) * width + i].Char.AsciiChar = '#';
        consoleBuffer[(height - 1) * width + i].Attributes = 0x07;
    }

    for (int i = 0; i < height; i++) {
        consoleBuffer[i * width].Char.AsciiChar = '#';
        consoleBuffer[i * width].Attributes = 0x07;
        consoleBuffer[i * width + (width - 1)].Char.AsciiChar = '#';
        consoleBuffer[i * width + (width - 1)].Attributes = 0x07;
    }

    consoleBuffer[y * width + x].Char.AsciiChar = 'O';
    consoleBuffer[y * width + x].Attributes = 0x0A;

    for (int i = 0; i < nTail; i++) {
        consoleBuffer[tailY[i] * width + tailX[i]].Char.AsciiChar = 'o';
        consoleBuffer[tailY[i] * width + tailX[i]].Attributes = 0x0A;
    }

    consoleBuffer[fruitY * width + fruitX].Char.AsciiChar = 'F';
    consoleBuffer[fruitY * width + fruitX].Attributes = 0x0C;

    SMALL_RECT rect = {0, 0, static_cast<SHORT>(width - 1), static_cast<SHORT>(height - 1)};
    WriteConsoleOutputA(hConsole, consoleBuffer, {width, height}, {0, 0}, &rect);
}

void Input() {
    if (_kbhit()) {
        switch (_getch()) {
            case 'a':
                dir = LEFT;
                break;
            case 'd':
                dir = RIGHT;
                break;
            case 'w':
                dir = UP;
                break;
            case 's':
                dir = DOWN;
                break;
            case 'p':
                dir = STOP;
                break;
            case 'x':
                gameOver = true;
                break;
        }
    }
}

void Logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }
    switch (dir) {
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        default:
            break;
    }
    if (x >= width) x = 0; else if (x < 0) x = width - 1;
    if (y >= height) y = 0; else if (y < 0) y = height - 1;

    for (int i = 0; i < nTail; i++)
        if (tailX[i] == x && tailY[i] == y)
            gameOver = true;

    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;
    }
}

int main() {
    SetupConsole();
    Setup();
    auto lastTime = high_resolution_clock::now();
    while (!gameOver) {
        auto currentTime = high_resolution_clock::now();
        duration<float> elapsedTime = currentTime - lastTime;
        if (elapsedTime.count() >= 0.1f) {
            lastTime = currentTime;
            Draw();
            Input();
            Logic();
        }
    }
    return 0;
}
