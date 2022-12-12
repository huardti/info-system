#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

#define TRUE  1
#define FALSE 0

typedef struct _threadargs
{
    int                 return_value;
    char*               character;
    pthread_cond_t*     cv;
    pthread_mutex_t*    mutex;
} threadargs;

void* keyboard_main(void* args);
void* display_main(void* args);
void keyboard_print(char* string);
void display_print(char* string);

int main(void)
{
    pthread_cond_t cv;
    if(pthread_cond_init(&cv, NULL) != 0)
    {
        perror("pthread_cond_init()");
        exit(EXIT_FAILURE);
    }
    pthread_mutex_t mutex;
    if(pthread_mutex_init(&mutex, NULL) != 0)
    {
        perror("pthread_mutex_init()");
        exit(EXIT_FAILURE);
    }

    pthread_t keyboard_thread, display_thread;

    char common_character = '\0';

    threadargs keyboard_args =
    {
        .character = &common_character,
        .cv = &cv,
        .mutex = &mutex
    };
    threadargs display_args =
    {
        .character = &common_character,
        .cv = &cv,
        .mutex = &mutex
    };

    int pthread_return;

    pthread_return = pthread_create
        (&keyboard_thread, NULL, keyboard_main, (void*) &keyboard_args);
    if(pthread_return != 0)
    {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
    pthread_return = pthread_create
        (&display_thread, NULL, display_main, (void*) &display_args);
    if(pthread_return != 0)
    {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    pthread_join(keyboard_thread, NULL);
    pthread_join(display_thread, NULL);

    printf("Keyboard : %d\tDisplay : %d\n",
        keyboard_args.return_value, display_args.return_value);

    pthread_cond_destroy(&cv);
    pthread_mutex_destroy(&mutex);

    return EXIT_SUCCESS;
}

void* keyboard_main(void* args)
{
    threadargs* cargs = (threadargs*) args;

    pthread_mutex_lock(cargs->mutex);
        keyboard_print("Hello !");
    pthread_cond_signal(cargs->cv);
    pthread_cond_wait(cargs->cv, cargs->mutex);

    char ch;

    while(*(cargs->character) != 'F')
    {
        do
        {
            printf("Enter a character : ");
            scanf("%c", cargs->character);
        } while( *(cargs->character) == '\n' );

        // Flush buffer
        while ((ch = getchar()) != '\n' && ch != EOF);

        pthread_cond_signal(cargs->cv);
        pthread_cond_wait(cargs->cv, cargs->mutex);
    }

    pthread_mutex_unlock(cargs->mutex);

    cargs->return_value = 0;
    return NULL;
}
void* display_main(void* args)
{
    threadargs* cargs = (threadargs*) args;

    pthread_mutex_lock(cargs->mutex);
        display_print("Hello !");
    pthread_cond_signal(cargs->cv);

    while(*(cargs->character) != 'F')
    {
        pthread_cond_wait(cargs->cv, cargs->mutex);

        printf("Received '%c'\n", *(cargs->character));

        pthread_cond_signal(cargs->cv);
    }

    pthread_mutex_unlock(cargs->mutex);

    cargs->return_value = 0;
    return NULL;
}

void keyboard_print(char* string)
{
    printf("[Keyboard] %s\n", string);
}
void display_print(char* string)
{
    printf("[Display] %s\n", string);
}