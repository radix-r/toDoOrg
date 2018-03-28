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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OK 0
#define NO_INPUT 1
#define TOO_LONG 2

#define MAX_LINE_SZ 100

// declarations
FILE* fileStuff(char *fName);
static int getLine(char *buff, size_t sz, FILE* fid);

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

  char line[MAX_LINE_SZ];
  int status = -1;
  status = getLine(line, MAX_LINE_SZ, fid);

  if(status == OK){
    printf("%s\n", line);
  }

  fclose(fid);
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
