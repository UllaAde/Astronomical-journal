#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <iomanip>
#include "astroTypes.h"

using namespace std; 

Object_count* objectsArr = nullptr;
int n = 0;
const char* FILE_NAME = "astro_object.dat";

void enterAstroObjects() {
    cout << "How many astronomical objects you want to enter? ";
    while (!(cin >> n) || n <= 0) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Please enter a valid positive integer: ";
    }
    cin.ignore();

    delete[] objectsArr;
    objectsArr = new Object_count[n];

    for (int i = 0; i < n; i++) {
        cout << i + 1 << ":" << endl;
        cout << " Enter object name: ";
        cin.getline(objectsArr[i].objectName, 30);

        cout << " Enter object type: ";
        cin.getline(objectsArr[i].objectType, 40);

        cout << " Enter constellation: ";
        cin.getline(objectsArr[i].objectConstellation, 40);

        cout << " Enter observations comments: ";
        cin.getline(objectsArr[i].observComments, 400);

        cout << " Enter observation date (format DDMMYYYY): ";
        while (true) {
            cin >> objectsArr[i].DayMonthYear;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Error: Please enter a valid date (8 digits, format DDMMYYYY): ";
            }
            else {
                
                int temp = objectsArr[i].DayMonthYear;
                int digits = 0;
                while (temp != 0) {
                    temp /= 10;
                    digits++;
                }
                if (digits == 8) {
                    cin.ignore(); 
                    break;
                }
                else {
                    cout << "Error: Date must have exactly 8 digits (DDMMYYYY): ";
                }
            }
        }
    }
}

void saveToFile() {
    ofstream file(FILE_NAME, ios::binary | ios::app);
    if (!file) {
        cout << "Error while opening file!" << endl;
        return;
    }

    for (int i = 0; i < n; i++) {
        file.write((char*)&objectsArr[i], sizeof(Object_count));
    }
    file.close();
    cout << "Astronomical object saved to file\n" << endl;
}



void printWrapped(const string& text, int width, int indent) {
    int pos = 0;
    while (pos < text.length()) {
        cout << string(indent, ' ') << text.substr(pos, width) << endl;
        pos += width;
    }
}


void readFromFile() {
    ifstream file(FILE_NAME, ios::binary);
    if (!file) {
        cout << "Data about astronomical object not found!" << endl;
        return;
    }

    cout << left
        << setw(5) << "No."
        << setw(20) << "Object Name"
        << setw(15) << "Object Type"
        << setw(20) << "Constellation"
        << setw(15) << "Obs. Date"
        << setw(30) << "Comments"
        << endl;

    cout << string(105, '-') << endl;

    Object_count s;
    int i = 1;

    while (file.read((char*)&s, sizeof(Object_count))) {
        string comment = s.observComments;

        
        cout << left
            << setw(5) << i++
            << setw(20) << s.objectName
            << setw(15) << s.objectType
            << setw(20) << s.objectConstellation
            << setw(15) << s.DayMonthYear;

        if (comment.length() > 30) {
            cout << setw(30) << comment.substr(0, 30) << endl;
            printWrapped(comment.substr(30), 30, 75); 
        }
        else {
            cout << setw(30) << comment << endl;
        }
    }

    file.close();
}

void editAstroObject() {
    ifstream inFile(FILE_NAME, ios::binary);
    if (!inFile) {
        cout << "File not found\n";
        return;
    }

    const int MAX_OBJECTS = 100;
    Object_count objects[MAX_OBJECTS];
    int count = 0;

    while (inFile.read((char*)&objects[count], sizeof(Object_count))) {
        count++;
        if (count >= MAX_OBJECTS) break;
    }
    inFile.close();

    if (count == 0) {
        cout << "No astronomical objects found.";
        return;
    }

    for (int i = 0; i < count; i++) {
        cout << i + 1 << ". " << objects[i].objectName << endl;
    }

    int choice;
    cout << "Enter the number of object to edit: ";
    cin >> choice;
    cin.ignore();

    if (choice < 1 || choice > count) {
        cout << "Invalid number.";
        return;
    }

    Object_count& obj = objects[choice - 1];

    cout << "New object name: ";
    cin.getline(obj.objectName, 30);

    cout << "New object type: ";
    cin.getline(obj.objectType, 40);

    cout << "New constellation: ";
    cin.getline(obj.objectConstellation, 40);

    cout << "New observation date: ";
    while (!(cin >> obj.DayMonthYear)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Error! Enter the date as a number: ";
    }
    cin.ignore();

    cout << "New observation comments: ";
    cin.getline(obj.observComments, 400);

    ofstream outFile(FILE_NAME, ios::binary | ios::trunc);
    if (!outFile) {
        cout << "Error opening file for editing!\n";
        return;
    }

    for (int i = 0; i < count; i++) {
        outFile.write((char*)&objects[i], sizeof(Object_count));
    }

    outFile.close();
    cout << "Object edited successfully!\n";
}

void deleteData() {
    ifstream inFile(FILE_NAME, ios::binary);
    if (!inFile) {
        cout << "File not found\n";
        return;
    }

    Object_count temp[100];
    int count = 0;

    while (inFile.read((char*)&temp[count], sizeof(Object_count))) {
        count++;
        if (count >= 100) break;
    }
    inFile.close();

    if (count == 0) {
        cout << "No records found to delete.\n";
        return;
    }

    int option;
    cout << "Search by:\n1. Object name\n2. Object type\n3. Full date (DDMMYYYY)\nEnter choice: ";
    cin >> option;
    cin.ignore();

    char text[100] = "";
    int dateValue = 0;

    if (option == 3) {
        cout << "Enter full date (DDMMYYYY): ";
        while (!(cin >> dateValue) || dateValue < 1010000 || dateValue > 31129999) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid date! Please enter date in DDMMYYYY format: ";
        }
        cin.ignore();
    }
    else if (option == 1 || option == 2) {
        cout << "Enter search text: ";
        cin.getline(text, 100);
    }
    else {
        cout << "Invalid choice.\n";
        return;
    }

    ofstream outFile(FILE_NAME, ios::binary | ios::trunc);
    if (!outFile) {
        cout << "Error opening file for writing\n";
        return;
    }

    int deleted = 0;

    for (int i = 0; i < count; i++) {
        bool match = false;

        if (option == 1) {
            if (strstr(temp[i].objectName, text) != nullptr)
                match = true;
        }
        else if (option == 2) {
            if (strstr(temp[i].objectType, text) != nullptr)
                match = true;
        }
        else if (option == 3) {
            if (temp[i].DayMonthYear == dateValue)
                match = true;
        }

        if (!match) {
            outFile.write((char*)&temp[i], sizeof(Object_count));
        }
        else {
            deleted++;
            cout << "Deleted: " << temp[i].objectName
                << " (Date: " << temp[i].DayMonthYear << ")\n";
        }
    }

    outFile.close();

    if (deleted > 0) {
        cout << deleted << " record(s) deleted successfully.\n";
    }
    else {
        cout << "No matching records found to delete.\n";
    }
}



void searchData() {
    ifstream file(FILE_NAME, ios::binary);
    if (!file) {
        cout << "File not found.\n";
        return;
    }

    Object_count obj;
    int choice;
    cout << "Search by:\n1. Object Name\n2. Object Type\n3. Observation Date (DDMMYYYY)\nEnter choice: ";
    cin >> choice;
    cin.ignore();

    char text[100];
    int date;
    int valid = 1;

    if (choice == 1 || choice == 2) {
        cout << "Enter text to search: ";
        cin.getline(text, 100);
    }
    else if (choice == 3) {
        cout << "Enter observation date (DDMMYYYY): ";
        cin >> date;
    }
    else {
        valid = 0;
    }

    if (!valid) {
        cout << "Invalid choice.\n";
        return;
    }

    int found = 0;

    while (file.read((char*)&obj, sizeof(Object_count))) {
        int match = 0;

        if (choice == 1 && strstr(obj.objectName, text) != nullptr) {
            match = 1;
        }
        else if (choice == 2 && strstr(obj.objectType, text) != nullptr) {
            match = 1;
        }
        else if (choice == 3 && obj.DayMonthYear == date) {
            match = 1;
        }

        if (match == 1) {
            found++;
            cout << "\nObject: " << obj.objectName
                << "\nType: " << obj.objectType
                << "\nConstellation: " << obj.objectConstellation
                << "\nDate: " << obj.DayMonthYear
                << "\nComments: " << obj.observComments << "\n";
        }
    }

    file.close();

    if (found == 0) {
        cout << "No matching records found.\n";
    }
    else {
        cout << "Found " << found << " matching record(s).\n";
    }
}


void sortData() {
    ifstream inFile(FILE_NAME, ios::binary);
    if (!inFile) {
        cout << "File not found\n";
        return;
    }

    Object_count data[100];
    int count = 0;

    while (inFile.read((char*)&data[count], sizeof(Object_count))) {
        count++;
        if (count >= 100) break;
    }
    inFile.close();

    if (count == 0) {
        cout << "No data to sort.\n";
        return;
    }

    int option;
    cout << "Sort by:\n1. Object Name\n2. Observation Date (DDMMYYYY)\n3. Object Type\nEnter choice: ";
    cin >> option;

    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            int swap = 0;

            if (option == 1) {
                if (strcmp(data[i].objectName, data[j].objectName) > 0)
                    swap = 1;
            }
            else if (option == 2) {
                int d1 = data[i].DayMonthYear / 1000000;
                int m1 = (data[i].DayMonthYear / 10000) % 100;
                int y1 = data[i].DayMonthYear % 10000;

                int d2 = data[j].DayMonthYear / 1000000;
                int m2 = (data[j].DayMonthYear / 10000) % 100;
                int y2 = data[j].DayMonthYear % 10000;

                if (y1 > y2 || (y1 == y2 && m1 > m2) || (y1 == y2 && m1 == m2 && d1 > d2))
                    swap = 1;
            }
            else if (option == 3) {
                if (strcmp(data[i].objectType, data[j].objectType) > 0)
                    swap = 1;
            }

            if (swap == 1) {
                Object_count temp = data[i];
                data[i] = data[j];
                data[j] = temp;
            }
        }
    }

    ofstream outFile(FILE_NAME, ios::binary | ios::trunc);
    if (!outFile) {
        cout << "Error opening file for writing\n";
        return;
    }

    
    cout << left
        << setw(5) << "No."
        << setw(20) << "Object Name"
        << setw(15) << "Object Type"
        << setw(15) << "Observation Date"
        << setw(25) << "Constellation"
        << setw(30) << "Comments" << endl;

    cout << string(110, '-') << endl;

    
    for (int i = 0; i < count; i++) {
        outFile.write((char*)&data[i], sizeof(Object_count));

        int d = data[i].DayMonthYear / 1000000;
        int m = (data[i].DayMonthYear / 10000) % 100;
        int y = data[i].DayMonthYear % 10000;

        cout << setw(5) << (i + 1)
            << setw(20) << data[i].objectName
            << setw(15) << data[i].objectType
            << setw(2) << (d < 10 ? "0" : "") << d << "/"
            << setw(2) << (m < 10 ? "0" : "") << m << "/"
            << setw(7) << y
            << setw(25) << data[i].objectConstellation
            << setw(30) << data[i].observComments << endl;
    }

    outFile.close();
    cout << "Data sorted and saved.\n";
}

void calculationAstro() {
    ifstream file(FILE_NAME, ios::binary);
    if (!file) {
        cout << "File not found.\n";
        return;
    }

    Object_count obj;
    int total = 0;
    int yearSum = 0;
    int earliestYear = 9999;
    int latestYear = 0;

    while (file.read((char*)&obj, sizeof(Object_count))) {
        total++;

        int year = obj.DayMonthYear % 10000; 
        yearSum += year;

        if (year < earliestYear) earliestYear = year;
        if (year > latestYear) latestYear = year;
    }

    file.close();

    if (total > 0) {
        cout << left << setw(30) << "Calculation" << "Value" << endl;
        cout << string(45, '-') << endl;

        cout << setw(30) << "Total objects:" << total << endl;
        cout << setw(30) << "Average observation year:" << (yearSum / total) << endl;
        cout << setw(30) << "Earliest observation year:" << earliestYear << endl;
        cout << setw(30) << "Latest observation year:" << latestYear << endl;
    }
    else {
        cout << "No data found.\n";
    }
    cout << endl;
}

