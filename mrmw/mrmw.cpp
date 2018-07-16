#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define ITERATION 100000
#define SIZE 1024
#define RTHREADS 4
#define WTHREADS 4
#define PADDINGWIDTH 64

int *array;

void *readerRun(void *p)
{
  int index = *((int *)p);
  //printf("%d\n",index);
  int i, j;
  int current=0, prev=0;
  long long diff_count=0;
  for(i=0; i<ITERATION; i++)
  {
    for(j=index; j<SIZE; j+=RTHREADS)
    {
      if(j!=index)prev=current;
      current=array[j*PADDINGWIDTH];
      if(current!=prev)diff_count+=1;
    }
  }
  return NULL;
}

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
  array=(int *)malloc(PADDINGWIDTH*SIZE*sizeof(int));
  pthread_t readThreads[RTHREADS], writeThreads[WTHREADS];
  int i, j, readIndices[RTHREADS], writeIndices[WTHREADS];
  for(i=0;i<RTHREADS;i++)
  {
    readIndices[i]=i;
  }
  for(i=0;i<WTHREADS;i++)
  {
    writeIndices[i]=i;
  }
  for(i=0;i<RTHREADS;i++)
  {
    pthread_create(&readThreads[i], NULL, readerRun, (void *)&readIndices[i]);
  }
  for(i=0;i<WTHREADS;i++)
  {
    pthread_create(&writeThreads[i], NULL, writerRun, (void *)&writeIndices[i]);
  }
  for(j=0;j<RTHREADS;j++)
  {
    pthread_join(readThreads[j], NULL);
  }
  for(j=0;j<WTHREADS;j++)
  {
    pthread_join(writeThreads[j], NULL);
  }
  return 0;
}
