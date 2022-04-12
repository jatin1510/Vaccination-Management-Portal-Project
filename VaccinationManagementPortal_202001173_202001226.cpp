/*

    Author : Harsh Umakant Patel and Jatin Bharatbhai Ranpariya
    Topic : IT-206 Project 
    Title : Vaccination Management Portal

*/

// Admin panel password - DAIICT
// libraries
#include <bits/stdc++.h>
#include <sstream>
#include <fstream>
#include <time.h>
#include <ctime>
#include <windows.h>
using namespace std;

// ******************* class fwd declaration. *********************
class vaccineData;
class customerData;
class StorageClass;
class Center;

// global varibales -  as many functions of different class use them ans are also used in main. So these are kept global

// keeps data of aadhar number and password to validate the login of user.
vector<pair<long long, string>> identity;

// vector of all the objects of the storage class(all the users data.)
vector<StorageClass> Data;

// to compute faster with directly having data of particular ID(object of Storgae class) vaccination - true is taken, false if not then.
vector<pair<bool, bool>> vaccination(2000);

// used to access same object at different location and for better display and proper allignment
int currID = 0, int_day, int_month, int_year, choice, succes = 1;
bool tmp = true, TMP = true;

// **************************************class definition.*********************************************

// Center class for storing data of every center
class Center
{
public:
    Center(){};
    int covaxin = 0, covishield = 0;
    int STOCK1 = 100, STOCK2 = 100;
    // pair of doctor with city wise centers allocated by us. The names of the doctors are taken randomly and with no intention to hurt or make fun of anyone .
    vector<pair<string, string>> citywiseCenter = {
        {"Tapovan Vidhyalaya", "Shrimanta Mandal"},
        {"Ashadeep High School", "Virat Chaudhari"},
        {"N K Thakkar High School", "Jaimin Satani"},
        {"P P Savani High School", "Aanand Tripathi"},
        {"Little flower English School", "Devarsh Nagrecha"},
        {"Divine Buds English School", "Het Patel"},
        {"Bhaipura AMC Library", "Yash vasavda"},
        {"Khokhra Public School", "Archana Nigam"},
        {"Maninagar Urban Health Center", "Nabin Sahu"},
        {"Swaminarayan Temple Maninagar", "Anil Roy"},
    };
    void Data_Display();
};

// each object represents a center from the mentioned 10 centers in citywisecenter
Center obj[10];

// Vaccine data - for vaccine stock and time allocation (1st and 2nd doze)
typedef class vaccineData
{
private:
public:
    vaccineData()
    {
    }
    void stock(int, int);
    string time_allocate();
} VD;

// All the data of customers (Users)
typedef class customerData
{
    // this contains - imp data such has phone number, aadhar number, password, birth-date
protected:
    long long phNO, aadhar_number;
    string password;
    string fname = "", lname = "", bDate = "";
    int age;
    string location1 = "";
    string location2 = "";
    string vacc1 = "", vacc2 = "";
    string date1 = "", date2 = "";
    string drname1 = "";
    string drname2 = "";
    string gender = "";

public:
    customerData() {}
    string demoPass();
    void display();
    int findAge(string);
} CD;

// Main class for program - whole program run based on this class
class StorageClass : public CD, public VD, public Center
{
public:
    void AdminPanel();
    bool generateObject();
    StorageClass(){};
    StorageClass(char);
    bool login(long long, string);
    bool check(long long);
    void vaccine_Data();
    void CertificateDisplay();
    void centerAssign();
    bool eligible(string);
};

//************************************** Extra functions ********************************************

// to save password in clipboard
void toClipboard(HWND hwnd, string &s)
{
    //HWND is a "handle to a window" and is part of the Win32 API . HWNDs are essentially pointers (IntPtr) with values that make them (sort of) point to a window-structure data. In general HWNDs are part an example for applying the ADT model. If you want a Control's HWND see Control.
    OpenClipboard(hwnd);
    EmptyClipboard();
    // A HGLOBAL is an handle on a global memory location. It is used with de memory functions like GlobalLock() wich lock the memory and return a standard pointer on it
    HGLOBAL temp = GlobalAlloc(GMEM_MOVEABLE, s.size() + 1);
    // The GlobalAlloc() function allocates a block of memory from the heap. ... This flag cannot be combined with the GMEM_FIXED flag, which allocates fixed memory. The GMEM_FIXED flag returns a pointer value to the memory block. To access this memory, cast the return value as a pointer.

    if (!temp)
    {
        CloseClipboard();
        return;
    }
    
    memcpy(GlobalLock(temp), s.c_str(), s.size() + 1);
    GlobalUnlock(temp);
    SetClipboardData(CF_TEXT, temp);
    CloseClipboard();
    GlobalFree(temp);
}

// for finding leap year
bool isLeap(int y)
{
    if (y % 100 != 0 && y % 4 == 0 || y % 400 == 0)
    {
        return true;
    }
    return false;
}

// for finding day before current day from starting of the year
int DayBeforeCurrDay(int d, int m, int y)
{
    switch (m - 1)
    {
    case 11:
        d += 30;
    case 10:
        d += 31;
    case 9:
        d += 30;
    case 8:
        d += 31;
    case 7:
        d += 31;
    case 6:
        d += 30;
    case 5:
        d += 31;
    case 4:
        d += 30;
    case 3:
        d += 31;
    case 2:
        d += 28;
    case 1:
        d += 31;
    }

    if (isLeap(y) && m > 2)
        d += 1;

    return d;
}

// Calculate the day after current day to end of the year
void CalculateDate(int finalDate, int y, int *d, int *m)
{
    int month[13] = {0, 31, 28, 31, 30, 31, 30,
                     31, 31, 30, 31, 30, 31};

    if (isLeap(y))
        month[2] = 29;

    int i;
    for (i = 1; i <= 12; i++)
    {
        if (finalDate <= month[i])
            break;
        finalDate = finalDate - month[i];
    }

    *d = finalDate;
    *m = i;
}

// adding desired day into current day
string addDays(int d1, int m1, int y1, int x)
{
    int temp1 = DayBeforeCurrDay(d1, m1, y1);
    int remDays = isLeap(y1) ? (366 - temp1) : (365 - temp1);

    int y2, temp2;
    if (x <= remDays)
    {
        y2 = y1;
        temp2 = temp1 + x;
    }
    else
    {
        x -= remDays;
        y2 = y1 + 1;
        int y2days = isLeap(y2) ? 366 : 365;
        while (x >= y2days)
        {
            x -= y2days;
            y2++;
            y2days = isLeap(y2) ? 366 : 365;
        }
        temp2 = x;
    }
    string op;
    int m2, d2;
    CalculateDate(temp2, y2, &d2, &m2);
    if (d2 <= 9)
    {
        op += "0";
    }
    op += to_string(d2);
    op += "/";
    if (m2 <= 9)
    {
        op += "0";
    }
    op += to_string(m2);
    op += "/";
    op += to_string(y2);

    return op;
}

// Give the size of integer number - for displaying data of vaccine
int size(int var)
{
    int count = 0;
    while (var != 0)
    {
        var /= 10;
        count++;
    }
    return count;
}

// for making program good looking
void space(int space)
{
    for (int i = 0; i < space; i++)
    {
        cout << " ";
    }
}

// for validation of aadhar number of length 12 , we will not allow if it is less or greater than it
bool validateAdharCard(string an)
{
    if (an.length() == 12)
        return true;
    else
        return false;
}

//********************************* Member function of Vaccine Data **********************************

// Time allocation of doze1 and doze2 for user
string VD::time_allocate()
{
    const int MAXLEN = 80;
    char s[MAXLEN];
    time_t t = time(0);
    strftime(s, MAXLEN, "%d/%m/%Y", localtime(&t));
    string day = "", month = "", year = "";
    for (int i = 0; i <= 1; i++)
    {
        day = day + s[i];
        month = month + s[i + 3];
        year = year + s[i + 6];
    }
    year = year + s[8] + s[9];

    if (vaccination[currID].first == 1 && vaccination[currID].second == 0)
    {
        string Output;
        Output += day;
        Output += "/";
        Output += month;
        Output += "/";
        Output += year;
        return Output;
    }
    else if (vaccination[currID].second == 1)
    {
        int_day = stoi(day);
        int_month = stoi(month);
        int_year = stoi(year);
        return addDays(int_day, int_month, int_year, 84);
    }
    return "0";
}

// maintain stock of vaccine, give choice to user which doze he/she want to take
void VD::stock(int index, int doze)
{
    if (doze == 1)
    {
        system("cls");
        cout << "---------------------------------------------------------------------\n";
        cout << "                           Vaccine Choice                            \n";
        cout << "---------------------------------------------------------------------\n\n";
        cout << "\n                            1. Covaxin\n\n     >> Covaxin has been developed by Hyderabad-based Bharat Biotech,\nInternational Ltd in association  with the Indian Council of Medical,\nResearch (ICMR) and the National Institute of Virology (NIV)\n     >> The vaccine received approval from Drug Controller General of\nIndia (DCGI) for Phase I & II Human  Clinical Trials and  an Adaptive\nSeamless  Phase I, Followed  by  Phase  II Randomized,  Double blind,\nMulti centre Study to Evaluate the Safety, Reactogenicity, Tolerabil-\nity  and Immunogenicity of the  Whole-Virion  Inactivated  SARS-CoV-2\nVaccine.\n\n                            2. Covishield\n\n     >> Covishield  has been developed by the Oxford-AstraZeneca  and\nis being manufactured by the Serum Institute of India (SII).\n     >> The Serum Institute of India (SII) and Indian Council of Med-\nical Research are jointly  conducting a Phase II/III, Observer Blind,\nRandomized, Controlled Study to Determine the Safety and Immunogenic-\nity of Covishield (COVID-19 Vaccine)\n\n";
    UP:
        cout << "                        Enter your choice : ";
        cin >> choice;
        if (obj[index].covaxin + obj[index].covishield == (obj[index].STOCK1 + obj[index].STOCK2))
        {
            cout << "\n               --------> Stock is not available <--------\n";
            cout << "                          Better luck tomorrow\n";
            Sleep(3000);
            vaccination[currID].first = false;
            return;
        }
        else
        {
            if (choice == 1)
            {
                if (obj[index].covaxin != obj[index].STOCK1)
                {
                    cout << "\n                      You will be given covaxin\n";
                    obj[index].covaxin++;
                    vaccination[currID].first = true;
                }
                else
                {
                    cout << "                        Try at another center\n";
                    Sleep(3000);
                }
            }
            else if (choice == 2)
            {
                if (obj[index].covishield != obj[index].STOCK2)
                {
                    cout << "\n                      You will be given Covishield\n";
                    obj[index].covishield++;
                    vaccination[currID].first = true;
                }
                else
                {
                    cout << "                        Try at another center\n";
                    Sleep(3000);
                }
            }
            else
            {
                goto UP;
            }
        }
    }
    else if (doze == 2)
    {

        if ((obj[index].STOCK1 - obj[index].covaxin == 0) && (obj[index].STOCK2 - obj[index].covishield == 0))
        {
            cout << "--------- Stock is not available ---------\n            Better luck tomorrow\n";
            vaccination[currID].second = false;
            return;
        }
        else
        {
            if (choice == 1)
            {
                cout << "\n                   You will be given covaxin in second doze\n";
                obj[index].covaxin++;
            }
            else
            {
                cout << "\n                   You will be given Covishield in second doze\n";
                obj[index].covishield++;
            }
            vaccination[currID].second = true;
        }
    }
}

//********************************* Member function of Customer Data **********************************

// finding age of the user by birthdate (which is string)
int CD::findAge(string Bd)
{
    char garbage;
    int birth_date = 0, birth_month = 0, birth_year = 0, current_date = 0, current_month = 0, current_year = 0;
    stringstream birthDate(Bd);
    birthDate >> birth_date >> garbage;
    birthDate >> birth_month >> garbage;
    birthDate >> birth_year;
    const int MAXLEN = 80;
    char s[MAXLEN];
    time_t t = time(0);
    strftime(s, MAXLEN, "%d/%m/%Y", localtime(&t));
    string day = "", month = "", year = "";
    for (int i = 0; i <= 1; i++)
    {
        day = day + s[i];
        month = month + s[i + 3];
        year = year + s[i + 6];
    }
    year = year + s[8] + s[9];
    current_date = stoi(day);
    current_month = stoi(month);
    current_year = stoi(year);
    if (current_month < birth_month)
        return (current_year - birth_year - 1);
    else if (current_month == birth_month)
    {
        if (current_date < birth_date)
            return (current_year - birth_year - 1);
        else
            return (current_year - birth_year);
    }
    else
        return (current_year - birth_year);
}

// For generating random password for user if he/she want
string CD::demoPass()
{
    string pass = "";
    char Choice = '0';
again:
    cout << "\n       >> Would You like to use Suggested Password (y/n) : ";
    cin >> Choice;
    if (Choice == 'y' || Choice == 'Y')
    {
        // set of letters
        string alphabet = "abcdefghijklmnopqrstuvwxyz";
        string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        string s_symbol = "!@#$%&";
        string number = "0123456789";

        int length = 10, key = 0;
        int i = pass.length();
        srand(time(0));
        while (i < 10)
        {
            key = rand() % 26;
            if (key < 6)
                pass += s_symbol[key];
            else if (key >= 6 && key <= 10)
            {
                key = rand() % 10;
                pass += number[key];
            }
            else
            {
                if (key % 2 == 0)
                {
                    key = rand() % 26;
                    pass += alphabet[key];
                }
                else
                {
                    key = rand() % 26;
                    pass += ALPHABET[key];
                }
            }
            i += 1;
        }
    }
    else if (Choice == 'n' || Choice == 'N')
    {
        cout << "       >> Enter Your Password: ";
        cin >> pass;
    }
    else
    {
        cout << "       >> Invalid Choice!.";
        goto again;
    }
    cout << "\n       >> Password has been copied to your Clipboard";
    HWND obj = GetDesktopWindow();
    toClipboard(obj, pass);
    return pass;
}

// display user's data at login screen
void CD::display()
{
    cout << "                   Your name is " << fname << " ";
    cout << lname << endl;
    cout << "                   Phone number : " << phNO << endl;
    cout << "                   Birthdate : " << bDate << endl;
    cout << "                   Age : " << age << endl;
    cout << "                   Gender : " << gender << endl;
}

//********************************* Member function of Center class **************************************

// Admin panel - display data of the all centers
void Center::Data_Display()
{
    cout << "\n            Vaccination Center available in Surat\n\n";
    for (int i = 0; i <= 2; i += 2)
    {
        cout << "Center-" << i + 1 << "                          Center-" << i + 2 << endl;
        cout << obj[i].citywiseCenter[i].first;
        space(34 - obj[i].citywiseCenter[i].first.length());
        cout << obj[i].citywiseCenter[i + 1].first << endl;
        cout << "   Doctor - " << obj[i].citywiseCenter[i].second;
        space(22 - obj[i].citywiseCenter[i].second.length());
        cout << "   Doctor - " << obj[i].citywiseCenter[i + 1].second << endl;
        cout << "Vaccine Available,                Vaccine Available,\n";
        cout << "   Covaxin : " << obj[i].STOCK1 - obj[i].covaxin;
        space(20 - size(obj[i].STOCK1 - obj[i].covaxin));
        cout << "   Covaxin : " << obj[i + 1].STOCK1 - obj[i + 1].covaxin << endl;
        cout << "   Covishield : " << obj[i].STOCK2 - obj[i].covishield;
        space(17 - size(obj[i].STOCK2 - obj[i].covishield));
        cout << "   Covishield : " << obj[i + 1].STOCK2 - obj[i + 1].covishield << endl
             << endl;
    }
    cout << "Center-5\n";
    cout << obj[4].citywiseCenter[4].first << endl;
    cout << "   Doctor - " << obj[4].citywiseCenter[4].second << endl;
    cout << "Vaccine Available,\n";
    cout << "   Covaxin : " << obj[4].STOCK1 - obj[4].covaxin << endl;
    cout << "   Covishield : " << obj[4].STOCK2 - obj[4].covishield << endl;

    cout << "\n";
    cout << "            Vaccination Center available in Ahmedabad\n\n";

    for (int i = 5; i <= 7; i += 2)
    {
        cout << "Center-" << i + 1 << "                          Center-" << i + 2 << endl;
        cout << obj[i].citywiseCenter[i].first;
        space(34 - obj[i].citywiseCenter[i].first.length());
        cout << obj[i].citywiseCenter[i + 1].first << endl;
        cout << "   Doctor - " << obj[i].citywiseCenter[i].second;
        space(22 - obj[i].citywiseCenter[i].second.length());
        cout << "   Doctor - " << obj[i].citywiseCenter[i + 1].second << endl;
        cout << "Vaccine Available,                Vaccine Available,\n";
        cout << "   Covaxin : " << obj[i].STOCK1 - obj[i].covaxin;
        space(20 - size(obj[i].STOCK1 - obj[i].covaxin));
        cout << "   Covaxin : " << obj[i + 1].STOCK1 - obj[i + 1].covaxin << endl;
        cout << "   Covishield : " << obj[i].STOCK2 - obj[i].covishield;
        space(17 - size(obj[i].STOCK2 - obj[i].covishield));
        cout << "   Covishield : " << obj[i + 1].STOCK2 - obj[i + 1].covishield << endl
             << endl;
    }

    cout << "Center-10\n";
    cout << obj[9].citywiseCenter[9].first << endl;
    cout << "   Doctor - " << obj[9].citywiseCenter[9].second << endl;
    cout << "Vaccine Available,\n";
    cout << "   Covaxin : " << obj[9].STOCK1 - obj[9].covaxin << endl;
    cout << "   Covishield : " << obj[9].STOCK2 - obj[9].covishield << endl;
}

//********************************* Member function of StorageClass **************************************

// eligibilty for second doze (return true if period of 84 days is over )
bool StorageClass ::eligible(string VaccDate2)
{
    char garbage;
    int vacc_date = 0, vacc_month = 0, vacc_year = 0, current_date = 0, current_month = 0, current_year = 0;
    stringstream vaccDate(VaccDate2);
    vaccDate >> vacc_date >> garbage;
    vaccDate >> vacc_month >> garbage;
    vaccDate >> vacc_year;
    const int MAXLEN = 80;
    char s[MAXLEN];
    time_t t = time(0);
    strftime(s, MAXLEN, "%d/%m/%Y", localtime(&t));
    string day = "", month = "", year = "";
    for (int i = 0; i <= 1; i++)
    {
        day = day + s[i];
        month = month + s[i + 3];
        year = year + s[i + 6];
    }
    year = year + s[8] + s[9];
    current_date = stoi(day);
    current_month = stoi(month);
    current_year = stoi(year);
    if (current_year - vacc_year < 0)
    {
        return false;
    }
    else if (current_year == vacc_year)
    {
        if (current_month - vacc_month < 0)
            return false;
        else if (current_month == vacc_month && current_date - vacc_date < 0)
            return false;
        else
            return true;
    }
    else
        return true;
}

// User's data of vaccine (when he/she took doze1) and update data in database (doze1 taken or not)
// Center assing
// Time allocating for doze1 and doze2
void StorageClass ::vaccine_Data()
{
    if (vaccination[currID].first == false)
    {
        cout << "                   You have not taken first doze\n\n               Do you want to take first doze (y/n): ";
        char character;
        cin >> character;
        if (character == 'y')
        {
            centerAssign();
            if (choice == 1)
            {
                if (vaccination[currID].first == true)
                {
                    Data[currID].vacc1 = "covaxin";
                }
            }
            else
            {
                if (vaccination[currID].first == true)
                {
                    Data[currID].vacc1 = "covishield";
                }
            }
            if (vaccination[currID].first == true)
            {
                Data[currID].date1 = time_allocate();
                vaccination[currID].second = true;
                Data[currID].date2 = time_allocate();
                vaccination[currID].second = false;
                cout << "               You will be given vaccine on " << Data[currID].date1 << endl;
                cout << "       You will be given 2nd Dose after " << Data[currID].date2 << " (84 days later)\n";
            }
        }
    }
    else
    {
        cout << "\n               Would You like to See Your Certificate(y/n): ";
        char character;
        cin >> character;
        if (character == 'y')
        {
            system("cls");
            cout << endl;
            CertificateDisplay();
            cout << endl;
        }
    }
    if (vaccination[currID].first == true && vaccination[currID].second == false)
    {
        if (!tmp && Data[currID].vacc2 == "")
        {
            cout << "\n\n           Do you want to take second doze (y/n): ";
            tmp = true;
        }
        else
        {
            cout << "\n\n                 Do you want to take second doze (y/n): ";
        }
        char secDoze;
        cin >> secDoze;
        if (secDoze == 'y')
        {
            if (eligible(Data[currID].date2))
            {
                centerAssign();
                Data[currID].vacc2 = Data[currID].vacc1;
                vaccination[currID].second = false;
                Data[currID].date2 = time_allocate();
                vaccination[currID].second = true;
                if (vaccination[currID].second == true)
                {
                    cout << "                   You will be given vaccine on " << Data[currID].date2 << endl;
                }
            }
            else
            {
                if (!TMP)
                {
                    cout << "\n           84 Days Rest Period is not Yet finished.\n            Take Rest and Apply after " << Data[currID].date2;
                    TMP = true;
                }
                else
                {
                    cout << "\n                 84 Days Rest Period is not Yet finished.\n                  Take Rest and Apply after " << Data[currID].date2;
                }
                Sleep(2000);
            }
        }
    }
    else if (vaccination[currID].second == true)
    {
        cout << "            You have taken second doze of ";
        if (Data[currID].vacc2 == "covaxin")
        {
            cout << "covaxin";
        }
        else
        {
            cout << "covishield";
        }
        cout << endl;
        cout << endl;
    }
    else
    {
        cout << "               Responsible Citizen should take Vaccine.\n";
    }
}

// Admin panel - Control center for whole program
void StorageClass ::AdminPanel()
{
    system("cls");
    int CHOICE;
    int flag = 1;
    while (flag)
    {
        system("cls");
        cout << "---------------------------------------------------------------------\n";
        cout << "                          Admin Panel\n";
        cout << "---------------------------------------------------------------------\n";
        cout << "\n                       1. Vaccine Data\n                       2. Registered Users\n                       3. Stock Management\n                       4. Main Menu\n\n";
        cout << "                       Enter your choice: ";
        cin >> CHOICE;
        if (CHOICE == 1)
        {
            cout << "\n                    -----------------------------                    \n";
            cout << "--------------------------- Vaccine Stock ---------------------------\n";
            cout << "                    -----------------------------                    \n";
            Data_Display();
            Sleep(10000);
        }
        else if (CHOICE == 2)
        {
            // int i = 1;
            cout << "\n                    -----------------------------                    \n";
            cout << "-------------------------- Registered Users -------------------------\n";
            cout << "                    -----------------------------                    \n";
            if (Data.size() == 0)
            {
                cout << "\n                         No Registered Users";
                Sleep(2000);
            }
            else
            {
                for (int i = 0; i < Data.size(); i++)
                {
                    cout << "                    User-" << i + 1 << ",\n";
                    cout << "                       Name : " << Data[i].fname << " " << Data[i].lname << endl;
                    cout << "                       Mobile Number : " << Data[i].phNO << endl;
                    cout << "                       Age : " << Data[i].age << endl;
                    cout << "                       BirthDate : " << Data[i].bDate << endl;
                    cout << "                       Gender : " << Data[i].gender << endl;
                    if (Data[i].vacc1 == "")
                    {
                        cout << "                       Doze1 : Not Taken" << endl;
                    }
                    else
                    {
                        cout << "                       Doze1 : Taken" << endl;
                    }
                    if (Data[i].vacc2 == "")
                    {
                        cout << "                       Doze2 : Not Taken" << endl;
                    }
                    else
                    {
                        cout << "                       Doze2 : Taken" << endl;
                    }
                    cout << "\n";
                }
                int sleepTime = Data.size();
                Sleep(2500 * sleepTime);
            }
        }
        else if (CHOICE == 3)
        {
            system("cls");
            cout << "\n                    -----------------------------                    \n";
            cout << "-------------------------- Stock Management -------------------------\n";
            cout << "                    -----------------------------                    \n";
            cout << "\n\n                        1. Surat\n                        2. Ahemdabad\n";
            cout << "\n                    Enter City:";
            int city, center;
            cin >> city;
            if (city == 1)
            {
                cout << "\n             These are the centers available in Surat\n\n                   1. Tapovan Vidhyalaya\n                   2. Ashadeep High School\n                   3. N K Thakkar High School\n                   4. P P Savani High School\n                   5. Little flower English School\n";
                cout << "\n\n                   Enter Center choice: ";
                cin >> center;
                cout << "\n                   Enter STOCK of covaxin : ";
                cin >> obj[center - 1].STOCK1;
                cout << "                   Enter STOCK of covishield : ";
                cin >> obj[center - 1].STOCK2;
            }
            else if (city == 2)
            {
                cout << "\n             These are the centers available in Ahemdabad\n\n                   1. Divine Buds English School\n                   2. Bhaipura AMC Library\n                   3. Khokhra Public School\n                   4. Maninagar Urban Health Center \n                   5. Swaminarayan Temple Maninagar\n";
                cout << "\n\n                   Enter Center choice: ";
                cin >> center;
                cout << "\n                   Enter STOCK of covaxin : ";
                cin >> obj[center + 4].STOCK1;
                cout << "                   Enter STOCK of covishield : ";
                cin >> obj[center + 4].STOCK2;
            }
            else
            {
                cout << "                    Enter Valid choice !!\n";
            }
        }
        else if (CHOICE == 4)
        {
            flag = 0;
        }
        else
        {
            cout << "                      INVALID CHOICE!!!\n";
            Sleep(3000);
        }
    }
}

// Constructor of StorageClass for making object
StorageClass ::StorageClass(char ch)
{
    if (ch == 'y' || ch == 'Y')
    {
    }
    else
    {
        if (generateObject())
        {
            cout << "\n       >> Your password is : "
                 << identity[identity.size() - 1].second << "\n";
            cout << "       >> Successfully";
            cout << " Registered";
            Sleep(1000);
            cout << ".";
            Sleep(1000);
            cout << ".";
            Sleep(1000);
            cout << ".";
            Sleep(1000);
            cout << ".\n";
            succes = 1;
        }
        else
        {
            succes = 0;
        }
    }
}

// Center assignment for user according to their choice (if available)
void StorageClass::centerAssign(void)
{
    system("cls");
    int city, centerChoice;
    cout << "---------------------------------------------------------------------\n";
    cout << "                       Vaccine Center Choice\n";
    cout << "---------------------------------------------------------------------\n\n";
    cout << "\n                           1. Surat\n                           2. Ahemdabad\n\n";
    cout << "                       Enter city : ";
    cin >> city;

    if (city == 1)
    {
        cout << "\n             These are the centers available in Surat\n\n                   1. Tapovan Vidhyalaya\n                   2. Ashadeep High School\n                   3. N K Thakkar High School\n                   4. P P Savani High School\n                   5. Little flower English School\n";
        cout << "\n                   Enter your choice: ";
        cin >> centerChoice;
    }
    else if (city == 2)
    {
        cout << "\n             These are the centers available in Ahemdabad\n\n                   1. Divine Buds English School\n                   2. Bhaipura AMC Library\n                   3. Khokhra Public School\n                   4. Maninagar Urban Health Center \n                   5. Swaminarayan Temple Maninagar\n";
        cout << "\n                       Enter your choice: ";
        cin >> centerChoice;
    }
    if (vaccination[currID].first == false)
    {
        if (city == 1)
        {
            stock(centerChoice - 1, 1);
            if (vaccination[currID].first == true)
            {
                Data[currID].location1 = (citywiseCenter[centerChoice - 1].first) + ", Surat";
                Data[currID].drname1 = citywiseCenter[centerChoice - 1].second;
            }
        }
        else if (city == 2)
        {
            stock(centerChoice + 4, 1);
            if (vaccination[currID].first == true)
            {
                Data[currID].location1 = citywiseCenter[centerChoice + 4].first + ", Ahmedabad";
                Data[currID].drname1 = citywiseCenter[centerChoice + 4].second;
            }
        }
    }
    else
    {
        if (city == 1)
        {
            stock(centerChoice - 1, 2);
            if (vaccination[currID].second == true)
            {
                Data[currID].location2 = citywiseCenter[centerChoice - 1].first + ", Surat";
                Data[currID].drname2 = citywiseCenter[centerChoice - 1].second;
            }
        }
        else if (city == 2)
        {
            stock(centerChoice + 4, 2);
            if (vaccination[currID].second == true)
            {
                Data[currID].location2 = citywiseCenter[centerChoice + 4].first + ", Ahmedabad";
                Data[currID].drname2 = citywiseCenter[centerChoice + 4].second;
            }
        }
    }
}

// Login function - If user is present then return true (based on aadhar number and password)
bool StorageClass::login(long long ai_p, string passi_p)
{
    for (int i = 0; i < identity.size(); i++)
    {
        if (identity[i].first == ai_p)
        {
            if (identity[i].second == passi_p)
            {
                currID = i;
                return true;
            }
        }
    }
    return false;
}

// checking - aadhar card number in the database.
bool StorageClass::check(long long ai_p)
{
    for (int i = 0; i < identity.size(); i++)
    {
        if (identity[i].first == ai_p)
        {
            return true;
        }
    }
    return false;
}

// Generating object of storage class (registration function)
bool StorageClass::generateObject()
{
    system("cls");
    string fn, ln, Bd, gd;
    long long pn, an;
    int calculate_Age;
    cout << "---------------------------------------------------------------------\n";
    cout << "                       Registration Section\n";
    cout << "---------------------------------------------------------------------\n\n\n";
    cout << "       >> Enter First Name : ";
    cin >> fn;
    cout << "       >> Enter Last Name : ";
    cin >> ln;
AGAIN:
    cout << "       >> Enter Aadhar Number : ";
    cin >> an;
    if (!validateAdharCard(to_string(an)))
    {
        cout << "       >> Check your Aadhar number again!!\n";
        Sleep(2000);
        goto AGAIN;
    }
    cout << "       >> Enter BirthDate : ";
    cin >> Bd;
    cout << "       >> Enter Phone Number : ";
    cin >> pn;
    cout << "       >> Enter Gender : ";
    cin >> gd;
    if (check(an))
    {
        cout << "\n      Already Registered!, Go to Login Page.";
        Sleep(2000);
        return false;
    }
    else
    {
        calculate_Age = findAge(Bd);
        fname = fn;
        lname = ln;
        aadhar_number = an;
        bDate = Bd;
        phNO = pn;
        gender = gd;
        age = calculate_Age;
        password = demoPass();
        identity.push_back(make_pair(aadhar_number, password));
        return true;
    }
}

// certificate display of user
void StorageClass ::CertificateDisplay()
{
    system("Color 06");
    int i, j;
    string disp;
    ofstream WriteCert("Certificate.txt");
    for (i = -2; i <= 24; i++)
    {
        WriteCert << "|";
        if (i == -2 || i == 0)
        {
            for (j = 1; j <= 58; j++)
            {
                WriteCert << "*";
            }
        }
        else if (i == -1)
        {
            WriteCert << "     Provisional Certificate for Covid-19 Vaccination     ";
        }
        else if (i == 3)
        {
            for (j = 1; j <= 5; j++)
                WriteCert << " ";
            WriteCert << "Name : " << Data[currID].fname << " " << Data[currID].lname;
            for (j = (12 + Data[currID].fname.length() + 1 + Data[currID].lname.length() + 1); j <= 58; j++)
                WriteCert << " ";
        }
        else if (i == 5)
        {
            for (j = 1; j <= 5; j++)
                WriteCert << " ";
            WriteCert << "BirthDate : " << Data[currID].bDate;
            for (j = (17 + Data[currID].bDate.length() + 1); j <= 58; j++)
                WriteCert << " ";
        }
        else if (i == 7)
        {
            for (j = 1; j <= 5; j++)
                WriteCert << " ";
            WriteCert << "Age : " << Data[currID].age;
            for (j = (11 + (to_string(Data[currID].age).length()) + 1); j <= 58; j++)
                WriteCert << " ";
        }
        else if (i == 9)
        {
            for (j = 1; j <= 5; j++)
                WriteCert << " ";
            WriteCert << "Gender : " << Data[currID].gender;
            for (j = (14 + (Data[currID].gender).length() + 1); j <= 58; j++)
                WriteCert << " ";
        }
        else if (i == 11)
        {
            for (j = 1; j <= 5; j++)
                WriteCert << " ";
            string cut = to_string(Data[currID].aadhar_number);
            WriteCert << "Aadhar number : XXXXXXXX" << cut[cut.length() - 4] << cut[cut.length() - 3] << cut[cut.length() - 2] << cut[cut.length() - 1];
            for (j = (21 + (to_string(Data[currID].aadhar_number)).length() + 1); j <= 58; j++)
                WriteCert << " ";
        }
        else if (i == 13)
        {
            for (j = 1; j <= 5; j++)
                WriteCert << " ";
            WriteCert << "Mobile Number : " << Data[currID].phNO;
            for (j = (22 + (to_string(Data[currID].phNO).length())); j <= 58; j++)
                WriteCert << " ";
        }
        else if (i == 15)
        {
            for (j = 1; j <= 5; j++)
                WriteCert << " ";
            WriteCert << "Vaccine 1 : " << Data[currID].vacc1;
            for (j = (17 + Data[currID].vacc1.length() + 1); j <= 31; j++)
                WriteCert << " ";
            WriteCert << "Date 1 : " << Data[currID].date1;
            for (j = (40 + Data[currID].date1.length() + 1); j <= 58; j++)
                WriteCert << " ";
        }
        else if (i == 16)
        {
            for (j = 1; j <= 5; j++)
                WriteCert << " ";
            WriteCert << "Center 1 : " << Data[currID].location1;
            for (j = (16 + Data[currID].location1.length() + 1); j <= 58; j++)
                WriteCert << " ";
        }
        else if (i == 17)
        {
            for (j = 1; j <= 5; j++)
                WriteCert << " ";
            WriteCert << "Doctor : " << Data[currID].drname1;
            for (j = (14 + Data[currID].drname1.length() + 1); j <= 58; j++)
                WriteCert << " ";
        }
        else if (i == 19 && Data[currID].vacc2 != "")
        {
            for (j = 1; j <= 5; j++)
                WriteCert << " ";
            WriteCert << "Vaccine 2 : " << Data[currID].vacc2;
            for (j = (17 + Data[currID].vacc2.length() + 1); j <= 31; j++)
                WriteCert << " ";
            WriteCert << "Date 2 : " << Data[currID].date2;
            for (j = (40 + Data[currID].date2.length() + 1); j <= 58; j++)
                WriteCert << " ";
        }
        else if (i == 20 && Data[currID].location2 != "")
        {
            for (j = 1; j <= 5; j++)
                WriteCert << " ";
            WriteCert << "Center 2 : " << Data[currID].location2;
            for (j = (16 + Data[currID].location2.length() + 1); j <= 58; j++)
                WriteCert << " ";
        }
        else if (i == 21 && Data[currID].drname2 != "")
        {
            for (j = 1; j <= 5; j++)
                WriteCert << " ";
            WriteCert << "Doctor : " << Data[currID].drname2;
            for (j = (14 + Data[currID].drname2.length() + 1); j <= 58; j++)
                WriteCert << " ";
        }
        else if (i == 24)
        {
            for (j = 1; j <= 58; j++)
            {
                WriteCert << "_";
            }
        }
        else
        {
            if (Data[currID].vacc2 == "" && i == 18)
            {
                i += 4;
            }
            for (j = 1; j <= 58; j++)
            {
                WriteCert << " ";
            }
        }
        WriteCert << "|\n";
    }
    WriteCert.close();

    ifstream ReadCert("Certificate.txt");
    while (getline(ReadCert, disp))
    {
        cout << disp << endl;
    }
    ReadCert.close();
    tmp = false;
    TMP = false;
    Sleep(10000);
    system("Color 07");
}

int main()
{
    system("cls");
    char c;
    int ch, temp = 1, x;
    while (temp == 1)
    {
        // starting of the program
        system("cls");
        cout << "                    -----------------------------                    \n";
        cout << "--------------------Welcome to Vaccination Portal--------------------\n";
        cout << "                    -----------------------------                    \n";
        cout << "\n                       1. Admin Panel\n                       2. User Panel\n                       3. Exit\n";
        cout << "\n                    Enter your Choice: ";
        cin >> x;
        if (x == 1)
        {
            string ADMINPASS = "DAIICT", pass;
        admin:
            cout << "                    Enter password: ";
            cin >> pass;
            if (pass == ADMINPASS)
            {
                cout << "\n";
                cout << "                    Gathering Data.";
                Sleep(800);
                cout << ".";
                Sleep(800);
                cout << ".";
                Sleep(800);
                StorageClass ad;
                ad.AdminPanel(); // calling admin panel
            }
            else
            {
                cout << "\n";
                cout << "                    Wrong Admin Password !!\n";
                goto admin;
            }
        }
        else if (x == 2)
        {
            system("cls");
            cout << "---------------------------------------------------------------------\n";
            cout << "                          User --- Panel\n";
            cout << "---------------------------------------------------------------------\n\n";
            cout << "                            1. Login\n                            2. Register\n                            3. Main Menu\n";
            cout << "\n                          Enter your choice: ";
            cin >> ch;
            if (ch == 1)
            {
                StorageClass s('y');
                long long AN;
                string pass;
                cout << "\n                  Enter your aadhar number: ";
                cin >> AN;
                cout << "                  Enter password: ";
                cin >> pass;
                cout << "\n                  Login In Process.";
                Sleep(500);
                cout << ".";
                Sleep(500);
                cout << ".";
                Sleep(500);
                cout << ".\n";
                bool check = s.login(AN, pass); // if aadhar number and password matches this will return true otherwise false
                if (check)
                {
                    system("cls");
                    cout << "---------------------------------------------------------------------\n";
                    cout << "                          Home --- Page\n";
                    cout << "---------------------------------------------------------------------\n\n";
                    cout << "\n               ~~> Successfully logged in!! <~~\n\n";
                    Data[currID].display(); // display data of user after successfull login
                    s.vaccine_Data();
                }
                else
                {
                    cout << "                  User does not exist !!\n\n                  CHECK YOUR PASSWORD or AADHAR NUMBER";
                }
                Sleep(3000);
            }
            else if (ch == 2)
            {
                StorageClass sc('n'); // registration of user through storage class constructer
                if (succes)
                    Data.push_back(sc); // storing data of only new users in Data vector
            }
            else
            {
            }
        }
        else
        {
            temp = 0;
        }
    }
    return 0;
}