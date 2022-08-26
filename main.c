#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    while(1) {
        printf(">>>"); // Prompt User 
        char buffer[2048];
        fgets(buffer,sizeof(buffer),stdin); // Read in user input
        char i1[2]="",i2[32]="",i3[32]="",i4[32]="",i5[32]="",i6[32]="",i7[32]="",i8[32]="";
        sscanf(buffer, "%s %s %s %s %s %s %s %s",i1,i2,i3,i4,i5,i6,i7,i8); // Parse user input
        if(strcmp(i1,"a") == 0) { // Add
            
        } else if(strcmp(i1,"l") == 0) { // List
            printf("list\n");
        } else if(strcmp(i1,"t") == 0) { // Transcript
            printf("transcript\n");
        } else if(strcmp(i1,"q") == 0) { // Quit
            return 0;
        } else { // Error: Wrong/Invalid Command
            printf("Invalid command. Valid commands are: (a)dd, (l)ist, (t)ranscript, and (q)uit.\n");
        }
    }
}