// preprocessor macros
#define SETTINGS "settings.txt"

// global variables
bool dark_mode;
char* dark_bg = "COLOR 2";
char* white_bg = "COLOR B7";
int todays_event, next_event;

enum _color_codes{
    black_on_black, indigo_on_black, green_on_black,
    blue_on_black, red_on_black, purple_on_black,
    yellow_on_black, white_on_black, grey_on_black,
    indigo_on_white = 113, green_on_white, blue_on_white,
    red_on_white, purple_on_white, yellow_on_white,
    grey_on_white, black_on_white, white_theme = 188, white_selected,
    white_not_selected = 176
};

enum _control_characters{
    backspace = 8, enter = 13, escape = 27, tilde = 96, stilde = 126,
    up_arrow = 72, down_arrow = 80
};

enum _color_codes selected;
enum _color_codes not_selected;
enum _color_codes theme;


// function prototypes
void color(int color);
void gotoxy(int x, int y);
void print_theme();
void start_up();
char login_menu();
char main_menu();
char password_manager_menu();
char diary_menu();
int todo_menu();
void load_settings();
void save_settings();
void toggle_darkmode();
void OpenReadMeFile();
void cheat_code(char ch); // :)
// taken from todolist
void Progress_Bar(void);
void Show_Bar (int);

// function definitions
void color(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void gotoxy(int x, int y)
{
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void print_theme()
{
    system("cls");
    color(theme);
    printf("\t\t _       _________ ______   _______  _        _        _______ \n"
           "\t\t( \\      \\__   __/(   ___\\ (  ____ \\( \\      ( \\      (  ___  )\n"
           "\t\t| (         ) (   | (   ) )| (    \\/| (      | (      | (   ) |\n"
           "\t\t| |         | |   | (__/ / | (__    | |      | |      | |   | |\n"
           "\t\t| |         | |   |  __ (  |  __)   | |      | |      | |   | |\n"
           "\t\t| |         | |   | (  \\ \\ | (      | |      | |      | |   | |\n"
           "\t\t| (____/\\___) (___| )___) )| (____/\\| (____/\\| (____/\\| (___) |\n"
           "\t\t(_______/\\_______/|/ \\___/ (_______/(_______/(_______/(_______)\n");

    gotoxy(2, 22);
    if(special_days[todays_event][0] != 0)
        printf("Today is: %s", special_days[todays_event]);
    gotoxy(2, 23);
    if(todays_event == 366)
    printf("Next event is: %s in 8 days", special_days[67]);
    else if(next_event != 366)
    printf("Next event is: %s in %d days", special_days[next_event], next_event-todays_event);
    gotoxy(2, 24);
    printf("The application was openned at %d/%d/%d", date, month, year);
    color(selected);
}

void start_up()
{
    load_settings();
    color(theme);
    printf("\t _    _        _                                 _             _      _  _            _  _        \n"
            "\t| |  | |      | |                               | |           | |    (_)| |          | || |       \n"
            "\t| |  | |  ___ | |  ___   ___   _ __ ___    ___  | |__   ___   | |     _ | |__    ___ | || |  ___  \n"
            "\t| |/\\| | / __\\| | / __| / _ \\ | '_ ` _ \\  / _ \\ |  __| / _ \\  | |    | || '_ \\  / _ \\| || | / _ \\ \n"
            "\t\\  /\\  /|  __/| || (__ | (_) || | | | | ||  __/ |  |_ | (_) | | |____| || |_) ||  __/| || || (_) |\n"
            "\t \\/  \\/  \\___||_| \\___| \\___/ |_| |_| |_| \\___|  \\___| \\___/  \\_____/|_||_.__/  \\___||_||_| \\___/ \n");


    printf("\n\n");
    printf("\t\t  _____                         ______         _               \n"
           "\t\t |  __ \\                       |  ____|       | |              \n"
           "\t\t | |__) |_ __   ___  ___  ___  | |__    _ __  | |_   ___  _ __  \n"
           "\t\t |  ___/| '__| / _ \\/ __|/ __| |  __|  | '_ \\ | __| / _ \\| '__| \n"
           "\t\t | |    | |   |  __/\\__ \\\\__ \\ | |____ | | | || |_ |  __/| |    \n"
           "\t\t |_|    |_|    \\___||___/|___/ |______||_| |_| \\__| \\___||_|    \n"
           "\t\t                                                              \n"
           "\t\t                                                              \n"
           "\t\t  _            _____                 _    _                     \n"
           "\t\t | |          / ____|               | |  (_)                    \n"
           "\t\t | |_   ___   | |       ___   _ __  | |_  _  _ __   _   _   ___ \n"
           "\t\t | __| / _ \\  | |      / _ \\ | '_ \\ | __|| || '_ \\ | | | | / _ \\\n"
           "\t\t | |_ | (_) | | |____ | (_) || | | || |_ | || | | || |_| ||  __/\n"
           "\t\t  \\__| \\___/   \\_____| \\___/ |_| |_| \\__||_||_| |_| \\__,_| \\___|");

    todays_event = days_gone(month, date);
    if(todays_event == 366)
        next_event = 67;
    else next_event = todays_event+1;
    while(special_days[next_event][0] == 0)
        next_event++;

    char ch;
    while((ch = _getch()) != enter);
    system("cls");
}
char login_menu()
{
    print_theme();

    int set_color[] = {selected, not_selected, not_selected, not_selected, not_selected, not_selected};
    int counter = 0, options = 6, x_offset, y_offset;
    char key;
    x_offset = 40;
    y_offset = 5;
    while (1)
    {
        gotoxy(x_offset +4 -5, y_offset +5);
        color(set_color[0]);
        if(set_color[0] == selected)
            printf("->>> Register <<<-");
        else printf("     Register     ");

        gotoxy(x_offset +5 -5, y_offset +6);
        color(set_color[1]);
        if(set_color[1] == selected)
            printf("->>> Login <<<-");
        else printf("     Login     ");

        gotoxy(x_offset +1 -5, y_offset +7);
        color(set_color[2]);
        if(set_color[2] == selected)
            printf("->>> List all users <<<-");
        else printf("     List all users     ");

        gotoxy(x_offset +2 -5, y_offset +8);
        color(set_color[3]);
        if(set_color[3] == selected)
            printf("->>> Change theme <<<-");
        else printf("     Change theme     ");

        gotoxy(x_offset +0 -5, y_offset +9);
        color(set_color[4]);
        if(set_color[4] == selected)
            printf("->>> Open readme file <<<-");
        else printf("     Open readme file     ");

        gotoxy(x_offset +4 -5, y_offset +10);
        color(set_color[5]);
        if(set_color[5] == selected)
        {
            printf("->>> Exit :( <<<-");
            gotoxy(x_offset +4 -1, y_offset +11);
            printf("NOOOOOOOO");
        }
        else
        {
            printf("     Exit :(     ");
            gotoxy(x_offset +4 -1, y_offset +11);
            printf("         ");
        }

        gotoxy(0,0);
        key = _getch();
        cheat_code(key);

        if (key == -32 || key == 0)
            key = _getch();
        if (key == up_arrow)
            counter = (counter + options -1) % options;
        if (key == down_arrow)
            counter = (counter + 1) % options;

        if (key == enter)
        {
            fflush(stdin);
            system("cls");
            color(selected);
            return counter +1 +'0';
        }

        set_color[counter] = selected;
        set_color[(counter + 1) % options] = not_selected;
        set_color[(counter + options -1) % options] = not_selected;
    }
}

char main_menu()//diary-rem-pm, changeinfo-logout-deleteacc
{
    print_theme();

    int set_color[] = {selected, not_selected, not_selected, not_selected, not_selected, not_selected};
    int counter = 0, options = 6, x_offset, y_offset;
    x_offset = 40;
    y_offset = 5;
    char key;
    while (1)
    {
        gotoxy(x_offset +3 -5, y_offset +5);
        color(set_color[0]);
        if(set_color[0] == selected)
            printf("->>> Diary <<<-");
        else printf("     Diary     ");

        gotoxy(x_offset -5 -5, y_offset +6);
        color(set_color[1]);
        if(set_color[1] == selected)
            printf("->>> To-do List / Reminders <<<-");
        else printf("     To-do List / Reminders     ");

        gotoxy(x_offset -2 -5, y_offset +7);
        color(set_color[2]);
        if(set_color[2] == selected)
            printf("->>> Password Manager <<<-");
        else printf("     Password Manager     ");

        gotoxy(x_offset -2 -5, y_offset +8);
        color(set_color[3]);
        if(set_color[3] == selected)
            printf("->>> Change login info <<<-");
        else printf("     Change login info     ");

        gotoxy(x_offset +3 -5, y_offset +9);
        color(set_color[4]);
        if(set_color[4] == selected)
            printf("->>> Logout <<<-");
        else printf("     Logout     ");

        gotoxy(x_offset -1 -5, y_offset +10);
        color(set_color[5]);
        if(set_color[5] == selected)
            printf("->>> Delete account <<<-");
        else printf("     Delete account     ");

        gotoxy(0,0);
        key = _getch();
        cheat_code(key);

        if (key == -32 || key == 0)
            key = _getch();
        if (key == up_arrow)
            counter = (counter + options -1) % options;
        if (key == down_arrow)
            counter = (counter + 1) % options;

        if (key == enter)
        {
            fflush(stdin);
            system("cls");
            color(selected);
            return counter +1 +'0';
        }

        set_color[counter] = selected;
        set_color[(counter + 1) % options] = not_selected;
        set_color[(counter + options -1) % options] = not_selected;
    }
}

char password_manager_menu()//add-search-remove-listall-strength-gen-exit
{
    print_theme();

    int set_color[] = {selected, not_selected, not_selected, not_selected, not_selected, not_selected, not_selected};
    int counter = 0, options = 7, x_offset, y_offset;
    x_offset = 40;
    y_offset = 5;
    char key;
    while (1)
    {
        gotoxy(x_offset +0 -4, y_offset +5);
        color(set_color[0]);
        if(set_color[0] == selected)
            printf("->>> Add entry <<<-");
        else printf("     Add entry     ");

        gotoxy(x_offset +0 -4, y_offset +6);
        color(set_color[1]);
        if(set_color[1] == selected)
            printf("->>> Find entry <<<-");
        else printf("     Find entry     ");

        gotoxy(x_offset -1 -4, y_offset +7);
        color(set_color[2]);
        if(set_color[2] == selected)
            printf("->>> Remove entry <<<-");
        else printf("     Remove entry     ");

        gotoxy(x_offset -1 -5, y_offset +8);
        color(set_color[3]);
        if(set_color[3] == selected)
            printf("->>> List all entries <<<-");
        else printf("     List all entries     ");

        gotoxy(x_offset -5 -5, y_offset +9);
        color(set_color[4]);
        if(set_color[4] == selected)
            printf("->>> Password strength checker <<<-");
        else printf("     Password strength checker     ");

        gotoxy(x_offset -2 -5, y_offset +10);
        color(set_color[5]);
        if(set_color[5] == selected)
            printf("->>> Password generator <<<-");
        else printf("     Password generator     ");


        gotoxy(x_offset +4 -5, y_offset +11);
        color(set_color[6]);
        if(set_color[6] == selected)
            printf("->>> Exit <<<-");
        else printf("     Exit     ");

        gotoxy(0,0);
        key = _getch();

        if (key == -32 || key == 0)
            key = _getch();
        if (key == up_arrow)
            counter = (counter + options -1) % options;
        if (key == down_arrow)
            counter = (counter + 1) % options;

        if (key == enter)
        {
            fflush(stdin);
            system("cls");
            color(selected);
            return counter +1 +'0';
        }

        set_color[counter] = selected;
        set_color[(counter + 1) % options] = not_selected;
        set_color[(counter + options -1) % options] = not_selected;
    }
}

char diary_menu()
{
    print_theme();

    int set_color[] = {selected, not_selected, not_selected, not_selected, not_selected, not_selected, not_selected};
    int counter = 0, options = 7, x_offset, y_offset;
    x_offset = 40;
    y_offset = 5;
    char key;
    while (1)
    {
        gotoxy(x_offset +0 -3, y_offset +5);
        color(set_color[0]);
        if(set_color[0] == selected)
            printf("->>> Add entry <<<-");
        else printf("     Add entry     ");

        gotoxy(x_offset +0 -3, y_offset +6);
        color(set_color[1]);
        if(set_color[1] == selected)
            printf("->>> Load entry <<<-");
        else printf("     Load entry     ");

        gotoxy(x_offset -1 -3, y_offset +7);
        color(set_color[2]);
        if(set_color[2] == selected)
            printf("->>> Remove entry <<<-");
        else printf("     Remove entry     ");

        gotoxy(x_offset -1 -5, y_offset +8);
        color(set_color[3]);
        if(set_color[3] == selected)
            printf("->>> List all entries <<<-");
        else printf("     List all entries     ");

        gotoxy(x_offset -5 +2, y_offset +9);
        color(set_color[4]);
        if(set_color[4] == selected)
            printf("->>> Edit list <<<-");
        else printf("     Edit list     ");

        gotoxy(x_offset -2 -2, y_offset +10);
        color(set_color[5]);
        if(set_color[5] == selected)
            printf("->>> Search entry <<<-");
        else printf("     Search entry     ");


        gotoxy(x_offset +4 -5, y_offset +11);
        color(set_color[6]);
        if(set_color[6] == selected)
            printf("->>> Exit <<<-");
        else printf("     Exit     ");

        gotoxy(0,0);
        key = _getch();

        if (key == -32 || key == 0)
            key = _getch();
        if (key == up_arrow)
            counter = (counter + options -1) % options;
        if (key == down_arrow)
            counter = (counter + 1) % options;

        if (key == enter)
        {
            fflush(stdin);
            system("cls");
            color(selected);
            return counter +1 +'0';
        }

        set_color[counter] = selected;
        set_color[(counter + 1) % options] = not_selected;
        set_color[(counter + options -1) % options] = not_selected;
    }
}

int todo_menu()
{
    print_theme();

    int set_color[] = {selected, not_selected, not_selected, not_selected, not_selected, not_selected, not_selected, not_selected};
    int counter = 0, options = 8, x_offset, y_offset;
    x_offset = 40;
    y_offset = 5;
    char key;
    while (1)
    {
        gotoxy(x_offset +0 -3, y_offset +5);
        color(set_color[0]);
        if(set_color[0] == selected)
            printf("->>> Add entry <<<-");
        else printf("     Add entry     ");

        gotoxy(x_offset +0 -3, y_offset +6);
        color(set_color[1]);
        if(set_color[1] == selected)
            printf("->>> Edit entry <<<-");
        else printf("     Edit entry     ");

        gotoxy(x_offset -1 -3, y_offset +7);
        color(set_color[2]);
        if(set_color[2] == selected)
            printf("->>> Search entry <<<-");
        else printf("     Search entry     ");

        gotoxy(x_offset -1 -3, y_offset +8);
        color(set_color[3]);
        if(set_color[3] == selected)
            printf("->>> Remove entry <<<-");
        else printf("     Remove entry     ");

        gotoxy(x_offset -5 -0, y_offset +9);
        color(set_color[4]);
        if(set_color[4] == selected)
            printf("->>> List all entries <<<-");
        else printf("     List all entries     ");

        gotoxy(x_offset -2 -4, y_offset +10);
        color(set_color[5]);
        if(set_color[5] == selected)
            printf("->>> Mark entry as done <<<-");
        else printf("     Mark entry as done     ");


        gotoxy(x_offset +4 -8, y_offset +11);
        color(set_color[6]);
        if(set_color[6] == selected)
            printf("->>> Calendar View <<<-");
        else printf("     Calendar View     ");

        gotoxy(x_offset +4 -4, y_offset +12);
        color(set_color[7]);
        if(set_color[7] == selected)
            printf("->>> Exit <<<-");
        else printf("     Exit     ");

        color(not_selected);
        Progress_Bar();

        gotoxy(0,0);
        key = _getch();

        if (key == -32 || key == 0)
            key = _getch();
        if (key == up_arrow)
            counter = (counter + options -1) % options;
        if (key == down_arrow)
            counter = (counter + 1) % options;

        if (key == enter)
        {
            fflush(stdin);
            system("cls");
            color(selected);
            return counter +1;
        }

        set_color[counter] = selected;
        set_color[(counter + 1) % options] = not_selected;
        set_color[(counter + options -1) % options] = not_selected;
    }
}

void load_settings()
{
    FILE* fp = fopen(SETTINGS, "r");
    if(fp)
        fscanf(fp,"dark mode = %d", &dark_mode);
    else dark_mode = 1;

    if(dark_mode)
    {
        system(dark_bg);
        selected = yellow_on_black;
        not_selected = red_on_black;
        theme = green_on_black;
    }
    else
    {
        system(white_bg);
        selected = white_selected;
        not_selected = white_not_selected;
        theme = white_theme;
    }

    fclose(fp);
}

void save_settings()
{
    FILE* fp = fopen(SETTINGS, "w");
    fprintf(fp, "dark mode = %d", dark_mode);
    fclose(fp);
}

void toggle_darkmode()
{
    dark_mode = !dark_mode;
    if(dark_mode)
    {
        system(dark_bg);
        selected = 6;
        not_selected = 4;
        theme = 2;
    }
    else
    {
        system(white_bg);
        selected = white_selected;
        not_selected = white_not_selected;
        theme = white_theme;
    }
    save_settings();
}

void OpenReadMeFile()
{
    system("cls");
    printf(read_me_text);
    system("pause");
}

void cheat_code(char ch)
{
    static int flashbang = 0, christmas = 0, sus = 0;
    if((ch == 'F' || ch == 'f') && flashbang == 0) flashbang++;
    else if((ch == 'L' || ch == 'l') && flashbang == 1) flashbang++;
    else if((ch == 'A' || ch == 'a') && flashbang == 2) flashbang++;
    else if((ch == 'S' || ch == 's') && flashbang == 3) flashbang++;
    else if((ch == 'H' || ch == 'h') && flashbang == 4) flashbang++;
    else if((ch == 'B' || ch == 'b') && flashbang == 5) flashbang++;
    else if((ch == 'A' || ch == 'a') && flashbang == 6) flashbang++;
    else if((ch == 'N' || ch == 'n') && flashbang == 7) flashbang++;
    else if((ch == 'G' || ch == 'g') && flashbang == 8) flashbang++;
    else flashbang = 0;

    if(flashbang == 9)
    {
        system("cls");
        for(int i = 0 ; i < 10 ; i++)
        {
            if(i%2 == 0) system(dark_bg);
            else system(white_bg);
            Sleep(200);
        }
        if(dark_mode) system(dark_bg);
        else system(white_bg);
        flashbang = 0;
        print_theme();
    }

    if((ch == 'C' || ch == 'c') && christmas == 0) christmas++;
    else if((ch == 'H' || ch == 'h') && christmas == 1) christmas++;
    else if((ch == 'R' || ch == 'r') && christmas == 2) christmas++;
    else if((ch == 'I' || ch == 'i') && christmas == 3) christmas++;
    else if((ch == 'S' || ch == 's') && christmas == 4) christmas++;
    else if((ch == 'T' || ch == 't') && christmas == 5) christmas++;
    else if((ch == 'M' || ch == 'm') && christmas == 6) christmas++;
    else if((ch == 'A' || ch == 'a') && christmas == 7) christmas++;
    else if((ch == 'S' || ch == 's') && christmas == 8) christmas++;
    else christmas = 0;

    if(christmas == 9)
    {
        system("cls");
        christmas_music();
        christmas = 0;
        print_theme();
    }

    if((ch == 'S' || ch == 's') && sus == 0) sus++;
    else if((ch == 'U' || ch == 'u') && sus == 1) sus++;
    else if((ch == 'S' || ch == 's') && sus == 2) sus++;
    else sus = 0;

    if(sus == 3)
    {
        sus = 0;
        system("cls");
        printf("                 @@@@@                  \n"
               "             @@@@     @@@               \n"
               "      @@@@@@@@      @@@@@@@@@        \n"
               "    @@      @     @@          @@       \n"
               "   @@      @@     @          @@        \n"
               "   @       @      @@         @@         \n"
               "    @@     @         @@@@@@@@           \n"
               "     @@@@@@@                @            \n"
               "           @     @@@@@      @            \n"
               "           @    @     @     @            \n"
               "           @    @     @     @             \n"
               "           @    @     @     @             \n"
               "           @    @     @    @@             \n"
               "             @@@        @@@               \n"
               "                                         \n"
               "                                        \n");
        Sleep(1000);
        // system("pause");
        system("cls");
        print_theme();
    }
}
