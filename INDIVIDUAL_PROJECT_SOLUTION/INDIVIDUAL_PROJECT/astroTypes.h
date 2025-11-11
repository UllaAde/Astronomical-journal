#ifndef ASTROTYPES_H
#define ASTROTYPES_H


struct Date {
    int day;
    int month;
    int year;

};



struct Object_count
{
    char objectName[30];
    char objectType[40];
    char objectConstellation[40];
    char observComments[400];
    int DayMonthYear;
  

};

extern Object_count* objectsArr;
extern int n;
extern const char* FILE_NAME;

#endif