#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int print_menue(void);
void load_target_file(void);
int validate_alphabet(char **single_target);
int validate_length(char **single_target);
int validate_range(char **single_target);
int validate_conflict(char **single_target);
void add_linked_list(char **targets, int number_targets);


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
    
    add_linked_list(targets, number_targets);

}



int validate_alphabet(char **single_target) {

    int length = 0;
    
    
    // validate the name -> single_target[0]
    while (single_target[0][length] != '\0') {
        
        if (
            (single_target[0][length] >47 && single_target[0][length] <58)    ||    //0-9 = 48-57
            (single_target[0][length] >64 && single_target[0][length] <91)    ||    //A-Z = 65-90
            (single_target[0][length] >96 && single_target[0][length] <123)         //a-z = 97-122
            )
        {
            length++;
        }
        
        else {
            // printf("This is the invalid character = length = %c\n", single_target[0][length]);
            return 0;
        }
    }
    length = 0;
    

    // validate the values -> single_target[1] and [2]
    for (int i = 1; i<3; i++) {
        while (single_target[i][length] != '\0') {
            
            if (
                (single_target[i][length] >44 && single_target[i][length] <47) ||    //  - = 45  . = 46
                (single_target[i][length] >47 && single_target[i][length] <58)       //0-9 = 48-57
                )
            {
                length++;
            }
            
            else {
                return 0;
            }
        }
        length = 0;

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



int validate_range(char **single_target) {
    
    // declare variables
    double min = 0;
    double max = 100;
    
    // convert the string to double
    double latitude = 0;
    sscanf(single_target[1], "%lf", &latitude);
    printf("latitude %lf\n", latitude);
    
    double longitude = 0;
    sscanf(single_target[2], "%lf", &longitude);
    printf("longitude %lf\n", longitude);
    
    
    // validate range
    if (latitude >= min && latitude <= max) {
        // printf("Check\n");
    }
    
    else {
        return 0;
    }
    
    if (longitude >= min && longitude <= max) {
        // printf("Check\n");
    }
    
    else {
        return 0;
    }
    
    return 1;
    
}



int validate_conflict(char **single_target) {
    
    return 1;
    
}



void add_linked_list(char **targets, int number_targets) {
    
    for (int i = 0; i<number_targets; i+=3) {
        char *single_target[3] = {};
        single_target[0] = targets[i+0];
        single_target[1] = targets[i+1];
        single_target[2] = targets[i+2];
        printf("\nsingle_target[%d] = %s %s %s\n", i/3, single_target[0], single_target[1], single_target[2]);
        
        
        // do the checking
        int a = validate_alphabet(single_target);
        int l = validate_length(single_target);
        int r = validate_range(single_target);
        int c = validate_conflict(single_target);
        
        
        // validate the single_target
        if (a == 1 && l == 1 && r == 1 && c == 1)
        {
            printf("Add.\n");
        }
        
        else {
            //printf("a = %d\nl = %d\nr = %d\nc = %d\n", a, l, r, c);
            printf("Remove.\n");
        }
        
        
        //debugging:
        // check for alphabet
        /*if (validate_alphabet(single_target) == 1) {
            printf("A valid = yes.\n");
        }
        
        else {
            printf("A valid = no.\n");
        }
        
        
        // check for length
        if (validate_length(single_target) == 1) {
            printf("L valid = yes.\n");
        }
        
        else {
            printf("L valid = no.\n");
        }*/
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