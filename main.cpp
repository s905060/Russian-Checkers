//
//  main.cpp
//  Russian Checker
//
//  Created by s905060 on 11/22/12.
//
//  Copyright (c) 2012 s905060. All rights reserved.
//

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <functional>
#include <string.h>

using std::string;
using namespace std;
int player = 0; //go first or not (first:1 = black =1, second:2 = white =2)
string name; // AI or You!

int total_node = 0;
int total_times_max_pruning = 0;
int total_times_min_pruning = 0;
int maximum_level = 50;
int max_nextmove = 0;
int min_nextmove = 0;


int alphabetToNum();
string numToAlphabet();

void draw_chessboard(int state[]);

void check_move(int total_turns, int total_times, int state[]);
void move_chess(int total_turns, int total_times, int state[]);

void check_jump(int total_turns, int total_times, int state[]);
void jump_chess(int total_turns, int total_times, int state[]);

void check_win(int total_turns, int times, int state[]);
void set_player();

int alpha_beta_search(int ai_state[]);
int max_value(int depth, int times, int ai_state[], int a, int b, int cutoff_level, int before_jump_state[]);
int min_value(int depth, int times, int ai_state[], int a, int b, int cutoff_level, int before_jump_state[]);

int ai_utility(int state[]);
int ai_terminal(int ai_state[]);

void set_level();

//Default Chess Value, will be changed.
const int row = 49;
int main_state[row]=
{
    9, 0, 2, 0, 2, 0, 2,
    9, 2, 0, 2, 0, 2, 0,
    9, 0, 0, 0, 0, 0, 0,
    9, 0, 0, 0, 0 ,0, 0,
    9, 0, 1, 0, 1, 0, 1,
    9, 1, 0, 1, 0, 1, 0,
    9, 9, 9, 9, 9, 9, 9
};
/*
int ai_state[row]=
{
    9, 0, 2, 0, 2, 0, 2,
    9, 2, 0, 2, 0, 2, 0,
    9, 0, 0, 0, 0, 0, 0,
    9, 0, 0, 0, 0 ,0, 0,
    9, 0, 1, 0, 1, 0, 1,
    9, 1, 0, 1, 0, 1, 0,
    9, 9, 9, 9, 9, 9, 9
};
*/
//Default ChessBoard layout.
string chessbord[row] =
{
    "  6", "   ", "*○*", "   ", "*○*", "   ", "*○*",
    "  5", "*○*", "   ", "*○*", "   ", "*○*", "   ",
    "  4", "   ", "***", "   ", "***", "   ", "***",
    "  3", "***", "   ", "***", "   ", "***", "   ",
    "  2", "   ", "*●*", "   ", "*●*", "   ", "*●*",
    "  1", "*●*", "   ", "*●*", "   ", "*●*", "   ",
    "   ", " A ", " B ", " C ", " D ", " E ", " F "
};

int alphabetToNum(string a){
    a[0] = toupper (a[0]);
    int c;
    string chessTable[row] =
    {
        "  6", "A6", "B6", "C6", "D6", "E6", "F6",
        "  5", "A5", "B5", "C5", "D5", "E5", "F5",
        "  4", "A4", "B4", "C4", "D4", "E4", "F4",
        "  3", "A3", "B3", "C3", "D3", "E3", "F3",
        "  2", "A2", "B2", "C2", "D2", "E2", "F2",
        "  1", "A1", "B1", "C1", "D1", "E1", "F1",
        "   ", "A ", "B ", "C ", "D ", "E ", "F "
    };
    for (int i=0; i < row; i++ ){
        if(chessTable[i] == a)
            c = i;
    }
    return c ;
}

string numToAlphabet(int a){
    string chessTable[row] =
    {
        "  6", "A6", "B6", "C6", "D6", "E6", "F6",
        "  5", "A5", "B5", "C5", "D5", "E5", "F5",
        "  4", "A4", "B4", "C4", "D4", "E4", "F4",
        "  3", "A3", "B3", "C3", "D3", "E3", "F3",
        "  2", "A2", "B2", "C2", "D2", "E2", "F2",
        "  1", "A1", "B1", "C1", "D1", "E1", "F1",
        "   ", "A ", "B ", "C ", "D ", "E ", "F "
    };
    string b;
    b = chessTable[a];
    return b;
}

// Printout Chessboard!
void draw_chessboard(int state[]){
    for(int i = 0; i < row; i++) {
        if (state[i] == 9) cout << chessbord[i]<< "\t";
        else if (state[i] == 0 && i%2 ==1)cout << "   "<< "\t";
        else if (state[i] == 1 && i%2 ==1)cout << " ● "<< "\t";
        else if (state[i] == 2 && i%2 ==1)cout << " ○ "<< "\t";
        else if (state[i] == 0 && i%2 ==0)cout << "***"<< "\t";
        else if (state[i] == 1 && i%2 ==0)cout << "*●*"<< "\t";
        else if (state[i] == 2 && i%2 ==0)cout << "*○*"<< "\t";
        if (i >0 && (i+1)%7 == 0 )
            cout << endl;
    }
}

//Check whcih chess an be moved
void check_move(int total_turns, int total_times, int state[]){
    int z = 0;
    int turn = total_turns;
    int times = total_times;
    times = 0;
    
    //Turn %2 => 1 Black :: 2 White
    for(int i = 0; i < row; i++) {
        if ((turn > 0 && turn %2 == 1 && state[i] == 1 && state[i-6] == 0 && i-6 >0)||
            (turn > 0 && turn %2 == 1 && state[i] == 1 && state[i-8] == 0 && i-8 >0)||
            
            ///////////////////////////////////////////////////////////////////////////////////////
            
            (turn > 0 && turn %2 == 0 && state[i] == 2 && state[i+6] == 0 && i+6 <row)||
            (turn > 0 && turn %2 == 0 && state[i] == 2 && state[i+8] == 0 && i+8 <row)){
            z++;
            cout <<"("<< z <<")  "<< numToAlphabet(i) << "\n";}
    }
    if ((z > 0 && player == 1 && turn > 0 && turn %2 == 1) || (z > 0 && player == 2 && turn >= 0 && turn %2 == 0)){name = "You ";}
    else name = "AI ";
    
    if (z > 0){
        if (name == "You ") {
            cout << name << "can move the above chess(es)\n";
            move_chess(turn, times, state);
        }
    }
}

// Which place can move to
void move_chess(int total_turns, int total_times, int state[]){
    string move;
    string chess;

    int z = 0;
    int turn = total_turns;
    int times = total_times;
    
    if (name == "You ") {
        cout << "Which chess to move? (e.g: D2):";
        cin >> chess;
        
        int check_move = alphabetToNum(chess);
        
        //Turn %2 => 1 Black :: 2 White
        if (turn > 0 && turn %2 == 1 && state[check_move-6] == 0 && state[check_move-6]!= 9 && check_move-6 >0){
            z++;
            cout <<"("<< z <<")  "<< numToAlphabet(check_move-6) << "\n";}
        
        if (turn > 0 && turn %2 == 1 && state[check_move-8] == 0 && state[check_move-8]!= 9 && check_move-8 >0){
            z++;
            cout <<"("<< z <<")  "<< numToAlphabet(check_move-8) << "\n";}
        
        //////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        if (turn >= 0 && turn %2 == 0 && state[check_move+6] == 0 && state[check_move+6]!= 9  && check_move+6 <row){
            z++;
            cout <<"("<< z <<")  "<< numToAlphabet(check_move+6) << "\n";}
        
        if (turn >= 0 && turn %2 == 0 && state[check_move+8] == 0 && state[check_move+8]!= 9  && check_move+8 <row){
            z++;
            cout <<"("<< z <<")  "<< numToAlphabet(check_move+8) << "\n";}
        
        if ((turn > 0 && turn %2 == 1 && player == 1) || (turn >= 0 && turn %2 == 0 && player == 2)){name = "You ";}
        else name = "AI ";
        
        cout << name << "can move to the above place(s)\n";
        cout << "Move to Where? (e.g: C3):";
        cin >> move;
        
        //Start to move check
        int move_check = alphabetToNum(move);
        int old_place = check_move;
        int new_place = move_check;
        
        main_state[new_place] = main_state[old_place];
        main_state[old_place] = 0;
        
        draw_chessboard(main_state);
        //next player
        check_win(turn+=1, times=0, main_state);
    }
}


void check_jump(int total_turns, int total_times, int state[]){
    
    int z = 0;
    int turn = total_turns;
    int times = total_times;
    
    //Turn %2 => 1 Black :: 2 White
    for(int i = 0; i < row; i++) {
        if ((turn > 0 && turn %2 == 1 && state[i] == 1 && state[i-12] == 0 && state[i-6] == 2 && i-12 >0)|| //jump Black state check
            (turn > 0 && turn %2 == 1 && state[i] == 1 && state[i-16] == 0 && state[i-8] == 2 && i-16 >0)||
            (turn > 0 && turn %2 == 1 && state[i] == 1 && state[i+12] == 0 && state[i+6] == 2 && i+12 <row)||
            (turn > 0 && turn %2 == 1 && state[i] == 1 && state[i+16] == 0 && state[i+8] == 2 && i+12 <row)||
            
            //////////////////////////////////////////////////////////////////////////////////////////////////////////
            
            (turn >= 0 && turn %2 == 0 && state[i] == 2 && state[i-12] == 0 && state[i-6] == 1 && i-12 >0)||  //jump White state check
            (turn >= 0 && turn %2 == 0 && state[i] == 2 && state[i-16] == 0 && state[i-8] == 1 && i-16 >0)||
            (turn >= 0 && turn %2 == 0 && state[i] == 2 && state[i+12] == 0 && state[i+6] == 1 && i+12 <row)||
            (turn >= 0 && turn %2 == 0 && state[i] == 2 && state[i+16] == 0 && state[i+8] == 1 && i+12 <row)){
            z++;
            cout <<"("<< z <<")  "<< numToAlphabet(i) << "\n";
        }
    }
    
    if ((z > 0 && player == 1 && turn > 0 && turn %2 == 1) || (z > 0 && player == 2 && turn >= 0 && turn %2 == 0)){name = "You ";}
    else name = "AI ";
    
    if (z > 0){
        if (name == "You ") {
            cout << name <<"have to jump the above chess(es)\n";
            jump_chess(turn, times, state);
        }
    }
    
}

void jump_chess(int total_turns, int total_times, int state[]){
    
    int turn = total_turns;
    int times = total_times;
    string jump;
    string chess;
    
    cout << "Which chess to jump? (e.g: D2):";
    cin >> chess;
    
    int check_jump = alphabetToNum(chess);
    int z = 0;
    
    //Turn %2 => 1 Black :: 2 White
    for(int i = 0; i < row; i++) {
        if (turn > 0 && turn %2 == 1 && state[check_jump-12] == 0 && state[check_jump-6] == 2 && check_jump-12 >0){ //jump Black state check
            z++;
            cout <<"("<< z <<")  "<< numToAlphabet(check_jump-12) << "\n";}
        
        if (turn > 0 && turn %2 == 1 && state[check_jump-16] == 0 && state[check_jump-8] == 2 && check_jump-16 >0){
            z++;
            cout <<"("<< z <<")  "<< numToAlphabet(check_jump-16) << "\n";}
        
        if (turn > 0 && turn %2 == 1 && state[check_jump+12] == 0 && state[check_jump+6] == 2 && check_jump+12 <row){
            z++;
            cout <<"("<< z <<")  "<< numToAlphabet(check_jump+12) << "\n";}
        
        if (turn > 0 && turn %2 == 1 &&state[check_jump+16] == 0 && state[check_jump+8] == 2 && check_jump+16 <row){
            z++;
            cout <<"("<< z <<")  "<< numToAlphabet(check_jump+16) << "\n";}
        
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        if (turn >= 0 && turn %2 == 0 &&  state[check_jump-12] == 0 && state[check_jump-6] == 1 && check_jump-12 >0){ //jump White state check
            z++;
            cout <<"("<< z <<")  "<< numToAlphabet(check_jump-12) << "\n";}
        
        if (turn >= 0 && turn %2 == 0 && state[check_jump-16] == 0 && state[check_jump-8] == 1 && check_jump-16 >0){
            z++;
            cout <<"("<< z <<")  "<< numToAlphabet(check_jump-16) << "\n";}
        
        if (turn >= 0 && turn %2 == 0 && state[check_jump+12] == 0 && state[check_jump+6] == 1 && check_jump+12 <row){
            z++;
            cout <<"("<< z <<")  "<< numToAlphabet(check_jump+12) << "\n";}
        
        if (turn >= 0 && turn %2 == 0 && state[check_jump+16] == 0 && state[check_jump+8] == 1 && check_jump+16 <row){
            z++;
            cout <<"("<< z <<")  "<< numToAlphabet(check_jump+16) << "\n";}
        
        if ((z > 0 && player == 1 && turn > 0 && turn %2 == 1) || (z > 0 && player == 2 && turn >= 0 && turn %2 == 0)){name = "You ";}
        else name = "AI ";
        
        cout << name <<"can jump to the above place(s)\n";
        cout << "Jump to Where? (e.g: C3):";
        cin >> jump;
        
        //Start to jump check
        int jump_check = alphabetToNum(jump);
        int old_place = check_jump;
        int new_place = jump_check;
        
        main_state[new_place] = main_state[old_place];
        main_state[old_place] = 0;
        
        if (jump_check == check_jump-16){main_state[check_jump-8] = 0;}
        else if (jump_check == check_jump+16){main_state[check_jump+8] = 0;}
        else if (jump_check == check_jump-12){main_state[check_jump-6] = 0;}
        else if (jump_check == check_jump+12){main_state[check_jump+6] = 0;}
        
        draw_chessboard(main_state);
        //jump times
        check_win(turn, times+=1, main_state); //check jump again
    }
}


//Check who is win or tie or continue
void check_win(int total_turns, int total_times, int state[]){
    
    int black = 0;
    int white = 0;
    int this_test_jump = 0;
    int this_test_move = 0;
    int last_test_jump = 0;
    int last_test_move = 0;
    
    int a = 0;
    int b = 0;
    char stop;
    int turn = total_turns;
    int times = total_times;

    for(int i = 0; i < row; i++) {  //check who's chess are all gone
        if (state[i] == 1){ black++; }
        else if (state[i] == 2){ white++; }
    }
    if (black == 0 && white != 0){
        if(player == 2){name = "You ";}
        else name = "AI ";
        cout << "\n" << name << "(White) Chess Won!!!!!";
        cin >> stop;
    }
    else if (white == 0 && black != 0){
        if(player == 1){name = "You ";}
        else name = "AI ";
        cout << "\n" << name <<"(Black) Chess Won!!!!!";
        cin >> stop;
    }
    else if (white == 0 && black == 0){
        cout << "\nIt is a tie!!!!!";
        cin >> stop;
    }
    
    
    for (int j=1; j<=2; j++){ //check this turn and last turn
        a = 0;
        b = 0;
        for (int i = 0; i < row; i++) {
            if ((turn > 0 && turn %2 == 1 && state[i] == 1 && state[i-12] == 0 && state[i-6] == 2 && i-12 >0)||  //jump Black state check
                (turn > 0 && turn %2 == 1 && state[i] == 1 && state[i-16] == 0 && state[i-8] == 2 && i-16 >0)||
                (turn > 0 && turn %2 == 1 && state[i] == 1 && state[i+12] == 0 && state[i+6] == 2 && i+12 <row)||
                (turn > 0 && turn %2 == 1 && state[i] == 1 && state[i+16] == 0 && state[i+8] == 2 && i+12 <row)||
                
                //////////////////////////////////////////////////////////////////////////////////////////////////////
                
                (turn > 0 && turn %2 == 0 && state[i] == 2 && state[i-12] == 0 && state[i-6] == 1 && i-12 >0)||  //jump White state check
                (turn > 0 && turn %2 == 0 && state[i] == 2 && state[i-16] == 0 && state[i-8] == 1 && i-16 >0)||
                (turn > 0 && turn %2 == 0 && state[i] == 2 && state[i+12] == 0 && state[i+6] == 1 && i+12 <row)||
                (turn > 0 && turn %2 == 0 && state[i] == 2 && state[i+16] == 0 && state[i+8] == 1 && i+12 <row)){
                a++;
            }
            
        }
        if (j==1){ this_test_jump = a; }
        else if (j==2){ last_test_jump = a;}
        
        // search if there is any Move
        for (int i = 0; i < row; i++) {
            if ((turn > 0 && turn %2 == 1 && state[i] == 1 && state[i-6] == 0 && i-6 >0)||
                (turn > 0 && turn %2 == 1 && state[i] == 1 && state[i-8] == 0 && i-8 >0)||
                
                /////////////////////////////////////////////////////////////////////////////////
                
                (turn > 0 && turn %2 == 0 && state[i] == 2 && state[i+6] == 0 && i+6 <row)||
                (turn > 0 && turn %2 == 0 && state[i] == 2 && state[i+8] == 0 && i+8 <row)){
                b++;
            }
        }
        if (j==1){this_test_move = b;}
        else if (j==2){last_test_move = b; }
        
        turn--;
    }
    
    turn = total_turns;
    
    if ((last_test_jump + last_test_move == 0) && (this_test_jump + this_test_move > 0) && turn > 0 && turn %2 == 0){
        if(player == 2){name = "You ";}
        else name = "AI ";
        cout << "\n" << name <<"(White) Chess Won!!!"; cin >> stop;
    }
    else if ((this_test_jump + this_test_move == 0) && (last_test_jump + last_test_move > 0) && turn > 0 && turn %2 == 1){
        if(player == 1){name = "You ";}
        else name = "AI ";
        cout << "\n" << name << "(Black) Chess Won!!!"; cin >> stop;
    }

    if ((player == 1 && turn %2 == 1)||(player == 2 && turn %2 == 0)){
        if (this_test_jump > 0){check_jump(turn, times, state);}
        else if (this_test_jump == 0 && times == 0){check_move(turn, times, state);}
        else if (this_test_jump == 0 && times > 0){check_win(turn+1, times=0, state);}
    }
    else if ((player == 1 && turn %2 == 0)||(player == 2 && turn %2 == 1)){
        
        int ai_state[row] ={0};
        for(int i = 0; i < row; i++) {
            ai_state[i] = state[i];
        }

        alpha_beta_search(ai_state);
        cout << "Maximum_level: "<< maximum_level <<"\n";
        cout << "total_nodes: "<< total_node <<"\n";
        cout << "total_times_max_pruning: "<< total_times_max_pruning <<"\n";
        cout << "total_times_min_pruning: "<< total_times_min_pruning <<"\n";
        
        draw_chessboard(main_state);
        check_win(turn+1, times = 0, main_state);
    }
    
}

//Choose who move first
void set_player(){
    
    char players; //players input
    cout << "\nWould you like to move first? (e.g: Y/N):";
    cin >> players;
    
    int times = 0;
    player = 0;
    int turn = 0;
    
    if (players == 'y'|| player == 'Y' ){
        player = 1; //go first black chess
        cout << "\nOK, you go first with Black chess!!! \n";
        turn +=1;
    }
    else if (players == 'n'|| player == 'N' ){
        player = 2; //go second white chess
        cout << "\nOK, you go second with White chess!!! \n";
        turn +=1;
    }
    
    else {cout <<"Typo!!!"; set_player();}

    check_win(turn, times, main_state);

}


int ai_terminal(int state[]){
    
    int black = 0;
    int white = 0;
    int value = 0;
    
    int black_jump_value = 0;
    int white_jump_value = 0;
    int black_move_value = 0;
    int white_move_value = 0;
    
    //check this turn and last turn
    for (int i = 0; i < row; i++) {
        if ((state[i] == 1 && state[i-12] == 0 && state[i-6] == 2 && i-12 >0)||  //jump Black state check
            (state[i] == 1 && state[i-16] == 0 && state[i-8] == 2 && i-16 >0)||
            (state[i] == 1 && state[i+12] == 0 && state[i+6] == 2 && i+12 <row)||
            (state[i] == 1 && state[i+16] == 0 && state[i+8] == 2 && i+12 <row)){
            black_jump_value+=1;
            
        }
        
        if ((state[i] == 2 && state[i-12] == 0 && state[i-6] == 1 && i-12 >0)||  //jump White state check
            (state[i] == 2 && state[i-16] == 0 && state[i-8] == 1 && i-16 >0)||
            (state[i] == 2 && state[i+12] == 0 && state[i+6] == 1 && i+12 <row)||
            (state[i] == 2 && state[i+16] == 0 && state[i+8] == 1 && i+12 <row)){
            white_jump_value+=1;
        }
    }
    
    // search if there is any Move
    for (int i = 0; i < row; i++) {
        if ((state[i] == 1 && state[i-6] == 0 && i-6 >0)||
            (state[i] == 1 && state[i-8] == 0 && i-8 >0)){
            black_move_value+=1;
        }
        if ((state[i] == 2 && state[i+6] == 0 && i+6 <row)||
            (state[i] == 2 && state[i+8] == 0 && i+8 <row)){
            white_move_value+=1;
        }
    }
    
    for(int i = 0; i < row; i++) {  //check who's chess are all gone is terminal
        if (state[i] == 1){ black+=1; }
        else if (state[i] == 2){ white+=1; }
    }
    
    int total_black = black_move_value + black_jump_value;
    int total_white = white_move_value + white_jump_value;
    
    if ((total_black > 0) && (total_white == 0)){
        value = 444; //game over to utility value
    }
    
    else if ((total_black == 0) && (total_white > 0)){
        value = -444; //game over to utility value
    }
    
    else if (black == 0 && white > 0){
        value = -666; //game over to utility value
    }
    else if (white == 0 && black > 0){
        value = 666; //game over to utility value
    }
    
    return value;
}


int ai_utility(int state[]){

    int black_jump_value = 0;
    int white_jump_value = 0;
    int black_move_value = 0;
    int white_move_value = 0;
    int total_utility_value = 0;
    int black = 0;
    int white = 0;
    int black_chess = 0;
    int white_chess = 0;
    
    for(int i = 0; i < row; i++) {  //check who's chess are all gone is terminal
        if (state[i] == 1){ black_chess+=1; }
        else if (state[i] == 2){ white_chess+=1; }
    }
    
    //check this turn and last turn
    for (int i = 0; i < row; i++) {
        if ((state[i] == 1 && state[i-12] == 0 && state[i-6] == 2 && i-12 >0)||  //jump Black state check
            (state[i] == 1 && state[i-16] == 0 && state[i-8] == 2 && i-16 >0)||
            (state[i] == 1 && state[i+12] == 0 && state[i+6] == 2 && i+12 <row)||
            (state[i] == 1 && state[i+16] == 0 && state[i+8] == 2 && i+12 <row)){
            black_jump_value+=1;
            
        }
        
        if ((state[i] == 2 && state[i-12] == 0 && state[i-6] == 1 && i-12 >0)||  //jump White state check
            (state[i] == 2 && state[i-16] == 0 && state[i-8] == 1 && i-16 >0)||
            (state[i] == 2 && state[i+12] == 0 && state[i+6] == 1 && i+12 <row)||
            (state[i] == 2 && state[i+16] == 0 && state[i+8] == 1 && i+12 <row)){
            white_jump_value+=1;
        }
    }
    
    // search if there is any Move
    for (int i = 0; i < row; i++) {
        if ((state[i] == 1 && state[i-6] == 0 && i-6 >0)||
            (state[i] == 1 && state[i-8] == 0 && i-8 >0)){
            black_move_value+=1;
        }
        if ((state[i] == 2 && state[i+6] == 0 && i+6 <row)||
            (state[i] == 2 && state[i+8] == 0 && i+8 <row)){
            white_move_value+=1;
        }
    }
        black = (black_move_value * 29) + (black_jump_value * 13);
        white = ((white_move_value * (-29)) + (white_jump_value * (-13)));
    
    if (black_chess > white_chess){
        total_utility_value = black;
    }
    
    if (white_chess >black_chess){
        total_utility_value = white;
    }
    
    if (black_jump_value > white_jump_value){
        total_utility_value = black_jump_value;
    }
    
    if (black_jump_value < white_jump_value){
        total_utility_value = white_jump_value;
    }
    
    if ((black_jump_value == 0 && white_jump_value == 0) && (black_move_value > white_move_value)){
        total_utility_value = black_move_value;
    }
    
    if ((black_jump_value == 0 && white_jump_value == 0) && (black_move_value < white_move_value)){
        total_utility_value = white_move_value;
    }
    
    else {total_utility_value = black + white;}
        
    return total_utility_value;
    
}

int max_value(int depth, int times, int ai_state[], int a, int b, int cutoff_level, int before_jump_state[]){
    int utility_value = 0;
    int value = -999;

    if (ai_terminal(ai_state) == 666){return 666;}
    else if (ai_terminal(ai_state) == -666){return -666;}
    else if (ai_terminal(ai_state) == 444){return 444;}
    else if (ai_terminal(ai_state) == -444){return -444;}
    else if (depth == 0){
        utility_value = ai_utility(ai_state);
        return utility_value;
    }
    
    int z = 0;
    string ai_jump[6]={""}; //most can jump chess is 6;
    //Turn %2 => 1 Black :: 2 White
    for(int i = 0; i < row; i++) {
        if ((ai_state[i] == 1 && ai_state[i-12] == 0 && ai_state[i-6] == 2 && i-12 >0)|| //jump Black state check
            (ai_state[i] == 1 && ai_state[i-16] == 0 && ai_state[i-8] == 2 && i-16 >0)||
            (ai_state[i] == 1 && ai_state[i+12] == 0 && ai_state[i+6] == 2 && i+12 <row)||
            (ai_state[i] == 1 && ai_state[i+16] == 0 && ai_state[i+8] == 2 && i+12 <row)){
            
            ai_jump[z] = numToAlphabet(i);
            z++;
        }
    }
    int how_many_chess_can_jump = z;
    
    if (how_many_chess_can_jump > 0){
        
        string chess;
        string jump;
        string ai_jump_to[4]={""}; //at most 4 direction can be jump
        int x = 0;
        int how_many_place_can_jump_to = 0;
        
        for (int j = 0; j < how_many_chess_can_jump; j++){
            
            chess = ai_jump[j];
            int check_jump = alphabetToNum(chess);
            
            //Turn %2 => 1 Black :: 2 White
            for(int i = 0; i < row; i++) {
                if (ai_state[check_jump-6] == 2 && ai_state[check_jump-12] == 0 && check_jump-12 >0){ //jump Black state check
                    ai_jump_to[x] = numToAlphabet(check_jump-12);
                    x++;}
                
                if (ai_state[check_jump-8] == 2 && ai_state[check_jump-16] == 0 && check_jump-16 >0){
                    ai_jump_to[x] = numToAlphabet(check_jump-16);
                    x++;}
                
                if (ai_state[check_jump+6] == 2 && ai_state[check_jump+12] == 0 && check_jump+12 <row){
                    ai_jump_to[x] = numToAlphabet(check_jump+12);
                    x++;}
                
                if (ai_state[check_jump+8] == 2 && ai_state[check_jump-12] == 0 && check_jump+16 <row){
                    ai_jump_to[x] = numToAlphabet(check_jump+16);
                    x++;}
                
                how_many_place_can_jump_to = x;
                
                for (int k = 0; k < how_many_place_can_jump_to; k++ ){
                    
                    jump = ai_jump_to[k];
                    
                    int new_state[row];
                    for (int i=0; i<row; i++){
                        new_state[i] = ai_state[i];
                    }
                    
                    //Start to jump check
                    int jump_check = alphabetToNum(jump);
                    
                    int old_place = check_jump;
                    int new_place = jump_check;
                    
                    new_state[new_place] = new_state[old_place];
                    new_state[old_place] = 0;
                    
                    if (jump_check == check_jump-16){new_state[check_jump-8] = 0;}
                    else if (jump_check == check_jump+16){new_state[check_jump+8] = 0;}
                    else if (jump_check == check_jump-12){new_state[check_jump-6] = 0;}
                    else if (jump_check == check_jump+12){new_state[check_jump+6] = 0;}
                    
                    if (times < 1){
                        for (int i=0; i<row; i++){
                            before_jump_state[i] = new_state[i]; //save the best min move
                        }
                    }
                    
                    max_value(depth, times+1, new_state, a, b, cutoff_level, before_jump_state);
                    total_node+=1;
                    
                    return value;
                }
            }
        }
    }
    
    else if (how_many_chess_can_jump < 1 && times <= 0){
        
        string ai_move[6]={""}; //at most 6 chess can move
        string ai_move_to[2]={""}; //at most 2 direction can be move to
        string move;
        string chess;
        int y = 0;
        int how_many_place_can_move_to = 0;
        int how_many_chess_can_move = 0;
        
        //Turn %2 => 1 Black :: 2 White
        for(int i = 0; i < row; i++) {
            if ((ai_state[i] == 1 && ai_state[i-6] == 0 && i-6 >0)||
                (ai_state[i] == 1 && ai_state[i-8] == 0 && i-8 >0)){
                
                ai_move[y] = numToAlphabet(i);
                y++;
            }
        }
        
        how_many_chess_can_move = y;
        
        if (how_many_chess_can_move > 0){
            
            for (int j=0; j< how_many_chess_can_move;j++){
                
                chess = ai_move[j];
                int q = 0;
                
                int check_move = alphabetToNum(chess);
                
                //Turn %2 => 1 Black :: 2 White
                if (ai_state[check_move-6] == 0 && ai_state[check_move-6]!= 9 && check_move-6 >0){
                    ai_move_to[q] = numToAlphabet(check_move-6);
                    q++;}
                
                if (ai_state[check_move-8] == 0 && ai_state[check_move-8]!= 9 && check_move-8 >0){
                    ai_move_to[q] = numToAlphabet(check_move-8);
                    q++;}
                
                how_many_place_can_move_to = q;
                
                for (int k = 0; k < how_many_place_can_move_to; k++ ){
                    //Start to move check
                    move = ai_move_to[k];
                    
                    int new_state[row];
                    for (int i=0; i<row; i++){
                        new_state[i] = ai_state[i];
                    }
                    
                    int move_check = alphabetToNum(move);
                    int old_place = check_move;
                    int new_place = move_check;
                    
                    new_state[new_place] = new_state[old_place];
                    new_state[old_place] = 0;
                    
                    int min_values = 0;
                    min_values = min_value(depth-1, times=0, new_state, a, b, cutoff_level+1, before_jump_state);
                    value = max(value, min_values);
                    
                    if (cutoff_level == 1 && max_nextmove < value){
                        max_nextmove = max(max_nextmove, value);
                        for (int i=0; i<row; i++){
                            main_state[i] = ai_state[i];   //save the best min move
                        }
                    }
                    
                    if (value >= b){ total_times_max_pruning+=1; return value; }
                    a = max(a, value);
                    
                    total_node+=1;
                    
                }
            }
        }
    }
    
    else if (how_many_chess_can_jump < 1 && times >=1){
        int min_values = 0;
        min_values = min_value(depth-1, times=0, ai_state, a, b, cutoff_level+1, before_jump_state);
        value = max(value, min_values);
        
        if (cutoff_level == 1 && max_nextmove < value){
            max_nextmove = max(max_nextmove, value);
            for (int i=0; i<row; i++){
                main_state[i] = before_jump_state[i];  //save the best min move
            }
        }

        if (value >= b){ total_times_max_pruning+=1; return value;}
        a = max(a, value);
        
    }
    
    return value;
}

int min_value(int depth, int times, int ai_state[], int a, int b, int cutoff_level, int before_jump_state[]){
    int utility_value = 0;
    int value = 999;
    
    if (ai_terminal(ai_state) == 666){return 666;}
    else if (ai_terminal(ai_state) == -666){return -666;}
    else if (ai_terminal(ai_state) == 444){return 444;}
    else if (ai_terminal(ai_state) == -444){return -444;}
    else if (depth == 0){
        utility_value = ai_utility(ai_state);
        return utility_value;
    }

    int z = 0;
    string ai_jump[6]={""}; //most can jump chess is 6;
    //Turn %2 => 1 Black :: 2 White
    for(int i = 0; i < row; i++) {
        if ((ai_state[i] == 2 && ai_state[i-12] == 0 && ai_state[i-6] == 1 && i-12 >0)||  //jump White state check
            (ai_state[i] == 2 && ai_state[i-16] == 0 && ai_state[i-8] == 1 && i-16 >0)||
            (ai_state[i] == 2 && ai_state[i+12] == 0 && ai_state[i+6] == 1 && i+12 <row)||
            (ai_state[i] == 2 && ai_state[i+16] == 0 && ai_state[i+8] == 1 && i+12 <row)){
            ai_jump[z] = numToAlphabet(i);
            z++;
        }
    }
    
    int how_many_chess_can_jump = z;
    
    if (how_many_chess_can_jump > 0){
        
        string chess;
        string jump;
        string ai_jump_to[4]= {""}; //at most 4 direction can be jump
        int x = 0;
        int how_many_place_can_jump_to = 0;
        
        for (int j = 0; j < how_many_chess_can_jump; j++){
            
            chess = ai_jump[j];
            int check_jump = alphabetToNum(chess);
            
            //Turn %2 => 1 Black :: 2 White
            for(int i = 0; i < row; i++) {
                
                if (ai_state[check_jump-12] == 0 && ai_state[check_jump-6] == 1 && check_jump-12 >0){ //jump White state check
                    ai_jump_to[x] = numToAlphabet(check_jump-12);
                    x++;}
                
                if (ai_state[check_jump-16] == 0 && ai_state[check_jump-8] == 1 && check_jump-16 >0){
                    ai_jump_to[x] = numToAlphabet(check_jump-16);
                    x++;}
                
                if (ai_state[check_jump+12] == 0 && ai_state[check_jump+6] == 1 && check_jump+12 <row){
                    ai_jump_to[x] = numToAlphabet(check_jump+12);
                    x++;}
                
                if (ai_state[check_jump+16] == 0 && ai_state[check_jump+8] == 1 && check_jump+16 <row){
                    ai_jump_to[x] = numToAlphabet(check_jump+16);
                    x++;}
                
                how_many_place_can_jump_to = x;
                
                for (int k = 0; k < how_many_place_can_jump_to; k++ ){
                    jump = ai_jump_to[k];
                    
                    int new_state[row];
                    for (int i=0; i<row; i++){
                        new_state[i] = ai_state[i];
                    }
                    
                    //Start to jump check
                    int jump_check = alphabetToNum(jump);
                    int old_place = check_jump;
                    int new_place = jump_check;
                    
                    new_state[new_place] = new_state[old_place];
                    new_state[old_place] = 0;
                    
                    if (jump_check == check_jump-16){new_state[check_jump-8] = 0;}
                    else if (jump_check == check_jump+16){new_state[check_jump+8] = 0;}
                    else if (jump_check == check_jump-12){new_state[check_jump-6] = 0;}
                    else if (jump_check == check_jump+12){new_state[check_jump+6] = 0;}
                    
                    if (times < 1){
                        for (int i=0; i<row; i++){
                            before_jump_state[i] = new_state[i];  //save the last max move
                        }
                    }
                    
                    min_value(depth, times+1, new_state, a, b, cutoff_level, before_jump_state);
                    total_node+=1;
                    
                    return value;
                }
            }
        }
    }
    
    else if (how_many_chess_can_jump == 0 && times == 0){
        
        string ai_move[6]={""}; //at most 6 chess can move
        string ai_move_to[2]={""}; //at most 2 direction can be move to
        string move;
        string chess;
        int y = 0;
        int how_many_place_can_move_to = 0;
        int how_many_chess_can_move = 0;
        
        //Turn %2 => 1 Black :: 2 White
        for(int i = 0; i < row; i++) {
            if ((ai_state[i] == 2 && ai_state[i+6] == 0 && i+6 <row)||
                (ai_state[i] == 2 && ai_state[i+8] == 0 && i+8 <row)){
                ai_move[y] = numToAlphabet(i);
                y++;
            }
        }
        
        how_many_chess_can_move = y;
        
        if (how_many_chess_can_move > 0){
            
            for (int j=0; j< how_many_chess_can_move;j++){
                chess = ai_move[j];
                int q = 0;
                
                int check_move = alphabetToNum(chess);
                
                //Turn %2 => 1 Black :: 2 White
                if (ai_state[check_move+6] == 0 && ai_state[check_move+6]!= 9  && check_move+6 <row){
                    ai_move_to[q] = numToAlphabet(check_move+6);
                    q++;}
                
                if (ai_state[check_move+8] == 0 && ai_state[check_move+8]!= 9  && check_move+8 <row){
                    ai_move_to[q] = numToAlphabet(check_move+8);
                    q++;}
                
                how_many_place_can_move_to = q;
                
                for (int k = 0; k < how_many_place_can_move_to; k++ ){
                    //Start to move check
                    move = ai_move_to[k];
                    
                    int new_state[row];
                    for (int i=0; i<row; i++){
                        new_state[i] = ai_state[i];
                    }
                    
                    int move_check = alphabetToNum(move);
                    int old_place = check_move;
                    int new_place = move_check;
                    
                    new_state[new_place] = new_state[old_place];
                    new_state[old_place] = 0;
                    int max_values = 0;
                    max_values = max_value(depth-1, times=0, new_state, a, b, cutoff_level+1, before_jump_state);
                    value = min (value, max_values );
                    
                    if (cutoff_level == 1 && min_nextmove > value){
                        min_nextmove = min(min_nextmove, value);
                        for (int i=0; i<row; i++){
                            main_state[i] = ai_state[i];  //save the best min move
                        }
                    }
                    
                    if (value <= a){ total_times_min_pruning+=1; return value;}
                    b = min (b, value);
                    
                    total_node+=1;
                    
                }
            }
        }
    }
    
    else if (how_many_chess_can_jump == 0 && times > 0){
        
        int max_values = 0;
        max_values = max_value(depth-1, times=0, ai_state, a, b, cutoff_level+1, before_jump_state);
        value = min (value, max_values );
        
        if (cutoff_level == 1 && min_nextmove > value){
            min_nextmove = min(min_nextmove, value);
            for (int i=0; i<row; i++){
                main_state[i] = before_jump_state[i]; //save the best min move
            }
        }
        
        if (value <= a){total_times_min_pruning+=1; return value;}
        b = min (b, value);
    }
    
    return value;
}

int alpha_beta_search(int state[]){
    
    int a = -999;
    int b = 999;
    int cutoff_level = 0;
    int depth = maximum_level;
    int times = 0;
    int value = 0 ;
    total_node = 0;
    total_times_max_pruning = 0;
    total_times_min_pruning = 0;
    int before_jump_state[row]={0};
    min_nextmove = 0;
    max_nextmove = 0;

    total_node+=1;
    
    if (player == 1){
        value = min_value(depth, times, state, a, b, cutoff_level, before_jump_state);
    }
    else if (player == 2){
        value = max_value(depth, times, state, a, b, cutoff_level, before_jump_state);
    }
    
    return value;
}

void set_level(){
    int level;
    cout <<"What level would you like to choose?"<<endl;
    cout <<"(1)Easy, (2)Normal, (3)Hard, (4)Very Hard, Pleas type number: ";
    cin >> level;
    //Searching more level is more hard
    if (level ==1){maximum_level = 5;}
    else if (level==2){maximum_level = 10;}
    else if (level ==3){maximum_level = 20;}
    else if (level ==4){maximum_level = 30;}
    else {maximum_level = 40;}
}

//Game On//
////////////////////////////////////////////////////
int main()
{
    draw_chessboard(main_state); //Print Default ChessBoard
    set_level();
    set_player();
    return 0;
}
/////////////////////////////////////////////////////

