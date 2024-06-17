#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define N 4
#define TAILLE (N*N)

typedef struct {
    int valeur;
    int candidats[TAILLE];
    int nbCandidats;
} tCase1;

typedef tCase1 tGrille[TAILLE][TAILLE];

int chargerGrille(tGrille g, char nomFichier[19]);
void afficherGrille(tGrille g);

int initialiserCandidats(tGrille g);
void ajouterCandidat(tCase1 *laCase, int val);
void retirerCandidat(tCase1 *laCase, int val);
bool estCandidat(tCase1 laCase, int val);
void singletonNu(tGrille g, int lig, int col, int *nbCasesVides, bool *progression);
void singletonCache(tGrille g, int lig, int col, int *nbCasesVides, bool *progression);
void majCandidats(tGrille g, int lig, int col, int val);

bool backtracking(tGrille g, int numeroCase);
bool absentSurLigne(int valeur, tGrille g, int ligne);
bool absentSurColonne(int valeur, tGrille g, int colonne);
bool absentSurBloc(int valeur, tGrille g, int ligne, int colonne);

int main(){
    tGrille g;

    int nbCasesVides;

    bool progression;
    char nomFichier[19];

    progression = true;

    nbCasesVides = chargerGrille(g, nomFichier);
    printf("\nGrille initiale");
    afficherGrille(g);

    clock_t begin = clock(); // début chrono

    initialiserCandidats(g);

    while (nbCasesVides > 0 && progression){
        progression = false;
        for(int i = 0; i < TAILLE; i++){
            for(int j = 0; j < TAILLE; j++){
                
                // technique du singleton nu
                if (g[i][j].valeur == 0 && g[i][j].nbCandidats == 1){
                    singletonNu(g, i, j, &nbCasesVides, &progression);
                }

                // technique du singleton caché
                if (g[i][j].valeur == 0){
                    singletonCache(g, i, j, &nbCasesVides, &progression);
                }
            }
        }
    }

    if (nbCasesVides > 0){
        backtracking(g, 0);
    }

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
            if (N == 3){
                printf("+---------+---------+---------+\n");
            }
            if (N == 4){
                printf("+------------+------------+------------+------------+\n");
            }
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
    if (N == 3){
        printf("+---------+---------+---------+\n");
    }
    if (N == 4){
        printf("+------------+------------+------------+------------+\n");
    }
}

void singletonNu(tGrille g, int i, int j, int *nbCasesVides, bool *progression){
    g[i][j].valeur = g[i][j].candidats[0];
    nbCasesVides--;
    *progression = true;
    majCandidats(g, i, j, g[i][j].candidats[0]);
}

void singletonCache(tGrille g, int i, int j, int *nbCasesVides, bool *progression){
    for (int x = 1; x < TAILLE+1; x++){

        // bloc
        int seule = 0;
        int ibloc = N*(i/N), jbloc = N*(j/N);;
        int k = ibloc;
        while (k < ibloc+N && seule < 2 && g[i][j].valeur == 0){
            int l = jbloc;
            while (l < jbloc+N && seule < 2 && g[i][j].valeur == 0){
                if (g[k][l].valeur == 0 && estCandidat(g[k][l], x)){
                    seule++;
                }
                l++;
            }
            k++;
        }
        if (seule == 1 && estCandidat(g[i][j], x)){
            g[i][j].valeur = x;
            nbCasesVides--;
            *progression = true;
            majCandidats(g, i, j, x);
        }

        // ligne
        seule = 0;
        k = 0;
        while (k < TAILLE && seule < 2 && g[i][j].valeur == 0){
            if (g[i][k].valeur == 0 && estCandidat(g[i][k], x)){
                seule++;
            }
            k++;
        }
        if (seule == 1 && estCandidat(g[i][j], x)){
            g[i][j].valeur = x;
            nbCasesVides--;
            *progression = true;
            majCandidats(g, i, j, x);
        }

        // colonne
        seule = 0;
        k = 0;
        while (k < TAILLE && seule < 2 && g[i][j].valeur == 0){
            if (g[k][j].valeur == 0 && estCandidat(g[k][j], x)){
                seule++;
            }
            k++;
        }
        if (seule == 1 && estCandidat(g[i][j], x)){
            g[i][j].valeur = x;
            nbCasesVides--;
            *progression = true;
            majCandidats(g, i, j, x);
        }
    }
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
        if (g[ligne][colonne].valeur != 0) {
            resultat = backtracking(g, numeroCase+1);
        }
        else{
            for(int i = 0; i < g[ligne][colonne].nbCandidats; i++){
                if (estCandidat(g[ligne][colonne], g[ligne][colonne].candidats[i]) && absentSurLigne(g[ligne][colonne].candidats[i], g, ligne) && absentSurColonne(g[ligne][colonne].candidats[i], g, colonne) && absentSurBloc(g[ligne][colonne].candidats[i], g, ligne, colonne)){
                    g[ligne][colonne].valeur = g[ligne][colonne].candidats[i];
                    if (backtracking(g, numeroCase+1)){
                        resultat = true;
                    }else{
                        g[ligne][colonne].valeur = 0;
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
        if (g[ligne][j].valeur == valeur){
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
        if (g[i][colonne].valeur == valeur){
            resultat = false;
        }
        i++;
    }
    
    return resultat;
}

bool absentSurBloc(int valeur, tGrille g, int ligne, int colonne){
    bool resultat = true;
    int ligbloc = N*(ligne/N), colbloc = N*(colonne/N);
    int i = ligbloc;
    while (i < ligbloc+N && resultat == true){ 
        int j = colbloc;
        while (j < colbloc+N && resultat == true){
            if (g[i][j].valeur == valeur){
                resultat = false;
            }
            j++;
        }
        i++;
    }
    
    return resultat;
}

int initialiserCandidats(tGrille g){
    int compteur = 0;

    for (int i = 0; i < TAILLE; i++){
        for (int j = 0; j < TAILLE; j++){
            if (g[i][j].valeur == 0){
                g[i][j].nbCandidats = 0;
                for (int x = 1; x < TAILLE+1; x++){
                    bool test = true;
                    int k = 0;
                    while (test && k < TAILLE){
                        if (x == g[k][j].valeur){
                            test = false;
                        }
                        if (x == g[i][k].valeur){
                            test = false;
                        }
                        k++;
                    }
                    int ibloc = N*(i/N), jbloc = N*(j/N);
                    k = ibloc;
                    while (test && k < ibloc+N){
                        int l = jbloc;
                        while (test && l < jbloc+N){
                            if (x == g[k][l].valeur){
                                test = false;
                            }
                            l++;
                        }
                        k++;
                    }
                    if (test){
                        ajouterCandidat(&g[i][j], x);
                        compteur++;
                    }  
                }
            }
        }
    }

    return compteur;
}

void majCandidats(tGrille g, int lig, int col, int val){
    for (int i = 0; i < TAILLE; i++){
        if (estCandidat(g[lig][i], val)){
            retirerCandidat(&g[lig][i], val);
        }

        if (estCandidat(g[i][col], val)){
            retirerCandidat(&g[i][col], val);
        }
    }

    int ibloc = N*(lig/N), jbloc = N*(col/N);
    for (int i = ibloc; i < ibloc+N; i++){
        for (int j = jbloc; j < jbloc+N; j++){
            if (estCandidat(g[i][j], val)){
                retirerCandidat(&g[i][j], val);
            }
        }
    }
}

void ajouterCandidat(tCase1 *laCase, int val){
    (*laCase).candidats[(*laCase).nbCandidats] = val; // laCase -> candidats
    (*laCase).nbCandidats++; // laCase -> nbCandidats
}

void retirerCandidat(tCase1 *laCase, int val){
    for (int i = 0; i < (*laCase).nbCandidats; i++){ // laCase -> nbCandidats
        if ((*laCase).candidats[i] >= val){ // laCase -> nbCandidats
            (*laCase).candidats[i] = (*laCase).candidats[i+1]; // laCase -> nbCandidats
        }
    }
    (*laCase).nbCandidats--; // laCase -> nbCandidats
}

bool estCandidat(tCase1 laCase, int val){
    int i = 0;
    bool estC = false;

    while (i < laCase.nbCandidats && !estC){
        if (laCase.candidats[i] == val){
            estC = true;
        }
        i++;
    }

    return estC;
}