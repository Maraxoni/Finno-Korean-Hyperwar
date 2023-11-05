#include <iostream>
#include <string>
#include <curses.h>
#include <fstream>
#include "Game.h"
#include "MainFunctions.h"
#include "Settings.cpp"

using namespace std;

void showMainMenu() {
	extern Settings globalSettings;
	int rows = 0;
	int cols = 0;
	int position = 0;

	//pdcurses initialization
	initscr();
	curs_set(0);
	noecho();
	getmaxyx(stdscr, rows, cols); //1

	WINDOW* mainWin = newwin(LINES, COLS, 0, 0);
	WINDOW* titleWin = subwin(mainWin, 8, 118, 1, 1);
	WINDOW* menuWin = subwin(mainWin, 18, 38, (LINES - 18) / 2 + 3, (COLS - 38) / 2);
	WINDOW* toolbarWin = subwin(mainWin, 3, 120, 27, 0);
	//refresh();

	box(mainWin, 0, 0);
	box(menuWin, 0, 0);
	box(toolbarWin, 0, 0);
	box(titleWin, 0, 0);

	refresh();

	ifstream logoFile(".\\Resources\\Logo.txt");
	printImageHere(titleWin, logoFile, 1, 2);

	wrefresh(mainWin);

	while (1) {
		if (position < 0) {
			position = 2;
		}
		else {
			position = position % 3;
		}

		mvwprintw(toolbarWin, 1, 1, "Movement: Up/Down:%c/%c Left/Right:%c/%c", globalSettings.getKeyUp(), globalSettings.getKeyDown(), globalSettings.getKeyLeft(), globalSettings.getKeyRight());
		mvwprintw(toolbarWin, 1, 40, "Confirm: %c", globalSettings.getKeyConfirm());
		mvwprintw(toolbarWin, 1, 60, "Speed: %d Difficulty: %d", globalSettings.getSpeed(), globalSettings.getDifficulty());
		mvwprintw(toolbarWin, 1, 100, globalSettings.getChosenCountry().c_str());
		wrefresh(toolbarWin);

		mvwprintw(menuWin, (LINES - 18) / 2 - 2, ((COLS - 82) / 2) - (sizeof("Start Game") / 2), "Start Game");
		mvwprintw(menuWin, (LINES - 18) / 2 + 2, ((COLS - 82) / 2) - (sizeof("Options") / 2), "Options");
		mvwprintw(menuWin, (LINES - 18) / 2 + 6, ((COLS - 82) / 2) - (sizeof("Exit") / 2), "Exit");
		
		switch (position) {
		case 0:
			wattron(menuWin, A_STANDOUT);
			mvwprintw(menuWin, (LINES - 18) / 2 - 2, ((COLS - 82) / 2) - (sizeof("Start Game") / 2), "Start Game");
			wattroff(menuWin, A_STANDOUT);
			break;
		case 1:
			wattron(menuWin, A_STANDOUT);
			mvwprintw(menuWin, (LINES - 18) / 2 + 2, ((COLS - 82) / 2) - (sizeof("Options") / 2), "Options");
			wattroff(menuWin, A_STANDOUT);
			break;
		case 2:
			wattron(menuWin, A_STANDOUT);
			mvwprintw(menuWin, (LINES - 18) / 2 + 6, ((COLS - 82) / 2) - (sizeof("Exit") / 2), "Exit");
			wattroff(menuWin, A_STANDOUT);
			break;
		default:
			break;
		}

		wrefresh(menuWin);

		getch();
		char g = getch();
		endwin();
		if (g == globalSettings.getKeyUp()) {
			position--;
		}
		else if (g == globalSettings.getKeyDown()){
			position++;
		}
		else if (g == globalSettings.getKeyConfirm()) {
			ifstream logoFile(".\\Resources\\Logo.txt");
			switch (position) {
			case 0:
				showMap();
				wclear(mainWin);
				box(mainWin, 0, 0);
				box(menuWin, 0, 0);
				box(toolbarWin, 0, 0);
				box(titleWin, 0, 0);
				printImageHere(titleWin, logoFile, 1, 2);
				wrefresh(mainWin);
				break;
			case 1:
				showSettings();
				wclear(mainWin);
				box(mainWin, 0, 0);
				box(menuWin, 0, 0);
				box(toolbarWin, 0, 0);
				box(titleWin, 0, 0);
				printImageHere(titleWin, logoFile, 1, 2);
				wrefresh(mainWin);
				break;
			case 2:
				break;
			default:
				break;
			}
		}
	}
	endwin();

}

void showSettings() {
	extern Settings globalSettings;
	int positionY = 0;
	WINDOW* mainWin = newwin(LINES, COLS, 0, 0);
	WINDOW* settingsWin = subwin(mainWin, 18, 38, (LINES - 18) / 2 + 3, (COLS - 38) / 2);

	box(mainWin, 0, 0);
	box(settingsWin, 0, 0);

	wrefresh(mainWin);

	while (1) {
		if (positionY < 0) {
			positionY = 8;
		}
		else {
			positionY = positionY % 9;
		}
		globalSettings.loadFromFile();

		mvwprintw(settingsWin, 1, 1, "Functions:");
		mvwprintw(settingsWin, 2, 1, "Key Up");
		mvwprintw(settingsWin, 3, 1, "Key Down");
		mvwprintw(settingsWin, 4, 1, "Key Left");
		mvwprintw(settingsWin, 5, 1, "Key Right");
		mvwprintw(settingsWin, 6, 1, "Key Confirm");
		mvwprintw(settingsWin, 7, 1, "Chosen Country");
		mvwprintw(settingsWin, 8, 1, "Speed");
		mvwprintw(settingsWin, 9, 1, "Difficulty");
		mvwprintw(settingsWin, 10, 1, "Exit");
		mvwprintw(settingsWin, 1, 22, "Keys:");
		mvwprintw(settingsWin, 2, 22, "%c", globalSettings.getKeyUp());
		mvwprintw(settingsWin, 3, 22, "%c", globalSettings.getKeyDown());
		mvwprintw(settingsWin, 4, 22, "%c", globalSettings.getKeyLeft());
		mvwprintw(settingsWin, 5, 22, "%c", globalSettings.getKeyRight());
		mvwprintw(settingsWin, 6, 22, "%c", globalSettings.getKeyConfirm());
		mvwprintw(settingsWin, 7, 22, globalSettings.getChosenCountry().c_str());
		mvwprintw(settingsWin, 8, 22, "%d", 6-globalSettings.getSpeed());
		mvwprintw(settingsWin, 9, 22, "%d", globalSettings.getDifficulty());

		switch (positionY) {
		case 0:
			wattron(settingsWin, A_STANDOUT);
			mvwprintw(settingsWin, 2, 1, "Key Up");
			mvwprintw(settingsWin, 2, 22, "%c", globalSettings.getKeyUp());
			wattroff(settingsWin, A_STANDOUT);
			break;
		case 1:
			wattron(settingsWin, A_STANDOUT);
			mvwprintw(settingsWin, 3, 1, "Key Down");
			mvwprintw(settingsWin, 3, 22, "%c", globalSettings.getKeyDown());
			wattroff(settingsWin, A_STANDOUT);
			break;
		case 2:
			wattron(settingsWin, A_STANDOUT);
			mvwprintw(settingsWin, 4, 1, "Key Left");
			mvwprintw(settingsWin, 4, 22, "%c", globalSettings.getKeyLeft());
			wattroff(settingsWin, A_STANDOUT);
			break;
		case 3:
			wattron(settingsWin, A_STANDOUT);
			mvwprintw(settingsWin, 5, 1, "Key Right");
			mvwprintw(settingsWin, 5, 22, "%c", globalSettings.getKeyRight());
			wattroff(settingsWin, A_STANDOUT);
			break;
		case 4:
			wattron(settingsWin, A_STANDOUT);
			mvwprintw(settingsWin, 6, 1, "Key Confirm");
			mvwprintw(settingsWin, 6, 22, "%c", globalSettings.getKeyConfirm());
			wattroff(settingsWin, A_STANDOUT);
			break;
		case 5:
			wattron(settingsWin, A_STANDOUT);
			mvwprintw(settingsWin, 7, 1, "Chosen Country");
			mvwprintw(settingsWin, 7, 22, globalSettings.getChosenCountry().c_str());
			wattroff(settingsWin, A_STANDOUT);
			break;
		case 6:
			wattron(settingsWin, A_STANDOUT);
			mvwprintw(settingsWin, 8, 1, "Speed");
			mvwprintw(settingsWin, 8, 22, "%d", 6 - globalSettings.getSpeed());
			wattroff(settingsWin, A_STANDOUT);
			break;
		case 7:
			wattron(settingsWin, A_STANDOUT);
			mvwprintw(settingsWin, 9, 1, "Difficulty");
			mvwprintw(settingsWin, 9, 22, "%d", globalSettings.getDifficulty());
			wattroff(settingsWin, A_STANDOUT);
			break;
		case 8:
			wattron(settingsWin, A_STANDOUT);
			mvwprintw(settingsWin, 10, 1, "Exit");
			wattroff(settingsWin, A_STANDOUT);
			break;
		default:
			break;
		}
		
		wrefresh(settingsWin);

		getch();
		char g = getch();

		if (g == globalSettings.getKeyUp()) {
			positionY--;
		}
		else if (g == globalSettings.getKeyDown()) {
			positionY++;
		}
		else if (g == globalSettings.getKeyConfirm()) {
			char gIn;
			if (positionY == 0 || positionY == 1 || positionY == 2 || positionY == 3 || positionY == 4) {
				mvwprintw(settingsWin, positionY + 2, 22, "Enter new key");
				wrefresh(settingsWin);
			}
			if (positionY == 0) {
				gIn = getch();
				globalSettings.setKeyUp(gIn);
				wclear(settingsWin);
				box(settingsWin, 0, 0);
			} 
			else if (positionY == 1) {
				gIn = getch();
				globalSettings.setKeyDown(gIn);
				wclear(settingsWin);
				box(settingsWin, 0, 0);
			}
			else if (positionY == 2) {
				gIn = getch();
				globalSettings.setKeyLeft(gIn);
				wclear(settingsWin);
				box(settingsWin, 0, 0);
			}
			else if (positionY == 3) {
				gIn = getch();
				globalSettings.setKeyRight(gIn);
				wclear(settingsWin);
				box(settingsWin, 0, 0);
			}
			else if (positionY == 4) {
				gIn = getch();
				globalSettings.setKeyConfirm(gIn);
				wclear(settingsWin);
				box(settingsWin, 0, 0);
			}
			else if (positionY == 5) {
				if (globalSettings.getChosenCountry() == "Finland") {
					globalSettings.setChosenCountry("Korea");
					wclear(settingsWin);
					box(settingsWin, 0, 0);
				}
				else if(globalSettings.getChosenCountry() == "Korea"){
					globalSettings.setChosenCountry("Finland");
					wclear(settingsWin);
					box(settingsWin, 0, 0);
				}
			}
			else if (positionY == 6) {
				globalSettings.setSpeed(globalSettings.getSpeed() - 1);
			}
			else if (positionY == 7) {
				globalSettings.setDifficulty(globalSettings.getDifficulty() + 1);
			}
			else if (positionY == 8) {
				break;
			}

		}
		globalSettings.saveToFile();
	}
	endwin();
}

void printImage(WINDOW* currentWindow, ifstream& currentFile) {
	if (currentFile.is_open()) {
		string line;
		int idY_temp = 1;
		while (getline(currentFile, line)) {
			mvwprintw(currentWindow, idY_temp, 1, line.c_str());
			idY_temp++;
		}
		currentFile.close(); // closing file
	}
	else {
		mvwprintw(currentWindow, 0, 0, "Error loading file.");
		wrefresh(currentWindow);
	}
}

void printImageHere(WINDOW* currentWindow, ifstream& currentFile, int idY, int idX) {
	if (currentFile.is_open()) {
		string line;
		int idY_temp = idY;
		while (getline(currentFile, line)) {
			mvwprintw(currentWindow, idY_temp, idX, line.c_str());
			idY_temp++;
		}
		currentFile.close(); //Close File
		wrefresh(currentWindow);
	}
	else {
		//Error
		mvwprintw(currentWindow, 0, 0, "Error loading file here.");
		wrefresh(currentWindow);
	}
}

