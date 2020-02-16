#include <sys/types.h>
#include <sys/wait.h>
// #include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#define MINE_NO 100


static int *test;

int main () 
{

    u_int32_t miners_no;
    u_int32_t ore_to_gold_ratio;

    printf("Please enter the number of extractors (children, miners): ");
    scanf("%u", &miners_no);
    printf("Please enter the ratio of gold ore to gold: ");
    scanf("%u", &ore_to_gold_ratio);

    pid_t *miner;
    miner = calloc(miners_no, sizeof(pid_t));
    

    test = mmap(NULL, sizeof(*test), PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *test = 1;

    
    // create shared memory
    const char *shm_name = "mine_info";
    const int   shm_size = 128;
    void *ptr;
    int shm_fd;
    
    shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, shm_size);
    ptr = mmap(0, shm_size, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);

    // save initial data into shared memory
    u_int32_t gold_mines = MINE_NO;
    u_int32_t this_miner = 0;
    sprintf(ptr, "%u", gold_mines);
    ptr += sizeof(gold_mines);
    sprintf(ptr, "%u", this_miner);
    ptr -= sizeof(gold_mines);


    srand(50);
    for (int i = 0; i < miners_no; i++) {
        
        miner[i] = fork();
        if (miner[i] < 0) {
        // if (miner[i] = fork() < 0) {
            printf("fork failed\n");
            exit(1);
        }
        else if (miner[i] == 0) {
            printf("New miner in child, Miner ID: %d, PID: %d\n", i, getpid());
            int gold_ore = 0;

            // shm_fd = shm_open(shm_name, O_RDWR, 0666);
            // ptr = mmap(0, shm_size, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);
            sscanf(ptr, "%u%u", &gold_mines, &this_miner);
            // printf("Miner ID %d, gold_mines %3u, this_miner %u\n", i, gold_mines, this_miner);
            
            
            // shm_unlink(shm_name);
            
            while (gold_mines > 0) {

                // shm_fd = shm_open(shm_name, O_RDWR, 0666);
                // ptr = mmap(0, shm_size, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);
                sscanf(ptr, "%u%u", &gold_mines, &this_miner);
                printf("Miner ID %d, gold_mines %3u, this_miner %u\n", i, gold_mines, this_miner);
                
                *test++;
                printf("test: %d\n", *test);
                
                if (this_miner == i) {
                    gold_ore += rand() % 11;
                    gold_ore += rand() % 11;
                    printf("%d\n", gold_ore);
                    gold_mines -=2;
                    if (this_miner == miners_no - 1) this_miner = 0;
                    else this_miner++;

                    
                    // sprintf(ptr, "%u", gold_mines);
                    // ptr += sizeof(gold_mines);
                    // sprintf(ptr, "%u", this_miner);
                    // ptr -= sizeof(gold_mines);

                    memcpy(ptr, &gold_mines, sizeof(gold_mines));
                    memcpy(ptr + sizeof(gold_mines), &this_miner, sizeof(this_miner));

                    printf("Miner ID: %d, gold_mines: %u, this_miner: %u. (should)\n", 
                        i, gold_mines, this_miner);
                    sscanf(ptr, "%u%u", &gold_mines, &this_miner);
                    printf("Miner ID: %d, gold_mines: %u, this_miner: %u. (memory)\n", 
                        i, gold_mines, this_miner);
                    // msync(ptr, sizeof(gold_mines) + sizeof(this_miner), MS_SYNC);
                }
                // munmap(ptr, shm_size);
                
                // shm_unlink(shm_name);
                // shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
                // ptr = mmap(0, shm_size, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);
                // shm_unlink(shm_name);
                sleep(1);
                // sscanf(ptr, "%u%u", &gold_mines, &this_miner);
            }
            // shm_unlink(shm_name);
            exit(gold_ore);
        }
        else {
            printf("New miner in parent, Miner ID: %d\n", i);
        }
    }


    int status;
    pid_t miner_id;
    while (miners_no > 0) {
        miner_id = wait(&status);
        printf("Miner %d exited with %d golds and %d ores left.\n", 
        miner_id, status / ore_to_gold_ratio, status % ore_to_gold_ratio);
        miners_no--;
    }


    exit(0);
}