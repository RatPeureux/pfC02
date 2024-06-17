#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define N 4
#define TAILLE (N*N)

typedef int tGrille[TAILLE][TAILLE];

int chargerGrille(tGrille g, char nomFichier[19]);
void afficherGrille(tGrille g);
bool backtracking(tGrille g, int numeroCase);
bool absentSurLigne(int valeur, tGrille g, int ligne);
bool absentSurColonne(int valeur, tGrille g, int colonne);
bool absentSurBloc(int valeur, tGrille g, int ligne, int colonne);

int main(){
    tGrille g;
    char nomFichier[19];

    chargerGrille(g, nomFichier);
    
    printf("\nGrille initiale");
    afficherGrille(g);

    clock_t begin = clock(); // début chrono
    backtracking(g, 0);
    clock_t end = clock(); // fin chrono
    
    printf("Grille finale");
    afficherGrille(g);

    double tmpsCPU = (end - begin)*1.0 / CLOCKS_PER_SEC; // calcul du chrono
    printf("Résolution de %s en %.6lf sec\n\n", nomFichier,tmpsCPU);

    return EXIT_SUCCESS;
}

int chargerGrille(tGrille g, char nomFichier[19]){
    int i, j;
    int compteur = 0;

    FILE * f;
    printf("\nNom du fichier ? ");
    scanf("%s", nomFichier);
    f = fopen(nomFichier, "rb");
    while (f==NULL)
    {
        printf("\n Fichier %s non reconnu \n", nomFichier);
        printf("\nNom du fichier ? ");
        scanf("%s", nomFichier);
        f = fopen(nomFichier, "rb");
    }
    for (i = 0;i< TAILLE;i++){
        for( j = 0; j < TAILLE; j++){
            fread(&g[i][j].valeur, sizeof(int), 1, f);
            if(g[i][j].valeur == 0){
                compteur++;
            }
        }  
    }
    fclose(f);
    return compteur;
}

void afficherGrille(tGrille g){
    int i, j;

    printf("\n");
    for (i = 0; i < TAILLE; i++){
        if (i == (N*0) || i == (N*1) || i == (N*2) || i == (N*3)){
            printf("+------------+------------+------------+------------+\n");
        }
        for (j = 0; j < TAILLE; j++){
            if (j == (N*0) || j == (N*1) || j == (N*2) || j == (N*3)){
                printf("%c", '|');
            }
            if (g[i][j].valeur == 0){
                printf("%s", " . ");
            }else{
                printf("%2d%c", g[i][j].valeur,' ');
            }
        }
        printf("%c", '|');
        printf("\n");
    }
    printf("+------------+------------+------------+------------+\n\n");
}

bool backtracking(tGrille g, int numeroCase){
    int ligne;
    int colonne;
    bool resultat = false;

    if (numeroCase == TAILLE*TAILLE){
        resultat = true;
    }else{
        ligne = numeroCase / TAILLE;
        colonne = numeroCase % TAILLE;
        if (g[ligne][colonne]!=0) {
            resultat = backtracking(g, numeroCase+1);
        }
        else{
            for(int valeur = 1; valeur < TAILLE+1; valeur++){
                if(absentSurLigne(valeur, g, ligne) && absentSurColonne(valeur, g, colonne) && absentSurBloc(valeur, g, ligne, colonne)){
                    g[ligne][colonne] = valeur;
                    if (backtracking(g, numeroCase+1)){
                        resultat = true;
                    }
                    else{
                        g[ligne][colonne] = 0;
                    }
                }
            }
        }
    }

    return resultat;
}

bool absentSurLigne(int valeur, tGrille g, int ligne){
    bool resultat = true;
    int j = 0;
    while (j < TAILLE && resultat == true){
        if (g[ligne][j] != 0 && g[ligne][j] == valeur){
            resultat = false;
        }
        j++;
    }

    return resultat;
}

bool absentSurColonne(int valeur, tGrille g, int colonne){
    bool resultat = true;
    int i = 0;
    while (i < TAILLE && resultat == true){
        if (g[i][colonne] != 0 && g[i][colonne] == valeur){
            resultat = false;
        }
        i++;
    }
    
    return resultat;
}

bool absentSurBloc(int valeur, tGrille g, int ligne, int colonne){
    bool resultat = true;
    int ligbloc = N*(ligne/N);
    int colbloc = N*(colonne/N);
    int i = ligbloc;
    while (i < ligbloc+N && resultat == true){ 
        int j = colbloc;
        while (j < colbloc+N && resultat == true){
            if (g[i][j].valeur != 0 && g[i][j].valeur == valeur){
                resultat = false;
            }
            j++;
        }
        i++;
    }
    
    return resultat;
}
