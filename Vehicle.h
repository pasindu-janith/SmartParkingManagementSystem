#include <ctime>
#include <iostream>
#include <string>

using namespace std;

class Vehicle {
public:
	string vehicleno;
	int category;
	long enter_time;

	Vehicle();
	Vehicle(string key, short cat, long etime);
};