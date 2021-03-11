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

typedef struct failfish_queue{
    char* name;
    int pondNum;
    int n;
    int e;
    int th;
    failFish* head;
    failFish* tail;
}failfish_queue;
// The next two functions were taken from professor gerber's lecture with his permission
void remove_crlf(char *s)
{
    char *t = s + strlen(s);

     t--;

    while((t >= s) && (*t == '\n' || *t == '\r'))
    {
        *t = '\0';
        t--;
    }
}
int get_next_nonblank_line(FILE *ifp, char *buf, int max_length)
{
    buf[0] = '\0';

    while(!feof(ifp) && (buf[0] == '\0'))
    {
        fgets(buf, max_length, ifp);
        remove_crlf(buf);
    }

    if(buf[0] != '\0') {
        return 1;
    } else {
        return 0;
    }
}

failFish* create_failfish(int num){
    failFish* new = malloc(sizeof(failFish));
    new->num=num;
    return new;

}
void dispose_failfish(failFish* f){
    free(f);

}
failfish_queue* create_failfish_queue(char *pondname,int pondNum,int n, int e, int th){
    failfish_queue* q= malloc(sizeof(failfish_queue));
    q->pondNum=pondNum;
    q->name=strdup(pondname);
    q->n=n;
    q->e=e;
    q->th=th;
    q->head=NULL;
    q->tail=NULL;
    return q;

}
void dispose_failfish_queue(failfish_queue* q){
    free(q->name);
    free(q);
}
void dispose_failfish_queues(failfish_queue* ponds[11]){
    for(int i=0;i<11;i++){
        if(ponds[i]!=NULL){
            dispose_failfish_queue(ponds[i]);
        }
    }
}
void print_failfish_queue(failfish_queue* q){
    printf("%d ",q->pondNum);
    printf("%s ",q->name);
    failFish* currentFish=q->tail;
    for(int i=0;i<q->n;i++){
        printf("%d ",currentFish->num);
        currentFish=currentFish->prev;
    }
    printf("\n");
}
int get_num_ponds(char* buf){

    int numPonds;
    sscanf(buf,"%d",&numPonds);
    return numPonds;
}
void get_ponds_info(failfish_queue* ponds[11],char* buf,FILE* ifp){
    get_next_nonblank_line(ifp, buf, 256);
    int num_ponds=get_num_ponds(buf);
    int pond_num;
    char pondname[256];
    int n;
    int e;
    int th;

    for(int i=0;i<num_ponds;i++){
        get_next_nonblank_line(ifp, buf, 256);
        sscanf(buf,"%d %s %d %d %d",&pond_num,pondname,&n,&e,&th);
        ponds[pond_num]=create_failfish_queue(pondname,pond_num, n, e, th);
    }

}
void add_failfish(failfish_queue *q, failFish *f)
{
    if(q->head == NULL)
    {
        q->head = f;
        q->tail = f;
        f->prev = f;
        f->next = f;
    } else {

        f->next = q->head;
        f->prev = q->tail;

        q->head->prev = f;
        q->tail->next = f;

        q->head=f;
    }
}
void remove_failfish(failfish_queue *q,failFish* f){

    /* Case 1: Emptying the list. */

    if((q->head == q->tail) && (q->head == f)) {
        q->head = NULL;
        q->tail = NULL;
        return;
    }

    /* Case 2: Removing the head. */

    if(f == q->head) {
        q->head = q->head->next;
        q->head->prev = q->tail;
        q->tail->next = q->head;
        return;
    }
    /* Case 3: Removing the tail. */
    if(f == q->tail) {
        q->tail = q->tail->prev;
        q->tail->next = q->head;
        q->head->prev = q->tail;

        return;
    }
    f->prev->next = f->next;
    f->next->prev = f->prev;
}
failFish* getfailfish(failfish_queue *q,int num){
    failFish* currentFish=q->head;
    for(int i =0;i<q->n;i++){
        if(currentFish->num==num){
            q->n-=1;
            return currentFish;
        }
        else{
            currentFish=currentFish->next;
        }
    }
    return currentFish;

}
void fill_pond(failfish_queue* pond){
    for(int i=0;i<pond->n;i++){
        add_failfish(pond,create_failfish(i+1));
    }
}
void fill_ponds(failfish_queue* ponds[11]){
    for(int i=0;i<11;i++){
        if(ponds[i]!=NULL){
            fill_pond(ponds[i]);
        }
    }
}

/* Dispose of a list of monsters and all the monsters in it.

   This is an O(n) operation that can't be improved (we have to visit each monster
   to dispose it). */

void dispose_all_failfish(failfish_queue *q)
{
    failFish *m = q->head;
    failFish *next;

    /* First, check if the list is empty. */

    if(m == NULL)
    {
        dispose_failfish_queue(q);
        return;
    }

    /* Okay, now we know the list isn't empty, so we can free at least one element. */

    do {
        /* We can't use m->next after disposing of m, so we save a copy of m->next and
           then copy it back after disposing m. */

        next = m->next;
        dispose_failfish(m);
        m = next;
    } while(m != q->head);

    /* ml->head - and therefore m - are dangling pointers here!  But that's fine as long
       as we don't dereference them. */

    dispose_failfish_queue(q);
}
void print_failfish_queues(failfish_queue* ponds[11]){
    for(int i = 1;i<11;i++){
        if(ponds[i]!=NULL){
            print_failfish_queue(ponds[i]);
        }
    }

}
void dispose_all_ponds(failfish_queue* ponds[11]){
    for(int i =1;i<11;i++){
        if(ponds[i]!=NULL){
            dispose_all_failfish(ponds[i]);
        }
    }

}
//removes failfish and returns pointer to the next failfish
failFish* remove_fish_with_distance(failfish_queue* q, failFish* f,int e){
    failFish* nextFish=f;
    failFish* reference;
    for(int i=0;i<e-1;i++){
        nextFish=nextFish->prev;
    }
    reference=nextFish->prev;
    printf("FailFish %d Eaten \n",nextFish->num);
    remove_failfish(q, nextFish);
    dispose_failfish(nextFish);
    q->n--;
    return reference;


}
void pond_first_course(failfish_queue* q){
    printf("Pond %d: %s \n",q->pondNum,q->name);
    failFish* startingFish=q->tail;
    failFish* newFish;
    printf("starting tail number %d \n",startingFish->num);
    while(q->n > q->th){
        newFish= remove_fish_with_distance(q,startingFish,q->e);
        startingFish=newFish;
    }
}
void ponds_first_course(failfish_queue* ponds[11]){
    printf("First Course \n \n");
    for(int i=1;i<11;i++){
        if(ponds[i]!=NULL){
            pond_first_course(ponds[i]);
            printf("\n");
        }

    }

}
int main(void){
    int numberPonds;
    char buf[256];
    FILE* ifp=fopen("cop3502-as2-input.txt","r");

    //Initialize ponds with null values
    failfish_queue *ponds[11];
    for(int i =0; i<11;i++){
        ponds[i]= NULL;
    }
    //get pond information from file
    get_ponds_info(ponds, buf, ifp);
    //fill ponds with appropriate amount of fish
    fill_ponds(ponds);
    //print each pond
    print_failfish_queues(ponds);
    ponds_first_course(ponds);
    printf("End of Course Pond Status \n");
    print_failfish_queues(ponds);
    //dispose all the ponds
    dispose_all_ponds(ponds);


    













}
