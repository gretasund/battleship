#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>


struct target_list {
    char *name;
    double latitude;
    double longitude;
    struct target_list *next;
};
typedef struct target_list TargetList;



typedef struct char_list CharList;
void print_menu(void);
int get_option(void);
int load_target_file(char **targets, TargetList **start);
char *dynamicString(void);
int validate_name(char *targetName);
int validate_coordinate(char *latitude, char *longitude);
int validate_length(char **single_target);
int validate_range(char *latitude, char *longitude);
int validate_conflict(char **single_target, TargetList *start);
int add_linked_list(TargetList **start, char *ptrName, char *ptrLatitude, char *ptrLongitude);
int add_linked_list2(TargetList **start, char *ptrName, double latitude, double longitude);
void list_print(TargetList *start);
int search_target(TargetList *start);
TargetList* targets_damage_zone(TargetList *start);
TargetList* execute_air_strike(TargetList **start1);
void delete_target(TargetList **head_ref, int position);



void print_menu(void) {

    printf("1) Load a target file\n");
    printf("2) Show current targets\n");
    printf("3) Search a target\n");
    printf("4) Plan an airstrike\n");
    printf("5) Execute an airstrike\n");
    printf("6) Quit\n");

}



int get_option(void) {

    // declare variables
    int status = 0;

    while(status == 0){

        char option[1] = {};

        // get user input
        printf("Option: ");
        status = scanf("%2s", option);

        if(getchar() == 1){
            print("hallo\n");
        }
        print("hallo\n");

        while(getchar() != '\n');

        // check the input buffer
        if (strlen(option) == 2) {
            if (option[0] >48 && option[0] <55) {
                status = 0;
                return (int)option[0]-48;
            }
            else {
                status = 0;
                printf("Unknown option.\n");
            }
        }

        else {
            status = 0;
            printf("Unknown option.\n");
        }

    }
    return 0;
}



int load_target_file(char **targets, TargetList **start) {

    // declare variables
    char *file_name = NULL;
    FILE *fp;


    // get user input
    printf("Enter a target file: ");
    file_name = dynamicString();


    // open the file, display error message if invalid
    fp = fopen(file_name,"r");
    if (fp == NULL) {
    printf("Invalid file.\n");
    return 0;
    }


    // read the whole file
    fseek(fp, 0, SEEK_END);                 // sets the offset to the end
    long fpsize = ftell(fp);                // returns the given file position
    rewind(fp);                             // sets the position to the beginning
    char *string = malloc(fpsize + 1);

    if (string == NULL) {                   // check the return value of malloc & fread!
        printf("Unable to allocate memory.\n");
        exit(-1);
    }

    fread(string, 1, fpsize, fp);           // read the whole file
    fclose(fp);                             // close the file
    string[fpsize] = 0;


    // separate the string
    char *temp = strtok(string, " \t\r\n\f\v");
    int number_targets = 0;

    while(temp != NULL) {
        targets[number_targets] = temp;
        temp = strtok(NULL, " \t\r\n\f\v");
        number_targets++;
    }

    for (int i = 0; i<number_targets; i+=3) {

        char *single_target[3] = {};
        single_target[0] = targets[i+0];
        single_target[1] = targets[i+1];
        single_target[2] = targets[i+2];

        /*int a = validate_name(targets[i+0]);
        int b = validate_coordinate(targets[i+1], targets[i+2]);
        int c = validate_length(single_target);
        int d = validate_range(targets[i+1], targets[i+1]);
        printf("a = %d, b = %d, c = %d, d = %d\n", a, b, c, d);*/

        if(validate_name(targets[i+0]) == 0 ||
           validate_coordinate(targets[i+1], targets[i+2]) == 0 ||
           validate_length(single_target) == 0 ||
           validate_range(targets[i+1], targets[i+1]) == 0) {
            printf("Invalid file.\n");
            return 0;
        }
    }


    for (int i = 0; i<number_targets; i+=3) {

        char *single_target2[3] = {};
        single_target2[0] = targets[i+0];
        single_target2[1] = targets[i+1];
        single_target2[2] = targets[i+2];

        if(validate_conflict(single_target2, *start) == 1) {
            add_linked_list(start, targets[i+0], targets[i+1], targets[i+2]);
        }
    }

    free(string);
    return 1;
}


char *dynamicString(void){

    char *line = NULL, *tmp = NULL;
    size_t size = 0, index = 0;
    int ch = EOF;

    while (ch) {
        ch = getc(stdin);

        // Check if we need to stop
        if (ch == EOF || ch == '\n') {
            ch = 0;
        }

        // Check if we need to expand
        if (size <= index) {
            size += sizeof(char);
            tmp = realloc(line, size);
            if (!tmp) {
                free(line);
                line = NULL;
                break;
            }
            line = tmp;
        }

        // Actually store the thing
        line[index++] = ch;
    }

    return line;

}



int validate_name(char *targetName) {

    // declare variables
    int length = 0;

    // validate each char of the name
    while (targetName[length] != '\0') {
        if (
            (targetName[length] >47 && targetName[length] <58)    ||    //0-9 = 48-57
            (targetName[length] >64 && targetName[length] <91)    ||    //A-Z = 65-90
            (targetName[length] >96 && targetName[length] <123)         //a-z = 97-122
            ){
            length++;
        }

        else {
            return 0;
        }
    }

    return 1;

}



int validate_coordinate(char *latitude, char *longitude) {

    // declare variables
    int length = 0;

    // validate the latitude
    while (latitude[length] != '\0') {

        if ((latitude[length] >44 && latitude[length] <47) ||     //  - = 45  . = 46
            (latitude[length] >47 && latitude[length] <58)) {     //0-9 = 48-57
            length++;
        }

        else {
            return 0;
        }
    }
    length = 0;

    // validate the longitude
    while (longitude[length] != '\0') {

        if ((longitude[length] >44 && longitude[length] <47) ||     //  - = 45  . = 46
            (longitude[length] >47 && longitude[length] <58)) {     //0-9 = 48-57
            length++;
        }

        else {
            return 0;
        }
    }
    length = 0;

    return 1;

}



int validate_length(char **single_target) {

    // declare variables
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



int validate_range(char *charLatitude, char *charLongitude) {

    // declare variables
    double min = 0;
    double max = 100;

    // convert the string to double
    double latitude = 0;
    sscanf(charLatitude, "%lf", &latitude);
    double longitude = 0;
    sscanf(charLongitude, "%lf", &longitude);


    // validate range
    if (latitude >= min && latitude <= max) {
    }

    else {
        return 0;
    }

    if (longitude >= min && longitude <= max) {
    }

    else {
        return 0;
    }

    return 1;

}



int validate_conflict(char **single_target, TargetList *start) {

    // declare variables
    double latitude = 0;
    double longitude = 0;
    sscanf(single_target[1], "%lf", &latitude);
    sscanf(single_target[2], "%lf", &longitude);

    if (start != NULL) {
        double vector = (pow((start->latitude)-latitude, 2) + pow((start->longitude)-longitude, 2));
        double length_vector = sqrt(vector);

        if (length_vector < 0.1) {
            return 0;
        }

        while (start->next != NULL) {
            start = start->next;
            double vector = (pow((start->latitude)-latitude, 2) + pow((start->longitude)-longitude, 2));
            double length_vector = sqrt(vector);

            if (length_vector < 0.1) {
                return 0;
            }
        }
    }
    return 1;
}



int add_linked_list(TargetList **start, char *ptrName, char *ptrLatitude, char *ptrLongitude) {

    // declare variables and change data type
    double latitude = 0;
    double longitude = 0;
    sscanf(ptrLatitude, "%lf", &latitude);
    sscanf(ptrLongitude, "%lf", &longitude);


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
    return 0;
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
    return 0;
}



void list_print(TargetList *start){

   while(start != NULL){
       printf("%s ", start->name);
       printf("%f ", start->latitude);
       printf("%f\n", start->longitude);
       start = start->next;
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
    printf("\n");
}



int search_target(TargetList *start){

    // declare variables
    char target[16] = { };

    // get user input
    printf("Enter the name: ");
    scanf("%[^\n]", target);
    if(strcmp(target, "\0") == 0) {
        return 0;
    }

    // search target if list is not empty
    while(start != NULL){
        if (strcmp(start->name, target) == 0) {
            printf("%s ", start->name);
            printf("%f ", start->latitude);
            printf("%f\n", start->longitude);
            return 1;
        }
       start = start->next;
    }

    // if the while loop didn´t return,
    // the target wasn´t found
    printf("Entry does not exist.\n");
    return 0;
}



TargetList *targets_damage_zone(TargetList *start) {

    // declare variables
    char charLatitude[4] = {};
    char charLongitude[4] = {};
    char charDamageZone[4] = {};
    double latitude = 0;
    double longitude = 0;
    double damage_zone = 0;
    TargetList *targets_damage_zone = NULL;


    // get user input
    printf("Enter targeted latitude: ");
    scanf("%s", charLatitude);
    printf("Enter targeted longitude: ");
    scanf("%s", charLongitude);
    printf("Enter radius of damage zone: ");
    scanf("%s", charDamageZone);


    // check input validity
    if (validate_coordinate(charLatitude, charLongitude) == 1 &&
        validate_range(charLatitude, charLongitude) == 1) {

        sscanf(charLatitude, "%lf", &latitude);
        sscanf(charLongitude, "%lf", &longitude);
        sscanf(charDamageZone, "%lf", &damage_zone);

        // check the first list entry
        if (start != NULL) {
            double vector = (pow((start->latitude)-latitude, 2) + pow((start->longitude)-longitude, 2));
            double length_vector = sqrt(vector);
            if (length_vector <= damage_zone) {
                add_linked_list2(&targets_damage_zone, start->name, start->latitude, start->longitude);
            }

            // check the following list entries
            while (start->next != NULL) {
                start = start->next;
                double vector = (pow((start->latitude)-latitude, 2) + pow((start->longitude)-longitude, 2));
                double length_vector = sqrt(vector);
                if (length_vector <= damage_zone) {
                    add_linked_list2(&targets_damage_zone, start->name, start->latitude, start->longitude);
                }
            }
        }
    }

    else {
        printf("Invalid coordinates.\n");
        return 0;
    }

    return targets_damage_zone;
}



TargetList *execute_air_strike(TargetList **start1) {

    // declare variables
    TargetList *list_targets_destroyed = NULL;
    TargetList *targets_damage_zone = NULL;
    TargetList *start = *(start1);
    char charLatitude[4] = {};
    char charLongitude[4] = {};
    char charDamageZone[4] = {};
    double latitude = 0;
    double longitude = 0;
    double damage_zone = 0;
    int targets_destroyed = 0;
    int index = 0;


    // get user input
    printf("Enter targeted latitude: ");
    scanf("%s", charLatitude);
    printf("Enter targeted longitude: ");
    scanf("%s", charLongitude);
    printf("Enter radius of damage zone: ");
    scanf("%s", charDamageZone);


    // check input validity
    if (validate_coordinate(charLatitude, charLongitude) == 1 &&
        validate_range(charLatitude, charLongitude) == 1) {

        sscanf(charLatitude, "%lf", &latitude);
        sscanf(charLongitude, "%lf", &longitude);
        sscanf(charDamageZone, "%lf", &damage_zone);


        // check the first entry
        if (start != NULL) {
            double vector = (pow((start->latitude)-latitude, 2) + pow((start->longitude)-longitude, 2));
            double length_vector = sqrt(vector);
            if (length_vector <= damage_zone) {
                targets_destroyed++;
                add_linked_list2(&list_targets_destroyed, start->name, start->latitude, start->longitude);
                delete_target(start1, index);
                index--;
            }

            // check the following entries
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

    }

    else {
        printf("Invalid coordinates.\n");
        return 0;
    }


    // print the number of destroyed targets
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
        *start = temp->next;      // Change start
        free(temp);               // free old start
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

    // print the menu
    print_menu();


    // declare variables
    int option = 0;
    char *targets = NULL;
    TargetList *start = NULL;
    TargetList *list_targets_damage_zone = NULL;


    // user chooses an option
    while (option != 6) {
        option = get_option();

        // perform the correct option
        switch (option) {
            case 1:
                load_target_file(&targets, &start);
                break;

            case 2:
                list_print(start);
                map_print(start);
                break;

            case 3:
                search_target(start);
                break;

            case 4:
                list_targets_damage_zone = targets_damage_zone(start);
                list_print(list_targets_damage_zone);
                break;

            case 5:
                execute_air_strike(&start);
                break;

            case 6:
                exit(1);

            default:
                printf("Something went wrong.\n");

        }
    }

    printf("Exit.\n");

    // Free memory

    return 0;
}
