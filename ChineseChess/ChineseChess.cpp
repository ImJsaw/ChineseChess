
#include "pch.h"
#include "Screen.h"


Screen myScreen;
bool up,down,r,l,enter;

void initial() {
	//init screen
	myScreen.init();
	//init key
	up = false;
	down = false;
	r = false;
	l = false;
}

//key press and release count one time & update screen
void keyEvent() {

	if (GetAsyncKeyState(VK_UP))
		up = true;
	if (!GetAsyncKeyState(VK_UP) && up == true) {
		up = false;
		myScreen.moveCursor(UP);
		myScreen.update();
		//up arrow event
		printf("The Up Arrow Has Been Pressed\n");
	}

	if (GetAsyncKeyState(VK_DOWN))
		down = true;
	if (!GetAsyncKeyState(VK_DOWN) && down == true) {
		down = false;
		myScreen.moveCursor(DOWN);
		myScreen.update();
		//down arrow event

		printf("The DOWN Arrow Has Been Pressed\n");
	}

	if (GetAsyncKeyState(VK_LEFT))
		l = true;
	if (!GetAsyncKeyState(VK_LEFT) && l == true) {
		l = false;
		myScreen.moveCursor(LEFT);
		myScreen.update();
		//left arrow event

		printf("The LEFT ARROW Has Been Pressed\n");
	}

	if (GetAsyncKeyState(VK_RIGHT))
		r = true;
	if (!GetAsyncKeyState(VK_RIGHT) && r == true) {
		r = false;
		myScreen.moveCursor(RIGHT);
		myScreen.update();
		//rihgt arrow event
		printf("The RIGHT ARROW Has Been Pressed\n");
	}

	if (GetAsyncKeyState(VK_RETURN))
		enter = true;
	if (!GetAsyncKeyState(VK_RETURN) && enter == true) {
		enter = false;

		myScreen.checkCursor();
		//enter event

		printf("ENTER Has Been Pressed\n");
	}
}

int main(){
	
	initial();

	for (;;) keyEvent();

}


