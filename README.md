# 2402mc11_Assign-5-
OS Lab Assignment 5 – Process Synchronization using xv6

Q1. Peterson's Algorithm

Peterson's Algorithm is implemented to provide mutual exclusion between two processes. A shared memory page is used so that the parent and child processes can access the same synchronization variables and shared counter. The shared memory contains two flags, a turn variable, and a shared counter.

Each process sets its flag before trying to enter the critical section and gives the other process the turn. It then waits while the other process also wants to enter and has the current turn. This ensures that both processes cannot enter the critical section at the same time. After incrementing the shared counter, the process clears its flag and leaves the critical section.

Each process performs 10 iterations, so the expected final value of the shared counter is 20. The program successfully produces a final counter value of 20, demonstrating that mutual exclusion is working correctly and no increments are lost.
