//Header Files

#include<iostream>
#include<ctime>
#include<string>
#include<fstream>
using namespace std;

//Constants and Global Variables
const int SIZE = 300;

//Enumerations for Bed and Payment Types
enum Beds {
	single = 3,
	queen = 2,
	king = 1
};
enum PaymentMethod {
	Cash = 1,
	Visa = 2
};

//Room Structure  (Describes a hotel room and its booking info)

struct Room {
	int RoomNo;     // Room Number (fixed unless deleted)

	int NoOfBeds;   // Number of beds in the room

	Beds BedType;   // Type of beds

	int BookingID;  // Unique booking ID (-1 if available)

	string StartDate; // Start date of stay

	string EndDate;   // End date of stay 

	double PricePerNight; // Room price per night

	string CustomerName; // Customer Name for BookingID

	bool CheckedIn; //customer checked in? (0/1)

	bool Paid; //did customer pay bill for room?


};

//function prototypes

void BuildHotel(Room rooms[], const int maxRooms);					//Build the hotel rooms
void BookRoom(Room rooms[], const int maxRooms);    // Book a room    
void DeleteRoom(Room rooms[], const int maxRooms);  // Delete a room   
void AddRoom(Room rooms[], const int maxRooms, int& Roomcount);     // Add a new room
void SavetoFile(Room rooms[], const int maxRooms, const string& filename);         // Save room data to a file 
void CalculateBill(Room rooms[], int const maxRooms);          // Calculate bill for a booked room 
int CalculateDaysStayed(string Start, string End);                  // Calculate total days stayed     
string intToBedType(int value);
//Date and String Validation

bool DateIsValid(string Date);                           // Validate date format 
bool IsNameValid(string name);                           // This function checks if the name contains only letters and spaces. 
bool EndDateValid(string Start, string End);             // Validate that end date is after start date 
bool IsDateWithinBooking(string Current, string Start, string End); // Return true if currentDate >= startDate and currentDate <= endDate
bool CompareWcurrentDate(string Current, string Date);

// Check-in/out processes

void CheckIn(Room rooms[], const int maxRooms);              // Process check-in for a booking
void CheckOut(Room rooms[], const int maxRooms);             // Process check-out for a booking
string CurrentDate;

int main()
{
	ofstream file("HotelManagement.csv");
	Room rooms[SIZE];
	int RoomCount = 300;//Keeps track of how many rooms have been added

	BuildHotel(rooms, RoomCount);

	SavetoFile(rooms, RoomCount, "HotelManagement.csv");

	//Ask for current date 
	cout << "Please enter the current date in the following format:" << endl << "dd/mm/yyyy:  ";
	cin >> CurrentDate;
	while (!DateIsValid(CurrentDate)) {
		cout << "Invalid Date Format! Please try again : ";
		cin >> CurrentDate;
	}

	int choice;
	do {
		cout << "\n\033[35mWelcome to Hotel Management System!\n";  // Magenta //ANSI code
		cout << "\033[36mPlease select an option: \n";  // Cyan
		cout << "\033[32m1 : Book Room\n";  
		cout << "2 : Add Room\n";  
		cout << "3 : Delete Room\n";  
		cout << "4 : Save Inventory to File\n";  
		cout << "5 : Check in\n";  
		cout << "6 : Check out\n";  
		cout << "7 : Calculate Bill\n";  
		cout << "8 : Exit\n";  
		cout << "\033[35mEnter your choice: \033[0m";  
		cin >> choice;



		switch (choice) {
		case 1:
			BookRoom(rooms, SIZE);
			break;
		case 2:
			AddRoom(rooms, SIZE, RoomCount);
			break;
		case 3:
			DeleteRoom(rooms, SIZE);
			break;
		case 4:
			SavetoFile(rooms, SIZE, "HotelManagement.csv");
			break;
		case 5:
			CheckIn(rooms, SIZE);
			break;
		case 6:
			CheckOut(rooms, SIZE);
			break;
		case 7:
			CalculateBill(rooms, SIZE);
			break;
		case 8:
			cout << "Program terminated";
			return 0;
		default:
			cout << "Invalid Choice! Please try again: ";
		}
	} while (choice != 8);
}
string intToBedType(int value) {
	switch (value) {
	case 3: return "Single";
	case 2: return "Queen";
	case 1: return "King";
	default: return "Unknown";
	}
}

// Date and String Validations

bool DateIsValid(string Date) {
	if (Date.length() != 10 || Date[2] != '/' || Date[5] != '/') return false;

	int day = stoi(Date.substr(0, 2));
	int month = stoi(Date.substr(3, 2));
	int year = stoi(Date.substr(6, 4));

	if (day < 1 || day > 31) return false;
	if (month < 1 || month > 12) return false;
	if (year < 2025 || year > 2026) return false;

	return true;
}

bool EndDateValid(string Start, string End) {
	// Extract day, month, year from Start
	int startday = stoi(Start.substr(0, 2));
	int startmonth = stoi(Start.substr(3, 2));
	int startyear = stoi(Start.substr(6, 4));

	// Extract day, month, year from End
	int endday = stoi(End.substr(0, 2));
	int endmonth = stoi(End.substr(3, 2));
	int endyear = stoi(End.substr(6, 4));


	if (endyear < startyear) return false;
	if (endyear == startyear && endmonth < startmonth) return false;
	if (endyear == startyear && endmonth == startmonth && endday <= startday) return false;
	return true;

}
bool CompareWcurrentDate(string Current, string Date) {
	// Extract day, month, year from Start
	int currentday = stoi(Current.substr(0, 2));
	int currentmonth = stoi(Current.substr(3, 2));
	int currentyear = stoi(Current.substr(6, 4));

	// Extract day, month, year from End
	int dateday = stoi(Date.substr(0, 2));
	int datemonth = stoi(Date.substr(3, 2));
	int dateyear = stoi(Date.substr(6, 4));
	if (dateyear < currentyear) return false;
	if (dateyear == currentyear && datemonth < currentmonth) return false;
	if (dateyear == currentyear && datemonth == currentmonth && dateday < currentday) return false;
	return true;

}

bool IsNameValid(string name) {
	// Name should not be empty
	if (name == "") {
		return false;
	}
	for (int i = 0; i < name.length(); i++) {
		// Check if each character is a letter or a space
		if (!isalpha(name[i]) && name[i] != ' ') {
			return false;
		}
	}
	return true;

}
void BuildHotel(Room rooms[], const int maxRooms) {
	// Room Initialization (assign types and prices)

	for (int i = 0; i < 100; i++) {
		rooms[i].RoomNo = i + 1;
		rooms[i].NoOfBeds = 3;
		rooms[i].BedType = single;
		rooms[i].BookingID = -1;
		rooms[i].PricePerNight = 400.0;
		rooms[i].CheckedIn = 0;
		rooms[i].Paid = 0;
	}


	for (int i = 100; i < 200; i++) {
		rooms[i].RoomNo = i + 1;
		rooms[i].NoOfBeds = 2;
		rooms[i].BedType = queen;
		rooms[i].BookingID = -1;
		rooms[i].PricePerNight = 500.0;
		rooms[i].CheckedIn = 0;
		rooms[i].Paid = 0;
	}

	for (int i = 200; i < 300; i++) {
		rooms[i].RoomNo = i + 1;
		rooms[i].NoOfBeds = 1;
		rooms[i].BedType = king;
		rooms[i].BookingID = -1;
		rooms[i].PricePerNight = 600.0;
		rooms[i].CheckedIn = 0;
		rooms[i].Paid = 0;
	}
}

int CalculateDaysStayed(string Start, string End) {
	// Extract day, month, year from Start
	int startday = stoi(Start.substr(0, 2));
	int startmonth = stoi(Start.substr(3, 2));
	int startyear = stoi(Start.substr(6, 4));

	// Extract day, month, year from End
	int endday = stoi(End.substr(0, 2));
	int endmonth = stoi(End.substr(3, 2));
	int endyear = stoi(End.substr(6, 4));

	// Approximate day calculation: year * 365 + month * 30 + day
	int startTotal = (startyear * 365) + (startmonth * 30) + startday;
	int endTotal = (endyear * 365) + (endmonth * 30) + endday;

	int total = (endTotal - startTotal);
	return total;


}

bool IsDateWithinBooking(string Current, string Start, string End) {
	// Extract day, month, year from Start
	int startday = stoi(Start.substr(0, 2));
	int startmonth = stoi(Start.substr(3, 2));
	int startyear = stoi(Start.substr(6, 4));

	// Extract day, month, year from End
	int endday = stoi(End.substr(0, 2));
	int endmonth = stoi(End.substr(3, 2));
	int endyear = stoi(End.substr(6, 4));

	// Extract day, month, year from Current
	int currentday = stoi(Current.substr(0, 2));
	int currentmonth = stoi(Current.substr(3, 2));
	int currentyear = stoi(Current.substr(6, 4));

	// Compare current date with start date
	if (
		(currentyear < startyear) ||
		(currentyear == startyear && currentmonth < startmonth) ||
		(currentyear == startyear && currentmonth == startmonth && currentday < startday)
		) {
		return false;
	}

	// Compare current date with end date
	if (
		(currentyear > endyear) ||
		(currentyear == endyear && currentmonth > endmonth) ||
		(currentyear == endyear && currentmonth == endmonth && currentday > endday)
		) {
		return false;
	}

	return true;
}

void AddRoom(Room rooms[], const int maxRooms, int& Roomcount) {

	Room newRoom;
	Roomcount++;
	// Get room details from the user

	newRoom.RoomNo = Roomcount;
	cout << "\033[32mThe room you are building now is room number: \033[32m" << Roomcount << endl;
	cout << "Enter Number of Beds(maximum 3): ";

	cin >> newRoom.NoOfBeds;


	int bedTypeInput;
	cout << "Enter Bed Type (3 = Single, 2 = Queen, 1 = King): ";
	cin >> bedTypeInput;
	newRoom.BedType = static_cast<Beds>(bedTypeInput);

	cout << "Enter Price per Night: ";
	cin >> newRoom.PricePerNight;

	newRoom.BookingID = -1; // Room starts as not booked

	// Add room to the array
	rooms[Roomcount] = newRoom;

	// Append the new room details to the CSV file
	ofstream file("HotelManagement.csv", ios::app);
	if (file.is_open()) {
		file << newRoom.RoomNo << ","
			<< intToBedType(newRoom.BedType) << ","
			<< newRoom.NoOfBeds << ","
			<< "$" << newRoom.PricePerNight << ","
			<< newRoom.BookingID << endl;

		file.close();
		cout << "Room added successfully and saved to file." << endl;
	}
	else {
		cout << "Error opening file for appending." << endl;
	}
}

void BookRoom(Room rooms[], const int maxRooms) {
	bool available = 0;

	for (int i = 0; i < maxRooms; i++)
	{
		if (rooms[i].BookingID == -1) { available = 1; break; }
		else { available = 0; }
	}

	if (available == 0) {
		cout << "All rooms are booked!!";
		return;
	}
	string BedChoice;
	char current;
	string start, end;
	bool validDate = 1;
	bool validBed = 0;
	int confirm;
	string name;
	Beds bedtype;
	int bedNo=0;
	// Let user choose bed type
	do {
		cout << "Please Enter type of bed" << endl << "";
		cout << "1. 3 Single Beds\n2. 2 Queen Beds\n3. 1 King Bed\n";
		cin >> BedChoice;
		current = BedChoice[0];


		switch (current) {

		case '1':
			bedtype = single;
			bedNo = 3;
			break;
		case '2':
			bedtype = queen;
			bedNo = 2;
			break;
		case '3':
			bedtype = king;
			bedNo = 1;
			break;

		default:
			cout << "Invalid choice!";

		}
		if (current != '1' && current != '2' && current != '3') {
			cout << "Invalid format!" << endl;
			validBed = 0;

		}
		else {
			validBed = 1;
		}
	} while (validBed == 0);

	// Input and validate dates
	do {
		cout << "Please Enter start stay date(Format: DD/MM/YYYY): " << endl;
		cin >> start;

		if (!DateIsValid(start) || !CompareWcurrentDate(CurrentDate, start)) {
			cout << "Invalid format!" << endl;
			validDate = 0;

		}
		else {
			validDate = 1;
		}

	} while (validDate == 0);


	do {
		cout << "Please Enter end date(Format: DD/MM/YYYY): " << endl;
		cin >> end;

		if (!DateIsValid(end) || !EndDateValid(start, end)) {
			cout << "Invalid format!" << endl;
			validDate = 0;

		}
		else {
			validDate = 1;
		}

	} while (!validDate);

	// Find available room that matches bed type
	int roomIndex = -1;
	for (int i = 0; i < maxRooms; i++) {
		if ((rooms[i].BookingID == -1) && (rooms[i].BedType == bedtype) && (rooms[i].NoOfBeds == bedNo)) {
			roomIndex = i;
			break;
		}
	}

	if (roomIndex == -1) {
		cout << "No available rooms with selected bed type.\n";
		return;
	}

	// Ask for the user's name and validate it
	do {
		cout << "Enter your name: ";
		cin.ignore();
		getline(cin, name);

		if (!IsNameValid(name)) {
			cout << "Invalid name! Please only use letters and spaces." << endl;
		}

	} while (!IsNameValid(name));


	// Confirm booking
	cout << "Available room found: " << endl << "Room number: " << rooms[roomIndex].RoomNo << endl;;
	cout << "From: " << start << " To: " << end << endl;
	cout << "Confirm booking? (1 for Yes / 2 for No): " << endl;
	cin >> confirm;

	if (confirm == 1) {
		cout << "Room Booked Successfully!" << endl;
		rooms[roomIndex].EndDate = end;
		rooms[roomIndex].StartDate = start;
		rooms[roomIndex].CustomerName = name;

		//Generating BookinID
		int nameTotal = 0;
		for (int i = 0; i < name.length(); i++) {
			nameTotal += (int)name[i];
		}

		rooms[roomIndex].BookingID = nameTotal + (rooms[roomIndex].RoomNo * 10);
	}

	else {
		cout << "Room deleted successfully " << endl;
	}
	SavetoFile(rooms, maxRooms, "HotelManagement.csv");
}

void DeleteRoom(Room rooms[], const int maxRooms) {
	int RoomNum;
	string name;
	bool found = false;


	// Ask user to enter the room number for cancellation
	cout << "Please enter room number";
	cin >> RoomNum;

	// Ask for the user's name and validate it
	do {
		cout << "Enter your name: ";
		cin.ignore();
		getline(cin, name);

		if (!IsNameValid(name)) {
			cout << "Invalid name! Please only use letters and spaces." << endl;
		}

	} while (!IsNameValid(name));

	// Loop through all rooms to find the matching room number and customer name
	for (int i = 0; i < maxRooms; i++) {
		if (RoomNum == rooms[i].RoomNo && name == rooms[i].CustomerName) {
			if (rooms[i].BookingID != -1) {
				rooms[i].EndDate = "";
				rooms[i].StartDate = "";
				rooms[i].CustomerName = "";
				rooms[i].BookingID = -1;
				cout << "Booking for Room Number: " << RoomNum << " has been cancelled! " << endl;

			}
			else {
				cout << "Room Number: " << RoomNum << " is not booked yet!" << endl;
				break;
			}
			found = true;
		}
	}

	if (!found) {
		cout << "Room number " << RoomNum << " is not found in the hotel." << endl;
	}

	SavetoFile(rooms, maxRooms, "HotelManagement.csv");


}

void SavetoFile(Room rooms[], const int maxRooms, const string& filename) {
	ofstream outfile(filename);

	if (!outfile) {
		cout << "File not found";
	}
	outfile << "Room No." << ","
		<< "Bed Type" << ","
		<< "No. of Beds" << ","
		<< "Price / Night" << ","
		<< "Booking ID" << ","
		<< "Customer Name" << ","
		<< "CheckedIn?" << ","
		<< "Start Date" << ","
		<< "End Date" << endl;
	for (int i = 0; i < maxRooms; i++) {
		outfile << rooms[i].RoomNo << ","  // Room Number
			<< intToBedType(rooms[i].BedType) << ","  // Bed Type
			<< rooms[i].NoOfBeds << ","  // Number of Beds
			<< "$" << rooms[i].PricePerNight << ","  // Price per Night
			<< rooms[i].BookingID << ","  // Booking ID
			<< rooms[i].CustomerName << ","  // Customer Name
			<< rooms[i].CheckedIn << ","  // Checked-in Status
			<< rooms[i].StartDate << ","  // Start Date
			<< rooms[i].EndDate << endl;  // End Date

	}
	outfile.close();
	cout << "Rooms saved to file successfully." << endl;
}


void CalculateBill(Room rooms[], int const maxRooms) {

	string name;
	double Total = 0;
	bool found = false;
	bool paid = false;

	// Ask for the user's name and validate it
	do {
		cout << "Enter your name: ";
		cin.ignore();
		getline(cin, name);

		if (!IsNameValid(name)) {
			cout << "Invalid name! Please only use letters and spaces." << endl;
		}

	} while (!IsNameValid(name));

	// Loop through all rooms to find bookings under this name
	for (int i = 0; i < maxRooms; i++)
	{
		if (name == rooms[i].CustomerName && (rooms[i].BookingID != -1)) {
			if (!rooms[i].Paid) {
				int days = ((CalculateDaysStayed(rooms[i].StartDate, rooms[i].EndDate)));
				double cost = days * (rooms[i].PricePerNight);

				Total += cost;
				found = true;
				cout << "----------------------------------------"<<endl;
				cout << "\033[35mRoom \033[0m" << rooms[i].RoomNo << "\033[35m: \033[35m" <<endl << days
					<< " nights x $" << rooms[i].PricePerNight
					<< " = $" << cost << endl;
				cout << "\033[37m----------------------------------------\033[37m"<<endl;
			}
			else {
				cout << "Room is already paid for." << endl;
				return;
			}
		}


	}

	// Show total or error message
	if (!found)
	{
		cout << "No bookings found under this name " << endl;
	}
	else {
		cout << "Your total is: $ " << Total;
	}
	int choice;
	PaymentMethod paymentChoice;

	do {
		cout << "\nChoose payment method:\n";
		cout << "1. Cash\n";
		cout << "2. Visa\n";
		cout << "Enter your choice (1 or 2): ";
		cin >> choice;

		if (choice == 1) {
			paymentChoice = Cash;
			cout << "Please proceed to the cashier to pay $" << Total << " in cash." << endl;
			for (int i = 0; i < maxRooms; i++) {
				if (rooms[i].CustomerName == name) {
					rooms[i].Paid = 1;
				}

			}
		}
		else if (choice == 2) {
			paymentChoice = Visa;
			string visaNumber;
			cout << "Enter your Visa card number (16 digits): ";
			cin >> visaNumber;

			while (visaNumber.length() != 16) {
				cout << "Invalid card number. Please enter a 16-digit number: ";
				cin >> visaNumber;
			}

			cout << "Payment of $" << Total << " was successful using Visa ending in "
				<< visaNumber.substr(12, 4) << "." << endl;
			for (int i = 0; i < maxRooms; i++) {
				if (rooms[i].CustomerName == name) {
					rooms[i].Paid = 1;
				}

			}
		}
		else {
			cout << "Invalid choice. Please enter 1 or 2." << endl;
		}

	} while (choice != 1 && choice != 2);
}

void CheckIn(Room rooms[], const int maxRooms) {
	string name;
	string date;
	int RoomNum;

	// Ask for the user's name and validate it
	do {
		cout << "Enter your name: ";
		cin.ignore();
		getline(cin, name);

		if (!IsNameValid(name)) {
			cout << "Invalid name! Please only use letters and spaces." << endl;
		}

	} while (!IsNameValid(name));

	//Ask for the current date

	cout << "Enter current date: ";
	cin >> date;

	cout << "Rooms booked under this name: " << endl;
	bool found = false;
	for (int i = 0; i < maxRooms; i++) {
		if (rooms[i].CustomerName == name && (IsDateWithinBooking(date, rooms[i].StartDate, rooms[i].EndDate) == true)) {
			cout << "Room Number: " << rooms[i].RoomNo << endl;
			found = true;

		}
	}
	if (!found) {
		cout << "No rooms booked under this name ";
		return;
	}

	//Allow the user to check into multiple rooms

	cout << "Enter the Room Numbers you would like to check in(enter 0 to stop)" << endl;;

	do {
		cout << "Room Number: ";
		cin >> RoomNum;

		if (RoomNum == 0) {
			break;

		}
		bool roomfound = false;

		//Verify that the room number belongs to the user and is valid for check - in

		for (int i = 0; i < maxRooms; i++) {
			if (rooms[i].RoomNo == RoomNum && rooms[i].CustomerName == name) {
				rooms[i].CheckedIn = true;
				cout << "You have successfully checked in to room " << RoomNum << endl;
				roomfound = true;
				break;
			}
		}
		if (!roomfound) {
			cout << "Room " << RoomNum << " not found or not booked under your name" << endl;
		}


	} while (RoomNum != 0);
	SavetoFile(rooms, maxRooms, "HotelManagement.csv");

}

void CheckOut(Room rooms[], const int maxRooms) {
	string name;
	string date;
	int RoomNum;

	// Ask for the user's name and validate it
	do {
		cout << "Enter your name: ";
		getline(cin, name);

		if (!IsNameValid(name)) {
			cout << "Invalid name! Please only use letters and spaces." << endl;
		}

	} while (!IsNameValid(name));

	//Ask for the current date

	cout << "Enter current date: ";
	cin >> date;

	cout << "Rooms Checked in under this name: " << endl;
	bool found = false;
	for (int i = 0; i < maxRooms; i++) {
		if (rooms[i].CustomerName == name && (rooms[i].CheckedIn)) {
			cout << "Room Number: " << rooms[i].RoomNo << endl;
			found = true;

		}
	}
	if (!found) {
		cout << "No rooms checked in under this name ";
		return;
	}

	//Allow the user to check out of multiple rooms

	cout << "Enter the Room Numbers you would like to check out (enter 0 to stop)" << endl;

	do {
		cout << "Room Number: ";
		cin >> RoomNum;

		if (RoomNum == 0) {
			break;

		}
		bool roomfound = false;

		//Verify that the room number belongs to the user and is valid for check - out

		for (int i = 0; i < maxRooms; i++) {
			if (rooms[i].RoomNo == RoomNum && rooms[i].CustomerName == name && (rooms[i].CheckedIn)) {

				rooms[i].CheckedIn = false;
				rooms[i].BookingID = -1;
				rooms[i].CustomerName = "";
				rooms[i].StartDate = "";
				rooms[i].EndDate = "";

				cout << "You have successfully checked out of room " << RoomNum << endl;
				roomfound = true;
				break;
			}
		}
		if (!roomfound) {
			cout << "Room " << RoomNum << " not found or not checked in under your name" << endl;
		}


	} while (RoomNum != 0);

	SavetoFile(rooms, maxRooms, "HotelManagement.csv");

}


