#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

#define TRUE  1
#define FALSE 0

typedef struct _threadargs
{
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
    // Allocate kernel resources (CV + Mutex)
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
    ////////////////////////////////////////////////////////////////////////////

    // Initialize thread arguments
    char common_character = '\0';
    threadargs keyboard_args =
    {
        .character      = &common_character,
        .cv             = &cv,
        .mutex          = &mutex
    };
    threadargs display_args =
    {
        .character      = &common_character,
        .cv             = &cv,
        .mutex          = &mutex
    };
    ////////////////////////////////////////////////////////////////////////////

    // Start the threads
    pthread_t keyboard_thread, display_thread;
    if
    (
        pthread_create
            (&keyboard_thread, NULL, keyboard_main, (void*) &keyboard_args)
        != 0
    )
    {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
    if
    (
        pthread_create
            (&display_thread, NULL, display_main, (void*) &display_args)
        != 0
    )
    {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
    ////////////////////////////////////////////////////////////////////////////

    // Wait for the threads to finish
    int* keyboard_return = NULL;
    if(pthread_join(keyboard_thread, (void*) &keyboard_return) != 0)
    {
        perror("pthread_join()");
        exit(EXIT_FAILURE);
    }
    int* display_return = NULL;
    if(pthread_join(display_thread, (void*) &display_return) != 0)
    {
        perror("pthread_join()");
        exit(EXIT_FAILURE);
    }
    ////////////////////////////////////////////////////////////////////////////

    // Display return values
    printf("Keyboard : %d\tDisplay : %d\n", *keyboard_return, *display_return);
    ////////////////////////////////////////////////////////////////////////////

    // Free allocated resources
    free(keyboard_return);
    free(display_return);
    pthread_cond_destroy(&cv);
    pthread_mutex_destroy(&mutex);
    ////////////////////////////////////////////////////////////////////////////

    return EXIT_SUCCESS;
}

void* keyboard_main(void* args)
{
    // Temporary pointer to avoid casting a billion times later on
    threadargs* cargs = (threadargs*) args;

    // Lock the mutex
    pthread_mutex_lock(cargs->mutex);
        // Display a greeting
        keyboard_print("Hello !");
    // Signal that the greeting is done
    pthread_cond_signal(cargs->cv);
    // Unlock the mutex, wait for a signal back, lock the mutex
    pthread_cond_wait(cargs->cv, cargs->mutex);

    // Temporary variable for user input
    char ch;

    while(*(cargs->character) != 'F')
    {
        // Get any character different than '\n', otherwise loop
        do
        {
            printf("Enter a character : ");
            scanf("%c", cargs->character);
        } while( *(cargs->character) == '\n' );

        // Flush buffer to keep only the first character
        while ((ch = getchar()) != '\n' && ch != EOF);

        // Signal that we're done getting a character
        pthread_cond_signal(cargs->cv);
        // Unlock the mutex, wait for a signal back, lock the mutex
        pthread_cond_wait(cargs->cv, cargs->mutex);
    }

    // Unlock the mutex
    pthread_mutex_unlock(cargs->mutex);
    // Signal that we're exiting
    pthread_cond_signal(cargs->cv);

    int* return_value = malloc(sizeof(int));
    *return_value = 2;

    return return_value;
}
void* display_main(void* args)
{
    // Temporary pointer to avoid casting a billion times later on
    threadargs* cargs = (threadargs*) args;

    // Lock the mutex
    pthread_mutex_lock(cargs->mutex);
        // Display a greeting
        display_print("Hello !");
    // Signal that the greeting is done
    pthread_cond_signal(cargs->cv);

    while(*(cargs->character) != 'F')
    {
        // Unlock the mutex, wait for a signal back, lock the mutex
        pthread_cond_wait(cargs->cv, cargs->mutex);
        
        // Display the received character
        printf("Received '%c'\n", *(cargs->character));

        // Signal that displaying is done
        pthread_cond_signal(cargs->cv);
    }

    // Unlock the mutex
    pthread_mutex_unlock(cargs->mutex);
    // Signal that we're exiting
    pthread_cond_signal(cargs->cv);

    int* return_value = malloc(sizeof(int));
    *return_value = 3;

    return return_value;
}

void keyboard_print(char* string)
{
    printf("[Keyboard] %s\n", string);
}
void display_print(char* string)
{
    printf("[Display] %s\n", string);
}