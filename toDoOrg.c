/*

This is a relitivly simple program that orginizes a to do list with
a specific format. This program reads the name of a text file from the command
line and sorts the lines base on what the start with and potentaly the date
associated with the line.

It follows the gerneral rule that lines that start with "(X)" , "(×)" , "(:C)",
or "(:c)" should go below line that dont start with those strings.

Also as an added goal I would like to recognize dates in the for <month>/<day>
and sort the lines based on that with later dates twords the top of the list. I
could probably do this using a priority queue where the value of the line is
31*<month>+<day>. This, however would not work well for goals that hapen after
the new year.

compile: gcc toDoOrg.c -o org
run: ./org <to do file name>

*/

#include <limits.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OK 0
#define NO_INPUT 1
#define TOO_LONG 2

#define MAX_LINE_SZ 100

typedef struct pqNode{
  int value;// this id the date value. 0 is default else 32*<month>+<day>
  char line[MAX_LINE_SZ];
  struct pqNode * next;
  struct pqNode * prev;
}pqNode;

// declarations
int dateValue(char ** linePtr);
void destroyLL(pqNode * head);
FILE* fileStuff(char *fName);
static int getLine(char *buff, size_t sz, FILE* fid);
static int initPQ(pqNode **pqHeadPtr, pqNode **pqTailPtr);
void offer(pqNode *** pqPtr, pqNode * new);
//int poll(pqNode * pq[2], char ** dest);
void printLL(pqNode ** pq, FILE * fid);

int main(int argc, char **argv){

  if(argc != 2){
    printf("Usage: ./org <to do file name>");
    return -1;
  }

  char * fName = argv[1];
  FILE *fid = fileStuff(fName);
  if(fid == NULL){
    return -1;
  }

  char * line = calloc(1 , MAX_LINE_SZ);
  //int status = -1;
  //status = ;

  pqNode ** toDo= calloc(2,sizeof(pqNode*));// head [0] and tail [1] of todo
  //toDo[0] = calloc(1,sizeof(pqNode*));
  int status1 = initPQ(&(toDo[0]), &(toDo[1]));
  pqNode ** done = calloc(2,sizeof(pqNode*));// head [0] and tail [1] of done
  int status2 = initPQ(&(done[0]), &(done[1]));
  pqNode ** header = calloc(2,sizeof(pqNode*));// head [0] and tail [1]
  int status3 = initPQ(&(header[0]), &(header[1]));
  pqNode ** space = calloc(2,sizeof(pqNode*));// head [0] and tail [1]
  int status4 = initPQ(&(space[0]), &(space[1]));

  // holds the next node to be printed to a file
  //char * hold;// = calloc(1, sizeof(pqNode));

  // check nulls
  if(status1 == -1 || status2 == -1 || status3 == -1 || status4 == -1){
    //printf("nulls\n");
    return -1;
  }

  while(getLine(line, MAX_LINE_SZ, fid) == OK){
    pqNode * new = calloc(1,sizeof(pqNode));

    strcpy(new->line, line);
    // detremine if line has a date in it and the value of the date if it does
    new->value = dateValue(&line);



    // detremine if line is done or not
    // if line starts with '(' it is done
    if(line[0] == '('){
      // offer to done queue
      offer(&done, new);
    }
    else if(line[0] == '-'){
      offer(&header,new);
    }
    else if(line[0] == '\r'){
      offer(&space, new);
    }
    else{
      // offer to todo queue
      offer(&toDo, new);
    }
    //printf("%s , %d\n", new->line, new->value);
  }

  free(line);
  fclose(fid);

  fid = fopen(fName,"w");
  if (fid == NULL){
    printf("Failed to reopen: %s\n", fName);
    return -1;
  }

  printLL(header, fid);
  printLL(toDo, fid);
  printLL(space, fid);
  printLL(done, fid);

  fclose(fid);

  destroyLL(toDo[0]);
  destroyLL(done[0]);
  destroyLL(space[0]);
  destroyLL(header[0]);
  free(toDo);
  free(done);
  free (header);
  free(space);
}


int dateValue(char **linePtr){
  int len = strlen(*linePtr);

  int i =0,mIndex, monthVal,dIndex, dayVal, found = 0,state = 0;
  char ch;
  char *monthTxt = calloc(1,MAX_LINE_SZ);
  char *dayTxt = calloc(1,MAX_LINE_SZ);
  char *dummy;


  while(i < len && !found){
    switch (state){
      case 0: // start
        mIndex = 0;
        dIndex = 0;
        ch = (*linePtr)[i++];
        if(ch >= '0' && ch <='9'){
          monthTxt[mIndex++] = ch;
          state = 1;
        }
        break;
      case 1: // digit found
        ch = (*linePtr)[i++];
        if(ch >= '0' && ch <='9'){
          monthTxt[mIndex++] = ch;
          state = 1;
        }
        else if(ch == '/'){
          state = 2;
        }
        else{
          state = 0;
        }
        break;
      case 2: // digits followed by '/'
        ch = (*linePtr)[i++];
        if(ch >= '0' && ch <='9'){
          dayTxt[dIndex++] = ch;
          state = 3;
        }
        else{
          state = 0;
        }
        break;
      case 3: // digits followed by '/' followed by digits
        ch = (*linePtr)[i++];
        if(ch >= '0' && ch <='9'){
          dayTxt[dIndex++] = ch;
          state = 3;
        }
        else{
          found = 1;
          monthTxt[mIndex]='\0';
          dayTxt[dIndex] = '\0';
          monthVal = (int) strtol(monthTxt, &dummy, 10);
          dayVal = (int) strtol(dayTxt, &dummy, 10);

        }
      break;
    }
  }

  free (monthTxt);
  free(dayTxt);
  if (found){
    return monthVal*32+dayVal;
  }
  else{
    return INT_MAX-1;
  }
}

/**
frees momory associated with a linked list
*/
void destroyLL(pqNode * head){
  if (head -> next == NULL){
    free(head);
    return;

  }
  destroyLL(head->next);
  free(head);
}

/*
this function hadels the opening of the file

@parameter fNamw, the desiered file name

@return file, a pointer to the open file containing the instructions for the vm.
returns null on failure
*/
FILE* fileStuff(char *fName){

  FILE* file = fopen(fName, "r");

  if(file == NULL){
    printf("Failed to open %s.\n", fName);
  }

  return file;
}



/**
A method of getting input from file line by line. lifted form
https://stackoverflow.com/questions/4023895/how-to-read-string-entered-by-user-in-c

@parameter buff, temporary location of the line read in by fgets
@parameter sz, max number of characters that fgets will read per line
@parameter fid, a file pointer to where we are reading line from

@return status, OK(0), NO_INPUT(1), and TOO_LONG(2)
*/
static int getLine(char *buff, size_t sz, FILE* fid){
  int last, ch, extra;//i=1;

  // get line with buffer overrun protection
  //fflush(stdout);

  if(fgets(buff, sz, fid) == NULL){
    return NO_INPUT;
  }

  // If it was too long, there'll be no newline. in that case we flush
  // to end of line so the excess doesnt affect the next call
  if ((last = buff[strlen(buff)-1] != '\n') && (last != EOF)){
    extra = 0;
    while(((ch = fgetc(fid)) != '\n') && (ch!= EOF)){
      extra = 1;
    }
    return (extra == 1) ? TOO_LONG : OK;
  }

  // Otherwise remove non digit trailing characters and give string back to caller
  //while(last = buff[strlen(buff)-i] < '0' || last > '9' ){
      buff[strlen(buff) - 1] = '\0';
  //}

  return OK;

}


/**
Initalizes a priority queue with two setinal nodes: head and tail

@param pq, where the pointer to the head of the Initalized priority queue will go

@return status, OK(0), -1 on failure
*/
static int initPQ(pqNode **pqHeadPtr, pqNode **pqTailPtr){
  pqNode * head = NULL;
  head = calloc(1, sizeof(pqNode));
  pqNode * tail = NULL;
  tail = calloc(1, sizeof(pqNode));

  if(head == NULL || tail == NULL){
    printf("Memory allocation failed\n" );
    return -1;
  }

  strcpy(head->line, "HEAD");
  head->value = INT_MAX;
  head->next = tail;
  head->prev = NULL;

  strcpy(tail->line,"TAIL");
  tail->value = INT_MIN;
  tail->next = NULL;
  tail->prev = head;

  *pqHeadPtr = head;
  *pqTailPtr = tail;
  return OK;
}

/**
add node to queue based on its value
*/
void offer(pqNode *** pqPtr, pqNode * new){
  // start at the tail
  pqNode * current = (*pqPtr)[1];
  // continue untill we encounter a value >= new->value
  while(current->value <= new->value && current != (*pqPtr)[0]){
    current = current->prev;
  }

  //
  new->prev = current;
  new->next = current->next;

  current->next->prev = new;
  current->next = new;
}

/**



int poll(pqNode * pq[2], char ** dest){
  if(pq[0]->next == pq[1]){
    // if head's next is tail
    return -1;
  }
  else{
    pqNode * ret; //what will be returned

    ret = pq[0]->next;
    pq[0]->next->next->prev=pq[0];
    pq[0]->next = pq[0]->next->next;

    strcpy(*dest, ret->line);
    free(ret);
    return OK;
  }
}
*/

/*
prints list from back(smallest vaulue) to front(largest values)
*/
void printLL(pqNode ** pq, FILE * fid){
  pqNode * current = pq[1]->prev;
  while(current != pq[0]){
    fprintf(fid,"%s\n", current->line);
    //fflush(stdout);
    current = current->prev;
  }
}
