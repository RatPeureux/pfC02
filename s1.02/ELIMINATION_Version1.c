#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define N 3
#define TAILLE (N*N)

typedef struct {
    int valeur;
    int candidats[TAILLE];
    int nbCandidats;
} tCase1;

typedef tCase1 tGrille[TAILLE][TAILLE];

int chargerGrille(tGrille g, char nomFichier[19]);
void afficherGrille(tGrille g);
void afficherStats(char nomFichier[19], int nbCasesRemplis, int nbCasesVides, int nbCandidatsAjouter, int nbCandidatsEliminer);

void initialiserCases(tGrille g);
void ajouterCandidat(tCase1 *laCase, int val);
void retirerCandidat(tCase1 *laCase, int val);
bool estCandidat(tCase1 laCase, int val);
int nbCandidats(tCase1 laCase);
int initialiserCandidats(tGrille g);
void majCandidats(tGrille g, int *nbCandidatsEliminer);

int main(){
    char nomFichier[19];

    tGrille g;
    bool progression;
    int nbCasesVides;
    int nbCasesRemplis = 0;
    int nbCandidatsAjouter;
    int nbCandidatsEliminer = 0;

    int seule;
    int ibloc, jbloc;
    int k, l;

    int paire;
    int m, n;

    progression = true;
    nbCasesVides = chargerGrille(g, nomFichier);

    initialiserCases(g);
    nbCandidatsAjouter = initialiserCandidats(g);

    clock_t begin = clock();
    while (nbCasesVides > 0 && progression){
        progression = false;
        for(int i = 0; i < TAILLE; i++){
            for(int j = 0; j < TAILLE; j++){
                ibloc = N*(i/N);
                jbloc = N*(j/N);

                // technique du singleton nu
                if (g[i][j].valeur == 0 && nbCandidats(g[i][j].valeur) == 1){
                    g[i][j].valeur = g[i][j].valeur.candidats[0];
                    nbCasesRemplis++;
                    retirerCandidat(&(g[i][j]), g[i][j].candidats[0]);   
                    nbCandidatsEliminer++;
                    progression = true;
                    majCandidats(g, &nbCandidatsEliminer);
                }

                // technique du singleton caché
                if (g[i][j].valeur == 0){
                    for (int x = 1; x < TAILLE+1; x++){

                        // bloc
                        seule = 0;
                        k = ibloc;
                        while (k < ibloc+N && seule < 2 && g[i][j].valeur == 0){
                            l = jbloc;
                            while (l < jbloc+N && seule < 2 && g[i][j].valeur == 0){
                                if (g[k][l].valeur == 0){
                                    if (estCandidat(g[k][l], x)){
                                        seule++;
                                    }
                                }
                                l++;
                            }
                            k++;
                        }
                        if (seule == 1 && estCandidat(g[i][j].valeur, x)){
                            g[i][j].valeur = x;
                            nbCasesRemplis++;
                            retirerCandidat(&(g[i][j].valeur), x);
                            nbCandidatsEliminer++;
                            k = 0;
                            while (k < TAILLE && nbCandidats(g[i][j].valeur) > 0){
                                retirerCandidat(&(g[i][j].valeur), g[i][j].valeur.candidats[k]);
                                nbCandidatsEliminer++;
                                k++;
                            }
                            progression = true;
                            majCandidats(g, &nbCandidatsEliminer);
                        }

                        // ligne
                        seule = 0;
                        k = 0;
                        while (k < TAILLE && seule < 2 && g[i][j].valeur == 0){
                            if (g[i][k].valeur == 0){
                                if (estCandidat(g[i][k], x)){
                                    seule++;
                                }
                            }
                            k++;
                        }
                        if (seule == 1 && estCandidat(g[i][j].valeur, x)){
                            g[i][j].valeur = x;
                            nbCasesRemplis++;
                            retirerCandidat(&(g[i][j].valeur), x);
                            nbCandidatsEliminer++;
                            k = 0;
                            while (k < TAILLE && nbCandidats(g[i][j].valeur) > 0){
                                retirerCandidat(&(g[i][j].valeur), g[i][j].valeur.candidats[k]);
                                nbCandidatsEliminer++;
                                k++;
                            }
                            progression = true;
                            majCandidats(g, &nbCandidatsEliminer);
                        }

                        // colonne
                        seule = 0;
                        k = 0;
                        while (k < TAILLE && seule < 2 && g[i][j].valeur == 0){
                            if (g[k][j].valeur == 0){
                                    if (estCandidat(g[k][j], x)){
                                        seule++;
                                    }
                                }
                            k++;
                        }
                        if (seule == 1 && estCandidat(g[i][j].valeur, x)){
                            g[i][j].valeur = x;
                            nbCasesRemplis++;
                            retirerCandidat(&(g[i][j].valeur), x);
                            nbCandidatsEliminer++;
                            k = 0;
                            while (k < TAILLE && nbCandidats(g[i][j].valeur) > 0){
                                retirerCandidat(&(g[i][j].valeur), g[i][j].valeur.candidats[k]);
                                nbCandidatsEliminer++;
                                k++;
                            }
                            progression = true;
                            majCandidats(g, &nbCandidatsEliminer);
                        }
                    }
                }
                
                // technique des paires cachés pour une case i j avec 3 candidats seulement
                if (g[i][j].valeur == 0 && nbCandidats(g[i][j].valeur) == 3){

                    // bloc
                    for (int a = 1; a < TAILLE+1; a++){
                        if (estCandidat(g[i][j].valeur, a)){
                            for (int b = 1; b < TAILLE+1; b++){
                                if (a != b && estCandidat(g[i][j].valeur, b) && nbCandidats(g[i][j].valeur) > 2){
                                    paire = 0;
                                    k = ibloc;
                                    while (k < ibloc+N && paire < 2){
                                        l = jbloc;
                                        while (l < jbloc+N && paire < 2){
                                            if ((k != i || l != j) && g[k][l].valeur == 0 && nbCandidats(g[k][l]) >= 2 && estCandidat(g[k][l], a) && estCandidat(g[k][l], b)){
                                                paire++;
                                                m = k;
                                                n = l;
                                            }
                                            if ((k != i || l != j) && (k != m || l != n) && g[k][l].valeur == 0 && (estCandidat(g[k][l], a) || estCandidat(g[k][l], b))){
                                                paire = 2;
                                            }
                                            l++;
                                        }
                                        k++;
                                    }

                                    if (paire == 1){

                                        k = 0;
                                        while (nbCandidats(g[i][j].valeur) > 2 && g[i][j].valeur.candidats[k] != 0){
                                            if (g[i][j].valeur.candidats[k] != a && g[i][j].valeur.candidats[k] != b){
                                                retirerCandidat(&(g[i][j].valeur), g[i][j].valeur.candidats[k]);
                                                nbCandidatsEliminer++;
                                                progression = true;
                                            }else{
                                                k++;
                                            }   
                                        }
                                        k = 0;
                                        while (nbCandidats(g[m][n]) > 2){
                                            if (g[m][n].candidats[k] != a && g[m][n].candidats[k] != b){
                                                retirerCandidat(&(g[m][n]), g[m][n].candidats[k]);
                                                nbCandidatsEliminer++;
                                                progression = true;
                                            }else{
                                                k++;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                // technique des paires cachés plus lourdes, soit pour un nombre de candidats quelconque et à partir de plus d'un tier de la grille remplie
                if (g[i][j].valeur == 0 && nbCandidats(g[i][j].valeur) > 2 && ((nbCasesRemplis*1.0/nbCasesVides)*100) > 35){

                    // bloc
                    for (int a = 1; a < TAILLE+1; a++){
                        if (estCandidat(g[i][j].valeur, a)){
                            for (int b = 1; b < TAILLE+1; b++){
                                if (a != b && estCandidat(g[i][j].valeur, b) && nbCandidats(g[i][j].valeur) > 2){
                                    paire = 0;
                                    k = ibloc;
                                    while (k < ibloc+N && paire < 2){
                                        l = jbloc;
                                        while (l < jbloc+N && paire < 2){
                                            if ((k != i || l != j) && g[k][l].valeur == 0 && nbCandidats(g[k][l]) >= 2 && estCandidat(g[k][l], a) && estCandidat(g[k][l], b)){
                                                paire++;
                                                m = k;
                                                n = l;
                                            }
                                            if ((k != i || l != j) && (k != m || l != n) && g[k][l].valeur == 0 && (estCandidat(g[k][l], a) || estCandidat(g[k][l], b))){
                                                paire = 2;
                                            }
                                            l++;
                                        }
                                        k++;
                                    }

                                    if (paire == 1){

                                        k = 0;
                                        while (nbCandidats(g[i][j].valeur) > 2 && g[i][j].valeur.candidats[k] != 0){
                                            if (g[i][j].valeur.candidats[k] != a && g[i][j].valeur.candidats[k] != b){
                                                retirerCandidat(&(g[i][j].valeur), g[i][j].valeur.candidats[k]);
                                                nbCandidatsEliminer++;
                                                progression = true;
                                            }else{
                                                k++;
                                            }   
                                        }
                                        k = 0;
                                        while (nbCandidats(g[m][n]) > 2){
                                            if (g[m][n].candidats[k] != a && g[m][n].candidats[k] != b){
                                                retirerCandidat(&(g[m][n]), g[m][n].candidats[k]);
                                                nbCandidatsEliminer++;
                                                progression = true;
                                            }else{
                                                k++;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    clock_t end = clock();
    double tmpsCPU = (end - begin)*1.0 / CLOCKS_PER_SEC;

    printf("\n");
    afficherStats(nomFichier, nbCasesRemplis, nbCasesVides, nbCandidatsEliminer, nbCandidatsAjouter);
    printf("Temps CPU = %.3f secondes\n\n",tmpsCPU);

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
    printf("%3c", ' ');
    for (i = 1; i <= TAILLE; i++){
        printf("%3d", i);

        if (i == (N*0) || i == (N*1) || i == (N*2)){
            printf("%c", ' ');
        }
    }
    printf("\n");
    for (i = 0; i < TAILLE; i++){
        if (i == (N*0) || i == (N*1) || i == (N*2)){
            printf("%3c", ' ');
            printf("+---------+---------+---------+\n");
        }
        printf("%d", i+1);
        printf("%2c", ' ');
        for (j = 0; j < TAILLE; j++){
            if (j == (N*0) || j == (N*1) || j == (N*2)){
                printf("%c", '|');
            }
            if (g[i][j].valeur == 0){
                printf("%s", " . ");
            }else{
                printf("%c%d%c", ' ',g[i][j].valeur,' ');
            }
        }
        printf("%c", '|');
        printf("\n");
    }
    printf("%3c", ' ');
    printf("+---------+---------+---------+\n\n");
}

void initialiserCases(tGrille g){
    for (int i = 0; i < TAILLE; i++){
        for (int j = 0; j < TAILLE; j++){
            g[i][j].valeur.nbCandidats = 0;
            for (int k = 0; k < TAILLE; k++){
                g[i][j].valeur.candidats[k] = 0;
            }
        }
    }
}

int initialiserCandidats(tGrille g){
    int compteur = 0;
    int ibloc, jbloc;
    bool test;

    for (int i = 0; i < TAILLE; i++){
        for (int j = 0; j < TAILLE; j++){
            if (g[i][j].valeur == 0){
                ibloc = N*(i/N);
                jbloc = N*(j/N);
                for (int x = 1; x < TAILLE+1; x++){
                    test = true;
                    for (int k = 0; k < TAILLE; k++){
                        if (x == g[k][j].valeur){
                            test = false;
                        }
                        if (x == g[i][k].valeur){
                            test = false;
                        }
                    }
                    for (int k = ibloc; k < ibloc+N; k++){
                        for (int l = jbloc; l < jbloc+N; l++){
                            if (x == g[k][l].valeur){
                                test = false;
                            }
                        }
                    }   
                    if (test && !estCandidat(g[i][j].valeur, x)){
                        ajouterCandidat(&(g[i][j].valeur), x);
                        compteur++;
                    }  
                }
            }
        }
    }

    return compteur;
}

void majCandidats(tGrille g, int *nbCandidatsEliminer){
    int ibloc, jbloc;

    for (int i = 0; i < TAILLE; i++){
        for (int j = 0; j < TAILLE; j++){
            if (g[i][j].valeur == 0){
                ibloc = N*(i/N);
                jbloc = N*(j/N);
                for (int x = 1; x < TAILLE+1; x++){
                    for (int k = 0; k < TAILLE; k++){
                        if (x == g[k][j].valeur){
                            if (estCandidat(g[i][j].valeur, x) && k != i){
                                retirerCandidat(&(g[i][j].valeur), x);
                                (*nbCandidatsEliminer)++;
                            }
                        }
                        if (x == g[i][k].valeur){
                            if (estCandidat(g[i][j].valeur, x) && k != j){
                                retirerCandidat(&(g[i][j].valeur), x);
                                (*nbCandidatsEliminer)++;
                            }
                        }
                    }
                    for (int k = ibloc; k < ibloc+N; k++){
                        for (int l = jbloc; l < jbloc+N; l++){
                            if (x == g[k][l].valeur && k != i && l != j){
                                if (estCandidat(g[i][j].valeur, x)){
                                    retirerCandidat(&(g[i][j].valeur), x);
                                    (*nbCandidatsEliminer)++;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void ajouterCandidat(tCase1 *laCase, int val){
    (*laCase).candidats[nbCandidats(*laCase)] = val; // laCase -> candidats
    (*laCase).nbCandidats++; // laCase -> nbCandidats
}

void retirerCandidat(tCase1 *laCase, int val){
    for (int i = 0; i < nbCandidats(*laCase); i++){ // laCase -> nbCandidats
        if ((*laCase).candidats[i] >= val){ // laCase -> nbCandidats
            (*laCase).candidats[i] = (*laCase).candidats[i+1]; // laCase -> nbCandidats
        }
    }
    (*laCase).nbCandidats--; // laCase -> nbCandidats
}

bool estCandidat(tCase1 laCase, int val){
    int i = 0;
    bool estC = false;

    while (i < nbCandidats(laCase) && !estC){
        if (laCase.candidats[i] == val){
            estC = true;
        }
        i++;
    }

    return estC;
}

int nbCandidats(tCase1 laCase){
    return laCase.nbCandidats;
}

void afficherStats(char nomFichier[19], int nbCasesRemplis, int nbCasesVides, int nbCandidatsEliminer, int nbCandidatsAjouter){
    printf("******%3cRESULTATS POUR %s%3c******\n\n", ' ',nomFichier,' ');
    if (nbCasesRemplis > 0){
        printf("Nombre de cases remplies = %2d sur %d     Taux de remplissage = %.3f %%\n\n", nbCasesRemplis,nbCasesVides,(nbCasesRemplis*1.0/nbCasesVides)*100);
    }else{
        printf("Nombre de cases remplies = %2d sur %d     Taux de remplissage = 0.000 %%\n\n", nbCasesRemplis,nbCasesVides);
    }
    if (nbCandidatsAjouter > 0){
        printf("Nombre de candidats elimines = %3d       Pourcentage d'élimination = %.3f %%\n\n", nbCandidatsEliminer,(nbCandidatsEliminer*1.0/nbCandidatsAjouter)*100);
    }else{
        printf("Nombre de candidats elimines = %3d       Pourcentage d'élimination = 0.000 %%\n\n", nbCandidatsEliminer);
    }
}