#ifndef __CONCURRENCY_H__
#define __CONCURRENCY_H__
#ifdef __cplusplus
extern "C" {
#endif

typedef struct p_queue PriorityQueue;
typedef struct p_node PriorityNode;
struct p_node{
  int priority;
  Priority Node * next;
}
struct p_queue {
  int priority; // value from 0 to 256
  
}

#ifdef __cplusplus
}
#endif

#endif /* __CONCURRENCY_H__ */
