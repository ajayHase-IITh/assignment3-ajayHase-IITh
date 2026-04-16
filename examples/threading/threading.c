#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Optional: use these functions to add debug or error prints to your application
//#define DEBUG_LOG(msg,...)
#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{

    // TODO: wait, obtain mutex, wait, release mutex as described by thread_data structure
    // hint: use a cast like the one below to obtain thread arguments from your parameter
    //struct thread_data* thread_func_args = (struct thread_data *) thread_param;
    	
	struct thread_data* thread_data_struct = (struct thread_data *) thread_param;
	if(thread_data_struct == NULL)
	{
		DEBUG_LOG("Null input param");
		return thread_param;
	}
	thread_data_struct->thread_complete_success = false;

       	usleep(thread_data_struct->wait_to_obtain_ms);
	
	// Get mutex lock failed
       	if(pthread_mutex_lock(thread_data_struct->mutex) != 0){
		DEBUG_LOG("Failed to lock mutex ");
		return thread_data_struct;
	}
       	
	// Mutex lock successful
	usleep(thread_data_struct->wait_to_release_ms);
	
	// Release mutex lock
	if(pthread_mutex_unlock(thread_data_struct->mutex) != 0){
		DEBUG_LOG("Failed to unlock mutex");
		return thread_data_struct;
	}
	
	DEBUG_LOG("Thread created successfully..!!");
	thread_data_struct->thread_complete_success = true;

	return thread_data_struct;
}


bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,int wait_to_obtain_ms, int wait_to_release_ms)
{
    /**
     * TODO: allocate memory for thread_data, setup mutex and wait arguments, pass thread_data to created thread
     * using threadfunc() as entry point.
     *
     * return true if successful.
     *
     * See implementation details in threading.h file comment block
     */
    	// null check
	if((thread == NULL)||(mutex == NULL)){
		DEBUG_LOG("Null input param");
		return false;
	}

	// get param
	struct thread_data *thread_data_s = malloc(sizeof(struct thread_data));
	
	if(thread_data_s == NULL){
		DEBUG_LOG("Failed to allocate memory");
		return false;
	}

	thread_data_s->mutex = mutex;
	thread_data_s->wait_to_obtain_ms = wait_to_obtain_ms;
	thread_data_s->wait_to_release_ms = wait_to_release_ms;
	thread_data_s->thread_complete_success = false;

	// create thread
	int rc = pthread_create(thread, NULL, threadfunc, thread_data_s);

	if(rc != 0){
		DEBUG_LOG("Failed to create thread");
		free(thread_data_s);
		return false;
	}

	return true;
}

