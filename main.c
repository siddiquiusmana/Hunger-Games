#include <stdio.h>
#include <stdlib.h>
#include "leak_detector_c.c"
#define MAX 10000

FILE *opf;

typedef struct prisoner{
  int prisoner_no;
  struct prisoner* next;
  struct prisoner* prev;
} prisoner;

typedef struct queue{
  int prisoner_no;
  char name[50];
  int prisoner_count;
  int k;
  int th;
  int noe;
  prisoner* back;
  prisoner* front;
} queue;

prisoner* createprisoner(int prisoner_no);
void createReverseCircle(queue *q);
queue* enqueue(queue *q, prisoner* node);
void prisonerOrder(queue** group, int numGroups);
void rearrangeCircle(queue* q);
void display(queue q);
void phase1(queue* q);
void phase2(queue** group, int numGroups);
int isEmpty(queue* q);
void dequeue(queue* q);

int main()
{
  atexit(report_mem_leak);

  FILE *ifp = fopen("in.txt", "r");
  int numGroups;
  fscanf(ifp, "%d", &numGroups);

  queue** group = (queue**) malloc(numGroups* sizeof(queue*));
  for(int i=0; i<numGroups; i++)
  {
    group[i] = (queue*) malloc(sizeof(queue));
    fscanf(ifp, "%d %s %d %d %d", &group[i]->prisoner_no, group[i]->name, &group[i]->prisoner_count, &group[i]->k, &group[i]->th);
    group[i]->back = NULL;
    group[i]->front = NULL;
    createReverseCircle(group[i]);
  }
  fclose(ifp);

  prisonerOrder(group, numGroups);

  opf = fopen("out.txt", "w");

  fprintf(opf, "Initial nonempty lists status\n");
  printf("Initial nonempty lists status\n");
  for(int i=0; i<numGroups; i++)
  {
    display(*group[i]);
  }

  for(int i=0; i<numGroups; i++)
  {
    rearrangeCircle(group[i]);
  }

  fprintf(opf, "\nAfter ordering nonempty lists status\n");
  printf("\nAfter ordering nonempty lists status\n");
  for(int i=0; i<numGroups; i++)
  {
    display(*group[i]);
  }

  fprintf(opf, "\nPhase1 execution\n");
  printf("\nPhase1 execution\n");
  for(int i=0; i<numGroups; i++)
  {
    fprintf(opf, "\nLine# %d %s\n", group[i]->prisoner_no, group[i]->name);
    printf("\nLine# %d %s\n", group[i]->prisoner_no, group[i]->name);
    phase1(group[i]);
  }

  fprintf(opf, "\nPhase2 execution\n");
  printf("\nPhase2 execution\n");
  phase2(group, numGroups);

  for(int i=0; i<numGroups; i++)
  {
    if(group[i]->prisoner_count!=0)
    {
      fprintf(opf, "\nprisoner %d from line %d will survive", group[i]->front->prisoner_no, group[i]->prisoner_no);
      printf("\nprisoner %d from line %d will survive", group[i]->front->prisoner_no, group[i]->prisoner_no);
      free(group[i]->front);
    }
  }

  for(int i=0; i<numGroups; i++)
  {
    free(group[i]);
  }
  free(group);

  fclose(opf);
  return 0;
}

prisoner* createprisoner(int prisoner_no)
{
  prisoner* temp = (prisoner*) malloc(sizeof(prisoner));
  temp->next = NULL;
  temp->prev = NULL;
  temp->prisoner_no = prisoner_no;
  return temp;
}

void createReverseCircle(queue *q)
{
  for(int i=q->prisoner_count; i>=1 ; i--)
  {
    prisoner* temp = createprisoner(i);
    q = enqueue(q, temp);
  }
}

queue* enqueue(queue *q, prisoner* node)
{
  if(q->back==NULL)
  {
    q->back = node;
    q->front = node;
  }
  else
  {
    q->back->next = node;
    q->back->next->prev = q->back;
    q->back = node;
    q->back->next = q->front;
    q->front->prev = q->back;
  }
  return q;
}

void prisonerOrder(queue** group, int numGroups)
{
  for(int i=0; i<numGroups; i++)
  {
    for (int j = i+1; j<numGroups; j++)
		{
			if(group[i]->prisoner_no > group[j]->prisoner_no)
			{
        queue* temp;
				temp = group[i];
				group[i] = group[j];
				group[j] = temp;
			}
		}
  }
}

void rearrangeCircle(queue* q)
{
  prisoner* t = q->front;
  q->front = q->back;
  q->back = t;
  t = q->front;
  prisoner* t2 = t->prev;
  for(int i=0; i<q->prisoner_count; i++)
  {
    t->next = t2;
    t = t2;
    t2 = t->prev;
    t->prev = t->next;
  }
  q->back->next = q->front;
  q->front->prev = q->back;
}

void display(queue q)
{
  if(q.prisoner_count!=0)
  {
    fprintf(opf, "%d %s", q.prisoner_no, q.name);
    printf("%d %s", q.prisoner_no, q.name);
    prisoner* temp = q.front;
    for(int j=q.prisoner_count; j>=1; j--)
    {
      fprintf(opf, " %d", temp->prisoner_no);
      printf(" %d", temp->prisoner_no);
      temp = temp->next;
    }
    fprintf(opf, "\n");
    printf("\n");
  }
}

void phase1(queue* q)
{
  int counter = 0;
  prisoner* t = q->front;
  for(int i=0; i<((q->prisoner_count)-(q->th)); i++)
  {
    for(int j=0; j<((q->k)-1); j++)
    {
      t=t->next;
    }
    fprintf(opf, "prisoner# %d executed\n", t->prisoner_no);
    printf("prisoner# %d executed\n", t->prisoner_no);
    if(t == q->back)
    {
      q->back = t->prev;
    }
    else if(t == q->front)
    {
      q->front = t->next;
    }
    prisoner* temp = t;
    t = t->prev;
    t->next = t->next->next;
    temp->next->prev = t;
    free(temp);
    counter++;
    t = t->next;
  }
  q->prisoner_count = q->prisoner_count - counter;
}

void phase2(queue** group, int numGroups)
{
  int totalNodes = 0;
  for(int i=0; i<numGroups; i++)
  {
    totalNodes += group[i]->prisoner_count;
  }
  queue* max = group[1];
  for(int i=totalNodes; i>1; i--)
  {
    for(int j=0; j<numGroups; j++)
    {
      if(group[j]->prisoner_count!=0)
      {
        if(group[j]->front->prisoner_no > max->front->prisoner_no)
        {
          max = group[j];
        }
      }
    }
    fprintf(opf, "Executed prisoner %d from line %d\n", max->front->prisoner_no, max->prisoner_no);
    printf("Executed prisoner %d from line %d\n", max->front->prisoner_no, max->prisoner_no);
    dequeue(max);
    for(int k=0; k<numGroups; k++)
    {
      if(group[k]->prisoner_count!=0)
      {
        max = group[k];
        break;
      }
    }
  }
}

int isEmpty(queue* q)
{
  if(q->prisoner_count < 1)
  {
    return 1;
  }
  else return 0;
}

void dequeue(queue* q)
{
  if(isEmpty(q) != 1)
  {
    prisoner* temp = q->front;
    if(q->prisoner_count == 2)
    {
      q->front = q->back;
      q->back->next = NULL;
      q->back->prev = NULL;
      free(temp);
      q->prisoner_count--;
    }
    else if(q->prisoner_count == 1)
    {
      free(temp);
      q->front = NULL;
      q->back = NULL;
      q->prisoner_count--;
    }
    else
    {
      q->front = temp->next;
      q->front->prev = q->back;
      q->back->next = q->front;
      free(temp);
      q->prisoner_count--;
    }
  }
}
