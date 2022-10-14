// KC Hotel Project
// Jeff Orcutt
// 2022 09 26

// UMKC 
// CS 201R Project 03

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include "functions.h"			

vector<Room> roomList;			//this will contain pricing and total inventory county of room types
vector<Booking> bookingList;	//this will contain each booking 
vector<Hotel> hotelRooms;		//contains all available rooms
vector<string> errorVector;		//contains information about errors found

void readFile() {
	ifstream csvFile;								// create a input filestream
	csvFile.open("CS201PGM3Hotel.csv", ios::in);	// open the csv

	vector<string> row;								// use a vector to load each record into
	string line, word;								// line is the raw line, word is each part of the record 

	if (!csvFile.good()) {							// check that file is open and not in an error state
		perror(("error while opening file"));		
		return;
	}
	while (getline(csvFile, line)) {				// get a record while not EOF
		row.clear();								// clear out row's contents
		stringstream str(line);						// use stringstream to convert line to a string stream
		int i = 0;
		while (getline(str, word, ',')) {			// iterate through the stringstream, using the comma delimeter
			row.push_back(word);					// to load them into separate locations within the word vector
			++i;
		}

		if (row.at(0) == "R") {						// checks if the record being read is a "room" type record
			Room temp;								// set up a temp struct to hold the values
			temp.roomType = row.at(1);
			try {
				temp.roomCost = stod(row.at(2));	// tries and catches if price is set to a non-numeric value
			}
			catch (...) {
				string write;
				write.append(row.at(1));
				write.append(" is not a valid price, error in room record.");	
				errorVector.push_back(write);
			}

			roomList.push_back(temp);				// write the temp struct out to the roomList
		}

		if (row.at(0) == "H") {						// check if the record is a hotel inventory record
			Hotel temp;								// create a Hotel struct to hold values
			try {									// try and catch errors if a non-numeric room number is 
				temp.roomNum = stoi(row.at(1));		// found 

			}
			catch (...) {
				string write;
				write.append(row.at(1));
				write.append(" is not a valid room number, error located in hotel room list.");
				errorVector.push_back(write);
			}
			
			temp.roomType = row.at(2);				// capture room's type
			temp.roomAvail = true;					// every room is initially available
			hotelRooms.push_back(temp);				// add temp struct to hotelRoom vector

													// below increments the amount of rooms of roomType available
													// in the roomsList
			for (int i = 0; i < roomList.size(); i++) {
				if (roomList.at(i).roomType.compare(0, temp.roomType.length(), temp.roomType) == 0) {
					roomList.at(i).numAvail += 1;
				}
			}

		}
		if (row.at(0) == "B") {						// check to see if the record is a booking
			Booking temp;							// create a booking struct to hold values

			try {									// checks to see if a valid reservation is present
				temp.resNumber = stoi(row.at(1));	// invalid res number leads to a non-usable record
													// catches the error and appends to error vector, no
			}										// reservation to fill, so program continues with next 
			catch (...) {							// record
				string write;
				write.append(row.at(1));
				write.append(" is not a valid reservation number, error located in record with last name: ");
				write.append(row.at(3));
				errorVector.push_back(write);
				continue;
			}
			temp.firstName = row.at(2);
			temp.lastName = row.at(3);
			temp.roomType = row.at(4);


			try {									// checks for a numeric number of nights in the record
				temp.numNights = stoi(row.at(5));	// if non-numeric/non-integer, will treat as invalid record
			}										// and no reservation is registered, writes to error vector
			catch (...) {							// continues to next record
				string write;
				write.append(row.at(5));
				write.append(" is not a valid number of nights. Error located in booking: ");
				write.append(row.at(1));
				errorVector.push_back(write);
				continue;
			}

			bookingList.push_back(temp);			// write the temp struct to the bookingList
		}
	}

	csvFile.close();								// close the file
	return;
}


string checkRoomType(string check) {				// compares the room type passed to function to all the 
	for (int i = 0; i < roomList.size();  i++) {	// values of room types contained in roomList. If valid, 
		int strlen = check.length();				// return value, if invalid, adds ** to value and return;
		if (check.compare(0, strlen, roomList.at(i).roomType) == 0) {
			return check;
		}
	}

	return check.append("**");
}


/* this commented out block would be used if we had a complete list of rooms
	in the csv to load in. 
int checkRoomNum(int check) {			
	for (int i = 0; i < hotelRooms.size(); i++) {
		cout << check << " " << hotelRooms.at(i).roomNum << endl;
		if (check == hotelRooms.at(i).roomNum) return check;
	}
	return 0;
}
*/

void printHotelSheet() {

	vector<int> inventoryTotal;								// wanted a total count of room by types

	string title = "KANSAS CITY INTERNATIONAL HOTEL";
	cout << setw(12) << " " << title << endl;


	for (int i = 0; i < roomList.size(); i++) {				// load the total count of rooms by type into vector
		inventoryTotal.push_back(roomList.at(i).numAvail);
	}

	for (int i = 0; i < bookingList.size(); i++) {

		bookingList.at(i).roomNum = findRoom(bookingList.at(i).roomType);	// checks for an available room of roomType
		if (bookingList.at(i).roomNum == 0) {				// if roomNum returned = 0, means none of roomType are 
			string tempStr;									// found, create an error message and push to errorVector
			tempStr.append("Reservation number ");
			tempStr.append(to_string(bookingList.at(i).resNumber));
			tempStr.append(" has not been assigned a room due to no availability of type: ");
			tempStr.append(bookingList.at(i).roomType);
			tempStr.append(".");
			errorVector.push_back(tempStr);
		}
															// find the per night cost of the room of roomType
		double tempCost = findPrice(bookingList.at(i).roomType, bookingList.at(i).roomNum);
															// validates the roomType requested
		string tempType = checkRoomType(bookingList.at(i).roomType);
															// print the booking record
		cout << setw(8) << bookingList.at(i).resNumber << setw(20) << bookingList.at(i).lastName
			<< setw(10) << bookingList.at(i).firstName << setw(10) << tempType 
			<< setw(10) << bookingList.at(i).roomNum << setw(10) << fixed << setprecision(2) 
			<< tempCost * bookingList.at(i).numNights << endl;

	}
	cout << endl << endl;
	cout << "Remaining room inventory: " << endl;			// wanted to print an inventory list of rooms by type
	for (int i = 0; i < roomList.size(); i++) {
		cout << setw(9) << roomList.at(i).roomType << ": " << setw(2)<<  roomList.at(i).numAvail << " / " << inventoryTotal.at(i);
	}
	return;
}

void printErrorVector() {									// iterate through the error vector printing the errors
	cout << endl << endl << endl;
	cout << "The following records have errors in them: " << endl;
	for (int i = 0; i < errorVector.size(); i++) {
		cout << errorVector.at(i) << endl;
	}
}

int findRoom(string roomType) {								// iterates through the vector of hotel rooms, looking
															// for an available room of roomType, returns the room 
	for (int i = 0; i < hotelRooms.size(); i++) {			// number if found, 0 if not found or none available
		int strlen = roomType.length();

		if ((roomType.compare(0,strlen, hotelRooms.at(i).roomType)) == 0) {
			if (hotelRooms.at(i).roomAvail == true) {		// set the room available to false
				hotelRooms.at(i).roomAvail = false;

				for (int j = 0; j < roomList.size(); j++) {	// find the appropriate roomType in the roomList and 
															// decrement the number of available rooms
					if ((roomType.compare(0, strlen, roomList.at(j).roomType)) == 0) {
						roomList.at(j).numAvail -= 1;
					}
				}
				return hotelRooms.at(i).roomNum;
			}
			continue;
		}
	}

	return 0;
}

double findPrice(string roomType, int roomNum) {
	if (roomNum == 0) return 0;							//if roomNum passed to function is 0, means price is 0
	for (int i = 0; i < roomList.size(); i++) {			// find and return the per night cost of the room based on 
														// the passed roomType value
		if (roomType.compare(roomList.at(i).roomType) == 0) {
			return roomList.at(i).roomCost;
		}
	}
	return 0;
}

