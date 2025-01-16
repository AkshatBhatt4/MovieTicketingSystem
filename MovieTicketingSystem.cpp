#include <bits/stdc++.h>
using namespace std;

bool isValidName(string name) {
    for (char c : name) {
        if (!isalpha(c) && c != ' ') {
            return false;
        }
    }
    return !name.empty();
}
bool isValidDate(string date) {
    if (date.size() != 6) return false;
    for (char c : date) {
        if (!isdigit(c)) return false;
    }
    int day = stoi(date.substr(0, 2));
    int month = stoi(date.substr(2, 2));
    int year = stoi(date.substr(4, 2));
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) return false;
    if (month == 2) {
        if (day > 28) return false;
    }
    return true;
}
class Seat{
public:
    bool booked;
    Seat* north;
    Seat* south;
    Seat* east;
    Seat* west;
    Seat() : booked(false), north(nullptr), south(nullptr), east(nullptr), west(nullptr) {}
};
class MTS {
public:
    vector<vector<Seat>> hall;
    vector<pair<char, int>> tickets;
    vector<pair<char,int>> BestSeats;
    string name, date, movie;
    int nTickets;

    MTS(){
        hall.resize(9, std::vector<Seat>(9));
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (i > 0) hall[i][j].north = &hall[i - 1][j];
                if (i < 8) hall[i][j].south = &hall[i + 1][j];
                if (j > 0) hall[i][j].west = &hall[i][j - 1];
                if (j < 8) hall[i][j].east = &hall[i][j + 1];
            }
        }
        name = Start();
        date = HomeScreen();
        movie = MoviePage();
        nTickets = NumberTickets();
        SeatSelect();
        ShowHall();
    }
    string Start() {
        string name;
        cout << "*****************************************************************************\n";
        cout << "***********************************WELCOME***********************************\n";
        cout << "*************************************TO**************************************\n";
        cout << "*******************************EPIC MOVIE HOUSE******************************\n";
        cout << "*****************************************************************************\n";
        cout << "Enter Your Name: ";
        getline(cin, name);
        while (!isValidName(name)) {
            cout << "Please provide a valid name: ";
            getline(cin, name);
        }
        return name;
    }
    string HomeScreen() {
        string date;
        cout << "Enter the date for the booking(DDMMYY): ";
        cin >> date;
        while (!isValidDate(date)) {
            cout << "Please enter a valid date(DDMMYY): ";
            cin >> date;
        }cout<<"\n";
        date.insert(2, "/");
        date.insert(5, "/");
        return date;
    }
    string MoviePage() {
        string movie;
        int m;
        cout << "                          MENU                          ";
        cout << "\nFollowing are the available movies:\n";
        cout << "1. Zindagi Na Milegi Dubara\n";
        cout << "2. Yeh Jawani Hai Diwani\n";
        cout << "3. Spider-man: No way Home\n";
        cout << "4. Inside Out\n";
        cout << "5. Top Gun:Maverick\n";
        cout << "Enter number of the movie: ";
        cin >> m;
        while (!(m > 0 && m < 6)) {
            cout << "Please choose a valid option(1-5): ";
            cin >> m;
        }
        switch (m) {
        case 1:
            movie = "Zindagi Na Milegi Dubara";
            break;
        case 2:
            movie = "Yeh Jawani Hai Diwani";
            break;
        case 3:
            movie = "Jab We Met";
            break;
        case 4:
            movie = "Inside Out";
            break;
        case 5:
            movie = "Top Gun:Maverick";
            break;
        }cout<<"\n";
        return movie;
    }
    int NumberTickets() {
        string s;
        cout << "Hello " << name << endl << "How many tickets do you want for the movie " << movie << " on " << date << ": ";
        cin >> s;
        while ((!isdigit(s[0])) || s.size() != 1 || s[0] == '0') {
            cout << "You can buy up to 9 tickets in one Transaction.\n";
            cout << "Please provide a valid number(1-9): ";
            cin >> s;
        }cout<<"\n";
        return stoi(s);
    }
    void ShowHall() {
        cout << "\n                       1 2 3 4 5 6 7 8 9\n";
        char r = 'A';
        for (const auto& row : hall) {
            cout<<"                     "<<r++;
            for (const auto& seat : row) {
                std::cout << (seat.booked ? " X" : " O");
            }
            std::cout << '\n';
        }
        cout << "                           Screen     \n";
        cout << "\nO->Available    X->Booked\n";
    }
    void SeatSelect() {
        ShowHall();
        ShowSuggestedSeats();
        string yesno;
        cout<<"Do you want to book the suggested seats?(y/n): ";
        cin>>yesno;
        while(yesno.size()!=1 || !(tolower(yesno[0])=='n' ||tolower(yesno[0])=='y')){
            cout<<"Please enter a valid response(y/n): ";
            cin>>yesno;
        }
        char yn=tolower(yesno[0]);
        if(yn=='n'){
            char row;
            int col;
            for (int i = 0; i < nTickets; ++i) {
                cout << "Select seat " << i + 1 << " (Row A-I and Column 1-9): ";
                cin >> row >> col;
                row = toupper(row);
                while (row < 'A' || row > 'I' || col < 1 || col > 9 || hall[row - 'A'][col - 1].booked) {
                    cout << "Invalid or already booked seat. Select seat again (Row A-I and Column 1-9): ";
                    cin >> row >> col;
                    row = toupper(row);
                }
                hall[row - 'A'][col - 1].booked = true;
                tickets.push_back({ row, col });
            }
        }
        else{
            for(auto i:BestSeats){
                hall[i.first - 'A'][i.second - 1].booked = true;
                tickets.push_back({ i.first, i.second });
            }
        }
        cout<<"The Tickets Selected are:";
        for(pair i:tickets){
            cout<<" "<<i.first<<i.second;
        }
        cout<<"Thanks for booking the movie tickets!\nEnjoy the movie!\n";
    }
    void SuggestBestSeats(int numSeats){
        int cRow=4;
        int cCol=4;
        auto distanceFromCenter = [cRow, cCol](int row, int col) {
            return sqrt(pow(cRow - row, 2) + pow(cCol - col, 2));
        };
        vector<tuple<double,int,int>> seatDistance;
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (!hall[i][j].booked) {
                    seatDistance.emplace_back(distanceFromCenter(i, j), i, j);
                }
            }
        }
        sort(seatDistance.begin(), seatDistance.end());
        for (const auto& [dist, row, col] : seatDistance) {
            if (BestSeats.size() >= numSeats) break;
            bool canPlace = true;
            for (int k = 0; k < numSeats; ++k) {
                if (col + k >= 9 || hall[row][col + k].booked) {
                    canPlace = false;
                    break;
                }
            }
            if (canPlace) {
                for (int k = 0; k < numSeats; ++k) {
                    BestSeats.emplace_back(char('A' + row), col + k);
                }
                return ;
            }
        }

        // If not possible, try to find the best hall in two adjacent rows
        for (const auto& [dist, row, col] : seatDistance) {
            if (BestSeats.size() >= numSeats) break;
            bool canPlace = true;
            int seatsPlaced = 0;
            for (int k = 0; k < numSeats; ++k) {
                if (col + k < 9 && !hall[row][col + k].booked) {
                    BestSeats.emplace_back(char('A' + row), col + k);
                    seatsPlaced++;
                } else if (row + 1 < 9 && col + k < 9 && !hall[row + 1][col + k].booked) {
                    BestSeats.emplace_back(char('A' + row + 1), col + k);
                    seatsPlaced++;
                } else {
                    canPlace = false;
                    break;
                }
            }
            if (canPlace && seatsPlaced == numSeats) {
                return ;
            } else {
                BestSeats.clear();
            }
        }
        return ;
    }
    void ShowSuggestedSeats(){
        SuggestBestSeats(nTickets);
        cout<<"The Suggest Seats are: ";
        for(auto i:BestSeats){
            cout<<i.first<<i.second<<" ";
        }
        cout<<"\n";
    }
};

int main() {
    MTS customer1;
    MTS customer2;
    return 0;
}
