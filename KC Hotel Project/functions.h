#pragma once
#include <string>

using namespace std;

// Customer Booking Data Struct
struct Booking {			// loads into vector bookingList
	int resNumber;			// reservation number
	string firstName;
	string lastName;
	string roomType;		// reservation roomType
	int numNights = 0;		// number of nights for reservation
	int roomNum;			// room number assigned

};

// Hotel Room Inventory 
struct Hotel {				// loads into vector hotelRooms
	int roomNum;			// room number 
	string roomType;		// room type
	bool roomAvail = true;	// available true/false
};

// Room Pricing and Inventory counter
struct Room {				// loads into vector roomList
	string roomType;
	double roomCost;
	int numAvail = 0;
};

void readFile();
// This function loads the data file into vectors

void printHotelSheet();
// This is the main hotel sheet print function

void printErrorVector();
// This function prints the error vector

string checkRoomType(string check);
// This function checks whether the room type is a valid
// room type in the hotel, returns either the valid string
// or adds ** as an indicator that the room type is wrong

// int checkRoomNum(int check);
// This function hasn't fully been implemented yet, but 
// would check the room number for a valid room number.

int findRoom(string roomType);
// This function looks for an availalbe room of roomType, 
// returns the room number, decrements available rooms, 
// and sets the room to be unavailble. If no room of roomType
// is available, will return 0

double findPrice(string roomType, int roomNum);
// This function finds the price of a room of type roomType, if 
// roomNum is not zero. If roomNum is zero, returns 0 for price.