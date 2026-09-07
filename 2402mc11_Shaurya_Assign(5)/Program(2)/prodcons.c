#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define BUFFER_SIZE 5

#define EMPTY  0
#define FULL   1
#define MUTEX  2

struct shared_data {
  int buffer[BUFFER_SIZE];
  int in;
  int out;
};

int
main(void)
{
  struct shared_data *shared;
  int pid;
  int i;

  // Get shared memory
  shared = (struct shared_data *)shm_get();

  if(shared == 0){
    printf("shm_get failed\n");
    exit(1);
  }

  // Initialize circular buffer
  shared->in = 0;
  shared->out = 0;

  // Initialize semaphores
  sem_init(EMPTY, BUFFER_SIZE);
  sem_init(FULL, 0);
  sem_init(MUTEX, 1);

  pid = fork();

  if(pid < 0){
    printf("fork failed\n");
    exit(1);
  }

  if(pid == 0){
    // =========================
    // CONSUMER
    // =========================

    for(i = 1; i <= 20; i++){

      sem_wait(FULL);
      sem_wait(MUTEX);

      int item = shared->buffer[shared->out];

      shared->out =
        (shared->out + 1) % BUFFER_SIZE;

      printf("Consumer: %d\n", item);

      sem_signal(MUTEX);
      sem_signal(EMPTY);
    }

    exit(0);

  } else {
    // =========================
    // PRODUCER
    // =========================

    for(i = 1; i <= 20; i++){

      sem_wait(EMPTY);
      sem_wait(MUTEX);

      shared->buffer[shared->in] = i;

      shared->in =
        (shared->in + 1) % BUFFER_SIZE;

      printf("Producer: %d\n", i);

      sem_signal(MUTEX);
      sem_signal(FULL);
    }

    wait(0);

    printf("Producer-Consumer completed\n");

    exit(0);
  }
}