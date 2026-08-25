#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
#include <cstdlib>

#ifdef _WIN32
    #include <conio.h>
#else
    #include <termios.h>
    #include <unistd.h>
    #include <cstdio>
#endif

using namespace std;

#ifndef _WIN32
int getch()
{
    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    int ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif

class Hotel
{
private:
    string hotelName;

    string userName;
    string userPhone;
    string userPassword;
    bool registered;

    string adminPassword;
    string checkoutPassword;
    string upiID;

    bool booked[15];

    string bookingID[15];
    string customerName[15];
    string customerPhone[15];
    string customerAadhar[15];
    string customerPAN[15];

    string checkInDate[15];
    int month[15];
    int daysStay[15];
    int members[15];

    double roomBill[15];
    double foodBill[15];
    double serviceBill[15];

    string roomService[15];

    int rating[15];
    string feedback[15];

    string complaint[15];
    string complaintStatus[15];

    string lostFoundItem[100];
    string lostFoundStatus[100];
    int lostFoundCount;

    string history[100];
    int historyCount;

    bool quizAttempted;
    bool freeBookingAvailable;

    string numberToString(int n)
    {
        stringstream ss;
        ss << n;
        return ss.str();
    }

    // Reads an integer safely. If the user types something that
    // isn't a number, this clears the error and asks again instead
    // of leaving cin stuck (which used to cause an infinite loop of
    // "Invalid Choice!").
    int readInt()
    {
        int value;
        while (!(cin >> value))
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Please enter a number: ";
        }
        return value;
    }

    // Builds a menu option list without needing C++11 brace-init lists,
    // so this compiles fine on older compiler defaults (e.g. Dev-C++ /
    // TDM-GCC with no -std flag set). Usage:
    //   vector<string> opts = makeMenu(5, "One", "Two", "Three", "Four", "Five");
    vector<string> makeMenu(int count, const string &a = "", const string &b = "",
        const string &c = "", const string &d = "", const string &e = "",
        const string &f = "", const string &g = "", const string &h = "",
        const string &i = "", const string &j = "", const string &k = "",
        const string &l = "", const string &m = "", const string &n2 = "",
        const string &o = "", const string &p = "")
    {
        const string all[16] = {a, b, c, d, e, f, g, h, i, j, k, l, m, n2, o, p};
        vector<string> result;
        for (int idx = 0; idx < count; idx++)
            result.push_back(all[idx]);
        return result;
    }

    // Shows options with arrow-key navigation, Firebase-CLI style.
    // Up/Down arrows move the highlight, Enter selects it. Returns a
    // 1-based index, same as what "Enter Choice:" + readInt() used to
    // return, so every existing switch(choice) still works unchanged.
    int selectMenu(vector<string> options, string title)
    {
        int selected = 0;
        int n = (int)options.size();

        while (true)
        {
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            cout << "\n============================================\n";
            cout << title << endl;
            cout << "============================================\n";
            cout << "(Use Up/Down arrows, Enter to select)\n\n";

            for (int i = 0; i < n; i++)
            {
                if (i == selected)
                    cout << " > " << options[i] << "\n";
                else
                    cout << "   " << options[i] << "\n";
            }

            int key = getch();

#ifdef _WIN32
            if (key == 224 || key == 0)
            {
                key = getch();
                if (key == 72) selected = (selected - 1 + n) % n;
                else if (key == 80) selected = (selected + 1) % n;
            }
            else if (key == 13)
            {
                return selected + 1;
            }
#else
            if (key == 27)
            {
                getch();
                int arrow = getch();
                if (arrow == 'A') selected = (selected - 1 + n) % n;
                else if (arrow == 'B') selected = (selected + 1) % n;
            }
            else if (key == 10)
            {
                return selected + 1;
            }
#endif
        }
    }

    double readDouble()
    {
        double value;
        while (!(cin >> value))
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Please enter a number: ";
        }
        return value;
    }

    vector<int> selectMultiMenu(vector<string> options, string title)
    {
        int n = (int)options.size();
        vector<bool> checked(n, false);
        int cursor = 0;

        while (true)
        {
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            cout << "\n============================================\n";
            cout << title << endl;
            cout << "============================================\n";
            cout << "(Up/Down to move, Space to select, Enter to confirm)\n\n";

            for (int i = 0; i < n; i++)
            {
                cout << (i == cursor ? " > " : "   ");
                cout << "[" << (checked[i] ? "x" : " ") << "] " << options[i] << "\n";
            }

            int key = getch();

#ifdef _WIN32
            if (key == 224 || key == 0)
            {
                key = getch();
                if (key == 72) cursor = (cursor - 1 + n) % n;
                else if (key == 80) cursor = (cursor + 1) % n;
            }
            else if (key == ' ')
            {
                checked[cursor] = !checked[cursor];
            }
            else if (key == 13)
            {
                break;
            }
#else
            if (key == 27)
            {
                getch();
                int arrow = getch();
                if (arrow == 'A') cursor = (cursor - 1 + n) % n;
                else if (arrow == 'B') cursor = (cursor + 1) % n;
            }
            else if (key == ' ')
            {
                checked[cursor] = !checked[cursor];
            }
            else if (key == 10)
            {
                break;
            }
#endif
        }

        vector<int> result;
        for (int i = 0; i < n; i++)
            if (checked[i])
                result.push_back(i + 1);

        return result;
    }

    void pauseScreen()
    {
        cout << "\nPress any key to continue...";
        getch();
    }

    bool isValidPhone(const string &phone)
    {
        if (phone.length() != 10)
            return false;
        for (size_t i = 0; i < phone.length(); i++)
            if (!isdigit((unsigned char)phone[i]))
                return false;
        return true;
    }

    bool isValidPassword(const string &pass)
    {
        return pass.length() >= 4 && pass.length() <= 16;
    }

public:
    Hotel()
    {
        registered = false;
        adminPassword = "admin123";
        checkoutPassword = "54321";
        upiID = "hotel@upi";

        lostFoundCount = 0;
        historyCount = 0;
        quizAttempted = false;
        freeBookingAvailable = false;

        for (int i = 0; i < 15; i++)
        {
            booked[i] = false;
            bookingID[i] = "";
            customerName[i] = "";
            customerPhone[i] = "";
            customerAadhar[i] = "";
            customerPAN[i] = "";
            checkInDate[i] = "";
            month[i] = 0;
            daysStay[i] = 0;
            members[i] = 0;
            roomBill[i] = 0;
            foodBill[i] = 0;
            serviceBill[i] = 0;
            roomService[i] = "";
            rating[i] = 0;
            feedback[i] = "";
            complaint[i] = "";
            complaintStatus[i] = "";
        }

        for (int i = 0; i < 100; i++)
        {
            lostFoundItem[i] = "";
            lostFoundStatus[i] = "";
            history[i] = "";
        }
    }

    void enterHotelName()
    {
        cout << "\n============================================\n";
        cout << "          HOTEL BOOKING SYSTEM\n";
        cout << "============================================\n";
        cout << "Enter Hotel Name: ";
        getline(cin, hotelName);
        cout << "\nHotel Name: " << hotelName << endl;
        cout << "Hotel Name Accepted Successfully!\n";
    }

    void registerUser()
    {
        string p, cp;

        cout << "\n========== REGISTER ==========\n";

        cout << "Enter Name: ";
        getline(cin, userName);

        cout << "Enter Phone Number: ";
        cin >> userPhone;

        while (!isValidPhone(userPhone))
        {
            cout << "Invalid Phone Number! Enter exactly 10 digits: ";
            cin >> userPhone;
        }

        cout << "Create Password: ";
        cin >> p;

        while (!isValidPassword(p))
        {
            cout << "Password must be between 4 and 16 characters! Create Password: ";
            cin >> p;
        }

        cout << "Confirm Password: ";
        cin >> cp;

        if (p != cp)
        {
            cout << "Password does not match!\n";
            return;
        }

        userPassword = p;
        registered = true;
        cout << "Registration Successfully Completed!\n";
    }

    void forgotPassword()
    {
        string phone, newPassword, confirmPassword;

        cout << "\n========== FORGOT PASSWORD ==========\n";

        if (!registered)
        {
            cout << "No registered account found.\n";
            return;
        }

        cout << "Enter Registered Phone Number: ";
        cin >> phone;

        if (phone != userPhone)
        {
            cout << "Phone Number Not Found!\n";
            return;
        }

        cout << "Enter New Password: ";
        cin >> newPassword;

        while (!isValidPassword(newPassword))
        {
            cout << "Password must be between 4 and 16 characters! Enter New Password: ";
            cin >> newPassword;
        }

        cout << "Confirm New Password: ";
        cin >> confirmPassword;

        if (newPassword != confirmPassword)
        {
            cout << "Password does not match!\n";
            return;
        }

        userPassword = newPassword;
        cout << "Password Changed Successfully!\n";
    }

    bool login()
    {
        string phone, password;

        if (!registered)
        {
            cout << "\nPlease Register First!\n";
            return false;
        }

        cout << "\n========== LOGIN ==========\n";
        cout << "Enter Phone Number: ";
        cin >> phone;

        cout << "Enter Password: ";
        cin >> password;

        if (phone == userPhone && password == userPassword)
        {
            cout << "Login Successfully!\n";
            return true;
        }

        cout << "Wrong Phone Number or Password!\n";
        return false;
    }

    void roomStatus()
    {
        cout << "\n========== ROOM STATUS ==========\n";

        for (int i = 0; i < 15; i++)
        {
            cout << "Room " << i + 1 << " : ";
            if (booked[i])
                cout << "Booked";
            else
                cout << "Available";
            cout << endl;
        }
    }

    double getRoomPrice(int people, int monthNumber)
    {
        double price;

        if (people <= 2)
            price = 1000;
        else if (people <= 4)
            price = 1500;
        else
            price = 2000;

        if (monthNumber == 12 || monthNumber == 1)
            price += 300;

        if (monthNumber == 4 || monthNumber == 5)
            price += 200;

        return price;
    }

    void roomRecommendation()
    {
        int people;
        double budget;
        double price;

        cout << "\n========== ROOM RECOMMENDATION ==========\n";
        cout << "Enter Number of Members: ";
        people = readInt();

        cout << "Enter Maximum Budget Per Day: Rs. ";
        budget = readDouble();

        if (people <= 0 || budget <= 0)
        {
            cout << "Invalid Input!\n";
            return;
        }

        price = getRoomPrice(people, 0);

        cout << "Recommended Price: Rs. " << price << " per day\n";

        if (budget >= price)
            cout << "Status: Suitable for your budget!\n";
        else
            cout << "Status: Budget is lower than recommended price.\n";
    }

    void quizTask()
    {
        int answer;
        int score = 0;

        cout << "\n========== FREE BOOKING QUIZ ==========\n";

        if (quizAttempted)
        {
            cout << "You have already attempted the quiz.\n";
            return;
        }

        quizAttempted = true;

        cout << "\nQ1. Which language is used in this project?\n";
        cout << "1. Java\n2. C++\n3. Python\n4. HTML\n";
        cout << "Answer: ";
        answer = readInt();
        if (answer == 2) score++;

        cout << "\nQ2. How many rooms are available?\n";
        cout << "1. 10\n2. 12\n3. 15\n4. 20\n";
        cout << "Answer: ";
        answer = readInt();
        if (answer == 3) score++;

        cout << "\nQ3. Which header is used for string?\n";
        cout << "1. <string>\n2. <iomanip>\n3. <cstdlib>\n4. <ctime>\n";
        cout << "Answer: ";
        answer = readInt();
        if (answer == 1) score++;

        cout << "\nQ4. Which object is used for output?\n";
        cout << "1. cin\n2. cout\n3. scanf\n4. input\n";
        cout << "Answer: ";
        answer = readInt();
        if (answer == 2) score++;

        cout << "\nQ5. Which keyword is used to create a class?\n";
        cout << "1. object\n2. function\n3. class\n4. create\n";
        cout << "Answer: ";
        answer = readInt();
        if (answer == 3) score++;

        cout << "\nYour Score: " << score << "/5\n";

        if (score == 5)
        {
            freeBookingAvailable = true;
            cout << "CONGRATULATIONS! One free room booking is available.\n";
        }
        else
        {
            cout << "Sorry! You need 5/5 for free booking.\n";
        }
    }

    void bookRoom()
    {
        int room;

        roomStatus();

        cout << "\nEnter Room Number (1-15): ";
        room = readInt();

        if (room < 1 || room > 15)
        {
            cout << "Invalid Room Number!\n";
            return;
        }

        if (booked[room - 1])
        {
            cout << "Room Already Booked!\n";
            return;
        }

        cin.ignore();

        cout << "\n========== CUSTOMER DETAILS ==========\n";

        cout << "Enter Customer Name: ";
        getline(cin, customerName[room - 1]);

        cout << "Enter Phone Number: ";
        getline(cin, customerPhone[room - 1]);

        while (!isValidPhone(customerPhone[room - 1]))
        {
            cout << "Invalid Phone Number! Enter exactly 10 digits: ";
            getline(cin, customerPhone[room - 1]);
        }

        cout << "Enter Aadhar Number: ";
        getline(cin, customerAadhar[room - 1]);

        cout << "Enter PAN Number: ";
        getline(cin, customerPAN[room - 1]);

        cout << "How Many Members: ";
        members[room - 1] = readInt();

        if (members[room - 1] <= 0)
        {
            cout << "Invalid Number of Members!\n";
            return;
        }

        cout << "Enter Check-in Date (e.g. 18-08-2026): ";
        getline(cin, checkInDate[room - 1]);

        cout << "Enter Month Number (1-12): ";
        month[room - 1] = readInt();

        if (month[room - 1] < 1 || month[room - 1] > 12)
        {
            cout << "Invalid Month!\n";
            return;
        }

        cout << "Number of Days Stay: ";
        daysStay[room - 1] = readInt();

        if (daysStay[room - 1] <= 0)
        {
            cout << "Invalid Number of Days!\n";
            return;
        }

        double price = getRoomPrice(members[room - 1], month[room - 1]);

        char useFree = 'N';

        if (freeBookingAvailable)
        {
            cout << "\nFREE BOOKING AVAILABLE!\n";
            cout << "Use Free Booking? (Y/N): ";
            cin >> useFree;
        }

        if (freeBookingAvailable && (useFree == 'Y' || useFree == 'y'))
        {
            roomBill[room - 1] = 0;
            freeBookingAvailable = false;
        }
        else
        {
            roomBill[room - 1] = price * daysStay[room - 1];
        }

        bookingID[room - 1] = "HTL2026R" + numberToString(room);
        booked[room - 1] = true;

        foodBill[room - 1] = 0;
        serviceBill[room - 1] = 0;
        roomService[room - 1] = "None";
        complaint[room - 1] = "None";
        complaintStatus[room - 1] = "None";

        if (historyCount < 100)
        {
            history[historyCount] =
                bookingID[room - 1] +
                " - Room " + numberToString(room) +
                " - " + customerName[room - 1];
            historyCount++;
        }

        cout << "\n========== BOOKING SUCCESSFUL ==========\n";
        cout << "Hotel: " << hotelName << endl;
        cout << "Booking ID: " << bookingID[room - 1] << endl;
        cout << "Customer: " << customerName[room - 1] << endl;
        cout << "Room Number: " << room << endl;
        cout << "Members: " << members[room - 1] << endl;
        cout << "Check-in Date: " << checkInDate[room - 1] << endl;
        cout << "Days Stay: " << daysStay[room - 1] << endl;
        cout << "Room Price/Day: Rs. " << price << endl;
        cout << "Room Bill: Rs. " << roomBill[room - 1] << endl;
    }

    void foodOrder()
    {
        int room;

        cout << "\n========== FOOD ORDER ==========\n";
        cout << "Enter Room Number: ";
        room = readInt();

        if (room < 1 || room > 15 || !booked[room - 1])
        {
            cout << "Invalid or Unbooked Room!\n";
            return;
        }

        string names[10] = {"Rice", "Chicken", "Dal", "Egg Curry", "Tea",
            "Roti", "Paneer", "Fish Curry", "Coffee", "Cold Drink"};
        double prices[10] = {50, 120, 40, 80, 20, 10, 150, 180, 40, 50};

        vector<string> menu = makeMenu(10, "Rice - Rs. 50", "Chicken - Rs. 120",
            "Dal - Rs. 40", "Egg Curry - Rs. 80", "Tea - Rs. 20", "Roti - Rs. 10",
            "Paneer - Rs. 150", "Fish Curry - Rs. 180", "Coffee - Rs. 40",
            "Cold Drink - Rs. 50");

        vector<int> selected = selectMultiMenu(menu, "FOOD ORDER - Room " + numberToString(room));

        if (selected.empty())
        {
            cout << "No Items Selected.\n";
            return;
        }

        double orderTotal = 0;

        for (size_t s = 0; s < selected.size(); s++)
        {
            int idx = selected[s] - 1;
            int quantity;

            cout << "\nEnter Quantity for " << names[idx] << ": ";
            quantity = readInt();

            if (quantity <= 0)
            {
                cout << "Invalid Quantity, Skipping " << names[idx] << ".\n";
                continue;
            }

            double total = prices[idx] * quantity;
            foodBill[room - 1] += total;
            orderTotal += total;

            cout << names[idx] << " x " << quantity << " = Rs. " << total << endl;
        }

        cout << "\nOrder Total: Rs. " << orderTotal << endl;
        cout << "Current Food Bill: Rs. " << foodBill[room - 1] << endl;
        cout << "Food Order Completed!\n";
    }

    void roomServiceRequest()
    {
        int room;

        cout << "\n========== ROOM SERVICE ==========\n";
        cout << "Enter Room Number: ";
        room = readInt();

        if (room < 1 || room > 15 || !booked[room - 1])
        {
            cout << "Invalid or Unbooked Room!\n";
            return;
        }

        string names[4] = {"Room Cleaning", "Extra Bed", "Mineral Water", "Maintenance"};
        double prices[4] = {100, 500, 30, 0};

        vector<string> menu = makeMenu(4, "Room Cleaning - Rs. 100", "Extra Bed - Rs. 500",
            "Mineral Water - Rs. 30", "Maintenance - Rs. 0");

        vector<int> selected = selectMultiMenu(menu, "ROOM SERVICE - Room " + numberToString(room));

        if (selected.empty())
        {
            cout << "No Service Selected.\n";
            return;
        }

        // Append instead of overwrite, so earlier requests for this
        // stay aren't lost when a new one is made.
        for (size_t s = 0; s < selected.size(); s++)
        {
            int idx = selected[s] - 1;
            serviceBill[room - 1] += prices[idx];

            if (roomService[room - 1] == "" || roomService[room - 1] == "None")
                roomService[room - 1] = names[idx];
            else
                roomService[room - 1] += ", " + names[idx];
        }

        cout << "Room Service Request Submitted!\n";
    }

    void payment()
    {
        int room, choice;

        cout << "\n========== PAYMENT ==========\n";
        cout << "Enter Room Number: ";
        room = readInt();

        if (room < 1 || room > 15 || !booked[room - 1])
        {
            cout << "Invalid or Unbooked Room!\n";
            return;
        }

        cout << "1. Cash\n";
        cout << "2. UPI\n";
        cout << "3. Card\n";
        cout << "4. Online Payment\n";
        cout << "Select Payment Method: ";
        choice = readInt();

        switch (choice)
        {
            case 1:
                cout << "Payment Method: Cash\n";
                break;
            case 2:
                cout << "Payment Method: UPI\n";
                cout << "UPI ID: " << upiID << endl;
                break;
            case 3:
                cout << "Payment Method: Card\n";
                break;
            case 4:
                cout << "Payment Method: Online Payment\n";
                break;
            default:
                cout << "Invalid Payment Method!\n";
        }
    }

    // Prints the bill for a room that's already been validated as
    // booked. Shared by receipt() (which asks the user for a room
    // number first) and checkout() (which already has one) so the
    // room is never asked for twice.
    void printReceipt(int room)
    {
        double total =
            roomBill[room - 1] +
            foodBill[room - 1] +
            serviceBill[room - 1];

        cout << fixed << setprecision(2);

        cout << "\n============================================\n";
        cout << "              " << hotelName << endl;
        cout << "                  RECEIPT\n";
        cout << "============================================\n";
        cout << "Booking ID    : " << bookingID[room - 1] << endl;
        cout << "Customer Name : " << customerName[room - 1] << endl;
        cout << "Phone Number  : " << customerPhone[room - 1] << endl;
        cout << "Room Number   : " << room << endl;
        cout << "Members       : " << members[room - 1] << endl;
        cout << "Check-in Date : " << checkInDate[room - 1] << endl;
        cout << "Days Stay     : " << daysStay[room - 1] << endl;
        cout << "--------------------------------------------\n";
        cout << "Room Bill     : Rs. " << roomBill[room - 1] << endl;
        cout << "Food Bill     : Rs. " << foodBill[room - 1] << endl;
        cout << "Service Bill  : Rs. " << serviceBill[room - 1] << endl;
        cout << "--------------------------------------------\n";
        cout << "Grand Total   : Rs. " << total << endl;
        cout << "============================================\n";
        cout << "          THANK YOU FOR VISITING!\n";
        cout << "============================================\n";
    }

    void receipt()
    {
        int room;

        cout << "\n========== RECEIPT ==========\n";
        cout << "Enter Room Number: ";
        room = readInt();

        if (room < 1 || room > 15 || !booked[room - 1])
        {
            cout << "Invalid or Unbooked Room!\n";
            return;
        }

        printReceipt(room);
    }

    void extendStay()
    {
        int room, extraDays;

        cout << "\n========== EXTEND STAY ==========\n";
        cout << "Enter Room Number: ";
        room = readInt();

        if (room < 1 || room > 15 || !booked[room - 1])
        {
            cout << "Invalid or Unbooked Room!\n";
            return;
        }

        cout << "Current Stay: " << daysStay[room - 1] << " days\n";
        cout << "Enter Additional Days: ";
        extraDays = readInt();

        if (extraDays <= 0)
        {
            cout << "Invalid Days!\n";
            return;
        }

        if (roomBill[room - 1] == 0)
        {
            daysStay[room - 1] += extraDays;
            cout << "Stay Extended Successfully!\n";
            cout << "Room Bill remains FREE.\n";
            return;
        }

        double pricePerDay =
            roomBill[room - 1] / daysStay[room - 1];

        daysStay[room - 1] += extraDays;
        roomBill[room - 1] =
            pricePerDay * daysStay[room - 1];

        cout << "Stay Extended Successfully!\n";
        cout << "New Stay: " << daysStay[room - 1] << " days\n";
        cout << "Updated Room Bill: Rs. " << roomBill[room - 1] << endl;
    }

    void cancelBooking()
    {
        int room;

        cout << "\n========== CANCEL BOOKING ==========\n";
        cout << "Enter Room Number: ";
        room = readInt();

        if (room < 1 || room > 15 || !booked[room - 1])
        {
            cout << "Invalid or Unbooked Room!\n";
            return;
        }

        // Only allow cancelling before any food/service has been
        // added or paid for, so this can't be used to dodge charges.
        if (foodBill[room - 1] > 0 || serviceBill[room - 1] > 0)
        {
            cout << "Cannot cancel: charges already added to this room.\n";
            cout << "Please use Checkout instead.\n";
            return;
        }

        cout << "Customer: " << customerName[room - 1] << endl;
        cout << "Are you sure you want to cancel this booking? (Y/N): ";
        char confirm;
        cin >> confirm;

        if (confirm != 'Y' && confirm != 'y')
        {
            cout << "Cancellation aborted.\n";
            return;
        }

        if (historyCount < 100)
        {
            history[historyCount] =
                bookingID[room - 1] +
                " - Room " + numberToString(room) +
                " - Cancelled";
            historyCount++;
        }

        booked[room - 1] = false;
        bookingID[room - 1] = "";
        customerName[room - 1] = "";
        customerPhone[room - 1] = "";
        customerAadhar[room - 1] = "";
        customerPAN[room - 1] = "";
        checkInDate[room - 1] = "";
        month[room - 1] = 0;
        daysStay[room - 1] = 0;
        members[room - 1] = 0;
        roomBill[room - 1] = 0;
        roomService[room - 1] = "";
        complaint[room - 1] = "";
        complaintStatus[room - 1] = "";

        cout << "Booking Cancelled Successfully.\n";
    }

    void checkout()
    {
        int room;
        string password;

        cout << "\n========== CHECKOUT ==========\n";
        cout << "Enter Room Number: ";
        room = readInt();

        if (room < 1 || room > 15 || !booked[room - 1])
        {
            cout << "Invalid or Unbooked Room!\n";
            return;
        }

        cout << "Customer: " << customerName[room - 1] << endl;
        cout << "Enter Checkout Password: ";
        cin >> password;

        if (password != checkoutPassword)
        {
            cout << "Wrong Checkout Password!\n";
            return;
        }

        printReceipt(room);

        booked[room - 1] = false;

        customerName[room - 1] = "";
        customerPhone[room - 1] = "";
        customerAadhar[room - 1] = "";
        customerPAN[room - 1] = "";
        checkInDate[room - 1] = "";

        month[room - 1] = 0;
        daysStay[room - 1] = 0;
        members[room - 1] = 0;

        roomBill[room - 1] = 0;
        foodBill[room - 1] = 0;
        serviceBill[room - 1] = 0;

        roomService[room - 1] = "";
        complaint[room - 1] = "";
        complaintStatus[room - 1] = "";
        rating[room - 1] = 0;
        feedback[room - 1] = "";

        if (historyCount < 100)
        {
            history[historyCount] =
                bookingID[room - 1] +
                " - Room " + numberToString(room) +
                " - Checked Out";
            historyCount++;
        }

        bookingID[room - 1] = "";

        cout << "Checkout Successfully Completed!\n";
        cout << "Room is now Available.\n";
    }

    void feedbackSystem()
    {
        int room;

        cout << "\n========== CUSTOMER FEEDBACK ==========\n";
        cout << "Enter Room Number: ";
        room = readInt();

        if (room < 1 || room > 15 || !booked[room - 1])
        {
            cout << "Invalid or Unbooked Room!\n";
            return;
        }

        cout << "Rating (1-5): ";
        rating[room - 1] = readInt();

        if (rating[room - 1] < 1 || rating[room - 1] > 5)
        {
            cout << "Invalid Rating!\n";
            return;
        }

        cin.ignore();

        cout << "Enter Feedback: ";
        getline(cin, feedback[room - 1]);

        cout << "Thank you for your feedback!\n";
    }

    void complaintSystem()
    {
        int room;

        cout << "\n========== COMPLAINT ==========\n";
        cout << "Enter Room Number: ";
        room = readInt();

        if (room < 1 || room > 15 || !booked[room - 1])
        {
            cout << "Invalid or Unbooked Room!\n";
            return;
        }

        cin.ignore();

        cout << "Enter Complaint: ";
        getline(cin, complaint[room - 1]);

        complaintStatus[room - 1] = "Pending";

        cout << "Complaint Submitted Successfully!\n";
    }

    void lostAndFound()
    {
        int choice;

        cout << "\n========== LOST AND FOUND ==========\n";
        cout << "1. Report Lost Item\n";
        cout << "2. View Lost Items\n";
        cout << "3. Back\n";
        cout << "Enter Choice: ";
        choice = readInt();

        if (choice == 1)
        {
            if (lostFoundCount >= 100)
            {
                cout << "Storage Full!\n";
                return;
            }

            cin.ignore();

            cout << "Enter Lost Item Name: ";
            getline(cin, lostFoundItem[lostFoundCount]);

            lostFoundStatus[lostFoundCount] = "Reported";
            lostFoundCount++;

            cout << "Lost Item Reported Successfully!\n";
        }
        else if (choice == 2)
        {
            if (lostFoundCount == 0)
            {
                cout << "No Lost Items Found.\n";
                return;
            }

            for (int i = 0; i < lostFoundCount; i++)
            {
                cout << i + 1 << ". "
                     << lostFoundItem[i]
                     << " - "
                     << lostFoundStatus[i]
                     << endl;
            }
        }
    }

    void bookingHistory()
    {
        cout << "\n========== BOOKING HISTORY ==========\n";

        if (historyCount == 0)
        {
            cout << "No Booking History Found.\n";
            return;
        }

        for (int i = 0; i < historyCount; i++)
        {
            cout << i + 1 << ". " << history[i] << endl;
        }
    }

    bool adminLogin()
    {
        string password;

        cout << "\n========== ADMIN LOGIN ==========\n";
        cout << "Enter Admin Password: ";
        cin >> password;

        if (password == adminPassword)
        {
            cout << "Admin Login Successfully!\n";
            return true;
        }

        cout << "Wrong Admin Password!\n";
        return false;
    }

    void changeAdminPassword()
    {
        string oldPassword, newPassword, confirmPassword;

        cout << "\n========== CHANGE ADMIN PASSWORD ==========\n";

        cout << "Enter Current Admin Password: ";
        cin >> oldPassword;

        if (oldPassword != adminPassword)
        {
            cout << "Wrong Current Admin Password!\n";
            return;
        }

        cout << "Enter New Admin Password: ";
        cin >> newPassword;

        cout << "Confirm New Password: ";
        cin >> confirmPassword;

        if (newPassword != confirmPassword)
        {
            cout << "Password does not match!\n";
            return;
        }

        adminPassword = newPassword;
        cout << "Admin Password Changed Successfully!\n";
    }

    void changeCheckoutPassword()
    {
        string oldPassword, newPassword, confirmPassword;

        cout << "\n========== CHANGE CHECKOUT PASSWORD ==========\n";

        cout << "Enter Current Checkout Password: ";
        cin >> oldPassword;

        if (oldPassword != checkoutPassword)
        {
            cout << "Wrong Current Checkout Password!\n";
            return;
        }

        cout << "Enter New Checkout Password: ";
        cin >> newPassword;

        cout << "Confirm New Password: ";
        cin >> confirmPassword;

        if (newPassword != confirmPassword)
        {
            cout << "Password does not match!\n";
            return;
        }

        checkoutPassword = newPassword;
        cout << "Checkout Password Changed Successfully!\n";
    }

    void viewComplaints()
    {
        bool found = false;

        cout << "\n========== CUSTOMER COMPLAINTS ==========\n";

        for (int i = 0; i < 15; i++)
        {
            if (complaint[i] != "" && complaint[i] != "None")
            {
                found = true;

                cout << "\nRoom: " << i + 1 << endl;
                cout << "Customer: " << customerName[i] << endl;
                cout << "Complaint: " << complaint[i] << endl;
                cout << "Status: " << complaintStatus[i] << endl;
            }
        }

        if (!found)
            cout << "No Complaints Found.\n";
    }

    void viewFeedback()
    {
        bool found = false;

        cout << "\n========== CUSTOMER FEEDBACK ==========\n";

        for (int i = 0; i < 15; i++)
        {
            if (feedback[i] != "")
            {
                found = true;

                cout << "\nRoom: " << i + 1 << endl;
                cout << "Customer: " << customerName[i] << endl;
                cout << "Rating: " << rating[i] << "/5" << endl;
                cout << "Feedback: " << feedback[i] << endl;
            }
        }

        if (!found)
            cout << "No Feedback Found.\n";
    }

    void adminPanel()
    {
        if (!adminLogin())
            return;

        int choice;

        do
        {
            choice = selectMenu(
                makeMenu(8, "Room Status", "Booking History", "Change Admin Password",
                    "Change Checkout Password", "Lost & Found", "View Complaints",
                    "View Feedback", "Logout"),
                "ADMIN PANEL"
            );

            switch (choice){
                case 1:
                    roomStatus();
                    break;
                case 2:
                    bookingHistory();
                    break;
                case 3:
                    changeAdminPassword();
                    break;
                case 4:
                    changeCheckoutPassword();
                    break;
                case 5:
                    lostAndFound();
                    break;
                case 6:
                    viewComplaints();
                    break;
                case 7:
                    viewFeedback();
                    break;
                case 8:
                    cout << "Admin Logged Out.\n";
                    break;
                default:
                    cout << "Invalid Choice!\n";
            }

            if (choice != 8)
                pauseScreen();

        } while (choice != 8);
    }

    void customerMenu()
    {
        int choice;

        do
        {
            choice = selectMenu(
                makeMenu(16, "Room Status", "Room Recommendation", "Book Room", "Food Order",
                    "Room Service", "Payment", "Receipt", "Extend Stay", "Checkout",
                    "Feedback", "Complaint", "Lost & Found", "Booking History",
                    "Free Booking Quiz", "Cancel Booking", "Logout"),
                "CUSTOMER MENU"
            );

            switch (choice)
            {
                case 1:
                    roomStatus();
                    break;
                case 2:
                    roomRecommendation();
                    break;
                case 3:
                    bookRoom();
                    break;
                case 4:
                    foodOrder();
                    break;
                case 5:
                    roomServiceRequest();
                    break;
                case 6:
                    payment();
                    break;
                case 7:
                    receipt();
                    break;
                case 8:
                    extendStay();
                    break;
                case 9:
                    checkout();
                    break;
                case 10:
                    feedbackSystem();
                    break;
                case 11:
                    complaintSystem();
                    break;
                case 12:
                    lostAndFound();
                    break;
                case 13:
                    bookingHistory();
                    break;
                case 14:
                    quizTask();
                    break;
                case 15:
                    cancelBooking();
                    break;
                case 16:
                    cout << "Customer Logged Out.\n";
                    break;
                default:
                    cout << "Invalid Choice!\n";
            }

            if (choice != 16)
                pauseScreen();

        } while (choice != 16);
    }

    void run()
    {
        int choice;

        enterHotelName();

        do
        {
            choice = selectMenu(
                makeMenu(5, "Register", "Login", "Forgot Password", "Admin Panel", "Exit"),
                hotelName + "\n          HOTEL BOOKING SYSTEM"
            );

            switch (choice)
            {
                case 1:
                    registerUser();
                    break;
                case 2:
                    if (login())
                        customerMenu();
                    break;
                case 3:
                    forgotPassword();
                    break;
                case 4:
                    adminPanel();
                    break;
                case 5:
                    cout << "\nTHANK YOU FOR USING HOTEL SYSTEM\n";
                    break;
                default:
                    cout << "Invalid Choice!\n";
            }

            if (choice != 5)
                pauseScreen();

        } while (choice != 5);
    }
};

int main()
{
    Hotel hotel;
    hotel.run();

    return 0;
}
