#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int print_menue(void);
void load_target_file(void);
int validate_alphabet(char **single_target);
int validate_length(char **single_target);


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
    char file_name[30] = "file_invalid.txt";
    FILE *fp;
    int number_targets = 0;
    
    
    // get user input
    printf("Enter a target file: \n");
    // scanf("%s", file_name);
    
    
    // open the file, display error message if invalid
    fp = fopen(file_name,"r");
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

    string[fpsize] = 0;                     // free string!!!
    
    
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
    printf("numberTargets: %d\n", number_targets);
    
    
    // print the separated string
    printf("Print the separated string:\n");
    for (int i = 0; i<number_targets; i+=3) {
        printf("targets[%d] =\n", i/3);
        printf(" name[%d] = %s\n", i/3, targets[i]);
        printf(" value[%d] = %s\n", i/3, targets[i+1]);
        printf(" value[%d] = %s\n", i/3, targets[i+2]);
        printf("\n");
    }
    
    
    // validate the target 1
    for (int i = 0; i<number_targets; i+=3) {
        char *single_target[3] = {};
        single_target[0] = targets[i+0];
        single_target[1] = targets[i+1];
        single_target[2] = targets[i+2];
    
        if (validate_alphabet(single_target) == 1) {
            printf("targets[%d] = valid target.\n", i/3);
        }
        
        else {
            printf("targets[%d] = invalid target.\n", i/3);
        }
    }

}



int validate_alphabet(char **single_target) {

    int length = 0;
    
    // no more than 15 chars / digits
    for (int i = 0; i<3; i++) {
        length = 0;
        
        while (single_target[i][length] != '\0') {
            
            if (single_target[i][length] == 45 ||         //  - = 45
                single_target[i][length] == 46 ||         //  . = 46
                single_target[i][length] >47 && <58 ||    //0-9 = 48-57
                single_target[i][length] >64 && <91 ||    //A-Z = 65-90
                single_target[i][length] >96 && <123)     //a-z = 97-122
            {
                length++;
            }
            
            else {
                return 0;
            }
        }

    }
    
    return 1;
    
}


int validate_length(char **single_target) {
    
    // for debugging purposes:
    /*printf("*(single_target) = %s\n", *(single_target));
    printf("single_target[0] = %s\n", single_target[0]);
    printf("single_target[1] = %s\n", single_target[1]);*/

    int length = 0;
    
    // no more than 15 chars / digits
    for (int i = 0; i<3; i++) {
        length = 0;
        
        while (single_target[i][length] != '\0') {
            length++;
        }
        // printf("At %d length = %d\n", i, length);

        if(length > 15) {
            return 0;
        }
    }
    
    return 1;
    
}



void add_linked_list(char **targets, int number_targets) {
    
    char *single_target[3] = {};                        // array that holds three char pointers
    

    for (int i = 0; i<number_targets; i+=3) {
    // fill single_target
    single_target[0] = targets[i+0];                    // name
    single_target[1] = targets[i+1];                    // value 1
    single_target[2] = targets[i+2];                    // value 2
    
    // validate the single_target
    if (validate_length(single_target) == 1){
        /*validate_alphabet(single_target[0]) == 1 &&
        validate_range(single_target[1], single_target[2]) == 1 &&
        validate_conflict(single_target) == 1*/
        
        // code to add to list
        printf("This targets should be added to the list.\n");
        
        }
    }
    
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