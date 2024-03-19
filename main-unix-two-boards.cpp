#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <sstream>
#include <string>
#include <vector>
#include <time.h>
using namespace std;

const int board_y = 24;
const int board_x = 75;
int alive = 0;
int alive2 = 0;
int steps = 0;

bool boardedit = true;
string input;
string buffer;

bool board[board_y][board_x]; // y then x
bool board2[board_y][board_x];

bool shouldIProceedWith(string buf) { // verify whether number is correct
    for(char e : buf) {
        return isdigit(e);
    }
    return true;
}

int countClosest(int whichBoard, int x, int y) {
    int closest = 0;

    // determine location
    int closestXes[3] = {x-1, x, x+1};
    int closestYs[3] = {y-1, y, y+1};

    // out of range correction - the board should be infinite
    // so it will overlap
    for(int i = 0; i < 3; i++) {
        if(closestXes[i] < 0) {
            closestXes[i] = board_x - 1;
        }
        if(closestXes[i] > board_x - 1) {
            closestXes[i] = 0;
        }
    }

    for(int i = 0; i < 3; i++) {
        if(closestYs[i] < 0) {
            closestYs[i] = board_y - 1;
        }
        if(closestYs[i] > board_y - 1) {
            closestYs[i] = 0;
        }
    }

    // search for nearby alive cells
    for(int e : closestXes) {
        for(int f : closestYs) {
            if(0 <= e && e < board_x && 0 <= f && f < board_y) {
                if(!(e == x && f == y)) { // Skip current cell
                    if (whichBoard == 1) {
                        if(board[f][e]) {
                            closest++;
                        }
                    } else if (whichBoard == 2) {
                        if(board2[f][e]) {
                            closest++;
                        }
                    }
                }
            }
        }
    }

    // return result
    return closest;
}

void clear(int whichBoard) {
    if(whichBoard == 1) {
        for(int i = 0; i < board_y; i++) {
            for(int j = 0; j < board_x; j++) {
                board[i][j] = false;
            }
        }
        alive = 0;
    } else if (whichBoard == 2) {
        for(int i = 0; i < board_y; i++) {
            for(int j = 0; j < board_x; j++) {
                board2[i][j] = false;
            }
        }
        alive2 = 0;
    }
}

void randomize_one() {
    alive = 0;
    for(int i = 0; i < board_y; i++) {
        for(int j = 0; j < board_x; j++) {
            if(i + j * i % 3 == 0) {
                srand(time(0) + rand());
            } else if (i % 2 == 0 && j % 2 == 0) {
                srand(time(0) - rand());
            } else {
                srand(rand() - time(0));
            }
            bool judge = (rand() % 2 == 0);
            board[i][j] = judge;
            if(judge) {alive++;}
        }
    }
}

void randomize_two() {
    alive2 = 0;
    for(int i = 0; i < board_y; i++) {
        for(int j = 0; j < board_x; j++) {
            if(i + j * i % 3 == 0) {
                srand(time(0) + rand());
            } else if (i % 2 == 0 && j % 2 == 0) {
                srand(time(0) - rand());
            } else {
                srand(rand() - time(0));
            }
            bool judge = (rand() % 2 == 0);
            board2[i][j] = judge;
            if(judge) {alive2++;}
        }
    }
}

void drawBoard() {
    // Find some better alternative later
    system("clear");
    cout << "             ";
    for(int i = 10; i <= board_x; i += 10) {
        cout << i / 10 << "         ";
    }
    cout << "             ";
    for(int i = 10; i <= board_x; i += 10) {
        cout << i / 10 << "         ";
    }

    cout << endl << "   ";
    for(int i = 0; i <= board_x; i ++) {
        cout << i - ((i / 10) * 10);
    }
    cout << "       ";
    for(int i = 0; i <= board_x; i ++) {
        cout << i - ((i / 10) * 10);
    }

    cout << endl << "0  ";
    for(int i = -1; i <= board_x; i++) {
        cout << "=";
    }
    cout << "   0  ";
    for(int i = -1; i <= board_x; i++) {
        cout << "=";
    }

    cout << endl;

    for(int i = 0; i < board_y; i++) {
        cout << i + 1 << " ";
        if (i + 1 < 10) {
            cout << " ";
        }
        cout << "|";
        for(int j = 0; j < board_x; j++) {
            if (board[i][j]) {
                cout << "*"; 
            } else {
                cout << " ";
            }
        }
        cout << "|   ";

        cout << i + 1 << " ";
        if (i + 1 < 10) {
            cout << " ";
        }
        cout << "|";
        for(int j = 0; j < board_x; j++) {
            if (board2[i][j]) {
                cout << "*";
            } else {
                cout << " ";
            }
        }
        cout << "|" << endl;
    }

    cout << "   ";
    for(int i = -1; i <= board_x; i++) {
        cout << "=";
    }
    cout << "      ";
    for(int i = -1; i <= board_x; i++) {
        cout << "=";
    }

    cout << endl;
}

int main() {
    // make some fields alive before starting simulation
    while(true) {
        drawBoard();
        cout << "q - rozpocznij; 1/2,c - wyczysc; 1/2,r - rozmiesc losowo" << endl;
        cout << "Aktywuj lub deaktywuj pole (1/2,x,y): ";
        int x = -1; int y = -1;

        getline(cin, input);

        if(input[2] == 'c') {
            switch(input[0]) {
                case '1':
                    clear(1);
                    break;
                case '2':
                    clear(2);
                    break;
                default:
                    break;
            }
        }
        if(input[2] == 'r') {
            switch(input[0]) {
                case '1':
                    randomize_one();
                    break;
                case '2':
                    randomize_two();
                    break;
                default:
                    break;
            }
        }
        if(input[0] == 'q') {break;}

        stringstream instream; instream << input;
        bool proceed = true;

        getline(instream, buffer, ',');
        switch(buffer[0]) {
            case '1':
                getline(instream, buffer, ',');
                if(shouldIProceedWith(buffer)) {x = stoi(buffer) - 1;}

                getline(instream, buffer);
                if(shouldIProceedWith(buffer)) {y = stoi(buffer) - 1;}

                if(0 <= x && x < board_x && 0 <= y && y < board_y) {
                    if(board[y][x]) {
                        board[y][x] = false;
                        alive--;
                    } else {
                        board[y][x] = true;
                        alive++;
                    }
                }
                break;
            case '2':
                getline(instream, buffer, ',');
                if(shouldIProceedWith(buffer)) {x = stoi(buffer) - 1;}

                getline(instream, buffer);
                if(shouldIProceedWith(buffer)) {y = stoi(buffer) - 1;}

                if(0 <= x && x < board_x && 0 <= y && y < board_y) {
                    if(board2[y][x]) {
                        board2[y][x] = false;
                        alive2--;
                    } else {
                        board2[y][x] = true;
                        alive2++;
                    }
                }
                break;
            default:
                break;
        }

    }

    while(true) {
        drawBoard();
        cout << "Krok numer " << steps << ". Na planszy 1 zyje " << alive << " komorek. Na planszy 2 zyje " << alive2 << " komorek."  << endl;

        if(alive == 0 && alive2 == 0) {
            cout << "Koniec symulacji! Komorki na obu planszach obumarly!" << endl;
            break;
        }

        vector<vector<int>> to_deactivate;
        vector<vector<int>> to_activate;

        for(int i = 0; i < board_y; i++) {
            for(int j = 0; j < board_x; j++) {
                if(!(board[i][j]) && (countClosest(1, j, i) == 3)) {
                    to_activate.push_back({j, i});
                    alive++;
                } else if (board[i][j] && (countClosest(1, j, i) < 2 || countClosest(1, j, i) > 3)) {
                    to_deactivate.push_back({j, i});
                    alive--;
                }
            }
        }

        for(int i = 0; i < to_activate.size(); i++) {
            board[to_activate[i][1]][to_activate[i][0]] = true;
        }
        for(int i = 0; i < to_deactivate.size(); i++) {
            board[to_deactivate[i][1]][to_deactivate[i][0]] = false;
        }

        to_activate.clear();
        to_deactivate.clear();

        for(int i = 0; i < board_y; i++) {
            for(int j = 0; j < board_x; j++) {
                if(!(board2[i][j]) && (countClosest(2, j, i) == 3)) {
                    to_activate.push_back({j, i});
                    alive2++;
                } else if (board2[i][j] && (countClosest(2, j, i) < 2 || countClosest(2, j, i) > 3)) {
                    to_deactivate.push_back({j, i});
                    alive2--;
                }
            }
        }

        for(int i = 0; i < to_activate.size(); i++) {
            board2[to_activate[i][1]][to_activate[i][0]] = true;
        }
        for(int i = 0; i < to_deactivate.size(); i++) {
            board2[to_deactivate[i][1]][to_deactivate[i][0]] = false;
        }

        to_activate.clear();
        to_deactivate.clear();

        steps++;
        usleep(250000);
    }

    return 0;
}
