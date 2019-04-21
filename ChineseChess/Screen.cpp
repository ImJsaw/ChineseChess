#include "pch.h"
#include "Screen.h"


Screen::Screen(){
}


Screen::~Screen(){
}

void Screen::update(){
	//clean screen
	system("cls");
	readBoard();
	//draw to buffer, easier to manage
	drawBorder(0,0,screenX-1,screenY-1);
	drawUI();
	//draw buffer to screen
	for (int y = 0; y < screenY; y++) {
		cout << "    ";
		for (int x = 0; x < screenX; x++) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), screenColor[x][y]);
			putchar(screenBuffer[x][y]);
		}
		putchar('\n');
	}
}

void Screen::init()
{
	//init text color
	for (int i = 0; i < screenX; i++)
		for (int j = 0; j < screenY; j++)
			screenColor[i][j] = 15;//15 : white text, black BG (normal)
}

void Screen::drawUI(){
	drawLog();
	drawBoard();
	drawHint();
}

void Screen::drawBorder(int xStart, int yStart, int xEnd, int yEnd){
	for (int x = xStart + 1; x < xEnd; x++) {
		//top border
		screenBuffer[x][yStart] = '_';
		//down border
		screenBuffer[x][yEnd] = '_';
	}

	for (int y = yStart + 1; y < yEnd; y++) {
		//left border
		screenBuffer[xStart][y] = '|';
		//right border
		screenBuffer[xEnd][y] = '|';
	}
	//edge
	screenBuffer[xStart][yStart] = ' ';
	screenBuffer[xStart][yEnd] = '|';
	screenBuffer[xEnd][yStart] = ' ';
	screenBuffer[xEnd][yEnd] = '|';
}

void Screen::writeString(int xStart, int y, string str, int color){
	int len = str.length();
	for (int i = 0; i < len; i++) {
		screenBuffer[xStart + i][y] = str[i];
		screenColor[xStart + i][y] = color;
	}
}

void Screen::changeBlockColor(int xStart, int yStart, int xEnd, int yEnd, int color){
	for (int y = yStart; y <= yEnd; y++)
		for (int x = xStart; x <= xEnd; x++)
			screenColor[x][y] = color;
}

void Screen::readBoard(){
	fstream boardFile;
	boardFile.open("data.txt");
	for (int y = 0; y < 10; y++)
		for (int x = 0; x < 9; x++) {
			boardFile >> board[x][y];
		}
	boardFile >> turn;
	boardFile.close();
}

void Screen::writeBoard(){
	fstream boardFile;
	boardFile.open("data.txt");
	for (int y = 0; y < 10; y++) {
		for (int x = 0; x < 9; x++) {
			boardFile << board[x][y];
		}
		boardFile << endl;
	}
	boardFile << endl << turn;
	boardFile.close();
}

void Screen::drawLog(){
	//(1,1) ~ (logX, UIY)
	drawBorder(1, 1, logX, UIY);
	writeString(5, 1, "             ");
	writeString(5, 2, " 戰 況 顯 示 ", 10);//10:green text,black BG
}

void Screen::drawBoard(){
	//(logX+1,1) ~ (logX+1 + boardX, UIY)
	int whiteBG = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
	writeString(logX+1, 1, " 1   2   3   4   5   6   7   8   9", whiteBG);
	writeString(logX + 1, UIY, "九  八  七  六  五  四  三  二  一", whiteBG);
	//midLine
	for(int i = 0;i < 9;i++)
		writeString(logX + 1, 3+2*i, "|   |   |   |   |   |   |   |    |", whiteBG);
	//up kingZone
	writeString(logX + 1 + 14, 3, "\\ | /", whiteBG);
	writeString(logX + 1 + 14, 5, "/ | \\", whiteBG);
	//down kingZone
	writeString(logX + 1 + 14, 17, "\\ | /", whiteBG);
	writeString(logX + 1 + 14, 19, "/ | \\", whiteBG);

	//row line
	for (int i = 0; i < 8; i++)
		writeString(logX + 1, 4 + 2 * i, "|---+---+---+---+---+---+---+---| ", whiteBG);	
	for (int i = logX + 3; i < logX + boardX; i++) {
		screenBuffer[i][UIY-1] = '-';
		screenBuffer[i][2] = '-';
	}

	writeString(logX + 1, 11, "|         楚河     漢界          | ", whiteBG);


	//draw board BG
	changeBlockColor(logX + 1, 1, logX + boardX + 1, UIY, whiteBG);

	for (int y = 0; y < 10; y++) {
		for (int x = 0; x < 9; x++) {
			drawChess(x, y);
		}

	}
	
}

void Screen::drawChess() {
	int whiteBG = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;

	for (int y = 0; y < 10; y++) {

		//writeString(logX + 1, 2 + 2 * y, num2chess(board[0][y], 0), whiteBG);
		for (int x = 1; x < 8; x++) {
			drawChess(x, y);
			//writeString(logX + 1 + 4 * x, 2 + 2 * y, num2chess(board[x][y], 1), whiteBG);
		}
		//writeString(logX + 4 * 8, 2 + 2 * y, num2chess(board[8][y], 2), whiteBG);

	}
}

void Screen::drawChess(int x, int y) {
	int bg;
	if (board[x][y] > 7) bg = FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;//red text
	else bg = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;//white

	int side = 1;
	if (x == 0) side = 0;
	else if (x == 8) side = 2;
	writeString(logX + 1 + 4 * x, 2 + 2 * y, num2chess(board[x][y], side), bg);
}

void Screen::drawHint(){
	//(screenX - hintX - 1,1) ~ (screenX -1 , UIY)
	drawBorder(logX + boardX + 2, 1, screenX - 2, UIY);
}

string Screen::num2chess(int num, int side){
	string text = "";
	switch (num){
	case 1:
		text =  "將";
		break;
	case 2:
		text = "士";
		break;
	case 3:
		text = "象";
		break;
	case 4:
		text = "車";
		break;
	case 5:
		text = "馬";
		break;
	case 6:
		text = "包";
		break;
	case 7:
		text = "卒";
		break;
	case 8:
		text = "帥";
		break;
	case 9:
		text = "仕";
		break;
	case 10:
		text = "相";
		break;
	case 11:
		text = "俥";
		break;
	case 12:
		text = "傌";
		break;
	case 13:
		text = "砲";
		break;
	case 14:
		text = "兵";
		break;
	default:
		if (side == 2) return "-|";
		if (side == 0) return "|-";
		text = "+-";
		break;
	}
	return  text;
}



