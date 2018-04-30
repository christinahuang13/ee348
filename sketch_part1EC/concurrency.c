#include <stdio.h>
#include <avr/io.h>
#include <stdlib.h>
#include "concurrency.h"
#define INIT_SIZE (10)

/* ========= START OF Priority Queue Implementation ========= */

int runflag = 0;
int PriorityQueueInit(PriorityQueue * p){
  p->max_size = INIT_SIZE;
  p->num_nodes = 0;
  for (int i = 0; i < 257; i++){
    p->usedPriority[i] = NULL;
  }
  p->heap = malloc(sizeof (process_t *) * (INIT_SIZE));
  if (!p->heap){
    return -1;
  }
  
  for (int i = 0; i < INIT_SIZE; i++){
    p->heap[i] = NULL;
  }
  return 0;
}

void heapify(process_t ** heap, int curr, int count){
  int left, right, largest;
  process_t * temp;
  left = 2*curr + 1;
  right = left +1;
  largest = curr;

  if (left <= count && heap[left]->priority > heap[largest]->priority){
    largest = left;
  }
  if (right <= count && heap[right]->priority > heap[largest]->priority){
    largest = right;
  }

  if (largest != curr){
    temp = heap[curr];
    heap[curr] = heap[largest];
    heap[largest] = temp;
    heapify(heap, largest, count);
  }
}

int pushPQueue (PriorityQueue * h, process_t * newElem){
  int index, parent;
  // resize heap in priority queue if too small
  if (h->num_nodes == h->max_size){
    h->max_size *=2;
    h->heap = realloc(h->heap, sizeof(process_t *) * h->max_size);
    for (int i = h->max_size/2; i < h->max_size; i++){
      h->heap[i] = NULL;
    }
    if (!h->heap)
      return -1;
  }
  process_t * t;
  if ((t = h->usedPriority[newElem->priority]) == NULL){
    // if there is no node with that priority, find start index; 
    // Gives first available space in array
    index = h->num_nodes; 
    h->num_nodes++;
    // Find location of element
    for (; index; index = parent){
      parent = (index - 1) / 2 ;
      if (h->heap[parent]->priority >= newElem->priority)
        break;
      h->heap[index] = h->heap[parent];
    }
    h->heap[index] = newElem;
    h->usedPriority[newElem->priority] = newElem;
  } else{
    // heap contains index with same priority. Find empty space at end of linked list
    process_t * temp = t;
    while(temp && temp->next != NULL){
      temp = temp->next;
    }
    temp->next = newElem;
    temp->next->next = NULL;
  }
  return 0;
}

process_t * popPQueue(PriorityQueue * h){
  if (h->num_nodes == 0){
    return NULL;
  }
  process_t * removed;
  process_t * temp = h->heap[h->num_nodes-1];
  // You can dynamically downsize this queue but I'm too lazy to
  removed = h->heap[0];
  h->heap[0] = h->heap[0]->next;
  
  if (h->heap[0] == NULL){
    h->num_nodes--;
    // Last node that that priority removed. Update usedPriority and heapify.
    h->usedPriority[removed->priority] = NULL;
    h->heap[0] = temp;
    heapify(h->heap, 0, h->num_nodes);
  } else{
    h->usedPriority[removed->priority] = h->heap[0];
    removed->next = NULL;
  }
  return removed;
}

int destroyPQueue(PriorityQueue * h){
  process_t * temp;
  while (h->num_nodes != 0){
    temp =  popPQueue(h);
    if (temp != NULL){
      free(temp);
    }
  }

  free(h->heap);
  return 0;
}

int PriorityQueueIsEmpty(PriorityQueue * h){
  if (h->num_nodes == 0)
    return 1;
  else
    return 0;
}

void displayPQueue(PriorityQueue * h){
  for (int i = 0; i < h->num_nodes; i++){
    process_t * temp = h->heap[i];
    printf("|(%i:%i): ", i, temp->priority);
    while (temp != NULL){
      printf("%i ", temp->sp);
      temp = temp->next;
    }
    printf("\n");
  }
    printf("*************************\n");
}

/* ========= END OF Priority Queue Implementation ========= */

/* linked list of available processes */
__attribute__((used)) unsigned char _orig_sp_hi, _orig_sp_lo;

__attribute__((used)) void process_begin ()
{
  asm volatile (
    "cli \n\t"
    "in r24,__SP_L__ \n\t"
    "sts _orig_sp_lo, r24\n\t"
    "in r25,__SP_H__ \n\t"
    "sts _orig_sp_hi, r25\n\t"
    "ldi r24, 0\n\t"
    "ldi r25, 0\n\t"
    "rjmp .dead_proc_entry\n\t"
    );
}

__attribute__((used)) void process_terminated ()
{
  asm volatile (
    "cli\n\t"
    "lds r25, _orig_sp_hi\n\t"
    "out __SP_H__, r25\n\t"
    "lds r24, _orig_sp_lo\n\t"
    "out __SP_L__, r24\n\t"
    "ldi r24, lo8(0)\n\t"
    "ldi r25, hi8(0)\n\t"
    "rjmp .dead_proc_entry"
    );
}

void process_timer_interrupt ();

__attribute__((used)) void yield ()
{
  if (!current_process) return;
  asm volatile ("cli\n\t");
  asm volatile ("rjmp process_timer_interrupt\n\t");
}

__attribute__((used)) void process_timer_interrupt()
{
  asm volatile (
    "push r31\n\t"
    "push r30\n\t"
    "push r29\n\t"
    "push r28\n\t"
    "push r27\n\t"
    "push r26\n\t"
    "push r25\n\t"
    "push r24\n\t"
    "push r23\n\t"
    "push r22\n\t"
    "push r21\n\t"
    "push r20\n\t"
    "push r19\n\t"
    "push r18\n\t"
    "push r17\n\t"
    "push r16\n\t"
    "push r15\n\t"
    "push r14\n\t"
    "push r13\n\t"
    "push r12\n\t"
    "push r11\n\t"
    "push r10\n\t"
    "push r9\n\t"
    "push r8\n\t"
    "push r7\n\t"
    "push r6\n\t"
    "push r5\n\t"
    "push r4\n\t"
    "push r3\n\t"
    "push r2\n\t"
    "push r1\n\t"
    "push r0\n\t"
    "in r24, __SREG__\n\t"
    "push r24\n\t"
    "in r24, __SP_L__\n\t"
    "in r25, __SP_H__\n\t"
    ".dead_proc_entry:\n\t"
    "rcall process_select\n\t"
    "eor r18,r18\n\t"
    "or r18,r24\n\t"
    "or r18,r25\n\t"
    "brne .label_resume\n\t"
    "lds r25, _orig_sp_hi\n\t"
    "out __SP_H__, r25\n\t"
    "lds r24, _orig_sp_lo\n\t"
    "out __SP_L__, r24\n\t"
    "ret\n\t"
    ".label_resume:\n\t"
    "out __SP_L__, r24\n\t"
    "out __SP_H__, r25\n\t"
    "pop r0\n\t"
    "out  __SREG__, r0\n\t"
    "pop r0\n\t"
    "pop r1\n\t"
    "pop r2\n\t"
    "pop r3\n\t"
    "pop r4\n\t"
    "pop r5\n\t"
    "pop r6\n\t"
    "pop r7\n\t"
    "pop r8\n\t"
    "pop r9\n\t"
    "pop r10\n\t"
    "pop r11\n\t"
    "pop r12\n\t"
    "pop r13\n\t"
    "pop r14\n\t"
    "pop r15\n\t"
    "pop r16\n\t"
    "pop r17\n\t"
    "pop r18\n\t"
    "pop r19\n\t"
    "pop r20\n\t"
    "pop r21\n\t"
    "pop r22\n\t"
    "pop r23\n\t"
    "pop r24\n\t"
    "pop r25\n\t"
    "pop r26\n\t"
    "pop r27\n\t"
    "pop r28\n\t"
    "pop r29\n\t"
    "pop r30\n\t"
    "pop r31\n\t"
    "reti\n\t");
}


/*
 * Stack: save 32 regs, +2 for entry point +2 for ret address
 */
#define EXTRA_SPACE 37
#define EXTRA_PAD 4

unsigned int process_init (void (*f) (void), int n, process_t * newProcess)
{
  unsigned long stk;
  int i;
  unsigned char *stkspace;

  /* Create a new process */
  n += EXTRA_SPACE + EXTRA_PAD;
  stkspace = (unsigned char *) malloc (n);

  if (stkspace == NULL) {
    /* failed! */
    return 0;
  }

  /* Create the "standard" stack, including entry point */
  for (i=0; i < n; i++) {
      stkspace[i] = 0;
  }

  n -= EXTRA_PAD;

  stkspace[n-1] = ( (unsigned int) process_terminated ) & 0xff;
  stkspace[n-2] = ( (unsigned int) process_terminated ) >> 8;
  stkspace[n-3] = ( (unsigned int) f ) & 0xff;
  stkspace[n-4] = ( (unsigned int) f ) >> 8;

  /* SREG */
  stkspace[n-EXTRA_SPACE] = SREG;
//  return stkspace;
  stk = (unsigned int)stkspace + n - EXTRA_SPACE - 1;
//
  newProcess->stkspace = stkspace;
  return stk;
}

/* ------------------------ Concurrency Implementation ------------------------ */
process_t *queue;
process_t *current_process;

// Updated process_select with checks for blocking / locks
// If a process is blocked, it means it tried acquiring the lock, but it couldn't get it
// It was thus sent to the lock queue. A blocked current process thus cannot be sent to
// the end of the ready queue.
__attribute__((used)) unsigned int process_select (unsigned int cursp)
{
  /* Called by the runtime system to select another process.
     "cursp" = the stack pointer for the currently running process
  */ 

  if (current_process && current_process->block == 1)
  {
    current_process->sp = cursp;      // Process is blocked, so we know it's waiting to acquire a lock in a lock queue
    current_process = popPQueue(prio_queue);
    return current_process->sp;
  }

  if (cursp == 0) {
    if (PriorityQueueIsEmpty(prio_queue)) {
      // No process has been created, or all processes have terminated
      // Free current_process is available
      if (current_process){
        free(current_process->stkspace);
        free(current_process);
      }
      return 0;
    }
    else {                           
      // If no current_process, then we need to schedule one
      // else, free current_process and stackspace
      if (current_process){
        free(current_process->stkspace);
        free(current_process);
      }
      current_process = popPQueue(prio_queue);
      return current_process->sp;
    }
  }

  if (cursp != 0) {
    if (PriorityQueueIsEmpty(prio_queue))
    {
      // We have a current process, but no queue
      // If this is the case, allow it to keep running
      return cursp;
    }
  }

  // Here, we know we have a queue, and current process. We want to find the 
  // end of the queue, and attach the current process to the end of it
  // and then we want to move the process 
  // that queue is pointing to to current process
  // and then have the tail of the queue point to
  // the current process. Set the new tail to point to null.

  current_process->sp = cursp;
  current_process->next = NULL;
  pushPQueue(prio_queue, current_process);
  current_process = popPQueue(prio_queue);
  //comment this out to get one led blinking
  return current_process->sp;
}

void process_start (void)
{
  /* Starts up the concurrent execution */
  process_begin();
}

int process_create(void (*f)(void), int n){
  // Default priority is 128
  return process_create_prio (f, n, 128);
}

int process_create_prio (void (*f)(void), int n, unsigned int prio)
{
  /* Create a new process */
  process_t *new_process = malloc(sizeof(process_t));
  
  if (new_process == NULL) { 
    // or, !new_process
    return -1;
  }

  // Get stack_pointer from process_init (if successful, will also set new_process->stkspace)
  unsigned int stack_pointer = process_init(f, n, new_process);
  
  if (stack_pointer == 0) {                  
    // Check to see if there is memory for stack and malloc
    return -1;
  }
  
  new_process->sp = stack_pointer;
  new_process->next = NULL;                    // New process doesn't point to anything
  new_process->block = 0;                      // New process starts unblocked
  new_process->priority = prio;
  pushPQueue(prio_queue, new_process);
  return new_process->sp;
}

/************************** Lock related functions and code ****************************/

void lock_init(lock_t *l) {
  l->current = NULL;                          // Pointer to current process that holds the lock
  l->locked = NULL;                           // Pointer to queue of locked processes
}

void lock_acquire(lock_t *l) {

  asm volatile ("cli\n\t");                  // disable interrupts before checking lock aspects
  
  if (l->current == NULL) {                  // No process currently holds the lock
                                             // So allow the current process requesting the lock to aquire the lock
    l->current = current_process;
    l->current->block = 0;                   // Not nessecary in this logic implementation 
  }
  else if (l->current != current_process) {
                                            // Block the current process if it doesn't have the lock
                                            // and throw it on the lock waiting queue
     if (l->locked == NULL) {
                                            // Nothing is waiting for the lock
                                            // Add to the lock queue, make it the head
                                            // Make it point to null since there is nothing else in the queue
      current_process->block = 1;
      current_process->next = NULL;
      l->locked = current_process;
      
     }
     else {
                                            // Otherwise, there is a queue for the lock
                                            // Find the tail of the lock queue, and link the current process to it
      int flag = 1;
      process_t *process_track = l->locked;

      while (flag) {
        if (process_track->next == NULL) {
          flag = 0;                             // We found the tail; process_track holds the tail
        }
        else {
        process_track = process_track->next;    // Point to the next waiting process in the lock queue
        }
      }
                                                // Add the current process to the lock queue of that particular lock
      current_process->next = NULL; 
      process_track->next = current_process;
                                                // make current process to point to null
     }
                                                // Block the current process, and call yield() to go to next function
     current_process->block = 1;
   
     yield();    
  }
  asm volatile ("sei\n\t"); // re-enable interrupts after you yield

}

void lock_release(lock_t *l) {
  asm volatile ("cli\n\t");                  // disable interrupts before checking lock aspects
 
  if (l->locked == NULL) {                   // No process is waiting for the lock, so
    l->current = NULL;                       // release the lock, set it to null and return
    l->current->block = 0;
    return;
  }

  // Otherwise, there is something in the lock queue
  // Let the first thing in the lock queue acquire the lock
  // and then send it to the end of the ready queue
  if (l->locked != NULL) {                    
    l->current = l->locked;
    l->current->block = 0;                   // unblock the process
    l->locked = l->locked->next;             // locked now points to whatever is next in the lock queue, or null
  }
  // l->current points to the process that now holds the lock
  // Find the end of the ready queue, and put the new process with the lock on the ready queue
  // If the ready queue is empty, make the process popping off the lock queue the head of the ready queue
  l->current->block = 0;
  l->current->next = NULL;
  pushPQueue(prio_queue, l->current);    
  asm volatile ("sei\n\t");             // re-enable interrupts
}


