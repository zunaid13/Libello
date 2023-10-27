// preprocessor declarations
#define listMaxIndex 50
#define maxListLength 100
#define SUBLEN 100 +2
#define DIARY "diaryinfo.bin"

// variables to store file names (doesnt have to be global)
char mainfile[MAXFILENAMELEN];
char listfile[MAXFILENAMELEN];

// global typedefs and structures
typedef struct _info{
    char subject[SUBLEN];
    int d, m, y;
} info;

// global variables
int databaseLength = 0, databaseCapacity = 4; // main array
int textLength = 0, textCapacity = 50;  // main body informations
char* text; // main body
int listLast = 0, listLength[listMaxIndex]; // length of list indx
char list[listMaxIndex][maxListLength]; // list array
enum _color_codes highlight_color, normal_color;
bool highlight_mode = 0;
info* database;
// taken from todolist
int dayNum[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};


// function prototypes
int isTagged();
void removeTag(int tag_type);
void serializeDiary();
void deserializeDiary();
void addInfo(char subject[SUBLEN], int d, int m, int y);
int removeInfo(int indx);
void listEntries();
void userScan();
void userListScan();
void searchDiary();
bool diarySubstringCheck(char* large, char* small);
void scan_entry(int indx);
void edit_list(int indx);
void add_list();
void remove_list(int indx);
void remove_nth_listend_tag_from_body(int indx, int n);
void print_list_only();
void serialize_list_only(FILE* fp);
bool check_list_endtag(char* s);
void remove_list_endtag(char* s);
int frontTagged(FILE* fp, FILE* ivan);
void readFromFile(int indx);
void readListFromFile(FILE* ivan);

int diary(void)
{
    database = (info*) malloc(databaseCapacity * sizeof(info));
    text = (char*) malloc(textCapacity * sizeof(char));
    normal_color = not_selected;
    highlight_color = selected;
    while(1)
    {
        deserializeDiary();
        char command = diary_menu();
        color(normal_color);
        if(command == '7') break;
        if(command == '1')
        {
            printf("Today is %2.2d/%2.2d/%4.4d\n", date, month, year);
            printf("Date (format dd/mm/yyyy): ");
            int d, m, y;
            scanf("%d / %d / %d", &d, &m, &y);
            fflush(stdin);
            if(d < 1 || m < 1 || ((m != 2 && d > dayNum[m-1]) || (m == 2 && isLeapYear(y) && d > 29) || (m == 2 && d > 28)) || m > 12)
            {
                printf("Invalid time!\n");
                system("pause");
                continue;
            }

            char subject[SUBLEN] = {};
            printf("Subject: ");
            fgets(subject, SUBLEN, stdin);
            addInfo(subject, d, m, y);
            printf("------------------>> ENTRY NO %d  <<--------------------\n", databaseLength);
            serializeDiary();
            userScan();

            mainfile[0] = listfile[0] = 0;
            num_to_string(mainfile, databaseLength-1);
            strcpy(listfile, mainfile);
            strcat(mainfile, ".txt");
            strcat(listfile, "list.txt");
            FILE* fp = fopen(mainfile, "w");
            fprintf(fp,"%s", text);
            fclose(fp);
            fp = fopen(listfile, "w");
            for(int i = 0 ; i < listLast-1 ; i++)
                fprintf(fp,"%s\n", list[i]);
            fprintf(fp, "%s", list[listLast-1]);
            fclose(fp);
            printf("\n");
        }
        if(command == '2')
        {
            printf("Give the number to load : ");
            int indx;
            scanf("%d", &indx);
            fflush(stdin);
            if(indx < 1 || indx > databaseLength)
            {
                printf("<ENTRY DOESNT EXIST>\n");
                system("pause");
                continue;
            }
            indx--;
            printf("Date: %2.2d/%2.2d/%4.4d\n", database[indx].d, database[indx].m, database[indx].y);
            printf("Subject: %s", database[indx].subject);
            printf("------------------------------------------------\n");
            readFromFile(indx);
            printf("\n");
        }
        if(command == '3')
        {
            printf("Give the number to delete : ");
            int indx;
            scanf("%d", &indx);
            fflush(stdin);
            if(indx < 1 || indx > databaseLength)
            {
                printf("<ENTRY DOESNT EXIST>\n");
                system("pause");
                continue;
            }
            indx--;
            removeInfo(indx);
            printf("<SUCCESSFULLY REMOVED THIS ENTRY>\n");
        }
        if(command == '4')
            listEntries();
        if(command == '5')
        {
            printf("Which entry would you like to edit? Ans: ");
            int indx;
            scanf("%d", &indx);
            fflush(stdin);
            if(indx < 1 || indx > databaseLength)
            {
                printf("<ENTRY DOESNT EXIST>\n");
                system("pause");
                continue;
            }
            indx--;
            edit_list(indx);
        }
        if(command == '6')
            searchDiary();
        system("pause");
    }
    free(text);
    free(database);
    return 0;
}

int isTagged()
{
    char checker[5] = {};
    for(int i = textLength-3 ; i < textLength ; i++)
        checker[i-textLength+3] = text[i];
    if(!strcmp(checker, "<h>"))
        return 1;
    if(!strcmp(checker, "<l>"))
        return 3;
    for(int i = textLength-4 ; i < textLength ; i++)
        checker[i-textLength+4] = text[i];
    if(!strcmp(checker, "</h>"))
        return 2;
    if(!strcmp(checker, "</l>"))
        return 4;
    return 0;
}

void removeTag(int tag_type)
{
    if(tag_type == 1 || tag_type == 2)
    {
        for(int i = textLength-3 ; i < textLength ; i++)
            text[i] = 0;
        textLength -= 3;
    }
    else if(tag_type == 3 || tag_type == 4)
    {
        for(int i = textLength-4 ; i < textLength ; i++)
            text[i] = 0;
        textLength -= 4;
    }
}

void serializeDiary()
{
    FILE* fp = fopen(DIARY, "wb");
    for(int i = 0 ; i < databaseLength ; i++)
        fwrite(&database[i], sizeof(database[0]), 1, fp);
    fclose(fp);
}

void deserializeDiary()
{
    databaseCapacity = 4;
    databaseLength = 0;
    FILE* fp = fopen(DIARY, "rb");
    if(fp == NULL)
        return;
    while(1)
    {
        info temp;
        int check = fread(&temp, sizeof(temp), 1, fp);
        if(check == 0)
            break;
        addInfo(temp.subject, temp.d, temp.m, temp.y);
    }
    fclose(fp);
}

void addInfo(char subject[SUBLEN], int d, int m, int y)
{
    strcpy(database[databaseLength].subject, subject);
    database[databaseLength].d = d;
    database[databaseLength].m = m;
    database[databaseLength].y = y;
    databaseLength++;
    if(databaseLength == databaseCapacity)
    {
        databaseCapacity *= 2;
        database = (info*) realloc(database, databaseCapacity * sizeof(info));
    }
}

int removeInfo(int indx)
{
    databaseLength--;
    char entry_name[MAXFILENAMELEN] = {};
    char list_name[MAXFILENAMELEN] = {};
    num_to_string(entry_name, indx);
    strcpy(list_name, entry_name);
    strcat(entry_name, ".txt");
    strcat(list_name, "list.txt");
    remove(entry_name);
    remove(list_name);

    char temp_name[MAXFILENAMELEN] = {};
    char temp_list_name[MAXFILENAMELEN] = {};
    num_to_string(temp_name, databaseLength);
    strcpy(temp_list_name, temp_name);
    strcat(temp_name, ".txt");
    strcat(temp_list_name, "list.txt");
    rename(temp_name, entry_name);
    rename(temp_list_name, list_name);

    database[indx] = database[databaseLength];
    serializeDiary();
    return 0;
}

void searchDiary()
{
    printf("Now we will insert the date range.\n "
     "Input a negative value to disable this filter\n");
    printf("Input starting date (dd/mm/yyyy): ");
    int sd, sm, sy; // start day-month-year
    int ed, em, ey; // ending day-month-year
    bool dateFilter, subFilter, bodyFilter;
    scanf("%d / %d / %d", &sd, &sm, &sy);
    fflush(stdin);
    if(sd == 0 || sm == 0 || ((sm != 2 && sd > dayNum[sm-1]) || (sm == 2 && isLeapYear(sy) && sd > 29) || (sm == 2 && sd > 28)) || sm > 12)
    {
        printf("Invalid time!\n");
        system("pause");
        return;
    }
    if(sd > 0 && sm > 0 && sy > 0)
    {
        printf("Input ending date (dd/mm/yyyy): ");
        scanf("%d / %d / %d", &ed, &em, &ey);
        fflush(stdin);
        if(ed < 1 || em < 1 || ((em != 2 && ed > dayNum[em-1]) || (em == 2 && isLeapYear(ey) && ed > 29) || (em == 2 && ed > 28)) || em > 12)
        {
            printf("Invalid time!\n");
            system("pause");
            return;
        }
        dateFilter = 1;
    }
    else dateFilter = 0;
    char sub_search[SUBLEN] = {}, body_search[SUBLEN * 5] = {};
    printf("Enter Subject Filter.\nBlank input disables this filter.\n");
    printf("Subject (can be a substring) : ");
    fgets(sub_search, SUBLEN, stdin);
    fflush(stdin);
    printf("Enter Body Filter.\nBlank input disables this filter.\n");
    printf("Body substring: ");
    fgets(body_search, 5 * SUBLEN, stdin);
    fflush(stdin);

    int* results = (int*) calloc(databaseLength +1 , sizeof(int));
    int r = 0;
    system("cls");
    printf("<SEARCH STARTED!>\n"
           "Please wait patiently as this can take a moment\n\n");
    int start;
    int end;
    if(dateFilter)
    {
        start = days_gone(sm, sd);
        if(start == 366) start = 59;
        end = days_gone(em, ed);
        if(end == 366) end = 59;
    }
    if(strlen(sub_search) == 1) subFilter = 0;
    else subFilter = 1;
    if(strlen(body_search) == 1) bodyFilter = 0;
    else bodyFilter = 1;

    for(int i = 0 ; i < databaseLength ; i++)
    {
        int current = days_gone(database[i].m, database[i].d);
        if(current == 366) current = 59;
        if(!dateFilter || (database[i].y > sy || (database[i].y == sy && current >= start))
        && (database[i].y < ey || (database[i].y == ey && current <= end)))
        {
            if(!subFilter || diarySubstringCheck(database[i].subject, sub_search))
            {
                scan_entry(i);
                bool flag = 0;
                if(!bodyFilter || diarySubstringCheck(text, body_search)) flag = 1;
                for(int j = 0 ; j < listLast && !flag ; j++)
                    if(diarySubstringCheck(list[j], body_search))
                        flag = 1;
                if(flag)
                {
                    results[r] = i;
                    r++;
                }
            }
        }
    }
    printf("FOUND: %d matches\n\n", r);
    for(int i = 0 ; i < r ; i++)
    {
        printf("Entry no : %d\n"
               "Date: %2.2d/%2.2d/%4.4d\n"
               "Subject: %s\n\n", results[i]+1, database[results[i]].d, database[results[i]].m, database[results[i]].y, database[results[i]].subject);
    }
    free(results);
}

bool diarySubstringCheck(char* large, char* small)
{
    int slen = strlen(small);
    int llen = strlen(large);
    for(int i = 0 ; i < llen ; i++)
    {
        for(int j = 0 ; j < slen ; j++)
        {
            char s = *(small+j);
            char l = *(large+i+j);
            while(1)
            {
                if(j == slen)
                    return true;
                if(i == llen)
                    return false;
                if(!(s == '<' || l == '<' || s == '\n' || l == '\n'))
                    break;

                // skip newlines
                while(s == '\n' && j < slen)
                {
                    j++;
                    s = *(small+j);
                }
                while(l == '\n' && i < llen)
                {
                    i++;
                    l = *(large+i+j);
                }

                // skip tags
                while(s == '<' && j < slen)
                {
                    if(*(small+j+1) == 'l' && *(small+j+2) == '>') j += 3;
                    if(*(small+j+1) == 'h' && *(small+j+2) == '>') j += 3;
                    if(*(small+j+1) == '/' && *(small+j+2) == 'l' && *(small+j+3) == '>') j += 4;
                    if(*(small+j+1) == '/' && *(small+j+2) == 'h' && *(small+j+3) == '>') j += 4;
                    if(j >= slen) return true;
                    s = *(small+j);
                }
                while(l == '<' && i < llen)
                {
                    if(*(large+j+i+1) == 'l' && *(large+j+i+2) == '>') i += 3;
                    if(*(large+j+i+1) == 'h' && *(large+j+i+2) == '>') i += 3;
                    if(*(large+j+i+1) == '/' && *(large+j+i+2) == 'l' && *(large+j+i+3) == '>') i += 4;
                    if(*(large+j+i+1) == '/' && *(large+j+i+2) == 'h' && *(large+j+i+3) == '>') i += 4;
                    if(i >= llen) return false;
                    l = *(large+i+j);
                }
            }

            if(isalpha(s)) s = tolower(s);
            if(isalpha(l)) l = tolower(l);
            if(l != s)
                break;
            if(j >= slen-1)
                return true;
        }
    }
    return false;
}

void scan_entry(int indx)
{
    char ch;
    textCapacity = 50;
    text = (char*) realloc(text, textCapacity * sizeof(char));
    textLength = 0;
    listLast = 0;
    // opening the file
    mainfile[0] = listfile[0] = 0;
    num_to_string(mainfile, indx);
    num_to_string(listfile, indx);
    strcat(mainfile, ".txt");
    strcat(listfile, "list.txt");
    FILE* fp = fopen(mainfile, "r");

    while(1)
    {
        if(fscanf(fp, "%c", &ch) == EOF) break;
        if(textLength == textCapacity - 1)
        {
            textCapacity *= 2;
            text = (char*) realloc(text, textCapacity * sizeof(char));
        }
        text[textLength] = ch;
        textLength++;
    }
    fclose(fp);
    fp = fopen(listfile, "r");
    memset(listLength, 0, listMaxIndex * sizeof(int));
    while(1)
    {
        if(fgets(list[listLast], maxListLength, fp) == NULL) break;
        listLength[listLast] = strlen(list[listLast]);
        listLast++;
    }
    fclose(fp);
}

void edit_list(int indx)
{
    listfile[0] = 0;
    num_to_string(listfile, indx);
    strcat(listfile, "list.txt");
    FILE* fp = fopen(listfile, "r");
    listLast = 0;
    memset(listLength, 0, listMaxIndex * sizeof(int));

    bool end_of_file = 0;
    while(!end_of_file)
    {
        while(!end_of_file)
        {
            char ch;
            if(fscanf(fp, "%c", &ch) == EOF)
            {
                end_of_file = 1;
                break;
            }
            if(ch == '\n') break;
            list[listLast][listLength[listLast]] = ch;
            listLength[listLast]++;
            list[listLast][listLength[listLast]] = 0;
        }
        listLast++;
    }
    fclose(fp);
    if(listLast == 1)
    {
        printf("Sorry, this entry does not have any list to edit\n"
               "You can only edit list if the entry has atleast one list point\n");
        return;
    }
    char input = 0;
    while(input != escape)
    {
        fp = fopen(listfile, "w");
        system("cls");
        printf("Press esc to exit\n\n");
        printf("CURRENT LIST:-\n");
        print_list_only();
        printf("\n\nWhat would you like to do? Add or remove? (A/R) : ");
        input = _getch();
        printf("%c \n", input);
        if(input == 'A') add_list();
        else if(input == 'R') remove_list(indx);
        serialize_list_only(fp);
        fclose(fp);
    }
}

void add_list()
{
    printf("Give a float value that is inbetween the two list numbers\n"
           "where you want your new list entry to be in\n"
           "(For example to add a list between 1 and 2, give 1.5) : ");
    double input;
    scanf("%lf", &input);
    fflush(stdin);
    input -= 1;
    printf("Give the new list entry: \n");
    fgets(list[listLast], maxListLength, stdin);
    list[listLast][strlen(list[listLast]) -1] = 0; //discarding the \n
    if(input + 1 > listLast)
    {
        strcat(list[listLast], "</l>");
        remove_list_endtag(list[listLast-1]);
    }
    listLength[listLast] = strlen(list[listLast]);
    listLast++;
    for(int i = listLast-1 ; i > input+1 && i > 0 ; i--)
    {
        swap_string(list[i], list[i-1]);
        int temp = listLength[i];
        listLength[i] = listLength[i-1];
        listLength[i-1] = temp;
    }
}

void remove_list(int indx)
{
    printf("Give the list number you want to remove: ");
    int input;
    scanf("%d", &input);
    fflush(stdin);
    input--;
    if(input < 0 || input >= listLast)
    {
        printf("<LIST ELEMENT DOESNT EXIST>\n");
        system("pause");
        return;
    }
    // first - last - middle
    if(input == 0 && check_list_endtag(list[input]))
    {
        if(input != listLast-1 && !check_list_endtag(list[input+1]))
        {
            strcat(list[input+1], "</l>");
            listLength[input+1] += 4;
        }
        else remove_nth_listend_tag_from_body(indx , 1);
    }
    else if(input == listLast-1 && check_list_endtag(list[input]))
    {
        if(!check_list_endtag(list[input-1]))
        {
            strcat(list[input-1], "</l>");
            listLength[input-1] += 4;
        }
        else
        {
            int count = 0;
            for(int i = 0 ; i <= input ; i++)
                if(check_list_endtag(list[i])) count++;
            remove_nth_listend_tag_from_body(indx, count);
        }
    }
    else if(check_list_endtag(list[input]))
    {
        if(!check_list_endtag(list[input+1]))
        {
            strcat(list[input+1], "</l>");
            listLength[input+1] += 4;
        }
        else if(!check_list_endtag(list[input-1]))
        {
            strcat(list[input-1], "</l>");
            listLength[input-1] += 4;
        }
        else
        {
            int count = 0;
            for(int i = 0 ; i <= input ; i++)
                if(check_list_endtag(list[i])) count++;
            remove_nth_listend_tag_from_body(indx, count);
        }
    }
    listLength[input] = 0;
    list[input][0] = 0;
    listLast--;
    for(int i = input ; i < listLast ; i++)
    {
        swap_string(list[i], list[i+1]);
        int temp = listLength[i];
        listLength[i] = listLength[i+1];
        listLength[i+1] = temp;
    }
}

void remove_nth_listend_tag_from_body(int indx, int n)
{
    mainfile[0] = 0;
    num_to_string(mainfile, indx);
    strcat(mainfile, ".txt");
    FILE* fp = fopen(mainfile, "r");

    int count = 0;
    textCapacity = 50;
    text = (char*) realloc(text, textCapacity * sizeof(char));
    text[0] = 0;
    textLength = 0;
    while(1)
    {
        char temp[500] = {};
        int templen;
        if(fgets(temp, 500, fp) == NULL) break;
        templen = strlen(temp);
        if(templen >= 3 && temp[0] == '<' && temp[1] == 'l' && temp[2] == '>') count++;
        if(count == n)
        {
            for(int i = 0 ; i < templen ; i++)
                temp[i] = temp[i+3];
        }

        if(templen + textLength >= textCapacity)
        {
            textCapacity *= 2;
            text = (char*) realloc(text, textCapacity * sizeof(char));
        }
        templen = strlen(temp);
        textLength += templen;
        strcat(text, temp);
    }
    fclose(fp);

    fp = fopen(mainfile, "w");
    fprintf(fp, "%s", text);
    fclose(fp);
    textLength = 0;
}

void print_list_only()// remove tags
{
    for(int i = 0 ; i < listLast ; i++)
    {
        if(check_list_endtag(list[i]))
        {
            if(i+1 < 10)
                printf("%d. %s\b\b\b\b    \b\b\b\b\n", i+1, list[i]);
            else
                printf("%d. %s\b\b\b\b\b     \b\b\b\b\b\n", i+1, list[i]);
        }
        else printf("%d. %s\n", i+1, list[i]);
    }
}

void serialize_list_only(FILE* fp)
{
    for(int i = 0 ; i < listLast-1 ; i++)
        fprintf(fp, "%s\n", list[i]);
    fprintf(fp, "%s", list[listLast-1]);
}

bool check_list_endtag(char* s)
{
    int len = strlen(s);
    if(s[len-4] == '<' && s[len-3] == '/' && s[len-2] == 'l' && s[len-1] == '>')
        return 1;
    return 0;
}

void remove_list_endtag(char* s)
{
    int len = strlen(s);
    s[len-4] = 0;
}

void listEntries()
{
    if(databaseLength == 0)
    {
        printf("THERE ARE NO ENTRIES!\n");
        return;
    }
    printf("The following entries are present:\n\n");
    for(int i = 0 ; i < databaseLength ; i++)
    {
        printf("Entry no %d\n"
               "Date: %2.2d/%2.2d/%4.4d\n"
               "Subject: %s\n", i+1, database[i].d, database[i].m, database[i].y, database[i].subject);
    }
}

void userScan()
{
    char ch;
    textCapacity = 50;
    text = (char*) realloc(text, textCapacity * sizeof(char));
    textLength = 0;
    listLast = 0;
    memset(listLength, 0, listMaxIndex * sizeof(int));
    int tag;
    while((ch = _getch()) != escape)
    {
        if(ch == backspace)
        {
            tag = isTagged();
            if(tag == 1 || tag == 2)
            {
                highlight_mode = !highlight_mode;
                if(highlight_mode)
                    color(highlight_color);
                else color(normal_color);
            }
            removeTag(tag); // incase there is a tag
            printf("\b \b");
            if(textLength != 0)
            {
                textLength--;
                text[textLength] = 0;
            }
        }
        else if(ch == stilde) // pressed highlight
        {
            highlight_mode = !highlight_mode;
            tag = isTagged();
            if(tag == 1) // if someone spammed highlightmode
                removeTag(tag);
            else if(highlight_mode == 1)
            {
                strcat(text, "<h>");
                textLength += 3;
            }
            else
            {
                strcat(text, "</h>");
                textLength += 4;
            }
            if(highlight_mode)
                color(highlight_color);
            else color(normal_color);
        }
        else if(ch == tilde && listLast < listMaxIndex) // pressed list mode
        {
            if(text[textLength-1] == '\n')
            {
                strcat(text, "<l>");
                textLength += 3;
            }
            else
            {
                strcat(text, "\n<l>");
                printf("\n");
                textLength += 4;
            }
            int temp = listLast;
            color(highlight_color);
            printf("%d. ", listLast+1);
            userListScan();
            if(temp == listLast)
                removeTag(isTagged());
        }
        else
        {
            if(ch == enter)
                ch = '\n';
            printf("%c", ch);
            text[textLength] = ch;
            textLength++;
            text[textLength] = 0;
            if(textLength == textCapacity-1)
            {
                textCapacity *= 2;
                text = (char*) realloc(text, textCapacity * sizeof(char));
            }
        }
    }
    color(normal_color);
    highlight_mode = 0;
}

void userListScan()
{
    char ch;
    bool flag = 0;
    while((ch = _getch()) != tilde)
    {
        flag = 1;
        if(ch == backspace)
        {
            if(listLength[listLast] != 0)
            {
                printf("\b \b");
                listLength[listLast]--;
                list[listLast][listLength[listLast]] = 0;
            }
        }
        else if(ch == enter && listLast < listMaxIndex)
        {
            listLast++;
            printf("\n%d. ", listLast+1);
        }
        else if(ch == enter)
        {
            listLast++;
            break;
        }
        else
        {
            printf("%c", ch);
            list[listLast][listLength[listLast]] = ch;
            listLength[listLast]++;
            list[listLast][listLength[listLast]] = 0;
            if(listLength[listLast] == maxListLength) ch = enter;
        }
    }
    for(int i = 0 ; i < maxListLength ; i++)
        printf("\b");
    for(int i = 0 ; i < maxListLength ; i++)
        printf(" ");
    for(int i = 0 ; i < maxListLength ; i++)
        printf("\b");
    if(listLast < listMaxIndex) listLength[listLast] = 0;
    if(flag == 0) return;
    strcat(list[listLast-1], "</l>");
    listLength[listLast-1] += 4;
    if(highlight_mode == 0)
    color(normal_color);
}

int frontTagged(FILE* fp, FILE* ivan)
{
    char tag[5] = "<";
    for(int i = 1 ; i < 3 ; i++)
        fscanf(fp, "%c", &tag[i]);
    if(!strcmp(tag, "<h>"))
    {
        highlight_mode = 1;
        color(highlight_color);
        return 1;
    }
    else if(!strcmp(tag, "<l>"))
    {
        readListFromFile(ivan);
        return 1;
    }
    fscanf(fp, "%c", &tag[3]);
    if(!strcmp(tag, "</h>"))
    {
        highlight_mode = 0;
        color(normal_color);
        return 1;
    }
    fseek(fp, -4, SEEK_CUR);
    return 0;
}

void readFromFile(int indx)
{
    listLast = 0;
    mainfile[0] = listfile[0] = 0;
    num_to_string(mainfile, indx);
    num_to_string(listfile, indx);
    strcat(mainfile, ".txt");
    strcat(listfile, "list.txt");

    FILE* fp = fopen(mainfile, "r");
    FILE* ivan = fopen(listfile, "r");
    char ch;
    textCapacity = 50;
    text = (char*) realloc(text, textCapacity * sizeof(char));
    textLength = 0;
    memset(listLength, 0, listMaxIndex * sizeof(int));
    while(1)
    {
        if(fscanf(fp, "%c", &ch) == EOF) break;
        if(ch == '<')
            frontTagged(fp, ivan);
        else
        {
            if(textLength == textCapacity - 1)
            {
                textCapacity *= 2;
                text = (char*) realloc(text, textCapacity * sizeof(char));
            }
            printf("%c", ch);
            text[textLength] = ch;
            textLength++;
        }
    }
    text[textLength] = 0;
    fclose(fp);
    fclose(ivan);
}

void readListFromFile(FILE* ivan)
{
    color(highlight_color);
    while(1)
    {
        while(1)
        {
            char ch;
            if(fscanf(ivan, "%c", &ch) == EOF) break;
            if(ch == '\n') break;
            list[listLast][listLength[listLast]] = ch;
            listLength[listLast]++;
            list[listLast][listLength[listLast]] = 0;
        }
        int flag = 0;
        if(list[listLast][listLength[listLast] - 1] == '>') flag++;
        if(list[listLast][listLength[listLast] - 2] == 'l') flag++;
        if(list[listLast][listLength[listLast] - 3] == '/') flag++;
        if(list[listLast][listLength[listLast] - 4] == '<') flag++;

        listLast++;

        if(flag == 4)
        {
            if(listLast < 10)
            printf("%d. %s\b\b\b\b    \b\b\b\b\n", listLast, list[listLast-1]);
            else
            printf("%d. %s\b\b\b\b\b     \b\b\b\b\b\n", listLast, list[listLast-1]);
            if(highlight_mode == 0)
                color(normal_color);
            break;
        }
        else
        {
            printf("%d. %s\n", listLast, list[listLast-1]);
        }
    }
}
// swap between modes frequently

// among us sus- text art
// finish these following functions one by one
// use memset to reset the listlength!
// backspace to previous line
// days gone used in the search (fix the leap year)
