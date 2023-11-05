#include <curses.h>
#include <fstream>

void showMainMenu();
void showSettings();
void printImage(WINDOW* currentWindow, std::ifstream& currentFile);
void printImageHere(WINDOW* currentWindow, std::ifstream& currentFile, int idY, int idX);