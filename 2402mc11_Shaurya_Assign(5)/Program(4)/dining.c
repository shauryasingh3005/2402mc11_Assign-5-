#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define NPHIL 5
#define NFORK 5

#define FORK0 0
#define FORK1 1
#define FORK2 2
#define FORK3 3
#define FORK4 4

// Semaphore 5 is used only to protect console output.
#define PRINT_LOCK 5

int forks[NFORK] = {FORK0, FORK1, FORK2, FORK3, FORK4};

#define DEFAULT_CYCLES 5

// Simple busy-wait delay.
// This gives the scheduler a chance to switch
// between philosopher processes.
void
delay(void)
{
  for (volatile int i = 0; i < 1000000; i++)
    ;
}

// Print one complete line without allowing
// another philosopher to interrupt it.
void
print_line(const char *message, int id)
{
  sem_wait(PRINT_LOCK);
  printf(message, id);
  sem_signal(PRINT_LOCK);
}

void
philosopher(int id, int cycles)
{
  int left = id;
  int right = (id + 1) % NPHIL;

  // Resource ordering:
  // Always acquire the lower-numbered fork first.
  int first, second;

  if (left < right) {
    first = left;
    second = right;
  } else {
    first = right;
    second = left;
  }

  for (int i = 0; i < cycles; i++) {

    // THINKING
    print_line("Philosopher %d: THINKING\n", id);
    delay();

    // HUNGRY
    print_line("Philosopher %d: THINKING -> HUNGRY\n", id);
    delay();

    // Acquire lower-numbered fork first.
    sem_wait(forks[first]);

    // Delay so other philosophers can run.
    delay();

    // Acquire higher-numbered fork second.
    sem_wait(forks[second]);

    // EATING
    print_line("Philosopher %d: HUNGRY -> EATING\n", id);
    delay();

    // Release both forks.
    sem_signal(forks[second]);
    sem_signal(forks[first]);

    // Back to thinking.
    print_line("Philosopher %d: EATING -> THINKING\n", id);
    delay();
  }

  sem_wait(PRINT_LOCK);
  printf("Philosopher %d completed all %d cycles\n", id, cycles);
  sem_signal(PRINT_LOCK);

  exit(0);
}

int
main(int argc, char *argv[])
{
  int cycles = DEFAULT_CYCLES;

  // Optional number of cycles:
  // $ dining 10
  if (argc > 1) {
    cycles = atoi(argv[1]);

    if (cycles <= 0)
      cycles = DEFAULT_CYCLES;
  }

  // Initialize all 5 forks as binary semaphores.
  // 1 = available
  // 0 = being used.
  for (int i = 0; i < NFORK; i++)
    sem_init(forks[i], 1);

  // Initialize console output lock.
  sem_init(PRINT_LOCK, 1);

  printf("Dining Philosophers started\n");
  printf("Using resource ordering for deadlock avoidance\n");
  printf("Each philosopher will eat %d times\n\n", cycles);

  // Create 5 philosopher processes.
  for (int i = 0; i < NPHIL; i++) {
    int pid = fork();

    if (pid < 0) {
      printf("fork failed\n");
      exit(1);
    }

    if (pid == 0)
      philosopher(i, cycles);
  }

  // Wait for all 5 philosophers.
  for (int i = 0; i < NPHIL; i++)
    wait(0);

  printf("\nAll philosophers completed successfully\n");
  printf("Dining Philosophers completed\n");

  exit(0);
}