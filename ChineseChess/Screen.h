#include<iostream>
#include<fstream>
#include<Windows.h>

//define 
#define screenX 81
#define screenY 23

#define UIY 21

#define logX 21
#define boardX 34
#define hintX 26

#define red 0
#define black 1

using namespace std;

class Screen{
public:
	Screen();
	~Screen();
	void update();
	void init();
private:
	//common
	void drawBorder(int xStart,int yStart, int xEnd, int yEnd);
	void writeString(int xStart, int y, string str, int color=15);
	void changeBlockColor(int xStart, int yStart, int xEnd, int yEnd, int color);
	
	//func
	void readBoard();
	void writeBoard();
	void drawUI();
	void drawLog();
	void drawBoard();
	void drawChess();
	void drawChess(int x, int y);
	void drawHint();
	string num2chess(int num, int side);

	//variable
	char screenBuffer[screenX][screenY];
	int screenColor[screenX][screenY];
	int board[9][10];
	int turn;

};

