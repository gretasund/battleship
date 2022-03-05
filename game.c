#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>



struct target_list {
    char *name;
    double latitude;
    double longitude;
    struct target_list *next;
};
typedef struct target_list TargetList;



typedef struct char_list CharList;
int print_menu(void);
int load_target_file(char **targets);
int validate_alphabet(char **single_target);
int validate_length(char **single_target);
int validate_range(char **single_target);
int validate_conflict(char **single_target, TargetList *start);
int validate_all_criteria(char *ptrName, char *ptrLatitude, char *ptrLongitude, TargetList *start);
int add_linked_list(TargetList **start, char *ptrName, char *ptrLatitude, char *ptrLongitude);
int add_linked_list2(TargetList **start, char *ptrName, double latitude, double longitude);
void list_print(TargetList *start);
void targets_print(char **targets, int number_targets);
int search_target(TargetList *start);
TargetList* targets_damage_zone(TargetList *start);
void delete_target(TargetList **head_ref, int position);


int print_menu(void) {

    int status, input, buffer;
    status = 0;

    printf("\n");
    printf("1) Load a target file\n");
    printf("2) Show current targets\n");
    printf("3) Search a target\n");
    printf("4) Plan an airstrike\n");
    printf("5) Execute an airstrike\n");
    printf("6) Quit\n\n");

    while(status==0){

        // get user input
        printf("Option: ");
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

    return input;
}



int load_target_file(char **targets) {

    // declare variables
    char file_name[30] = {};//"file_valid.txt"
    FILE *fp;
    int number_targets = 0;


    // get user input
    printf("Enter a target file: ");
    scanf("%s", file_name);


    // open the file, display error message if invalid
    fp = fopen(file_name,"r");
    if (fp == NULL) {
    printf("Invalid file.\n");
    return 0;
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
    //targets_print(targets, number_targets);

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



int validate_conflict(char **single_target, TargetList *start) {

    //printf("validate conflict\n");
    double latitude = 0;
    double longitude = 0;
    sscanf(single_target[1], "%lf", &latitude);
    sscanf(single_target[2], "%lf", &longitude);

    if (start != NULL) {

        double vector = (pow((start->latitude)-latitude, 2) + pow((start->longitude)-longitude, 2));
        double length_vector = sqrt(vector);
        /*printf("latitude        = %lf\n", latitude);
        printf("longitude       = %lf\n", longitude);
        printf("cur->latitude   = %lf\n", cur->latitude);
        printf("cur->longitude  = %lf\n", cur->longitude);
        printf("vector          = %lf\n", vector);
        printf("length_vector   = %lf\n\n", length_vector);*/

        if (length_vector < 0.1) {
            //printf("\n\n");
            return 0;
        }

        while (start->next != NULL) {
            start = start->next;
            double vector = (pow((start->latitude)-latitude, 2) + pow((start->longitude)-longitude, 2));
            double length_vector = sqrt(vector);
            /*printf("latitude        = %lf\n", latitude);
            printf("longitude       = %lf\n", longitude);
            printf("cur->latitude   = %lf\n", cur->latitude);
            printf("cur->longitude  = %lf\n", cur->longitude);
            printf("vector          = %lf\n", vector);
            printf("length_vector   = %lf\n\n", length_vector);*/

            if (length_vector < 0.1) {
                //printf("\n\n");
                return 0;
            }
        }
    }

    //printf("\n\n");
    return 1;

}



int validate_all_criteria(char *ptrName, char *ptrLatitude, char *ptrLongitude, TargetList *start) {

    char *single_target[3] = {};
    single_target[0] = ptrName;
    single_target[1] = ptrLatitude;
    single_target[2] = ptrLongitude;

    // check the different validity criteria
    int a = validate_alphabet(single_target);
    int l = validate_length(single_target);
    int r = validate_range(single_target);
    int c = validate_conflict(single_target, start);


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



int add_linked_list2(TargetList **start, char *ptrName, double latitude, double longitude) {

    // Allocate memory
    TargetList *newTarget = malloc(sizeof(TargetList));


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

   while(start != NULL){
       printf("%s ", start->name);
       printf("%f ", start->latitude);
       printf("%f\n", start->longitude);
       start = start->next;
   }
}



void targets_print(char **targets, int number_targets) {

    // print the separated string
    printf("\nTargets:\n");
    for (int i = 0; i<number_targets; i+=3) {
        printf("targets[%d]\n", i/3);
        printf("name[%d]      = %s\n", i/3, targets[0+i]);
        printf("latitude[%d]  = %s\n", i/3, targets[1+i]);
        printf("longitude[%d] = %s\n", i/3, targets[2+i]);
        printf("\n");
    }
}



void map_print(TargetList *start) {

    if (start != NULL) {

        int mapY;
        int mapX;

        // initialize the map
        char map[40][80] = { };
        for (int y=0; y<40; y++){
            for (int x=0; x<80; x++) {
            map[y][x] = '_';
            }
        }

        // fill the map
        mapY = start->latitude * 0.39;
        mapX = start->longitude * 0.79;
        map[mapY][mapX] = 'x';

        while (start->next != NULL) {
            start = start->next;
            //printf("cur->latitude y   = %lf\n", start->latitude);
            //printf("cur->longitude x  = %lf\n", start->longitude);
            mapY = start->latitude * 0.39;
            mapX = start->longitude * 0.79;
            map[mapY][mapX] = 'x';
        }

        // print the map
        for (int y=39; y>-1; y--){
            for (int x=0; x<80; x++) {
            printf("%c", map[y][x]);
            }
            printf("\n");
        }
    }
}



int search_target(TargetList *start){

    char target[16] = { };

    printf("Enter the name: ");
    scanf("%[^\n]", target);
    //printf("You entered: %s\n", target);

    if(strcmp(target, "\0") == 0) {
        return 0;
    }

    while(start != NULL){
        if (strcmp(start->name, target) == 0) {
            printf("%s ", start->name);
            printf("%f ", start->latitude);
            printf("%f\n", start->longitude);
            return 1;
        }
       start = start->next;
    }
    printf("Entry does not exist.\n");
    return 0;
}



TargetList* targets_damage_zone(TargetList *start) {

    double latitude = 0;
    double longitude = 0;
    double damage_zone = 0;

    printf("Enter targeted latitude: ");
    scanf("%lf", &latitude);
    printf("Enter targeted longitude: ");
    scanf("%lf", &longitude);
    printf("Enter radius of damage zone: ");
    scanf("%lf", &damage_zone);

    TargetList *targets_damage_zone = NULL;

    if (start != NULL) {
        double vector = (pow((start->latitude)-latitude, 2) + pow((start->longitude)-longitude, 2));
        double length_vector = sqrt(vector);
        if (length_vector <= damage_zone) {
            add_linked_list2(&targets_damage_zone, start->name, start->latitude, start->longitude);
        }

        while (start->next != NULL) {
            start = start->next;
            double vector = (pow((start->latitude)-latitude, 2) + pow((start->longitude)-longitude, 2));
            double length_vector = sqrt(vector);
            if (length_vector <= damage_zone) {
                add_linked_list2(&targets_damage_zone, start->name, start->latitude, start->longitude);
            }
        }
    //printf("\n\n");
    }
    return targets_damage_zone;

}



TargetList* execute_air_strike(TargetList **start1) {

    double latitude = 0;
    double longitude = 0;
    double damage_zone = 0;
    int targets_destroyed = 0;
    int index = 0;
    TargetList *list_targets_destroyed = NULL;
    TargetList *start = *(start1);

    printf("Enter targeted latitude: ");
    scanf("%lf", &latitude);
    printf("Enter targeted longitude: ");
    scanf("%lf", &longitude);
    printf("Enter radius of damage zone: ");
    scanf("%lf", &damage_zone);

    TargetList *targets_damage_zone = NULL;

    if (start != NULL) {
        double vector = (pow((start->latitude)-latitude, 2) + pow((start->longitude)-longitude, 2));
        double length_vector = sqrt(vector);
        if (length_vector <= damage_zone) {
            targets_destroyed++;
            add_linked_list2(&list_targets_destroyed, start->name, start->latitude, start->longitude);
            delete_target(start1, index);
            index--;
        }

        while (start->next != NULL) {
            start = start->next;
            index++;
            double vector = (pow((start->latitude)-latitude, 2) + pow((start->longitude)-longitude, 2));
            double length_vector = sqrt(vector);
            if (length_vector <= damage_zone) {
                targets_destroyed++;
                add_linked_list2(&list_targets_destroyed, start->name, start->latitude, start->longitude);
                delete_target(start1, index);
                index--;
            }
        }
    }

    if (targets_destroyed > 0){
        printf("%d target destroyed\n", targets_destroyed);
        list_print(list_targets_destroyed);
    }

    else {
        printf("No target aimed. Mission cancelled.\n");
    }

    return targets_damage_zone;

}



void delete_target(TargetList **start, int position)
{
   // If TargetList is empty
   if (*start == NULL)
      return;

   // Store head node
   TargetList* temp = *start;

    // If head needs to be removed
    if (position == 0)
    {
        *start = temp->next;      // Change head
        free(temp);               // free old head
        return;
    }

    // Find previous node of the node to be deleted
    for (int i=0; temp!=NULL && i<position-1; i++)
         temp = temp->next;

    // If position is more than number of ndoes
    if (temp == NULL || temp->next == NULL)
         return;

    // Node temp->next is the node to be deleted
    // Store pointer to the next of node to be deleted
    TargetList* *next = &temp->next->next;

    // Unlink the node from linked list
    free(temp->next);  // Free memory

    temp->next = *next;  // Unlink the deleted node from list
}



int main(void) {

    int option = 0;
    int number_targets = 0;
    char *targets[30] = {};
    char *ptrName = NULL;
    char *ptrLatitude = 0;
    char *ptrLongitude = 0;
    TargetList *start = NULL;
    TargetList *list_targets_damage_zone = NULL;
    TargetList *list_targets_destroyed = NULL;

    // user chooses an option
    while (option != 6) {
        option = print_menu();

        if (option != 6) {
        }

        else {
            printf("Exit.\n");
        }

        // perform the correct option
        switch (option) {
            case 1:
                number_targets = load_target_file(targets);

                for (int i = 0; i<number_targets; i+=3) {
                ptrName = targets[i+0];
                ptrLatitude = targets[i+1];
                ptrLongitude = targets[i+2];

                    if(validate_all_criteria(ptrName, ptrLatitude, ptrLongitude, start) == 1){
                        add_linked_list(&start, ptrName, ptrLatitude, ptrLongitude);
                    }
                }
                break;

            case 2:
                list_print(start);
                map_print(start);
                printf("\n");
                break;

            case 3:
                search_target(start);
                break;

            case 4:
                list_targets_damage_zone = targets_damage_zone(start);
                list_print(list_targets_damage_zone);
                break;

            case 5:
                list_targets_destroyed = execute_air_strike(&start);
                break;

            case 6:
                exit(1);

            default:
                printf("Not implemented yet.\n");
                break;
        }
    }

}


