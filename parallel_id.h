/**
* Written by Collin Meeker
*/

#ifndef EMPLOYEE_GENERATOR_H
#define EMPLOYEE_GENERATOR_H

#include <pthread.h>



void setThreads(int threads);

// Function executed by each thread to generate a random 9-digit employee ID
void *generate_employee_id(void *arg);

// Function to generate employee IDs using multiple threads
int generate_employee_ids_multithreaded();

#endif // EMPLOYEE_GENERATOR_H

