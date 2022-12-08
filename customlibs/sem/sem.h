typedef int SEMAPHORE;

int P(SEMAPHORE sem);
int V(SEMAPHORE sem);

SEMAPHORE sem_create(int private, char* filepath, int id);
SEMAPHORE sem_use(int private, char* filepath, int id);
void sem_delete(SEMAPHORE sem);
void sem_init(SEMAPHORE sem, unsigned short* intarray);