#include <iostream>
#include <pthread.h>
#include <random>
#include <ctime>
#include "parallel_id.h"
#include <mutex>
#include <vector>

/**
* Written by Collin Meeker
*/

int NUM_THREADS;
std::vector<int> employee_ids; // Global vector to store the employee IDs
std::mutex mutex;

void setThreads(int threads_parameter) {
    NUM_THREADS = threads_parameter;
    employee_ids.resize(NUM_THREADS); // Resize the vector to the number of threads
}

// Function executed by each thread to generate a random 9-digit employee ID
void* generate_employee_id(void* arg) {
    int thread_id = *((int*)arg);
    
    // Seed for the random number generator
    std::mt19937 rng(std::random_device{}() + thread_id);
    
    // Distribution for generating 9-digit numbers
    std::uniform_int_distribution<int> distribution(100000000, 999999999);
    
    // Generate the employee ID
    int id = distribution(rng);
    
    // Lock the mutex before accessing the global vector
    mutex.lock();
    employee_ids[thread_id] = id; // Store the generated ID in the vector
    mutex.unlock();
    
    pthread_exit(nullptr);
}

// Function to generate employee IDs using multiple threads
int generate_employee_ids_multithreaded() {
    pthread_t* threads = new pthread_t[NUM_THREADS];
    int* thread_ids = new int[NUM_THREADS];
    
    // Create threads to generate employee IDs
    for (int i = 0; i < NUM_THREADS; ++i) {
        thread_ids[i] = i;
        if (pthread_create(&threads[i], nullptr, generate_employee_id, &thread_ids[i]) != 0) {
            std::cerr << "Error creating thread\n";
            return 1;
        }
    }
    
    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; ++i) {
        if (pthread_join(threads[i], nullptr) != 0) {
            std::cerr << "Error joining thread\n";
            return 1;
        }
    }
    
    // Calculate the average employee ID
    int total_ids = 0;
    for (int id : employee_ids) { // Use range-based for loop to calculate total
        total_ids += id;
    }
    
    delete[] threads;    // Clean up dynamic memory
    delete[] thread_ids; // Clean up dynamic memory
    
    return total_ids / NUM_THREADS;
}

