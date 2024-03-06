#include <iostream>
#include <string>
#include "Vehicle.h"

using namespace std;

class HashedTable {
private:
	static const int slotsNumber = 25; //Number of all slots in car park
	Vehicle* table[slotsNumber]; //Hashtable array
	int current_size = 0; //Current count of vehicles inside the car park

public:
	HashedTable();
	int getHash(string key);
	void vehicleIncome(string key, short cat);
	void vehicleIncome(string key, short cat, long t);
	void vehicleExit(string key);
	int search(string key);
	void print();
	void priceCal(int index);
	int getCurrentSize();
	int getSlotsNumber();
	void getSlotData(int slot);
	bool checkSpaceAvailability();
	void getSortedList();
	void writeToFile();
};
