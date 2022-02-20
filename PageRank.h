#ifndef _PAGERANK_H
#define _PAGERANK_H

#include "PageRank.h"

#define bool unsigned int
#define true 1
#define false 0

#define TOLERENCE 0.00000000000000000001
// #define TOLERENCE 0.00001

#define DATA_PATH "doc/facebook_combined.txt"
#define NB_NOEUD 4039
#define NB_ARETE 88234

// #define DATA_PATH "doc/test.txt"
// #define NB_NOEUD 3
// #define NB_ARETE 4

typedef struct Matrice {
	int lig;
	int col;
	double val;
} Matrice;

typedef struct Transition {
	Matrice* m;
	int* nbVal;
} Transition;

typedef struct Resultat {
	double temps;
	int nbExecution;
} Resultat;

typedef struct VecteurSort {
	double val;
	int indice;
} VecteurSort;

Transition creeMatriceTransition();
Transition mettreLaDivision(Transition t);
bool checkExiste(Transition t, int var);
Transition initTransition();
void freeTransition(Transition t);
Transition addElem(Transition t, int ligne, int colonne, int indiceMatrice, int indiceNbVal);
Transition addElemAlreadyExiste(Transition t, int ligne, int colonne, int indice);
void afficherMatrice(Transition t);
void afficherNbValeur(Transition t);
Resultat calculeVecteur(Transition t, double dumping);
double* multiplicationMatriceVecteur(Transition t, double* vecteur, double* vecteurResult);
double* multiplicationVecteurConstante(Transition t, double* vecteur, double var);
double* additionVecteur(double* a, double* b);
double* copieVecteur(double* a, double* b);
double* resetVecteur(double* vecteur);
bool stop(double* a, double* b);

double* initVecteur(int version, int dumping);
void afficherVecteur(double* vecteurResult);
VecteurSort* sortVecteur(double* v);
VecteurSort* swapVecteurSort(VecteurSort* vs, int a, int b);
void afficherVecteurSort(VecteurSort* vs);

void clearFile();
void recordDampingTemps(double dumping, Resultat result);
void recordDampingNbExecution(double dumping, Resultat result);
void recordClassementPage(VecteurSort* vs, double dumping);

#endif