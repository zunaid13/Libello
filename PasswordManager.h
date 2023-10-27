// preprocessor definitions
#define WEBLEN 50 +2

// global variables
int user_size = 0;
int user_capacity = 4;

// structures
typedef struct _entry{
    char website[WEBLEN];
    char username[USERLEN];
    char password[PASSLEN];
} entry;


// function prototypes
entry* New_entry(entry arr[], char* website, char* username, char* password);
int FindWebsite(entry arr[], char* website);
void RemoveWebsite(entry arr[], int index);
void SerializeUser(entry arr[], FILE *fp);
entry* DeserializeUser(FILE *fp);
void PrintIndexEntry(entry arr[], int index);
void ListAllPassword(entry arr[]);
int PasswordManager(void);
void PasswordGenerator(void);
void StrengthCheck(char* password, char* username);
char* scan_password(char* password);

// main utility function
int PasswordManager(void)
{
    char command;
    char website[WEBLEN], username[USERLEN], password[PASSLEN], hint[HINTLEN], question[QUESLEN], answer[ANSLEN];
    entry* storage;
    char file_name[MAXFILENAMELEN] = "PMfile";
    strcat(file_name, ".bin");
    while(1)
    {
        command = password_manager_menu();
        system("cls");
        if(command == '7')
            break;


        FILE* fp = fopen(file_name, "rb");
        if(fp != NULL)
            storage = DeserializeUser(fp);
        else
        {
            user_capacity = 4;
            user_size = 0;
        }

        if(command == '1')
        {
            while(1)
            {
                printf("Give the website you want to add:\n");
                fgets(website, WEBLEN, stdin);
                int check = FindWebsite(storage, website);
                if(check != -1)
                    printf("Sorry, you already have an account in this website...\n");
                else break;
            }
            printf("Now give us your username:\n");
            fgets(username, USERLEN, stdin);
            printf("Now give us your password:\n");
            scan_password(password);

            storage = New_entry(storage, website, username, password);
            system("cls");
            printf("<SUCCESSFULLY ADDED A NEW ENTRY>\n");
            fp = fopen(file_name, "wb");
            SerializeUser(storage, fp);
            system("pause");
        }

        else if(command == '2')
        {
            printf("Give the website you want to find\n");
            fgets(website, WEBLEN, stdin);
            int check = FindWebsite(storage, website);
            if(check == -1)
                printf("Could not find this website in your entries\n");
            else
            {
                PrintIndexEntry(storage, check);
                printf("Do you want to copy password to clipboard? (Y/N)\n");
                char check2 = getchar();
                fflush(stdin);
                if(check2 == 'Y')
                {
                    copy_text_to_clipboard(storage[check].password);
                    printf("<SUCCESSFULLY COPIED TO CLIPBOARD>\n");
                }
            }
            printf("\n");
            system("pause");
        }

        else if(command == '3')
        {
            printf("Give the website you want to remove\n");
            fgets(website, WEBLEN, stdin);
            int check = FindWebsite(storage, website);
            if(check == -1)
                printf("Could not find this website in your entries\n");
            else
            {
                RemoveWebsite(storage, check);
                printf("<REMOVE SUCCESS>\n");
                fp = fopen(file_name, "wb");
                SerializeUser(storage, fp);
            }
            system("pause");
        }

        else if(command == '4')
        {
            if(user_size != 0)
            {
                printf("All entries are listed below:\n");
                ListAllPassword(storage);
            }
            else printf("This user has no entry\n");
            system("pause");
        }

        else if(command == '5')
        {
            printf("Give us your username:\n");
            fgets(username, USERLEN, stdin);
            printf("Give your password and we will check its strength\n");
            scan_password(password);
            // getting rid of the \n scanned by fgets
            username[strlen(username) - 1] = 0;
            password[strlen(password) - 1] = 0;
            StrengthCheck(password, username);
        }

        else if(command == '6')
        {
            PasswordGenerator();
            system("pause");
        }

    }
    return 0;
}

// function definitions
char* scan_password(char* password)
{
    int len = 0;
    char ch;
    while((ch = getch()) != enter && len < PASSLEN)
    {
        if(ch == backspace)
        {
            printf("\b \b");
            if(len != 0)
            {
                len--;
                password[len] = 0;
            }
        }
        else
        {
            printf("*");
            password[len] = ch;
            len++;
            password[len] = 0;
            if(len == PASSLEN-1)
                break;
        }
    }
    printf("\n");
    password[len] = '\n';
    len++;
    password[len] = 0;
    return password;
}

entry* New_entry(entry arr[], char* website, char* username, char* password)
{
    if(user_size == 0)
    {
        arr = (entry*) malloc(user_capacity * sizeof(entry));
    }
    user_size++;
    if(user_size >= user_capacity)
    {
        user_capacity *= 2;
        arr = (entry*) realloc(arr, user_capacity * sizeof(entry));
        if(arr == NULL)
        {
            fprintf(stderr, "Could not allocate enough memory...Exiting\n");
            exit(EXIT_FAILURE);
        }
    }
    strcpy(arr[user_size-1].password, password);
    strcpy(arr[user_size-1].username, username);
    strcpy(arr[user_size-1].website, website);
    return arr;
}

int FindWebsite(entry arr[], char* website)
{
    int ret = -1;
    for(int i = 0 ; i < user_size ; i++)
    {
        if(!strcmp(arr[i].website, website))
        {
            ret = i;
            break;
        }
    }
    return ret;
}

void RemoveWebsite(entry arr[], int index)
{
    for(int i = index ; i < user_size-1 ; i++)
        arr[i] = arr[i+1];
    user_size--;
    if(user_size == 0)
        free(arr);
    else if(user_size < user_capacity/4)
    {
        user_capacity /= 4;
        arr = (entry*) realloc(arr, user_capacity * sizeof(entry));
        if(arr == NULL)
        {
            fprintf(stderr, "Something went wrong...Exiting\n");
            exit(EXIT_FAILURE);
        }
    }
}

void SerializeUser(entry arr[], FILE *fp)
{
    if(fp == NULL)
    {
        fprintf(stderr, "Could not save/create file...Exiting\n");
        exit(EXIT_FAILURE);
    }
    for(int i = 0 ; i < user_size ; i++)
    {
        char key = KeyGen(arr[i].website);
        fwrite(&key, sizeof(key), 1, fp);
        Encrypt(arr[i].website, key);
        Encrypt(arr[i].username, key);
        Encrypt(arr[i].password, key);
        fwrite(&arr[i], sizeof(arr[0]), 1, fp);
    }
    fclose(fp);
}

entry* DeserializeUser(FILE *fp)
{
    entry* arr;
    user_size = 0;
    while(1)
    {
        char website[WEBLEN];
        char username[USERLEN];
        char password[PASSLEN];
        char key;
        int check = fread(&key, sizeof(key), 1, fp);
        if(check == 0) break;
        fread(website, WEBLEN, 1, fp);
        fread(username, USERLEN, 1, fp);
        fread(password, PASSLEN, 1, fp);
        Encrypt(website, key);
        Encrypt(username, key);
        Encrypt(password, key);
        arr = New_entry(arr, website, username, password);
    }
    fclose(fp);
    return arr;
}

void PrintIndexEntry(entry arr[], int index)
{
    printf("\nWebsite: %s"
           "Username: %s"
           "Password: %s", arr[index].website, arr[index].username, arr[index].password);
}

void ListAllPassword(entry arr[])
{
    for(int i = 0 ; i < user_size ; i++)
        printf("%s", arr[i].website);
    printf("\n");
}

void StrengthCheck(char* password, char* username)
{
    bool capital, small, special, numbers, notlong, substring;
    capital = small = special = numbers = notlong = substring = false;
    int len = strlen(password), weakness = 0;
    if(len <= 5) notlong = true;
    for(int i = 0 ; i < len-1 ; i++)
    {
        if(password[i] >= '0' && password[i] <= '9')
            numbers = true;
        else if(password[i] >= 'a' && password[i] <= 'z')
            small = true;
        else if(password[i] >= 'A' && password[i] <= 'Z')
            capital = true;
        else special = true;
    }
    if(len > strlen(username))
        substring = is_substring(username, password);
    else substring = is_substring(password, username);
    if(substring)
    {
        printf("Your password and username have too much in common\n");
        weakness += 5;
    }
    if(!numbers)
    {
        printf("Your password does not have numbers\n");
        weakness++;
    }
    if(!capital)
    {
        printf("Your password does not have capital letters\n");
        weakness++;
    }
    if(!small)
    {
        printf("Your password does not have small letters\n");
        weakness++;
    }
    if(!special)
    {
        printf("Your password does not have special symbols\n");
        weakness++;
    }
    if(notlong)
    {
        printf("Your password is very short\n");
        weakness++;
    }
    if(numbers && capital && small && special && !notlong && !substring)
        printf("Your password is strong because it has all parameters!!!\n");
    if(weakness >= 3)
        printf("Your password is weak\n");
    else if(weakness >= 1)
        printf("Your password is medium in strength\n");
    system("pause");
    system("cls");
}

void PasswordGenerator(void)
{
    int length, position;
    char input;
    char allowed[5] = {}; // small = 1, cap = 2, num = 3, special = 4
    int allowed_len = 0, must_haves = 0;
    srand(time(0));
    char special_symbols[] = {','  , ';', ':', '<', '>', '/'
                            , '|', '[', ']', '{', '}', '.', '?'
                            , ')', '(', '+', '-', '_', '=', '*', '&'
                            , '^', '%', '$', '#', '@', '!', '`', '~'
                            , '\\', '\"', '\'', ' '};
    printf("Enter the length of your password :\n");
    scanf("%d", &length);
    fflush(stdin);
    char *temp_pass = (char*) calloc(length+1, sizeof(char));
    printf("Y = must have atleast one present in password\n"
           "N = must NOT be in password\n"
           "M = may or may not be in password\n"
           "Remember that inputs are case-sensitive\n\n");


    printf("Do you want your password to have small letters? (Y/N/M) : ");
    scanf("%c", &input);
    fflush(stdin);
    if(input == 'Y')
    {
        must_haves++;
        if(must_haves > length)
        {
            printf("Sorry, password is not possible\n");
            free(temp_pass);
            return;
        }
        position = rand()%length;
        while(temp_pass[position] != 0) position = rand()%length;
        temp_pass[position] = (rand()%26) + 'a';
    }
    if(input != 'N')
    {
        allowed[allowed_len] = '1';
        allowed_len++;
    }

    printf("Do you want your password to have capital letters? (Y/N/M) : ");
    scanf("%c", &input);
    fflush(stdin);
    if(input == 'Y')
    {
        must_haves++;
        if(must_haves > length)
        {
            printf("Sorry, password is not possible\n");
            free(temp_pass);
            return;
        }
        position = rand()%length;
        while(temp_pass[position] != 0) position = rand()%length;
        temp_pass[position] = (rand()%26) + 'A';
    }
    if(input != 'N')
    {
        allowed[allowed_len] = '2';
        allowed_len++;
    }

    printf("Do you want your password to have numbers? (Y/N/M) : ");
    scanf("%c", &input);
    fflush(stdin);
    if(input == 'Y')
    {
        must_haves++;
        if(must_haves > length)
        {
            printf("Sorry, password is not possible\n");
            free(temp_pass);
            return;
        }
        position = rand()%length;
        while(temp_pass[position] != 0) position = rand()%length;
        temp_pass[position] = (rand()%10) + '0';
    }
    if(input != 'N')
    {
        allowed[allowed_len] = '3';
        allowed_len++;
    }

    printf("Do you want your password to have special symbols? (Y/N/M) : ");
    scanf("%c", &input);
    fflush(stdin);
    if(input == 'Y')
    {
        must_haves++;
        if(must_haves > length)
        {
            printf("Sorry, password is not possible\n");
            free(temp_pass);
            return;
        }
        position = rand()%length;
        while(temp_pass[position] != 0) position = rand()%length;
        temp_pass[position] = special_symbols[(rand()%32)];
    }
    if(input != 'N')
    {
        allowed[allowed_len] = '4';
        allowed_len++;
    }

    if(allowed_len == 0)
    {
        printf("Sorry, password is not possible\n");
        free(temp_pass);
        return;
    }

    for(int i = 0 ; i < length ; i++)
    {
        if(temp_pass[i] == 0)
        {
            position = rand()%allowed_len;
            if(allowed[position] == '1')
                temp_pass[i] = (rand()%26) + 'a';
            if(allowed[position] == '2')
                temp_pass[i] = (rand()%26) + 'A';
            if(allowed[position] == '3')
                temp_pass[i] = (rand()%10) + '0';
            if(allowed[position] == '4' && i != length - 1)
                temp_pass[i] = special_symbols[rand()%33];
            if(allowed[position] == '4')
                temp_pass[i] = special_symbols[rand()%32];
        }
    }

    printf("Generated password : \n%s\n", temp_pass);
    printf("Do you want to copy password to clipboard? (Y/N)\n");
    char check = getchar();
    fflush(stdin);
    if(check == 'Y')
    {
        copy_text_to_clipboard(temp_pass);
        printf("<SUCCESSFULLY COPIED TO CLIPBOARD>\n");
    }
    free(temp_pass);
}

char KeyGen(char* sample)
{
    unsigned int ret = 0;
    int length = strlen(sample);
    for(int i = 0 ; i < length ; i++)
    {
        ret += sample[i] + length * i;
    }
    return ret%255+33;
}

char* Encrypt(char* sample, char key)
{
    int length = strlen(sample);
    for(int i = 0 ; i < length-2 ; i++)
        sample[i] = sample[i] ^ key;
    return sample;
}