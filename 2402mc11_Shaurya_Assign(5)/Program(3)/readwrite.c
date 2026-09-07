#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define R_MUTEX   3
#define RESOURCE  4
#define TURNSTILE 5


struct shared_rw {
  int shared_data;
  int read_count;
};

void
reader(struct shared_rw *shared)
{
  int i;
  int active;

  for(i = 0; i < 3; i++){

    // Reader enters through turnstile
    sem_wait(TURNSTILE);
    sem_signal(TURNSTILE);

    // Protect read_count
    sem_wait(R_MUTEX);

    shared->read_count++;
    active = shared->read_count;

    // First reader locks the resource
    if(shared->read_count == 1)
      sem_wait(RESOURCE);

    printf("[Reader] PID=%d Time=%d ENTER  shared_data=%d ActiveReaders=%d\n",
           getpid(), uptime(), shared->shared_data, active);

    sem_signal(R_MUTEX);
     
    for(volatile int delay = 0; delay < 1000000; delay++);

    // Leave reader section
    sem_wait(R_MUTEX);

    shared->read_count--;
    active = shared->read_count;

    printf("[Reader] PID=%d Time=%d EXIT   ActiveReaders=%d\n",
           getpid(), uptime(), active);

    // Last reader releases resource
    if(shared->read_count == 0)
      sem_signal(RESOURCE);

    sem_signal(R_MUTEX);

    for(volatile int delay = 0; delay < 1000000; delay++);
  }

  exit(0);
}

void
writer(struct shared_rw *shared)
{
  int i;

  for(i = 0; i < 3; i++){

    // Writer blocks new readers
    sem_wait(TURNSTILE);

    // Get exclusive access
    sem_wait(RESOURCE);

    printf("[Writer] PID=%d Time=%d ENTER  shared_data=%d\n",
           getpid(), uptime(), shared->shared_data);

    for(volatile int delay = 0; delay < 1000000; delay++);

    shared->shared_data++;

    printf("[Writer] PID=%d Time=%d WRITE  shared_data=%d\n",
           getpid(), uptime(), shared->shared_data);

    sem_signal(RESOURCE);

    printf("[Writer] PID=%d Time=%d EXIT\n",
           getpid(), uptime());

    sem_signal(TURNSTILE);

    for(volatile int delay = 0; delay < 1000000; delay++);
  }

  exit(0);
}

int
main(void)
{
  struct shared_rw *shared;
  int i;
  int pid;

  shared = (struct shared_rw *)shm_get();

  if(shared == 0){
    printf("shm_get failed\n");
    exit(1);
  }

  // Initialize shared data
  shared->shared_data = 0;
  shared->read_count = 0;

  // Initialize semaphores
  sem_init(R_MUTEX, 1);
  sem_init(RESOURCE, 1);
  sem_init(TURNSTILE, 1);

  // Create 3 readers
  for(i = 0; i < 3; i++){
    pid = fork();

    if(pid < 0){
      printf("fork failed\n");
      exit(1);
    }

    if(pid == 0){
      reader(shared);
    }
  }

  // Create 2 writers
  for(i = 0; i < 2; i++){
    pid = fork();

    if(pid < 0){
      printf("fork failed\n");
      exit(1);
    }

    if(pid == 0){
      writer(shared);
    }
  }

  // Wait for all 5 children
  for(i = 0; i < 5; i++)
    wait(0);

  printf("\nFinal shared_data = %d\n", shared->shared_data);
  printf("Readers-Writers completed\n");

  exit(0);
}