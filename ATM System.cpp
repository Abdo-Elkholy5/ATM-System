#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <vector>
#include <windows.h>
using namespace std;

enum enMainMenueOptions {
    eQuickWithdraw = 1,
    eNormalWithdraw = 2,
    eDeposite = 3,
    eChecklBalance = 4,
    Logout = 5
};

struct stClientData {
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
};

const string ClientsFileName = "Clients.txt";

stClientData CurrentClient;

void Login();
void MainMenu();

void BackToMainMenu()
{
    cout << "\nPress any key to go back to main menue....";
    system("pause>0");
}

vector <string> SplitString(string str, string delim)
{
    vector <string> Takens;
    int pos;
    string word;
    while ((pos = str.find(delim)) != string::npos)
    {
        word = str.substr(0, pos);
        if (word != "")
        {
            Takens.push_back(word);
        }
        str.erase(0, pos + delim.length());
    }
    if (str != "")
    {
        Takens.push_back(str);
    }
    return Takens;
}

stClientData ConvertLineToRecordForClients(string str, string Seperator = "#//#")
{
    stClientData ClientData;
    vector <string> vstr;
    vstr = SplitString(str, Seperator);
    ClientData.AccountNumber = vstr[0];
    ClientData.PinCode = vstr[1];
    ClientData.Name = vstr[2];
    ClientData.Phone = vstr[3];
    ClientData.AccountBalance = stod(vstr[4]);
    return ClientData;
}

string ConvertRecordToLineForClients(stClientData Client, string Seperator = "#//#")
{
    string stClientRecord = "";
    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);
    return stClientRecord;
}

vector <stClientData> FillLinesFromFileToVectorForClients(string FileName)
{
    fstream MyFile;
    vector <stClientData> vClients;
    MyFile.open(FileName, ios::in);//read Mode
    if (MyFile.is_open())
    {
        string Line;
        while (getline(MyFile, Line))
        {
            vClients.push_back(ConvertLineToRecordForClients(Line));
        }
        MyFile.close();
    }
    return vClients;
}

string Equals(short num)
{
    string us = "";
    for (int i = 1; i <= num; i++)
    {
        us += "=";
    }
    return us;
}

bool CheckExceed(short amount)
{
    if (amount > CurrentClient.AccountBalance)
    {
        cout << "\nThe amount exceeds your Balance.\n";
        cout << "\nPress any key to Continue....";
        system("pause>0");
        return true;
    }
    return false;
}

void QuickWithdrawHeader()
{
    system("cls");
    cout << Equals(36) << endl;
    cout << "            Quick Withdraw Screen\n";
    cout << Equals(36) << endl;
    cout << "        [1] 50        [2] 100 " << endl;
    cout << "        [3] 200       [4] 400 " << endl;
    cout << "        [5] 500       [6] 800 " << endl;
    cout << "        [7] 1000      [8] 2000 " << endl;
    cout << "        [9] Exit        " << endl;
    cout << Equals(36) << endl;
    cout << "Your Balance is " << CurrentClient.AccountBalance << " EGP\n";
}

void printDoneSuccessfully()
{
    cout << "\n\nDone Successfully. New Balance is : " << CurrentClient.AccountBalance << endl << endl;
    BackToMainMenu();
}

void PerformQuickWithdraw(short choice)
{
    int amounts[] = { 50, 100, 200, 400, 500, 800, 1000, 2000 };

    if (choice >= 1 && choice <= 8 && !CheckExceed(amounts[choice - 1])) 
    {
        CurrentClient.AccountBalance -= amounts[choice - 1];
        printDoneSuccessfully();
    }

}

void Quick_Withdraw()
{
    short choice;
    char check;
    QuickWithdrawHeader();
    cout << "Choose what to withdraw from [1] to [8] ? ";
    cin >> choice;

    cout << "\n\nAre you sure you want perform this transaction ? y/n ? ";
    cin >> check;

    if (check == 'y')
    {
        PerformQuickWithdraw(choice);
    }
}

void NormalWithdrawHeader()
{
    system("cls");
    cout << Equals(36) << endl;
    cout << "            Normal Withdraw Screen\n";
    cout << Equals(36) << endl << endl;
}

void Normal_Withdraw()
{
    char check;
    short amount;
    do
    {
        NormalWithdrawHeader();
        cout << "Enter an amount multible of 50's ? ";
        cin >> amount;
    } while (CheckExceed(amount) || amount % 50 != 0);

    cout << "\n\nAre you sure you want perform this transaction ? y/n ? ";
    cin >> check;

    if (check == 'y')
    {
        CurrentClient.AccountBalance -= amount;
        printDoneSuccessfully();
    }
}

void DepositeHeader()
{
    system("cls");
    cout << Equals(36) << endl;
    cout << "            Deposite Screen\n";
    cout << Equals(36) << endl << endl;
}

void Deposit_Choice()
{
    char check;
    short amount;
    do
    {
        DepositeHeader();
        cout << "Enter an amount multible of 50's ? ";
        cin >> amount;
    } while (amount % 50 != 0);

    cout << "\n\nAre you sure you want perform this transaction ? y/n ? ";
    cin >> check;

    if (check == 'y')
    {
        CurrentClient.AccountBalance += amount;
        printDoneSuccessfully();
    }
}

void CheckBalancHeader()
{
    system("cls");
    cout << Equals(36) << endl;
    cout << "            Check Balance Screen\n";
    cout << Equals(36) << endl;
}

void Check_Balance()
{
    CheckBalancHeader();
    cout << "Your Balance is : " << CurrentClient.AccountBalance << "\n\n";
    BackToMainMenu();
}

void SaveClientData()
{
    vector <stClientData> vClients = FillLinesFromFileToVectorForClients(ClientsFileName);
    fstream MyFile;
    MyFile.open(ClientsFileName, ios::out);
    if (MyFile.is_open())
    {
        for (stClientData Line : vClients)
        {
            if (Line.AccountNumber == CurrentClient.AccountNumber)
            {
                MyFile << ConvertRecordToLineForClients(CurrentClient) << endl;
            }
            else
            {
                MyFile << ConvertRecordToLineForClients(Line) << endl;
            }
        }
        MyFile.close();
    }
}

void PerfromMainMenueOption(enMainMenueOptions choice)
{
    switch (choice)
    {
    case eQuickWithdraw:
        Quick_Withdraw();
        MainMenu();
        break;
    case eNormalWithdraw:
        Normal_Withdraw();
        MainMenu();
        break;
    case eDeposite:
        Deposit_Choice();
        MainMenu();
        break;
    case eChecklBalance:
        Check_Balance();
        MainMenu();
        break;
    case Logout:
        SaveClientData();
        Login();
        break;
    default:
        break;
    }
}

void LoginScreenHeader()
{
    system("cls");
    cout << Equals(36) << endl;
    cout << "            Login Screen\n";
    cout << Equals(36) << endl << endl;
}

stClientData ReadLoginData()
{
    stClientData LoginData;
    cout << "Enter Account Number? ";
    cin >> LoginData.AccountNumber;
    cout << "Enter PIN Code? ";
    cin >> LoginData.PinCode;
    return LoginData;
}

bool IsLoginDataExist(stClientData LoginData)
{
    vector <stClientData> vClients = FillLinesFromFileToVectorForClients(ClientsFileName);
    for (stClientData& client : vClients)
    {
        if (LoginData.AccountNumber == client.AccountNumber)
            if (LoginData.PinCode == client.PinCode)
            {
                CurrentClient = client;
                return true;
            }
    }
    return false;
}

void ShowMainMenueScreen()
{
    system("cls");
    cout << "========================================\n";
    cout << "\t   ATM Main Menue Screen\n";
    cout << "========================================\n";
    cout << "\t[1] Quick Withdraw.\n";
    cout << "\t[2] Normal Withdraw.\n";
    cout << "\t[3] Deposite.\n";
    cout << "\t[4] Check Balance.\n";
    cout << "\t[5] Logout.\n";
    cout << "========================================\n";
}

short ReadChoice()
{
    short Number = 0;
    do
    {
        cout << "Choose What do you want to do? [1,5]?" << endl;
        cin >> Number;
    } while (Number < 1 || Number > 5);
    return Number;
}

void MainMenu()
{
    ShowMainMenueScreen();
    enMainMenueOptions choice = enMainMenueOptions(ReadChoice());
    PerfromMainMenueOption(choice);
}

void Login()
{
    stClientData LoginData;
    bool b = false;
    do
    {
        system("cls");
        LoginScreenHeader();
        if (b)
            cout << "Invalid Account Number/PIN Code!\n";
        LoginData = ReadLoginData();
        b = true;
    } while (!IsLoginDataExist(LoginData));

    MainMenu();
}


int main()
{
    
    Login();

    cout << "\n\n\n";
    system("pause>0");
    return 0;
}

