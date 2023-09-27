#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>

/*
    RTOS Scheduler Implementation - Embedded Systems Project

    This is a small C++ Real-Time Operating System (RTOS) scheduler for embedded systems. 
    The scheduler manages tasks with varying priorities and executes them in 
    order of priority. It serves as a foundational example and can be extended to create a more 
    comprehensive RTOS scheduler tailored to specific embedded system requirements.

    Author: James Covert
    Date: January 2021
*/

// Define a Task structure to represent individual tasks
struct Task {
    int id;
    std::function<void()> function;
    int priority;
};

// Define a mutex for task synchronization
std::mutex taskMutex;

// Vector to store the tasks to be scheduled
std::vector<Task> taskQueue;

// Function to add a task to the task queue
void addTask(int id, std::function<void()> func, int priority) {
    Task task;
    task.id = id;
    task.function = func;
    task.priority = priority;

    // Lock the mutex before modifying the task queue
    std::lock_guard<std::mutex> lock(taskMutex);
    
    // Add the task to the queue
    taskQueue.push_back(task);
    
    // Sort the task queue based on priority (you can implement a custom sorting function)
    // This ensures higher priority tasks get executed first
    std::sort(taskQueue.begin(), taskQueue.end(), [](const Task& a, const Task& b) {
        return a.priority > b.priority;
    });
}

// Function to execute tasks from the task queue
void executeTasks() {
    while (true) {
        Task task;
        
        // Lock the mutex before accessing the task queue
        std::lock_guard<std::mutex> lock(taskMutex);
        
        // Check if there are tasks in the queue
        if (!taskQueue.empty()) {
            // Get the highest priority task
            task = taskQueue.back();
            taskQueue.pop_back();
        } else {
            // No tasks in the queue, wait for a while (you can implement a sleep mechanism)
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }
        
        // Execute the task
        task.function();
    }
}

int main() {
    // Create a thread for task execution
    std::thread taskExecutor(executeTasks);

    // Add tasks to the scheduler
    addTask(1, []() {
        std::cout << "Task 1 executed with priority 1" << std::endl;
    }, 1);

    addTask(2, []() {
        std::cout << "Task 2 executed with priority 2" << std::endl;
    }, 2);

    addTask(3, []() {
        std::cout << "Task 3 executed with priority 1" << std::endl;
    }, 1);

    // Wait for the task executor thread to finish (you can implement graceful shutdown)
    taskExecutor.join();

    return 0;
}
