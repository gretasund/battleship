#include <stdio.h>
#include <stdlib.h>
#include <string.h>



struct target_list {
    char *name;
    double latitude;
    double longitude;
    struct target_list *next;
};
typedef struct target_list TargetList;



typedef struct char_list CharList;
int print_menue(void);
int load_target_file(char **targets);
int validate_alphabet(char **single_target);
int validate_length(char **single_target);
int validate_range(char **single_target);
int validate_conflict(char **single_target);
int validate_all_criteria(char *ptrName, char *ptrLatitude, char *ptrLongitude);
int add_linked_list(TargetList **start, char *ptrName, char *ptrLatitude, char *ptrLongitude);
void list_print(TargetList *start);


int print_menue(void) {
    
    int status, input, buffer;
    status = 0;
    
    printf("1) Load a target file\n");
    printf("2) Show current targets\n");
    printf("3) Search a target\n");
    printf("4) Plan an airstrike\n");
    printf("5) Execute an airstrike\n");
    printf("6) Quit\n");

    while(status==0){
        
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



int load_target_file(char **targets) {
    
    // declare variables
    char file_name[30] = {};//"file_valid.txt"
    FILE *fp;
    int number_targets = 0;
    
    
    // get user input
    printf("Enter a target file: \n");
    scanf("%s", file_name);
    
    
    // open the file, display error message if invalid
    fp = fopen(file_name,"r");
    if (fp == NULL) {
    printf("Invalid file.\n");
    }
    
    else {
    //printf("Valid file.\n");
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
    int j = 0;

    while(temp != NULL) {
        targets[j] = temp;
        temp = strtok(NULL, " \t\r\n\f\v");
        j++;
    }
    number_targets = j;
    
    
    // print the separated string
    printf("\nPrint the separated string:\n");
    for (int i = 0; i<number_targets; i+=3) {
        printf("targets[%d] =\n", i/3);
        printf(" name[%d] = %s\n", i/3, targets[0]);
        printf("  latitude[%d] = %s\n", i/3, targets[1]);
        printf(" longitude[%d] = %s\n", i/3, targets[2]);
        printf("\n");
    }
    
    return number_targets;

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
    // printf("latitude %lf\n", latitude);
    
    double longitude = 0;
    sscanf(single_target[2], "%lf", &longitude);
    // printf("longitude %lf\n", longitude);
    
    
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



int validate_all_criteria(char *ptrName, char *ptrLatitude, char *ptrLongitude) {
    
    char *single_target[3] = {};
    single_target[0] = ptrName;
    single_target[1] = ptrLatitude;
    single_target[2] = ptrLongitude;

    // check the different validity criteria
    int a = validate_alphabet(single_target);
    int l = validate_length(single_target);
    int r = validate_range(single_target);
    int c = validate_conflict(single_target);
    
    
    // validate the single_target as a whole
    if (a == 1 && l == 1 && r == 1 && c == 1) {
        return 1;
    }
    
    else {
    return 0;
    }

    return 0;
  
}
    
    

int add_linked_list(TargetList **start, char *ptrName, char *ptrLatitude, char *ptrLongitude) {
    
    // Allocate memory
    TargetList *newTarget = malloc(sizeof(TargetList));
    
    // change data type
    double latitude = 0;
    double longitude = 0;
    sscanf(ptrLatitude, "%lf", &latitude);
    sscanf(ptrLongitude, "%lf", &longitude);
    
    // If memory could be allocated
    if (newTarget != NULL) {
        
        // set parameters
        newTarget->name = ptrName;
        newTarget->latitude = latitude;
        newTarget->longitude = longitude;
        newTarget->next = NULL;
        
        // make start of the list
        if(*start == NULL){
           *start = newTarget;
        }
         
        // append to the list
        else {
           TargetList *cur = *start;
           while(cur->next != NULL){
               cur = cur->next;
           }
           cur->next = newTarget;
        }
        
        return 1;
        
    }
    
    else {
        return 0;
    }

}



void list_print(TargetList *start){
    
   TargetList *cur = start;
   while(cur != NULL)
   {
       printf("%3s - ", cur->name);
       printf("%3f - ", cur->latitude);
       printf("%3f\n", cur->longitude);
       cur = cur->next;
   }
   printf("\n");
}



int main(void) {
    
    int option = 0;
    char *targets[30] = {};
    int number_targets = 0;
    
    char *ptrName = NULL;
    char *ptrLatitude = 0;
    char *ptrLongitude = 0;
    TargetList *start = NULL;
    
    // user chooses an option
    while (option != 6) {
        
        option = print_menue();
        
        if (option != 6) {
        printf("Your choice is %d\n", option);
        }
            
        
        // perform the correct option
        switch (option) {
            case 1:
                number_targets = load_target_file(targets);
                
                for (int i = 0; i<number_targets; i+=3) {
                ptrName = targets[i+0];
                ptrLatitude = targets[i+1];
                ptrLongitude = targets[i+2];
                
                    if(validate_all_criteria(ptrName, ptrLatitude, ptrLongitude) == 1){
                        add_linked_list(&start, ptrName, ptrLatitude, ptrLongitude);
                    }
                }
                list_print(start);
                break;
                
            case 6:
                exit(1);
                
            default:
                printf("Not implemented yet.\n");
                break;
        }
    }

}