#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "PageRank.h"

int main() {
	system("clear");
	printf("*** PageRank ***\n");
	clearFile();
	Resultat resultat;
	Transition transition = creeMatriceTransition();

	for(double i=0.00; i<1.01; i+=0.01){
		resultat = calculeVecteur(transition, i);
		recordDampingTemps(i, resultat);
		recordDampingNbExecution(i, resultat);
	}


	// resultat = calculeVecteur(transition, 0.85);
	freeTransition(transition);

	return 0;
}