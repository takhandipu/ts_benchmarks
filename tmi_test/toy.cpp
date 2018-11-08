#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define ITERATION 1000000000
#define SIZE 1024
#define RWTHREADS 1
#define PADDINGWIDTH 1

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
    current=array[index*PADDINGWIDTH];
    if(current!=prev)diff_count+=1;
    prev=current;
    /*for(j=index; j<SIZE; j+=RWTHREADS)
    {
      if(j!=index)prev=current;
      current=array[j*PADDINGWIDTH];
      if(current!=prev)diff_count+=1;
    }*/
  }
  return NULL;
}

void *writerRun(void *p)
{
  int index = *((int *)p);
  //printf("%d\n",index);
  int i, j;
  for(j=index;j<SIZE;j+=RWTHREADS)
  {
    array[j*PADDINGWIDTH]=0;
  }
  for(i=0; i<ITERATION; i++)
  {
    int incr;
    if(i%2)incr=1;
    else if(i%3)incr=1;
    else if(i%5)incr=1;
    else incr=0;
    array[index*PADDINGWIDTH]+=incr;
    /*for(j=index; j<SIZE; j+=RWTHREADS)
    {
      int incr;
      if(((i+j)%2)==0)incr=0;
      else incr=1;
      array[j*PADDINGWIDTH]+=incr;
    }*/
  }
  return NULL;
}

int main(int argc, char *argv[])
{
  array=(int *)malloc(PADDINGWIDTH*SIZE*sizeof(int));
  pthread_t readThreads[RWTHREADS], writeThreads[RWTHREADS];
  int i, j, readIndices[RWTHREADS], writeIndices[RWTHREADS];
  for(i=0;i<RWTHREADS;i++)
  {
    readIndices[i]=i;
    writeIndices[i]=i+1;
  }
  for(i=0;i<RWTHREADS;i++)
  {
    pthread_create(&readThreads[i], NULL, readerRun, (void *)&readIndices[i]);
    pthread_create(&writeThreads[i], NULL, writerRun, (void *)&writeIndices[i]);
  }
  for(j=0;j<RWTHREADS;j++)
  {
    pthread_join(readThreads[j], NULL);
    pthread_join(writeThreads[j], NULL);
  }
  return 0;
}
