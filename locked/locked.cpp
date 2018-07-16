#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <atomic>

#define ITERATION 100000
#define SIZE 1024
#define WTHREADS 4
#define PADDINGWIDTH 1

using namespace std;

atomic<int> *array;

void *writerRun(void *p)
{
  int index = *((int *)p);
  //printf("%d\n",index);
  int i, j;
  for(j=0;j<SIZE;j+=1)
  {
    array[PADDINGWIDTH*j]=0;
  }
  for(i=0; i<ITERATION; i++)
  {
    for(j=0; j<SIZE; j+=1)
    {
      int incr;
      if(((i+j)%2)==0)incr=0;
      else incr=1;
      array[PADDINGWIDTH*j]+=incr;
    }
  }
  return NULL;
}

int main(int argc, char *argv[])
{
  array=(atomic<int> *)malloc(PADDINGWIDTH*SIZE*sizeof(atomic<int>));
  pthread_t writeThreads[WTHREADS];
  int i, j, writeIndices[WTHREADS]; 
  for(i=0;i<WTHREADS;i++)
  {
    writeIndices[i]=i;
  }
  for(i=0;i<WTHREADS;i++)
  {
    pthread_create(&writeThreads[i], NULL, writerRun, (void *)&writeIndices[i]);
  } 
  for(j=0;j<WTHREADS;j++)
  {
    pthread_join(writeThreads[j], NULL);
  }
  return 0;
}
