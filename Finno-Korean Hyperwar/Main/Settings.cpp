#include <fstream>
#include <iostream>
#include <string>
#include <curses.h>
#include "Country.h"
#include "MainFunctions.h"

class Settings {
private:
    char keyUp = 'w';
    char keyDown = 's';
    char keyLeft = 'a';
    char keyRight = 'd';
    char keyConfirm = 'z';
    std::string chosenCountry = "Finland";
    int speed = 1;
    int difficulty = 2;

    std::string settingsFilename = ".\\Resources\\Settings.txt";
public:
 //Constructor
    Settings() {
        loadFromFile();
    }
    // Load function
    void loadFromFile() {
        std::ifstream file(settingsFilename);
        if (file.is_open()) {
            file >> keyUp >> keyDown >> keyLeft >> keyRight >> keyConfirm >> chosenCountry >> speed >> difficulty;
            file.close();
        }
        else {
            std::cerr << "B³¹d odczytu pliku: " << settingsFilename << std::endl;
        }
    }

    // Write function
    void saveToFile() {
        std::ofstream file(settingsFilename);
        if (file.is_open()) {
            file << keyUp << " " << keyDown << " " << keyLeft << " " << keyRight << " " << keyConfirm << " " << chosenCountry << " " << speed << " " << difficulty;
            file.close();
        }
        else {
            std::cerr << "B³¹d zapisu do pliku: " << settingsFilename << std::endl;
        }
    }
public:
    // Getter
    char getKeyUp() const {
        return keyUp;
    }

    void setKeyUp(char key) {
        keyUp = key;
    }

    // Getter
    char getKeyDown() const {
        return keyDown;
    }

    void setKeyDown(char key) {
        keyDown = key;
    }
    // Getter
    char getKeyLeft() const {
        return keyLeft;
    }

    void setKeyLeft(char key) {
        keyLeft = key;
    }

    // Getter
    char getKeyRight() const {
        return keyRight;
    }

    void setKeyRight(char key) {
        keyRight = key;
    }
    // Getter
    char getKeyConfirm() const {
        return keyConfirm;
    }

    void setKeyConfirm(char key) {
        keyConfirm = key;
    }

    //Country selection

    std::string getChosenCountry() const {
        return chosenCountry;
    }

    void setChosenCountry(const std::string& country) {
        chosenCountry = country;
    }

    //Speed
    int getSpeed() const {
        return speed;
    }

    void setSpeed(int newSpeed) {
        if (newSpeed < 1 || newSpeed > 5) {
            speed = 5;
        }
        else {
            speed = newSpeed;
        }
    }

    //Difficulty
    int getDifficulty() const {
        return difficulty;
    }

    void setDifficulty(int newDifficulty) {
        if (newDifficulty < 1 || newDifficulty > 3) {
            difficulty = 1;
        }
        else {
            difficulty = newDifficulty;
        }
    }

};