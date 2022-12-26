#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

#define TAILLE_TABLEAU 10

typedef int matrice_t[TAILLE_TABLEAU][TAILLE_TABLEAU];

matrice_t MA, MB, MC;

void multiplication
(
    int taille,
    int ligne,
    int colonne,
    matrice_t MA,
    matrice_t MB,
    matrice_t MC
);

typedef struct _threadargs
{
    int id;
    int taille;
    int ligneA;
    matrice_t* MA;
    matrice_t* MB;
    matrice_t* MC;
} threadargs;

void* multhread_main(void* args);

int main(void)
{
    int ligne, colonne;

    // Initialisation des matrices MA et MB
    for(int i = 0 ; i < TAILLE_TABLEAU ; ++i)
    {
        for(int j = 0 ; j < TAILLE_TABLEAU ; ++j)
        {
            MA[i][j] = 2;
            MB[i][j] = 3;
        }
    }

    pthread_t multhreads[TAILLE_TABLEAU];
    threadargs multhreadargs[TAILLE_TABLEAU];

    /* Calcul de la matrice resultante */
    int id = 0;
    for(ligne = 0; ligne < TAILLE_TABLEAU; ++ligne)
    {
        multhreadargs[ligne] = (threadargs)
        {
            .id         = id,
            .taille     = TAILLE_TABLEAU,
            .ligneA     = ligne,
            .MA         = &MA,
            .MB         = &MB,
            .MC         = &MC
        };

        if
        (
            pthread_create
            (
                &(multhreads[ligne]),
                NULL,
                multhread_main,
                (void*) &(multhreadargs[ligne])
            )
            != 0
        )
        {
            perror("pthread_create()");
            exit(EXIT_FAILURE);
        }

        ++id;
    }

    /* Affichage du resultat */
    printf("MATRICE: resultat de la matrice C;\n");
    for(ligne = 0; ligne < TAILLE_TABLEAU; ++ligne)
    {
        pthread_join(multhreads[ligne], NULL);
        
        for (colonne = 0; colonne < TAILLE_TABLEAU; ++colonne)
        {
            printf("%5d ",MC[ligne][colonne]);
        }
        printf("\n");
    }
    
    return 0;
}

void multiplication
(
    int taille,
    int ligne,
    int colonne,
    matrice_t MA,
    matrice_t MB,
    matrice_t MC
)
{
    int position;
    MC[ligne][colonne] = 0;
    for(position = 0; position < taille; position++)
    {
        MC[ligne][colonne] = MC[ligne][colonne] +
            ( MA[ligne][position] * MB[position][colonne] ) ;
    }
}

void* multhread_main(void* args)
{
    threadargs* cargs = (threadargs*) args;

    //printf("[Thread %02d] Hello !\n", cargs->id);

    for(int colonne = 0 ; colonne < cargs->taille ; ++colonne)
    {
        multiplication
        (
            cargs->taille,
            cargs->ligneA,
            colonne,
            *(cargs->MA),
            *(cargs->MB),
            *(cargs->MC)
        );
    }

    return NULL;
}