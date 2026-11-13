#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
#include <cstdlib>

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#else
    #include <termios.h>
    #include <unistd.h>
    #include <cstdio>
#endif

#ifdef _WIN32
// Use the underscored CRT function on Windows. It avoids the getch()
// compatibility issue that can trigger the "no current window" popup
// in some MinGW/Dev-C++ console setups.
inline int readKey()
{
    return _getch();
}
#endif

using namespace std;

#ifndef _WIN32
int readKey()
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

// ============================================================
//  UI LAYER  (ANSI colours, boxes, banners, messages)
//  Pure presentation - no hotel logic lives in here.
// ============================================================
namespace ui
{
    const string RESET   = "\033[0m";
    const string BOLD    = "\033[1m";
    const string DIM     = "\033[2m";
    const string REV     = "\033[7m";

    const string RED     = "\033[91m";
    const string GREEN   = "\033[92m";
    const string YELLOW  = "\033[93m";
    const string BLUE    = "\033[94m";
    const string MAGENTA = "\033[95m";
    const string CYAN    = "\033[96m";
    const string WHITE   = "\033[97m";
    const string GREY    = "\033[90m";

    const string BG_CYAN = "\033[46m\033[30m";   // cyan bar, black text

    const int W = 56;   // inner width of boxes

    inline void enableAnsi()
    {
#ifdef _WIN32
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD mode = 0;
        if (GetConsoleMode(h, &mode))
            SetConsoleMode(h, mode | 0x0004); // ENABLE_VIRTUAL_TERMINAL_PROCESSING
#endif
    }

    inline string repeat(const string &s, int n)
    {
        string r;
        for (int i = 0; i < n; i++) r += s;
        return r;
    }

    inline string padRight(const string &s, int width)
    {
        string r = s;
        while ((int)r.length() < width) r += " ";
        return r;
    }

    inline string center(const string &s, int width)
    {
        int len = (int)s.length();
        if (len >= width) return s;
        int left = (width - len) / 2;
        int right = width - len - left;
        return repeat(" ", left) + s + repeat(" ", right);
    }

    inline void clearScreen()
    {
        cout << "\033[2J\033[H";
        cout.flush();
    }

    inline void hideCursor() { cout << "\033[?25l"; cout.flush(); }
    inline void showCursor() { cout << "\033[?25h"; cout.flush(); }

    inline void borderTop(const string &color)
    {
        cout << color << "+" << repeat("=", W) << "+" << RESET << "\n";
    }

    inline void borderMid(const string &color)
    {
        cout << color << "+" << repeat("-", W) << "+" << RESET << "\n";
    }

    inline void borderBottom(const string &color)
    {
        cout << color << "+" << repeat("=", W) << "+" << RESET << "\n";
    }

    // One line inside a box: | text |
    inline void boxLine(const string &text, const string &borderColor,
                        const string &textColor = "", bool centered = false)
    {
        string body = centered ? center(text, W - 2) : padRight(text, W - 2);
        cout << borderColor << "|" << RESET << " "
             << textColor << body << RESET << " "
             << borderColor << "|" << RESET << "\n";
    }

    // Big title banner
    inline void banner(const string &title, const string &subtitle = "",
                       const string &color = CYAN)
    {
        cout << "\n";
        borderTop(color);
        boxLine("", color);
        boxLine(title, color, BOLD + WHITE, true);
        if (subtitle != "")
            boxLine(subtitle, color, YELLOW, true);
        boxLine("", color);
        borderBottom(color);
    }

    // Section header used at the top of each feature
    inline void header(const string &title)
    {
        cout << "\n" << CYAN << BOLD << "  >> " << title << RESET << "\n";
        cout << GREY << "  " << repeat("-", W - 2) << RESET << "\n\n";
    }

    inline void ok(const string &msg)
    {
        cout << GREEN << BOLD << "  [ OK ] " << RESET << GREEN << msg << RESET << "\n";
    }

    inline void err(const string &msg)
    {
        cout << RED << BOLD << "  [ !! ] " << RESET << RED << msg << RESET << "\n";
    }

    inline void warn(const string &msg)
    {
        cout << YELLOW << BOLD << "  [ !  ] " << RESET << YELLOW << msg << RESET << "\n";
    }

    inline void info(const string &msg)
    {
        cout << CYAN << "  [ i  ] " << RESET << msg << "\n";
    }

    inline void line(const string &label, const string &value,
                     const string &valueColor = "")
    {
        cout << "  " << GREY << padRight(label, 16) << RESET
             << ": " << valueColor << value << RESET << "\n";
    }

    // Prompt that is followed by cin / getline
    inline void prompt(const string &text)
    {
        cout << YELLOW << "  > " << RESET << BOLD << text << RESET;
        cout.flush();
    }

    inline void numbered(int n, const string &text)
    {
        cout << "  " << CYAN << BOLD << n << "." << RESET << " " << text << "\n";
    }
}

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

    string moneyToString(double v)
    {
        stringstream ss;
        ss << fixed << setprecision(2) << v;
        return ss.str();
    }

    // Reads an integer safely. If the user types something that
    // isn't a number, this clears the error and asks again instead
    // of leaving cin stuck.
    int readInt()
    {
        int value;
        while (!(cin >> value))
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << ui::RED << "  Please enter a number: " << ui::RESET;
        }
        return value;
    }

    // Builds a menu option list without needing C++11 brace-init lists,
    // so this compiles on older compiler defaults (Dev-C++ / TDM-GCC).
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

    // Arrow-key menu. Up/Down move the highlight, Enter selects.
    // Returns a 1-based index so every switch(choice) still works.
    int selectMenu(vector<string> options, string title, string subtitle = "")
    {
        int selected = 0;
        int n = (int)options.size();

        ui::hideCursor();

        while (true)
        {
            ui::clearScreen();
            ui::banner(title, subtitle);
            cout << ui::GREY << "   Up/Down arrows to move  |  Enter to select"
                 << ui::RESET << "\n\n";

            for (int i = 0; i < n; i++)
            {
                string num = (i + 1 < 10 ? "0" : "") + numberToString(i + 1);
                if (i == selected)
                {
                    cout << "  " << ui::BG_CYAN << ui::BOLD
                         << " > " << num << "  "
                         << ui::padRight(options[i], 38) << ui::RESET << "\n";
                }
                else
                {
                    cout << "  " << ui::GREY << "   " << num << "  " << ui::RESET
                         << options[i] << "\n";
                }
            }

            int key = readKey();

#ifdef _WIN32
            if (key == 224 || key == 0)
            {
                key = readKey();
                if (key == 72) selected = (selected - 1 + n) % n;
                else if (key == 80) selected = (selected + 1) % n;
            }
            else if (key == 13)
            {
                ui::showCursor();
                ui::clearScreen();
                return selected + 1;
            }
#else
            if (key == 27)
            {
                readKey();
                int arrow = readKey();
                if (arrow == 'A') selected = (selected - 1 + n) % n;
                else if (arrow == 'B') selected = (selected + 1) % n;
            }
            else if (key == 10)
            {
                ui::showCursor();
                ui::clearScreen();
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
            cout << ui::RED << "  Please enter a number: " << ui::RESET;
        }
        return value;
    }

    vector<int> selectMultiMenu(vector<string> options, string title)
    {
        int n = (int)options.size();
        vector<bool> checked(n, false);
        int cursor = 0;

        ui::hideCursor();

        while (true)
        {
            ui::clearScreen();
            ui::banner(title, "", ui::MAGENTA);
            cout << ui::GREY << "   Up/Down to move  |  Space to select  |  Enter to confirm"
                 << ui::RESET << "\n\n";

            for (int i = 0; i < n; i++)
            {
                string box = checked[i]
                    ? (ui::GREEN + ui::BOLD + "[x]" + ui::RESET)
                    : (ui::GREY + "[ ]" + ui::RESET);

                if (i == cursor)
                {
                    cout << "  " << ui::CYAN << ui::BOLD << ">" << ui::RESET
                         << " " << box << " " << ui::BOLD << ui::WHITE
                         << options[i] << ui::RESET << "\n";
                }
                else
                {
                    cout << "    " << box << " " << options[i] << "\n";
                }
            }

            int key = readKey();

#ifdef _WIN32
            if (key == 224 || key == 0)
            {
                key = readKey();
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
                readKey();
                int arrow = readKey();
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

        ui::showCursor();
        ui::clearScreen();

        vector<int> result;
        for (int i = 0; i < n; i++)
            if (checked[i])
                result.push_back(i + 1);

        return result;
    }

    void pauseScreen()
    {
        cout << "\n" << ui::GREY << "  Press any key to continue..." << ui::RESET;
        cout.flush();
        readKey();
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
        ui::clearScreen();
        ui::banner("HOTEL BOOKING SYSTEM", "Welcome - let's get started", ui::MAGENTA);
        cout << "\n";
        ui::prompt("Enter Hotel Name: ");
        getline(cin, hotelName);
        cout << "\n";
        ui::line("Hotel Name", hotelName, ui::BOLD + ui::YELLOW);
        ui::ok("Hotel Name Accepted Successfully!");
    }

    void registerUser()
    {
        string p, cp;

        ui::header("REGISTER");

        ui::prompt("Enter Name: ");
        getline(cin, userName);

        ui::prompt("Enter Phone Number: ");
        cin >> userPhone;

        while (!isValidPhone(userPhone))
        {
            ui::err("Invalid Phone Number!");
            ui::prompt("Enter exactly 10 digits: ");
            cin >> userPhone;
        }

        ui::prompt("Create Password: ");
        cin >> p;

        while (!isValidPassword(p))
        {
            ui::err("Password must be between 4 and 16 characters!");
            ui::prompt("Create Password: ");
            cin >> p;
        }

        ui::prompt("Confirm Password: ");
        cin >> cp;

        if (p != cp)
        {
            ui::err("Password does not match!");
            return;
        }

        userPassword = p;
        registered = true;
        ui::ok("Registration Successfully Completed!");
    }

    void forgotPassword()
    {
        string phone, newPassword, confirmPassword;

        ui::header("FORGOT PASSWORD");

        if (!registered)
        {
            ui::warn("No registered account found.");
            return;
        }

        ui::prompt("Enter Registered Phone Number: ");
        cin >> phone;

        if (phone != userPhone)
        {
            ui::err("Phone Number Not Found!");
            return;
        }

        ui::prompt("Enter New Password: ");
        cin >> newPassword;

        while (!isValidPassword(newPassword))
        {
            ui::err("Password must be between 4 and 16 characters!");
            ui::prompt("Enter New Password: ");
            cin >> newPassword;
        }

        ui::prompt("Confirm New Password: ");
        cin >> confirmPassword;

        if (newPassword != confirmPassword)
        {
            ui::err("Password does not match!");
            return;
        }

        userPassword = newPassword;
        ui::ok("Password Changed Successfully!");
    }

    bool login()
    {
        string phone, password;

        if (!registered)
        {
            cout << "\n";
            ui::warn("Please Register First!");
            return false;
        }

        ui::header("LOGIN");
        ui::prompt("Enter Phone Number: ");
        cin >> phone;

        ui::prompt("Enter Password: ");
        cin >> password;

        if (phone == userPhone && password == userPassword)
        {
            ui::ok("Login Successfully!");
            pauseScreen();
            return true;
        }

        ui::err("Wrong Phone Number or Password!");
        return false;
    }

    void roomStatus()
    {
        ui::header("ROOM STATUS");

        int bookedCount = 0;

        for (int i = 0; i < 15; i++)
        {
            cout << "  " << ui::GREY << "Room " << ui::RESET << ui::BOLD
                 << (i + 1 < 10 ? " " : "") << i + 1 << ui::RESET << " : ";
            if (booked[i])
            {
                cout << ui::RED << ui::BOLD << "[ Booked    ]" << ui::RESET;
                bookedCount++;
            }
            else
            {
                cout << ui::GREEN << ui::BOLD << "[ Available ]" << ui::RESET;
            }
            cout << endl;
        }

        cout << "\n  " << ui::GREEN << "Available: " << (15 - bookedCount) << ui::RESET
             << "   " << ui::RED << "Booked: " << bookedCount << ui::RESET << "\n";
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

        ui::header("ROOM RECOMMENDATION");
        ui::prompt("Enter Number of Members: ");
        people = readInt();

        ui::prompt("Enter Maximum Budget Per Day: Rs. ");
        budget = readDouble();

        if (people <= 0 || budget <= 0)
        {
            ui::err("Invalid Input!");
            return;
        }

        price = getRoomPrice(people, 0);

        cout << "\n";
        ui::line("Recommended", "Rs. " + moneyToString(price) + " per day", ui::BOLD + ui::YELLOW);

        if (budget >= price)
            ui::ok("Status: Suitable for your budget!");
        else
            ui::warn("Status: Budget is lower than recommended price.");
    }

    void quizTask()
    {
        int answer;
        int score = 0;

        ui::header("FREE BOOKING QUIZ");

        if (quizAttempted)
        {
            ui::warn("You have already attempted the quiz.");
            return;
        }

        quizAttempted = true;

        cout << ui::BOLD << ui::MAGENTA << "  Q1. " << ui::RESET << "Which language is used in this project?\n";
        cout << "      1. Java   2. C++   3. Python   4. HTML\n";
        ui::prompt("Answer: ");
        answer = readInt();
        if (answer == 2) score++;

        cout << "\n" << ui::BOLD << ui::MAGENTA << "  Q2. " << ui::RESET << "How many rooms are available?\n";
        cout << "      1. 10   2. 12   3. 15   4. 20\n";
        ui::prompt("Answer: ");
        answer = readInt();
        if (answer == 3) score++;

        cout << "\n" << ui::BOLD << ui::MAGENTA << "  Q3. " << ui::RESET << "Which header is used for string?\n";
        cout << "      1. <string>   2. <iomanip>   3. <cstdlib>   4. <ctime>\n";
        ui::prompt("Answer: ");
        answer = readInt();
        if (answer == 1) score++;

        cout << "\n" << ui::BOLD << ui::MAGENTA << "  Q4. " << ui::RESET << "Which object is used for output?\n";
        cout << "      1. cin   2. cout   3. scanf   4. input\n";
        ui::prompt("Answer: ");
        answer = readInt();
        if (answer == 2) score++;

        cout << "\n" << ui::BOLD << ui::MAGENTA << "  Q5. " << ui::RESET << "Which keyword is used to create a class?\n";
        cout << "      1. object   2. function   3. class   4. create\n";
        ui::prompt("Answer: ");
        answer = readInt();
        if (answer == 3) score++;

        cout << "\n";
        ui::line("Your Score", numberToString(score) + "/5", ui::BOLD + ui::YELLOW);

        if (score == 5)
        {
            freeBookingAvailable = true;
            ui::ok("CONGRATULATIONS! One free room booking is available.");
        }
        else
        {
            ui::warn("Sorry! You need 5/5 for free booking.");
        }
    }

    void bookRoom()
    {
        int room;

        roomStatus();

        cout << "\n";
        ui::prompt("Enter Room Number (1-15): ");
        room = readInt();

        if (room < 1 || room > 15)
        {
            ui::err("Invalid Room Number!");
            return;
        }

        if (booked[room - 1])
        {
            ui::err("Room Already Booked!");
            return;
        }

        cin.ignore();

        ui::header("CUSTOMER DETAILS");

        ui::prompt("Enter Customer Name: ");
        getline(cin, customerName[room - 1]);

        ui::prompt("Enter Phone Number: ");
        getline(cin, customerPhone[room - 1]);

        while (!isValidPhone(customerPhone[room - 1]))
        {
            ui::err("Invalid Phone Number!");
            ui::prompt("Enter exactly 10 digits: ");
            getline(cin, customerPhone[room - 1]);
        }

        ui::prompt("Enter Aadhar Number: ");
        getline(cin, customerAadhar[room - 1]);

        ui::prompt("Enter PAN Number: ");
        getline(cin, customerPAN[room - 1]);

        ui::prompt("How Many Members: ");
        members[room - 1] = readInt();

        if (members[room - 1] <= 0)
        {
            ui::err("Invalid Number of Members!");
            return;
        }

        ui::prompt("Enter Check-in Date (e.g. 18-08-2026): ");
        getline(cin, checkInDate[room - 1]);

        ui::prompt("Enter Month Number (1-12): ");
        month[room - 1] = readInt();

        if (month[room - 1] < 1 || month[room - 1] > 12)
        {
            ui::err("Invalid Month!");
            return;
        }

        ui::prompt("Number of Days Stay: ");
        daysStay[room - 1] = readInt();

        if (daysStay[room - 1] <= 0)
        {
            ui::err("Invalid Number of Days!");
            return;
        }

        double price = getRoomPrice(members[room - 1], month[room - 1]);

        char useFree = 'N';

        if (freeBookingAvailable)
        {
            cout << "\n";
            ui::ok("FREE BOOKING AVAILABLE!");
            ui::prompt("Use Free Booking? (Y/N): ");
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

        cout << "\n";
        ui::borderTop(ui::GREEN);
        ui::boxLine("BOOKING SUCCESSFUL", ui::GREEN, ui::BOLD + ui::GREEN, true);
        ui::borderMid(ui::GREEN);
        ui::boxLine("Hotel          : " + hotelName, ui::GREEN);
        ui::boxLine("Booking ID     : " + bookingID[room - 1], ui::GREEN, ui::YELLOW);
        ui::boxLine("Customer       : " + customerName[room - 1], ui::GREEN);
        ui::boxLine("Room Number    : " + numberToString(room), ui::GREEN);
        ui::boxLine("Members        : " + numberToString(members[room - 1]), ui::GREEN);
        ui::boxLine("Check-in Date  : " + checkInDate[room - 1], ui::GREEN);
        ui::boxLine("Days Stay      : " + numberToString(daysStay[room - 1]), ui::GREEN);
        ui::boxLine("Room Price/Day : Rs. " + moneyToString(price), ui::GREEN);
        ui::boxLine("Room Bill      : Rs. " + moneyToString(roomBill[room - 1]), ui::GREEN, ui::BOLD);
        ui::borderBottom(ui::GREEN);
    }

    void foodOrder()
    {
        int room;

        ui::header("FOOD ORDER");
        ui::prompt("Enter Room Number: ");
        room = readInt();

        if (room < 1 || room > 15 || !booked[room - 1])
        {
            ui::err("Invalid or Unbooked Room!");
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
            ui::warn("No Items Selected.");
            return;
        }

        double orderTotal = 0;

        for (size_t s = 0; s < selected.size(); s++)
        {
            int idx = selected[s] - 1;
            int quantity;

            cout << "\n";
            ui::prompt("Enter Quantity for " + names[idx] + ": ");
            quantity = readInt();

            if (quantity <= 0)
            {
                ui::warn("Invalid Quantity, Skipping " + names[idx] + ".");
                continue;
            }

            double total = prices[idx] * quantity;
            foodBill[room - 1] += total;
            orderTotal += total;

            cout << "  " << ui::GREEN << names[idx] << " x " << quantity
                 << " = Rs. " << total << ui::RESET << endl;
        }

        cout << "\n";
        ui::line("Order Total", "Rs. " + moneyToString(orderTotal), ui::BOLD + ui::YELLOW);
        ui::line("Food Bill", "Rs. " + moneyToString(foodBill[room - 1]), ui::BOLD);
        ui::ok("Food Order Completed!");
    }

    void roomServiceRequest()
    {
        int room;

        ui::header("ROOM SERVICE");
        ui::prompt("Enter Room Number: ");
        room = readInt();

        if (room < 1 || room > 15 || !booked[room - 1])
        {
            ui::err("Invalid or Unbooked Room!");
            return;
        }

        string names[4] = {"Room Cleaning", "Extra Bed", "Mineral Water", "Maintenance"};
        double prices[4] = {100, 500, 30, 0};

        vector<string> menu = makeMenu(4, "Room Cleaning - Rs. 100", "Extra Bed - Rs. 500",
            "Mineral Water - Rs. 30", "Maintenance - Rs. 0");

        vector<int> selected = selectMultiMenu(menu, "ROOM SERVICE - Room " + numberToString(room));

        if (selected.empty())
        {
            ui::warn("No Service Selected.");
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

        ui::ok("Room Service Request Submitted!");
    }

    void payment()
    {
        int room, choice;

        ui::header("PAYMENT");
        ui::prompt("Enter Room Number: ");
        room = readInt();

        if (room < 1 || room > 15 || !booked[room - 1])
        {
            ui::err("Invalid or Unbooked Room!");
            return;
        }

        cout << "\n";
        ui::numbered(1, "Cash");
        ui::numbered(2, "UPI");
        ui::numbered(3, "Card");
        ui::numbered(4, "Online Payment");
        cout << "\n";
        ui::prompt("Select Payment Method: ");
        choice = readInt();

        cout << "\n";
        switch (choice)
        {
            case 1:
                ui::ok("Payment Method: Cash");
                break;
            case 2:
                ui::ok("Payment Method: UPI");
                ui::line("UPI ID", upiID, ui::BOLD + ui::YELLOW);
                break;
            case 3:
                ui::ok("Payment Method: Card");
                break;
            case 4:
                ui::ok("Payment Method: Online Payment");
                break;
            default:
                ui::err("Invalid Payment Method!");
        }
    }

    // Prints the bill for a room that's already been validated as
    // booked. Shared by receipt() and checkout().
    void printReceipt(int room)
    {
        double total =
            roomBill[room - 1] +
            foodBill[room - 1] +
            serviceBill[room - 1];

        cout << "\n";
        ui::borderTop(ui::YELLOW);
        ui::boxLine(hotelName, ui::YELLOW, ui::BOLD + ui::WHITE, true);
        ui::boxLine("RECEIPT", ui::YELLOW, ui::CYAN, true);
        ui::borderMid(ui::YELLOW);
        ui::boxLine("Booking ID    : " + bookingID[room - 1], ui::YELLOW);
        ui::boxLine("Customer Name : " + customerName[room - 1], ui::YELLOW);
        ui::boxLine("Phone Number  : " + customerPhone[room - 1], ui::YELLOW);
        ui::boxLine("Room Number   : " + numberToString(room), ui::YELLOW);
        ui::boxLine("Members       : " + numberToString(members[room - 1]), ui::YELLOW);
        ui::boxLine("Check-in Date : " + checkInDate[room - 1], ui::YELLOW);
        ui::boxLine("Days Stay     : " + numberToString(daysStay[room - 1]), ui::YELLOW);
        ui::borderMid(ui::YELLOW);
        ui::boxLine("Room Bill     : Rs. " + moneyToString(roomBill[room - 1]), ui::YELLOW);
        ui::boxLine("Food Bill     : Rs. " + moneyToString(foodBill[room - 1]), ui::YELLOW);
        ui::boxLine("Service Bill  : Rs. " + moneyToString(serviceBill[room - 1]), ui::YELLOW);
        ui::borderMid(ui::YELLOW);
        ui::boxLine("Grand Total   : Rs. " + moneyToString(total), ui::YELLOW, ui::BOLD + ui::GREEN);
        ui::borderMid(ui::YELLOW);
        ui::boxLine("THANK YOU FOR VISITING!", ui::YELLOW, ui::MAGENTA, true);
        ui::borderBottom(ui::YELLOW);
    }

    void receipt()
    {
        int room;

        ui::header("RECEIPT");
        ui::prompt("Enter Room Number: ");
        room = readInt();

        if (room < 1 || room > 15 || !booked[room - 1])
        {
            ui::err("Invalid or Unbooked Room!");
            return;
        }

        printReceipt(room);
    }

    void extendStay()
    {
        int room, extraDays;

        ui::header("EXTEND STAY");
        ui::prompt("Enter Room Number: ");
        room = readInt();

        if (room < 1 || room > 15 || !booked[room - 1])
        {
            ui::err("Invalid or Unbooked Room!");
            return;
        }

        ui::line("Current Stay", numberToString(daysStay[room - 1]) + " days");
        ui::prompt("Enter Additional Days: ");
        extraDays = readInt();

        if (extraDays <= 0)
        {
            ui::err("Invalid Days!");
            return;
        }

        if (roomBill[room - 1] == 0)
        {
            daysStay[room - 1] += extraDays;
            ui::ok("Stay Extended Successfully!");
            ui::info("Room Bill remains FREE.");
            return;
        }

        double pricePerDay =
            roomBill[room - 1] / daysStay[room - 1];

        daysStay[room - 1] += extraDays;
        roomBill[room - 1] =
            pricePerDay * daysStay[room - 1];

        ui::ok("Stay Extended Successfully!");
        ui::line("New Stay", numberToString(daysStay[room - 1]) + " days");
        ui::line("Updated Room Bill", "Rs. " + moneyToString(roomBill[room - 1]), ui::BOLD + ui::YELLOW);
    }

    void cancelBooking()
    {
        int room;

        ui::header("CANCEL BOOKING");
        ui::prompt("Enter Room Number: ");
        room = readInt();

        if (room < 1 || room > 15 || !booked[room - 1])
        {
            ui::err("Invalid or Unbooked Room!");
            return;
        }

        // Only allow cancelling before any food/service has been
        // added or paid for, so this can't be used to dodge charges.
        if (foodBill[room - 1] > 0 || serviceBill[room - 1] > 0)
        {
            ui::err("Cannot cancel: charges already added to this room.");
            ui::info("Please use Checkout instead.");
            return;
        }

        ui::line("Customer", customerName[room - 1], ui::BOLD);
        ui::prompt("Are you sure you want to cancel this booking? (Y/N): ");
        char confirm;
        cin >> confirm;

        if (confirm != 'Y' && confirm != 'y')
        {
            ui::warn("Cancellation aborted.");
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

        ui::ok("Booking Cancelled Successfully.");
    }

    void checkout()
    {
        int room;
        string password;

        ui::header("CHECKOUT");
        ui::prompt("Enter Room Number: ");
        room = readInt();

        if (room < 1 || room > 15 || !booked[room - 1])
        {
            ui::err("Invalid or Unbooked Room!");
            return;
        }

        ui::line("Customer", customerName[room - 1], ui::BOLD);
        ui::prompt("Enter Checkout Password: ");
        cin >> password;

        if (password != checkoutPassword)
        {
            ui::err("Wrong Checkout Password!");
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

        cout << "\n";
        ui::ok("Checkout Successfully Completed!");
        ui::info("Room is now Available.");
    }

    void feedbackSystem()
    {
        int room;

        ui::header("CUSTOMER FEEDBACK");
        ui::prompt("Enter Room Number: ");
        room = readInt();

        if (room < 1 || room > 15 || !booked[room - 1])
        {
            ui::err("Invalid or Unbooked Room!");
            return;
        }

        ui::prompt("Rating (1-5): ");
        rating[room - 1] = readInt();

        if (rating[room - 1] < 1 || rating[room - 1] > 5)
        {
            ui::err("Invalid Rating!");
            return;
        }

        cin.ignore();

        ui::prompt("Enter Feedback: ");
        getline(cin, feedback[room - 1]);

        ui::ok("Thank you for your feedback!");
    }

    void complaintSystem()
    {
        int room;

        ui::header("COMPLAINT");
        ui::prompt("Enter Room Number: ");
        room = readInt();

        if (room < 1 || room > 15 || !booked[room - 1])
        {
            ui::err("Invalid or Unbooked Room!");
            return;
        }

        cin.ignore();

        ui::prompt("Enter Complaint: ");
        getline(cin, complaint[room - 1]);

        complaintStatus[room - 1] = "Pending";

        ui::ok("Complaint Submitted Successfully!");
    }

    void lostAndFound()
    {
        int choice;

        ui::header("LOST AND FOUND");
        ui::numbered(1, "Report Lost Item");
        ui::numbered(2, "View Lost Items");
        ui::numbered(3, "Back");
        cout << "\n";
        ui::prompt("Enter Choice: ");
        choice = readInt();

        if (choice == 1)
        {
            if (lostFoundCount >= 100)
            {
                ui::err("Storage Full!");
                return;
            }

            cin.ignore();

            ui::prompt("Enter Lost Item Name: ");
            getline(cin, lostFoundItem[lostFoundCount]);

            lostFoundStatus[lostFoundCount] = "Reported";
            lostFoundCount++;

            ui::ok("Lost Item Reported Successfully!");
        }
        else if (choice == 2)
        {
            if (lostFoundCount == 0)
            {
                ui::warn("No Lost Items Found.");
                return;
            }

            cout << "\n";
            for (int i = 0; i < lostFoundCount; i++)
            {
                cout << "  " << ui::CYAN << ui::BOLD << i + 1 << "." << ui::RESET << " "
                     << lostFoundItem[i]
                     << ui::GREY << " - " << ui::RESET << ui::YELLOW
                     << lostFoundStatus[i] << ui::RESET
                     << endl;
            }
        }
    }

    void bookingHistory()
    {
        ui::header("BOOKING HISTORY");

        if (historyCount == 0)
        {
            ui::warn("No Booking History Found.");
            return;
        }

        for (int i = 0; i < historyCount; i++)
        {
            cout << "  " << ui::CYAN << ui::BOLD << i + 1 << "." << ui::RESET
                 << " " << history[i] << endl;
        }
    }

    bool adminLogin()
    {
        string password;

        ui::header("ADMIN LOGIN");
        ui::prompt("Enter Admin Password: ");
        cin >> password;

        if (password == adminPassword)
        {
            ui::ok("Admin Login Successfully!");
            pauseScreen();
            return true;
        }

        ui::err("Wrong Admin Password!");
        pauseScreen();
        return false;
    }

    void changeAdminPassword()
    {
        string oldPassword, newPassword, confirmPassword;

        ui::header("CHANGE ADMIN PASSWORD");

        ui::prompt("Enter Current Admin Password: ");
        cin >> oldPassword;

        if (oldPassword != adminPassword)
        {
            ui::err("Wrong Current Admin Password!");
            return;
        }

        ui::prompt("Enter New Admin Password: ");
        cin >> newPassword;

        ui::prompt("Confirm New Password: ");
        cin >> confirmPassword;

        if (newPassword != confirmPassword)
        {
            ui::err("Password does not match!");
            return;
        }

        adminPassword = newPassword;
        ui::ok("Admin Password Changed Successfully!");
    }

    void changeCheckoutPassword()
    {
        string oldPassword, newPassword, confirmPassword;

        ui::header("CHANGE CHECKOUT PASSWORD");

        ui::prompt("Enter Current Checkout Password: ");
        cin >> oldPassword;

        if (oldPassword != checkoutPassword)
        {
            ui::err("Wrong Current Checkout Password!");
            return;
        }

        ui::prompt("Enter New Checkout Password: ");
        cin >> newPassword;

        ui::prompt("Confirm New Password: ");
        cin >> confirmPassword;

        if (newPassword != confirmPassword)
        {
            ui::err("Password does not match!");
            return;
        }

        checkoutPassword = newPassword;
        ui::ok("Checkout Password Changed Successfully!");
    }

    void viewComplaints()
    {
        bool found = false;

        ui::header("CUSTOMER COMPLAINTS");

        for (int i = 0; i < 15; i++)
        {
            if (complaint[i] != "" && complaint[i] != "None")
            {
                found = true;

                cout << "\n";
                ui::line("Room", numberToString(i + 1), ui::BOLD);
                ui::line("Customer", customerName[i]);
                ui::line("Complaint", complaint[i], ui::RED);
                ui::line("Status", complaintStatus[i], ui::YELLOW);
            }
        }

        if (!found)
            ui::warn("No Complaints Found.");
    }

    void viewFeedback()
    {
        bool found = false;

        ui::header("CUSTOMER FEEDBACK");

        for (int i = 0; i < 15; i++)
        {
            if (feedback[i] != "")
            {
                found = true;

                cout << "\n";
                ui::line("Room", numberToString(i + 1), ui::BOLD);
                ui::line("Customer", customerName[i]);
                ui::line("Rating", numberToString(rating[i]) + "/5", ui::YELLOW);
                ui::line("Feedback", feedback[i], ui::GREEN);
            }
        }

        if (!found)
            ui::warn("No Feedback Found.");
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
                "ADMIN PANEL", hotelName
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
                    ui::ok("Admin Logged Out.");
                    break;
                default:
                    ui::err("Invalid Choice!");
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
                "CUSTOMER MENU", hotelName
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
                    ui::ok("Customer Logged Out.");
                    break;
                default:
                    ui::err("Invalid Choice!");
            }

            if (choice != 16)
                pauseScreen();

        } while (choice != 16);
    }

    void run()
    {
        int choice;

        enterHotelName();
        pauseScreen();

        do
        {
            choice = selectMenu(
                makeMenu(5, "Register", "Login", "Forgot Password", "Admin Panel", "Exit"),
                "HOTEL BOOKING SYSTEM", hotelName
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
                    ui::clearScreen();
                    ui::banner("THANK YOU FOR USING HOTEL SYSTEM", "See you again!", ui::MAGENTA);
                    cout << "\n";
                    break;
                default:
                    ui::err("Invalid Choice!");
            }

            if (choice != 5)
                pauseScreen();

        } while (choice != 5);
    }
};

int main()
{
    ui::enableAnsi();

    Hotel hotel;
    hotel.run();

    return 0;
}
