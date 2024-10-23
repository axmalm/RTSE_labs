/*
 * The code below has been developed by Johan Nordlander and Fredrik Bengtsson at LTU.
 * Part of the code has been also developed, modified and extended to ARMv8 by Wagner de Morais and Hazem Ali.
*/
/*
 * Modified by Wagner Morais on Oct 2024.
 */

#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
#include <string.h>

#include "tinythreads.h"
#include "rpi-interrupts.h"
#include "uart.h"
#include "piface.h"
#include "rpi-systimer.h"

/*----------------------------------------------------------------------------
  Constants
 *----------------------------------------------------------------------------*/
#define STACKSIZE	1024
#define NTHREADS	5
// #define NULL 		0


/*----------------------------------------------------------------------------
  Internal References and Macros
 *----------------------------------------------------------------------------*/

__attribute__(( always_inline )) static inline void enable() {
	__asm volatile("CPSIE  i \n"); //Clear PRIMASK
}

__attribute__(( always_inline )) static inline void disable(){
	__asm volatile("cpsid i \n"); //set PRIMASK
}

#define DISABLE() disable()
#define ENABLE()  enable()
#define SETSTACK(buf,a) *((unsigned int *)(buf)+8) = (unsigned int)(a) + STACKSIZE - 4;	

/*----------------------------------------------------------------------------
  Thread control structures
 *----------------------------------------------------------------------------*/
struct thread_block {
	short idx;								// Unique identifier
	void (*function)(int);					// Code to run, i.e. the routine to run
	int arg;								// Argument to the above
	thread next;							// For use in linked lists
	jmp_buf context;						// Machine state
	char stack[STACKSIZE];					// Execution stack space
	unsigned int Period_Deadline;			// Absolute Period and Deadline of the thread
	unsigned int Rel_Period_Deadline;		// Relative Period and Deadline of the thread
};

struct thread_block threads[NTHREADS];
struct thread_block initp;

// @brief Points to a queue of free thread_block instances/element in the threads array.
thread freeQ 	= threads;
// @brief Points to a queue of thread_block instances in the threads array that are ready to execute.
thread readyQ	= NULL;
// @brief Points to a queue of thread_block instances in the threads array that have finished execution
thread doneQ	= NULL;

thread current	= &initp;

int initialized = 0;

/** @brief Initializes each thread in the threads array.
 * For each thread in the threads array, a unique identifier is assigned
 * along with the task information. 
 */
void initialize(void) {
	initp.idx = -1;
	initp.function = NULL;
	initp.arg = -1;
	initp.next = NULL;
	initp.Period_Deadline = INT_MAX;
	initp.Rel_Period_Deadline = INT_MAX;	
	
	for (int i=0; i < NTHREADS; i++)
	{
		threads[i].idx = i;
		threads[i].function = NULL;
		threads[i].arg = -1;
		threads[i].next = &threads[i+1];
		threads[i].Period_Deadline = INT_MAX;
		threads[i].Rel_Period_Deadline = INT_MAX;
	}
	threads[NTHREADS-1].next = NULL;
	initialized = 1;
}

/** @brief Adds an element to the tail of the queue  
 * @note In Assignment 4, parts 2 and 3, you might want to change this
 * implementation to enqueue with insertion sort.
 */
static void enqueue(thread p, thread *queue) {
	p->next = NULL;
	if (*queue == NULL) {
		*queue = p;
	} else {
		thread q = *queue;
		while (q->next)
			q = q->next;
		q->next = p;
	}
}

/** @brief Remove an element from the head of the queue
 */
static thread dequeue(thread *queue) {
	thread p = *queue;
	if (*queue) {
		*queue = (*queue)->next;
	} else {
		// PUTTOLDC("%s", "Empty queue!!!");
		// Empty queue, kernel panic!!!
		// while (1) ;  // not much else to do...
		return NULL;
	}
	return p;
}

/** @brief Starts or resumes the execution of the thread
 * select to execute.
 */
static void dispatch(thread next) {
	if (next != NULL) {
		if (setjmp(current->context) == 0) {
			current = next;
			longjmp(next->context, 1);
		}       
	}	
}


/** @brief Creates an thread block instance and assign to it an start routine, 
 * i.e., the procedure that the thread will execute.
 * @param function is a pointer to the start routine
 * @param int arg is the parameter to the start routine
 */
void spawn(void (* function)(int), int arg) {
	thread newp;
	DISABLE();
	if (!initialized) 
		initialize();
	newp = dequeue(&freeQ);
	newp->function = function;
	newp->arg = arg;
	newp->next = NULL;
	if (setjmp(newp->context) == 1) {
		ENABLE();
		current->function(current->arg);
		DISABLE();
		enqueue(current, &doneQ);
		current = NULL;
		dispatch(dequeue(&readyQ));	
	}
	SETSTACK(&newp->context, &newp->stack);
	enqueue(newp, &readyQ);
	ENABLE();
}

/** @brief Preempts the execution of the current thread and a new 
 * thread gets to run.
 */
void yield(void) {
	if (readyQ==NULL){
		
	}
	DISABLE();
	if (readyQ != NULL){		
		thread p = dequeue(&readyQ);
		if(p != NULL){
			enqueue(current, &doneQ);
			dispatch(p);
		}
	}
	ENABLE();
}

/** @brief Sets the locked flag of the mutex if it was previously unlocked,
 * otherwise, the running thread shall be placed in the waiting queue of the
 * mutex and a new thread should be dispatched from the ready queue. 
 */
void lock(mutex *m) {
	DISABLE();
	if (m->locked == 0){
		m->locked = 1;
	} else {
		enqueue(current, &m->waitQ);
		dispatch(dequeue(&readyQ));
	}
	ENABLE();
}

/** @brief Activate a thread in the waiting queue of the mutex if it is
 * non-empty, otherwise, the locked flag shall be reset.
 */
void unlock(mutex *m) {
	DISABLE();
	if (m->waitQ != NULL || m->waitQ != 0) {
		enqueue(current, &readyQ);
		dispatch(dequeue(&m->waitQ));
	} else {
		m->locked = 0;
	}
	ENABLE();

}

/** @brief Creates an thread block instance and assign to it an start routine, 
 * i.e., the procedure that the thread will execute.
 * @param function is a pointer to the start routine
 * @param int arg is the parameter to the start routine
 */
void spawnWithDeadline(void (* function)(int), int arg, unsigned int deadline, unsigned int rel_deadline) {
	thread newp;
	DISABLE();
	if (!initialized) 
		initialize();
	newp = dequeue(&freeQ);
	newp->function = function;
	newp->arg = arg;
	newp->next = NULL;
	newp->Period_Deadline = deadline;
	newp->Rel_Period_Deadline = rel_deadline;
	if (setjmp(newp->context) == 1) {
		ENABLE();
		current->function(current->arg);
		DISABLE();
		// enqueue the current, done thread into the doneQ
		enqueue(current, &doneQ);
		// current = NULL;
		// dispatch(current);
		dispatch(dequeue(&readyQ));
			
	}
	SETSTACK(&newp->context, &newp->stack);
	enqueue(newp, &readyQ);
	ENABLE();
}

/** @brief Removes a specific element from the queue.
 */
static thread dequeueItem(thread *queue, int idx) {
	thread p;
	thread temp;
	int counter = 0;
	// if statement to check if there is something in the queue
	if (*queue) {
		// if the chosen index was zero, the same procedure as in dequeue() is done
		if (idx == 0){
			temp = *queue;
			*queue = (*queue)->next;
			return temp;
		// if the chosen index was not zero
		} else {
			p = *queue;
			// traverse through all threads in the queue, until reaching the one chosen through the index
			while(p->next != NULL && counter < idx-1){
				p = p->next;
				counter++;
			}
			// if the chosen thread was found, store it in temp & return that, also skip it in the queue
			if (p->next != NULL && counter == idx -1){
				temp = p->next;
				p->next = p->next->next;
				return temp;
			}
		}
	}
	return NULL;
}

/** @brief Sort the elements a given queue container by a given 
 * field or attribute.
 * https://arxiv.org/abs/2110.01111
 */
/*static void sortX(thread *queue) {
	DISABLE();
	int n = 0, highest_prio_idx = 0;
	// first, calculate the amount of threads inside the queue
	for (thread t = *queue; t != NULL; t = t->next){
		n++;
	}
	// if there are threads in the queue, enter this loop
	while (n != 0){
		int idx = 0;
		thread highest_prio = NULL;
		// go through all the threads in the queue
		for (thread tt = *queue; idx < n; tt = tt->next ){
			// if the highest priority thread hasnt been set yet, set it now
			if (highest_prio == NULL){
				// set the thread, and its index
				highest_prio = tt; 
				highest_prio_idx = idx;
			} else {
				//otherwise, compare the priorities of the saved thread with the current one. The higher priority one gets saved
				if (highest_prio->Period_Deadline > tt->Period_Deadline) {
					highest_prio = tt;
					highest_prio_idx = idx;
				}
			}
			// increase the index to check each thread
			idx++;
		}
		// if the thread with the highest priority was found, take it out of the readyQ, and place it back into its end.
		enqueue(dequeueItem(queue, highest_prio_idx), &readyQ);
		// decrease the n value and go around the loop again
		n--;
	}
	ENABLE();
}*/
static void sortX(thread *queue) {
    DISABLE();
    if (*queue == NULL) return;

    thread sorted = NULL; // Initialize an empty sorted queue
    thread current = *queue;

    // Traverse through the original queue and insert each thread into the sorted queue
    while (current != NULL) {
        thread next = current->next; // Save next thread to process
        if (sorted == NULL || sorted->Period_Deadline > current->Period_Deadline) {
            // Insert at the beginning if it's the first element or has the highest priority
            current->next = sorted;
            sorted = current;
        } else {
            // Find the correct position to insert the current thread in the sorted queue
            thread temp = sorted;
            while (temp->next != NULL && temp->next->Period_Deadline <= current->Period_Deadline) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
        // Move to the next thread in the original queue
        current = next;
    }

    *queue = sorted; // Set the original queue to be the sorted queue
    ENABLE();
}


/** @brief Periodic tasks have to be activated at a given frequency. Their activations are generated by timers .
 */
void respawn_periodic_tasks(void) {
	DISABLE();
	thread curr = doneQ;
	int idx = 0;
	// if there are elements in the doneQ
	while (curr != NULL){
		// check if its time to respawn any of the threads
		if (ticks % curr->Period_Deadline == 0){
			// if yes, the thread gets respawned (placed into the readyQ)
			spawnWithDeadline(curr->function, curr->arg, curr->Period_Deadline, curr->Period_Deadline);
			// and it gets deleted from the doneQ
			thread del = dequeueItem(&doneQ, idx);
		} 
		// then, the next thread inside the doneQ is checked
		curr = curr->next;
		// and the index is increased
		idx++;
	}
	ENABLE();
}

/** @brief Schedules tasks using time slicing
 */
static void scheduler_RR(void){
	yield();
}

/** @brief Schedules periodic tasks using Rate Monotonic (RM) 
 */
static void scheduler_RM(void){
	sortX(&readyQ);
	yield();
}

/** @brief Schedules periodic tasks using Earliest Deadline First  (EDF) 
 */
static void scheduler_EDF(void){
	// To be implemented in Assignment 4!!!
}

/** @brief Calls the actual scheduling mechanisms, i.e., Round Robin,
 * Rate monotonic, or Earliest Deadline First.
 * When dealing with periodic tasks with fixed execution time,
 * it will first call the method that re-spawns period tasks.
 */
void scheduler(void){
	// To be implemented in Assignment 4!!!
	respawn_periodic_tasks();
	scheduler_RM();
}

/** @brief Prints via UART the content of the main variables in TinyThreads
 */
void printTinyThreadsUART(void) {	
	thread t;
	t = threads;
	print2uart("\nThreads\n");
	for (int i=0; i<NTHREADS; i++)
		print2uart("t[%i] @%#010x arg: %d idx: %d dl: %d\n", i, &t[i], t[i].arg, t[i].idx, t[i].Period_Deadline);		
	
	print2uart("Current\n");
	print2uart("t[%i] @%#010x arg: %d dl: %d\n", current->idx, &current, current->arg, current->Period_Deadline);		

	print2uart("freeQ\n");
	t=freeQ;
	while(t)
	{
		print2uart("t[%i] @%#010x arg: %d dl: %d\n", t->idx, t, t->arg, t->Period_Deadline);		
		t = t->next;
	}

	print2uart("readyQ\n");
	t=readyQ;
	while(t)
	{
		print2uart("t[%i] @%#010x arg: %d dl: %d\n", t->idx, t, t->arg, t->Period_Deadline);		
		t = t->next;
	}
	print2uart("doneQ\n");
	t=doneQ;
	while(t)
	{
		print2uart("t[%i] @%#010x arg: %d dl: %d\n", t->idx, t, t->arg, t->Period_Deadline);		
		t = t->next;
	}	
}

/** @brief Prints on the PiFace the content of the main variables in TinyThreads
 */
void printTinyThreadsPiface(void) {	
	thread t;

	piface_clear();
	piface_puts("t for thread");
	RPI_WaitMicroSeconds(2000000);

	t = threads;
	piface_clear();
	piface_puts("Threads");
	RPI_WaitMicroSeconds(2000000);
	for (int i=0; i<NTHREADS; i++)
	{
		piface_clear();
		PUTTOLDC("t[%i] @%#010x (%d)", i, &t[i], t[i].arg);		
		RPI_WaitMicroSeconds(2000000);
	}
	
	piface_clear();
	piface_puts("Current");
	RPI_WaitMicroSeconds(2000000);
	piface_clear();
	PUTTOLDC("t[%i] @%#010x (%d)", current->idx, &current, current->arg);		
	RPI_WaitMicroSeconds(2000000);

	piface_clear();
	t = freeQ;
	piface_puts("freeQ");
	RPI_WaitMicroSeconds(2000000);
	while(t)
	{
		piface_clear();
		PUTTOLDC("t[%i] @%#010x (%d)", t->idx, t, t->arg);		
		RPI_WaitMicroSeconds(2000000);
		t = t->next;
	}
	
	piface_clear();
	t = readyQ;
	piface_puts("readyQ");
	RPI_WaitMicroSeconds(2000000);
	while(t)
	{
		piface_clear();
		PUTTOLDC("t[%i] @%#010x (%d)", t->idx, t, t->arg);		
		RPI_WaitMicroSeconds(2000000);
		t = t->next;
	}
	
	piface_clear();
	t = doneQ;
	piface_puts("doneQ");
	RPI_WaitMicroSeconds(2000000);
	while(t)
	{
		piface_clear();
		PUTTOLDC("t[%i] @%#010x (%d)", t->idx, t, t->arg);		
		RPI_WaitMicroSeconds(2000000);
		t = t->next;
	}
	piface_clear();
}
