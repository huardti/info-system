#include <stdio.h>
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

int main(void)
{
    int taille = TAILLE_TABLEAU;
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

    /* Calcul de la matrice resultante */
    for(ligne = 0; ligne < taille; ligne++)
    {
        for (colonne = 0; colonne < taille; colonne++)
        {
            multiplication(taille, ligne, colonne, MA, MB, MC);
        }
    }

    /* Affichage du resultat */
    printf("MATRICE: resultat de la matrice C;\n");
    for(ligne = 0; ligne < taille; ligne++)
    {
        for (colonne = 0; colonne < taille; colonne++)
        {
            printf("%5d ",MC[ligne][colonne]);
        }
        printf("\n");
    }
    
    return 0;
}