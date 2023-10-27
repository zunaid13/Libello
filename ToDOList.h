#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

#define NAMELENGTH 200
#define DESCLENGTH 800
#define TODOLIST "ToDo-List.txt"

// Global Variables
int TotalTask = 0;
int taskno = 0;
int capacity = 4;
// int mm, dd, yy, date, num, leapyear;
int num, leapyear;
char *months[] =
    {
        "\n\n ----------January ----------",
        "\n\n ----------February ----------",
        "\n\n ----------March ----------",
        "\n\n ----------April ----------",
        "\n\n ----------May ----------",
        "\n\n ----------June ----------",
        "\n\n ----------July ----------",
        "\n\n ----------August ----------",
        "\n\n ----------September ----------",
        "\n\n ----------October ----------",
        "\n\n ----------November ----------",
        "\n\n ----------December ----------"
    };
// find shortcut (idea)

typedef struct new_entry
{
    char taskname[NAMELENGTH];
    char Description[DESCLENGTH];
    int d;
    int m;
    int y;
    int countdays; // using days gone
    bool mark_as_done;
}   newentry;

newentry *node;

//Function Prototypes
int dayMove(void);
void calendar(void);
int calendarView(void);
newentry* ADD_LIST (void);
newentry* REMOVE_LIST(void);
void SERIALIZE_LIST (void);
void SHOW_ALL_LIST (void);
void SHOW_LIST(int);
void DESERIALIZE_LIST (void);
void ADD_INFO ( char taskname[NAMELENGTH] , char description[DESCLENGTH] , int d , int m , int y , bool mark_as_done);
newentry* EDIT_ENTRY(void);
newentry* SWITCH_TO_EDIT (int);
void Mark_Done (void);
void SORT_LIST (void);
// void Progress_Bar(void);
// void Show_Bar (int);
int daysGone (int, int, int);
int yeargone (int, int, int);
void SEARCH_ENTRY();
int todo(void);


// definitions
int dayMove()
{
    int flag_1, flag_2, flag_3;

    flag_1 = (year - 1) / 4;
    flag_2 = (year - 1) / 100;
    flag_3 = (year - 1) / 400;
    num = (year + flag_1 - flag_2 + flag_3) % 7;
    return num;
}

void calendar(void)
{
    int i = 1, mLoop, flag, Lday;
    for (mLoop = 0; mLoop < 12; mLoop++)
    {
        if (mLoop + 1 == month || mLoop == month)
            printf("%s", months[mLoop]); // printing month name
        if (mLoop + 1 == month || mLoop == month)
            printf("\n\nSun  Mon  Tue  Wed  Thu  Fri  Sat\n");

        for (Lday = 1; Lday <= 1 + num * 5; Lday++) // fixing orientation
        {
            if (mLoop + 1 == month || mLoop == month)
                printf(" ");
        }

        for (Lday = 1; Lday <= dayNum[mLoop]; Lday++) // printing dates
        {
            flag = 0;

            for (int j = 0; j < taskno; j++)
            {
                if (year == node[j].y && i == daysGone(node[j].d, node[j].m, node[j].y))
                {
                    if(node[j].mark_as_done == true) // fix this
                    {
                        color(theme);
                        if(j != taskno-1) continue;
                    }
                    else color(selected);
                    if (mLoop + 1 == month || mLoop == month)
                        printf("%2d", Lday);
                    i++;
                    flag = 1;
                    color(not_selected);
                    break;
                }
            }

            if (!flag)
            {
                if (mLoop + 1 == month || mLoop == month)
                    printf("%2d", Lday);
                i++;
            }
            if (mLoop + 1 == month || mLoop == month)
            {
                if ((Lday + num) % 7 > 0)
                    printf("   ");
                else
                    printf("\n ");
            }
        }
        num = (num + dayNum[mLoop]) % 7;
    }
}

int calendarView(void)
{
    color(not_selected);
    num = dayMove();
    printf("Today : %2.2d/%2.2d/%4.4d", date, month, year);
    isLeapYear(year);
    calendar();
    printf("\n");
    system("pause");
    return 0;
}

void SERIALIZE_LIST (void)
{
    FILE *fp = fopen ( TODOLIST , "wb" );
    for ( int I = 0 ; I < taskno ; I++ )
        fwrite ( &node[I] , sizeof(node[0]) , 1 , fp );

    fclose(fp);
}


void DESERIALIZE_LIST (void)
{
    taskno = 0;
    capacity = 4;
    FILE *fp = NULL;
    fp = fopen (TODOLIST , "rb");
    if (fp == NULL)
        return;
    while(1)
    {
        newentry Temp;
        int check = fread(&Temp , sizeof(Temp) , 1 , fp);
        if (check == 0)
            break;
    ADD_INFO (Temp.taskname , Temp.Description , Temp.d , Temp.m , Temp.y, Temp.mark_as_done);
    }
    fclose(fp);
}

void ADD_INFO ( char taskname[NAMELENGTH] , char description[DESCLENGTH] , int d , int m , int y , bool mark_as_done)
{
    strcpy(node[taskno].taskname , taskname);
    strcpy(node[taskno].Description , description);
    node[taskno].d = d;
    node[taskno].m = m;
    node[taskno].y = y;
    node[taskno].countdays = yeargone(d,m,y);
    node[taskno].mark_as_done = mark_as_done;

    taskno++;
    TotalTask = taskno;
    if(taskno == capacity)
    {
        capacity *= 2;
        node = (newentry*) realloc(node, capacity * sizeof(newentry));
    }
    // return;
}
// add korle length barbe capacity barbe length kombe remove entry er shomoy

newentry* ADD_LIST (void)
{
    char taskname[NAMELENGTH] , description[DESCLENGTH];
    int d , m , y;
    system("cls");
    printf("Task Name : ");
    fgets(taskname,NAMELENGTH,stdin);
    printf("Description : ");
    fgets(description,DESCLENGTH,stdin);
    printf("Deadline (dd/mm/yyyy):  ");
    scanf("%d / %d / %d", &d , &m , &y);
getchar();
    if(d < 1 || m < 1 || ((m != 2 && d > dayNum[m-1]) || (m == 2 && isLeapYear(y) && d > 29) || (m == 2 && d > 28)) || m > 12)
    {
        printf("Invalid time!\n");
        system("pause");
        return NULL;
    }

    // newentry node;
    if ( taskno == 0 )
    {
        node = (newentry*) realloc (node , capacity * sizeof(newentry));
    }
    taskno++;
    TotalTask = taskno;
    if(taskno >= capacity)
    {
        capacity *= 2;
        node = (newentry*) realloc ( node , capacity * sizeof(newentry));
        if (node == NULL)
        {
            fprintf(stderr, "Could not allocate enough memory ... Exiting\n");
            exit(EXIT_FAILURE);
        }
    }
    strcpy( node[taskno-1].taskname , taskname );
    strcpy( node[taskno-1].Description , description );
    node[taskno-1].d = d;
    node[taskno-1].m = m;
    node[taskno-1].y = y;
    node[taskno-1].countdays = yeargone(d,m,y);
    node[taskno-1].mark_as_done = false;

    SORT_LIST();
    SERIALIZE_LIST();

    return node;
}

newentry* EDIT_ENTRY(void)
{
    int indx_to_edit;
        printf("Enter the index No you want to edit\n");
        scanf("%d",&indx_to_edit);
        getchar();
        if(indx_to_edit < 1 || indx_to_edit > TotalTask)
        {
            printf("Task number does not exist\n");
            system("pause");
            return NULL;
        }
        puts("\n");

    node = SWITCH_TO_EDIT(indx_to_edit);
    SORT_LIST();
    SERIALIZE_LIST();
        return node;

}

newentry* SWITCH_TO_EDIT(int indx_to_edit)
{
    printf("What you want to Edit?\n");
        printf("\t1. Task Name\n");
        printf("\t2. Description\n");
        printf("\t3. Time\n");
        printf("\t4. All\n");
    int switch_case;
    scanf("%d",&switch_case);
    getchar();
        switch(switch_case)
        {
            case 1:
            {
                char taskname[NAMELENGTH];
                printf("Task Name : ");
                fgets(taskname, NAMELENGTH , stdin);
                strcpy(node[indx_to_edit-1].taskname , taskname);
            }
                break;
            case 2:
            {
                char DESCRIPTION[DESCLENGTH];
                printf("Description : ");
                fgets(DESCRIPTION, DESCLENGTH , stdin);
                strcpy(node[indx_to_edit-1].Description , DESCRIPTION);
            }
                break;
            case 3:
            {
                int d, m , y;
                scanf("%d / %d / %d",&d ,&m ,&y);
                getchar();
                if(d < 1 || m < 1 || ((m != 2 && d > dayNum[m-1]) || (m == 2 && isLeapYear(y) && d > 29) || (m == 2 && d > 28)) || m > 12)
                {
                    printf("Invalid time!\n");
                    system("pause");
                    return NULL;
                }
                node[indx_to_edit-1].d = d;
                node[indx_to_edit-1].m = m;
                node[indx_to_edit-1].y = y;
                node[indx_to_edit-1].countdays = yeargone(d,m,y);
            }
                break;
            case 4:
            {
                char taskname[NAMELENGTH] , description[DESCLENGTH];
    int d , m , y;
    system("cls");
    printf("Task Name : ");
    fgets(taskname,NAMELENGTH,stdin);
    printf("\nDescription : ");
    fgets(description,DESCLENGTH,stdin);
    printf("Deadline (dd/mm/yyyy):  ");
    scanf("%d / %d / %d", &d , &m , &y);
getchar();
    if(d < 1 || m < 1 || ((m != 2 && d > dayNum[m-1]) || (m == 2 && isLeapYear(y) && d > 29) || (m == 2 && d > 28)) || m > 12)
    {
        printf("Invalid time!\n");
        system("pause");
        return NULL;
    }
        strcpy( node[indx_to_edit-1].taskname , taskname );
    strcpy( node[indx_to_edit-1].Description , description );
    node[indx_to_edit-1].d = d;
    node[indx_to_edit-1].m = m;
    node[indx_to_edit-1].y = y;
    node[indx_to_edit-1].countdays = yeargone(d,m,y);
            }
            break;
        default:
        {
        printf("\n\tInvalid Input!\t\n");
        }
        break;
        }
    return node;
}

newentry* REMOVE_LIST(void)
{
    // system("cls");
    int indx_to_remove;
        printf("Enter the index No you want to remove\n");
        scanf("%d",&indx_to_remove);
        getchar();
    if(indx_to_remove>taskno || indx_to_remove<1)
    {
    printf("Unable to find Task at index no %d\n\n",indx_to_remove);
    printf("Press Any Key to Continue\n");
    getchar();
        return node;
    }
    node [indx_to_remove-1] = node[taskno-1];
    taskno--;
    TotalTask = taskno;

    SORT_LIST();
    SERIALIZE_LIST();
    printf("%d no task successfully removed from your To Do List%c\n\n",indx_to_remove , 3);
    printf("Press Any Key to Continue\n");
    getchar();
    return node;
}

void SHOW_ALL_LIST (void)
{
    char ch;
    int no;
    // SORT_LIST();
    system("cls");
    for (int I=0 ; I<taskno ; I++)
    {
        // if(node[I].mark_as_done)
        printf("%d.\t",I+1);
        printf("Task Name : %s",node[I].taskname);
        printf("\tTime : %2.2d/%2.2d/%4.4d\n",node[I].d, node[I].m, node[I].y);
    }
    printf("\n");
    if(TotalTask>0)
    {
        printf("Do you want to view any task? (y/n)\n");
        scanf("%c",&ch);
        getchar();
    }
    else
    {
        printf("No entries present currently\n");
        printf("Press Any Key to Continue\n");
        getchar();
    }
    if(ch=='y')
    {
        printf("Enter the no of Task : ");
        scanf("%d",&no);
        getchar();
        SHOW_LIST(no);
    printf("Press Any Key to Continue\n");
    getchar();
    }
}

void SHOW_LIST(int No_of_Task)
{
    printf("\n");
    printf("\tTask Name : %s",node[No_of_Task-1].taskname);
    printf("\tDescription : %s",node[No_of_Task-1].Description);
    printf("\tTime : %2.2d/%2.2d/%4.4d\n",node[No_of_Task-1].d, node[No_of_Task-1].m, node[No_of_Task-1].y);
}

void Mark_Done(void)
{
    int index;
    printf("\tEnter the Index No of Task you have completed : ");
    scanf("%d",&index);
    getchar();
    if((index-1) > TotalTask || index < 1)
    {
        printf("Entry Not Found!\n");
        return;
    }
    if(node[index-1].mark_as_done)
    {
        printf("This task was already done\n");
        return;
    }
    node[index-1].mark_as_done = true;

    char tempname[NAMELENGTH];
    strcpy(tempname, node[index-1].taskname);
    tempname[strlen(tempname)-1] = 0;
    strcat(tempname, " (Done)\n");
    strcpy(node[index-1].taskname, tempname);

    printf("\n\t\tTask No %d Marked As Done\n",index);
    printf("\nPress Any Key to Continue\n");
    getchar();
    SERIALIZE_LIST();
}

void Progress_Bar(void)
{
    int completed_task = 0;

    // system("color 0A");

    for (int I=0 ; I<TotalTask ; I++)
    {
        if(node[I].mark_as_done)
            completed_task++;
    }
    Show_Bar(completed_task);

}

void Show_Bar (int completed_task)
{
    // Initialize Character for Printing
    char a = 177 , b = 219;
    double pgsn = (((double)completed_task / (double)TotalTask) * 50);
    int progression = pgsn;
    gotoxy(39, 19);
    printf("Progression...(%d/%d)",completed_task ,TotalTask);
    gotoxy(24, 20);

    // Print Initial Loading Bar
        for (int I=0 ; I<50 ; I++)
        {
            if(I<progression)
                printf("%c", b);
            else
                printf("%c", a);
        }

    // Set The Curson Again at starting point of progression bar
    // printf("\r");
    // printf("\t\t\t\t\t"
    // printf("\n");
    // system("pause");

}

void SORT_LIST (void)
{
    for ( int I=0 ; I<taskno-1 ; I++ )
    {
    int FLAG = 0;
    for ( int J=0 ; J<taskno-1-I ; J++ )
    {
    if((node[J].countdays > node[J+1].countdays))
    {
    newentry temp = node[J];
    node[J] = node[J+1];
    node[J+1] = temp;
    FLAG = 1;
    }
    }
    if (FLAG == 0)
    break;
    }
    SERIALIZE_LIST();

}

int isLeapYear(int y)
{
    if (y % 4 == 0 && y % 100 != 0 || y % 400 == 0)
    {
        dayNum[1] = 29;
        return 1;
    }
    else
    {
        dayNum[1] = 28;
        return 0;
    }
}

int daysGone(int d, int m, int y)
{
    int jan = 31, feb = 28, mar = 31, apr = 30, may = 31, jun = 30, jul = 31, aug = 31, sep = 30, oct = 31, nov = 30;
    int daysGone = 0;
    if (isLeapYear(y))
    {
        feb = 29;
    }

    switch (m)
    {
    case 12:
        daysGone = nov;

    case 11:
        daysGone += oct;

    case 10:
        daysGone += sep;

    case 9:
        daysGone += aug;

    case 8:
        daysGone += jul;

    case 7:
        daysGone += jun;

    case 6:
        daysGone += may;

    case 5:
        daysGone += apr;

    case 4:
        daysGone += mar;

    case 3:
        daysGone += feb;

    case 2:
        daysGone += jan;

    case 1:
        daysGone += d;
    }

    return daysGone;
}

int yeargone (int d, int m, int y) // nott for real
{
    return (daysGone(d,m,y) + ((y-2000)*365));
}

void SEARCH_ENTRY()
{
    int d, m, y;
    printf("Give the date (dd/mm/yyyy) : ");
    scanf("%d / %d / %d", &d, &m, &y);
    getchar();
    if(d < 1 || m < 1 || ((m != 2 && d > dayNum[m-1]) || (m == 2 && isLeapYear(y) && d > 29) || (m == 2 && d > 28)) || m > 12)
    {
        printf("Invalid time!\n");
        system("pause");
        return;
    }
    int y_val = yeargone(d, m, y);
    bool flag = 0;
    for(int I = 0 ; I < taskno ; I++)
    {
        if(node[I].countdays == y_val)
        {
            if(flag == 0) printf("ENTRY FOUND\n");
            flag = 1;
            printf("Task name: %s"
                   "Description: %s", node[I].taskname, node[I].Description);
        }
    }
    if(flag == 0) printf("NOT FOUND\n");
    system("pause");
}


int todo(void)
{
    node = (newentry*) malloc (capacity * sizeof(newentry));
    int NUMBER;
    // int index = 0;


    while(true)
    {
        system("cls");
    //     printf("Choose One:\n");
    //         printf("\t1. Add Entry\n");
    //         printf("\t2. Edit Entry\n");
    //         printf("\t3. Search entry\n");
    //         printf("\t4. Remove Entry\n");
    //         printf("\t5. Show All List\n");
    //         printf("\t6. Mark As Done\n");
    //         printf("\t7. Exit To-Do List\n");
    // printf("\n");

    // scanf("%d",&NUMBER);
    // getchar();
    DESERIALIZE_LIST();
    NUMBER = todo_menu();

    // Progress_Bar();
    switch (NUMBER)
    {
    case 1:
    {
        ADD_LIST();
        // index++;
    }
        break;
    case 2:
    {
        EDIT_ENTRY();
    }
        break;
    case 3:
    {
        SEARCH_ENTRY();
    }
        break;
    case 4:
    {
        REMOVE_LIST();
    }
        break;
    case 5:
    {
        SHOW_ALL_LIST();
    }
        break;
    case 6:
    {
        Mark_Done();
    }
        break;
    case 7:
    {
        calendarView();
    }
        break;
    case 8:
    {
        return 0;
    }
    default:
    {
        printf("\n\tInvalid Input!\t\n");
    }
        break;
    }
    }

    free(node);
    return 0;
}
