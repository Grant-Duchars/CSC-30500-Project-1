#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int addItem(char filename[], char data[], int twoPKs);
int addTakenCourse(char data[]);
int checkItem(char filename[], char firstPK[], char secondPK[]);
// int listItems(char type[]);
// int printTranscript(char lastName[], char firstName[]);

struct stat st = {0};

int main(int argc, char *argv[])
{
    while (1) // Main loop until quit command is entered
    {
        // Check if there is a database directory
        if (stat("database", &st) == -1)
            mkdir("database", 0700); // Make one if there is not
        // Prompt User
        printf("\e[33m>>>");
        printf("\e[0m");
        // Read in user input
        char buffer[100];
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        // Parse user input
        char *command, *subcommand, *data;
        command = strtok(buffer, " ");  // Split off command from input
        subcommand = strtok(NULL, " "); // Split off subcommand/last name from input
        data = strtok(NULL, "");        // Split off data from input
        // Execute input command
        if (command == NULL) // Check for error: no command entered
        {
            printf("\e[31mERROR:\e[0m Please enter a command. Valid commands are: (a)dd, (l)ist, (t)ranscript, and (q)uit.\n");
        }
        else if (strcmp(command, "a") == 0) // Add Command
        {
            if (subcommand == NULL) // Check for error: no subcommand entered
            {
                printf("\e[31mERROR:\e[0m Please enter a subcommand for add. Valid subcommands are: (c)ourse, (g)rade, se(m)ester, (s)tudent, and (t)aken course.\n");
            }
            else
            {
                if (data == NULL) // Check for error: no item entered
                {
                    printf("\e[31mERROR:\e[0m Please enter an item to be added.\n");
                }
                else if (strcmp(subcommand, "c") == 0) // Add course
                {
                    addItem("database/courses.txt", data, 1) == 0 ? printf("\e[32mSuccessfully added grade to the system.\n") : printf("\e[31mERROR:\e[0m Unable to add course. Course already exists in the system.\n");
                }
                else if (strcmp(subcommand, "g") == 0) // Add grade
                {
                    addItem("database/grades.txt", data, 0) == 0 ? printf("\e[32mSuccessfully added grade to the system.\n") : printf("\e[31mERROR:\e[0m Unable to add grade. Grade already exists in the system.\n");
                }
                else if (strcmp(subcommand, "m") == 0) // Add semester
                {
                    addItem("database/semesters.txt", data, 0) == 0 ? printf("\e[32mSuccessfully added semester to the system.\n") : printf("\e[31mERROR:\e[0m Unable to add semester. Semester already exists in the system.\n");
                }
                else if (strcmp(subcommand, "s") == 0) // Add student
                {
                    addItem("database/students.txt", data, 1) == 0 ? printf("\e[32mSuccessfully added student to the system.\n") : printf("\e[31mERROR:\e[0m Unable to add student. Student already exists in the system.\n");
                }
                else if (strcmp(subcommand, "t") == 0) // Add taken course
                {
                    int exitCode = addTakenCourse(data);
                    switch (exitCode)
                    {
                    case 1:
                        printf("\e[31mERROR:\e[0m Unable to add taken course. Taken course already exists in the system.\n");
                        break;
                    case 2:
                        printf("\e[31mERROR:\e[0m Unable to add taken course. Given student does not exist in the system.\n");
                        break;
                    case 3:
                        printf("\e[31mERROR:\e[0m Unable to add taken course. Given course does not exist in the system.\n");
                        break;
                    case 4:
                        printf("\e[31mERROR:\e[0m Unable to add taken course. Given grade does not exist in the system.\n");
                        break;
                    case 5:
                        printf("\e[31mERROR:\e[0m Unable to add taken course. Given semester does not exist in the system.\n");
                        break;
                    default:
                        printf("\e[32mSuccessfully added taken course to the system.\n");
                        break;
                    }
                }
                else // Error: invalid subcommand entered
                {
                    printf("\e[31mERROR:\e[0m Invalid subcommand for add. Valid subcommands are: (c)ourse, (g)rade, se(m)ester, (s)tudent, and (t)aken course.\n");
                }
            }
        }
        // else if (strcmp(command, "l") == 0) // List Command
        // {
        //     if (subcommand == NULL) // Check for error: no subcommand entered
        //     {
        //         printf("\e[31mERROR:\e[0m Please enter a subcommand for list. Valid subcommands are: (c)ourse, (g)rade, se(m)ester, (s)tudent, and (t)aken course.\n");
        //     }
        //     else
        //     {
        //         if (listItems(subcommand) != 0) // Run listItems and check for error: invalid subcommand entered
        //         {
        //             printf("\e[31mERROR:\e[0m Invalid subcommand for list. Valid subcommands are: (c)ourse, (g)rade, se(m)ester, (s)tudent, and (t)aken course.\n");
        //         }
        //     }
        // }
        // else if (strcmp(command, "t") == 0) // Transcript Command
        // {
        //     if (subcommand == NULL) // Check for error: no student name entered
        //     {
        //         printf("\e[31mERROR:\e[0m No name entered. Please enter a student's last name followed by their first name.\n");
        //     }
        //     else
        //     {
        //         if (data == NULL) // Check for error: no student first name entered
        //         {
        //             printf("\e[31mERROR:\e[0m No first name entered. Please enter a student's last name followed by their first name.\n");
        //         }
        //         else
        //         {
        //             if (printTranscript(subcommand, data) != 0) // Run printTranscript and check for error: no student in system
        //             {
        //                 printf("\e[31mERROR:\e[0m No student with given name found in system.\n");
        //             }
        //         }
        //     }
        // }
        else if (strcmp(command, "q") == 0) // Quit Command
        {
            return 0;
        }
        else // Error: invalid command entered
        {
            printf("\e[31mERROR:\e[0m Invalid command. Valid commands are: (a)dd, (l)ist, (t)ranscript, and (q)uit.\n");
        }
    }
}

int addItem(char filename[], char data[], int twoPKs)
{
    char *saveData = (char *)malloc(strlen(data) + 1);
    strcpy(saveData, data);
    // Extract primary keys from data
    char *firstPK = strtok(data, " ");
    char *secondPK = NULL;
    if (twoPKs)
        secondPK = strtok(NULL, " ");
    // Check for duplicate
    if (checkItem(filename, firstPK, secondPK))
    {
        free(saveData); // FREE THE VARIABLE
        return 1;       // Duplicate found.
    }
    // Add item to database
    FILE *file;
    file = fopen(filename, "a");
    fputs(saveData, file);
    fputs("\n", file);
    fclose(file);   // CLOSE THE FILE
    free(saveData); // FREE THE VARIABLE
    return 0;       // Successfully added item.
}

int addTakenCourse(char data[])
{
    char *saveData = (char *)malloc(strlen(data) + 1);
    strcpy(saveData, data);
    // Extract primary keys from data
    char *studentLName = strtok(data, " ");
    char *studentFName = strtok(NULL, " ");
    char *coursePrefix = strtok(NULL, " ");
    char *courseNumber = strtok(NULL, " ");
    char *gradeType = strtok(NULL, " ");
    char *semesterCode = strtok(NULL, " ");
    // Check to see if items entered exist in system
    if (checkItem("database/students.txt", studentLName, studentFName) == 0)
    {
        free(saveData); // FREE THE VARIABLE
        return 2;       // No student found.
    }
    else if (checkItem("database/courses.txt", coursePrefix, courseNumber) == 0)
    {
        free(saveData); // FREE THE VARIABLE
        return 3;       // No course found.
    }
    else if (checkItem("database/grades.txt", gradeType, NULL) == 0)
    {
        free(saveData); // FREE THE VARIABLE
        return 4;       // No grade found.
    }
    else if (checkItem("database/semesters.txt", semesterCode, NULL) == 0)
    {
        free(saveData); // FREE THE VARIABLE
        return 5;       // No semester found.
    }
    // Open database file to find duplicate
    FILE *file;
    char *line;
    size_t len = 0;
    file = fopen("database/takencourses.txt", "r");
    if (file == NULL) // Check if the file actually exists
    {
        file = fopen("database/takencourses.txt", "w"); // Create it if it doesn't
        fclose(file);
        file = fopen("database/takencourses.txt", "r");
    }
    while (getline(&line, &len, file) != -1) // Read file line by line
    {
        char editLine[strlen(line)];
        strcpy(editLine, line);
        if (strcmp(line, editLine) == 0) // Check for duplicate
        {
            fclose(file);   // CLOSE THE FILE
            free(saveData); // FREE THE VARIABLE
            return 1;       // Duplicate item found.
        }
    }
    // Didn't find any duplicates
    fclose(file); // CLOSE THE FILE
    // Open database file to append the string
    file = fopen("database/takencourses.txt", "a");
    fputs(saveData, file);
    fputs("\n", file);
    fclose(file);   // CLOSE THE FILE
    free(saveData); // FREE THE VARIABLE
    return 0;       // Successfully added item.
}

int checkItem(char filename[], char firstPK[], char secondPK[])
{
    // Open the file to look for item
    FILE *file;
    char *line;
    size_t len = 0;
    file = fopen(filename, "r");
    if (file == NULL) // Check if the file actually exists
    {
        file = fopen(filename, "w"); // Create it if it doesn't
        fclose(file);
        file = fopen(filename, "r");
    }
    while (getline(&line, &len, file) != -1) // Read file line by line
    {
        int dupeFPK = strcmp(strtok(line, " "), firstPK); // Check if the first primary key matches
        int dupeSPK = 0;
        if (secondPK != NULL)
        {
            dupeSPK = strcmp(strtok(NULL, " "), secondPK); // Check if the second primary key matches
        }
        if (dupeFPK == 0 && dupeSPK == 0) // Check if both primary keys matched
        {
            fclose(file); // CLOSE THE FILE
            return 1;     // Item found.
        }
    }
    fclose(file); // CLOSE THE FILE
    return 0;     // No item found.
}

// int listItems(char type[])
// {
//     char *filename;
//     if (strcmp(type, "c") == 0)
//     {
//         filename = "database/courses.txt";
//     }
//     else if (strcmp(type, "g") == 0)
//     {
//         filename = "database/grades.txt";
//     }
//     else if (strcmp(type, "m") == 0)
//     {
//         filename = "database/semesters.txt";
//     }
//     else if (strcmp(type, "s") == 0)
//     {
//         filename = "database/students.txt";
//     }
//     else if (strcmp(type, "t") == 0) // List taken courses
//     {
//         FILE *takencourses;
//         char *line;
//         size_t len = 0;
//         char *studentLName;
//         char *studentFName;
//         char *coursePrefix;
//         char *courseNumber;
//         char *gradeType;
//         char *semesterCode;
//         char *course;
//         char *courseName;
//         // char *output;
//         takencourses = fopen("database/takencourses.txt", "r");
//         if (takencourses == NULL) // Check if the file actually exists
//         {
//             takencourses = fopen("database/takencourses.txt", "w"); // Create it if it doesn't
//             fclose(takencourses);
//             takencourses = fopen("database/takencourses.txt", "r");
//         }
//         while (getline(&line, &len, takencourses) != -1) // Read file line by line
//         {
//             studentLName = strtok(line, " ");
//             studentFName = strtok(NULL, " ");
//             coursePrefix = strtok(NULL, " ");
//             courseNumber = strtok(NULL, " ");
//             gradeType = strtok(NULL, " ");
//             semesterCode = strtok(NULL, " ");
//             course = getItem("database/courses.txt", coursePrefix, courseNumber);
//             strtok(course, " ");
//             strtok(NULL, " ");
//             courseName = strtok(NULL, " ");
//             // output = stpcpy(stpcpy(stpcpy(stpcpy(stpcpy(stpcpy(stpcpy(output, studentLName), studentFName), semesterCode), coursePrefix), courseNumber), courseName), gradeType);
//             printf("%s %s %s %s %s %s %s\n", studentLName, studentFName, semesterCode, coursePrefix, courseNumber, courseName, gradeType);
//         }
//         fclose(takencourses);
//         return 0;
//     }
//     else // ERROR: Invalid subcommand
//     {
//         return 1;
//     } // List non taken courses
//     FILE *file;
//     char *line;
//     size_t len = 0;
//     file = fopen(filename, "r");
//     if (file == NULL) // Check if the file actually exists
//     {
//         file = fopen(filename, "w"); // Create it if it doesn't
//         fclose(file);
//         file = fopen(filename, "r");
//     }
//     while (getline(&line, &len, file) != -1) // Read file line by line
//     {
//         printf("%s\n", line); // Print out items
//     }
//     fclose(file); // CLOSE THE FILE
//     return 0;
// }

// int printTranscript(char lastName[], char firstName[])
// {
//     return 0;
// }