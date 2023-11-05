#include <iostream>
#include <string>
#include <curses.h>
#include <fstream>
#include "MainFunctions.h"
#include "Country.cpp"
#include "Settings.cpp"
#include "Game.h"
#include <thread>
#include <mutex>

using namespace std;

Country Finland;
Country Korea;

void showMap() {
	extern Settings globalSettings;
	//terminal size
	int rows = 0;
	int cols = 0;

	//pdcurses initializations
	initscr();
	curs_set(0);
	raw();
	noecho();
	getmaxyx(stdscr, rows, cols);
	clear();
	start_color();

	//window sizes
	int height = 6;
	int width = 40;

	WINDOW* mainWin = newwin(LINES, COLS, 0, 0);
	WINDOW* gameWin = subwin(mainWin, 20, 120, (LINES - 20) / 2, (COLS - 120) / 2);

	WINDOW* resourcesWin = subwin(mainWin, 4, width + 24, 2, 28);
	WINDOW* flagWin = subwin(mainWin, height, width - 12, 0, 0);
	WINDOW* countryWin = subwin(mainWin, 3, width + 24, 0, 28);
	WINDOW* clockWin = subwin(mainWin, height, width - 12, 0, 92);

	WINDOW* logWin = subwin(mainWin, height-2, width - 10, 24, 0);
	WINDOW* selectMenuWin = subwin(mainWin, height-2, width - 10, 24, 30);
	WINDOW* toolbarWin = subwin(mainWin, height-3, width + 20, 27, 0);


	refresh();
	//boxes
	box(mainWin, 0, 0);

	box(resourcesWin, 0, 0);
	box(flagWin, 0, 0);
	box(countryWin, 0, 0);

	box(logWin, 0, 0);
	box(selectMenuWin, 0, 0);
	box(toolbarWin, 0, 0);

	box(gameWin, 0, 0);
	box(clockWin, 0, 0);

	refresh();

	//map and flag files
	ifstream mapFile(".\\Resources\\WorldMap.txt");
	printImage(gameWin, mapFile);

	//wrefresh(gameWin);
	//printing resources info
	mvwprintw(resourcesWin, 1, 1, "Food:");
	mvwprintw(resourcesWin, 1, 14, "Metal:");
	mvwprintw(resourcesWin, 1, 29, "Uranium:");
	mvwprintw(resourcesWin, 1, 44, "Nukes:");
	mvwprintw(resourcesWin, 1, 56, "Cities:");
	mvwprintw(countryWin, 1, 28, globalSettings.getChosenCountry().c_str());
	mvwprintw(toolbarWin, 1, 1, "Movement: Up/Down:%c/%c Left/Right:%c/%c", globalSettings.getKeyUp(), globalSettings.getKeyDown(), globalSettings.getKeyLeft(), globalSettings.getKeyRight());
	mvwprintw(toolbarWin, 1, 40, "Confirm: %c", globalSettings.getKeyConfirm());
	wrefresh(mainWin);


	//threads
	thread clockThread(showClock, clockWin);
	thread gameThread(game, gameWin, resourcesWin, clockWin, flagWin);
	thread listenerThread(gameListener, mainWin, selectMenuWin, logWin, flagWin);
	clockThread.join();
	gameThread.join();
	listenerThread.join();
	//game loop

	getch();
	endwin();
}

std::mutex mtx;

void showClock(WINDOW* clockWin) {
	int time_elapsed = 0;
	
	while (1) {
		mtx.lock();
		mvwprintw(clockWin, 3, 6, "Time elapsed: %d", time_elapsed);
		wrefresh(clockWin);
		mtx.unlock();

		time_elapsed++;
		napms(1000);
	}
}

void gameListener(WINDOW* mainWin, WINDOW* selectMenuWin, WINDOW* logWin, WINDOW* flagWin) {
	extern Settings globalSettings;

	Country* PlayerCountry = nullptr;
	Country* EnemyCountry = nullptr;

	if (globalSettings.getChosenCountry() == "Finland") {
		PlayerCountry = &Finland;
		EnemyCountry = &Korea;
	}
	else if (globalSettings.getChosenCountry() == "Korea") {
		PlayerCountry = &Korea;
		EnemyCountry = &Finland;
	}

	wrefresh(flagWin);
	//variables
	int positionX = 0;
	int positionY = 0;

	int height = 4;
	int width = 11;

	WINDOW* cityWin1 = subwin(mainWin, height, width, 25, 60);
	WINDOW* cityWin2 = subwin(mainWin, height, width, 25, 72);
	WINDOW* cityWin3 = subwin(mainWin, height, width, 25, 84);
	WINDOW* cityWin4 = subwin(mainWin, height, width, 25, 96);
	WINDOW* cityWin5 = subwin(mainWin, height, width, 25, 108);
	box(cityWin1, 0, 0);
	box(cityWin2, 0, 0);
	box(cityWin3, 0, 0);
	box(cityWin4, 0, 0);
	box(cityWin5, 0, 0);

	while (1) {
		if (positionX < 0) {
			positionX = 4;
		}
		else {
			positionX = positionX % 5;
		}
		if (positionY < 0) {
			positionY = 3;
		}
		else {
			positionY = positionY % 4;
		}

		wclear(cityWin1);
		wclear(cityWin2);
		wclear(cityWin3);
		wclear(cityWin4);
		wclear(cityWin5);

		box(cityWin1, 0, 0);
		box(cityWin2, 0, 0);
		box(cityWin3, 0, 0);
		box(cityWin4, 0, 0);
		box(cityWin5, 0, 0);

		switch (positionY) {
		case 0:
			if (globalSettings.getChosenCountry() == "Finland") {
				mvwprintw(cityWin1, 1, 1, "Helsinki");
				mvwprintw(cityWin2, 1, 1, "Tallinn");
				mvwprintw(cityWin3, 1, 1, "Dublin");
				mvwprintw(cityWin4, 1, 1, "Rome");
				mvwprintw(cityWin5, 1, 1, "Istanbul");
			}
			else if (globalSettings.getChosenCountry() == "Korea") {
				mvwprintw(cityWin1, 1, 1, "Seoul");
				mvwprintw(cityWin2, 1, 1, "Nanjing");
				mvwprintw(cityWin3, 1, 1, "Lhasa");
				mvwprintw(cityWin4, 1, 1, "Tuuva");
				mvwprintw(cityWin5, 1, 1, "Yakuck");
			}
			break;
		case 1:
			break;
		case 2:
			if (globalSettings.getChosenCountry() == "Finland") {
				mvwprintw(cityWin1, 1, 1, "Seoul");
				mvwprintw(cityWin2, 1, 1, "Nanjing");
				mvwprintw(cityWin3, 1, 1, "Lhasa");
				mvwprintw(cityWin4, 1, 1, "Tuuva");
				mvwprintw(cityWin5, 1, 1, "Yakuck");
			}
			else if (globalSettings.getChosenCountry() == "Korea") {
				mvwprintw(cityWin1, 1, 1, "Helsinki");
				mvwprintw(cityWin2, 1, 1, "Tallinn");
				mvwprintw(cityWin3, 1, 1, "Dublin");
				mvwprintw(cityWin4, 1, 1, "Rome");
				mvwprintw(cityWin5, 1, 1, "Istanbul");
			}
			break;
		case 3:
			break;
		default:
			break;
		}
		//highlight
		if (globalSettings.getChosenCountry() == "Finland"){
			switch (positionY) {
			case 0:
				switch (positionX) {
				case 0:
					wattron(cityWin1, A_STANDOUT);
					mvwprintw(cityWin1, 1, 1, "Helsinki");
					wattroff(cityWin1, A_STANDOUT);
					break;
				case 1:
					wattron(cityWin2, A_STANDOUT);
					mvwprintw(cityWin2, 1, 1, "Tallinn");
					wattroff(cityWin2, A_STANDOUT);
					break;
				case 2:
					wattron(cityWin3, A_STANDOUT);
					mvwprintw(cityWin3, 1, 1, "Dublin");
					wattroff(cityWin3, A_STANDOUT);
					break;
				case 3:
					wattron(cityWin4, A_STANDOUT);
					mvwprintw(cityWin4, 1, 1, "Rome");
					wattroff(cityWin4, A_STANDOUT);
					break;
				case 4:
					wattron(cityWin5, A_STANDOUT);
					mvwprintw(cityWin5, 1, 1, "Istanbul");
					wattroff(cityWin5, A_STANDOUT);
					break;
				default:
					break;
				}

				break;
			case 1:
				break;
			case 2:
				switch (positionX) {
				case 0:
					wattron(cityWin1, A_STANDOUT);
					mvwprintw(cityWin1, 1, 1, "Seoul");
					wattroff(cityWin1, A_STANDOUT);
					break;
				case 1:
					wattron(cityWin2, A_STANDOUT);
					mvwprintw(cityWin2, 1, 1, "Nanjing");
					wattroff(cityWin2, A_STANDOUT);
					break;
				case 2:
					wattron(cityWin3, A_STANDOUT);
					mvwprintw(cityWin3, 1, 1, "Lhasa");
					wattroff(cityWin3, A_STANDOUT);
					break;
				case 3:
					wattron(cityWin4, A_STANDOUT);
					mvwprintw(cityWin4, 1, 1, "Tuuva");
					wattroff(cityWin4, A_STANDOUT);
					break;
				case 4:
					wattron(cityWin5, A_STANDOUT);
					mvwprintw(cityWin5, 1, 1, "Yakuck");
					wattroff(cityWin5, A_STANDOUT);
					break;
				default:
					break;
				}
				break;
			case 3:
				break;
			default:
				break;
			}
		}
		else if (globalSettings.getChosenCountry() == "Korea") {
			switch (positionY) {
			case 0:
				switch (positionX) {
				case 0:
					wattron(cityWin1, A_STANDOUT);
					mvwprintw(cityWin1, 1, 1, "Seoul");
					wattroff(cityWin1, A_STANDOUT);
					break;
				case 1:
					wattron(cityWin2, A_STANDOUT);
					mvwprintw(cityWin2, 1, 1, "Nanjing");
					wattroff(cityWin2, A_STANDOUT);
					break;
				case 2:
					wattron(cityWin3, A_STANDOUT);
					mvwprintw(cityWin3, 1, 1, "Lhasa");
					wattroff(cityWin3, A_STANDOUT);
					break;
				case 3:
					wattron(cityWin4, A_STANDOUT);
					mvwprintw(cityWin4, 1, 1, "Tuuva");
					wattroff(cityWin4, A_STANDOUT);
					break;
				case 4:
					wattron(cityWin5, A_STANDOUT);
					mvwprintw(cityWin5, 1, 1, "Yakuck");
					wattroff(cityWin5, A_STANDOUT);
					break;
				default:
					break;
				}

				break;
			case 1:
				break;
			case 2:
				switch (positionX) {
				case 0:
					wattron(cityWin1, A_STANDOUT);
					mvwprintw(cityWin1, 1, 1, "Helsinki");
					wattroff(cityWin1, A_STANDOUT);
					break;
				case 1:
					wattron(cityWin2, A_STANDOUT);
					mvwprintw(cityWin2, 1, 1, "Tallinn");
					wattroff(cityWin2, A_STANDOUT);
					break;
				case 2:
					wattron(cityWin3, A_STANDOUT);
					mvwprintw(cityWin3, 1, 1, "Dublin");
					wattroff(cityWin3, A_STANDOUT);
					break;
				case 3:
					wattron(cityWin4, A_STANDOUT);
					mvwprintw(cityWin4, 1, 1, "Rome");
					wattroff(cityWin4, A_STANDOUT);
					break;
				case 4:
					wattron(cityWin5, A_STANDOUT);
					mvwprintw(cityWin5, 1, 1, "Istanbul");
					wattroff(cityWin5, A_STANDOUT);
					break;
				default:
					break;
				}
				break;
			case 3:
				break;
			default:
				break;
			}
		}
		wrefresh(cityWin1);
		wrefresh(cityWin2);
		wrefresh(cityWin3);
		wrefresh(cityWin4);
		wrefresh(cityWin5);

		mvwprintw(selectMenuWin, 1, 2, "Build");
		mvwprintw(selectMenuWin, 1, 17, "Nuke");
		mvwprintw(selectMenuWin, 2, 2, "Preapare Nuke");
		mvwprintw(selectMenuWin, 2, 17, "Exit");

		switch (positionY) {
		case 0:
			wattron(selectMenuWin, A_STANDOUT);
			mvwprintw(selectMenuWin, 1, 2, "Build");
			wattroff(selectMenuWin, A_STANDOUT);
			break;
		case 1:
			wattron(selectMenuWin, A_STANDOUT);
			mvwprintw(selectMenuWin, 2, 2, "Preapare Nuke");
			wattroff(selectMenuWin, A_STANDOUT);
			break;
		case 2:
			wattron(selectMenuWin, A_STANDOUT);
			mvwprintw(selectMenuWin, 1, 17, "Nuke");
			wattroff(selectMenuWin, A_STANDOUT);
			break;
		case 3:
			wattron(selectMenuWin, A_STANDOUT);
			mvwprintw(selectMenuWin, 2, 17, "Exit");
			wattroff(selectMenuWin, A_STANDOUT);
			break;
		default:
			break;
		}

		wrefresh(selectMenuWin);

		getch();
		char g = getch();

		if (g == globalSettings.getKeyUp()) {
			positionY--;
		}
		else if (g == globalSettings.getKeyDown()) {
			positionY++;
		}
		else if (g == globalSettings.getKeyLeft()) {
			positionX--;
		}
		else if (g == globalSettings.getKeyRight()) {
			positionX++;
		}
		else if (g == globalSettings.getKeyConfirm()) {
			switch (positionY) {
			case 0:
				if (PlayerCountry->getMetalStockpile() > 50) {
					wclear(logWin);
					box(logWin, 0, 0);
					mvwprintw(logWin, 1, 1, "Choose Building");
					mvwprintw(logWin, 2, 1, "1.Farm 2.Mine 3.Reactor");
					wrefresh(logWin);
					getch();
					char gIn = getch();
					switch (gIn) {
					case '1':
						PlayerCountry->setMetalStockpile(PlayerCountry->getMetalStockpile() - 50);
						PlayerCountry->cities[positionX].setFarms(PlayerCountry->cities[positionX].getFarms() + 1);
						wclear(logWin);
						box(logWin, 0, 0);
						mvwprintw(logWin, 1, 1, "Built Farm");
						wrefresh(logWin);
						break;
					case '2':
						PlayerCountry->setMetalStockpile(PlayerCountry->getMetalStockpile() - 50);
						PlayerCountry->cities[positionX].setMines(PlayerCountry->cities[positionX].getMines() + 1);
						wclear(logWin);
						box(logWin, 0, 0);
						mvwprintw(logWin, 1, 1, "Built Mine");
						wrefresh(logWin);
						break;
					case '3':
						PlayerCountry->setMetalStockpile(PlayerCountry->getMetalStockpile() - 50);
						PlayerCountry->cities[positionX].setReactors(PlayerCountry->cities[positionX].getReactors() + 1);
						wclear(logWin);
						box(logWin, 0, 0);
						mvwprintw(logWin, 1, 1, "Built Reactor");
						wrefresh(logWin);
						break;
					default:
						break;
					}
				}
				else {
					wclear(logWin);
					box(logWin, 0, 0);
					mvwprintw(logWin, 1, 1, "Not enough Metal");
					wrefresh(logWin);
				}
				break;
			case 1:
				if (PlayerCountry->getUraniumStockpile() >= 100) {
					wclear(logWin);
					box(logWin, 0, 0);
					mvwprintw(logWin, 1, 1, "Nuke Constructed");
					wrefresh(logWin);
					PlayerCountry->setUraniumStockpile(PlayerCountry->getUraniumStockpile() - 100);
					PlayerCountry->setNukes(PlayerCountry->getNukes() + 1);
				}
				else {
					wclear(logWin);
					box(logWin, 0, 0);
					mvwprintw(logWin, 1, 1, "Not enough Uranium");
					wrefresh(logWin);
				}
				break;
			case 2:
				if (PlayerCountry->getNukes()>0) {
					wclear(logWin);
					box(logWin, 0, 0);
					mvwprintw(logWin, 1, 1, "Nuke launched");
					wrefresh(logWin);
					PlayerCountry->setNukes(PlayerCountry->getNukes()-1);
					EnemyCountry->cities[positionX].setIsDestroyed(true);
				}
				else{
					wclear(logWin);
					box(logWin, 0, 0);
					mvwprintw(logWin, 1, 1, "Not enough Nukes");
					wrefresh(logWin);
				}
				break;
			case 3:
				exit(0);
			default:
				break;
			}
			

		}

		if (EnemyCountry->getNumberOfCities() == 0 || EnemyCountry->getFoodStockpile() < 0) {
			wclear(logWin);
			box(logWin, 0, 0);
			mvwprintw(logWin, 1, 1, "You've won!");
			wrefresh(logWin);
			break;
		}
		else if (PlayerCountry->getNumberOfCities() == 0 || PlayerCountry->getFoodStockpile() < 0) {
			wclear(logWin);
			box(logWin, 0, 0);
			mvwprintw(logWin, 1, 1, "You've lost!");
			wrefresh(logWin);
			break;
		}

		napms(10);
	}
}

void game(WINDOW* gameWin, WINDOW* resourcesWin, WINDOW* clockWin, WINDOW* flagWin) {
	extern Settings globalSettings;
	ifstream flagKoreaFile(".\\Resources\\FlagKorean.txt");
	ifstream flagFinlandFile(".\\Resources\\FlagFinnish.txt");
	Country* PlayerCountry = nullptr;
	Country* EnemyCountry = nullptr;

	if (globalSettings.getChosenCountry() == "Finland") {
		printImage(flagWin, flagFinlandFile);
		PlayerCountry = &Finland;
		EnemyCountry = &Korea;
	}
	else if (globalSettings.getChosenCountry() == "Korea") {
		printImage(flagWin, flagKoreaFile);
		PlayerCountry = &Korea;
		EnemyCountry = &Finland;
	}
	wrefresh(flagWin);


	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_CYAN, COLOR_BLACK);
	init_pair(3, COLOR_BLACK, COLOR_BLACK);

	if (globalSettings.getDifficulty() == 1) {
		EnemyCountry->setFoodStockpile(0);
		EnemyCountry->setMetalStockpile(0);
		EnemyCountry->setUraniumStockpile(0);
	}
	else if (globalSettings.getDifficulty() == 2) {
		EnemyCountry->setFoodStockpile(20);
		EnemyCountry->setMetalStockpile(20);
		EnemyCountry->setUraniumStockpile(20);
	}
	else if (globalSettings.getDifficulty() == 3) {
		EnemyCountry->setFoodStockpile(20);
		EnemyCountry->setMetalStockpile(20);
		EnemyCountry->setUraniumStockpile(20);
		EnemyCountry->setNukes(1);
	}

	int id = 4;
	int game_month = 1;
	int game_year = 2137;
	while (1) {
		if (game_month > 12) {
			game_month = 1;
			game_year++;
			wclear(clockWin);
			box(clockWin, 0, 0);
		}

		mvwprintw(resourcesWin, 2, 1, "%d#/100 (+%d)", PlayerCountry->getFoodStockpile(), PlayerCountry->getFoodIncome());
		mvwprintw(resourcesWin, 2, 14, "%d&/100 (+%d)", PlayerCountry->getMetalStockpile(), PlayerCountry->getMetalIncome());
		mvwprintw(resourcesWin, 2, 29, "%dU/100 (+%d)", PlayerCountry->getUraniumStockpile(), PlayerCountry->getUraniumIncome());
		mvwprintw(resourcesWin, 2, 44, "%d", PlayerCountry->getNukes());
		mvwprintw(resourcesWin, 2, 56, "%d", PlayerCountry->getNumberOfCities());
		wrefresh(resourcesWin);
		attron(COLOR_PAIR(2));
		Finland.showCity(gameWin, 0, 5, 28, "Helsinki");
		Finland.showCity(gameWin, 1, 7, 29, "Tallinn");
		Finland.showCity(gameWin, 2, 7, 10, "Dublin");
		Finland.showCity(gameWin, 3, 12, 20, "Rome");
		Finland.showCity(gameWin, 4, 14, 30, "Istanbul");
		attroff(COLOR_PAIR(2));
		attron(COLOR_PAIR(1));
		Korea.showCity(gameWin, 0, 14, 82, "Seoul");
		Korea.showCity(gameWin, 1, 17, 76, "Nanjing");
		Korea.showCity(gameWin, 2, 14, 57, "Lhasa");
		Korea.showCity(gameWin, 3, 10, 70, "Tuuva");
		Korea.showCity(gameWin, 4, 4, 88, "Yakuck");
		attroff(COLOR_PAIR(1));
		wrefresh(gameWin);

		PlayerCountry->setFoodStockpile(PlayerCountry->getFoodStockpile() + PlayerCountry->getFoodIncome());
		PlayerCountry->setMetalStockpile(PlayerCountry->getMetalStockpile() + PlayerCountry->getMetalIncome());
		PlayerCountry->setUraniumStockpile(PlayerCountry->getUraniumStockpile() + PlayerCountry->getUraniumIncome());

		EnemyCountry->setFoodStockpile(EnemyCountry->getFoodStockpile() + EnemyCountry->getFoodIncome());
		EnemyCountry->setMetalStockpile(EnemyCountry->getMetalStockpile() + EnemyCountry->getMetalIncome());
		EnemyCountry->setUraniumStockpile(EnemyCountry->getUraniumStockpile() + EnemyCountry->getUraniumIncome());

		
		if (EnemyCountry->getUraniumStockpile() >= 100) {
			EnemyCountry->setUraniumStockpile(EnemyCountry->getUraniumStockpile() - 100);
			EnemyCountry->setNukes(EnemyCountry->getNukes()+1);
		}
		if (EnemyCountry->getNukes() > 0) {
			EnemyCountry->setNukes(EnemyCountry->getNukes() - 1);
			PlayerCountry->cities[id].setIsDestroyed(true);
			id = id - 1;
			wrefresh(resourcesWin);
		}

		mtx.lock();
		mvwprintw(clockWin, 1, 3, "Month/Year: %d/%d BFK", game_month, game_year);
		wrefresh(clockWin);
		mtx.unlock();

		game_month++;
		napms(500*globalSettings.getSpeed());
	}
}
