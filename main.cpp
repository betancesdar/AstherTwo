/*
	Created by: Omar Pena, 
	Year: 2024,
	Version: 1.1,
	Language: C++,
	Credits: 
*/
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <mmsystem.h>
#include <stdlib.h>
#include <list>
using namespace std;

#define UP 72
#define LEFT 75
#define RIGHT 77
#define DOWN 80
#define SHOOT_KEY 'a'

void gotoxy(int x, int y) {
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD dwPos;
	dwPos.X = x;
	dwPos.Y = y;

	SetConsoleCursorPosition(hCon, dwPos);
}

void HideCursor() {
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 1;
	cci.bVisible = FALSE;

	SetConsoleCursorInfo(hCon, &cci);
}

void draw_limit() {
	for (int i = 2; i < 78; i++) {
		gotoxy(i, 3);  printf("%c", 186);
		gotoxy(i, 33); printf("%c", 186);
	}
	for (int i = 4; i < 33; i++) {
		gotoxy(2, i); printf("%c", 186);
		gotoxy(77, i); printf("%c", 186);
	}

	gotoxy(2, 3); printf("%c", 201);
	gotoxy(2, 33); printf("%c", 200);
	gotoxy(77, 3); printf("%c", 187);
	gotoxy(77, 33); printf("%c", 188);

}

class SHIP {
private: 
	int x, y;
	int hearts;
	int lives;
	//std::vector<std::pair<int, int>> bullets; 

public: 
	SHIP(int _x, int _y, int _hearts, int _lives) : x(_x), y(_y), hearts(_hearts), lives(_lives) {}
	int X() { return x; }
	int Y() { return y; }
	void HER() { hearts--; }
	void draw();
	void remove();
	void move();
	void draw_heart();
	void lose();
	void shoot();
};

void SHIP::draw() {
	gotoxy(x, y);	  printf("  T  ");
	gotoxy(x, y + 1); printf("->*<-");
	gotoxy(x, y + 2); printf("*****");
}

void SHIP::remove() {
	gotoxy(x, y);	  printf("        ");
	gotoxy(x, y + 1); printf("        ");
	gotoxy(x, y + 2); printf("        ");
}

void SHIP::move() {
	if (_kbhit()) {
		char key = _getch();
		remove();
		if (key == LEFT && x > 3) x--;
		if (key == RIGHT && x + 6 < 77) x++;
		if (key == UP && y > 4) y--;
		if (key == DOWN && y + 3 < 33) y++;
		draw();
	}

	/*for (size_t i = 0; i < bullets.size(); ++i) {
		gotoxy(bullets[i].first, bullets[i].second);
		printf(" ");
		bullets[i].second--;

		if (bullets[i].second <= 0) {
			bullets.erase(bullets.begin() + i);
			i--;
		}
		else {
			gotoxy(bullets[i].first, bullets[i].second);
			printf("^");
		}

	}*/
}

/*void SHIP::shoot() {
	bullets.push_back(std::make_pair(x + 2, y - 1));
}*/

void clearScreen() {
	system("cls");
}

void SHIP::draw_heart() {
	gotoxy(50, 2); printf("Lives %d", lives);
	gotoxy(64, 2); printf("Health ");
	gotoxy(70, 2); printf("      ");

	for (int i = 0; i < hearts; i++) {
		
		gotoxy(71 + i, 2); printf("%c", 3);
	}
}

void SHIP::lose() {
	if (hearts == 0) {
		remove();

		gotoxy(x, y);    printf("   **   ");
		gotoxy(x, y+1);  printf("  ****  ");
		gotoxy(x, y+2);  printf("   **   ");
		Sleep(200);

		remove();
		gotoxy(x, y);	   printf(" * ** *");
		gotoxy(x, y + 1);  printf("  **** ");
		gotoxy(x, y + 2);  printf(" * ** *");
		Sleep(200);
		remove();
		lives--;
		hearts = 3;
		draw_heart();
		draw();
	}
}

class AST {
	int x, y;
public:
	AST(int _x, int _y) : x(_x), y(_y) {}
	void draw();
	void move();
	void crash(class SHIP& N); //Buenas practicas al utilizar la memoria heap quitarle el tipo a la clase o estructura 
};

//Heap

void AST::draw() {
	gotoxy(x, y); printf("%c", 199);
}

void AST::move() {
	gotoxy(x, y); printf(" ");
	y++;
	if (y > 32) {
		x = rand() % 71 + 4;
		y = 4;
	}

	draw();
}

void AST::crash(SHIP& N) {
	if (x >= N.X() && x < N.X() + 6 && y >= N.Y() && y <= N.Y() + 2) {
		N.HER();
		N.remove();
		N.draw();
		N.draw_heart();
		x = rand() % 71 + 4;
		y = 4;
	}
}

void PlayStartMusic() {
	PlaySound(L"C:\\Users\\AlexScott\\source\\repos\\AstrerTwo\\AstrerTwo\\splashsong.wav", NULL, SND_FILENAME | SND_LOOP);
}

void PlayGameMusic() {
	//Stop the splash
	PlaySound(NULL, 0, 0);

	PlaySound(L"C:\\Users\\AlexScott\\source\\repos\\AstrerTwo\\AstrerTwo\\gamesong.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}

class BULLET {
	int x, y;
public:
	BULLET(int _x, int _y) : x(_x), y(_y) {}
	void move();
};

void BULLET::move() {
	gotoxy(x,y); printf(" ");
	if (y > 4) y--;
	gotoxy(x, y); printf("^");

}

int main() {
	HideCursor();
	draw_limit();

	printf("\n\n\t\t\t AstherTwo - The vengace\n\n");
	printf("\t\t\t 1. Start\n");
	printf("\t\t\t 2. Exit\n");
	PlayStartMusic();

	int choice;
	scanf_s("%d", &choice);

	switch (choice)
	{
	case 1: {
		PlayGameMusic();
		SHIP n(7, 7, 3, 3);
		n.draw();
		n.draw_heart();

		AST ast(10, 4), ast2(4, 8), ast3(10, 15);

		list<BULLET*> B; 
		list<BULLET*>::iterator it;

		bool game_over = false;

		while (!game_over) {

			if (_kbhit()) {
			
				char key = _getch();
				if (key == SHOOT_KEY) {
					B.push_back(new BULLET(n.X() + 2, n.Y() - 1));
				}
			}

			for (it = B.begin(); it != B.end(); it++ ) {
				(*it)->move(); //desferencia puntero puntero es la dirrecion
			}

			ast.move(); ast.crash(n);
			ast2.move(); ast2.crash(n);
			ast3.move(); ast3.crash(n);

			n.lose();
			n.move();
			Sleep(30);
		}
		break;
	}
	case 2:
		break;

	default:
		printf("Invalid choice\n");
		break;
	}

	return 0;
}