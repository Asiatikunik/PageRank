#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

#include "PageRank.h"

Transition creeMatriceTransition() {
	printf("Lecture du fichier\n");
	FILE * fp;
    char * chaine = NULL;
    char* token = NULL;
    int ligne, colonne;
    Transition trans = initTransition();
    int indiceNbVal = 0, indiceMatrice = 0;
    size_t len = 0;
    size_t read;

    fp = fopen(DATA_PATH, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&chaine, &len, fp)) != -1) {
		token = strtok(chaine, " ");
		ligne = atoi(token);
		token = strtok(NULL, " ");
		colonne = atoi(token);

		if(checkExiste(trans, ligne) && indiceMatrice != 0) {
			trans = addElemAlreadyExiste(trans, ligne, colonne, indiceMatrice);
		} else {
			trans = addElem(trans, ligne, colonne, indiceMatrice, indiceNbVal);
			indiceNbVal++;
		}
		indiceMatrice++;
    }
    if (chaine)
        free(chaine);


    trans = mettreLaDivision(trans);
    printf("Lecture du fichier done\n\n");
	return trans;
}

Transition mettreLaDivision(Transition t) {
	int ligne;
	for(int s=0; s<NB_ARETE; s++) {
		ligne = t.m[s].lig;
		if (t.nbVal[ligne] != 0 && t.nbVal[ligne] != 1)
			t.m[s].val = t.m[s].val / t.nbVal[ligne];
	}

	return t;
}

bool checkExiste(Transition t, int var) {
	for(int s=0; s<NB_ARETE; s++) {
		if(t.m[s].lig == var) {
			return true;
		}
	}
	return false;
}

Transition initTransition() {
	Transition trans;
	trans.m = (Matrice*) calloc(NB_ARETE, sizeof(Matrice));
	trans.nbVal = (int*) calloc(NB_NOEUD, sizeof(int));
	return trans;
}

void freeTransition(Transition t) {
	free(t.m);
	free(t.nbVal);
}

Transition addElem(Transition t, int ligne, int colonne, int indiceMatrice, int indiceNbVal) {
    t.m[indiceMatrice].lig = ligne;
    t.m[indiceMatrice].col = colonne;
    t.m[indiceMatrice].val = 1;
    t.nbVal[indiceNbVal]++;
    return t;
}

Transition addElemAlreadyExiste(Transition t, int ligne, int colonne, int indice) {
    t.m[indice].lig = ligne;
    t.m[indice].col = colonne;
    t.m[indice].val++;
    t.nbVal[ligne]++;
    return t;
}

void afficherMatrice(Transition t) {
	printf("    Lig:   Col:  Val:\n");
	for(int s=0; s<NB_ARETE; s++) {
		printf("%d: %3d    %3d     %3.10f\n", s, t.m[s].lig, t.m[s].col, t.m[s].val);
	}
	printf("\n");
}

void afficherNbValeur(Transition t) {
	printf("Nombre de valeur \n");
	for(int s=0; s<NB_NOEUD; s++)
		printf("%d ", s);
	printf("\n");

	for(int s=0; s<NB_NOEUD; s++)
		printf("%d ", t.nbVal[s]);
	printf("\n\n");
}

Resultat calculeVecteur(Transition t, double dumping) {
	double* vecteurResult = initVecteur(0, dumping);
	double* vecteur = initVecteur(1, dumping);
	double* vecteurGout = initVecteur(2, dumping);
	double* vecteurPrec = initVecteur(0, dumping);
	bool flag = false;
	int iteration = 0;
	double temps;
	clock_t t1, t2;
	Resultat returned;
	VecteurSort* vs = NULL;

	t1 = clock();
	while(!flag) {
		t1 = clock();
		// Partie de gauche
		vecteurResult = multiplicationMatriceVecteur(t, vecteur, vecteurResult);
		vecteurResult = multiplicationVecteurConstante(t, vecteurResult, dumping);

		// Addition du tout
		vecteurResult = additionVecteur(vecteurResult, vecteurGout);

		if (iteration != 0)
			flag = stop(vecteurPrec, vecteurResult);

		vecteurPrec = copieVecteur(vecteurPrec, vecteurResult);
		vecteur = copieVecteur(vecteur, vecteurResult);

		iteration++;
		vecteurResult = resetVecteur(vecteurResult);
	}

	t2 = clock();
	temps = (double) (t2-t1)/CLOCKS_PER_SEC;
	printf("Dumping: %1.2f\n", dumping);
    printf("Temps: %f\n", temps);
	printf("Il y a eu %d itération\n\n", iteration);
	returned.temps = temps;
	returned.nbExecution = iteration;

	vs = sortVecteur(vecteur);
	recordClassementPage(vs, dumping);

	free(vecteurResult);
	free(vecteur);
	free(vecteurGout);
	free(vecteurPrec);
	free(vs);
	return returned;
}

double* multiplicationMatriceVecteur(Transition t, double* vecteur, double* vecteurResult) {
	for(int i=0; i<NB_ARETE; i++) {
		vecteurResult[t.m[i].col] += (t.m[i].val * vecteur[t.m[i].lig]);
	}
	return vecteurResult;
}

double* multiplicationVecteurConstante(Transition t, double* vecteur, double var) {
	for(int i=0; i<NB_NOEUD; i++) {
		vecteur[i] *= var;
	}
	return vecteur;
}

double* additionVecteur(double* a, double* b) {
	for(int i=0; i<NB_NOEUD; i++) {
		a[i] += b[i];
	}
	return a;
}

double* copieVecteur(double* a, double* b) {
	for(int i=0; i<NB_NOEUD; i++) {
		a[i] = b[i];
	}
	return a;
}

double* resetVecteur(double* vecteur) {
	for(int i=0; i<NB_NOEUD; i++) {
		vecteur[i] = 0;
	}
	return vecteur;
}

bool stop(double* a, double* b) {
	double result = 0;
	double tmp = 0;
	double puissance = 2.0;

	for(int i=0; i<NB_NOEUD; i++) {
		tmp = fabs(a[i]-b[i]);
		tmp = pow(tmp, puissance);
		result += tmp;
	}
	result = sqrt(result);
	if(result < TOLERENCE) {
		return true;
	}
	return false;
}

double* initVecteur(int version, int dumping) { 
	double* vecteur = (double*) calloc(NB_NOEUD, sizeof(double));
	double jumping = 1-dumping;

	if(version == 1) {
		for(int i=0; i<NB_NOEUD; i++) {
			vecteur[i] = 1;
			vecteur[i] = vecteur[i] / NB_NOEUD;
		}
	} else if (version == 2) {
		for(int i=0; i<NB_NOEUD; i++) {
			vecteur[i] = 1;
			vecteur[i] = vecteur[i] / NB_NOEUD;
			vecteur[i] *= jumping;
		}
	}
	return vecteur;
}

void afficherVecteur(double* vecteurResult){
	// printf("\nvecteurResult: \n");
	// for(int s=0; s<NB_NOEUD; s++)
	// 	printf("%d     ", s);
	// printf("\n");
	for(int i=0; i<NB_NOEUD; i++) {
		printf("%1.10f ", vecteurResult[i]);
	}
	printf("\n\n");
}

VecteurSort* sortVecteur(double* v) {
	VecteurSort* vs = (VecteurSort*) calloc(NB_NOEUD, sizeof(VecteurSort));
	bool flag = false;

	for(int i=0; i<NB_NOEUD; i++) {
		vs[i].val = v[i];
		vs[i].indice = i;
	}

	while(!flag) {
		flag = true;
		for(int i=0; i<NB_NOEUD-1; i++) {
			if (vs[i].val < vs[i+1].val) {
				vs = swapVecteurSort(vs, i, i+1);
				flag = false;
			}
		}
	}

	return vs;
}

VecteurSort* swapVecteurSort(VecteurSort* vs, int a, int b) {
	VecteurSort tmp;

	tmp.indice = vs[a].indice;
	tmp.val = vs[a].val;

	vs[a].indice = vs[b].indice;
	vs[a].val = vs[b].val;

	vs[b].indice = tmp.indice;
	vs[b].val = tmp.val;

	return vs;
}

void afficherVecteurSort(VecteurSort* vs) {
	for(int i=0; i<NB_NOEUD; i++) {
		printf("%d %1.3f\n", vs[i].indice, vs[i].val);
	}
	printf("\n\n");
}


void clearFile(){

	FILE *file = NULL;

	if (file = fopen("resultat/damping_temps.txt", "r")) {
	    fclose(file);
		system("rm resultat/damping_temps.txt");
	}
	if (file = fopen("resultat/damping_NbExecution.txt", "r")) {
	    fclose(file);
		system("rm resultat/damping_NbExecution.txt");
	}
	if (file = fopen("resultat/classementPage.txt", "r")) {
	    fclose(file);
		system("rm resultat/classementPage.txt");
	}
}

void recordDampingTemps(double dumping, Resultat result) {
    FILE *f = NULL;
    f = fopen("resultat/damping_temps.txt", "a");
    if (f==NULL) {
        printf("Error open file for record\n");
        exit(1);
    }
    fprintf(f, "%1.2f %f\n", dumping, (double) result.temps);
    fclose(f);
}

void recordDampingNbExecution(double dumping, Resultat result) {
    FILE *f = NULL;
    f = fopen("resultat/damping_NbExecution.txt", "a");
    if (f==NULL) {
        printf("Error open file for record\n");
        exit(1);
    }
    fprintf(f, "%1.2f %d\n", dumping, (int) result.nbExecution);
    fclose(f);
}

void recordClassementPage(VecteurSort* vs, double dumping) {
    FILE *f = NULL;
    f = fopen("resultat/classementPage.txt", "a");
    if (f==NULL) {
        printf("Error open file for record\n");
        exit(1);
    }

    fprintf(f, "Avec %1.2f dumping\n", dumping);
    for(int i=0; i<NB_NOEUD; i++) {
		fprintf(f, "%d %1.10f\n", vs[i].indice, vs[i].val);
    }
    fprintf(f, "\n\n");
    fclose(f);
}
