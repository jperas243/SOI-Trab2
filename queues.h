typedef struct queue queue_t;
queue_t *create_queue (int sz);
void insert (queue_t *queue, int element);
bool full (queue_t *queue);
bool empty (queue_t *queue);
int get (queue_t *queue);
void printQueue(queue_t *queue);
int top(queue_t *queue);
int get_pos(queue_t *queue, int pos);