#include<iostream>
#include<fstream>
#include<Windows.h>

//define out border size
#define screenX 81
#define screenY 23
//define UI height
#define UIY 21
//define UI width
#define logX 21
#define boardX 34
#define hintX 26

#define RED 1
#define BLACK 0
//define xy, easy to access array
#define X 0
#define Y 1

//define arrow face
#define UP 0
#define DOWN 1
#define LEFT 2 
#define RIGHT 3
//define moveable table content
#define CANTMOVE 0
#define MOVEABLE 1
#define EATABLE 2

#define KINGZONE_X 5
#define KINGZONE_BLACK_Y 1
#define KINGZONE_RED_Y 9


using namespace std;

enum gameState
{
	PICK = 0,
	MOVE,
};

class Screen{
public:
	Screen();
	~Screen();
	void update();
	void init();
	void callMenu();
	void moveCursor(int face);
	void checkCursor();

	bool showMenu = false;
private:
	//common
	void drawBorder(int xStart,int yStart, int xEnd, int yEnd);
	void writeString(int xStart, int y, string str, int color=15);
	void changeBlockColor(int xStart, int yStart, int xEnd, int yEnd, int color);
	void cleanBlock(int xStart, int yStart, int xEnd, int yEnd, int color);
	bool markMove(int x, int y, int yStart, int yEnd, int xStart, int xEnd);
	void changeTurn() { if (turn == RED) turn = BLACK; else turn = RED;};

	//draw func
	void readBoard();
	void writeBoard();
	void drawUI();
	void drawCursor();
	void drawMoveable();
	void drawLog();
	void drawMenu();
	void drawBoard();
	void drawChess(int x, int y);
	void drawHint();

	//other func
	string num2chess(int num, int side);
	bool calcAvaliableMove();

	//variable
	char screenBuffer[screenX][screenY];
	int screenColor[screenX][screenY];
	int board[9][10];
	int moveAble[9][10];
	int turn;
	int cursor[2];
	int choosedChess[2];
	int gameState = PICK;

};

