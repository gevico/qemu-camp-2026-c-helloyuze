#include <stdio.h>
#include <stdbool.h>

#define MAX_PEOPLE 50

typedef struct {
    int id;
} People;

typedef struct {
    People data[MAX_PEOPLE];
    int head;
    int tail;
    int count;
} Queue;

// 入队操作
void enqueue(Queue *q, int id) {
       q->data[q->tail].id=id;
       q->tail=(q->tail+1)%MAX_PEOPLE;
       q->count++;
}

// 出队操作
int dequeue(Queue *q) {
      int id=q->data[q->head].id;
       q->head=(q->head+1)%MAX_PEOPLE;
       q->count--;
       return id;
}

int main() {
    Queue q={0};
    int total_people=50;
    int report_interval=5;
    for (int i=1;i<= total_people;i++)
    enqueue(&q,i);
    int k=0;
    while(q.count>1){
     int id =dequeue(&q);
      k++;
      if(k==report_interval)
      {
        k=0;
        printf("淘汰:%d\n", id);
      }

    else enqueue(&q,id);



   }

    
    printf("最后剩下的人是: %d\n", q.data[q.head].id);

    return 0;
}