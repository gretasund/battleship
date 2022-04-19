#include <stdio.h>
#include <stdlib.h>

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
    char file_name[30] = {};
    FILE *fp;
    // char buffer[255];
    // char buffer2[255];
    
    
    // get user input
    printf("Enter a target file: \n");
    scanf("%s", file_name);
    
    // open the file
    fp = fopen("file_valid.txt","r");
    
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
}



int main(void) {
    
    // declare variables
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