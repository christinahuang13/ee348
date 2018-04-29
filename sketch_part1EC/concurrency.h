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


/*------------------------------------------------------------------------
   THE FOLLOWING FUNCTIONS MUST BE PROVIDED.
------------------------------------------------------------------------*/
/* ========= Priority Queue Implementation ========= */
typedef struct p_queue PriorityQueue;
extern PriorityQueue * prio_queue;

int process_create_prio(void (*f)(void), int n, unsigned char prio);

struct p_queue {
  process_t ** heap; // Array of process_t pointers representing linked list
  int max_size;
  int num_nodes;
  process_t* usedPriority[257];
};

int PriorityQueueIsEmpty(PriorityQueue * h);
void displayPQueue(PriorityQueue * h);
int PriorityQueueInit(PriorityQueue * p);
int pushPQueue (PriorityQueue * h, process_t *);
process_t * popPQueue(PriorityQueue * h);
int destroyPQueue(PriorityQueue *);


/* ====== Part 1 ====== */
struct process_state
{
  // Value of the stack pointer
  unsigned int sp;
  struct process_state *next;
  int block;                        // 1 if blocks, 0 if not blocked
  unsigned int priority;
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

int process_create (void (*f)(void), int n);
/* Create a new process */



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

unsigned int process_init (void (*f) (void), int n);
void process_begin ();
void yield ();

#ifdef __cplusplus
}
#endif

#endif /* __CONCURRENCY_H__ */
