#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <stdlib.h> 
#include <queue> 
#include <semaphore.h>
#include "linkedlist.h"
#include "Queue.h"

using namespace std;

#define NUM_THREADS 5
#define MEMORY_SIZE M

struct lol
{
	int id;
	int size;
};


queue<lol> myqueue;
pthread_mutex_t sharedLock = PTHREAD_MUTEX_INITIALIZER; // mutex
pthread_t server; // server thread handle
sem_t semlist[NUM_THREADS]; // thread semaphores
LinkedList memlist;

int thread_message[NUM_THREADS]; // thread memory information
char  memory[MEMORY_SIZE]; // memory size

void release_function();
void my_malloc(int thread_id, int size);
void * server_function(void *);
void * thread_function(void * id);
void use_mem(int thread_id);
void free_mem();
void dump_memory();
void free_mem(int id);


void release_function() {
	pthread_exit(&thread1);
	pthread_exit(&thread3);
	pthread_exit(&thread4);
	pthread_exit(&thread5);

	for (int i = 0; i < myqueue.size(); i++)
	{
		myqueue.pop();
	}

	memlist.deleteList();
}

void my_malloc(int thread_id, int size) {
	sem_wait(&semlist[thread_id]);


	node * ptr;
	ptr = memlist.head();
	bool free;

	while (ptr->next != NULL) {
		if (ptr->size != size) {
			free = true;
		}
		else
		{
			free = false;
		}
	}

	if (free) {
		lol *node;
		node->id = thread_id;
		node->size = size;

		myqueue.push(node);

	}
	sem_post(&semlist[thread_id]);

	use_mem();
	free_mem(thread_id);

}


void use_mem() {
	int randtime = rand() % ((5 - 1) + 1) + 1;
	sleep(randtime);
}

void free_mem(int id) {

	int memsize;
	node * ptr = memlist.head();
	while (ptr->next != NULL) {
		if (ptr->id == id) {
			memsize = ptr->size;
			delete ptr;
		}
		ptr = ptr->next;
	}
	lol * node;
	node->id = id;
	node->size = memsize;
	myqueue.push(node);
	release_function();
}

void * thread_function(int * id)
{

	//This function will create a random size, and call my_malloc
	int size = (rand() % (1000 / 3)) + 1;
	my_malloc(id, size);

	//Block
	sem_wait(semlist[id]);
	//Then fill the memory with 1's or give an error prompt
	server_function();

	sem_post(semlist[id]);
}

void server_function(void *) {


	int index = 0;
	int count = 0;

	while (count != MEMORY_SIZE)
	{
		if (memory[count] == 1)
		{
			index++;
		}
		count++;
	}

	lol *current = myqueue.pop();
	int request_size = current->size;
	if (request_size < (MEMORY_SIZE - index))
	{
		thread_message[current->idid] = index;
		for (int i = 0; i < request_size; i++)
		{
			memory[i] = 1;
		}

		node * listnode = new node();
		listnode->id = current->id;
		listnode->size = current->size;
		listnode->next = NULL;

	}
	else
	{
		thread_message[current->id] = -1;
		cout << "Thread " << current->id << ": Not enough memory" << endl;
	}

}




void init()
{
	pthread_mutex_lock(&sharedLock);	//lock
	for (int i = 0; i < NUM_THREADS; i++) //initialize semaphores
	{
		sem_init(&semlist[i], 0, 0);
	}
	for (int i = 0; i < MEMORY_SIZE; i++)	//initialize memory 
	{
		char empty = 'X'; memory[i] = empty;
	}
	pthread_create(&server, NULL, server_function, NULL); //start server 
	pthread_mutex_unlock(&sharedLock); //unlock
}


void dump_memory() {

	memlist.printList();

	cout << "Memory Dump: " << endl;
	for (int i = 0; i < MEMORY_SIZE; i++)
	{
		cout << memory[i];
	}

}

int main(int argc, char *argv[])
{
	//You need to create a thread ID array here
	int thread_ids[NUM_THREADS];
	for (int i = 0; i < NUM_THREADS; i++)
	{
		thread_ids[i] = i;
	}

	init();	// call init
		//You need to create threads with using thread ID array, using pthread_create()
	pthread_t thread1, thread2, thread3, thread4, thread5;

	pthread_create(&thread1, NULL, thread_function, (void*)thread_ids[0]);
	pthread_create(&thread2, NULL, thread_function, (void*)thread_ids[1]);
	pthread_create(&thread3, NULL, thread_function, (void*)thread_ids[2]);
	pthread_create(&thread4, NULL, thread_function, (void*)thread_ids[3]);
	pthread_create(&thread5, NULL, thread_function, (void*)thread_ids[4]);

	//You need to join the threads
	pthread_join(thread1, NULL);
	pthread_join(thread3, NULL);
	pthread_join(thread3, NULL);
	pthread_join(thread4, NULL);
	pthread_join(thread5, NULL);

	dump_memory(); // this will print out the memory
	printf("\nMemory Indexes:\n");
	for (int i = 0; i < NUM_THREADS; i++)
	{
		printf("[%d]", thread_message[i]); // this will print out the memory indexes
	}
	printf("\nTerminating...\n");
	release_function();
	return 0;
}

//remember to call release_function, free_mem & use_mem