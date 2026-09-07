# 2402mc11_Assign-5-
OS Lab Assignment 5 – Process Synchronization using xv6

Q1. Peterson's Algorithm

Peterson's Algorithm is implemented to provide mutual exclusion between two processes. A shared memory page is used so that the parent and child processes can access the same synchronization variables and shared counter. The shared memory contains two flags, a turn variable, and a shared counter.
