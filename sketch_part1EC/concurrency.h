/*************************************************************************
 *
 *  Copyright (c) 2013--2018 Rajit Manohar
 *  All Rights Reserved
 *
 **************************************************************************
 */
#ifndef __CONCURRENCY_H__
#define __CONCURRENCY_H__


#ifdef __cplusplus
extern "C" {
#endif

typedef struct process_state process_t;
   /* opaque definition of process type; you must provide this
      implementation.
   */

// External functions used in cpp file.
extern void printFromC(const char *);
extern void printFromC_ln(const char *);
extern void printJobTerm(process_t *);    // Prints out all terminating job information
extern unsigned long getCurrMillis();     // Hack to print out current millis from Arduino library

/*------------------------------------------------------------------------
   THE FOLLOWING FUNCTIONS MUST BE PROVIDED.
------------------------------------------------------------------------*/
/* ========= Priority Queue Implementation ========= */
typedef struct p_queue PriorityQueue;
extern PriorityQueue * prio_queue;

//int process_create_prio(void (*f)(void), int n, unsigned int prio);

struct p_queue {
  process_t ** heap; // Array of process_t pointers representing linked list
  int max_size;
  int num_nodes;
  process_t* usedPriority[257];
};

process_t * peekPQueue(PriorityQueue * h);
void displayPQueue(PriorityQueue * h);
int PriorityQueueInit(PriorityQueue * p);
int pushPQueue (PriorityQueue * h, process_t *);
process_t * popPQueue(PriorityQueue * h);
int destroyPQueue(PriorityQueue *);
int isFirstRealTimeJob(PriorityQueue * h);

/* ====== Part 1 ====== */
struct process_state
{
  // Value of the stack pointer
  unsigned int sp;
  struct process_state *next;
  int block;                        // 1 if blocks, 0 if not blocked
  unsigned int priority;
  unsigned char * stkspace;
  unsigned int wcet;
  unsigned long start_time;     // Takes in millis() of when program first receives process
  unsigned long finish_time;    // start_time + deadline
};

extern process_t *current_process; 
extern process_t *tail_process; 
/* the currently running process */

__attribute__((used)) unsigned int process_select (unsigned int cursp);
/* Called by the runtime system to select another process.
   "cursp" = the stack pointer for the currently running process
*/

void process_start (void);
/* Starts up the concurrent execution */

int process_create_general(void (*f)(void), int n, unsigned int prio, int wcet, process_t *new_process);

int process_create (void (*f)(void), int n);
/* Create a new process - calls process_create_prio with prio == 128*/

int process_create_prio (void (*f)(void), int n, unsigned int prio);
/* Create a new priority process */

int process_create_rtjob(void(*f)(void), int n, unsigned int wcet, unsigned int deadline);
/* Create a new real time process */
/* ===== Part 2 ====== */
typedef struct lock_state lock_t;
  /* you have to define the lock_state structure */
  
struct lock_state
{
  struct process_state *current;
  struct process_state *locked;    // Points to head of a lock queue for a specific lock                                 
};



void lock_init (lock_t *l);
void lock_acquire (lock_t *l);
void lock_release (lock_t *l);

/*-- functions provided in the .c file --*/

// Process_init altered to keep track of stackspace (for freeing later)
unsigned int process_init (void (*f) (void), int n, process_t * );
void process_begin ();
void process_timer_interrupt ();
void yield ();

#ifdef __cplusplus
}
#endif

#endif /* __CONCURRENCY_H__ */
