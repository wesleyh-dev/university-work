
#define LOCK -1
#define UNLOCK 1

typedef struct vpop {
  int op;
  int *sem;
} vpop; 


