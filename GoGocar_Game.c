#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<stdbool.h>
#include<time.h>
#include<stdlib.h>

#define screen_x 120
#define screen_y 40
#define scount_obs 4
#define scount_star 2

HANDLE rHnd;
HANDLE wHnd;
CHAR_INFO consoleBuffer[screen_x * screen_y];
COORD bufferSize = { screen_x, screen_y };
COORD characterPos = { 0, 0 };
SMALL_RECT windowSize = { 0, 0, screen_x - 1, screen_y - 1 };
COORD obstruction[scount_obs];
COORD star[scount_star];
DWORD fdwMode;

char name[50];
int x_car = 49, y_car = 30;
int x_heart = 104, y_heart = 3;
int x_border = 20, y_border = 0;
int x_score = 105, y_score = 1;
int sum = 0;
int score = 0;
int check_obs[scount_obs];
int check_star[scount_star];
int lastest_obs[5] = {-1, -1, -1, -1, -1};
int lastest_star[5] = {-1, -1, -1, -1, -1};
int latest_lane_obs_index;
int lastest_obs_y;
int latest_lane_star_index;
int lastest_star_y;
int delay = 100;
int y;
bool is_start_random_star = false;
bool play = false;
bool hit = false;
bool play_menu = true;
bool endgame = true;
char digit[5];
char ch = ' ';

int setConsole(int x, int y) {
    wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
    SetConsoleScreenBufferSize(wHnd, bufferSize);
    return 0;
}

int setMode() {
    rHnd = GetStdHandle(STD_INPUT_HANDLE);
    fdwMode = ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
    SetConsoleMode (rHnd, fdwMode);
    return 0;
}

void setcursor(bool visible) {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO lpCursor;
    lpCursor.bVisible = visible;
    lpCursor.dwSize = 15;
    SetConsoleCursorInfo(console, &lpCursor);
}

void gotoxy(int x, int y) {
    COORD c = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void fill_buffer_to_console() {
    WriteConsoleOutputA(wHnd, consoleBuffer, bufferSize, characterPos, &windowSize);
}

void clear_buffer() {
    for (int y = 0; y < screen_y; ++y) {
        for (int x = 0; x < screen_x; ++x) {
            consoleBuffer[x + screen_x * y].Char.AsciiChar = ' ';
            consoleBuffer[x + screen_x * y].Attributes = 7;
        }
    }
}

//-------------Menu---------------//

void draw_G(int x, int y) {
    gotoxy(x, y);
    printf(" __________________");
    gotoxy(x, y + 1);
    printf("/                  |");
    gotoxy(x, y + 2);
    printf("|   _______________|");
    gotoxy(x, y + 3);
    printf("|   |");
    gotoxy(x, y + 4);
    printf("|   |   ___________");
    gotoxy(x, y + 5);
    printf("|   |   |          |");
    gotoxy(x, y + 6);
    printf("|   |   |_______   |");
    gotoxy(x, y + 7);
    printf("|   |          |   |");
    gotoxy(x, y + 8);
    printf("|   |__________|   |");
    gotoxy(x, y + 9);
    printf("|                  |");
    gotoxy(x, y + 10);
    printf("|__________________|");
}

void draw_o_big(int x, int y) {
    gotoxy(x, y + 5);
    printf(" ___________");
    gotoxy(x, y + 6);
    printf("|   _____   |");
    gotoxy(x, y + 7);
    printf("|  |     |  |");
    gotoxy(x, y + 8);
    printf("|  |     |  |");
    gotoxy(x, y + 9);
    printf("|  |_____|  |");
    gotoxy(x, y + 10);
    printf("|___________|");
}

void draw_C(int x, int y) {
    gotoxy(x, y);
    printf("__________________");
    gotoxy(x, y + 1);
    printf("|                  |");
    gotoxy(x, y + 2);
    printf("|   _______________|");
    gotoxy(x, y + 3);
    printf("|   |");
    gotoxy(x, y + 4);
    printf("|   |");
    gotoxy(x, y + 5);
    printf("|   |");
    gotoxy(x, y + 6);
    printf("|   |");
    gotoxy(x, y + 7);
    printf("|   |");
    gotoxy(x, y + 8);
    printf("|   |_______________");
    gotoxy(x, y + 9);
    printf("|                   |");
    gotoxy(x, y + 10);
    printf("|___________________|");
}

void draw_a(int x, int y) {
    gotoxy(x, y + 5);
    printf(" ___________");
    gotoxy(x, y + 6);
    printf("|   _____   |");
    gotoxy(x, y + 7);
    printf("|  |     |  |");
    gotoxy(x, y + 8);
    printf("|  |     |  |");
    gotoxy(x, y + 9);
    printf("|  |_____|  |__");
    gotoxy(x, y + 10);
    printf("|______________|");
}

void draw_r_big(int x, int y) {
    gotoxy(x, y + 5);
    printf(" __  ____");
    gotoxy(x, y + 6);
    printf("|  |/    |");
    gotoxy(x, y + 7);
    printf("|    /|__|");
    gotoxy(x, y + 8);
    printf("|   /");
    gotoxy(x, y + 9);
    printf("|  /");
    gotoxy(x, y + 10);
    printf("|_/");
}

void draw_g_big(int x, int y) {
    gotoxy(x, y + 5);
    printf(" _____________");
    gotoxy(x, y + 6);
    printf("|   __________|");
    gotoxy(x, y + 7);
    printf("|  |   _______");
    gotoxy(x, y + 8);
    printf("|  |  |_____  |");
    gotoxy(x, y + 9);
    printf("|  |________| |");
    gotoxy(x, y + 10);
    printf("|_____________|");
}

void draw_a_big(int x, int y) {
    gotoxy(x, y + 6);
    printf("    /\\    ");
    gotoxy(x, y + 7);
    printf("   /  \\   ");
    gotoxy(x, y + 8);
    printf("  / /\\ \\  ");
    gotoxy(x, y + 9);
    printf(" / /__\\ \\ ");
    gotoxy(x, y + 10);
    printf("/________\\");
}

void draw_m_big(int x, int y) {

    gotoxy(x, y + 5);
    printf(" __        __ ");
    gotoxy(x, y + 6);
    printf("|  \\      /  |");
    gotoxy(x, y + 7);
    printf("|   \\    /   |");
    gotoxy(x, y + 8);
    printf("|    \\  /    |");
    gotoxy(x, y + 9);
    printf("|  |\\ \\/ /|  |");
    gotoxy(x, y + 10);
    printf("|__| \\__/ |__|");
}

void draw_e_big(int x, int y) {

    gotoxy(x, y + 5);
    printf(" ____________");
    gotoxy(x, y + 6);
    printf("|   _________|");
    gotoxy(x, y + 7);
    printf("|  |________");
    gotoxy(x, y + 8);
    printf("|   ________|");
    gotoxy(x, y + 9);
    printf("|  |_________");
    gotoxy(x, y + 10);
    printf("|____________|");
}

void draw_v_big(int x, int y) {
    gotoxy(x, y + 5);
    printf("__        __");
    gotoxy(x, y + 6);
    printf("\\ \\      / /");
    gotoxy(x, y + 7);
    printf(" \\ \\    / / ");
    gotoxy(x, y + 8);
    printf("  \\ \\  / /  ");
    gotoxy(x, y + 9);
    printf("   \\ \\/ /   ");
    gotoxy(x, y + 10);
    printf("    \\__/    ");
}

void print_menu(int x, int y) {
    gotoxy(x, y);
    printf("/=====================================\\");
    gotoxy(x, y + 1);
    printf("|                MENU                 |");
    gotoxy(x, y + 2);
    printf("\\=====================================/");
}

void instructions_game(int x, int y) {
    system("cls");
    gotoxy(x, y);
    printf("INSTRUCTIONS");
    gotoxy(x, y + 1);
    printf("------------------------------------------------");
    gotoxy(x, y + 2);
    printf("Avoid car by moving left, right, top and below ");
    gotoxy(x, y + 3);
    printf("Press 'a' to move left");
    gotoxy(x, y + 4);
    printf("Press 'd' to move right");
    gotoxy(x, y + 5);
    printf("Press 'w' to move top");
    gotoxy(x, y + 6);
    printf("Press 's' to move below");
    gotoxy(x, y + 8);
    printf("Press any key to go back to menu");
    getch();
    system("cls");
}

void show_read_name_and_score(int x, int y) {
	FILE *fp;
    struct player {
        char namep[50];
        char scorep[50];
    } p;
    int size = 0;
    fp = fopen("score.txt", "r");

    while (fread(&p,sizeof(struct player),1,fp)!=0)
    {
        size = size + 1;
    }
    fclose(fp);
    fp = fopen("score.txt", "r");
    
    struct player pla[size];
    int score [size] ;
    for(int i = 0 ; i < size ; i++){
        fread(&p,sizeof(struct player),1,fp);
        strcpy(pla[i].namep, p.namep);
        score[i] = atoi(p.scorep);
    }
    gotoxy(x, y);
    printf("Score Top 5 \n");
    for (int i = 0; i < size; ++i)
    {
        for (int j = i + 1; j < size; ++j)
        {
            if (score[i] < score[j])
            {
                int a = score[i];
                char b[50];
                strcpy(b, pla[i].namep);
                score[i] = score[j];
                strcpy(pla[i].namep, pla[j].namep);
                score[j] = a;
                strcpy(pla[j].namep, b);
            }
        }
    }
    for(int i = 0 ; i < 5 ; i++){
    	gotoxy(x, y + ((2*i)+1));
        printf("Top %d \n", i+1);
        gotoxy(x, y + ((2*i)+2));
        printf("Name: %s ", pla[i].namep);
        printf("Score: %d \n", score[i]);
    }
    gotoxy(x, y + 12);
    printf("Press any key to go back to menu");
    fclose(fp);
}

void top_score(int x, int y) {
    system("cls");
    show_read_name_and_score(x, y);
    getch();
    system("cls");
}

void print_sub_menu(int x, int y) {
    gotoxy(x, y);
    printf("1. START GAME");
    gotoxy(x, y + 1);
    printf("2. INSTRUCTIONS");
    gotoxy(x, y + 2);
    printf("3. TOP 5 MOST SCORE");
    gotoxy(x, y + 3);
    printf("4. QUIT");
    gotoxy(x, y + 4);
    printf("SELECT OPTION : ");
}

void print_my_name(int x, int y) {
    gotoxy(x, y);
    printf("64010267  NICHAPAT  UAHCHINKUL");
}

void exit_menu() {
    play_menu = false;
}

void start_game() {
    endgame = false;
}

void print_case_choice() {
    if(_kbhit()) {
        ch = getch();
        if(ch == '1') {
            sum = 0;
            exit_menu();
            start_game();
        }
        if(ch == '2') {
            system("cls");
            instructions_game(45, 10);
        }
        if(ch == '3') {
            system("cls");
            top_score(50, 10);
        }
        if(ch == '4') {
            printf("EXIT");
            exit(0);
        }
    }
}

void enter_name(int x, int y) {
    gotoxy(x, y);
    printf("/=====================================\\");
    gotoxy(x, y + 2);
    printf("\\=====================================/");
    gotoxy(x, y + 1);
    printf("        Enter Your Name : ");
    scanf("%s", name);
}

// fill to buffer

void fill_car_to_buffer() {
    consoleBuffer[x_car + screen_x * y_car].Char.AsciiChar  = '/';
    consoleBuffer[x_car + screen_x * y_car].Attributes = 7;
    consoleBuffer[(x_car + 1) + screen_x * y_car].Char.AsciiChar  = '|';
    consoleBuffer[(x_car + 1) + screen_x * y_car].Attributes = 7;
    consoleBuffer[(x_car + 2) + screen_x * y_car].Char.AsciiChar  = '\\';
    consoleBuffer[(x_car + 2) + screen_x * y_car].Attributes = 7;
    consoleBuffer[(x_car - 1) + screen_x * (y_car + 1)].Char.AsciiChar  = '0';
    consoleBuffer[(x_car - 1) + screen_x * (y_car + 1)].Attributes = 7;
    consoleBuffer[x_car + screen_x * (y_car + 1)].Char.AsciiChar  = '|';
    consoleBuffer[x_car + screen_x * (y_car + 1)].Attributes = 7;
    consoleBuffer[(x_car + 1) + screen_x * (y_car + 1)].Char.AsciiChar  = '|';
    consoleBuffer[(x_car + 1) + screen_x * (y_car + 1)].Attributes = 7;
    consoleBuffer[(x_car + 2) + screen_x * (y_car + 1)].Char.AsciiChar  = '|';
    consoleBuffer[(x_car + 2) + screen_x * (y_car+ 1)].Attributes = 7;
    consoleBuffer[(x_car + 3) + screen_x * (y_car + 1)].Char.AsciiChar  = '0';
    consoleBuffer[(x_car + 3) + screen_x * (y_car + 1)].Attributes = 7;
    consoleBuffer[(x_car - 1) + screen_x * (y_car + 2)].Char.AsciiChar  = '|';
    consoleBuffer[(x_car - 1) + screen_x * (y_car + 2)].Attributes = 7;
    consoleBuffer[x_car + screen_x * (y_car + 2)].Char.AsciiChar  = '|';
    consoleBuffer[x_car + screen_x * (y_car + 2)].Attributes = 7;
    consoleBuffer[(x_car + 1) + screen_x * (y_car + 2)].Char.AsciiChar  = '|';
    consoleBuffer[(x_car + 1) + screen_x * (y_car + 2)].Attributes = 7;
    consoleBuffer[(x_car + 2) + screen_x * (y_car + 2)].Char.AsciiChar  = '|';
    consoleBuffer[(x_car + 2) + screen_x * (y_car + 2)].Attributes = 7;
    consoleBuffer[(x_car + 3) + screen_x * (y_car + 2)].Char.AsciiChar  = '|';
    consoleBuffer[(x_car + 3) + screen_x * (y_car + 2)].Attributes = 7;
    consoleBuffer[(x_car - 1) + screen_x * (y_car + 3)].Char.AsciiChar  = '0';
    consoleBuffer[(x_car - 1) + screen_x * (y_car + 3)].Attributes = 7;
    consoleBuffer[x_car + screen_x * (y_car + 3)].Char.AsciiChar  = '|';
    consoleBuffer[x_car + screen_x * (y_car + 3)].Attributes = 7;
    consoleBuffer[(x_car + 1) + screen_x * (y_car + 3)].Char.AsciiChar  = '|';
    consoleBuffer[(x_car + 1) + screen_x * (y_car + 3)].Attributes = 7;
    consoleBuffer[(x_car + 2) + screen_x * (y_car + 3)].Char.AsciiChar  = '|';
    consoleBuffer[(x_car + 2) + screen_x * (y_car + 3)].Attributes = 7;
    consoleBuffer[(x_car + 3) + screen_x * (y_car + 3)].Char.AsciiChar  = '0';
    consoleBuffer[(x_car + 3) + screen_x * (y_car + 3)].Attributes = 7;
    consoleBuffer[(x_car - 1) + screen_x * (y_car + 4)].Char.AsciiChar  = '-';
    consoleBuffer[(x_car - 1) + screen_x * (y_car + 4)].Attributes = 7;
    consoleBuffer[x_car + screen_x * (y_car + 4)].Char.AsciiChar  = '-';
    consoleBuffer[x_car + screen_x * (y_car + 4)].Attributes = 7;
    consoleBuffer[(x_car + 1) + screen_x * (y_car + 4)].Char.AsciiChar  = '-';
    consoleBuffer[(x_car + 1) + screen_x * (y_car + 4)].Attributes = 7;
    consoleBuffer[(x_car + 2) + screen_x * (y_car + 4)].Char.AsciiChar  = '-';
    consoleBuffer[(x_car + 2) + screen_x * (y_car + 4)].Attributes = 7;
    consoleBuffer[(x_car + 3) + screen_x * (y_car + 4)].Char.AsciiChar  = '-';
    consoleBuffer[(x_car + 3) + screen_x * (y_car + 4)].Attributes = 7;
}

void fill_obstruction_to_buffer() {
    for (int i = 0; i < scount_obs; i++) {
        if (obstruction[i].Y >= 0) {
            consoleBuffer[obstruction[i].X + screen_x * obstruction[i].Y].Char.AsciiChar  = '_';
            consoleBuffer[obstruction[i].X + screen_x * obstruction[i].Y].Attributes = 7;
            consoleBuffer[(obstruction[i].X + 1)  + screen_x * obstruction[i].Y].Char.AsciiChar  = '_';
            consoleBuffer[(obstruction[i].X + 1)  + screen_x * obstruction[i].Y].Attributes = 7;
            consoleBuffer[(obstruction[i].X + 2)  + screen_x * obstruction[i].Y].Char.AsciiChar  = '_';
            consoleBuffer[(obstruction[i].X + 2)  + screen_x * obstruction[i].Y].Attributes = 7;
            consoleBuffer[(obstruction[i].X + 3)  + screen_x * obstruction[i].Y].Char.AsciiChar  = '_';
            consoleBuffer[(obstruction[i].X + 3)  + screen_x * obstruction[i].Y].Attributes = 7;
            consoleBuffer[(obstruction[i].X - 1)  + screen_x * (obstruction[i].Y + 1)].Char.AsciiChar  = '/';
            consoleBuffer[(obstruction[i].X - 1)  + screen_x * (obstruction[i].Y + 1)].Attributes = 7;
            consoleBuffer[obstruction[i].X + screen_x * (obstruction[i].Y + 1)].Char.AsciiChar  = '.';
            consoleBuffer[obstruction[i].X + screen_x * (obstruction[i].Y + 1)].Attributes = 7;
            consoleBuffer[(obstruction[i].X + 1) + screen_x * (obstruction[i].Y + 1)].Char.AsciiChar  = '.';
            consoleBuffer[(obstruction[i].X + 1) + screen_x * (obstruction[i].Y + 1)].Attributes = 7;
            consoleBuffer[(obstruction[i].X + 2) + screen_x * (obstruction[i].Y + 1)].Char.AsciiChar  = '.';
            consoleBuffer[(obstruction[i].X + 2) + screen_x * (obstruction[i].Y + 1)].Attributes = 7;
            consoleBuffer[(obstruction[i].X + 3) + screen_x * (obstruction[i].Y + 1)].Char.AsciiChar  = '.';
            consoleBuffer[(obstruction[i].X + 3) + screen_x * (obstruction[i].Y + 1)].Attributes = 7;
            consoleBuffer[(obstruction[i].X + 4) + screen_x * (obstruction[i].Y + 1)].Char.AsciiChar  = '\\';
            consoleBuffer[(obstruction[i].X + 4) + screen_x * (obstruction[i].Y + 1)].Attributes = 7;
            consoleBuffer[(obstruction[i].X - 1)  + screen_x * (obstruction[i].Y + 2)].Char.AsciiChar  = '|';
            consoleBuffer[(obstruction[i].X - 1)  + screen_x * (obstruction[i].Y + 2)].Attributes = 7;
            consoleBuffer[obstruction[i].X + screen_x * (obstruction[i].Y + 2)].Char.AsciiChar  = '.';
            consoleBuffer[obstruction[i].X + screen_x * (obstruction[i].Y + 2)].Attributes = 7;
            consoleBuffer[(obstruction[i].X + 1) + screen_x * (obstruction[i].Y + 2)].Char.AsciiChar  = '.';
            consoleBuffer[(obstruction[i].X + 1) + screen_x * (obstruction[i].Y + 2)].Attributes = 7;
            consoleBuffer[(obstruction[i].X + 2) + screen_x * (obstruction[i].Y + 2)].Char.AsciiChar  = '.';
            consoleBuffer[(obstruction[i].X + 2) + screen_x * (obstruction[i].Y + 2)].Attributes = 7;
            consoleBuffer[(obstruction[i].X + 3) + screen_x * (obstruction[i].Y + 2)].Char.AsciiChar  = '.';
            consoleBuffer[(obstruction[i].X + 3) + screen_x * (obstruction[i].Y + 2)].Attributes = 7;
            consoleBuffer[(obstruction[i].X + 4) + screen_x * (obstruction[i].Y + 2)].Char.AsciiChar  = '|';
            consoleBuffer[(obstruction[i].X + 4) + screen_x * (obstruction[i].Y + 2)].Attributes = 7;
            consoleBuffer[(obstruction[i].X - 1)  + screen_x * (obstruction[i].Y + 3)].Char.AsciiChar  = '\\';
            consoleBuffer[(obstruction[i].X - 1)  + screen_x * (obstruction[i].Y + 3)].Attributes = 7;
            consoleBuffer[obstruction[i].X  + screen_x * (obstruction[i].Y + 3)].Char.AsciiChar  = '_';
            consoleBuffer[obstruction[i].X  + screen_x * (obstruction[i].Y + 3)].Attributes = 7;
            consoleBuffer[(obstruction[i].X + 1)  + screen_x * (obstruction[i].Y + 3)].Char.AsciiChar  = '_';
            consoleBuffer[(obstruction[i].X + 1)  + screen_x * (obstruction[i].Y + 3)].Attributes = 7;
            consoleBuffer[(obstruction[i].X + 2)  + screen_x * (obstruction[i].Y + 3)].Char.AsciiChar  = '_';
            consoleBuffer[(obstruction[i].X + 2)  + screen_x * (obstruction[i].Y + 3)].Attributes = 7;
            consoleBuffer[(obstruction[i].X + 3)  + screen_x * (obstruction[i].Y + 3)].Char.AsciiChar  = '_';
            consoleBuffer[(obstruction[i].X + 3)  + screen_x * (obstruction[i].Y + 3)].Attributes = 7;
            consoleBuffer[(obstruction[i].X + 4)  + screen_x * (obstruction[i].Y + 3)].Char.AsciiChar  = '/';
            consoleBuffer[(obstruction[i].X + 4)  + screen_x * (obstruction[i].Y + 3)].Attributes = 7;
            consoleBuffer[obstruction[i].X  + screen_x * (obstruction[i].Y + 4)].Char.AsciiChar  = '/';
            consoleBuffer[obstruction[i].X  + screen_x * (obstruction[i].Y + 4)].Attributes = 7;
            consoleBuffer[(obstruction[i].X + 3)  + screen_x * (obstruction[i].Y + 4)].Char.AsciiChar  = '\\';
            consoleBuffer[(obstruction[i].X + 3)  + screen_x * (obstruction[i].Y + 4)].Attributes = 7;
        }
    }
}

void fill_star_to_buffer() {
    for (int i = 0; i < scount_star; i++) {
        if (star[i].Y >= 0) {
            consoleBuffer[star[i].X + screen_x * star[i].Y].Char.AsciiChar  = '*';
            consoleBuffer[star[i].X + screen_x * star[i].Y].Attributes = 7;
            consoleBuffer[(star[i].X - 1)  + screen_x * (star[i].Y + 1)].Char.AsciiChar  = '*';
            consoleBuffer[(star[i].X - 1)  + screen_x * (star[i].Y + 1)].Attributes = 7;
            consoleBuffer[(star[i].X + 1) + screen_x * (star[i].Y + 1)].Char.AsciiChar  = '*';
            consoleBuffer[(star[i].X + 1) + screen_x * (star[i].Y + 1)].Attributes = 7;
            consoleBuffer[star[i].X + screen_x * (star[i].Y + 2)].Char.AsciiChar  = '*';
            consoleBuffer[star[i].X + screen_x * (star[i].Y + 2)].Attributes = 7;
        }
    }
}

void fill_instruction (int x_instruction, int y_instruction) {
	consoleBuffer[(x_instruction) + screen_x * (y_instruction)].Char.AsciiChar  = 'C';
    consoleBuffer[(x_instruction) + screen_x * (y_instruction)].Attributes = 7;
    consoleBuffer[(x_instruction + 1) + screen_x * (y_instruction)].Char.AsciiChar  = 'O';
    consoleBuffer[(x_instruction + 1) + screen_x * (y_instruction)].Attributes = 7;
    consoleBuffer[(x_instruction + 2) + screen_x * (y_instruction)].Char.AsciiChar  = 'N';
    consoleBuffer[(x_instruction + 2) + screen_x * (y_instruction)].Attributes = 7;
    consoleBuffer[(x_instruction + 3) + screen_x * (y_instruction)].Char.AsciiChar  = 'T';
    consoleBuffer[(x_instruction + 3) + screen_x * (y_instruction)].Attributes = 7;
    consoleBuffer[(x_instruction + 4) + screen_x * (y_instruction)].Char.AsciiChar  = 'R';
    consoleBuffer[(x_instruction + 4) + screen_x * (y_instruction)].Attributes = 7;
    consoleBuffer[(x_instruction + 5) + screen_x * (y_instruction)].Char.AsciiChar  = 'O';
    consoleBuffer[(x_instruction + 5) + screen_x * (y_instruction)].Attributes = 7;
    consoleBuffer[(x_instruction + 6) + screen_x * (y_instruction)].Char.AsciiChar  = 'L';
    consoleBuffer[(x_instruction + 6) + screen_x * (y_instruction)].Attributes = 7;
    consoleBuffer[(x_instruction) + screen_x * (y_instruction + 1)].Char.AsciiChar  = '_';
    consoleBuffer[(x_instruction) + screen_x * (y_instruction + 1)].Attributes = 7;
    consoleBuffer[(x_instruction + 1) + screen_x * (y_instruction + 1)].Char.AsciiChar  = '_';
    consoleBuffer[(x_instruction + 1) + screen_x * (y_instruction + 1)].Attributes = 7;
    consoleBuffer[(x_instruction + 2) + screen_x * (y_instruction + 1)].Char.AsciiChar  = '_';
    consoleBuffer[(x_instruction + 2) + screen_x * (y_instruction + 1)].Attributes = 7;
    consoleBuffer[(x_instruction + 3) + screen_x * (y_instruction+ 1)].Char.AsciiChar  = '_';
    consoleBuffer[(x_instruction + 3) + screen_x * (y_instruction + 1)].Attributes = 7;
    consoleBuffer[(x_instruction + 4) + screen_x * (y_instruction + 1)].Char.AsciiChar  = '_';
    consoleBuffer[(x_instruction + 4) + screen_x * (y_instruction + 1)].Attributes = 7;
    consoleBuffer[(x_instruction + 5) + screen_x * (y_instruction + 1)].Char.AsciiChar  = '_';
    consoleBuffer[(x_instruction + 5) + screen_x * (y_instruction + 1)].Attributes = 7;
    consoleBuffer[(x_instruction + 6) + screen_x * (y_instruction + 1)].Char.AsciiChar  = '_';
    consoleBuffer[(x_instruction + 6) + screen_x * (y_instruction + 1)].Attributes = 7;
    
    consoleBuffer[(x_instruction - 2) + screen_x * (y_instruction + 3)].Char.AsciiChar  = 'A';
    consoleBuffer[(x_instruction - 2) + screen_x * (y_instruction + 3)].Attributes = 7;
    consoleBuffer[(x_instruction) + screen_x * (y_instruction + 3)].Char.AsciiChar  = 'K';
    consoleBuffer[(x_instruction) + screen_x * (y_instruction + 3)].Attributes = 7;
    consoleBuffer[(x_instruction + 1) + screen_x * (y_instruction + 3)].Char.AsciiChar  = 'E';
    consoleBuffer[(x_instruction + 1) + screen_x * (y_instruction + 3)].Attributes = 7;
    consoleBuffer[(x_instruction + 2) + screen_x * (y_instruction + 3)].Char.AsciiChar  = 'Y';
    consoleBuffer[(x_instruction + 2) + screen_x * (y_instruction + 3)].Attributes = 7;
    consoleBuffer[(x_instruction + 4) + screen_x * (y_instruction + 3)].Char.AsciiChar  = '-';
    consoleBuffer[(x_instruction + 4) + screen_x * (y_instruction + 3)].Attributes = 7;
    consoleBuffer[(x_instruction + 6) + screen_x * (y_instruction + 3)].Char.AsciiChar  = 'L';
    consoleBuffer[(x_instruction + 6) + screen_x * (y_instruction + 3)].Attributes = 7;
    consoleBuffer[(x_instruction + 7) + screen_x * (y_instruction + 3)].Char.AsciiChar  = 'E';
    consoleBuffer[(x_instruction + 7) + screen_x * (y_instruction + 3)].Attributes = 7;
    consoleBuffer[(x_instruction + 8) + screen_x * (y_instruction + 3)].Char.AsciiChar  = 'F';
    consoleBuffer[(x_instruction + 8) + screen_x * (y_instruction + 3)].Attributes = 7;
    consoleBuffer[(x_instruction + 9) + screen_x * (y_instruction + 3)].Char.AsciiChar  = 'T';
    consoleBuffer[(x_instruction + 9) + screen_x * (y_instruction + 3)].Attributes = 7;
    
    consoleBuffer[(x_instruction - 2) + screen_x * (y_instruction + 4)].Char.AsciiChar  = 'D';
    consoleBuffer[(x_instruction - 2) + screen_x * (y_instruction + 4)].Attributes = 7;
    consoleBuffer[(x_instruction) + screen_x * (y_instruction + 4)].Char.AsciiChar  = 'K';
    consoleBuffer[(x_instruction) + screen_x * (y_instruction + 4)].Attributes = 7;
    consoleBuffer[(x_instruction + 1) + screen_x * (y_instruction + 4)].Char.AsciiChar  = 'E';
    consoleBuffer[(x_instruction + 1) + screen_x * (y_instruction + 4)].Attributes = 7;
    consoleBuffer[(x_instruction + 2) + screen_x * (y_instruction + 4)].Char.AsciiChar  = 'Y';
    consoleBuffer[(x_instruction + 2) + screen_x * (y_instruction + 4)].Attributes = 7;
    consoleBuffer[(x_instruction + 4) + screen_x * (y_instruction + 4)].Char.AsciiChar  = '-';
    consoleBuffer[(x_instruction + 4) + screen_x * (y_instruction + 4)].Attributes = 7;
    consoleBuffer[(x_instruction + 6) + screen_x * (y_instruction + 4)].Char.AsciiChar  = 'R';
    consoleBuffer[(x_instruction + 6) + screen_x * (y_instruction + 4)].Attributes = 7;
    consoleBuffer[(x_instruction + 7) + screen_x * (y_instruction + 4)].Char.AsciiChar  = 'I';
    consoleBuffer[(x_instruction + 7) + screen_x * (y_instruction + 4)].Attributes = 7;
    consoleBuffer[(x_instruction + 8) + screen_x * (y_instruction + 4)].Char.AsciiChar  = 'G';
    consoleBuffer[(x_instruction + 8) + screen_x * (y_instruction + 4)].Attributes = 7;
    consoleBuffer[(x_instruction + 9) + screen_x * (y_instruction + 4)].Char.AsciiChar  = 'H';
    consoleBuffer[(x_instruction + 9) + screen_x * (y_instruction + 4)].Attributes = 7;
    consoleBuffer[(x_instruction + 10) + screen_x * (y_instruction + 4)].Char.AsciiChar  = 'T';
    consoleBuffer[(x_instruction + 10) + screen_x * (y_instruction + 4)].Attributes = 7;
    
    consoleBuffer[(x_instruction - 2) + screen_x * (y_instruction + 5)].Char.AsciiChar  = 'W';
    consoleBuffer[(x_instruction - 2) + screen_x * (y_instruction + 5)].Attributes = 7;
    consoleBuffer[(x_instruction) + screen_x * (y_instruction + 5)].Char.AsciiChar  = 'K';
    consoleBuffer[(x_instruction) + screen_x * (y_instruction + 5)].Attributes = 7;
    consoleBuffer[(x_instruction + 1) + screen_x * (y_instruction + 5)].Char.AsciiChar  = 'E';
    consoleBuffer[(x_instruction + 1) + screen_x * (y_instruction + 5)].Attributes = 7;
    consoleBuffer[(x_instruction + 2) + screen_x * (y_instruction + 5)].Char.AsciiChar  = 'Y';
    consoleBuffer[(x_instruction + 2) + screen_x * (y_instruction + 5)].Attributes = 7;
    consoleBuffer[(x_instruction + 4) + screen_x * (y_instruction + 5)].Char.AsciiChar  = '-';
    consoleBuffer[(x_instruction + 4) + screen_x * (y_instruction + 5)].Attributes = 7;
    consoleBuffer[(x_instruction + 6) + screen_x * (y_instruction + 5)].Char.AsciiChar  = 'T';
    consoleBuffer[(x_instruction + 6) + screen_x * (y_instruction + 5)].Attributes = 7;
    consoleBuffer[(x_instruction + 7) + screen_x * (y_instruction + 5)].Char.AsciiChar  = 'O';
    consoleBuffer[(x_instruction + 7) + screen_x * (y_instruction + 5)].Attributes = 7;
    consoleBuffer[(x_instruction + 8) + screen_x * (y_instruction + 5)].Char.AsciiChar  = 'P';
    consoleBuffer[(x_instruction + 8) + screen_x * (y_instruction + 5)].Attributes = 7;
    
    consoleBuffer[(x_instruction - 2) + screen_x * (y_instruction + 6)].Char.AsciiChar  = 'S';
    consoleBuffer[(x_instruction - 2) + screen_x * (y_instruction + 6)].Attributes = 7;
    consoleBuffer[(x_instruction) + screen_x * (y_instruction + 6)].Char.AsciiChar  = 'K';
    consoleBuffer[(x_instruction) + screen_x * (y_instruction + 6)].Attributes = 7;
    consoleBuffer[(x_instruction + 1) + screen_x * (y_instruction + 6)].Char.AsciiChar  = 'E';
    consoleBuffer[(x_instruction + 1) + screen_x * (y_instruction + 6)].Attributes = 7;
    consoleBuffer[(x_instruction + 2) + screen_x * (y_instruction + 6)].Char.AsciiChar  = 'Y';
    consoleBuffer[(x_instruction + 2) + screen_x * (y_instruction + 6)].Attributes = 7;
    consoleBuffer[(x_instruction + 4) + screen_x * (y_instruction + 6)].Char.AsciiChar  = '-';
    consoleBuffer[(x_instruction + 4) + screen_x * (y_instruction + 6)].Attributes = 7;
    consoleBuffer[(x_instruction + 6) + screen_x * (y_instruction + 6)].Char.AsciiChar  = 'B';
    consoleBuffer[(x_instruction + 6) + screen_x * (y_instruction + 6)].Attributes = 7;
    consoleBuffer[(x_instruction + 7) + screen_x * (y_instruction + 6)].Char.AsciiChar  = 'E';
    consoleBuffer[(x_instruction + 7) + screen_x * (y_instruction + 6)].Attributes = 7;
    consoleBuffer[(x_instruction + 8) + screen_x * (y_instruction + 6)].Char.AsciiChar  = 'L';
    consoleBuffer[(x_instruction + 8) + screen_x * (y_instruction + 6)].Attributes = 7;
    consoleBuffer[(x_instruction + 9) + screen_x * (y_instruction + 6)].Char.AsciiChar  = 'O';
    consoleBuffer[(x_instruction + 9) + screen_x * (y_instruction + 6)].Attributes = 7;
    consoleBuffer[(x_instruction + 10) + screen_x * (y_instruction + 6)].Char.AsciiChar  = 'W';
    consoleBuffer[(x_instruction + 10) + screen_x * (y_instruction + 6)].Attributes = 7;
}

void fill_heart_to_buffer() {
    if(sum < 3)
    {
        consoleBuffer[x_heart + screen_x * y_heart].Char.AsciiChar  = '*';
        consoleBuffer[x_heart + screen_x * y_heart].Attributes = 7;
        consoleBuffer[(x_heart + 2) + screen_x * y_heart].Char.AsciiChar  = '*';
        consoleBuffer[(x_heart + 2) + screen_x * y_heart].Attributes = 7;
        consoleBuffer[(x_heart - 1) + screen_x * (y_heart + 1)].Char.AsciiChar  = '*';
        consoleBuffer[(x_heart - 1) + screen_x * (y_heart + 1)].Attributes = 7;
        consoleBuffer[(x_heart + 1) + screen_x * (y_heart + 1)].Char.AsciiChar  = '*';
        consoleBuffer[(x_heart + 1) + screen_x * (y_heart + 1)].Attributes = 7;
        consoleBuffer[(x_heart + 3) + screen_x * (y_heart + 1)].Char.AsciiChar  = '*';
        consoleBuffer[(x_heart + 3) + screen_x * (y_heart + 1)].Attributes = 7;
        consoleBuffer[x_heart + screen_x * (y_heart + 2)].Char.AsciiChar  = '*';
        consoleBuffer[x_heart + screen_x * (y_heart + 2)].Attributes = 7;
        consoleBuffer[(x_heart + 2) + screen_x * (y_heart + 2)].Char.AsciiChar  = '*';
        consoleBuffer[(x_heart + 2) + screen_x * (y_heart + 2)].Attributes = 7;
        consoleBuffer[(x_heart + 1) + screen_x * (y_heart + 3)].Char.AsciiChar  = '*';
        consoleBuffer[(x_heart + 1) + screen_x * (y_heart + 3)].Attributes = 7;
    }
    if(sum < 2)
    {
        consoleBuffer[(x_heart + 6) + screen_x * y_heart].Char.AsciiChar  = '*';
        consoleBuffer[(x_heart + 6) + screen_x * y_heart].Attributes = 7;
        consoleBuffer[(x_heart + 8) + screen_x * y_heart].Char.AsciiChar  = '*';
        consoleBuffer[(x_heart + 8) + screen_x * y_heart].Attributes = 7;
        consoleBuffer[(x_heart + 5) + screen_x * (y_heart + 1)].Char.AsciiChar  = '*';
        consoleBuffer[(x_heart + 5) + screen_x * (y_heart + 1)].Attributes = 7;
        consoleBuffer[(x_heart + 7) + screen_x * (y_heart + 1)].Char.AsciiChar  = '*';
        consoleBuffer[(x_heart + 7) + screen_x * (y_heart + 1)].Attributes = 7;
        consoleBuffer[(x_heart + 9) + screen_x * (y_heart + 1)].Char.AsciiChar  = '*';
        consoleBuffer[(x_heart + 9) + screen_x * (y_heart + 1)].Attributes = 7;
        consoleBuffer[(x_heart + 6) + screen_x * (y_heart + 2)].Char.AsciiChar  = '*';
        consoleBuffer[(x_heart + 6) + screen_x * (y_heart + 2)].Attributes = 7;
        consoleBuffer[(x_heart + 8) + screen_x * (y_heart + 2)].Char.AsciiChar  = '*';
        consoleBuffer[(x_heart + 8) + screen_x * (y_heart + 2)].Attributes = 7;
        consoleBuffer[(x_heart + 7) + screen_x * (y_heart + 3)].Char.AsciiChar  = '*';
        consoleBuffer[(x_heart + 7) + screen_x * (y_heart + 3)].Attributes = 7;
    }
    if(sum < 1)
    {
        consoleBuffer[(x_heart + 12) + screen_x * y_heart].Char.AsciiChar  = '*';
        consoleBuffer[(x_heart + 12) + screen_x * y_heart].Attributes = 7;
        consoleBuffer[(x_heart + 14) + screen_x * y_heart].Char.AsciiChar  = '*';
        consoleBuffer[(x_heart + 14) + screen_x * y_heart].Attributes = 7;
        consoleBuffer[(x_heart + 11) + screen_x * (y_heart + 1)].Char.AsciiChar  = '*';
        consoleBuffer[(x_heart + 11) + screen_x * (y_heart + 1)].Attributes = 7;
        consoleBuffer[(x_heart + 13) + screen_x * (y_heart + 1)].Char.AsciiChar  = '*';
        consoleBuffer[(x_heart + 13) + screen_x * (y_heart + 1)].Attributes = 7;
        consoleBuffer[(x_heart + 15) + screen_x * (y_heart + 1)].Char.AsciiChar  = '*';
        consoleBuffer[(x_heart + 15) + screen_x * (y_heart + 1)].Attributes = 7;
        consoleBuffer[(x_heart + 12) + screen_x * (y_heart + 2)].Char.AsciiChar  = '*';
        consoleBuffer[(x_heart + 12) + screen_x * (y_heart + 2)].Attributes = 7;
        consoleBuffer[(x_heart + 14) + screen_x * (y_heart + 2)].Char.AsciiChar  = '*';
        consoleBuffer[(x_heart + 14) + screen_x * (y_heart + 2)].Attributes = 7;
        consoleBuffer[(x_heart + 13) + screen_x * (y_heart + 3)].Char.AsciiChar  = '*';
        consoleBuffer[(x_heart + 13) + screen_x * (y_heart + 3)].Attributes = 7;
    }
    
}

void fill_name_to_buffer(int x_name, int y_name) {
	consoleBuffer[(x_name) + screen_x * (y_name)].Char.AsciiChar  = '6';
    consoleBuffer[(x_name) + screen_x * (y_name)].Attributes = 7;
    consoleBuffer[(x_name + 1) + screen_x * (y_name)].Char.AsciiChar  = '4';
    consoleBuffer[(x_name + 1) + screen_x * (y_name)].Attributes = 7;
    consoleBuffer[(x_name + 2) + screen_x * (y_name)].Char.AsciiChar  = '0';
    consoleBuffer[(x_name + 2) + screen_x * (y_name)].Attributes = 7;
    consoleBuffer[(x_name + 3) + screen_x * (y_name)].Char.AsciiChar  = '1';
    consoleBuffer[(x_name + 3) + screen_x * (y_name)].Attributes = 7;
    consoleBuffer[(x_name + 4) + screen_x * (y_name)].Char.AsciiChar  = '0';
    consoleBuffer[(x_name + 4) + screen_x * (y_name)].Attributes = 7;
    consoleBuffer[(x_name + 5) + screen_x * (y_name)].Char.AsciiChar  = '2';
    consoleBuffer[(x_name + 5) + screen_x * (y_name)].Attributes = 7;
    consoleBuffer[(x_name + 6) + screen_x * (y_name)].Char.AsciiChar  = '6';
    consoleBuffer[(x_name + 6) + screen_x * (y_name)].Attributes = 7;
    consoleBuffer[(x_name + 7) + screen_x * (y_name)].Char.AsciiChar  = '7';
    consoleBuffer[(x_name + 7) + screen_x * (y_name)].Attributes = 7;
    
    consoleBuffer[(x_name - 5) + screen_x * (y_name + 1)].Char.AsciiChar  = 'N';
    consoleBuffer[(x_name - 5) + screen_x * (y_name + 1)].Attributes = 7;
    consoleBuffer[(x_name - 4) + screen_x * (y_name + 1)].Char.AsciiChar  = 'I';
    consoleBuffer[(x_name - 4) + screen_x * (y_name + 1)].Attributes = 7;
    consoleBuffer[(x_name - 3) + screen_x * (y_name + 1)].Char.AsciiChar  = 'C';
    consoleBuffer[(x_name - 3) + screen_x * (y_name + 1)].Attributes = 7;
    consoleBuffer[(x_name - 2) + screen_x * (y_name + 1)].Char.AsciiChar  = 'H';
    consoleBuffer[(x_name - 2) + screen_x * (y_name + 1)].Attributes = 7;
    consoleBuffer[(x_name - 1) + screen_x * (y_name + 1)].Char.AsciiChar  = 'A';
    consoleBuffer[(x_name - 1) + screen_x * (y_name + 1)].Attributes = 7;
    consoleBuffer[(x_name) + screen_x * (y_name + 1)].Char.AsciiChar  = 'P';
    consoleBuffer[(x_name) + screen_x * (y_name + 1)].Attributes = 7;
    consoleBuffer[(x_name + 1) + screen_x * (y_name + 1)].Char.AsciiChar  = 'A';
    consoleBuffer[(x_name + 1) + screen_x * (y_name + 1)].Attributes = 7;
    consoleBuffer[(x_name + 2) + screen_x * (y_name + 1)].Char.AsciiChar  = 'T';
    consoleBuffer[(x_name + 2) + screen_x * (y_name + 1)].Attributes = 7;
    
    consoleBuffer[(x_name + 3) + screen_x * (y_name + 2)].Char.AsciiChar  = 'U';
    consoleBuffer[(x_name + 3) + screen_x * (y_name + 2)].Attributes = 7;
    consoleBuffer[(x_name + 4) + screen_x * (y_name + 2)].Char.AsciiChar  = 'A';
    consoleBuffer[(x_name + 4) + screen_x * (y_name + 2)].Attributes = 7;
    consoleBuffer[(x_name + 5) + screen_x * (y_name + 2)].Char.AsciiChar  = 'H';
    consoleBuffer[(x_name + 5) + screen_x * (y_name + 2)].Attributes = 7;
    consoleBuffer[(x_name + 6) + screen_x * (y_name + 2)].Char.AsciiChar  = 'C';
    consoleBuffer[(x_name + 6) + screen_x * (y_name + 2)].Attributes = 7;
    consoleBuffer[(x_name + 7) + screen_x * (y_name + 2)].Char.AsciiChar  = 'H';
    consoleBuffer[(x_name + 7) + screen_x * (y_name + 2)].Attributes = 7;
    consoleBuffer[(x_name + 8) + screen_x * (y_name + 2)].Char.AsciiChar  = 'I';
    consoleBuffer[(x_name + 8) + screen_x * (y_name + 2)].Attributes = 7;
    consoleBuffer[(x_name + 9) + screen_x * (y_name + 2)].Char.AsciiChar  = 'N';
    consoleBuffer[(x_name + 9) + screen_x * (y_name + 2)].Attributes = 7;
    consoleBuffer[(x_name + 10) + screen_x * (y_name + 2)].Char.AsciiChar  = 'K';
    consoleBuffer[(x_name + 10) + screen_x * (y_name + 2)].Attributes = 7;
    consoleBuffer[(x_name + 11) + screen_x * (y_name + 2)].Char.AsciiChar  = 'U';
    consoleBuffer[(x_name + 11) + screen_x * (y_name + 2)].Attributes = 7;
    consoleBuffer[(x_name + 12) + screen_x * (y_name + 2)].Char.AsciiChar  = 'L';
    consoleBuffer[(x_name + 12) + screen_x * (y_name + 2)].Attributes = 7;
}

//---------------- obs : random & fall & hit ------------------//

void random_obstruction_road(int index) {
    int num = (rand() % screen_x) % 5;
    int latest_lane_obs_index = lastest_obs[num];
    int latest_lane_star_index = lastest_star[num];
    int min_y = 0;
    int updated_obs_y = 0;

    obstruction[index].X = (20 * num) + 8;
    
	if (latest_lane_obs_index != -1 && latest_lane_star_index == -1) {
		min_y = obstruction[latest_lane_obs_index].Y;
	}
    
    if (latest_lane_obs_index == -1 && latest_lane_star_index != -1) {
    	min_y = star[latest_lane_star_index].Y;
	}
	
	if (latest_lane_obs_index != -1 && latest_lane_star_index != -1) {
		int lastest_star_y = star[latest_lane_star_index].Y;
        int lastest_obs_y = obstruction[latest_lane_obs_index].Y;
        
        if (lastest_star_y > lastest_obs_y) {
        	min_y = lastest_obs_y;
		} else {
			min_y = lastest_star_y;
		}
	}
	
	if (min_y < 10) {
		updated_obs_y = min_y - (10 - rand() % 5);
	} else {
		updated_obs_y = 1;
	}
	
	obstruction[index].Y = updated_obs_y;
	lastest_obs[num] = index;
}

void random_obstruction() {
    for (int i = 0; i < scount_obs; i++) {
        int lane = (rand() % screen_x) % 5;
        
        if (lastest_obs[lane] == -1) {
        	obstruction[i].X = (20 * lane) + 8;
        	obstruction[i].Y = -4;
        	lastest_obs[lane] = i;
		} else {
			int latest_lane_obs_index = lastest_obs[lane];
			lastest_obs_y = obstruction[latest_lane_obs_index].Y;
			
			obstruction[i].X = (20 * lane) + 8;
			
			if (lastest_obs_y < 10) {
            	obstruction[i].Y = lastest_obs_y - (10 - rand() % 5);
        	} else {
            	obstruction[i].Y = 1;
        	}

        	lastest_obs[lane] = i;
		}
    }
}

void obstruction_fall() {
    for (int i = 0; i < scount_obs; i++) {
        if (obstruction[i].Y >= screen_y - 5) {
            random_obstruction_road(i);
        } else {
            obstruction[i].Y = obstruction[i].Y + 1;
        }
    }
}

void reset_obs(int index) {
    random_obstruction_road(index);
}

bool check_front_hit(int i) {
    for(int car_x = 0; car_x < 3; car_x++) {
        if((x_car + car_x == obstruction[i].X + car_x || x_car + car_x == obstruction[i].X + 3) && (y_car == obstruction[i].Y + 5)) {
            reset_obs(i);
            return true;
        }
    }
    
    return false;
}

bool check_right_hit(int i) {
    for(int car_x = -1; car_x < 4; car_x++) {
        for(int y_all = 0; y_all < 5; y_all++) {
            if((x_car + car_x == obstruction[i].X + car_x || x_car + car_x == obstruction[i].X + car_x + 1) && ((y_car + y_all == obstruction[i].Y) || (y_car + y_all == obstruction[i].Y + 1) || (y_car + y_all == obstruction[i].Y + 2) || (y_car + y_all == obstruction[i].Y + 3))) {
                reset_obs(i);
                return true;
            }
        }
    }
    
    return false;
}

bool check_left_hit(int i) {
    for(int car_x = 4; car_x > -2; car_x--) {
        for(int y_all = 0; y_all < 5; y_all++) {
            if((x_car + car_x == obstruction[i].X + car_x || x_car + car_x == obstruction[i].X - 1 || x_car + car_x == obstruction[i].X || x_car + car_x == obstruction[i].X + 1 || x_car + car_x == obstruction[i].X + 4) && (y_car + y_all == obstruction[i].Y)) {
                reset_obs(i);
                return true;
            }
        }
    }
    
    return false;
}

int check_hit_all_obs() {

    for(int i = 0; i < scount_obs; i++) {
        if(check_front_hit(i) == true) {
        	Beep(700, 100);
            return 1;
        }
        if(check_right_hit(i) == true) {
        	Beep(700, 100);
            return 1;
        }
        if(check_left_hit(i) == true) {
        	Beep(700, 100);
            return 1;
        }
    }
    return 0;
}

//------------------star : random & fall & hit--------------------//

void random_star_road(int index) {
    int num = (rand() % screen_x) % 5;
    int latest_lane_obs_index = lastest_obs[num];
    int latest_lane_star_index = lastest_star[num];
    int min_y = 0;
    int updated_star_y = 0;

    star[index].X = (20 * num) + 10;
    
	if (latest_lane_obs_index != -1 && latest_lane_star_index == -1) {
		min_y = obstruction[latest_lane_obs_index].Y;
	}
    
    if (latest_lane_obs_index == -1 && latest_lane_star_index != -1) {
    	min_y = star[latest_lane_star_index].Y;
	}
	
	if (latest_lane_obs_index != -1 && latest_lane_star_index != -1) {
		int lastest_star_y = star[latest_lane_star_index].Y;
        int lastest_obs_y = obstruction[latest_lane_obs_index].Y;
        
        if (lastest_star_y > lastest_obs_y) {
        	min_y = lastest_obs_y;
		} else {
			min_y = lastest_star_y;
		}
	}
	
	if (min_y < 10) {
		updated_star_y = min_y - (10 - rand() % 5);
	} else {
		updated_star_y = 1;
	}
	
	star[index].Y = updated_star_y;
	lastest_star[num] = index;
}

void random_star() {
    for (int i = 0; i < scount_star; i++) {
        int lane = (rand() % screen_x) % 5;
		
        int latest_lane_star_index = lastest_star[lane];
    	int latest_lane_obs_index = lastest_obs[lane];

    	star[i].X = (20 * lane) + 10;
    	
    	if (latest_lane_star_index != -1) {
    		lastest_star_y = star[latest_lane_star_index].Y;
        	lastest_obs_y = obstruction[latest_lane_obs_index].Y;
        	
			if(lastest_star_y > lastest_obs_y) {
            	lastest_star_y = obstruction[latest_lane_obs_index].Y;
        	} else {
            	lastest_star_y = star[latest_lane_star_index].Y;
        	}
        	
        	if (lastest_star_y < 10) {
            	star[i].Y = lastest_star_y - (9 - rand() % 5);
        	} else {
            	star[i].Y = 1;
        	}
        	
        	lastest_star[lane] = i;
		} else {
        	lastest_obs_y = obstruction[latest_lane_obs_index].Y;
        	
        	if (lastest_obs_y < 10) {
            	star[i].Y = lastest_obs_y - (9 - rand() % 5);
        	}
        	
        	lastest_star[lane] = i;
    	}
    }
}

void star_fall() {
    for (int i = 0; i < scount_star; i++) {
        if (star[i].Y >= screen_y - 5) {
            random_star_road(i);
        } else {
            star[i].Y = star[i].Y + 1;
        }
    }
}

void reset_star(int index) {
    random_star_road(index);
}

void check_hit_star() {
    for(int i = 0; i < scount_star; i++) {
        for (int car_x = 0; car_x < 3; car_x++) {
            for(int car_y = 0; car_y < 5; car_y++) {
                if((x_car + car_x == star[i].X || x_car + car_x == star[i].X - 1 || x_car + car_x == star[i].X + 1) && (y_car + car_y == star[i].Y || y_car + car_y == star[i].Y + 1 || y_car + car_y == star[i].Y + 2)) {
					reset_star(i);
                    score = score + 50;
                    if(delay > 20 && delay < 75)
                    delay = delay + 26;
                }
            }
        }
    }
}

//---------------score & border & game over-----------//

void get_score_to_show() {
    int input;
    int output;
    int summand;
    int divisor = 10000;

    input = score;
    for(int i = 0; i < 5; i++) {
        output = input / divisor;
        summand = input % divisor;
        digit[i] = output + '0';
        input = summand;
        divisor = divisor/10;
    }
}

void save_score() {
	struct player {
    char namep[50];
    char scorep[50];
    } p1;

    char str_score[50];
    sprintf(str_score, "%d", score);
    strcpy( p1.namep, name);
    strcpy( p1.scorep, str_score);
    FILE * fp;
    fp = fopen("score.txt", "a");
    fwrite(&p1, sizeof(struct player),1,fp);
    fclose(fp);
}

void write_score() {
    consoleBuffer[(x_score) + screen_x * (y_score)].Char.AsciiChar  = 'S';
    consoleBuffer[(x_score) + screen_x * (y_score)].Attributes = 7;
    consoleBuffer[(x_score + 1) + screen_x * (y_score)].Char.AsciiChar  = 'C';
    consoleBuffer[(x_score + 1) + screen_x * (y_score)].Attributes = 7;
    consoleBuffer[(x_score + 2) + screen_x * (y_score)].Char.AsciiChar  = 'O';
    consoleBuffer[(x_score + 2) + screen_x * (y_score)].Attributes = 7;
    consoleBuffer[(x_score + 3) + screen_x * (y_score)].Char.AsciiChar  = 'R';
    consoleBuffer[(x_score + 3) + screen_x * (y_score)].Attributes = 7;
    consoleBuffer[(x_score + 4) + screen_x * (y_score)].Char.AsciiChar  = 'E';
    consoleBuffer[(x_score + 4) + screen_x * (y_score)].Attributes = 7;
    consoleBuffer[(x_score + 5) + screen_x * (y_score)].Char.AsciiChar  = ' ';
    consoleBuffer[(x_score + 5) + screen_x * (y_score)].Attributes = 7;
    consoleBuffer[(x_score + 6) + screen_x * (y_score)].Char.AsciiChar  = ':';
    consoleBuffer[(x_score + 6) + screen_x * (y_score)].Attributes = 7;
    consoleBuffer[(x_score + 7) + screen_x * (y_score)].Char.AsciiChar  = ' ';
    consoleBuffer[(x_score + 7) + screen_x * (y_score)].Attributes = 7;
    consoleBuffer[(x_score + 8) + screen_x * (y_score)].Char.AsciiChar  = digit[0];
    consoleBuffer[(x_score + 8) + screen_x * (y_score)].Attributes = 7;
    consoleBuffer[(x_score + 9) + screen_x * (y_score)].Char.AsciiChar  = digit[1];
    consoleBuffer[(x_score + 9) + screen_x * (y_score)].Attributes = 7;
    consoleBuffer[(x_score + 10) + screen_x * (y_score)].Char.AsciiChar = digit[2];
    consoleBuffer[(x_score + 10) + screen_x * (y_score)].Attributes = 7;
    consoleBuffer[(x_score + 11) + screen_x * (y_score)].Char.AsciiChar = digit[3];
    consoleBuffer[(x_score + 11) + screen_x * (y_score)].Attributes = 7;
    consoleBuffer[(x_score + 12) + screen_x * (y_score)].Char.AsciiChar = digit[4];
    consoleBuffer[(x_score + 12) + screen_x * (y_score)].Attributes = 7;
}

void draw_border() {
    for (int i = 0; i < screen_y; i++) {
        consoleBuffer[x_border + screen_x * (y_border + i)].Char.AsciiChar  = '|';
        consoleBuffer[x_border + screen_x * (y_border + i)].Attributes = 7;
    }

    for (int i = 0; i < screen_y; i++) {
        consoleBuffer[(x_border + 20) + screen_x * (y_border + i)].Char.AsciiChar  = '|';
        consoleBuffer[(x_border + 20) + screen_x * (y_border + i)].Attributes = 7;
    }

    for (int i = 0; i < screen_y; i++) {
        consoleBuffer[(x_border + 40) + screen_x * (y_border + i)].Char.AsciiChar  = '|';
        consoleBuffer[(x_border + 40) + screen_x * (y_border + i)].Attributes = 7;
    }

    for (int i = 0; i < screen_y; i++) {
        consoleBuffer[(x_border + 60) + screen_x * (y_border + i)].Char.AsciiChar  = '|';
        consoleBuffer[(x_border + 60) + screen_x * (y_border + i)].Attributes = 7;
    }

    for (int i = 0; i < screen_y; i++) {
        consoleBuffer[(x_border + 80) + screen_x * (y_border + i)].Char.AsciiChar  = '|';
        consoleBuffer[(x_border + 80) + screen_x * (y_border + i)].Attributes = 7;
    }
}

void game_over() {
	Beep(900, 300);
    system("cls");
    draw_g_big(30, 5);
    draw_a_big(48, 5);
    draw_m_big(60, 5);
    draw_e_big(75, 5);
    draw_o_big(35, 12);
    draw_v_big(50, 12);
    draw_e_big(63, 12);
    draw_r_big(78, 12);
    enter_name(40, 25);
    system("cls");
}

//-----------------car---------------------//

void move_car() {
    DWORD numEvents = 0;
    DWORD numEventsRead = 0;
    int min_x_car = 9;
    int max_x_car = 85;
    int min_y_car = 30;
    int max_y_car = 34;

    GetNumberOfConsoleInputEvents(rHnd, &numEvents);
    if (numEvents != 0) {
        INPUT_RECORD eventBuffer[numEvents];
        ReadConsoleInput(rHnd, eventBuffer, numEvents, &numEventsRead);

        for (DWORD i = 0; i < numEventsRead; ++i) {
            if (eventBuffer[i].EventType == KEY_EVENT && eventBuffer[i].Event.KeyEvent.bKeyDown == true ) {
                if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE) {
                	game_over();
                	save_score();
                	exit(0);
                }
                if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'a' && x_car > min_x_car) {
                    x_car = x_car - 20;
                }
                if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'd' && x_car <= max_x_car) {
                    x_car = x_car + 20;
                }
                if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 's' && y_car <= max_y_car) {
                    y_car = y_car + 5;
                }
                if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'w' && y_car >= min_y_car) {
                    y_car = y_car - 5;
                }
            }
        }
    }
}

//--------------------Menu-----------------//

void menu_play() {
    system("cls");
    do {
        draw_G(30, 1);
        draw_o_big(50, 1);
        draw_G(63, 1);
        draw_o_big(83, 1);
        draw_C(40, 13);
        draw_a(61, 13);
        draw_r_big(77, 13);
        print_menu(43, 25);
        print_sub_menu(55, 30);
        print_case_choice();
        print_my_name(90, 38);
    } while (play_menu);
}

//--------------check-------------------//

void check_delay() {
    if(delay > 25){
        delay = delay - 1;
    }
}

void check_star_fall_if_enough_score() {
    if(score >= 50) {
    	int i;
    	if (is_start_random_star == false) {
    		
			random_star();
			is_start_random_star = true;
		} else {
			fill_star_to_buffer();
        	star_fall();
		}
    }
}

void restart_game() {
    clear_buffer();
    sum = 0;
    score = 0;
    delay = 100;
    menu_play();
    x_car = 49, y_car = 30;
    int lastest_obs[5] = {-1, -1, -1, -1, -1};
	int lastest_star[5] = {-1, -1, -1, -1, -1};
	is_start_random_star = false;
    start_game();
    
}

void check_end_game_and_re() {
    if(sum == 3) {
    	endgame = true;
        game_over();
        save_score();
        play_menu = true;
        restart_game();
        system("cls");
        random_obstruction();
        
        do {
        	fill_name_to_buffer(107,36);
    		fill_instruction(107,10);
            check_star_fall_if_enough_score();
            get_score_to_show();
            write_score();
            fill_car_to_buffer();
            move_car();
            fill_heart_to_buffer();
            fill_obstruction_to_buffer();
            fill_buffer_to_console();
            obstruction_fall();
            clear_buffer();
            check_hit_star();
            draw_border();
            check_delay();
            Sleep(delay);
            score++;
            sum = sum + check_hit_all_obs();
            check_end_game_and_re();
        } while (endgame != true );
        
        system("cls");
    }
}

//---------------play--------------------//

void function_play(){
    sum = 0;
    system("cls");
    random_obstruction();
    
    do {
    	fill_name_to_buffer(107,36);
    	fill_instruction(107,10);
        check_star_fall_if_enough_score();
        get_score_to_show();
        write_score();
        fill_car_to_buffer();
        move_car();
        fill_heart_to_buffer();
        fill_obstruction_to_buffer();
        fill_buffer_to_console();
        obstruction_fall();
        clear_buffer();
        check_hit_star();
        draw_border();
        check_delay();
        Sleep(delay);
        score++;
        sum = sum + check_hit_all_obs();
        check_end_game_and_re();
    } while (endgame != true );

    system("cls");
}

int main() {
    srand(time(NULL));
    setConsole(screen_x, screen_y);
    setcursor(0);
    setMode();

    menu_play();
    function_play();

    return 0;
}
