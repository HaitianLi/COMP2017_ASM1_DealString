#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define MAXBUFFER 4097
//because hh:mm:ss\n\0 is 10
#define TimeBuffer 10
#define TimeSize 3
#define DailyTotalSec 86400
#define COMMANDARGLIMIT 4



void Clean_buffer() {
    //may be dont need this, even there are garbage left
    //since the while lop of fgets, and keep check 4096 bit
    //with funcion check colon, if not good input, it will keep going.
    //

    char ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        ;
    }
}

int Check_time_format(char * timestr) {


    if ((*timestr >= '0' && *timestr<= '2')&&
        (*(timestr + 1)>= '0' && *(timestr + 1)<= '9')&&
        (*(timestr + 2) == ':')&&
        (*(timestr + 3)>= '0' && *(timestr + 3)<= '5')&&
        (*(timestr + 4)>= '0' && *(timestr + 4)<= '9')&&
        (*(timestr + 5) == ':')&&
        (*(timestr + 6)>= '0' && *(timestr + 6)<= '5')&&
        (*(timestr + 7)>= '0' && *(timestr + 7)<= '9')&&
        ((*timestr - '0') * 10 + (*(timestr + 1) - '0') <= 23) &&
        (*(timestr + 8) == '\n')
       ) 
    {
        return 1;
    } else {
        //Clean_buffer();
        return 0;
    }
}

//fgets will auto add \0 at the end of the buffer
//the format of a string is c c c c c \n \0;
int Check_Double_Colons(char * str) {
    
    int DoubleColons_counts = 0;
    //3 possible way the while loop stop
    //1. the end of the string(since fgets will auto add \0 at the end)
    //2. already get 2 double colons
    //3. read in one : but not double
    //if 1, means there no enough colons, return 0;
    //if 2, it should immediately check the time format. let the check_time() do the last check
    //  since the time format is fixed with 00:00:00\n
    while (*str) {
        //check the colons between source and destination
        if (*str == ':') {
            str++;
            if (*str == ':') {
                DoubleColons_counts++;
            } else {
                return 0;
            }
        }
        str++;
        if (DoubleColons_counts == 2) {
            return Check_time_format(str);
        }
    }
    //since the colons are not as the good format, the entry is bad
    //need to be clean up the stdin until the next entry
    //could make a test case here for garbage handle with the 4096garbage + good entry
    //without the clean_buffer() it will read the garbage in;
    Clean_buffer();
    return 0;
}
//just add this to push
void My_strcpy(char *to, char *from) {
    assert(from && to);

    while ((*to++ = *from++) != '\0' ) {
        ;
    }
}

int My_strcmp(const char* w1, const char * w2) {
    int ret = 0;
    assert(w1 != NULL);
    assert(w2 != NULL);

    while (!(ret = *w1 - *w2) && *w2) {
        w1++;
        w2++;
    }
    
    return ret;
}

int My_strlen(char* target) {
    
    if (*target) {
        return My_strlen(target + 1) + 1;
    }
    return 0;
    
}

char * SeparateBuffer(char * ptr, char * to) {
    char * start = ptr;
    while (*ptr++ != ':') {
        ;
    }
    //*ptr == :, go back a unit.
    ptr--;
    *ptr = '\0';
    My_strcpy(to, start);
    //printf("%s\n", to);
    *ptr = ':'; //make it as origion

    //skip the ::
    ptr += 2;
    return ptr;
}

int main(int argc, char ** argv) {
    //check the command line arguments
    if (argc != COMMANDARGLIMIT) {
        printf("Please provide <source> <destination> <time> as command line arguments\n");
        exit(1);
    }
    
    //set the max day sec which is 24 * 60 * 60
    //set the user's time in sec as well
    int offset_time = DailyTotalSec; //24 * 60 * 60
    int target_time[TimeSize] = {0};
    int user_time;
    int Early_time = DailyTotalSec;
    
    // if (Check_time_format(argv[3])) {

    // } else {
    //     printf("In time section, please provide <hh>:<mm>:<ss>\n");
    //     exit(1);
    // }
    sscanf(argv[3], "%d:%d:%d", &target_time[0],&target_time[1],&target_time[2]);







































 














    user_time = target_time[0] * 60 * 60 + target_time[1] * 60 + target_time[2];


    char answer_source[MAXBUFFER] = {0};
    char answer_dest[MAXBUFFER] = {0};
    int answer_time[TimeSize];

    char answer_early_source[MAXBUFFER] = {0};
    char answer_early_dest[MAXBUFFER] = {0};
    int answer_early_time[TimeSize];

    //noolean variable to count if any data exist or time for train exist
    int time_exist = 0;
    int data_exist = 0;
    int early_time_exist = 0;
    
    //the buffer traverse store the entry, the data will refresh with the while loop
    char Buffer[MAXBUFFER] = {0};

    
    while (fgets(Buffer, MAXBUFFER, stdin) != NULL) {
        //not a empty input file
        //printf("%s\n", Buffer);
        if (Check_Double_Colons(Buffer)) {
            data_exist = 1;
            //printf("%s\n", Buffer);
            //since it is a valid entry
            //separate different parts
            char source[MAXBUFFER] = {0};
            char destination[MAXBUFFER] = {0};
            char time[TimeBuffer] = {0};
            int int_time[TimeSize] = {0};
            
            //start creat strings by using the pointer and My strcpy
            char * ptr = Buffer;
            char * second_ptr = SeparateBuffer(ptr, source);
            My_strcpy(time, SeparateBuffer(second_ptr,destination));
            sscanf(time, "%d:%d:%d", &int_time[0],&int_time[1],&int_time[2]);
            
            //find the same source and destination
            if (My_strcmp(source, argv[1]) == 0 && My_strcmp(destination, argv[2]) == 0) {
                early_time_exist = 1;

                int entry_time = int_time[0] * 60 * 60 + int_time[1] * 60 + int_time[2];

                //if the source and the dest match, there at least a time
                //since it is a fixed day timetable, if there is no train for later day.
                //will keep track the earlyest day time.
                if (entry_time < Early_time) {
                    Early_time = entry_time;
                    My_strcpy(answer_early_source, source);
                    My_strcpy(answer_early_dest, destination);
                    answer_early_time[0] = int_time[0];
                    answer_early_time[1] = int_time[1];
                    answer_early_time[2] = int_time[2];
                }
                //find the closed time and refresh the answer
                if (entry_time >= user_time && entry_time < offset_time) {
                    
                    time_exist = 1;
                    offset_time = entry_time;
                    My_strcpy(answer_source, source);
                    My_strcpy(answer_dest, destination);
                    answer_time[0] = int_time[0];
                    answer_time[1] = int_time[1];
                    answer_time[2] = int_time[2];
                }
                
            }
            
        }
    }
    if (!data_exist) {
        //printf("No timetable to process\n");
        fprintf(stderr,"%s" ,"No timetable to process\n");
        exit(0);
    }

    if (!time_exist) {
        if (early_time_exist) {
            printf("The next train to %s from %s departs at %02d:%02d:%02d\n", answer_early_dest, answer_early_source, answer_early_time[0],answer_early_time[1],answer_early_time[2]);
        } else {
            printf("No suitable trains can be found\n");
        }
        
    } else {
        printf("The next train to %s from %s departs at %02d:%02d:%02d\n", answer_dest, answer_source, answer_time[0],answer_time[1],answer_time[2]);
    }

    return 0;
}