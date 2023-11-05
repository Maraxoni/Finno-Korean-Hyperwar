#include "City.h"
class City {
public:
	City() = default;
private:
	bool is_destroyed = false;
	bool is_upgraded = false;
	int farms = 2;
	int mines = 1;
	int reactors = 1;
public:
    bool getIsDestroyed() const {
        return is_destroyed;
    }

    void setIsDestroyed(bool value) {
        is_destroyed = value;
    }

    bool getIsUpgraded() const {
        return is_upgraded;
    }

    void setIsUpgraded(bool value) {
        is_upgraded = value;
    }

    int getFarms() const {
        return farms;
    }

    void setFarms(int value) {
        farms = value;
    }

    int getMines() const {
        return mines;
    }

    void setMines(int value) {
        mines = value;
    }

    int getReactors() const {
        return reactors;
    }

    void setReactors(int value) {
        reactors = value;
    }
};

