#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int print_menue(void) {
    
    int status, input, buffer;
    status = 0;
    
    printf("1) Load a target file\n");
    printf("2) Show current targets\n");
    printf("3) Search a target\n");
    printf("4) Plan an airstrike\n");
    printf("5) Execute an airstrike\n");
    printf("6) Quit\n");

    while(status == 0){
        
        // get user input
        printf("Please enter a number: ");
        status = scanf("%d", &input);
        
        // check the input buffer
        buffer = getchar();
        if (buffer == 10) {
            if (input > 0 && input <7) {
                break;
            }
            else {
                status = 0;
                printf("Unknown option.\n");
            }
        }
        
        else {
            status = 0;
            printf("Unknown option.\n");
            while(getchar() != '\n');
        }
        
    }
    
    printf("Option: %d\n", input);

    return input;
    
}


void replaceAll(char *string, char oldChar, char newChar)
{
    int i = 0;

    /* Run till end of string */
    while(string[i] != '\0')
    {
        /* If occurrence of character is found */
        if(string[i] == oldChar)
        {
            string[i] = newChar;
        }

        i++;
    }
}


void load_target_file(void) {
    
    // declare variables
    char file_name[30] = {};
    FILE *fp;
    // char buffer[255];
    // char buffer2[255];
    
    // get user input
    printf("Enter a target file: \n");
    scanf("%s", file_name);
    
    // open the file
    fp = fopen(file_name,"r");              // file_valid.txt
    
    // display error message if invalid
    if (fp == NULL) {
    printf("Invalid file.\n");
    }
    
    // read and print the whole file
    fseek(fp, 0, SEEK_END);                 // sets the offset to the end
    long fpsize = ftell(fp);                // returns the given file position
    rewind(fp);                             // sets the position to the beginning
    
    char *string = malloc(fpsize + 1);      // check the return value of malloc & fread!
    fread(string, 1, fpsize, fp);           // read the whole file
    fclose(fp);                             // close the file

    string[fpsize] = 0;                     // free string
    printf("%s\n", string);
    
    // print the string´s single chars
    printf("Print the string´s single chars:\n");
    for (int i = 0; i<fpsize; i++) {
        printf("string[%d] = %c\n", i, string[i]);
    }
    
    // replace all '\n' with a ' '
    // string[35] = ' ';
    // string[77] = ' ';
    // replaceAll(string, '\n', ' ');
    
    // print the string´s single chars
    printf("Print the string´s single chars:\n");
    for (int i = 0; i<fpsize; i++) {
        printf("string[%d] = %c\n", i, string[i]);
    }

    // separate the string
    char *temp = strtok(string, " \t\r\n\f\v");
    char *targets[30][17] = {};
    int j = 0;
    int k = 0;

    while(temp != NULL) {
        targets[j][0] = temp;
        //printf("%s\n", temp);
        temp = strtok(NULL, " \t\r\n\f\v");
        j++;
        
        if (j%3  == 0) {
            printf("\n");
        }
    }
    
    
    // print the separated string
    printf("Print the separated string:\n");
    for (int i = 0; i<30; i++) {
        printf("targets[%d] = %s\n", i, *(targets[i]));
    }
    
    
    // Print the separated string
    /*while (targets[k] != NULL && j < 18) {
    int length = strlen(*(targets[k]));
    printf("targets[%d] = %s, length = %d\n",  k, *(targets[k]), length);
    k++;
    }*/
    
    //printf("target[5] = %s and %lu\n", *(targets[5]), strlen(*(targets[5])));
    //printf("target[12] = %s and %lu\n", *(targets[12]), strlen(*(targets[12])));
    
    for (int i = 0; i< fpsize; i++) {
        printf("%c", string[i]);
    }
    puts("\n");
    
    printf("targets[12] = %s\n", targets[12][0]);
    printf("targets[12] = %d\n", (int)targets[12][0]);
}


int main(void) {
    
    int option;
    
    // user chooses an option
    option = print_menue();
    printf("Your choice is %d\n", option);
        
    // perform the correct option
    switch (option) {
        case 1:
            load_target_file();
            break;
            
        default:
            printf("Not implemented yet.\n");
            break;
    }

}