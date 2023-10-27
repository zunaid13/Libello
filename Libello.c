#include "Main_log.h"
#include "eventlist.h"
#include "Menu.h"
#include "PasswordManager.h"
#include "diary.h"
#include "ToDOList.h"

int main(void)
{
    data* UserName_Password;
    char user[USERLEN], password[PASSLEN], question[QUESLEN], answer[ANSLEN], hint[HINTLEN];
    FILE* fp;
    generate_time();
    start_up();
    CreateReadMeFile();
    // test
    // diary();
    // todo();
    // return 0;
    
    while(1)
    {
        char command;
        command = login_menu();
        if(command == '6')
            break;
        fp = fopen(MAINFILENAME, "rb");
        if(fp != NULL)
            UserName_Password = DeserializeMainFile(fp);
        else
        {
            current_size = 0;
            current_capacity = 4;
        }
        
        if(command == '1')
        {
            while(1)
            {
                printf("Give us the username you would like to use (max length 50):\n");
                fgets(user, USERLEN, stdin);
                int check = FindUser(UserName_Password, user);
                if(check != -1)
                    printf("Sorry this username is already taken\n");
                else break;
            }
            printf("Now give us your password (Max 50):\n");
            scan_password(password);
            printf("Confirm your password:\n");
            char password_confirmation[PASSLEN];
            scan_password(password_confirmation);
            if(strcmp(password, password_confirmation))
            {
                printf("Your confirmation didn't match!\n");
                system("pause");
                continue;
            }
            printf("Give a hint for your password (just in case): ");
            fgets(hint, HINTLEN, stdin);
            printf("In case you forget your password, it's time to set up a backup Q/A\n");
            printf("Question: ");
            fgets(question, QUESLEN, stdin);
            printf("Answer: ");
            fgets(answer, ANSLEN, stdin);
            UserName_Password = InsertUser(UserName_Password, user, password, question, answer, hint);

            system("cls");
            printf("<SUCCESSFULLY CREATED ACCOUNT>\n");
            system("pause");

            normalize_file(MAINFILENAME);
            fp = fopen(MAINFILENAME, "wb");
            SerializeMainFile(UserName_Password, fp);
            read_only_file(MAINFILENAME);
            hide_file(MAINFILENAME);
        }

        else if(command == '2')
        {
            int check;
            printf("Give us your username:\n");
            fgets(user, USERLEN, stdin);
            fflush(stdin);
            check = FindUser(UserName_Password, user);
            if(check == -1)
            {
                printf("No such user found...try again\n");
                system("pause");
                continue;
            }
            printf("Now give us your password (give blank enter to show hint):\n");
            scan_password(password);
            fflush(stdin);
            if(strlen(password) == 1 && password[0] == '\n')
            {
                printf("Hint : %s", UserName_Password[check].hint);
                printf("Now give us your password (give blank enter to show Q/A):\n");
                scan_password(password);
                fflush(stdin);
            }
            if(strlen(password) == 1 && password[0] == '\n')
            {
                printf("Question : %s", UserName_Password[check].question);
                printf("Now give us your answer:\n");
                fgets(answer, PASSLEN, stdin);
                fflush(stdin);
            }
            if((!(strlen(password) == 1 && password[0] == '\n') && !strcmp(UserName_Password[check].password, password)) 
                || !strcmp(UserName_Password[check].answer, answer))
            {
                system("cls");
                printf("<SUCCESSFULLY LOGGED IN>\n");
                system("pause");
                // changing directory to the user folder
                char folder_name[MAXFILENAMELEN] = {}, extension[MAXFILENAMELEN] = {};
                num_to_string(extension, check);
                strcpy(folder_name, "User");
                strcat(folder_name, extension);

                mkdir(folder_name);
                normalize_file(folder_name);
                
                // making the directory read-write
                chdir(folder_name);

                check = main_menu();
                while(check == '3' || check == '2' || check == '1')
                {
                    if(check == '1') diary();
                    if(check == '2') todo();
                    if(check == '3') PasswordManager();
                    check = main_menu();
                }

                // getting out of the directory
                chdir("..");
                read_only_file(folder_name);
                hide_file(folder_name);

                if(check == '6')
                {
                    system("cls");
                    printf("Are you sure you want to delete this account?\n");

                    normalize_file(folder_name);
                    strcpy(extension, "del ");
                    strcat(extension, folder_name);
                    system(extension);
                    fflush(stdin);
                    int d = rmdir(folder_name);
                    if(!d)
                    {
                        RemoveUser(UserName_Password, FindUser(UserName_Password, user));
                        printf("<SUCCESSFULLY DELETED USER>\n");

                        normalize_file(MAINFILENAME);
                        fp = fopen(MAINFILENAME, "wb");
                        SerializeMainFile(UserName_Password, fp);
                        read_only_file(MAINFILENAME);
                        hide_file(MAINFILENAME);
                        
                        system("pause");
                    }
                    else
                    {
                        read_only_file(folder_name);
                        hide_file(folder_name);
                        printf("Verification failed, ABORTING\n");
                        system("pause");
                    }
                }
                if(check == '4')
                {
                    char temp_user[USERLEN] = {};
                    int user_id = FindUser(UserName_Password, user);
                    while(1)
                    {
                        printf("Give us the username you would like to use (max length 50):\n");
                        fgets(temp_user, USERLEN, stdin);
                        fflush(stdin);
                        check = FindUser(UserName_Password, temp_user);
                        if(!strcmp(temp_user, user))
                            break;
                        if(check != -1)
                            printf("Sorry this username is already taken\n");
                        else break;
                    }
                    printf("Enter your password: \n");
                    scan_password(password);
                    fflush(stdin);
                    printf("Confirm your password:\n");
                    char password_confirmation[PASSLEN];
                    scan_password(password_confirmation);
                    if(strcmp(password, password_confirmation))
                    {
                        printf("Your confirmation didn't match!\n");
                        system("pause");
                        continue;
                    }
                    printf("Enter your password hint: \n");
                    fgets(hint, HINTLEN, stdin);
                    fflush(stdin);
                    printf("Enter your backup question: \n");
                    fgets(question, QUESLEN, stdin);
                    fflush(stdin);
                    printf("Enter the answer to your question: \n");
                    fgets(answer, ANSLEN, stdin);

                    strcpy(UserName_Password[user_id].user, temp_user);
                    strcpy(UserName_Password[user_id].password, password);
                    strcpy(UserName_Password[user_id].hint, hint);
                    strcpy(UserName_Password[user_id].question, question);
                    strcpy(UserName_Password[user_id].answer, answer);

                    normalize_file(MAINFILENAME);
                    fp = fopen(MAINFILENAME, "wb");
                    SerializeMainFile(UserName_Password, fp);
                    read_only_file(MAINFILENAME);
                    hide_file(MAINFILENAME);
                    
                    printf("<SUCCESSFULLY CHANGED INFO>\n");
                    system("pause");
                }
            }
            else if(strlen(password) == 1 && password[0] == '\n')
            {
                system("cls");
                printf("Sorry, wrong answer\n");
                system("pause");
            }
            else
            {
                system("cls");
                printf("Sorry, wrong password\n");
                system("pause");
            }
        }

        else if(command == '3')
        {
            if(current_size == 0)
                printf("There are no users currently\n");
            else
            {
                printf("All the current users are:\n");
                ListAllUsers(UserName_Password);
            }
            system("pause");
        }

        else if(command == '4')
            toggle_darkmode();

        else if(command == '5')
            OpenReadMeFile();
    }
    return 0;
}