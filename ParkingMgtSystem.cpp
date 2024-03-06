/*
* Smart Vehicle Parking Management System
* Group Members
* EG/2021/4545 Hathurusinghe H.A.P.J
* EG/2021/4451 Chandrarathna H.N.M.D
*/
#include <iostream>
#include <ctime>
#include <fstream>

#include "HashedTable.h"

using namespace std;

const static string vCategories[] = { "Bicycle","Tuk-Tuk","Car","Van","Lorry" };//Vehicle types array
const static double price_array[] = { 50.00, 100.00, 150.00, 200.00,250.00 };//Price for one hour parked for
																				//each vehicle types

string getFormattedTime(long t);

/*
*Vehicle class implementation start
*/
Vehicle::Vehicle() {
	vehicleno = "";
	category = -1;
	enter_time = 0;
}

Vehicle::Vehicle(string key, short cat, long etime) {
	vehicleno = key;
	category = cat;
	enter_time = etime;
}
/*
*Vehicle class implementation end
*/


/*
*Hashedtable class implementation start
*/
HashedTable::HashedTable() {
	for (int i = 0; i < slotsNumber; i++) {
		table[i] = NULL;
	}
}

int HashedTable::getCurrentSize() {
	return current_size;
}

int HashedTable::getSlotsNumber() {
	return slotsNumber;
}

//Hash function
int HashedTable::getHash(string key)
{
	int sum = 0;
	for (int i = 0; i < key.length(); i++) {
		sum = sum + key[i] - '0';
	}
	return sum % slotsNumber;
}

//For entering vehicle without set time, use current time of system
void HashedTable::vehicleIncome(string key, short cat)
{
	int index = getHash(key);

	time_t now = time(0);
	if (slotsNumber > current_size) {
		while (table[index] != NULL)
		{
			index++;
			index %= slotsNumber;
		}
		table[index] = new Vehicle(key, cat, now);
		cout << "Welcome " << key << ",  Your slot is " << index << endl;
		current_size++;
	}
	else {
		cout << "Park Full..." << endl;
	}
}

//For enter vehicle with entering time setting
void HashedTable::vehicleIncome(string key, short cat, long t)
{
	int index = getHash(key);
	if (slotsNumber > current_size) {
		while (table[index] != NULL)
		{
			index++;
			index %= slotsNumber;
		}
		table[index] = new Vehicle(key, cat, t);
		cout << "Welcome " << key << ", Your slot is " << index << endl;
		current_size++;
	}
	else {
		cout << endl;
		cout << "Park Full..." << endl;
	}
}

//Vehicle Exit from the park
void HashedTable::vehicleExit(string key)
{
	int index = search(key);
	if (index != -1) {
		cout << endl;
		cout << "Vehicle: " << key << " " << vCategories[table[index]->category]<<endl;
		priceCal(index);
		delete table[index];
		table[index] = NULL;
		current_size--;
		cout << key << " Vehicle exit..." << endl << endl;
	}
	else {
		cout << key << " No vehicle from this number..." << endl << endl;
	}
}

//Search a slot of a vehicle parked
int HashedTable::search(string key)
{
	int index = getHash(key);

	for (int i = 0; i < slotsNumber; i++) {
		if (table[index] != NULL && table[index]->vehicleno == key)
		{
			return index;
		}
		else
		{
			index++;
			index %= slotsNumber;
		}
	}
	return -1;
}

//Print all vehicles in park
void HashedTable::print()
{
	for (int i = 0; i < slotsNumber; i++) {
		if (table[i] != NULL) {
			string datetime = getFormattedTime(table[i]->enter_time);

			cout << i << "\t" << table[i]->vehicleno << "\t" << vCategories[table[i]->category] <<
				"\t" << datetime << endl;
		}
	}
}

//Print a vehicle data in a specific slot in park
void HashedTable::getSlotData(int slot) {
	if (slot > -1 && slot < slotsNumber) {
		if (table[slot] != NULL) {
			cout << table[slot]->vehicleno << "\t" << vCategories[table[slot]->category] << "\t"
				<< getFormattedTime(table[slot]->enter_time) << endl;
		}
		else {
			cout << "Slot is empty..." << endl;
		}
	}
	else {
		cout << "Enter valid slot" << endl;
	}
}

bool HashedTable::checkSpaceAvailability()
{
	if (slotsNumber > current_size)
		return true;
	else
		return false;
}

//Get a sorted list of vehicles according to the income time ascending order
void HashedTable::getSortedList()
{
	Vehicle* v = new Vehicle[current_size];
	int index = 0;
	for (int i = 0; i < slotsNumber; i++) {
		if (table[i] != NULL) {
			v[index].vehicleno = table[i]->vehicleno;
			v[index].enter_time = table[i]->enter_time;
			v[index].category = table[i]->category;
			index++;
		}
	}
	//Bubble sort algorithm
	for (int i = 0; i < current_size - 1; i++) {
		for (int j = 0; j < current_size - 1; j++) {
			if (v[j].enter_time > v[j + 1].enter_time) {
				Vehicle v1 = v[j];
				v[j] = v[j + 1];
				v[j + 1] = v1;
			}
		}
	}

	for (int i = 0; i < current_size; i++) {
		cout << v[i].vehicleno << "\t" << vCategories[v[i].category] << "\t" << getFormattedTime(v[i].enter_time) << endl;
	}
}

//Write all vehicle details inside park to a txt file. File will be created in project directory
void HashedTable::writeToFile()
{
	ofstream file1;
	file1.open(to_string(time(0))+".txt", ios::app);
	for (int i = 0; i < slotsNumber; i++) {
		if (table[i] != NULL) {
			file1 << i << "\t";
			file1 << table[i]->vehicleno << "\t";
			file1 << vCategories[table[i]->category] << "\t";
			file1 << getFormattedTime(table[i]->enter_time) << endl;
		}
	}
	file1.close();
	cout << "File Saved..." << endl;
}

/*Calculate price of ticket according to the parked hours and vehicle category. Function use inside
vehicleExit function*/
void HashedTable::priceCal(int index) {
	long income_time = table[index]->enter_time;
	time_t now = time(0);
	short spend_hours = (now - income_time) / 3600;
	if (spend_hours == 0) {
		spend_hours = 1;
	}
	cout << "Entered time: " << getFormattedTime(income_time) << endl;
	cout << "Spended time: " << spend_hours << " hrs" << endl;
	cout << "Charge: Rs." << price_array[table[index]->category] * spend_hours << endl;
}

//Get Formatted time string by using given seconds
string getFormattedTime(long t) {
	time_t past = time_t(t);

	tm* ltm = localtime(&past);

	string enter_date = to_string(1900 + ltm->tm_year) + "."
		+ to_string(1 + ltm->tm_mon) + "." + to_string(ltm->tm_mday);
	string enter_time = to_string(ltm->tm_hour) + ":" + to_string(ltm->tm_min)
		+ ":" + to_string(ltm->tm_sec);
	return enter_date + " " + enter_time;
}

int main()
{
	HashedTable ht;
	string input = "";

	//Already added some vehicle objects to hashed table for testing purpose
	//Objects with setted time
	ht.vehicleIncome("KO5115", 2, 1701730902);
	ht.vehicleIncome("KO5835", 1, 1701720902);
	ht.vehicleIncome("KA2335", 2, 1701700902);
	ht.vehicleIncome("KQ3235", 2, 1701715400);
	ht.vehicleIncome("KO5325", 2, 1701725902);
	ht.vehicleIncome("CBA5683", 3);//Now entered
	//Objects without set time, use current time
	/*
	ht.vehicleIncome("CC4553", 3);
	ht.vehicleIncome("KI5555", 0);
	ht.vehicleIncome("KL1234", 3);
	ht.vehicleIncome("CAL4355", 2);
	ht.vehicleIncome("CBC4565", 2);
	ht.vehicleIncome("XS3347", 0);
	ht.vehicleIncome("AN8889", 0);
	ht.vehicleIncome("KV4333", 2);
	ht.vehicleIncome("LM3233", 3);
	ht.vehicleIncome("JB4433", 4);
	ht.vehicleIncome("BF4577", 0);
	ht.vehicleIncome("AC3455", 1);
	ht.vehicleIncome("KA6665", 2);
	ht.vehicleIncome("CC4325", 2);
	ht.vehicleIncome("CC4555", 2);
	ht.vehicleIncome("AAJ5338", 1);
	ht.vehicleIncome("LM5544", 0);
	ht.vehicleIncome("CR4478", 2);
	*/

	while (input != "exit") {
		cin.clear();
		cout << "====================================================================================" << endl;
		cout << "i - Vehicle Enter\te - Vehicle Exit\t\tp - Print Vehicle List" << endl;
		cout << "m - What is the slot\tz - What is the vehicle\t\to - Park Status" << endl;
		cout << "y - Get entering time sorted list\t\tc - Save to file\texit - Quit" << endl;
		cout << "====================================================================================" << endl;

		cout << "Input: ";
		cin >> input;
		if (input == "i" || input == "I") {
			string vno;
			int category;
			if (ht.checkSpaceAvailability()) {
				cout << "Enter Vehicle No.: ";
				cin >> vno;
				cout << "Enter Vehicle Category: " << endl;
				cout << "0-Bicycle | 1-Tuk-Tuk | 2-Car | 3-Van | 4- Lorry" << endl;
				cin >> category;
				ht.vehicleIncome(vno, category);
			}
			else {
				cout << "Park Full..."<<endl;
			}
		}
		else if (input == "e" || input == "E") {
			string vno;
			cout << "Enter Vehicle No.: ";
			cin >> vno;
			ht.vehicleExit(vno);
		}
		else if (input == "p") {
			ht.print();
		}
		else if (input == "m") {
			string vno;
			cout << "Enter Vehicle No.: ";
			cin >> vno;
			int index = ht.search(vno);
			if (index != -1) {
				cout << "Slot is " << index << endl;
			}
			else {
				cout << "No such vehicle from that number..." << endl;
			}
		}
		else if (input == "o" || input == "0") {
			cout << "Number of slots in the park: " << ht.getSlotsNumber() << endl;
			cout << "Number of vehicles parked: " << ht.getCurrentSize() << endl;
			cout << "Available number of slots: " << ht.getSlotsNumber() - ht.getCurrentSize() << endl;
		}
		else if (input == "z" || input == "Z") {
			int slot;
			cout << "Enter Slot No.: ";
			cin >> slot;
			ht.getSlotData(slot);
		}
		else if (input == "y" || input == "Y") {
			ht.getSortedList();
		}
		else if (input == "c" || input == "C") {
			ht.writeToFile();
		}
	}
	cout << endl;
}