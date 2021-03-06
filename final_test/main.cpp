#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include "vector"
#include "string"
#include "Date.h"
#include "Time.h"
#include "AirQuality.h"
using namespace std;

void getData(ifstream &inFile, vector<string>& fields, vector<AirQuality>& airQualityCollection);
Date processDate(string inputStr);
Time processTime(string inputStr);
double calculateAvgTemp(int yearInput, int monthInput, vector<AirQuality>& airQualityCollection);
double calculateRelHumidity(int yearInput, int monthInput, vector<AirQuality>& airQualityCollection);
double calculateAbsHumidity(int yearInput, int monthInput, vector<AirQuality>& airQualityCollection);
void displayTempAndRelHumidity(Date dateInput, Time timeInput, double &currentTemp, double &currentRelHumidity, vector<AirQuality>& airQualityCollection);
double displayMaxTemp(int yearInput, int monthInput, vector<AirQuality>& airQualityCollection);
double displayMaxRelHumidity(int yearInput, int monthInput, vector<AirQuality>& airQualityCollection);
double displayMaxAbsHumidity(int yearInput, int monthInput, vector<AirQuality>& airQualityCollection);
void findTempHigherThanAvg(int yearInput, int monthInput, vector<AirQuality>& airQualityCollection, vector<AirQuality>& tempHigherThanAvg);
void findRelHumidHigherThanAvg(int yearInput, int monthInput, vector<AirQuality>& airQualityCollection, vector<AirQuality>& relHumidHigherThanAvg);
void findAbsHumidHigherThanAvg(int yearInput, int monthInput, vector<AirQuality>& airQualityCollection, vector<AirQuality>& absHumidHigherThanAvg);
void showWelcomeMsg();
void showMenu();
int  userChoice();
bool validMonth(int monthInput);
void doUserChoice(int option, vector<AirQuality>& airQualityCollection);
Time getValidTime();
int getValidInputMonth();
int getValidInputYear();
int getValidInputDay(int inputYear, int inputMonth);
void doCalOrDis(int option, int yearInput, int monthInput,vector<AirQuality>& airQualityCollection);
void doCalThanAvg(int option, int yearInput, int monthInput, vector<AirQuality>& airQualityCollection, vector<AirQuality>& absHumidHigherThanAvg);
void doFindTempAndHum(Date inputDate, Time inputTime, double &currentTemp, double &currentRelHumidity, vector<AirQuality>& airQualityCollection);


//int main()
//{
//    ifstream myFile("AirQualityUCI.csv");
//
//    if (!myFile.is_open())
//    {
//        cout << "File not found." << endl;
//        return 1;
//    }
//
//    vector<string> fields;
//    vector<AirQuality> airQualityCollection;
//    getData(myFile, fields, airQualityCollection);
//    showWelcomeMsg();
//    showMenu();
//    int res = userChoice();
//    int EXIT = -1;
//    while (res != EXIT)
//    {
//        doUserChoice(res, airQualityCollection);
//        cout << endl;
//        showMenu();
//        res = userChoice();
//        if (res == EXIT)
//        {
//            break;
//        }
//    }
//    return 0;
//}

int main()
{
    ifstream myFile("AirQualityUCI.csv");

    if (!myFile.is_open())
    {
        cout << "File not found." << endl;
        return 1;
    }

    vector<string> fields;
    vector<AirQuality> airQualityCollection;

    // Test: getData()
    getData(myFile, fields, airQualityCollection);
    // Expected: 9357, Actual: 9357
    cout << "Get data, size is: " << airQualityCollection.size() << endl;

    // Test: calculateAvgTemp()
    // Expected: 14.3908, Actual: 14.3908
    cout << "avg temp is: " << calculateAvgTemp(4, 3, airQualityCollection) << endl;
    // Test: calculateRelHumidity()
    // Expected: 50.1708, Actual: 50.1708
    cout << "avg relHumidity is: " << calculateRelHumidity(4, 3, airQualityCollection) << endl;
    // Test: calculateAbsHumidity()
    // Expected: 0.789413, Actual: 0.789413
    cout << "abs relHumidity is: " << calculateAbsHumidity(4, 3, airQualityCollection) << endl;

    // Test: displayTempAndRelHumidity()
    // Expected: temp:13.6, relative humid:48.9, Actual: temp:13.6, relative humid:48.9
    Date d1(10, 3, 4);
    Time t1(0, 0, 18);
    double currentTemp, currentRelHumidity;
    displayTempAndRelHumidity(d1, t1, currentTemp, currentRelHumidity, airQualityCollection);
    cout << "At that date and time, temp is: " << currentTemp << ", rel humid is: " << currentRelHumidity << endl;

    // Test: displayMaxTemp()
    // Expected: 29.3, Actual: 29.3
    cout << "max temp is: " << displayMaxTemp(4, 3, airQualityCollection) << endl;
    // Test: displayMaxRelHumidity()
    // Expected: 83.2, Actual: 83.2
    cout << "max rel is: " << displayMaxRelHumidity(4, 3, airQualityCollection) << endl;
    // Test: displayMaxAbsHumidity()
    // Expected: 1.0945, Actual: 1.0945
    cout << "max abs is: " << displayMaxAbsHumidity(4, 3, airQualityCollection) << endl;

    // Test: findTempHigherThanAvg()
    // Expected: size 239, Actual: size 239
    vector<AirQuality> tempHigherThanAvg;
    findTempHigherThanAvg(4, 3, airQualityCollection, tempHigherThanAvg);
    cout << "Higher than avg temp num: " << tempHigherThanAvg.size() << endl;

    // Test: findRelHumidHigherThanAvg()
    // Expected: size 285, Actual: size 285
    vector<AirQuality>relHumidHigherThanAvg;
    findRelHumidHigherThanAvg(4, 3, airQualityCollection, relHumidHigherThanAvg);
    cout << "Higher than avg rel num: " << relHumidHigherThanAvg.size() << endl;

    // Test: findAbsHumidHigherThanAvg()
    // Expected: size 255, Actual: size 255
    vector<AirQuality>absHumidHigherThanAvg;
    findAbsHumidHigherThanAvg(4, 3, airQualityCollection, absHumidHigherThanAvg);
    cout << "Higher than avg abs: " << absHumidHigherThanAvg.size() << endl;

    return 0;

}

void getData(ifstream &inFile, vector<string>& fields, vector<AirQuality>& airQualityCollection)
{
    static const int DATE_IDX = 0;
    static const int TIME_IDX = 1;
    static const int TEMP_IDX = 12;
    static const int REL_HUMID_IDX = 13;
    static const int ABS_HUMID_IDX = 14;

    string strs;
    string delimiter = ",";
    size_t pos;
    // skip the first line
    getline(inFile, strs);
    // read each line, store as strs
    while (getline(inFile, strs))
    {
        size_t size = strs.size() - 1;
        // skip empty line (',,,,')
        if (strs[0] == ',')
        {
            break;
        }
        for (int i = 0; i < size; ++i)
        {
            pos = strs.find(delimiter, i);
            if (pos < size)
            {
                string token = strs.substr(i, pos - i);
                fields.push_back(token);
                i = pos + delimiter.size() - 1;
            }
        }

        // create date object with element in fields vector (index = 0)
        Date dateVal = processDate(fields.at(DATE_IDX));
        // create time object with element in fields vector (index = 1)
        Time timeVal = processTime(fields.at(TIME_IDX));
        // convert other parameters from string to double
        double temperature = stod(fields.at(TEMP_IDX));
        double relHumid = stod(fields.at(REL_HUMID_IDX));
        double absHumid = stod(fields.at(ABS_HUMID_IDX));
        // create AirQuality object, store in airQualityCollection vector
        AirQuality airQualityVal(dateVal, timeVal, temperature, relHumid, absHumid);
        airQualityCollection.push_back(airQualityVal);
        fields.clear();
    }

    inFile.close();
}

Date processDate(string inputStr)
{
    string delimiter = "/";
    size_t pos;
    string strs = inputStr + delimiter;
    size_t size = strs.size();
    vector<int> dateValues;
    // split the input string to 3 substrings, convert to int
    for (int i = 0; i < size; ++i)
    {
        pos = strs.find(delimiter, i);
        if (pos < size)
        {
            string token = strs.substr(i, pos - i);
            int tokenNum = stoi(token);
            dateValues.push_back(tokenNum);
            i = pos + delimiter.size() - 1;
        }
    }

    static const int DAY_IDX = 1;
    static const int MONTH_IDX = 0;
    static const int YEAR_IDX = 2;
    // create date object with elements in dateValues vector
    Date dateVal(dateValues.at(DAY_IDX), dateValues.at(MONTH_IDX), dateValues.at(YEAR_IDX));
    return dateVal;
}

Time processTime(string inputStr)
{
    string delimiter = ":";
    size_t pos;
    string strs = inputStr + delimiter;
    size_t size = strs.size();
    vector<int> timeValues;
    // split the input string to 3 substrings, convert to int
    for (int i = 0; i < size; ++i)
    {
        pos = strs.find(delimiter, i);
        if (pos < size)
        {
            string token = strs.substr(i, pos - i);
            int tokenNum = stoi(token);
            timeValues.push_back(tokenNum);
            i = pos + delimiter.size() - 1;
        }
    }

    static const int HOUR_IDX = 0;
    static const int MIN_IDX = 1;
    static const int SEC_IDX = 2;
    // create Time object with elements in timeVal vector
    Time timeVal(timeValues.at(SEC_IDX), timeValues.at(MIN_IDX), timeValues.at(HOUR_IDX));
    return timeVal;
}

double calculateAvgTemp(int yearInput, int monthInput, vector<AirQuality>& airQualityCollection)
{
    static const double MISS_VALUE = -200;
    double totalTemp = 0;
    int count = 0;
    for (int i = 0; i < airQualityCollection.size(); i++){
        if (airQualityCollection[i].getDate().getYear() == yearInput && airQualityCollection[i].getDate().getMonth() == monthInput){
            if (airQualityCollection[i].getTemp() != MISS_VALUE){
                totalTemp += airQualityCollection[i].getTemp();
                count++;
            }
        }
    }
    double avgTemp = totalTemp / count;

    if (count == 0){
        return 0;
    }
    else{
        return avgTemp;
    }
}

double calculateRelHumidity(int yearInput, int monthInput, vector<AirQuality>& airQualityCollection)
{
    static const double MISS_VALUE = -200;
    double totalHumidity = 0;
    int count = 0;
    for (int i = 0; i < airQualityCollection.size(); i++){
        if (airQualityCollection[i].getDate().getYear() == yearInput && airQualityCollection[i].getDate().getMonth() == monthInput){
            if (airQualityCollection[i].getRelativeHumidity() != MISS_VALUE){
                totalHumidity += airQualityCollection[i].getRelativeHumidity();
                count++;
            }
        }
    }
    double avgHumidity = totalHumidity / count;

    if (count == 0){
        return 0;
    }
    else{
        return avgHumidity;
    }
}


double calculateAbsHumidity(int yearInput, int monthInput, vector<AirQuality>& airQualityCollection)
{
    static const double MISS_VALUE = -200;
    double totalHumidity = 0;
    int count = 0;
    for (int i = 0; i < airQualityCollection.size(); i++){
        if (airQualityCollection[i].getDate().getYear() == yearInput && airQualityCollection[i].getDate().getMonth() == monthInput){
            if (airQualityCollection[i].getAbsHumidity() != MISS_VALUE){
                totalHumidity += airQualityCollection[i].getAbsHumidity();
                count++;
            }
        }
    }
    double avgHumidity = totalHumidity / count;

    if (count == 0){
        return 0;
    }
    else{
        return avgHumidity;
    }
}

void displayTempAndRelHumidity(Date dateInput, Time timeInput, double &currentTemp, double &currentRelHumidity, vector<AirQuality>& airQualityCollection)
{
    for (int i = 0; i < airQualityCollection.size(); i++){
        if (dateInput == airQualityCollection[i].getDate() && timeInput == airQualityCollection[i].getTime()){
            currentTemp = airQualityCollection[i].getTemp();
            currentRelHumidity = airQualityCollection[i].getRelativeHumidity();
            return;
        }
    }
    currentTemp = 0;
    currentRelHumidity = 0;
}

double displayMaxTemp(int yearInput, int monthInput, vector<AirQuality>& airQualityCollection)
{
    static const double THRESHOLD = -100;
    double maxTemp = THRESHOLD;
    for (int i = 0; i < airQualityCollection.size(); i++){
        if (airQualityCollection[i].getDate().getYear() == yearInput && airQualityCollection[i].getDate().getMonth() == monthInput) {
            maxTemp = max(maxTemp, airQualityCollection[i].getTemp());
        }
    }
    return maxTemp;
}

double displayMaxRelHumidity(int yearInput, int monthInput, vector<AirQuality>& airQualityCollection)
{
    static const double THRESHOLD = -100;
    double maxRelHumidity = THRESHOLD;
    for (int i = 0; i < airQualityCollection.size(); i++){
        if (airQualityCollection[i].getDate().getYear() == yearInput && airQualityCollection[i].getDate().getMonth() == monthInput) {
            maxRelHumidity = max(maxRelHumidity, airQualityCollection[i].getRelativeHumidity());
        }
    }
    return maxRelHumidity;
}

double displayMaxAbsHumidity(int yearInput, int monthInput, vector<AirQuality>& airQualityCollection)
{
    static const double THRESHOLD = -100;
    double maxAbsHumidity = THRESHOLD;
    for (int i = 0; i < airQualityCollection.size(); i++){
        if (airQualityCollection[i].getDate().getYear() == yearInput && airQualityCollection[i].getDate().getMonth() == monthInput) {
            maxAbsHumidity = max(maxAbsHumidity, airQualityCollection[i].getAbsHumidity());
        }
    }
    return maxAbsHumidity;
}

void findTempHigherThanAvg(int yearInput, int monthInput, vector<AirQuality>& airQualityCollection, vector<AirQuality>& tempHigherThanAvg)
{
    double avgTemp = calculateAvgTemp(yearInput, monthInput, airQualityCollection);
    // if avg = 0 -> cannot find data corresponding to monthInput
    if (avgTemp == 0)
    {
        return;
    }
    for (int i = 0; i < airQualityCollection.size(); i++){
        if (airQualityCollection[i].getDate().getYear() == yearInput && airQualityCollection[i].getDate().getMonth() == monthInput){
            if (airQualityCollection[i].getTemp() > avgTemp){
                tempHigherThanAvg.push_back(airQualityCollection[i]);
            }
        }
    }
}

void findRelHumidHigherThanAvg(int yearInput, int monthInput, vector<AirQuality>& airQualityCollection, vector<AirQuality>& relHumidHigherThanAvg)
{
    double avgRelHumid = calculateRelHumidity(yearInput, monthInput, airQualityCollection);
    // if avg = 0 -> cannot find data corresponding to monthInput
    if (avgRelHumid == 0)
    {
        return;
    }
    for (int i = 0; i < airQualityCollection.size(); i++){
        if (airQualityCollection[i].getDate().getYear() == yearInput && airQualityCollection[i].getDate().getMonth() == monthInput){
            if (airQualityCollection[i].getRelativeHumidity() > avgRelHumid){
                relHumidHigherThanAvg.push_back(airQualityCollection[i]);
            }
        }
    }
}

void findAbsHumidHigherThanAvg(int yearInput, int monthInput, vector<AirQuality>& airQualityCollection, vector<AirQuality>& absHumidHigherThanAvg)
{
    double avgAbsHumid = calculateAbsHumidity(yearInput, monthInput, airQualityCollection);
    // if avg = 0 -> cannot find data corresponding to monthInput
    if (avgAbsHumid == 0)
    {
        return;
    }
    for (int i = 0; i < airQualityCollection.size(); i++){
        if (airQualityCollection[i].getDate().getYear() == yearInput && airQualityCollection[i].getDate().getMonth() == monthInput){
            if (airQualityCollection[i].getAbsHumidity() > avgAbsHumid){
                absHumidHigherThanAvg.push_back(airQualityCollection[i]);
            }
        }
    }
}

void showWelcomeMsg()
{
    cout << endl;
    cout << "Welcome to the Air Quality Processor!" << endl;
    cout << endl;
}

void showMenu()
{
    cout << "******************************************************************************" << endl;
    cout << "*                                 MENU                                       *" << endl;
    cout << "* 0.  Display the average temperature for a certain month.                   *" << endl;
    cout << "* 1.  Display the average relative humidity for a certain month.             *" << endl;
    cout << "* 2.  Display the average absolute humidity temperature for a certain month. *" << endl;
    cout << "* 3.  Display the temperature and temperature at a certain date and time.    *" << endl;
    cout << "* 4.  Display the highest temperature for a certain month.                   *" << endl;
    cout << "* 5.  Display the highest relative humidity value for a certain month.       *" << endl;
    cout << "* 6.  Display the highest absolute humidity for a certain month.             *" << endl;
    cout << "* 7.  Display temperature higher than the average for a certain month.       *" << endl;
    cout << "* 8.  Display relative humidity higher than the average for a certain month. *" << endl;
    cout << "* 9. Display absolute humidity higher than the average for a certain month. *" << endl;
    cout << "*                                                                            *" << endl;
    cout << "******************************************************************************" << endl;
}

int userChoice()
{
    cout << endl;
    cout << "------------------------------------------------------------------------------" << endl;
    cout << "Please select your option by enter the No." << endl
         << "Enter -1 when you finished." << endl;
    int MAX_OPTION = 10;
    bool selecting = true;
    while (selecting)
    {
        int userInput;
        cin >> userInput;
        while (cin.fail())
        {
            cin.clear();
            int const IGNORE_SIZE = 9999999;
            cin.ignore(IGNORE_SIZE, '\n');
            cout << "ERROR: " << endl;
            cout << "Invalid option. Only number accepted. Please Re-enter." << endl;
            showMenu();
            cin >> userInput;
        }
        if (userInput >= -1 && userInput <= MAX_OPTION)
        {
            selecting = false;
            return userInput;
        }
        else
        {
            cout << "ERROR: " << endl;
            cout << "Invalid option. Option out of range. Please Re-enter." << endl;
            showMenu();
        }
    }
    return -1;
}

bool validMonth(int monthInput)
{
    int MIN_MONTH = 1;
    int MAX_MONTH = 12;
    return (monthInput >= MIN_MONTH && monthInput <= MAX_MONTH);
}

bool validTime(int h, int m, int s)
{
    int MIN_TIME = 0;
    int MAX_HOUR = 24;
    int MAX_MIN_SEC = 59;
    bool hourValidity = ((h >= MIN_TIME) && (h <= MAX_HOUR));
    bool minValidity = ((m >= MIN_TIME) && (m <= MAX_MIN_SEC));
    bool secValidity = ((s >= MIN_TIME ) && (s <= MAX_MIN_SEC));
    return (hourValidity && minValidity && secValidity);
}

Time getValidTime()
{
    bool invalid = true;
    while (invalid)
    {
        int inputHour, inputMin, inputSec;
        cout << "Please enter the value of hour." << endl;
        cin >> inputHour;
        while (cin.fail())
        {
            cin.clear();
            int const IGNORE_SIZE = 9999999;
            cin.ignore(IGNORE_SIZE, '\n');
            cout << "ERROR: " << endl;
            cout << "Invalid option. Only number accepted. Please Re-enter." << endl;
            cin >> inputHour;
        }
        cout << "Please enter the value of minutes." << endl;
        cin >> inputMin;
        while (cin.fail())
        {
            cin.clear();
            int const IGNORE_SIZE = 9999999;
            cin.ignore(IGNORE_SIZE, '\n');
            cout << "ERROR: " << endl;
            cout << "Invalid option. Only number accepted. Please Re-enter." << endl;
            cin >> inputMin;
        }
        cout << "Please enter the value of seconds." << endl;
        cin >> inputSec;
        while (cin.fail())
        {
            cin.clear();
            int const IGNORE_SIZE = 9999999;
            cin.ignore(IGNORE_SIZE, '\n');
            cout << "ERROR: " << endl;
            cout << "Invalid option. Only number accepted. Please Re-enter." << endl;
            cin >> inputSec;
        }
        if (validTime(inputHour, inputMin, inputSec))
        {
            invalid = false;
            return Time(inputSec, inputMin, inputHour);
        }
        else
        {
            cout << "Invalid combination, please Re-enter." << endl;
        }
    }
    return Time();
}

int getValidInputYear()
{
    int yearInput;
    cout << "Please enter a year: " << endl;
    cin >> yearInput;
    while (cin.fail())
    {
        cin.clear();
        int const IGNORE_SIZE = 9999999;
        cin.ignore(IGNORE_SIZE, '\n');
        cout << "ERROR: " << endl;
        cout << "Invalid option. Only number accepted. Please Re-enter." << endl;
        cin >> yearInput;
    }
    return yearInput;
}

int getValidInputMonth()
{
    int monthInput;
    cout << "Please enter a month: " << endl;
    cin >> monthInput;
    while (cin.fail())
    {
        cin.clear();
        int const IGNORE_SIZE = 9999999;
        cin.ignore(IGNORE_SIZE, '\n');
        cout << "ERROR: " << endl;
        cout << "Invalid option. Only number accepted. Please Re-enter." << endl;
        cin >> monthInput;
    }
    while (!validMonth(monthInput)) {
        cout << "Month should be in range of [1, 12], inclusive" << endl;
        cout << "Please re-enter." << endl;
        cin >> monthInput;
        while (cin.fail()) {
            cin.clear();
            int const IGNORE_SIZE = 9999999;
            cin.ignore(IGNORE_SIZE, '\n');
            cout << "ERROR: " << endl;
            cout << "Invalid option. Only number accepted. Please Re-enter." << endl;
            cin >> monthInput;
        }
    }
    return monthInput;
}

bool validDay(int inputYear, int inputMonth, int inputDay) {
    int validDay[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if ((inputYear % 4 == 0) && (inputYear % 100 != 0) || (inputYear % 400 == 0))
    {
        int februaryIndex = 2;
        validDay[februaryIndex] = 29;
    }
    int SHIFT = 1;
    return ((inputDay > 0) && (inputDay <= (validDay[(inputMonth) - SHIFT])));
}

int getValidInputDay(int inputYear, int inputMonth) {
    int dayInput;
    cout << "Please enter a day: " << endl;
    cin >> dayInput;
    while (cin.fail())
    {
        cin.clear();
        int const IGNORE_SIZE = 9999999;
        cin.ignore(IGNORE_SIZE, '\n');
        cout << "ERROR: " << endl;
        cout << "Invalid option. Only number accepted. Please Re-enter." << endl;
        cin >> dayInput;
    }
    while (!validDay(inputYear,inputMonth,dayInput)) {
        cout << "Input day is not valid, there is no " << dayInput << " in the month of " << inputMonth << endl;
        cout << "Please re-enter." << endl;
        cin >> dayInput;
        while (cin.fail()) {
            cin.clear();
            int const IGNORE_SIZE = 9999999;
            cin.ignore(IGNORE_SIZE, '\n');
            cout << "ERROR: " << endl;
            cout << "Invalid option. Only number accepted. Please Re-enter." << endl;
            cin >> dayInput;
        }
    }
    return dayInput;
}

void doUserChoice(int option, vector<AirQuality>& airQualityCollection)
{
    int EXIT = -1;
    if (option == EXIT)
    {
        return;
        // exit from the function
    }
    int yearInput = getValidInputYear();
    int monthInput = getValidInputMonth();
    if (option == 3)
    {
        int dayInput = getValidInputDay(yearInput, monthInput);
        Date curDay = Date(dayInput, monthInput, yearInput);
        Time curTime = getValidTime();
        double curTemp, curRelHumidity;
        doFindTempAndHum(curDay, curTime, curTemp, curRelHumidity, airQualityCollection);
    }
    else
    {
        if (option == 0 || option == 1 || option == 2 ||
            option == 4 || option == 5 || option == 6)
        {

            doCalOrDis(option, yearInput, monthInput, airQualityCollection);
        }
        else
        {
            vector<AirQuality> res;
            doCalThanAvg(option, yearInput, monthInput, airQualityCollection, res);
        }
    }
}

void doCalOrDis(int option, int yearInput, int monthInput,vector<AirQuality>& airQualityCollection)
{
    double res;
    int NO_RECORD_CAL = 0;
    int NO_RECORD_DIS = -100;
    switch (option) {
        case 0:
            res = calculateAvgTemp(yearInput, monthInput, airQualityCollection);
            if (res == NO_RECORD_CAL)
            {
                cout << "No records found." << endl;
            }
            else
            {
                cout << "The average temperature is: " << res << endl;
            }
            break;
        case 1:
            res = calculateRelHumidity(yearInput, monthInput, airQualityCollection);
            if (res == NO_RECORD_CAL)
            {
                cout << "No records found." << endl;
            }
            else
            {
                cout << "The average relative humidity is: " << res << endl;
            }
            break;
        case 2:
            res = calculateAbsHumidity(yearInput, monthInput, airQualityCollection);
            if (res == NO_RECORD_CAL)
            {
                cout << "No records found." << endl;
            }
            else
            {
                cout << "The average absolute humidity is: " << res << endl;
            }
            break;
        case 4:
            res = displayMaxTemp(yearInput, monthInput, airQualityCollection);
            if (res == NO_RECORD_DIS)
            {
                cout << "No records found." << endl;
            }
            else
            {
                cout << "The highest temperature is: " << res << endl;
            }
            break;
        case 5:
            res = displayMaxRelHumidity(yearInput, monthInput, airQualityCollection);
            if (res == NO_RECORD_DIS)
            {
                cout << "No records found." << endl;
            }
            else
            {
                cout << "The highest relative humidity is: " << res << endl;
            }
            break;
        case 6:
            res = displayMaxAbsHumidity(yearInput, monthInput, airQualityCollection);
            if (res == NO_RECORD_DIS)
            {
                cout << "No records found." << endl;
            }
            else
            {
                cout << "The highest absolute humidity is: " << res << endl;
            }
            break;
    }
}

void doCalThanAvg(int option, int yearInput, int monthInput, vector<AirQuality>& airQualityCollection, vector<AirQuality>& res)
{
    switch (option) {
        case 7:
            findTempHigherThanAvg(yearInput, monthInput, airQualityCollection, res);
            if (res.size() == 0)
            {
                cout << "No records found." << endl;
            }
            else
            {
                cout << "The dates and times when temperature is higher than the average are listed below: " << endl;
                for (int i = 0; i < res.size(); i++)
                {
                    cout << "Date:\t" << res[i].getDate() << "\t" << "Time:\t" << res[i].getTime() << endl;
                }
            }
        case 8:
            findRelHumidHigherThanAvg(yearInput, monthInput, airQualityCollection, res);
            if (res.size() == 0)
            {
                cout << "No records found." << endl;
            }
            else
            {
                cout << "The dates and times when temperature is higher than the average are listed below: " << endl;
                for (int i = 0; i < res.size(); i++)
                {
                    cout << "Date:\t" << res[i].getDate() << "\t" << "Time:\t" << res[i].getTime() << endl;
                }
            }
        case 9:
            findAbsHumidHigherThanAvg(yearInput, monthInput, airQualityCollection, res);
            if (res.size() == 0)
            {
                cout << "No records found." << endl;
            }
            else
            {
                cout << "The dates and times when temperature is higher than the average are listed below: " << endl;
                for (int i = 0; i < res.size(); i++)
                {
                    cout << "Date:\t" << res[i].getDate() << "\t" << "Time:\t" << res[i].getTime() << endl;
                }
            }
    }
}

void doFindTempAndHum(Date inputDate, Time inputTime, double &currentTemp, double &currentRelHumidity, vector<AirQuality>& airQualityCollection)
{
    displayTempAndRelHumidity(inputDate, inputTime, currentTemp, currentRelHumidity,airQualityCollection);
    if (currentTemp == 0 && currentRelHumidity == 0)
    {
        cout << "No records found." << endl;
    }
    else
    {
        cout << "At that date and time, temp is: " << currentTemp << ", rel humid is: " << currentRelHumidity << endl;
    }
}
