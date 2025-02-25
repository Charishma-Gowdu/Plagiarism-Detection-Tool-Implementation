#include <iostream>
#include <cstdlib>
using namespace std;

void print (int** arr) {
    cout << endl;
    for (int i = 7; i >= 0; i--) {
        for (int j = 0; j < 8; j++) {
            cout << "+--------";
        } 
        cout << "+" << endl;
        for (int j = 0; j < 8; j++) {
            cout << "|";
            switch (arr [j] [i]) {
                case -1: cout << " /^^^^\\ "; break;
                case 1: cout << " /@@@@\\ "; break;
                default: cout << "        "; break;
            } 
        } 
        cout << "|" << endl;
        for (int j = 0; j < 8; j++) {
            cout << "|";
            switch (arr [j] [i]) {
                case -1: cout << " [    ] "; break;
                case 1: cout << " @@@@@@ "; break;
                default: cout << "        "; break;
            }
        } 
        cout << "|" << endl;
        for (int j = 0; j < 8; j++) {
            cout << "|";
            switch (arr [j] [i]) {
                case -1: cout << " \\____/ "; break;
                case 1: cout << " \\@@@@/ "; break;
                default: cout << "        "; break;
            }
        } 
        cout << "|" << endl; 
    } 
    for (int j = 0; j < 8; j++) {
        cout << "+--------";
    } 
    cout << "+" << endl; 
} 

void flip (int** board, int x, int y, int* moves) {
    for (int i = 1; i <= moves[0]; i++) board[x + i][y] *= -1;
    for (int i = 1; i <= moves[1]; i++) board[x - i][y] *= -1;
    for (int i = 1; i <= moves[2]; i++) board[x][y + i] *= -1;
    for (int i = 1; i <= moves[3]; i++) board[x][y - i] *= -1;
    for (int i = 1; i <= moves[4]; i++) board[x + i][y + i] *= -1;
    for (int i = 1; i <= moves[5]; i++) board[x + i][y - i] *= -1;
    for (int i = 1; i <= moves[6]; i++) board[x - i][y + i] *= -1;
    for (int i = 1; i <= moves[7]; i++) board[x - i][y - i] *= -1;
    return;
} 

int playerMove (int** board, int x, int y, int person, int isPlayer, int makeMove) {
    int* moves = new int [8]; 
    for (int i = 0; i < 8; i++) moves[i] = 0; 
    if (x < 1 || x > 8 || y < 1 || y > 8) {
        if (isPlayer) 
            cout << "Out of board square. Enter valid move." << endl; 
        delete [] moves;
        return 0; 
    } 
    if (board[x - 1][y - 1] != 0) {
        delete [] moves;
        if (isPlayer) cout << "Piece exists on chosen square. Enter valid move. " << endl;
        return 0; 
    } 
    x -= 1; y -= 1; 
    int count = 0; 
    int i = 1; 

    while (true) { 
        if (x + i == 8 || board[x + i][y] == 0) break;
        if (board[x + i][y] == person) {
            count += (i - 1);
            moves[0] = (i - 1);
            break;
        } 
        if (board[x + i][y] == -person) i++;
    } 
    i = 1; 
    while (true) { 
        if (x - i == -1 || board[x - i][y] == 0) break;
        if (board[x - i][y] == person) {
            count += (i - 1);
            moves[1] = (i - 1);
            break;
        } 
        if (board[x - i][y] == -person) i++;
    } 
    i = 1;
    while (true) { 
        // Check top squares - direction 2
        if (y + i == 8 || board[x][y + i] == 0) break;
        if (board[x][y + i] == person) {
            count += (i - 1);
            moves[2] = (i - 1);
            break;
        } 
        if (board[x][y + i] == -person) i++;
    } 
    i = 1;
    while (true) { 
        if (y - i == -1 || board[x][y - i] == 0) break;
        if (board[x][y - i] == person) {
            count += (i - 1);
            moves[3] = (i - 1);
            break;
        } 
        if (board[x][y - i] == -person) i++;
    } 
    i = 1;
    while (true) { 
        if (x + i == 8 || y + i == 8 || board[x + i][y + i] == 0) break;
        if (board[x + i][y + i] == person) {
            count += (i - 1);
            moves[4] = (i - 1);
            break;
        } 
        if (board[x + i][y + i] == -person) i++;
    } 
    i = 1;
    while (true) { 
        if (x + i == 8 || y - i == -1 || board[x + i][y - i] == 0) break;
        if (board[x + i][y - i] == person) {
            count += (i - 1);
            moves[5] = (i - 1);
            break;
        } 
        if (board[x + i][y - i] == -person) i++;
    } 
    i = 1;
    while (true) { 
        if (x - i == -1 || y + i == 8 || board[x - i][y + i] == 0) break;
        if (board[x - i][y + i] == person) {
            count += (i - 1);
            moves[6] = (i - 1);
            break;
        } 
        if (board[x - i][y + i] == -person) i++;
    } 
    i = 1;
    while (true) { 
        if (x - i == -1 || y - i == -1 || board[x - i][y - i] == 0) break;
        if (board[x - i][y - i] == person) {
            count += (i - 1);
            moves[7] = (i - 1);
            break;
        } 
        if (board[x - i][y - i] == -person) i++;
    } 
    i = 1;

    if (count == 0) {
        if (isPlayer) 
            cout << "No piece to flip to your color. Enter valid move. " << endl;
        delete [] moves;
        return 0; 
    }
    if (makeMove) { 
        board[x][y] = person; 
        flip (board, x, y, moves); 
    }
    delete [] moves;
    return count; 
}

int isMovable (int** board, int person, int isPlayer, int diff, int diff0, char* X) { 
    int max = 0, xin = 0, yin = 0;
    int maxin = 0; 
    int* maxarr = new int [28];
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++)
        {
            int point = playerMove (board, i + 1, j + 1, person, 0, 0); 
            if (point > 0 && i % 7 == 0) point += 2; 
            if (point > 0 && j % 7 == 0) point += 2;
            if (point > 0 && diff > 1) {
                int** board1 = new int* [8]; 

                for (int k = 0; k < 8; k++) 
                    board1[k] = new int [8];
                for (int k = 0; k < 8; k++)
                    for (int l = 0; l < 8; l++)
                        board1[k][l] = board[k][l];
                
                point -= playerMove (board1, i + 1, j + 1, person, 0, 0);
                point += playerMove (board1, i + 1, j + 1, person, 0, 1); 
                point -= isMovable (board1, -person, 0, diff-1, diff0, X); 
                point += isMovable (board1, person, 0, diff-1, diff0, X) + 1; 
                for (int k = 0; k < 8; k++) delete [] board1[i];
                delete [] board1;
            }
            if ((diff == 0 && point > 0) || (point == max && point > 0))
                maxarr[maxin++] = i*8 + j;
            if ((point > max && diff > 0) || (point > 0 && max == 0)) {
                max = point; 
                maxin = 1; 
                maxarr[0] = i*8 + j;
            }
        }
    } 
    srand (time(0)); 
    if (max > 0) { 
        int ptin = maxarr[(rand() % maxin)]; 
        delete [] maxarr;
        xin = ptin / 8; yin = ptin % 8;

        if (!(isPlayer)) {
            if (diff == diff0) { 
                cout << "Computer's turn. Enter to Continue: "; 
                cin.getline(X, 200);
            }

            if (diff == diff0) { 
                cout << "Computer made a move at (" << xin + 1;
                cout << ", " << yin + 1 << ")." << endl;
            }
            playerMove (board, xin + 1, yin + 1, person, 0, 1); 
            if (diff == diff0) print (board); 
        } 
        return max;
    } 
    else if (diff > 1) { 
        delete [] maxarr;
        return isMovable (board, person, 0, diff - 1, diff0 - 1, X);
    }
    delete [] maxarr;
    return 0;
}


int results (int** board) { 
    int p1 = 0, p2 = 0; 
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j] == -1) p2 += 1;
            if (board[i][j] == 1) p1 += 1;
        }
    } 
    if (p1 == p2) return 0;
    else if (p1 > p2) return 1;
    else return -1;
} 


int main() {
    int** board = new int* [8];
    for (int i = 0; i < 8; i++) {
        board[i] = new int [8];
    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if ((i == 3 && j == 3) || (i == 4 && j == 4)) board[i][j] = 1;
            else if ((i == 3 && j == 4) || (i == 4 && j == 3)) board[i][j] = -1;
            else board[i][j] = 0;
        }
    }
    char* X = new char [201];
    int comp = 0, color = 0, diff = 1;
    cout << endl << "OTHELLO GAME" << endl;
    cout << "Gameplay options: " << endl;
    cout << "    1: Two Player Game." << endl;
    cout << "    0: You vs Computer Game." << endl;
    cout << "Enter choice: " ; 
    cin.getline(X, 200);
    if (X[0] == '0') comp = 0;
    else if (X[0] == '1') comp = 1;
    else {
        cerr << "Invalid Choice!" << endl;
        abort();
    }

    if (comp == 0) {
        cout << endl << "Difficulty Levels: " << endl;
        cout << "    0. Easy" << endl;
        cout << "    1. Normal" << endl;
        cout << "    2. Hard" << endl;
        cout << "    3. Extreme" << endl;
        cout << "Enter your choice: ";
        cin.getline(X, 200);
        diff = X[0] - 48;
        cout << endl;
        if (diff < 0 || diff > 3) {
            cerr << "Invalid Difficulty!" << endl;
            abort();
        }
        cout << "Enter your color of choice (Black starts first) "; 
        cout << "[Enter 1 for black, -1 for white]: ";
    } 
    else {
        cout << "Enter color of player 1 (Black starts first) ";
        cout << "[Enter 1 for black, -1 for white]: ";
    } 
    cin.getline(X, 200);
    if (X[0] == '-') color = 48 - X[1];
    else color = X[0] - 48;
    if (color*color != 1) {
        cerr << "Invalid Color!" << endl;
        abort();
    }
    cout << endl << "Squares (x, y) are numbered from (1, 1) to (8, 8). ";
    cout << "(1, 1) being the bottom left and (8, 8) top right" << endl; 
    cout << "Note that x --> left (1) to right (8). ";
    cout << "y --> bottom (1) to right (8)." << endl;

    int player = color; 
    print (board);

    while (true) { 
        cout << endl; cin.sync();
        if (player == -1 && comp == 0) { 
            if (isMovable (board, player*color, 0, diff, diff, X)) { 
                player *= -1; 
            } else {
                cout << "No valid move for computer. It's your turn now!" << endl;
                player *= -1;
            }
        } 
        else if (player == -1) { 
            if (isMovable (board, player*color, 1, 1, 1, X)) {
                cout << "Player 2: Enter move ";
                cout << "(place your color coin on x, y coordinates)." << endl;
                cout << "Enter space separated integers x, y starting 1 to 8: ";
                cin.getline(X, 200);
                int x = X[0] - 48;
                int y = X[2] - 48;

                int chk = playerMove (board, x, y, player*color, 1, 1);
                if (chk) {
                    print (board); 
                    player *= -1; 
                    continue;
                } 
                else continue; 
            } 
            else { 
                if (!(isMovable (board, -player*color, 1, 1, 1, X))) break; 
                cout << "No valid move for Player 2. Player 1, play again!" << endl;
                player *= -1;
            }
        } 
        else {
            if (isMovable (board, player*color, 1, 1, 1, X)) {
                if (comp) {
                    cout << "Player 1: Enter move ";
                    cout << "(place your color coin on x, y coordinates)." << endl;
                }
                else {
                    cout << "Enter your move ";
                    cout << "(place your color coin on x, y coordinates)." << endl;
                }
                cout << "Enter space separated integers x, y starting 1 to 8: ";
                cin.getline(X, 200);
                int x = X[0] - 48;
                int y = X[2] - 48;

                int chk = playerMove (board, x, y, player*color, 1, 1);
                if (chk) {
                    print (board);
                    player *= -1; 
                    continue;
                } 
                else 
                    continue; 
            } 
            else {
                if (!(isMovable (board, -player*color, comp, diff, 1, X))) 
                    break; 
                if (comp) 
                    cout << "No valid move for Player 1. Player 2, play again!" << endl; 
                else 
                    cout << "You have no valid moves. Computer will play again!" << endl; 
                player *= -1;
            }
        }
    } 
    cout << endl << "No possible moves. Game has ended." << endl; 
    delete [] X;

    int win = results (board); 
    for (int i = 0; i < 8; i++) delete [] board[i];
    delete [] board;
    if (win == 0) {
        cout << "THE RESULT OF THIS GAME IS: A TIE !!" << endl << endl;
    } 
    else if (win == color) { 
        if (comp) cout << "THE WINNER IS: PLAYER 1 !!" << endl << endl;
        else cout << "YOU ARE THE WINNER !!" << endl << endl;
    } 
    else { 
        if (comp) cout << "THE WINNER IS: PLAYER 2 !!" << endl << endl;
        else cout << "THE WINNER IS: THE COMPUTER !!" << endl << endl;
    } 
} 
