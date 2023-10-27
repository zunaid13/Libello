#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>
#include <conio.h>
#include <windows.h>
#include <dir.h>
#include <unistd.h> //unneeded
#include <sys/stat.h> //unneeded
#include <errno.h> //unneeded
#include <process.h> // unneeded

// preprocessor defines
#define MAINFILENAME "Manager.bin"
#define README "Read me.txt"
#define USERLEN 50 +2
#define PASSLEN 50 +2
#define QUESLEN 100 +2
#define ANSLEN 100 +2
#define HINTLEN 100 +2
#define MAXFILENAMELEN 15 +2

// global variables
int current_size = 0;
int current_capacity = 4;
int month, date, year, day;
char* read_me_text = "-----------IDEA----------\n"
                     "This is a very generic and simple management application.\n\n"
                     "----------WARNING---------\n"
                     "This software uses a lot of files.\n"
                     "So, be sure to put it in a separate folder.\n\n"
                     "----------How to use---------\n"
                     "1.Follow the on screen instructions\n"
                     "2.To give input use enter/return key of the keyboard\n"
                     "3.Everything is case sensitive here\n"
                     "4.Remember to read what the prompt asks you to do\n"
                     "5.In diary mode you can use tilde(`) to toggle between modes\n"
                     "6.Press Esc to finish entry in diary\n"
                     "7.In diary press LShift+Tilde(~) for highlight mode\n"
                     "8.You can have maximum 50 list entries with highest 100 length strings\n\n";

// structures
typedef struct _data{
    char user[USERLEN];
    char password[PASSLEN];
    char question[QUESLEN];
    char answer[ANSLEN];
    char hint[HINTLEN];
} data;

// function prototypes
data* InsertUser(data arr[], char* user, char* password, char* question, char* answer, char* hint);
int FindUser(data arr[], char* user);
void RemoveUser(data arr[], int index);
void SerializeMainFile(data arr[], FILE* fp);
data* DeserializeMainFile(FILE* fp);
void ListAllUsers(data arr[]);
char KeyGen(char* sample);
void CreateReadMeFile();
char* Encrypt(char* sample, char key);
bool is_substring(const char small[], const char large[]);
char* num_to_string(char* destination, int number);
void copy_text_to_clipboard(char* txt_to_cpy);
void normalize_file(char* file_name);
void hide_file(char* file_name);
void read_only_file(char* file_name);
void generate_time(void);
void swap_string(char* a, char* b);
// taken from todolist
int isLeapYear(int );

// function definitions
char* num_to_string(char* destination, int number)
{
    int len = 0;
    do
    {
        destination[len] = number%10 + '0';
        len++;
        number /= 10;
    } while (number);
    //now to reverse it
    char temp;
    for(int i = 0 ; i < (len+1)/2 ; i++)
    {
        temp = destination[i];
        destination[i] = destination[len-1-i];
        destination[len-1-i] = temp;
    }
    //null character
    destination[len] = 0;
    return destination;
}

bool is_substring(const char small[], const char large[])
{
    int slen = strlen(small);
    int llen = strlen(large);
    for(int i = 0 ; i < llen ; i++)
    {
        for(int j = 0 ; j < slen ; j++)
        {
            char s, l;
            s = *(small+j);
            l = *(large+i+j);
            if(isalpha(s)) s = tolower(s);
            if(isalpha(l)) l = tolower(l);
            if(l != s)
                break;
            if(j == slen-1)
                return true;
        }
    }
    return false;
}

data* InsertUser(data arr[], char* user, char* password, char* question, char* answer, char* hint)
{
    if(current_size == 0)
    {
        arr = (data*) malloc(current_capacity * sizeof(data));
    }
    current_size++;
    if(current_size >= current_capacity)
    {
        current_capacity *= 2;
        arr = (data*) realloc(arr, current_capacity * sizeof(data));
        if(arr == NULL)
        {
            fprintf(stderr, "Could not allocate enough memory...Exiting\n");
            exit(EXIT_FAILURE);
        }
    }
    strcpy(arr[current_size-1].password, password);
    strcpy(arr[current_size-1].user, user);
    strcpy(arr[current_size-1].question, question);
    strcpy(arr[current_size-1].answer, answer);
    strcpy(arr[current_size-1].hint, hint);
    return arr;
}

int FindUser(data arr[], char* user)
{
    int ret = -1;
    for(int i = 0 ; i < current_size ; i++)
    {
        if(!strcmp(arr[i].user, user))
        {
            ret = i;
            break;
        }
    }
    return ret;
}

void RemoveUser(data arr[], int index)
{
    char file_name[MAXFILENAMELEN] = "User", file_name_extension[MAXFILENAMELEN] = {};
    char temp_file_name[MAXFILENAMELEN] = "User", temp_file_name_extension[MAXFILENAMELEN] = {};
    strcat(file_name, num_to_string(file_name_extension, index));
    strcat(temp_file_name, num_to_string(temp_file_name_extension, current_size-1));

    arr[index] = arr[current_size-1];
    rename(temp_file_name, file_name);

    current_size--;
    if(current_size == 0)
        free(arr);
    else if(current_size < current_capacity/4)
    {
        current_capacity /= 4;
        arr = (data*) realloc(arr, current_capacity * sizeof(data));
        if(arr == NULL)
        {
            fprintf(stderr, "Something went wrong...Exiting\n");
            exit(EXIT_FAILURE);
        }
    }
}

void SerializeMainFile(data arr[], FILE* fp)
{
    if(fp == NULL)
    {
        fprintf(stderr, "Could not save/create file...Exiting\n");
        exit(EXIT_FAILURE);
    }
    for(int i = 0 ; i < current_size ; i++)
    {
        char key = KeyGen(arr[i].user);
        fwrite(&key, sizeof(key), 1, fp);
        Encrypt(arr[i].user, key);
        Encrypt(arr[i].password, key);
        Encrypt(arr[i].question, key);
        Encrypt(arr[i].answer, key);
        Encrypt(arr[i].hint, key);
        fwrite(&arr[i], sizeof(arr[0]), 1, fp);
    }
    fclose(fp);
}

data* DeserializeMainFile(FILE* fp)
{
    data* arr;
    current_size = 0;
    current_capacity = 4;
    while(1)
    {
        char user[USERLEN], password[PASSLEN], question[QUESLEN], answer[ANSLEN], hint[HINTLEN];
        char key;
        int check = fread(&key, sizeof(key), 1, fp);
        if(check == 0) break;
        fread(user, sizeof(user), 1, fp);
        fread(password, sizeof(password), 1, fp);
        fread(question, sizeof(question), 1, fp);
        fread(answer, sizeof(answer), 1, fp);
        fread(hint, sizeof(hint), 1, fp);
        Encrypt(user, key);
        Encrypt(password, key);
        Encrypt(question, key);
        Encrypt(answer, key);
        Encrypt(hint, key);
        arr = InsertUser(arr, user, password, question, answer, hint);
    }
    fclose(fp);
    return arr;
}

void ListAllUsers(data arr[])
{
    for(int i = 0 ; i < current_size ; i++)
    {
        printf("Username : %s", arr[i].user);
    }
}

void CreateReadMeFile()
{
    FILE* fp = fopen(README, "r");
    if(fp != NULL) fclose(fp);
    else
    {
        fclose(fp);
        fp = fopen(README, "w");
        fprintf(fp, read_me_text);
        fclose(fp);
    }
}

void copy_text_to_clipboard(char* txt_to_cpy)
{
    OpenClipboard(0);
    EmptyClipboard();
    size_t l = strlen(txt_to_cpy) + 1;
    HGLOBAL h = GlobalAlloc(GMEM_MOVEABLE, l);
    memcpy(GlobalLock(h), txt_to_cpy, l);
    GlobalUnlock(h);
    SetClipboardData(CF_TEXT, h);
    CloseClipboard();
}

void normalize_file(char* file_name)
{
    int attr = GetFileAttributes(file_name);
    SetFileAttributes(file_name, FILE_ATTRIBUTE_NORMAL);
}

void hide_file(char* file_name)
{
    int attr = GetFileAttributes(file_name);
    SetFileAttributes(file_name, (attr | FILE_ATTRIBUTE_HIDDEN));
}

void read_only_file(char* file_name)
{
    int attr = GetFileAttributes(file_name);
    SetFileAttributes(file_name, (attr | FILE_ATTRIBUTE_READONLY));
}

void generate_time(void)
{
    month = day = year = date = -1; //initializing

    time_t current_time;
    time(&current_time);
    char mm[4] = {}, dd[3] = {}, yyyy[5] = {}, ww[4] = {};
    char* temp = ctime(&current_time);
    char month_names[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", 
                            "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    char week[7][4] = {"Sat", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri"};

    //separating from the string
    for(int i = 0 ; i < 3 ; i++)
        ww[i] = temp[i];
    for(int i = 4 ; i < 4+3 ; i++)
        mm[i-4] = temp[i];
    for(int i = 8 ; i < 8+2 ; i++)
        dd[i-8] = temp[i];
    for(int i = 20 ; i < 20+4 ; i++)
        yyyy[i-20] = temp[i];
    for(int i = 0 ; i < 12 ; i++)
    {
        if(!strcmp(mm, month_names[i]))
            month = i+1;
    }
    for(int i = 0 ; i < 7 ; i++)
    {
        if(!strcmp(ww, week[i]))
            day = i+1;
    }
    date = (dd[1]-'0') + (dd[0]-'0')*10;
    year = yyyy[3]-'0' + (yyyy[2]-'0')*10 + (yyyy[1]-'0')*100 + (yyyy[0]-'0')*1000;
}

void swap_string(char* a, char* b)
{
    char* temp;
    int len = strlen(a);
    if(strlen(b) > len)
        len = strlen(b);
    temp = (char*) calloc(len +1, sizeof(char));
    strcpy(temp, a);
    strcpy(a, b);
    strcpy(b, temp);
    free(temp);
}