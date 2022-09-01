#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int addItem(char subcommand[], char data[]);
int listItems(char type[]);
int printTranscript(char lastName[], char firstName[]);

int main(int argc, char *argv[])
{
    while (1) // Main loop until quit command is entered
    {
        // Prompt User
        printf("\e[33m>>>");
        printf("\e[0m");
        // Read in user input
        char buffer[2048];
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
                else
                {
                    int errorCode = addItem(subcommand, data);
                    if (errorCode == 0) // Successfully added item to database
                    {
                        printf("\e[32mSuccessfully added item to system.\n");
                    }
                    else if (errorCode == 1) // Error: duplicate item
                    {
                        printf("\e[31mERROR:\e[0m Unable to add item. Item already exists in system.\n");
                    }
                    else // Error: invalid subcommand entered
                    {
                        printf("\e[31mERROR:\e[0m Invalid subcommand for add. Valid subcommands are: (c)ourse, (g)rade, se(m)ester, (s)tudent, and (t)aken course.\n");
                    }
                }
            }
        }
        else if (strcmp(command, "l") == 0) // List Command
        {
            if (subcommand == NULL) // Check for error: no subcommand entered
            {
                printf("\e[31mERROR:\e[0m Please enter a subcommand for list. Valid subcommands are: (c)ourse, (g)rade, se(m)ester, (s)tudent, and (t)aken course.\n");
            }
            else
            {
                if (listItems(subcommand) != 0) // Run listItems and check for error: invalid subcommand entered
                {
                    printf("\e[31mERROR:\e[0m Invalid subcommand for list. Valid subcommands are: (c)ourse, (g)rade, se(m)ester, (s)tudent, and (t)aken course.\n");
                }
            }
        }
        else if (strcmp(command, "t") == 0) // Transcript Command
        {
            if (subcommand == NULL) // Check for error: no student name entered
            {
                printf("\e[31mERROR:\e[0m No name entered. Please enter a student's last name followed by their first name.\n");
            }
            else
            {
                if (data == NULL) // Check for error: no student first name entered
                {
                    printf("\e[31mERROR:\e[0m No first name entered. Please enter a student's last name followed by their first name.\n");
                }
                else
                {
                    if (printTranscript(subcommand, data) != 0) // Run printTranscript and check for error: no student in system
                    {
                        printf("\e[31mERROR:\e[0m No student with given name found in system.\n");
                    }
                }
            }
        }
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

// Add Item to Database Function
// Return values: 0 = success, 1 = duplicate item, 2 = invalid subcommand
int addItem(char subcommand[], char data[])
{
    // Determine which file to use
    char *fileName;
    if (strcmp(subcommand, "c") == 0)
    {
        fileName = "database/courses.txt";
    }
    else if (strcmp(subcommand, "g") == 0)
    {
        fileName = "database/grades.txt";
    }
    else if (strcmp(subcommand, "m") == 0)
    {
        fileName = "database/semesters.txt";
    }
    else if (strcmp(subcommand, "s") == 0)
    {
        fileName = "database/students.txt";
    }
    else if (strcmp(subcommand, "t") == 0)
    {
        fileName = "database/takencourses.txt";
    }
    else
    {
        return 2; // Invalid subcommand entered
    }
    // Open database file to find duplicate
    FILE *file;
    char *line;
    size_t len = 0;
    file = fopen(fileName, "r");
    if (file == NULL) {
        file = fopen(fileName, "w");
        fclose(file);
        file = fopen(fileName, "r");
    }
    while (getline(&line, &len, file) != -1) // Read file line by line
    {
        if (strncmp(line, data, strlen(data)) == 0) // Check for duplicate item
        {
            fclose(file); // CLOSE THE FILE
            return 1;     // Duplicate item found.
        }
    }
    fclose(file); // CLOSE THE FILE
    // Open database file to append the string
    file = fopen(fileName, "a");
    fputs(data, file);
    fputs("\n", file);
    fclose(file); // CLOSE THE FILE
    return 0;     // Successfully added item
}

int listItems(char type[])
{
    return 0;
}

int printTranscript(char lastName[], char firstName[])
{
    return 0;
}