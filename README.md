# 2402mc11_Assign-5-
OS Lab Assignment 5 – Process Synchronization using xv6

Q1. Peterson's Algorithm

Peterson's Algorithm is implemented to provide mutual exclusion between two processes. A shared memory page is used so that the parent and child processes can access the same synchronization variables and shared counter. The shared memory contains two flags, a turn variable, and a shared counter.

Each process sets its flag before trying to enter the critical section and gives the other process the turn. It then waits while the other process also wants to enter and has the current turn. This ensures that both processes cannot enter the critical section at the same time. After incrementing the shared counter, the process clears its flag and leaves the critical section.

Each process performs 10 iterations, so the expected final value of the shared counter is 20. The program successfully produces a final counter value of 20, demonstrating that mutual exclusion is working correctly and no increments are lost.

Q2.Producer–Consumer Problem

The Producer–Consumer problem is implemented using a bounded circular buffer of size 5. A producer process generates items from 1 to 20, while a consumer process removes the items from the buffer. Since both processes access the same buffer, synchronization is required to prevent race conditions and incorrect buffer operations.

Three semaphores are used for synchronization. EMPTY keeps track of the number of empty spaces in the buffer and is initially set to 5. FULL keeps track of the number of items available for consumption and is initially set to 0. MUTEX provides mutual exclusion so that only one process can modify the buffer and its indices at a time.

Before inserting an item, the producer waits for an empty buffer slot and then obtains the mutex. After inserting the item, it releases the mutex and signals that a new item is available. Similarly, the consumer waits for an available item, obtains the mutex, removes the item, and then signals that a buffer slot has become free.

The semaphore implementation uses the xv6 sleep_prepare(), sleep(), and wakeup() mechanisms. The producer successfully generates all 20 items and the consumer receives them in the correct order from 1 to 20. This shows that the bounded buffer is working correctly without losing, duplicating, or incorrectly ordering items.

Q3.Readers–Writers Problem

The Readers–Writers problem is implemented using shared memory and semaphores. The program creates three reader processes and two writer processes. The main objective is to allow multiple readers to access the shared data simultaneously while ensuring that a writer has exclusive access when modifying the data.

A read_count variable is used to keep track of the number of readers currently accessing the shared data. R_MUTEX protects changes to read_count, while RESOURCE controls access to the actual shared data. The first reader acquires the resource and the last reader releases it. This allows multiple readers to read at the same time. Each of the two writers performs three increments. Therefore, the expected final value of shared_data is 6. The output also shows cases where more than one reader is active at the same time, demonstrating concurrent reader access. The program successfully reaches Final shared_data = 6, confirming that the synchronization works correctly.
