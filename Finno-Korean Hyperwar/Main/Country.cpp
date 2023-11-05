#include <fstream>
#include <iostream>
#include <string>
#include <curses.h>
#include "Country.h"
#include "MainFunctions.h"
#include "City.cpp"

class Country {
    int limit = 100;
	int food_stockpile = 0;
	int metal_stockpile = 0;
	int uranium_stockpile = 0;
	int nukes = 0;
public:
	City cities[5];

	//getters and setters

    int getFoodStockpile() const {
        return food_stockpile;
    }

    void setFoodStockpile(int value) {
        if (value > limit) {
            value -= (value - limit);
        }
        food_stockpile = value;
    }

    int getMetalStockpile() const {
        return metal_stockpile;
    }

    void setMetalStockpile(int value) {
        if (value > limit) {
            value -= (value - limit);
        }
        metal_stockpile = value;
    }

    int getUraniumStockpile() const {
        return uranium_stockpile;
    }

    void setUraniumStockpile(int value) {
        if (value > limit) {
            value -= (value - limit);
        }
        uranium_stockpile = value;
    }

    int getNukes() const {
        return nukes;
    }

    void setNukes(int value) {
        nukes = value;
    }
    //incomes
    int getFoodIncome() const {
        int sum = 0;

        for (int i = 0; i < 5; i++)
        {
            sum += cities[i].getFarms()-(cities[i].getMines()+cities[i].getReactors());
        }

        return sum;
    }

    int getMetalIncome() const {
        int sum = 0;

        for (int i = 0; i < 5; i++)
        {
            sum += cities[i].getMines();
        }

        return sum;
    }

    int getUraniumIncome() const {
        int sum = 0;

        for (int i = 0; i < 5; i++)
        {
            sum += cities[i].getReactors();
        }

        return sum;
    }
    //other
    int getNumberOfCities() {
        int sum = 0;
        for (int i = 0; i < 5; i++) {
            if(cities[i].getIsDestroyed()==false){
                sum++;
            }
        }
        return sum;
    }

	void showCity(WINDOW* currentWindow, int idCity, int idY, int idX, std::string nameC) {
        std::ifstream cityFile(".\\Resources\\City.txt");
        std::ifstream cityBuilt1File(".\\Resources\\City_Built_1.txt");
        std::ifstream cityBuilt2File(".\\Resources\\City_Built_2.txt");
        std::ifstream cityDestroyedFile(".\\Resources\\City_Destroyed.txt");

        if (cities[idCity].getIsDestroyed()) {
            printImageHere(currentWindow, cityDestroyedFile, idY, idX);
        }
        else if (cities[idCity].getIsUpgraded()) {
            printImageHere(currentWindow, cityBuilt1File, idY, idX);
            napms(20);
            printImageHere(currentWindow, cityBuilt2File, idY, idX);
            napms(20);
        }
        else {
            printImageHere(currentWindow, cityFile, idY, idX);
        }
        mvwprintw(currentWindow, idY, idX+2, nameC.c_str());
        //if (1) { mvwprintw(currentWindow, idY+1, idX+2, "Upgrading %d/5", 1); }
        cityFile.close();
        cityBuilt1File.close();
        cityBuilt2File.close();
        cityDestroyedFile.close();
	}
	void showUpgradingCity() {


	}
};