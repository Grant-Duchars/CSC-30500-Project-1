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
        printf(">>>");
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
            printf("ERROR: Please enter a command. Valid commands are: (a)dd, (l)ist, (t)ranscript, and (q)uit.\n");
        }
        else if (strcmp(command, "a") == 0) // Add Command
        {
            if (subcommand == NULL) // Check for error: no subcommand entered
            {
                printf("ERROR: Please enter a subcommand for add. Valid subcommands are: (c)course, (g)rade, se(m)ester, (s)tudent, and (t)aken course.\n");
            }
            else
            {
                if (data == NULL) // Check for error: no item entered
                {
                    printf("ERROR: Please enter an item to be added.\n");
                }
                else
                {
                    int errorCode = addItem(subcommand, data);
                    if (errorCode == 0) // Successfully added item to database
                    {
                        printf("Successfully added item to system.\n");
                    }
                    else if (errorCode == 1) // Error: duplicate item
                    {
                        printf("ERROR: Unable to add item. Item already exists in system.\n");
                    }
                    else // Error: invalid subcommand entered
                    {
                        printf("ERROR: Invalid subcommand for add. Valid subcommands are: (c)lass, (g)rade, se(m)ester, (s)tudent, and (t)aken course.\n");
                    }
                }
            }
        }
        else if (strcmp(command, "l") == 0) // List Command
        {
            if (subcommand == NULL) // Check for error: no subcommand entered
            {
                printf("ERROR: Please enter a subcommand for list. Valid subcommands are: (c)lass, (g)rade, se(m)ester, (s)tudent, and (t)aken course.\n");
            }
            else
            {
                if (listItems(subcommand) != 0) // Run listItems and check for error: invalid subcommand entered
                {
                    printf("ERROR: Invalid subcommand for list. Valid subcommands are: (c)lass, (g)rade, se(m)ester, (s)tudent, and (t)aken course.\n");
                }
            }
        }
        else if (strcmp(command, "t") == 0) // Transcript Command
        {
            if (subcommand == NULL) // Check for error: no student name entered
            {
                printf("ERROR: No name entered. Please enter a student's last name followed by their first name.\n");
            }
            else
            {
                if (data == NULL) // Check for error: no student first name entered
                {
                    printf("ERROR: No first name entered. Please enter a student's last name followed by their first name.\n");
                }
                else
                {
                    if (printTranscript(subcommand, data) != 0) // Run printTranscript and check for error: no student in system
                    {
                        printf("ERROR: No student with given name found in system.\n");
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
            printf("ERROR: Invalid command. Valid commands are: (a)dd, (l)ist, (t)ranscript, and (q)uit.\n");
        }
    }
}

// Add Item to Database Function
// Return values: 0 = success, 1 = duplicate item, 2 = invalid subcommand
int addItem(char subcommand[], char data[])
{
    // // Open database file to find duplicate
    // FILE *courses;
    // courses = fopen("database/courses.txt", "r");
    // fclose(courses); // CLOSE THE FILE
    // // Set up string to add to database file
    // char output[100];
    // sprintf(output, "%s %s %s %s\n", prefix, number, title, credits);
    // // Open database file to append the string
    // FILE *courses;
    // courses = fopen("database/courses.txt", "a");
    // fputs(output, courses);
    // fclose(courses); // CLOSE THE FILE
    return 0;
}

int listItems(char type[])
{
    return 0;
}

int printTranscript(char lastName[], char firstName[])
{
    return 0;
}