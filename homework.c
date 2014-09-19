/*
 * file:        homework.c
 * description: Skeleton for homework 1
 *
 * CS 5600, Computer Systems, Northeastern CCIS
 * Peter Desnoyers, Jan. 2012
 * $Id: homework.c 500 2012-01-15 16:15:23Z pjd $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "uprog.h"

/***********************************/
/* DECLARATIONS FOR code in misc.c */
/***********************************/

typedef int *stack_ptr_t;
extern void init_memory(void);
extern void do_switch(stack_ptr_t *location_for_old_sp, stack_ptr_t new_value);
extern stack_ptr_t setup_stack(int *stack, void *func);
extern int get_term_input(char *buf, size_t len);
extern void init_terms(void);

extern void  *proc1;
extern void  *proc1_stack;
extern void  *proc2;
extern void  *proc2_stack;
extern void **vector;

int readfile(char *file, char *buf, int buflen);
char *strword(char *s, char *buf, size_t len);

/***********************************************/
/********* Your code starts here ***************/
/***********************************************/

/*
 * Question 1.
 *
 * The micro-program q1prog.c has already been written, and uses the
 * 'print' micro-system-call (index 0 in the vector table) to print
 * out "Hello world".
 *
 * You'll need to write the (very simple) print() function below, and
 * then put a pointer to it in vector[0].
 *
 * Then you read the micro-program 'q1prog' into memory starting at
 * address 'proc1', and execute it, printing "Hello world".
 *
 */
void print(char *line)
{
    /*
     * Your code goes here. 
     */
    printf("%s", line);
}

void q1(void)
{
    /*
     * Your code goes here. Initialize the vector table, load the
     * code, and go.
     */
    readfile("q1prog", (char*) proc1, 106);
    *vector = print;
    //print((char*)proc1);
    int (*a)(void) = NULL;
    a = proc1;
    a();
}

int readfile(char *file, char *buf, int buflen)
{
    FILE *fp = fopen(file, "r");
    if (fp == NULL) {
	perror("can't open file");
	return 0;
    }
    int i, c;
    for (i = 0; i < buflen; i++)
      {
	c = getc(fp);
	if (c == EOF)
		break;
	buf[i] = c;
    }
    fclose(fp);
    return i;
}

char *strword(char *s, char buf[], size_t len)
{
  s += strspn(s, " ");
  int n = strcspn(s, " ");
  if(len-1 < n)
    n = len-1;
  memcpy(buf, s, n);
  buf[n] = 0;
  s += n;
  return (*s == 0) ? NULL : s;
}

/*
 * Question 2.
 *
 * Add two more functions to the vector table:
 *   void readline(char *buf, int len) - read a line of input into 'buf'
 *   char *getarg(int i) - gets the i'th argument (see below)

 * Write a simple command line which prints a prompt and reads command
 * lines of the form 'cmd arg1 arg2 ...'. For each command line:
 *   - save arg1, arg2, ... in a location where they can be retrieved
 *     by 'getarg'
 *   - load and run the micro-program named by 'cmd'
 *   - if the command is "quit", then exit rather than running anything
 *
 * Note that this should be a general command line, allowing you to
 * execute arbitrary commands that you may not have written yet. You
 * are provided with a command that will work with this - 'q2prog',
 * which is a simple version of the 'grep' command.
 *
 * NOTE - your vector assignments have to mirror the ones in vector.s:
 *   0 = print
 *   1 = readline
 *   2 = getarg
 */
void readline(char *buf, int len) /* vector index = 1 */
{
    /*
     * Your code here.
     */
  int i = 0;
  char c;
  FILE *fp = stdin;
  while((c = getc(fp)) != 13 && c != 10 && i < len) {
    buf[i++] = c;
  }
  buf[i] = '\0';
}

char *getarg(int i)		/* vector index = 2 */
{
    /*
     * Your code here. 
     */

    return NULL;
}

/*
 * Note - see c-programming.pdf for sample code to split a line into
 * separate tokens. 
 */
void q2(void)
{
    /* Your code goes here */
  char buf[128];
  void *temp_vector = NULL;
	char words[10][20];
	char *line = malloc(20*sizeof(char));

  //temp_vector = *(vector + 1);
	*(vector+1) = readline;
	//temp_vector = readline;
  temp_vector = *(vector + 2);
  temp_vector = getarg;
 
    while (1) {
	/* get a line */
	/* split it into words */
	/* if zero words, continue */
	/* if first word is "quit", break */
	/* make sure 'getarg' can find the remaining words */
	/* load and run the command */
      readline(buf, sizeof(buf));
      //readfile("q1prog", (char*) proc1, 106);
		int i;
		for (i = 0; i < 10; i++) {
			line = strword(buf, words[i], sizeof(words[i]));
			if (buf == NULL)
				break;
		}
		
		if (strcmp(words[0], "quit"))
			break;
		readfile(words[0], (char*) proc1, 3000);
    //print((char*)proc1);
    int (*a)(void) = NULL;
    a = proc1;
    a();
    }
    /*
     * Note that you should allow the user to load an arbitrary command,
     * and print an error if you can't find and load the command binary.
     */
}

/*
 * Question 3.
 *
 * Create two processes which switch back and forth.
 *
 * You will need to add another 3 functions to the table:
 *   void yield12(void) - save process 1, switch to process 2
 *   void yield21(void) - save process 2, switch to process 1
 *   void uexit(void) - return to original homework.c stack
 *
 * The code for this question will load 2 micro-programs, q3prog1 and
 * q3prog2, which are provided and merely consists of interleaved
 * calls to yield12() or yield21() and print(), finishing with uexit().
 *
 * Hints:
 * - Use setup_stack() to set up the stack for each process. It returns
 *   a stack pointer value which you can switch to.
 * - you need a global variable for each process to store its context
 *   (i.e. stack pointer)
 * - To start you use do_switch() to switch to the stack pointer for 
 *   process 1
 */

void yield12(void)		/* vector index = 3 */
{
    /* Your code here */
}

void yield21(void)		/* vector index = 4 */
{
    /* Your code here */
}

void uexit(void)		/* vector index = 5 */
{
    /* Your code here */
}

void q3(void)
{
    /* Your code here */
    /* load q3prog1 into process 1 and q3prog2 into process 2 */
}


/***********************************************/
/*********** Your code ends here ***************/
/***********************************************/
