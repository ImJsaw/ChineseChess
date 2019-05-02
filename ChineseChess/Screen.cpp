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
	//
	cout <<"gameState" <<gameState << endl;
	if (gameState == MOVE) {
		drawMoveable();
	}
	drawCursor();
	//draw buffer to screen
	for (int y = 0; y < screenY; y++) {
		cout << "    ";
		for (int x = 0; x < screenX; x++) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), screenColor[x][y]);
			putchar(screenBuffer[x][y]);
		}
		putchar('\n');
	}
	//cursor position
	cout << "cursor : " << cursor[X] << "," << cursor[Y] << endl;
}

void Screen::init()
{
	//init text color
	for (int i = 0; i < screenX; i++)
		for (int j = 0; j < screenY; j++)
			screenColor[i][j] = 15;//15 : white text, black BG (normal)
	//init cursor 
	cursor[X] = 0;
	cursor[Y] = 0;
}

void Screen::drawUI(){
	drawLog();
	drawBoard();
	drawHint();
}

void Screen::drawCursor(){
	int cursorBG = BACKGROUND_BLUE | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
	screenColor[logX + 1 + 4 * cursor[X]][2 + 2*cursor[Y]] = cursorBG;
	screenColor[logX + 2 + 4 * cursor[X]][2 + 2 * cursor[Y]] = cursorBG;
}

void Screen::drawMoveable(){
	int moveBG = BACKGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
	for (int y = 0; y < 10; y++) {
		for (int x = 0; x < 9; x++) {
			if (moveAble[x][y] == MOVEABLE) {
				//cout << "x,y:" << x << "," << y << endl;
				//system("pause");
				screenColor[logX + 1 + 4 * x][2 + 2 * y] = moveBG;
				screenColor[logX + 2 + 4 * x][2 + 2 * y] = moveBG;
			}
		}
	}
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

bool Screen::markMove(int x, int y, int yStart = 0, int yEnd = 9, int xStart = 0, int xEnd = 8){
	if (x < xStart || x > xEnd) //x exceed border
		return false;
	if (y < yStart || y > yEnd) //y exceed border
		return false;
	if (board[x][y] != 0)//not blank //eat
		return false;
	moveAble[x][y] = MOVEABLE;

	return true;
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
			boardFile << board[x][y] << " ";
		}
		boardFile << endl;
	}
	boardFile << turn;
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

void Screen::moveCursor(int face){
	switch (face){
	case UP:
		if (cursor[Y] > 0) cursor[Y]--;
		break;
	case DOWN:
		if (cursor[Y] < 9) cursor[Y]++;
		break;
	case LEFT:
		if (cursor[X] > 0) cursor[X]--;
		break;
	case RIGHT:
		if (cursor[X] < 8) cursor[X]++;
		break;
	default:
		break;
	}
}

void Screen::checkCursor(){
	switch (gameState){
	case PICK:
		if (calcAvaliableMove()) {
			//switch to move state
			gameState = MOVE;
			choosedChess[X] = cursor[X];
			choosedChess[Y] = cursor[Y];
			cout << "pick valiable chess";
			update();
			
		}
		break;
	case MOVE:
		if (moveAble[cursor[X]][cursor[Y]] == CANTMOVE) {
			//switch to move state
			gameState = PICK;
			cout << "goto pick state";
			update();
		}
		else if (moveAble[cursor[X]][cursor[Y]] == MOVEABLE) {
			gameState = PICK;
			cout << "MOVE!!!" << choosedChess[X] << "," << choosedChess[Y] 
				<< "to" << cursor[X] << "," << cursor[Y];
			board[cursor[X]][cursor[Y]] = board[choosedChess[X]][choosedChess[Y]];
			board[choosedChess[X]][choosedChess[Y]] = 0;
			//system("pause");
			writeBoard();
			update();
		}

		break;
	default:
		break;
	}
	
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
		text = "車";
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

bool Screen::calcAvaliableMove(){
	if (board[cursor[X]][cursor[Y]] == 0) {
		cout << "pick blank" << endl;
		return false;
	}
	//pick self chess
	cout << "pick" << board[cursor[X]][cursor[Y]] << endl;

	//init moveable table
	for (int y = 0; y < 10; y++)
		for (int x = 0; x < 9; x++)
			moveAble[x][y] = CANTMOVE;
	int x = cursor[X];
	int y = cursor[Y];
	int tmpX, tmpY;

	switch (board[x][y]){
	case 1://將
		cout << "pick 1";
		markMove(x, y - 1, 0, 2, 3, 5);
		markMove(x, y + 1, 0, 2, 3, 5);
		markMove(x-1, y, 0, 2, 3, 5);
		markMove(x+1, y, 0, 2, 3, 5);
		if ((x + y) % 2 == 1) {//line use
			markMove(x-1, y - 1, 0, 2,3,5);
			markMove(x+1, y + 1, 0, 2, 3, 5);
			markMove(x - 1, y+1, 0, 2, 3, 5);
			markMove(x + 1, y-1, 0, 2, 3, 5);
		}
		break;
	case 8://帥
		cout << "pick 1";
		markMove(x, y - 1, 7, 9, 3, 5);
		markMove(x, y + 1, 7, 9, 3, 5);
		markMove(x - 1, y, 7, 9, 3, 5);
		markMove(x + 1, y, 7, 9, 3, 5);
		if ((x + y) % 2 == 0) {//line use
			markMove(x - 1, y - 1, 7, 9, 3, 5);
			markMove(x + 1, y + 1, 7, 9, 3, 5);
			markMove(x - 1, y + 1, 7, 9, 3, 5);
			markMove(x + 1, y - 1, 7, 9, 3, 5);
		}
		break;
	case 2://士
		cout << "pick 1";
		if ((x + y) % 2 == 1) {//line use
			markMove(x - 1, y - 1, 0, 2, 3, 5);
			markMove(x + 1, y + 1, 0, 2, 3, 5);
			markMove(x - 1, y + 1, 0, 2, 3, 5);
			markMove(x + 1, y - 1, 0, 2, 3, 5);
		}
		break;
	case 9:
		if ((x + y) % 2 == 0) {//line use
			markMove(x - 1, y - 1, 7, 9, 3, 5);
			markMove(x + 1, y + 1, 7, 9, 3, 5);
			markMove(x - 1, y + 1, 7, 9, 3, 5);
			markMove(x + 1, y - 1, 7, 9, 3, 5);
		}
		break;
	case 3://象
		if (board[x + 1][y + 1] == 0)
			markMove(x + 2, y + 2, 0, 4);
		if (board[x + 1][y - 1] == 0)
			markMove(x + 2, y - 2, 0, 4);
		if (board[x - 1][y + 1] == 0)
			markMove(x - 2, y + 2, 0, 4);
		if (board[x - 1][y - 1] == 0)
			markMove(x - 2, y - 2, 0, 4);
		break;
	case 10://象
		if (board[x + 1][y + 1] == 0)
			markMove(x + 2, y + 2, 5, 9);
		if (board[x + 1][y - 1] == 0)
			markMove(x + 2, y - 2, 5, 9);
		if (board[x - 1][y + 1] == 0)
			markMove(x - 2, y + 2, 5, 9);
		if (board[x - 1][y - 1] == 0)
			markMove(x - 2, y - 2, 5, 9);
		break;
	case 4://車
	case 11:
	case 6://包
	case 13:
		tmpX = x+1;
		while (markMove(tmpX, y)) {
			tmpX++;
		}
		tmpX = x-1;
		while (markMove(tmpX, y)) {
			tmpX--;
		}
		tmpY = y+1;
		while (markMove(x, tmpY)) {
			tmpY++;
		}
		tmpY = y-1;
		while (markMove(x, tmpY)) {
			tmpY--;
		}
		break;
	case 5://馬
	case 12:
		if (board[x][y + 1] == 0) {
			markMove(x + 1, y + 2);
			markMove(x - 1, y + 2);
		}
		if (board[x][y - 1] == 0) {
			markMove(x + 1, y - 2);
			markMove(x - 1, y - 2);
		}
		if (board[x+1][y] == 0) {
			markMove(x + 2, y + 1);
			markMove(x + 2, y - 1);
		}
		if (board[x-1][y] == 0) {
			markMove(x-2, y + 1);
			markMove(x-2, y -1);
		}
		break;
	case 7://卒
		markMove(x, y + 1);
		break;
	case 14://兵
		markMove(x, y - 1);
		break;

	default:
		break;
	}
	return true;
	
}
