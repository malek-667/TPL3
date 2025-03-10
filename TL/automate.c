#include <stdlib.h>
#include <stdio.h>


typedef struct s_liste {
        int state;	// Nom de l'etat
        struct s_liste* nxt;	// Etat suivant
} liste;

typedef struct {
         int size;
         int sizeAlpha;
         int* initial;
         int* final;
         liste*** trans;
} automate;

typedef struct {
	int nb_summit;
	liste** neighbours;
} graphe;

typedef struct s_iliste {
         int* val;
         int tailleVal;
         int state;
         struct s_iliste* nxt;
} iliste;

typedef struct {
         iliste* debut;
         iliste* fin;
} ifile;

/* Fonction qui permet de savoir si
un état est déja dans la file
(avec le tableau d'état correspondant 
passé en paramètre ) retourne 1
si il est dans la file, 0 sinon*/
int estDansFile(ifile f, int* pt, int n){
	int i;
	iliste* tmp;
	tmp = f.debut;
	while(tmp){
		if(tmp->tailleVal == n){
			i = 0;
			while(i<n && pt[i] == tmp->val[i]){
				i++;
			}
			if(i == n){
				return 1;
			}
		}
		tmp = tmp->nxt;
	}
	return 0;
}

/* Fonction qui permet de savoir si
un état est déja dans la file et de retourner 
son numéro */
int numEtatDansFile(ifile f, int* pt, int n){
	int i;
	iliste* tmp;
	tmp = f.debut;
	while(tmp){
		if(tmp->tailleVal == n){
			i = 0;
			while(i<n && pt[i] == tmp->val[i]){
				i++;
			}
			if(i == n){
				return tmp->state;
			}
		}
		tmp = tmp->nxt;
	}
	return -1;

}

/*Fonction qui permet d'ajouter un élément 
à la file sans doublon */
void ajouteFile(ifile* f, int* pt, int n){
	int i;
	iliste* tmp;
	tmp = (iliste*) malloc (sizeof(iliste));
	tmp->val = (int*) malloc (n*sizeof(int));
	for(i=0; i<n; i++){
		tmp->val[i] = pt[i];
	}
	tmp->nxt = NULL;
	tmp->tailleVal = n;
	if(f->fin != NULL){
		tmp->state = f->fin->state + 1;
		f->fin->nxt = tmp;
		f->fin = tmp;

		return;
	}
	tmp->state = 0;
	f->fin = tmp;
	f->debut = tmp;
}

/* Fonction qui permet d'afficher la file */
void afficheFile(ifile f){
	iliste* pt;
	int i;
	int j = 0;
	pt = f.debut;
	while(pt){
		printf("-----------------------------\n");
		printf("Element %d\n", pt->state);
		for(i=0; i<pt->tailleVal; i++){
			printf(" %d", pt->val[i]);
		}
	
		pt = pt->nxt;
		j++;
		printf("\n");
	}
}

/*Fonction qui permet de supprimer une file
sans qu'il y est de fuite mémoire */
void libereFile(ifile* F){
	iliste* tmp = NULL;

	// Libère tous les éléments de la file
	while(F->debut != NULL){
		tmp = F->debut;
		F->debut = F->debut->nxt;
		free(tmp->val);
		free(tmp);
		tmp = NULL;
	}

	// Libère la file
	free(F);
	F = NULL;
}


/////////////////


// Fonction qui ajouter un element à une liste
void ajouteListe(liste** l,int q){
	liste* ptl;
	liste* tmp;
	ptl=*l;
	if(!ptl){
		ptl=(liste*) malloc(sizeof(liste));
		ptl->state=q;
		ptl->nxt=NULL;
		*l=ptl;
		return;
	}
	if(ptl->state == q){
		return;
	}
	if(q< ptl->state){
		tmp=*l;
		*l=(liste*) malloc(sizeof(liste));
		(*l)->state=q;
		(*l)->nxt=tmp;
		return;
	}
	while(ptl->nxt && ptl->nxt->state <q){
		ptl=ptl->nxt;
	}
	if(!ptl->nxt){
		ptl->nxt=(liste*) malloc(sizeof(liste));
		ptl=ptl->nxt;
		ptl->state=q;
		ptl->nxt=NULL;
		return;
	}
	if(ptl->nxt->state==q){
		return;
	}
	tmp=ptl->nxt;
	ptl->nxt=(liste*) malloc(sizeof(liste));
	ptl=ptl->nxt;
	ptl->state=q;
	ptl->nxt=tmp;
}

// Fonction qui ajoute une transition qui part de src vers targ, avec lettre alpha
void ajouteTransition(automate* au, int src, int targ, char alpha){
	if (src >= au->size || src < 0 || targ >= au->size || targ < 0 || ((int)(alpha - 'a') >= au->sizeAlpha)){
		printf("L'Etat ou la lettre n'existe pas. \n");
		return;
	}
	ajouteListe(&(au->trans[src][(int)(alpha - 97)]), targ);	
}

// Construit un automate exemple 
void construitAutomateExemple(automate* au) {
	int i,j,k;
	int targ;
	char tran;

	// Tailles générales de l'automates
	au->size = 5;
	au->sizeAlpha = 2;

	// Etats initiaux
	au->initial = (int*) malloc(au->size*sizeof(int));
	/*
	for(i=0; i<au->size; i++) {
		printf("Etat:%d, Initial? Oui:1 | Non:0\n", i);
		scanf ("%d",&k);
		au->initial[i] = k;
	}//*/
	//*
	au->initial[0]=1;
	au->initial[1]=1;
	au->initial[2]=0;
	au->initial[3]=0;
	au->initial[4]=0;//*/

	// Etats finaux
	au->final = (int*) malloc(au->size*sizeof(int));
	/*
	for(i=0; i<au->size; i++) {
		printf("Etat:%d, Final? Oui:1 | Non:0\n", i);
		scanf ("%d",&k);
		au->initial[i] = k;
	}//*/
	//*
	au->final[0]=0;
	au->final[1]=0;
	au->final[2]=0;
	au->final[3]=0;
	au->final[4]=1; //*/

	// Creation/initialisations de transitions vides
	au->trans = (liste***) malloc(au->size*sizeof(liste***));
	for(i=0;i<au->size;i++){
		au->trans[i]=(liste**) malloc(au->sizeAlpha*sizeof(liste*));
		for(k=0;k<au->sizeAlpha;k++){
			au->trans[i][k]=NULL;
		}
	}

	/*
	for(i=0;i<au->size;i++) {
		printf("Etat: %d | ", i);
		printf("Combien de transitions voulez vous ajouter\n");
		scanf ("%d",&k);
		for(j=0; j<k; j++) {
			printf("\nEtat cible ? (de 0 à %d): ", au->size-1);
			scanf ("%d",&targ);
			printf("\nPar transition ? (a, ..., %c /!\\ à la valeur !)", au->sizeAlpha+96);	// 97:a; -1 pour la taille
			scanf ("%s",&tran);
			printf("%c\n", tran);
			ajouteTransition(au,i,targ,tran);
		}
	}//*/
	//*
	ajouteTransition(au,0,1,'a');
	ajouteTransition(au,0,2,'a');
	ajouteTransition(au,0,3,'a');

	ajouteTransition(au,1,3,'b');

	ajouteTransition(au,2,3,'a');
	ajouteTransition(au,2,4,'b');

	ajouteTransition(au,3,3,'b');
	ajouteTransition(au,3,4,'b');

	ajouteTransition(au,4,4,'a');//*/


}

// Construit l'automate exemple 1 pour le produit d'automate
void construitAutomateExempleProd1(automate* au) {
	int i,j,k;
	int targ;
	char tran;

	// Tailles générales de l'automates
	au->size = 2;
	au->sizeAlpha = 2;

	// Etats initiaux
	au->initial = (int*) malloc(au->size*sizeof(int));
	au->initial[0]=1;
	au->initial[1]=0;


	// Etats finaux
	au->final = (int*) malloc(au->size*sizeof(int));
	au->final[0]=0;
	au->final[1]=1;

	// Creation/initialisations de transitions vides
	au->trans = (liste***) malloc(au->size*sizeof(liste***));
	for(i=0;i<au->size;i++){
		au->trans[i]=(liste**) malloc(au->sizeAlpha*sizeof(liste*));
		for(k=0;k<au->sizeAlpha;k++){
			au->trans[i][k]=NULL;
		}
	}
	
	ajouteTransition(au,0,0,'a');
	ajouteTransition(au,0,1,'b');

	ajouteTransition(au,1,1,'a');



}//*/

// Construit l'automate exemple 2 pour le produit d'automate 
void construitAutomateExempleProd2(automate* au) {
	int i,j,k;
	int targ;
	char tran;

	// Tailles générales de l'automates
	au->size = 3;
	au->sizeAlpha = 2;

	// Etats initiaux
	au->initial = (int*) malloc(au->size*sizeof(int));
	au->initial[0]=1;
	au->initial[1]=0;
	au->initial[2]=0;


	// Etats finaux
	au->final = (int*) malloc(au->size*sizeof(int));
	au->final[0]=0;
	au->final[1]=0;
	au->final[2]=1;

	// Creation/initialisations de transitions vides
	au->trans = (liste***) malloc(au->size*sizeof(liste***));
	for(i=0;i<au->size;i++){
		au->trans[i]=(liste**) malloc(au->sizeAlpha*sizeof(liste*));
		for(k=0;k<au->sizeAlpha;k++){
			au->trans[i][k]=NULL;
		}
	}
	
	ajouteTransition(au,0,0,'b');
	ajouteTransition(au,0,1,'a');

	ajouteTransition(au,1,1,'b');
	ajouteTransition(au,1,2,'b');

	ajouteTransition(au,2,2,'b');
}//*/


// Fonction qui construit l'automate exemple pour la derterminisation
void construitAutomateExempleDeterm(automate* au) {
	int i,j,k;
	int targ;
	char tran;

	// Tailles générales de l'automates
	au->size = 3;
	au->sizeAlpha = 2;

	// Etats initiaux
	au->initial = (int*) malloc(au->size*sizeof(int));
	au->initial[0]=1;
	au->initial[1]=0;
	au->initial[2]=0;


	// Etats finaux
	au->final = (int*) malloc(au->size*sizeof(int));
	au->final[0]=0;
	au->final[1]=0;
	au->final[2]=1;

	// Creation/initialisations de transitions vides
	au->trans = (liste***) malloc(au->size*sizeof(liste***));
	for(i=0;i<au->size;i++){
		au->trans[i]=(liste**) malloc(au->sizeAlpha*sizeof(liste*));
		for(k=0;k<au->sizeAlpha;k++){
			au->trans[i][k]=NULL;
		}
	}

	ajouteTransition(au,0,0,'a');
	ajouteTransition(au,0,0,'b');
	ajouteTransition(au,0,1,'a');

	ajouteTransition(au,1,2,'a');
	ajouteTransition(au,1,2,'b');

}


void construitAutomateExempleInclusion(automate* au1, automate* au2) {
	int i,j,k;
	int targ;
	char tran;

	// Tailles générales de l'automates
	au1->size = 4;
	au1->sizeAlpha = 2;

	// Etats initiaux
	au1->initial = (int*) malloc(au1->size*sizeof(int));
	au1->initial[0]=1;
	au1->initial[1]=0;
	au1->initial[2]=0;
	au1->initial[3]=0;


	// Etats finaux
	au1->final = (int*) malloc(au1->size*sizeof(int));
	au1->final[0]=0;
	au1->final[1]=0;
	au1->final[2]=1;
	au1->final[3]=1;

	// Creation/initialisations de transitions vides
	au1->trans = (liste***) malloc(au1->size*sizeof(liste***));
	for(i=0;i<au1->size;i++){
		au1->trans[i]=(liste**) malloc(au1->sizeAlpha*sizeof(liste*));
		for(k=0;k<au1->sizeAlpha;k++){
			au1->trans[i][k]=NULL;
		}
	}

	ajouteTransition(au1,0,1,'a');
	ajouteTransition(au1,0,2,'a');

	ajouteTransition(au1,1,3,'b');


	// Tailles générales de l'automates
	au2->size = 3;
	au2->sizeAlpha = 2;

	// Etats initiaux
	au2->initial = (int*) malloc(au2->size*sizeof(int));
	au2->initial[0]=1;
	au2->initial[1]=0;
	au2->initial[2]=0;


	// Etats finaux
	au2->final = (int*) malloc(au2->size*sizeof(int));
	au2->final[0]=0;
	au2->final[1]=0;
	au2->final[2]=1;

	// Creation/initialisations de transitions vides
	au2->trans = (liste***) malloc(au2->size*sizeof(liste***));
	for(i=0;i<au2->size;i++){
		au2->trans[i]=(liste**) malloc(au2->sizeAlpha*sizeof(liste*));
		for(k=0;k<au2->sizeAlpha;k++){
			au2->trans[i][k]=NULL;
		}
	}

	ajouteTransition(au2,0,1,'a');

	ajouteTransition(au2,1,2,'b');

	ajouteTransition(au2,2,1,'b');
}

void construitAutomateExempleMinimization(automate* au) {
	int i,j,k;
	int targ;
	char tran;

	// Tailles générales de l'automates
	au->size = 6;
	au->sizeAlpha = 2;

	// Etats initiaux
	au->initial = (int*) malloc(au->size*sizeof(int));
	au->initial[0]=1;
	au->initial[1]=0;
	au->initial[2]=0;
	au->initial[3]=0;
	au->initial[4]=0;
	au->initial[5]=0;



	// Etats finaux
	au->final = (int*) malloc(au->size*sizeof(int));
	au->final[0]=1;
	au->final[1]=0;
	au->final[2]=0;
	au->final[3]=0;
	au->final[4]=0;
	au->final[5]=1;

	// Creation/initialisations de transitions vides
	au->trans = (liste***) malloc(au->size*sizeof(liste***));
	for(i=0;i<au->size;i++){
		au->trans[i]=(liste**) malloc(au->sizeAlpha*sizeof(liste*));
		for(k=0;k<au->sizeAlpha;k++){
			au->trans[i][k]=NULL;
		}
	}

	ajouteTransition(au,0,5,'a');
	ajouteTransition(au,0,1,'b');

	ajouteTransition(au,1,2,'b');
	ajouteTransition(au,1,4,'a');

	ajouteTransition(au,2,2,'a');
	ajouteTransition(au,2,1,'b');

	ajouteTransition(au,3,3,'a');
	ajouteTransition(au,3,5,'b');

	ajouteTransition(au,4,1,'a');
	ajouteTransition(au,4,3,'b');

	ajouteTransition(au,5,5,'a');
	ajouteTransition(au,5,4,'b');
}


// Affiche l'automate passé en paramètre
void afficheAutomate(automate au){
	int i,j;
	unsigned char c;

// affichage des états initiaux
	printf("\nLes etats initiaux\n");
	
	for (i=0; i<au.size; i++){
		if (au.initial[i] == 1){
			printf(" %d",i);
		}
	}

// affichage des états finaux
	printf("\nLes etats finaux\n");
	for (i=0; i<au.size; i++){
		if(au.final[i] == 1){
			printf(" %d",i);
		}
	}

// affichage des transitions
	printf("\nLes Transitions\n");
	
	for (i=0; i<au.size; i++){
	printf("------------------------------\n");
		printf("\nDepuis l'etat %d",i);
		for (j=0; j<au.sizeAlpha; j++){
			printf("\navec la lettre %c :\n", c = (unsigned char) j+97);	
			liste* tmp;
			tmp = au.trans[i][j];

			while (tmp != NULL){
				printf(" %d",tmp->state);
				tmp = tmp->nxt;
			}
			printf("\n");
		}
	}
	printf("\n");
	
}

// renvoie le nombre de transition de l'automate mis en paramètre
int compteTransitions(automate au){
	int nb=0;
	int i,j;
	
	// nb est le nombre de transitions
	for (i=0; i<au.size; i++){
		for (j=0; j<au.sizeAlpha; j++){
			liste* tmp;
			tmp = au.trans[i][j];

			while (tmp != NULL){
				nb++;
				tmp = tmp->nxt;
			}
		}
	}

	return nb;
}

// Fonction qui renvoie 1 si l'automate est déterministe, 0 si il ne l'est pas
int deterministe(automate au) {
	int i,j;
	int ref = 1;
	int state = 0;

	// On regarde si il n'y a qu'un seul etat initial
	while(state < au.size) {
		if(au.initial[state] ==1) {
			// Au premier état on met la reference a 0. Si il y a un
			// deuxieme etat on retournera directement 0
			if(ref) {
				ref = 0;
			} else {
				return 0;
			}
		}
		state++;
	}
	// On a bien un seul etat initial, on remet la ref a 1
	ref = 1;

	// On regarde si il y a au max une transition par lettre
	for(i=0; i<au.size;i++) {
		for(j=0; j<au.sizeAlpha; j++) {
			// Si pour une lettre on en a UNE seule (il faut qu'elle existe)
			if(au.trans[i][j] != NULL && au.trans[i][j]->nxt != NULL) {
				ref = 0;
			}
		}
	}
	return ref;
}

// Fonction qui renvoie 1 si l'automate mis en paramètre est complet, 0 sinon
int complet(automate au) {
	int i,j;
	int nb =0;

	// On regarde si toute les lettres sont utilisees pour chaques etats
	for(i=0; i<au.size;i++) {
		for(j=0; j<au.sizeAlpha; j++) {
			if(au.trans[i][j] != NULL) {
				nb++;
			}
		}
		// On compte les transitions par lettres, si leurs nombres
		// est different de sizeAlpha, l'automate n'est pas complet
		if(nb != au.sizeAlpha) {
			return 0;
		}
		nb = 0;
	}

	return 1;
}

// Fonction qui supprime une transition de src vers targ et de lettre alpha
void supprimeTransition(automate* au, int src, int targ, char alpha) {
	liste* tmpTrans;
	liste* lastTmp;
	lastTmp = NULL;

	if (src >= au->size || src < 0 || targ >= au->size || targ < 0 || ((int)(alpha - 'a') >= au->sizeAlpha)){
		printf("L'Etat ou la lettre n'existe pas. \n");
		return;
	}

	tmpTrans = au->trans[src][(int) (alpha - 'a')];
	// Si la transition demandee n'existe pas on sort
	if(tmpTrans == NULL) {
		return;
	}

	// On recherche la transition cible. lastTmp est la transition precedent
	// la transition actuelle. On sort de la boucle quand on a notre etat
	while(tmpTrans != NULL && tmpTrans->state != targ) {
		lastTmp = tmpTrans;
		tmpTrans = tmpTrans->nxt;
	}

	if(tmpTrans == NULL) {
		return;
	}

	// Si la transition n'existe pas (notre cible est la premiere)
	// On met la transition suivant en premier de la liste des transi
	// Sinon on enleve notre element l'element suivant de la transition
	// precedante devient l'element suivant de la transtion  courantesupprimee
	// et on libere l'element courant
	if(lastTmp == NULL) {
		au->trans[src][(int) (alpha - 'a')] = tmpTrans->nxt;
	} else {
		lastTmp->nxt = tmpTrans->nxt;
	}

	free(tmpTrans);
}

// Supprime les transitions de l'état passé en paramètre
void supprimeTransEtat(automate* au, int state) {
	if(state >= au->size || state < 0) {
		printf("L'etat n'existe pas\n");
		return;
	}


	liste* tmpCurrent;
	int i;

	for(i=0;i<au->sizeAlpha; i++) {
		tmpCurrent = au->trans[state][i];
		while(tmpCurrent != NULL) {
			supprimeTransition(au, state, tmpCurrent->state,(char)('a'+i));
			tmpCurrent = au->trans[state][i];
		}
	}
}

// Fonction qui l'état passé en paramètre (y compris ses transitions)
void supprimeEtat(automate* au, int state) {
	int i,j,k;
	int* tmp;
	liste* tmpLst;
	liste*** tmpListeTrans;

	if(state >= au->size || state < 0) {
		printf("L'etat n'existe pas !\n");
		return;
	}

	for(i=0; i<au->size; i++){
		if(i == state){
			supprimeTransEtat(au, state);
		}else{
			for(j=0; j<au->sizeAlpha; j++){
				supprimeTransition(au, i, state, ((char)('a'+j)));
			}
		}
	}
	// Supression des transitions de chaques états  
	au->size = au->size - 1;
	tmp = au->initial;
	au->initial = (int*) malloc(au->size*sizeof(int));
	j = 0;
	for(i=0; i<=au->size; i++){
		if(i != state){
			au->initial[j] = tmp[i];
			j++;
		}
	}
	free(tmp);
	tmp = au->final;
	au->final = (int*) malloc(au->size*sizeof(int));
	j = 0;
	for(i=0; i<=au->size; i++){
		if(i != state){
			au->final[j] = tmp[i];
			j++;
		}
	}
	free(tmp);

	// Changement des variables dans les tableaux des états

	tmpListeTrans = au->trans;
	
	au->trans = (liste***) malloc (au->size*sizeof(liste**));

	j = 0;
	for(i=0; i<=au->size; i++){
		if(i != state){
			au->trans[j] = tmpListeTrans[i];
			j++;
		}else{
			free(tmpListeTrans[i]);
			
		}
	}

	free(tmpListeTrans);

	// Changement des transitions

	for(i=0; i<au->size; i++){
		for(j=0; j<au->sizeAlpha; j++){
			tmpLst = au->trans[i][j];
			
			while(tmpLst != NULL){
				if(tmpLst->state > state){
					tmpLst->state = tmpLst->state - 1;
				}
				tmpLst = tmpLst->nxt;
			}

				
		}
	}
}

// Fonction qui supprime tous les états de l'automate passé en paramètre
// Il supprime l'automate entier
void supprimeAutomate(automate* au){
	int i = au->size;
	automate* tmp;

	//Suppression des tous les états de l'automates
	while(i > 0){
		supprimeEtat(au, i-1);
		i = au->size;
	}

	// Liberation des états initials et finaux (ainsi que les transitions restantes)
	tmp = au;
	free(au->initial);
	free(au->final);
	free(au->trans);
	au = NULL;
	free(tmp);

	
}

// Fonction qui complete l'automate passé en paramètre
void completer(automate* au) {
	liste*** tmp = (liste***) malloc((au->size + 1)*sizeof(liste**));
    int* newinit = (int*) malloc((au->size + 1)*sizeof(int));
    int* newfinal = (int*) malloc((au->size + 1)*sizeof(int));

    int i,j,db = au->size;

    for(i=0;i<au->size;i++) {
        tmp[i] = au->trans[i];
        newinit[i] = au->initial[i];
        newfinal[i] = au->final[i];
    }

    tmp[db] =(liste**) malloc(au->sizeAlpha*sizeof(liste*));

    for(j=0;j<au->sizeAlpha;j++) {
        tmp[db][j] = NULL;
    }
    newinit[db] = 0;
    newfinal[db] = 0;

    au->size = db + 1;
    au->trans = tmp;
    au->initial = newinit;
    au->final = newfinal;

    for(i=0;i<au->size;i++) {
        for(j=0;j<au->sizeAlpha;j++) {
            if(au->trans[i][j] == NULL) {
                ajouteTransition(au,i,db,(char) j + 'a');
            }
        }
    }
}

// Fussione deux état de l'automate passé en paramètre. Ces états sont
// Etat1 et etat2
void fusionEtats(automate* au, int etat1, int etat2){
	int i,j;
	liste* tmpLst;

	if (etat1 >= au->size || etat1 < 0 || etat2 >= au->size || etat2 < 0 ){
		printf("L'Etat ou la lettre n'existe pas. \n");
		return;
	}
	
	

	// on met les transitions de l'état 2 dans l'état 1
	for(i=0; i<au->size; i++){
		for(j=0; j<au->sizeAlpha; j++){
			tmpLst = au->trans[i][j];
			while(tmpLst != NULL){
				if(i==etat2){
					if(tmpLst->state == etat2){
						ajouteTransition(au,etat1,etat1,('a'+j));
					}else{
						ajouteTransition(au,etat1,tmpLst->state,('a'+j));
					}
				}
				if(tmpLst->state == etat2){
					ajouteTransition(au,i,etat1,('a'+j));
				}
				tmpLst = tmpLst->nxt;	
			}
		}
	}

// on met à jour les tableaux d'états initiaux et finaux

	if(au->initial[etat2] == 1){
		au->initial[etat1] = 1;
	}
	if(au->final[etat2] == 1){
		au->final[etat1] = 1;
	}

	supprimeEtat(au,etat2);	
}


///////////////////////// PARTIE DU VIDE / GRAPHE

//fonction de parcour en profondeur
void parcour(graphe* au, int p, int** tab){
	int* color = *tab;

	color[p] = 1;
	liste* tmp = (au->neighbours)[p];
	while(tmp != NULL){
		if(color[tmp->state] == 0){
			parcour(au, tmp->state, tab);
		}
		tmp = tmp->nxt;
	}
}

// Fonction qui retourne 1 si il existe un chemin de p vers q
int chemin(graphe* gr,int p, int q){
	int i;
	int ** color;
	int *tmp;
	int res = 0;
	color = (int **) malloc (sizeof(int*));
	*color = (int*) malloc((gr->nb_summit)*sizeof(int));
	for(i=0; i<gr->nb_summit; i++){
		(*color)[i] = 0;
	}

	for(i=0; i<(gr->nb_summit); i++){
		parcour(gr,p,color);
	}

		
	tmp = (*color);
	res = tmp[q];
	free(*color);
	free(color);
	return res;	
}

// Fonction qui cree un graphe a partir d'un automate
void automateToGraphe(automate* au, graphe* gr){
	int i,j;
	liste* tmp;

	// initialisation du graphe avec la taille 
	gr->nb_summit = au->size;

	// création de la liste des voisins
	gr->neighbours = (liste**)malloc(gr->nb_summit*sizeof(liste*));
	for(i=0; i<gr->nb_summit; i++){
		gr->neighbours[i] = NULL;
	}
	// ajout des neighbours suivant les transitions présentes dans l'automate
	for(i=0;i<au->size;i++){
		for(j=0;j<au->sizeAlpha;j++){
			tmp = au->trans[i][j];
			while(tmp != NULL){
				ajouteListe(&(gr->neighbours[i]),tmp->state);
				tmp = tmp->nxt; 
			}
		}
	}

}

// Fonction qui affiche le graphe passé en paramètre
void afficheGraphe(graphe gr){
	int i;
	liste* tmp;

	printf("---> Grraphe <---\n");
	for(i=0 ; i<gr.nb_summit ; i++){
		tmp = gr.neighbours[i];
		printf("%d : ",i);

		while(tmp != NULL){
			printf(" %d,",tmp->state);
			tmp = tmp->nxt; 
		}

		printf("\n");
	}
	
}

// Fonction qui supprime le graphe passé en paramètre
void supprimeGraphe(graphe* gr){
	int i;
	liste* tmp_current;
	liste* tmp_delete;

	// Ici on supprime les voisins de chaques sommets
	for(i=0 ; i<gr->nb_summit ; i++){
		tmp_current = gr->neighbours[i];
		while(tmp_current != NULL){
			tmp_delete = tmp_current;
			tmp_current = tmp_current->nxt;
			free(tmp_delete);
		}
	}

	// libération de la mémoire
	free(gr->neighbours);
	free(gr);
	gr = NULL;
}

int langageVide(automate* au){
	int i,k;
	int sum = 0;

	graphe* gr = (graphe*)malloc(sizeof(graphe));

	automateToGraphe(au, gr);


	for(i=0 ; i<au->size ; i++){
		if(au->initial[i] == 1){
			for(k=0 ; k<au->size ; k++){
				if(au->final[k] == 1){
					sum = sum + chemin(gr,i,k);
				}
			}
		}
	}

	supprimeGraphe(gr);
	
	if(sum == 0){
		return 1;
	}else{
		return 0;
	}
}

// Fonction qui supprime les états non coaccessible de l'automate passé en param-ètre
void supprimeNonCoAccessibles(automate* au){
	int i, k, sum, size;
	graphe* gr = (graphe*)malloc(sizeof(graphe));
	i = 0;
	size = au->size;
	int toDelete[size];

	for(k=0; k<size; k++){
		toDelete[k] = 0;
	}

	automateToGraphe(au, gr);
	afficheGraphe(*gr);

	// Recherche des états non coaccessible
	// Pour chaque état on compte le nombre de chemin qui y parte
	// Si cette sum == 0 alors l'état est non coaccessible
	// On l'ajoute alors dans la liste des états a supprimer
	for(i=0; i<gr->nb_summit; i++){
		sum = 0;
		for(k=0; k < size; k++){
			if(au->final[k] == 1){
				sum += chemin(gr,i,k);
			}
		}
		if(sum == 0){
			toDelete[i] = 1;
		}
	}

	// supression des états
	sum = 0;
	for(k=0; k < size; k++){
		if(toDelete[k] == 1){
			supprimeEtat(au,(k - sum));
			sum++;
		}
	}
	supprimeGraphe(gr);

}

// Fonction qui supprime les états non accessible de l'automate passé en param-ètre
void supprimeNonAccessibles(automate* au){
	int i, k, sum, size;
	graphe* gr = (graphe*)malloc(sizeof(graphe));
	i = 0;
	size = au->size;
	int toDelete[size];

	for(k=0; k<size; k++){
		toDelete[k] = 0;
	}

	automateToGraphe(au, gr);
	afficheGraphe(*gr);

	// Recherche des états non accessible
	// Pour chaque état on compte le nombre de chemin qui y mène
	// Si cette sum == 0 alors l'état est non accessible
	// On l'ajoute alors dans la liste des états a supprimer
	for(i=0; i<gr->nb_summit; i++){
		sum = 0;
		for(k=0; k < size; k++){
			if(au->final[k] == 1){
				sum += chemin(gr,k,i);
			}
		}
		if(sum == 0){
			toDelete[i] = 1;
		}
	}

	// supression des états
	sum = 0;
	for(k=0; k < size; k++){
		if(toDelete[k] == 1){
			supprimeEtat(au,(k - sum));
			sum++;
		}
	}
	supprimeGraphe(gr);

}

// Fonction qui fais le produit de l'automate au1 par au2, et le stock dans prod
void produit(automate* au1, automate* au2, automate* prod){
	int m,i,j;
	int taille, currentSizeAlpha;
	automate* petit;
	automate* grand;
	liste* tmpPetit;
	liste* tmpGrand;

	// La taille de l'automate sous produit est celle du plus petit automate

	if(au1->size >= au2->size){
		m = au1->size;
		grand = au1;
		petit = au2;
	}else{
		m = au2->size;
		grand = au2;
		petit = au1;
	}

	taille = m * petit->size;
	
	if(au1->sizeAlpha >= au2->sizeAlpha){
		currentSizeAlpha = au2->sizeAlpha;
	}else{
		currentSizeAlpha = au1->sizeAlpha;
	}

// initialisation du nouvel automate

	prod->size = taille;
	prod->sizeAlpha = currentSizeAlpha;

	
	prod->initial=(int*) malloc(taille*sizeof(int));
	prod->final=(int*) malloc(taille*sizeof(int));

// gestion états initiaux/finaux

	for(i=0; i<taille; i++){
		if(petit->initial[i/m] == 1 && grand->initial[i%m] == 1){
			prod->initial[i] = 1;
		}else{
			prod->initial[i] = 0;
		}


		if(petit->final[i/m] == 1 && grand->final[i%m] == 1){
			prod->final[i] = 1;
		}else{
			prod->final[i] = 0;
		}
	}

	
// création de la liste des transition
	prod->trans=(liste***) malloc(taille*sizeof(liste**));
	for(i=0;i<taille;i++){
		prod->trans[i]=(liste**) malloc(currentSizeAlpha*sizeof(liste*));
		for(j=0;j<currentSizeAlpha;j++){
			prod->trans[i][j]=NULL;
		}
	}


// ajout des transitions d'après le modèle mathématique proposé

	for(i=0; i<taille; i++){
		for(j=0; j<currentSizeAlpha; j++){
			tmpGrand = grand->trans[i%m][j] ;
			tmpPetit = petit->trans[i/m][j] ;


			while(tmpPetit != NULL){
				
				while(tmpGrand != NULL){
					
					ajouteTransition(prod,i,((tmpPetit->state * m)+tmpGrand->state),(char)('a' + j));
					tmpGrand = tmpGrand->nxt;
				}
				tmpPetit = tmpPetit->nxt;
				tmpGrand = grand->trans[i%m][j] ;
			}

		}
	}
	
	
}


// Fonction qui renvoie 1 si l'intersection est vide, et 0 le contraire
int intersectionVide(automate* au1, automate* au2){
	automate* tmp = (automate*)malloc(sizeof(automate));
	int res = 0;

	// On effectue le produit des deux automates
	// Le langage est vide si l'intersection est vide

	produit(au1, au2, tmp);

	res = langageVide(tmp);	

	//supprimeAutomate(tmp);

	// REtour 1 si l'intersection est vide, et 0 le contraire
	return res;
	
}

// Fonction qui permet de déterminiser un automate passé en paramètre
void determinise(automate** ap){
	automate* au = *ap;
	if(deterministe(*au) == 1){
		return;
	}
		
	int i=0;
	int j=0;
	int init;
	init = -1;
	liste* tmp;
	iliste* tmpFile;
	int n = 0;
	int* pt = (int*) malloc (sizeof(int));
	int k, estDansTab;
	automate* D = (automate*) malloc (sizeof(automate));


	ifile* file = (ifile*) malloc (sizeof(ifile));
	file->debut = NULL;
	file->fin = NULL;

	//ajout du premier etat
	while(init == -1 && i<au->size){
		if(au->initial[i] == 1){
			init = i;
		}
		i++;
	} 
	pt[0] = init;
	ajouteFile(file, pt, 1);


	// création et remplissage de la file avec les nouveaux états
	tmpFile = file->debut;
	while(tmpFile != NULL){
		for(j=0; j<au->sizeAlpha; j++){
			pt = (int*) realloc(pt,0*sizeof(int));
			n = 0;
			estDansTab = 0;
			for(i=0; i<tmpFile->tailleVal; i++){
				tmp = au->trans[tmpFile->val[i]][j]; 
				while(tmp != NULL){
					for(k=0; k<n; k++){
						if(tmp->state == pt[k]){
							estDansTab = 1;
						}
					}
					if(estDansTab == 0){
						n++;
						pt = (int*) realloc(pt,n*sizeof(int));
						pt[n-1] = tmp->state;
						tmp = tmp->nxt;
					}
				}
				
			}
			if(estDansFile(*file,pt,n) == 0){
				ajouteFile(file,pt,n);
			}
		}

		tmpFile = tmpFile->nxt;
	}

	// Creation de l'automate
	D->size = file->fin->state + 1;
	D->sizeAlpha = au->sizeAlpha;

	
	D->initial=(int*) malloc(D->size*sizeof(int));

	for(i=0; i<D->size; i++){
		if(i != 0){
			D->initial[i] = 0;
		}else{
			D->initial[i] = 1;
		}
	}

	D->final=(int*) malloc(D->size*sizeof(int));
	tmpFile = file->debut;

	while(tmpFile != NULL){
		for(i=0; i<tmpFile->tailleVal; i++){
			if(au->final[tmpFile->val[i]] == 1){
				D->final[tmpFile->state] = 1;
			}else{
				D->final[tmpFile->state] = 0;
			}
		}
		tmpFile = tmpFile->nxt;	
	
	}
	
	D->trans=(liste***) malloc(D->size*sizeof(liste**));
	for(i=0;i<D->size;i++){
		D->trans[i]=(liste**) malloc(D->sizeAlpha*sizeof(liste*));
		for(j=0;j<D->sizeAlpha;j++){
			D->trans[i][j]=NULL;
		}
	}



// ajout des transitions en se servant de la fonction numEtatDansFile
// qui retourne le numéro dans le nouvel automate d'un ensemble d'état

	tmpFile = file->debut;
	while(tmpFile != NULL){
		for(j=0; j<au->sizeAlpha; j++){
			pt = (int*) realloc(pt,0*sizeof(int));
			n = 0;
			estDansTab = 0;
			for(i=0; i<tmpFile->tailleVal; i++){
				tmp = au->trans[tmpFile->val[i]][j];
				while(tmp != NULL){
					for(k=0; k<n; k++){
						if(tmp->state == pt[k]){
							estDansTab = 1;
						}
					}
					if(estDansTab == 0){
						n++;
						pt = (int*) realloc(pt,n*sizeof(int));
						pt[n-1] = tmp->state;
						tmp = tmp->nxt;
					}
				}
				
			}

			if((init = numEtatDansFile(*file, pt, n)) != -1){
				ajouteTransition(D,tmpFile->state,init,(char)('a' + j));
			}
		}

		tmpFile = tmpFile->nxt;
	}

	
	free(pt);
	libereFile(file);

	*ap = D;

	supprimeAutomate(au);
}


/* Fonction qui copie un automate dans un autre 
utile pour tester l'inclusion*/
void copieAutomate(automate orig, automate* copy){

	int i,j;
	liste* tmp = NULL;
	
	copy->size = orig.size;
	copy->sizeAlpha = orig.sizeAlpha;	

	copy->initial = (int*) malloc(copy->size*sizeof(int));
	copy->final = (int*) malloc(copy->size*sizeof(int));

	
	for(i=0; i<copy->size; i++){
		copy->initial[i] = orig.initial[i];
		copy->final[i] = orig.final[i];
	}

	
	copy->trans=(liste***) malloc(copy->size*sizeof(liste**));
	for(i=0;i<copy->size;i++){
		copy->trans[i]=(liste**) malloc(copy->sizeAlpha*sizeof(liste*));
		for(j=0;j<copy->sizeAlpha;j++){
			tmp = orig.trans[i][j];
			copy->trans[i][j] = NULL;
			while(tmp != NULL){
				ajouteTransition(copy,i,tmp->state,(char)('a' + j));
				tmp = tmp->nxt;
			}
		}
	}
}


/* Fonction qui calcule l'automate complémentaire
de l'automate passé en premier paramètre
dans celui passé en deuxième paramètre */
void complementaire(automate* au, automate** comp){
	int i;
	copieAutomate(*au, *comp);

	
	determinise(comp);

	if (complet(**comp) == 0){
		completer(*comp);
	}

// inversion des états finaux et non finaux

	for(i=0; i<(*comp)->size; i++){
		if((*comp)->final[i] == 0){
			(*comp)->final[i] = 1;
		}else{
			(*comp)->final[i] = 0;
		}
	}
			
	afficheAutomate(**comp);

}

/* Fonction qui permet de savoir si le premier 
automate est inclut dans le second 1 si oui 0 sinon*/
/* Le principe d'inclusion réside dans le fait que
l'inclusion de au dans B correspond au fait que l'intersection
de au avec le complémentaire de B est vide */
int inclusion(automate** au, automate* B){
	int res = 0;
	automate* compB = (automate*)malloc(sizeof(automate));

	determinise(au);

	complementaire(B, &compB);

	res = intersectionVide(*au,compB);

	supprimeAutomate(compB);

	return res;
}


/* Fonction qui détermine si deux états sont
équivalents par l'équivalence de Nérode 
1 si oui 0 sinon*/
int nerodeEquivalent(automate* au, int e1, int e2){
	int j=0;

	if(au->final[e1] != au->final[e2]){
		return 0;
	}

// si une transition est différente alors ils ne sont pas équivalent
	while(j<au->sizeAlpha){
		if(au->final[(au->trans[e1][j])->state] != au->final[(au->trans[e2][j])->state] ){
			return 0;
		}
		j++;
	}
	
// sinon il le sont
	return 1;
}


/* Fonction qui minimise un automate en fusionnant 
les états qui sont nérode-équivalent */
void minimiseNerode(automate** au){
	int i = 0;
	int j = 0;
	
	if(deterministe(**au) == 0){
		determinise(au);
	}

	if(complet(**au) == 0){
		completer(*au);
	}

// pour tous les états ont teste si il sont deux à deux 
// équivalent, si c'est le cas, on les fusionne
	while(i<(*au)->size){
		j = i + 1;
		while(j<(*au)->size){
			if(nerodeEquivalent(*au,i,j)){
				fusionEtats(*au,i,j);
			}else{
				j++;
			}
		}
	i++;
	}
}

/* Fonction qui minimise un automate en se servant
de la méthode de Hopcroft NON FONCTIONNELLE */
void minimiseHopcroft(automate* au){

	if(deterministe(*au) == 0){
		determinise(&au);
	}

	if(complet(*au) == 0){
		completer(au);
	}

	automate* T = au;
	int i,j,k,l;
	int numClasse = 1;
	int dif = 0;
	int equiv = 0;
	int tailleClasse;
	int cpt = 0;
	tailleClasse = T->sizeAlpha + 2;


// on stock les classes d'équivalence dans la matrice classe
// la première ligne est l'équivalence courante et la dernière 
// ligne correspond à la prochaine équivalence
// les lignes entre correspondent aux états d'arrivé par la transition
// avec la lettre d'indice de ligne - 1
// depuis l'état d'indice de colonne

	int** class;
	class = (int**) malloc (tailleClasse*sizeof(int*));

	for(i=0; i<tailleClasse; i++){
		class[i] = (int*) malloc(T->size * sizeof(int));
		for(j=0; j<T->size; j++){
			if(i==0){
				class[i][j] = T->final[j];
			}else{
				class[i][j] = -1;
			}
		}
	} 


// tant que la première et la dernière ligne ne sont pas égales
	while(equiv != 1){
		numClasse = 1;
		for(i=0; i<T->size; i++){
			for(j=1; j<tailleClasse - 1; j++){
				class[j][i] = class[0][(T->trans[i][j-1])->state];
			}

			if(i==0){
				class[tailleClasse-1][i] = 0;
			}else{
				for(k=0; k<i; k++){
					dif = 0;
					for(l=0; l<tailleClasse -1; l++){
						if(class[l][k] != class[l][i]){
							dif = 1;
						}
					}

					if(dif != 0){
						class[tailleClasse-1][i] = numClasse;
						numClasse ++;
					}else{
						class[tailleClasse-1][i] = class[tailleClasse-1][k];
					}
				}
			}
		}
	

	// on test l'égalité des deux lignes
		equiv = 1;
		for(i=0; i<T->size; i++){
			if(class[0][i] != class[tailleClasse-1][i]){
				equiv = 0;
			}
		}
	// on met la dernière ligne dans la première pour le prochain tour
		for(i=0; i<T->size; i++){
			class[0][i] = class[tailleClasse-1][i];
		}
		
		cpt++;
			
	}

	if(cpt>2){//on construit le nouvel automate car cela veut dire que l'automate n'était pas minimal
		cpt = 0;
		for(i=0; i<T->size; i++){
			if(class[0][i] >= cpt){
				cpt++;
			}
		}

		
		automate* new = (automate*) malloc(sizeof(automate));
		new->size = cpt;
		new->sizeAlpha = tailleClasse -2;
		new->initial = (int*) malloc(new->size*sizeof(automate));
		new->final = (int*) malloc(new->size*sizeof(automate));

		for(i=0; i<T->size; i++){
				new->initial[class[0][i]] = T->initial[i];
				new->final[class[0][i]] = T->final[i];
		}

		new->trans=(liste***) malloc(new->size*sizeof(liste**));
		for(i=0;i<new->size;i++){
			new->trans[i]=(liste**) malloc(new->sizeAlpha*sizeof(liste*));
			for(j=0;j<new->sizeAlpha;j++){
				new->trans[i][j]=NULL;
			}
		}

		
		for(i=0; i<T->size; i++){
			for(j=1; j<tailleClasse - 1; j++){
				ajouteTransition(new, class[0][i], class[j][i], (char)('a' + (j-1)));
			}
		}
		

		au = new;
		supprimeAutomate(T);
	}

	for(i=0; i<tailleClasse; i++){
		free(class[i]);
	}
	free(class);

}





int main() {
   	int continuer = 1;
   	int continuer2 = 1;
	automate* au = (automate*) malloc(sizeof(automate));
	automate* auProd = (automate*) malloc(sizeof(automate));
	automate* auProd1 = (automate*) malloc(sizeof(automate));
	automate* auProdResult = (automate*) malloc(sizeof(automate));
    int choix = 0;
    int size, sizeAlpha;
    int i, k, j;
    int targ;
	char tran;

	graphe* gr = (graphe*) malloc(sizeof(graphe));

    while(continuer) {
    	printf("Voulez-vous utiliser un automate exemple ? (1: Oui; 2 Non)\n");
    	scanf("%i", &choix);
    	switch(choix) {
    		case 1:
    			construitAutomateExemple(au);
    			afficheAutomate(*au);
    			break;
    		case 2:
    			printf("Taille?(size): \n");
    			scanf("%i", &size);
    			printf("Taille Alpha?(sizeAlpha: \n");
    			scanf("%i", &sizeAlpha);

    			au->size = size;
				au->sizeAlpha = sizeAlpha;

				au->initial = (int*) malloc(au->size*sizeof(int));

				for(i=0; i<au->size; i++) {
					printf("Etat:%d, Initial? Oui:1 | Non:0\n", i);
					scanf("%d",&k);
					au->initial[i] = k;
				}

				au->final = (int*) malloc(au->size*sizeof(int));

				for(i=0; i<au->size; i++) {
					printf("Etat:%d, Final? Oui:1 | Non:0\n", i);
					scanf ("%d",&k);
					au->initial[i] = k;
				}

				// Creation/initialisations de transitions vides
				au->trans = (liste***) malloc(au->size*sizeof(liste***));
				for(i=0;i<au->size;i++){
					au->trans[i]=(liste**) malloc(au->sizeAlpha*sizeof(liste*));
					for(k=0;k<au->sizeAlpha;k++){
						au->trans[i][k]=NULL;
					}
				}

				for(i=0;i<au->size;i++) {
					printf("Etat: %d | ", i);
					printf("Combien de transitions voulez vous ajouter\n");
					scanf ("%d",&k);
					for(j=0; j<k; j++) {
						printf("\nEtat cible ? (de 0 à %d): ", au->size-1);
						scanf ("%d",&targ);
						printf("\nPar transition ? (a, ..., %c /!\\ à la valeur !)", au->sizeAlpha+96); // 97:a; -1 pour la taille
						scanf ("%s",&tran);
						printf("%c\n", tran);
						ajouteTransition(au,i,targ,tran);
					}
				}
				afficheAutomate(*au);

    			break;
    	}

    	int selection1, selection2;
    	char trans;
    	while(continuer2) {
    		printf("\n>1. Afficher l'automate\n>2. Completer l'automate\n>3. L'automate est-il complet ?\n>4. L'automate est-il deterministe?\n>5. Supprimer un etat de l'automate\n>6. Fusionner 2 etats de l'automate\n>7. Supprimer une transition de l'automate\n>8. Ajouter une transition a l'automate\n>9. Creer graphe et l\'afficher\n>10. Langage Vide ? \n>11. Supression etat non coaccessible\n>12. Supression etat non accessible\n>13. Creation et produit de deux automates\n>14. Intersection des deux langages vides ?\n>15 compteTransitions.\n>16 determinise(exemple)\n>17 Inclusion? (exemple) \n>18 Minimization (exemple)\n>19. Arreter programme\n> Choix : ");
			scanf("%i", &choix);

			switch(choix) {
				case 1:
					afficheAutomate(*au);
					break;
				case 2:
					printf("Completition:\n");
					completer(au);
					break;
				case 3:
					if(complet(*au))
                        printf("\nL'automate est complet.");
                    else
                        printf("\nL'automate n'est pas complet.");
                    break;
                case 4:
                	if(deterministe(*au))
                        printf("\nL'automate est deterministe.");
                    else
                        printf("\nL'automate n'est pas deterministe.");
                    break;
                case 5:
                	printf("\nEntrer le numero de l'etat : ");
                    scanf("%i", &selection1);
                    supprimeEtat(au,selection1);
                    break;
                case 6:
                	printf("\nEntrer le numero du premier etat : ");
                    scanf("%i",&selection1);
                    printf("\nEntrer le numero du second etat : ");
                    scanf("%i",&selection2);
					fusionEtats(au,selection1,selection2);
					break;
				case 7:
					printf("\nEntrer le numero de l'etat depart : ");
                    scanf("%d",&selection1);
                    printf("\nEntrer le numero de l'etat d'arrivee : ");
                    scanf("%d",&selection2);
                    printf("\nEntrer la lettre de la transition : ");
                    scanf("%c",&trans);
                    scanf("%c",&trans);
                    supprimeTransition(au,selection1,selection2,trans);
                    break;
                case 8:
                	printf("\nEntrer le numero de l'etat depart : ");
                    scanf("%d",&selection1);
                    printf("\nEntrer le numero de l'etat d'arrivee : ");
                    scanf("%d",&selection2);
                    printf("\nEntrer la lettre de la transition : ");
                    scanf("%c",&trans);
                    scanf("%c",&trans);
                    ajouteTransition(au,selection1,selection2,trans);
                    break;
                case 9:
                	printf("Creation du graphe\n");
                	automateToGraphe(au,gr);
                	afficheGraphe(*gr);
                	break;
                case 10:
                	printf("L'automate a-t-il un langage vide ?\n");
                	if(langageVide(au)) {
                		printf("Oui\n");
                	} else {
                		printf("Non\n");
                	}
               		break;
               	case 11:
               		printf("Suppresion des états non coaccessible\n");
               		supprimeNonCoAccessibles(au);
               		break;
               	case 12:
               		printf("Suppresion des états non accessible\n");
               		supprimeNonAccessibles(au);
               		break;
               	case 13:
               		printf("Creation et affichage du produit de deux automate (Utilisation des automates exemples)\n");
               		construitAutomateExempleProd1(auProd);
               		construitAutomateExempleProd2(auProd1);
               		printf("Automate exemple 1\n");
               		afficheAutomate(*auProd);
               		printf("Automate exemple 2\n");
               		afficheAutomate(*auProd1);
               		produit(auProd,auProd1,auProdResult);
               		printf("Automate produit\n");
               		afficheAutomate(*auProdResult);

               		supprimeAutomate(auProd1);
    				supprimeAutomate(auProd);
               		break;
               	case 14:
               		printf("Test si l'Intersection des automates est vide. (utilisation des automates exemples)\n");
               		construitAutomateExempleProd1(auProd);
               		construitAutomateExempleProd2(auProd1);;
               		printf("intersectionVide ?\n");
               		if(intersectionVide(auProd,auProd1)) {
               			printf("OUI\n");
               		} else {
               			printf("NON\n");
               		};
               		break;
               	case 15:
               		printf("Compte Transitions\n");
               		printf("Il y a %i transition(s)\n",compteTransitions(*au));
               		break;
               	case 16:
               		printf("Determinisation\n");
               		automate* auDeter = (automate*) malloc(sizeof(automate));
               		construitAutomateExempleDeterm(auDeter);
               		determinise(&auDeter);
               		afficheAutomate(*auDeter);
               		supprimeAutomate(auDeter);
               		break;
               	case 17:
               		printf("Test d'inclusion\n");
               		automate* auInc1 = (automate*) malloc(sizeof(automate));
               		automate* auInc2 = (automate*) malloc(sizeof(automate));
               		construitAutomateExempleInclusion(auInc1, auInc2);
               		if(inclusion(&auInc2,auInc1)) {
               			printf("L'automate 1 est inclu dans le 2\n");
               		} {
               			printf("L'automate 1 n'est pas inclu dans le 2\n");
               		}

               		supprimeAutomate(auInc1);
    				supprimeAutomate(auInc2);
               		break;
           		case 18:
           			printf("Minimization\n");
           			automate* auMinim = (automate*) malloc(sizeof(automate));
           			//automate* auMinim2 = (automate*) malloc(sizeof(automate));
           			construitAutomateExempleMinimization(auMinim);
           			printf("Minimise NERODE !\n");
           			minimiseNerode(&auMinim);
           			afficheAutomate(*auMinim);
           			supprimeAutomate(auMinim);
           			//printf("Minimise HOPCROFT !\n");
           			//construitAutomateExempleMinimization(auMinim2);
           			//minimiseHopcroft(auMinim2);
           			//afficheAutomate(*auMinim2);
           			break;
                case 19:
                	continuer = 0;
                	continuer2 = 0;
                	break;
                default:
                	printf("Mauvaise entrée\n");
			}
    	}
    }
    supprimeAutomate(au);

	return 0;
}