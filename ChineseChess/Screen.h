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

#define red 0
#define black 1
//define xy, easy to access array
#define X 0
#define Y 1

//define arrow face
#define UP 0
#define DOWN 1
#define LEFT 2 
#define RIGHT 3

using namespace std;

enum gameState
{
	chooseChess,
	chooseMove,
	chooseEat
};

class Screen{
public:
	Screen();
	~Screen();
	void update();
	void init();
	void moveCursor(int face);
private:
	//common
	void drawBorder(int xStart,int yStart, int xEnd, int yEnd);
	void writeString(int xStart, int y, string str, int color=15);
	void changeBlockColor(int xStart, int yStart, int xEnd, int yEnd, int color);
	
	//draw func
	void readBoard();
	void writeBoard();
	void drawUI();
	void drawCursor();
	void drawLog();
	void drawBoard();
	void drawChess(int x, int y);
	void drawHint();

	//other func
	string num2chess(int num, int side);

	//variable
	char screenBuffer[screenX][screenY];
	int screenColor[screenX][screenY];
	int board[9][10];
	int turn;
	int cursor[2];

};

