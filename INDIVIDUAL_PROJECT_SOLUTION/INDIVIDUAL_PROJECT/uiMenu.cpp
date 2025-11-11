#include <iostream>
#include "uiMenu.h"
#include "astroView.h"


using namespace std;


void menu() {
    int choice;
    do {
        cout << "Menu:\n";
        cout << "1. View astronomical observational list \n";
        cout << "2. Add new astronomical object \n";
        cout << "3. Edit astronomical object \n";
        cout << "4. Delete data \n";
        cout << "5. Search data \n";
        cout << "6. Sort data \n";
        cout << "7. Calculations\n";
        cout << "8. Exit \n";
        cout << endl;
        cout << " Enter number of menu ";
        cin >> choice;
        cout << endl;

        cin.ignore();

        switch (choice) {
        case 1:
            readFromFile();
            break;
        case 2:
            enterAstroObjects();
            saveToFile();
            break;
        case 3:
            editAstroObject();
            break;
        case 4:
            deleteData();
            break;
        case 5:
            searchData();
            break;
        case 6:
            sortData();
            break;
        case 7:
            calculationAstro();
            break;
        case 8:
            cout << "Exit from program";
            break;
        default:
            cout << "Invalid choice. Try again";
        }

        
    } while (choice != 8);

}