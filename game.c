#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int print_menue(void);
void load_target_file(void);
int validate_input(char **targets, int number_targets);


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


void load_target_file(void) {
    
    // declare variables
    char file_name[30] = "file_valid.txt";
    FILE *fp;
    int number_targets = 0;
    
    // get user input
    printf("Enter a target file: \n");
    // scanf("%s", file_name);
    
    // open the file
    fp = fopen(file_name,"r");
    
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
    //printf("%s\n", string);
    
    // print the string´s single chars
    /*printf("Print the string´s single chars:\n");
    for (int i = 0; i<fpsize; i++) {
        printf("string[%d] = %c\n", i, string[i]);
    }*/
    
    // separate the string
    char *temp = strtok(string, " \t\r\n\f\v");
    char *targets[30] = {};
    int j = 0;

    while(temp != NULL) {
        targets[j] = temp;
        temp = strtok(NULL, " \t\r\n\f\v");
        j++;
    }
    number_targets = j;
    
    // print the separated string
    printf("Print the separated string:\n");
    for (int i = 0; i<number_targets+1; i+=3) {
        printf("targets[%d] =\n", i);
        printf(" name[%d] = %s\n", i, targets[i]);
        printf(" value[%d] = %s\n", i, targets[i+1]);
        printf(" value[%d] = %s\n", i, targets[i+2]);
        printf("\n");
    }
    
    // validate the input
    if (validate_input(targets, number_targets) == 1) {
        printf("Valid file.\n");
    }
    
    else {
        printf("Invalid file!\n");
    }

}


int validate_input(char **targets, int number_targets) {

    int j = 0;
    int length = 0;
    
    for (int i = 0; i<number_targets; i++) {
        
        j = 0;
        length = 0;
        
        while (targets[i][j] != '\0') {
            length++;
            j++;
        }
        
        //printf("At %d length = %d\n", i, length);

        if(length > 16) {
            return 0;
        }
    }
    
    return 1;
    
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
