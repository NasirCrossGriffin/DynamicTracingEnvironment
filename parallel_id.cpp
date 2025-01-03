#include <iostream>
#include <pthread.h>
#include <random>
#include <ctime>
#include "parallel_id.h"
#include <mutex>

// NUMTHREADS IS EQUAL TO 500


// Global array to store the employee IDs generated by each thread
int employee_ids[NUM_THREADS];

// Mutex to ensure thread safety
std::mutex mutex1;

// Function executed by each thread to generate a random 9-digit employee ID
void *generate_employee_id(void *arg) {
    int thread_id = *((int*) arg);
    
    // Seed for the random number generator
    std::mt19937 rng(std::random_device{}() + thread_id);
    
    // Distribution for generating 9-digit numbers
    std::uniform_int_distribution<int> distribution(100000000, 999999999);
    
    // Generate the employee ID
    int id = distribution(rng);
    
    // Lock the mutex before accessing the global array
    mutex1.lock();
    
    // Store the generated employee ID in the array
    employee_ids[thread_id] = id;
    
    // Unlock the mutex
    mutex1.unlock();
    
    // Exit the thread
    pthread_exit(nullptr);
}

// Function to generate employee IDs using multiple threads
int generate_employee_ids_multithreaded() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    
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
    for (int i = 0; i < NUM_THREADS; ++i) {
        total_ids += employee_ids[i];
    }
    return total_ids / NUM_THREADS;
}

