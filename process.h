typedef struct process process_t;
process_t *create_process(int sz);
process_t *insert_process(int beg, int end, int queues, int arr[]);
int find_PID(int PID, process_t *process_arr[], int n_process);
void update_run(int n_process, process_t *process_arr[]);
void update_blocked(int n_process, process_t *process_arr[]);
void update_index_run(int n_process, process_t *process_arr[], int size);
void update_index_blocked(int n_process, process_t *process_arr[], int size);