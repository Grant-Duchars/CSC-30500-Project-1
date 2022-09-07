#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int addItem(char filename[], char data[], int twoPKs);
void sortSemesters();
int addTakenCourse(char data[]);
int checkItem(char filename[], char firstPK[], char secondPK[]);
int listItems(char type[]);
int printTranscript(char lastName[], char firstName[]);

const char DELIM[2] = " ";

struct stat st = {0};

struct semesterNode
{
    char semesterLine[17];
    int year;
    int season;
    struct semesterNode *next;
};

struct takenCourseNode
{
    char coursePrefix[4];
    char courseNumber[6];
    char gradeType[3];
    char semesterCode[5];
    struct takenCourseNode *next;
};

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
        char *command = strtok(buffer, DELIM);  // Split off command from input
        char *subcommand = strtok(NULL, DELIM); // Split off subcommand/last name from input
        char *data = strtok(NULL, "");          // Split off data from input
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
                    if (addItem("database/semesters.txt", data, 0) == 0)
                    {
                        sortSemesters();
                        printf("\e[32mSuccessfully added semester to the system.\n");
                    }
                    else
                    {
                        printf("\e[31mERROR:\e[0m Unable to add semester. Semester already exists in the system.\n");
                    }
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

/**
 * Adds provided item to file. First checks if there is already an item with
 * the same primary keys then adds it if there is no duplicate.
 *
 * @param filename String of filename to add item to.
 * @param data String that will be added to file.
 * @param twoPKs Int specifying if data contains two primary keys.
 * @return 0 if added item successfully and 1 if there was a duplicate found.
 */
int addItem(char filename[], char data[], int twoPKs)
{
    char *saveData = (char *)malloc(strlen(data) + 1);
    strcpy(saveData, data);
    // Extract primary keys from data
    char *firstPK = strtok(data, DELIM);
    char *secondPK = NULL;
    if (twoPKs)
        secondPK = strtok(NULL, DELIM);
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
    fputs(" \n", file);
    fclose(file);   // CLOSE THE FILE
    free(saveData); // FREE THE VARIABLE
    return 0;       // Successfully added item.
}

/**
 * Function to sort the semesters.txt file by ascending date.
 */
void sortSemesters()
{
    FILE *file;
    char *line;
    size_t len = 0;
    struct semesterNode *head = NULL;
    struct semesterNode *current;
    struct semesterNode *next;
    file = fopen("database/semesters.txt", "r"); // Open the semesters file
    if (file == NULL)                            // Check if the file actually exists
    {
        file = fopen("database/semesters.txt", "w"); // Create it if it doesn't
        fclose(file);
        file = fopen("database/semesters.txt", "r");
    }
    int count = 0;
    while (getline(&line, &len, file) != -1) // Read file line by line
    {
        // Count how many lines are in the file
        count++;
        // Create a linked list node for each semester
        struct semesterNode *link = (struct semesterNode *)malloc(sizeof(struct semesterNode));
        strcpy(link->semesterLine, line);
        strtok(line, DELIM);
        char *year = strtok(NULL, DELIM);   // Extract the year from line
        char *season = strtok(NULL, DELIM); // Extract the season from line
        link->year = atoi(year);
        link->next = head;
        if (strcmp(season, "Spring\n") == 0)
        {
            link->season = 1;
        }
        else if (strcmp(season, "Summer\n") == 0)
        {
            link->season = 2;
        }
        else // Fall
        {
            link->season = 3;
        }
        head = link;
    }
    fclose(file); // CLOSE THE FILE
    int i, j, k = count, tempYear, tempSeason;
    char tempSemesterLine[17];
    // Start sorting the linked list by year and season
    for (i = 0; i < count - 1; i++, k--)
    {
        current = head;
        next = head->next;
        for (j = 1; j < k; j++)
        {
            // Swap nodes if the current node comes later than the next one
            if ((current->year > next->year) || ((current->year == next->year) && (current->season > next->season)))
            {
                strcpy(tempSemesterLine, current->semesterLine);
                strcpy(current->semesterLine, next->semesterLine);
                strcpy(next->semesterLine, tempSemesterLine);
                tempYear = next->year;
                current->year = next->year;
                next->year = tempYear;
                tempSeason = next->season;
                current->season = next->season;
                next->season = tempSeason;
            }
            current = current->next;
            next = next->next;
        }
    }
    file = fopen("database/semesters.tmp", "w");
    // Run through the sorted linked list and write the lines to a temporary file
    while (head != NULL)
    {
        fputs(head->semesterLine, file);
        current = head;
        head = head->next;
        free(current); // FREE THE NODE
    }
    fclose(file);
    remove("database/semesters.txt");                           // Remove the old semesters file
    rename("database/semesters.tmp", "database/semesters.txt"); // Rename the temporary semesters file
    // Done.
}

/**
 * Adds provided taken course to takencourses.txt. First checks to see if items
 * within provided taken course exist elsewhere in the system. Next checks to
 * see if there is a duplicate taken course in takencourses.txt. Finally adds
 * the taken course to takencourses.txt if the items exist there is no duplicate.
 *
 * @param data String to be added to takencourses.txt.
 * @return 0 if successfully added, 1-5 if there is a dupe or an item does not exist.
 */
int addTakenCourse(char data[])
{
    char *saveData = (char *)malloc(strlen(data) + 1);
    strcpy(saveData, data);
    // Extract primary keys from data
    char *studentLName = strtok(data, DELIM);
    char *studentFName = strtok(NULL, DELIM);
    char *coursePrefix = strtok(NULL, DELIM);
    char *courseNumber = strtok(NULL, DELIM);
    char *gradeType = strtok(NULL, DELIM);
    char *semesterCode = strtok(NULL, DELIM);
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
        if (strcmp(saveData, editLine) == 0) // Check for duplicate
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
    fputs(" \n", file);
    fclose(file);   // CLOSE THE FILE
    free(saveData); // FREE THE VARIABLE
    return 0;       // Successfully added item.
}

/**
 * Checks given file for an item that matches the one/two given primary keys
 *
 * @param filename String specifying filename of file to check.
 * @param firstPK String of first primary key to check for.
 * @param secondPK String of second primary key to check for. Can be null if there is no second primary key.
 * @return 0 if no item found or 1 if item was found.
 */
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
        int dupeFPK = strcmp(strtok(line, DELIM), firstPK); // Check if the first primary key matches
        int dupeSPK = 0;
        if (secondPK != NULL)
        {
            dupeSPK = strcmp(strtok(NULL, DELIM), secondPK); // Check if the second primary key matches
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

/**
 * Takes a type of item and prints all items found in corresponding file.
 *
 * @param type String of desired item type
 * @return 0 if printed items or 1 is specified type is invalid
 */
int listItems(char type[])
{
    char *filename;
    if (strcmp(type, "c") == 0)
    {
        filename = "database/courses.txt";
    }
    else if (strcmp(type, "g") == 0)
    {
        filename = "database/grades.txt";
    }
    else if (strcmp(type, "m") == 0)
    {
        filename = "database/semesters.txt";
    }
    else if (strcmp(type, "s") == 0)
    {
        filename = "database/students.txt";
    }
    else if (strcmp(type, "t") == 0) // List taken courses
    {
        FILE *takencourses;
        FILE *courses;
        char *curTakenCourse = (char *)malloc(50);
        char *curCourse = (char *)malloc(50);
        size_t len = 0;
        // char *output;
        takencourses = fopen("database/takencourses.txt", "r");
        if (takencourses == NULL) // Check if the file actually exists
        {
            takencourses = fopen("database/takencourses.txt", "w"); // Create it if it doesn't
            fclose(takencourses);
            takencourses = fopen("database/takencourses.txt", "r");
        }
        while (getline(&curTakenCourse, &len, takencourses) != -1) // Read file line by line
        {
            char *studentLName = strtok(curTakenCourse, DELIM);
            char *studentFName = strtok(NULL, DELIM);
            char *coursePrefix = strtok(NULL, DELIM);
            char *courseNumber = strtok(NULL, DELIM);
            char *gradeType = strtok(NULL, DELIM);
            char *semesterCode = strtok(NULL, DELIM);
            // Open the file to look for item
            courses = fopen("database/courses.txt", "r");
            if (courses == NULL) // Check if the file actually exists
            {
                courses = fopen("database/courses.txt", "w"); // Create it if it doesn't
                fclose(courses);
                courses = fopen("database/courses.txt", "r");
            }
            while (getline(&curCourse, &len, courses) != -1) // Read file line by line
            {
                int dupeFPK = strcmp(strtok(curCourse, DELIM), coursePrefix); // Check if the first primary key matches
                int dupeSPK = strcmp(strtok(NULL, DELIM), courseNumber);      // Check if the second primary key matches
                if (dupeFPK == 0 && dupeSPK == 0)                             // Check if both primary keys matched
                {
                    break; // If so we found the correct course
                }
            }
            fclose(courses); // CLOSE THE FILE
            char *courseName = strtok(NULL, DELIM);
            printf("%s %s %s %s %s %s %s\n", studentLName, studentFName, semesterCode, coursePrefix, courseNumber, courseName, gradeType);
        }
        fclose(takencourses); // CLOSE THE FILE
        free(curTakenCourse); // FREE THE VARIABLE
        free(curCourse);      // FREE THE VARIABLE
        return 0;             // Printed all taken courses.
    }
    else // ERROR: Invalid subcommand
    {
        return 1;
    } // List non taken courses
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
        printf("%s", line); // Print out items
    }
    fclose(file); // CLOSE THE FILE
    return 0;
}

int printTranscript(char lastName[], char firstName[])
{
    FILE *file;
    char *line;
    size_t len = 0;
    struct takenCourseNode *head = NULL;
    int numCourses = 0;
    file = fopen("database/takencourses.txt", "r"); // Open the taken courses file
    if (file == NULL)                               // Check if the file actually exists
    {
        file = fopen("database/takencourses.txt", "w"); // Create it if it doesn't
        fclose(file);
        file = fopen("database/takencourses.txt", "r");
    }
    while (getline(&line, &len, file) != -1) // Read line by line
    {
        int dupeFPK = strcmp(strtok(line, DELIM), lastName);  // Check if the first primary key matches
        int dupeSPK = strcmp(strtok(NULL, DELIM), firstName); // Check if the second primary key matches
        if (dupeFPK == 0 && dupeSPK == 0)                     // Check if both primary keys matched
        {
            numCourses++;
            // Create a linked list node for each taken course by the given student
            struct takenCourseNode *link = (struct takenCourseNode *)malloc(sizeof(struct takenCourseNode));
            strcpy(link->coursePrefix, strtok(NULL, DELIM));
            strcpy(link->courseNumber, strtok(NULL, DELIM));
            strcpy(link->gradeType, strtok(NULL, DELIM));
            strcpy(link->semesterCode, strtok(NULL, DELIM));
            link->next = head;
            head = link;
        }
    }
    if (head == NULL)
    {
        return 1;
    }
    fclose(file);
    struct takenCourseNode *current;
    struct takenCourseNode *previous;
    FILE *courses;
    char *curCourse;
    FILE *grades;
    char *curGrade;
    int usedCurrentSemester;
    int totalCreditHours = 0;
    float totalGradePoints = 0;
    file = fopen("database/semesters.txt", "r"); // Open the semesters file
    if (file == NULL)                            // Check if the file actually exists
    {
        file = fopen("database/semesters.txt", "w"); // Create it if it doesn't
        fclose(file);
        file = fopen("database/semesters.txt", "r");
    }
    while (getline(&line, &len, file) != -1) // Read line by line
    {
        usedCurrentSemester = 0;
        current = head;
        previous = head;
        char *semesterCode = strtok(line, DELIM);
        while (current != NULL)
        {
            if (strcmp(current->semesterCode, semesterCode) == 0)
            {
                if (!usedCurrentSemester)
                {
                    char *year = strtok(NULL, DELIM);
                    char *season = strtok(NULL, DELIM);
                    usedCurrentSemester = 1;
                    printf("============ Semester: %s %s ============\n", season, year);
                }
                char *courseTitle;
                char *courseHours;
                courses = fopen("database/courses.txt", "r"); // Open the courses file
                if (file == NULL)                             // Check if the file actually exists
                {
                    file = fopen("database/semesters.txt", "w"); // Create it if it doesn't
                    fclose(file);
                    file = fopen("database/semesters.txt", "r");
                }
                while (getline(&curCourse, &len, courses) != -1) // Read line by line
                {
                    int dupeFPK = strcmp(strtok(curCourse, DELIM), current->coursePrefix); // Check if the first primary key matches
                    int dupeSPK = strcmp(strtok(NULL, DELIM), current->courseNumber);      // Check if the second primary key matches
                    if (dupeFPK == 0 && dupeSPK == 0)                                      // Check if both primary keys matched
                    {
                        courseTitle = strtok(NULL, DELIM);
                        courseHours = strtok(NULL, DELIM);
                        break; // Found the course we were looking for and saved the information
                    }
                }
                fclose(courses);
                char *gradePoints;
                grades = fopen("database/grades.txt", "r"); // Open the grades file
                if (file == NULL)                           // Check if the file actually exists
                {
                    file = fopen("database/semesters.txt", "w"); // Create it if it doesn't
                    fclose(file);
                    file = fopen("database/semesters.txt", "r");
                }
                while (getline(&curGrade, &len, grades) != -1) // Read line by line
                {
                    if (strcmp(strtok(curGrade, DELIM), current->gradeType) == 0) // Check if the grade types match
                    {
                        gradePoints = strtok(NULL, DELIM);
                        break; // Found the grade we were looking for and saved the information
                    }
                }
                fclose(grades);
                // Add this course's credits hours to the total
                totalCreditHours += atoi(courseHours);
                // Add this course's grade's grade points to the total
                totalGradePoints += atof(gradePoints);
                // Print out the taken course
                printf("%s%s %s (%s) %s\n", current->coursePrefix, current->courseNumber, courseTitle, courseHours, current->gradeType);
                // Remove the current linked list node
                if (current == head)
                {
                    head = head->next;
                    free(current);
                    current = head;
                    previous = head;
                }
                else
                {
                    previous->next = current->next;
                    free(current);
                    current = previous->next;
                }
            }
            else
            {
                previous = current;
                current = current->next;
            }
        }
    }
    fclose(file);
    printf("  STUDENT HOURS COMPLETED: %d\n", totalCreditHours);
    printf("  STUDENT GPA: %f\n", (totalGradePoints / numCourses));
    return 0;
}