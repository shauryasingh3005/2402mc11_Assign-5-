#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define ITERATIONS 10

struct shared_data {
int flag[2];
int turn;
int shared_counter;
};

int 
main(void)
{
struct shared_data *shared;
int pid;
int id;

shared=(struct shared_data *)shm_get();
if(shared==0){
printf("shm_get failed\n");
exit(1);
}

shared->flag[0]=0;
shared->flag[1]=0;
shared->turn=0;
shared->shared_counter =0;

pid=fork();

if(pid<0){
printf("fork failed\n");
exit(1);
}

if(pid==0)
id=1;
else
id=0;

for(int i=0;i<ITERATIONS;i++){

shared->flag[id]=1;
shared->turn = 1-id;

while(shared->flag[1-id] && shared->turn == 1-id){
}

int temp=shared->shared_counter;
for(volatile int d=0;d<100000;d++)
;
shared->shared_counter=temp+1;
printf("Process %d in CS, counter= %d\n",id,shared->shared_counter);

shared->flag[id]=0;

for(volatile int d=0;d<100000;d++)
;
}

if(pid!=0)
wait(0);
if(pid==0)
printf("Final shared counter = %d\n",shared->shared_counter);

exit(0);
}
