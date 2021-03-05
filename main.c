#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
typedef struct failFish{
    int num;
    struct failFish *next;
    struct failFish *prev;
}failFish;

typedef struct failGroup{
    char* name;
    failFish* head;
    failFish* tail;
}failGroup;

failFish* makeFailFish(int num){
    failFish* new = malloc(sizeof(failFish));
    new->num=num;
    return new;

}
int main(void){
    failGroup *failGroups[10];
    for(int i =0; i<11;i++){
        failGroups[i]= NULL;
    }


}
