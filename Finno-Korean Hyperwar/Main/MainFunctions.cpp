#include <iostream>
#include <string>
#include <curses.h>
#include <fstream>

using namespace std;

void showMainMenu() {
	int cols = 0;
	int rows = 0;
	int position = 0;

	initscr();
	getmaxyx(stdscr, rows, cols); //1

	while (1) {
		if (position < 0) {
			position = 2;
		}
		else {
			position = position % 3;
		}

		

		clear();
		switch (position) {
		case 0:
			mvprintw(rows / 3, (cols / 2) - 1 - (sizeof("New Game") / 2), ">"); //2
			break;
		case 1:
			mvprintw(rows / 2, (cols / 2) - 1 - (sizeof("Options") / 2), ">"); //2
			break;
		case 2:
			mvprintw(rows / 3 * 2, (cols / 2) - 1 - (sizeof("Exit") / 2), ">"); //2
			break;
		default:
			break;
		}


		mvprintw(rows / 3, (cols / 2) - (sizeof("Start") / 2), "Start"); //2
		mvprintw(rows / 2, (cols / 2) - (sizeof("Options") / 2), "Options"); //2
		mvprintw(rows / 3 * 2, (cols / 2) - (sizeof("Exit") / 2), "Exit"); //2
		mvprintw(rows / 6 * 5, (cols / 4), "Pozycja: %d", position);
		noecho();
		getch();
		char g = getch();
		endwin();
		switch (g)
		{
		case 'w':
			position--;
			break;
		case 's':
			position++;
			break;
		case 'z':
			switch (position) {
			case 0:
				//showMap();
				break;
			default:break;
			}
		default:
			break;
		}
	}

}

void printString(WINDOW* currentWindow, ifstream& currentFile) {
	if (currentFile.is_open()) {
		string line;
		int idY = 1;
		while (getline(currentFile, line)) {
			mvwprintw(currentWindow, idY, 1, line.c_str());
			idY++;
		}
		currentFile.close(); // Zamknij plik po u¿yciu
	}
	else {
		// Mo¿esz wypisaæ b³¹d do okna ncurses zamiast na konsoli
		mvwprintw(currentWindow, 0, 0, "Nie uda³o siê otworzyæ pliku.");
		wrefresh(currentWindow);
	}
}

void showMap() {
	int cols = 0;
	int rows = 0;

	initscr();
	raw();
	noecho();
	getmaxyx(stdscr, rows, cols); //1
	clear();

	int height = 6;
	int width = 40;
	//int starty = (LINES - height) / 2;
	//int startx = (COLS - width) / 2;

	WINDOW* mainWin = newwin(LINES, COLS, 0, 0);
	WINDOW* gameWin = subwin(mainWin, 20, 120, (LINES - 20) / 2, (COLS - 120) / 2);

	WINDOW* resourcesWin = subwin(mainWin, 4, width+24, 2, 28);
	WINDOW* flagWin = subwin(mainWin, height, width-12, 0, 0);
	WINDOW* countryWin = subwin(mainWin, 3, width+24, 0, 28);
	WINDOW* clockWin = subwin(mainWin, height, width-12, 0, 92);
	
	WINDOW* selectedWin = subwin(mainWin, height, width, 6, 6);
	WINDOW* toolbarWin = subwin(mainWin, height, width, 6, 6);
	WINDOW* placeholderWin = subwin(mainWin, height, width, 6, 6);
	refresh();
	box(mainWin, 0, 0);
	
	box(gameWin, 0, 0);

	box(resourcesWin, 0, 0);
	box(flagWin, 0, 0);
	box(countryWin, 0, 0);
	box(clockWin, 0, 0);
	refresh();
	string worldMap = "";
	string city = "";
	ifstream mapFile("D:\\Maraxowanie\\Programowanie\\KCK\\Finno-Korean Hyperwar Console\\Finno-Korean Hyperwar\\Resources\\WorldMap.txt");
	ifstream flagKoreaFile("D:\\Maraxowanie\\Programowanie\\KCK\\Finno-Korean Hyperwar Console\\Finno-Korean Hyperwar\\Resources\\FlagKorean.txt");
	ifstream flagFinlandFile("D:\\Maraxowanie\\Programowanie\\KCK\\Finno-Korean Hyperwar Console\\Finno-Korean Hyperwar\\Resources\\FlagFinnish.txt");
	ifstream cityFile("D:\\Maraxowanie\\Programowanie\\KCK\\Finno-Korean Hyperwar Console\\Finno-Korean Hyperwar\\Resources\\City.txt");

	printString(gameWin, mapFile);
	printString(clockWin, flagFinlandFile);
	printString(flagWin, flagKoreaFile);

	wrefresh(gameWin);
	wrefresh(clockWin);
	refresh();
	//box(toolbarWin, 0, 0);
	//mvwprintw(mainwin, 1, 1, "Bazinga");
	mvwprintw(resourcesWin, 1, 1, "Food:");
	mvwprintw(resourcesWin, 1, 14, "Metal:");
	mvwprintw(resourcesWin, 1, 29, "Uranium:");
	mvwprintw(resourcesWin, 1, 44, "Nukes:");
	mvwprintw(resourcesWin, 1, 56, "Cities:");
	mvwprintw(countryWin, 1, 28, "Hyperborea");
	wrefresh(mainWin);
	refresh();
	getch();
	/*while (1) {
		
		
		cout << "test";
		
		//box(mainwin, 0, 0);  // Draw a border around the subwindow
		
		
		
		box(citywindow, 0, 0);  // Draw a border around the subwindow
		// Print content to the sub-window
		
		wrefresh(citywindow);
		refresh();

		

		

		
		
		//cout << worldMap;
		//mvprintw(rows / 2, (cols) / 2, "%s", worldMapC); //2
		//mvprintw( 5 , 5, cityC); //2
		//refresh();

		getch();
		napms(5000);
		
	}*/
	endwin();
}