//
//  main.cpp
//  gorkem_guzeler_hw3
//
//  Created by gorkemg on 4.12.2020.
//
#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <stdlib.h>
#include <queue>
#include <semaphore.h>
#include <random>
#include <stdio.h>
using namespace std;

#define NUM_THREADS 10
#define MEMORY_SIZE 150

struct node
{
    int id;
    int size;
    
};

queue<node> myqueue; // shared que
pthread_mutex_t sharedLock = PTHREAD_MUTEX_INITIALIZER; // mutex
pthread_t server; // server thread handle
sem_t semlist[NUM_THREADS]; // thread semaphores

int thread_message[NUM_THREADS]; // thread memory information
char  memory[MEMORY_SIZE]; // memory size
int done = 0;
int starting_point = 0;

void my_malloc(int thread_id, int size)
{
    node new_request;
    new_request.id =thread_id;
    new_request.size =size;
    
    myqueue.push(new_request);
}

//server thread will update thread_message array and unblock the proper threads semaphore.
void * server_function(void *)
{
    int index = 0;
    
    while(true){
       
        if(!myqueue.empty()){
        
            node * request = &myqueue.front();
            
            myqueue.pop();
            
            if(request->size <= (MEMORY_SIZE - index)){
                thread_message[request->id] = index;
                index = index + request->size;
            }
            else{ //decline
                thread_message[request->id] = -1;
            }
            sem_post(&semlist[request->id]);
        }
    }
    
    return 0;
}

//This function will create a random size, and call my_malloc, Block, Then fill the memory with id's or give an error prompt
void * thread_function(void * id)
{
    pthread_mutex_lock(&sharedLock);
    
    int* p = (int*)id;
    
    int random_size;
    random_size = (1 + (rand()%MEMORY_SIZE/6) );

    my_malloc(*p, random_size);
    
    //block until the memory request is handled by the server thread
    sem_wait(&semlist[*p]);
    
    char m = '0' + *p;
    
    if(thread_message[*p] > -1){
        for (int i = 0; i < random_size; i++){
            memory[starting_point +i] = m ; //as the starting point in the memory.
        }
        starting_point = starting_point + random_size;
    }
    else if(thread_message[*p] == -1){
        printf("thread[%d]: Not enough memory\n", * p);
    }
    
    pthread_mutex_unlock(&sharedLock);

    return 0;
}

void init()
{
    pthread_mutex_lock(&sharedLock);    //lock
    for(int i = 0; i < NUM_THREADS; i++) //initialize semaphores
    {sem_init(&semlist[i],0,0);}
    for (int i = 0; i < MEMORY_SIZE; i++)    //initialize memory
        {char zero = '0'; memory[i] = zero;}
    pthread_create(&server,NULL,server_function,NULL); //start server
    pthread_mutex_unlock(&sharedLock); //unlock
}

void dump_memory()
{
 // You need to print the whole memory array here.
    cout << "MEMORY ARRAY: " << endl;
    for(int i = 0; i < MEMORY_SIZE ;i++){
        cout << memory[i];
    }
}

int main (int argc, char *argv[])
 {
     //You need to create a thread ID array here

     init();    // call init
     
     pthread_t threads[NUM_THREADS];
     int id_array[NUM_THREADS];
     
     for (int i=0; i < NUM_THREADS; i++){
        id_array[i] = i;
     }
     
     //You need to create threads with using thread ID array, using pthread_create()
     for (int i=0; i < NUM_THREADS; i++){
         pthread_create(&threads[i] , NULL, thread_function, &id_array[i]);
     }
     
     //You need to join the threads
     for (int i=0; i < NUM_THREADS; i++){
         pthread_join(threads[i], NULL);
     }

     dump_memory(); // this will print out the memory

     printf("\nMemory Indexes:\n" );
     for (int i = 0; i < NUM_THREADS; i++)
     {
         printf("[%d]" ,thread_message[i]); // this will print out the memory indexes
     }
     printf("\nTerminating...\n");
     
 }

