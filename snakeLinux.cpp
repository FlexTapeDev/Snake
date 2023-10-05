#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>	
#include <termios.h>
#include <fcntl.h>
#include <time.h>

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

bool gameOver;
const int hight = 20;
const int width = 50;

int x, y, fruitX, fruitY, score;

int tailX[100], tailY[100], nTail;

enum eDirection { STOP = 0, UP, DOWN, LEFT, RIGHT };
eDirection dir;

void Setup() {
	gameOver = false;
	dir = STOP;
	x = width / 2;
	y = hight / 2;

	srand(time(NULL));

	fruitX = rand() % width;
	fruitY = rand() % hight;
};
void Draw() {


	system("clear");

	for (int i = 0; i < width; i++) {
		std::cout << "#";
	} std::cout << std::endl;

	for (int i = 0; i < hight; i++) {
		for (int j = 0; j < width; j++) {
			if (j == 0 || j == width - 1) {
				std::cout << "#";
			}
			else if (i == y && j == x) {
				std::cout << "ö";
			}
			else if (i == fruitY && j == fruitX) {
				std::cout << "@";
			}
			else
			{
				bool print = false;

				for (int k = 0; k < nTail; k++) {
					if (i == tailY[k] && j == tailX[k]) {
						std::cout << "o";
						print = true;
					}
				}if (!print) { std::cout << " "; }
			}

		} std::cout << std::endl;
	}

	for (int i = 0; i < width; i++) {
		std::cout << "#";
	} std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "Score: " << score << std::endl;
};

int kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

void Input() {
    if (kbhit()) {
        char input = getchar();

        switch (input) {
            case 'w':
            case KEY_UP:
                dir = UP;
                break;

            case 'a':
            case KEY_LEFT:
                dir = LEFT;
                break;

            case 's':
            case KEY_DOWN:
                dir = DOWN;
                break;

            case 'd':
            case KEY_RIGHT:
                dir = RIGHT;
                break;

            default:
                break;
        }
    }
}


void Logic() {

	int prevX = tailX[0];
	int prevY = tailY[0];

	tailX[0] = x;
	tailY[0] = y;

	int prev2X, prev2Y;

	for (int i = 1; i < nTail; i++) {

		prev2X = tailX[i];
		prev2Y = tailY[i];

		tailX[i] = prevX;
		tailY[i] = prevY;

		prevX = prev2X;
		prevY = prev2Y;

	}


	switch (dir) {
	case UP:

		y--;
		break;

	case DOWN:
		y++;
		break;

	case LEFT:
		x--;
		break;

	case RIGHT:
		x++;
		break;

	default:
		break;

	}
	if (x<0 || x>width || y<0 || y>hight) {
		gameOver = true;
	}

	for (int i = 0; i < nTail; i++) {
		if (x == tailX[i] && y == tailY[i]) {
			gameOver = true;
		}
	}

	if (x == fruitX && y == fruitY) {
		score += 1;
		srand(time(NULL));

		fruitX = rand() % width;
		fruitY = rand() % hight;
		nTail++;
	}
};

int main() {
	Setup();
	while (!gameOver) {
		Draw();
		Input();
		Input();
		Logic();
		usleep(150000);
	}
	system("clear");

	std::cout << "Game Over!\n" << "score: " << score << std::endl;

	usleep(1000000);

	gameOver = false;

	nTail = 0;
	score = 0;

	main();
}
