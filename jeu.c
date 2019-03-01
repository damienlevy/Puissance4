/*
  Canvas pour algorithmes de jeux à 2 joueurs
	
  joueur 0 : humain
  joueur 1 : ordinateur
			
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Paramètres du jeu
#define LARGEUR_MAX 7		// nb max de fils pour un noeud (= nb max de coups possibles)
#define NBJETONALIGNER 4
#define TEMPS 3		// temps de calcul pour un coup avec MCTS (en secondes)

#define COLONNE 7
#define LIGNE 6

// macros
#define AUTRE_JOUEUR(i) (1-(i))
#define min(a, b)       ((a) < (b) ? (a) : (b))
#define max(a, b)       ((a) < (b) ? (b) : (a))

// Critères de fin de partie
typedef enum {NON, MATCHNUL, ORDI_GAGNE, HUMAIN_GAGNE } FinDePartie;

// Definition du type Etat (état/position du jeu)
typedef struct EtatSt {

  int joueur; // à qui de jouer ? 

  char plateau[LIGNE][COLONNE];	

} Etat;

// Definition du type Coup
typedef struct {
  // TODO: à compléter par la définition d'un coup 

  /* par exemple, pour morpion: */
  //int ligne;
  int colonne;

} Coup;

// Copier un état 
Etat * copieEtat( Etat * src ) {
  Etat * etat = (Etat *)malloc(sizeof(Etat));

  etat->joueur = src->joueur;
	
		
  // TODO: à compléter avec la copie de l'état src dans etat
	
  /* par exemple : */
  int i,j;	
  for (i=0; i< LIGNE; i++)
    for ( j=0; j<COLONNE; j++)
      etat->plateau[i][j] = src->plateau[i][j];
	

	
  return etat;
}

// Etat initial 
Etat * etat_initial( void ) {
  Etat * etat = (Etat *)malloc(sizeof(Etat));
	
  // TODO: à compléter avec la création de l'état initial
	
  /* par exemple : */
  int i,j;	
  for (i=0; i< LIGNE; i++)
    for ( j=0; j< COLONNE; j++)
      etat->plateau[i][j] = ' ';
	
  return etat;
}


void afficheJeu(Etat * etat) {

  // TODO: à compléter

  /* par exemple : */
  int i,j;
  printf("   |");
  for ( j = 0; j < COLONNE; j++) 
    printf(" %d |", j);
  printf("\n");
  printf("--------------------------------");
  printf("\n");
	
  for(i=0; i < LIGNE; i++) {
    printf(" %d |", i);
    for ( j = 0; j < COLONNE; j++) 
      printf(" %c |", etat->plateau[i][j]);
    printf("\n");
    printf("--------------------------------");
    printf("\n");
  }
}


// Nouveau coup 
// TODO: adapter la liste de paramètres 42au jeu
Coup * nouveauCoup( int j ) {
  Coup * coup = (Coup *)malloc(sizeof(Coup));
	
  // TODO: à compléter avec la création d'un nouveau coup
	
  /* par exemple : */
  
  //coup->ligne = i;
  coup->colonne = j;
	
  return coup;
}

// Nouveau coup 
// TODO: adapter la liste de paramètres au jeu
/*Coup * nouveauCoupcolonne( int j ,Etat * etat) {
  Coup * coup = (Coup *)malloc(sizeof(Coup));
	
  // TODO: à compléter avec la création d'un nouveau coup
  int i = 0;
  
  while (i < LIGNE && etat->plateau[i][j] == ' '){
  i++;
  }
  
  coup->ligne = i-1;
  coup->colonne = j;
	
  return coup;
  }*/
// Demander à l'humain quel coup jouer 
Coup * demanderCoup (Etat * etat) {

  // TODO...

  /* par exemple : */
  int j;
  printf(" quelle colonne ? ") ;
  scanf("%d",&j); 
	
  return nouveauCoup(j);
}

// Modifier l'état en jouant un coup 
// retourne 0 si le coup n'est pas possible
int jouerCoup( Etat * etat, Coup * coup ) {


  int i = 0;
  
  while (i < LIGNE && etat->plateau[i][coup->colonne] == ' '){
    i++;
  }
  if ( etat->plateau[i-1][coup->colonne] != ' ' )
    return 0;
  else {
    etat->plateau[i-1][coup->colonne] = etat->joueur ? 'O' : 'X';
		
    // à l'autre joueur de jouer
    etat->joueur = AUTRE_JOUEUR(etat->joueur); 	

    return 1;
  }	
}

// Retourne une liste de coups possibles à partir d'un etat 
// (tableau de pointeurs de coups se terminant par NULL)
Coup ** coups_possibles( Etat * etat ) {
	
  Coup ** coups = (Coup **) malloc((1+LARGEUR_MAX) * sizeof(Coup *) );
	
  int k = 0;
	
  // TODO: à compléter
	
  /* par exemple */
  int i,j;
  
  for (j=0; j < COLONNE; j++) {
   
    if ( etat->plateau[0][j] == ' ' ) {

      coups[k] = nouveauCoup(j); 
      k++;
    }
	
  }
  /* fin de l'exemple */
	
  coups[k] = NULL;

  return coups;
}


// Definition du type Noeud 
typedef struct NoeudSt {
		
  int joueur; // joueur qui a joué pour arriver ici
  Coup * coup;   // coup joué par ce joueur pour arriver ici
	
  Etat * etat; // etat du jeu
			
  struct NoeudSt * parent; 
  struct NoeudSt * enfants[LARGEUR_MAX]; // liste d'enfants : chaque enfant correspond à un coup possible
  int nb_enfants;	// nb d'enfants présents dans la liste
	
  // POUR MCTS:
  int nb_victoires;
  int nb_simus;
	
} Noeud;


// Créer un nouveau noeud en jouant un coup à partir d'un parent 
// utiliser nouveauNoeud(NULL, NULL) pour créer la racine
Noeud * nouveauNoeud (Noeud * parent, Coup * coup ) {
  Noeud * noeud = (Noeud *)malloc(sizeof(Noeud));
	
  if ( parent != NULL && coup != NULL ) {
    noeud->etat = copieEtat ( parent->etat );
    jouerCoup ( noeud->etat, coup );
    noeud->coup = coup;			
    noeud->joueur = AUTRE_JOUEUR(parent->joueur);		
  }
  else {
    noeud->etat = NULL;
    noeud->coup = NULL;
    noeud->joueur = 0; 
  }
  noeud->parent = parent; 
  noeud->nb_enfants = 0; 
	
  // POUR MCTS:
  noeud->nb_victoires = 0;
  noeud->nb_simus = 0;	
	

  return noeud; 	
}

// Ajouter un enfant à un parent en jouant un coup
// retourne le pointeur sur l'enfant ajouté
Noeud * ajouterEnfant(Noeud * parent, Coup * coup) {
  Noeud * enfant = nouveauNoeud (parent, coup ) ;
  parent->enfants[parent->nb_enfants] = enfant;
  parent->nb_enfants++;
  return enfant;
}

void freeNoeud ( Noeud * noeud) {
  if ( noeud->etat != NULL)
    free (noeud->etat);
		
  while ( noeud->nb_enfants > 0 ) {
    freeNoeud(noeud->enfants[noeud->nb_enfants-1]);
    noeud->nb_enfants --;
  }
  if ( noeud->coup != NULL)
    free(noeud->coup); 

  free(noeud);
}
	

// Test si l'état est un état terminal 
// et retourne NON, MATCHNUL, ORDI_GAGNE ou HUMAIN_GAGNE
FinDePartie testFin( Etat * etat ) {

  // TODO...
	
  /* par exemple	*/
	
  // tester si un joueur a gagné
  int i,j,k,n = 0;
  for ( i=0;i < LIGNE; i++) {
    for(j=0; j < COLONNE; j++) {
      if ( etat->plateau[i][j] != ' ') {
	n++;	// nb coups joués
			
	// lignes
	k=0;
	while ( k < NBJETONALIGNER && i+k < LIGNE && etat->plateau[i+k][j] == etat->plateau[i][j] ) 
	  k++;
	if ( k == NBJETONALIGNER ) 
	  return etat->plateau[i][j] == 'O'? ORDI_GAGNE : HUMAIN_GAGNE;

	// colonnes
	k=0;
	while ( k < NBJETONALIGNER && j+k < COLONNE && etat->plateau[i][j+k] == etat->plateau[i][j] ) 
	  k++;
	if ( k == NBJETONALIGNER ) 
	  return etat->plateau[i][j] == 'O'? ORDI_GAGNE : HUMAIN_GAGNE;

	// diagonales
	k=0;
	while ( k < NBJETONALIGNER && i+k < LIGNE && j+k < COLONNE && etat->plateau[i+k][j+k] == etat->plateau[i][j] ) 
	  k++;
	if ( k == NBJETONALIGNER ) 
	  return etat->plateau[i][j] == 'O'? ORDI_GAGNE : HUMAIN_GAGNE;

	k=0;
	while ( k < NBJETONALIGNER && i+k < LIGNE && j-k >= 0 && etat->plateau[i+k][j-k] == etat->plateau[i][j] ) 
	  k++;
	if ( k == NBJETONALIGNER ) 
	  return etat->plateau[i][j] == 'O'? ORDI_GAGNE : HUMAIN_GAGNE;		
      }
    }
  }

  // et sinon tester le match nul	
  if ( n == LIGNE*COLONNE ) 
    return MATCHNUL;
		
  return NON;
}
double b_value(Noeud * noeud, float c){
  double b = 0.0;
  double u;
  double valeur;
  if(noeud->nb_simus> 0){
   u = (double)noeud->nb_victoires/(double)noeud->nb_simus;
   if(noeud->parent->nb_simus> 0){
   valeur =  log(noeud->parent->nb_simus)/(double)noeud->nb_simus;
   }else{
      valeur = 1;
   }
    if(noeud->joueur == 0){
     // b =  (-((double)noeud->nb_victoires/(double)noeud->nb_simus) + c * sqrt(log((double)noeud->parent->nb_simus)/(double)noeud->nb_simus));
      b=c*sqrt(valeur)-u;
    }else{
      b=c*sqrt(valeur)+u;
      //b =  (((double)noeud->nb_victoires/(double)noeud->nb_simus) + c * sqrt(log((double)noeud->parent->nb_simus)/(double)noeud->nb_simus));

    }
  }

 
  //printf("%lf,%lf,%lf,%d\n",b,u,valeur,noeud->parent->nb_simus);
  return b; 

}

Noeud * meilleurNoeud(Noeud* noeud){
  Noeud* n = noeud->enfants[0];
  int i;
  for (i=1 ; i < noeud->nb_enfants;i++){
    
    if (b_value(noeud->enfants[i],sqrt(2)) > b_value(n,sqrt(2))){
      n = noeud->enfants[i];
    }
          
  }
  return n;

}

Coup * meilleurCoup(Noeud* noeud){  
  
  return  meilleurNoeud(noeud)->coup;
}
// Calcule et joue un coup de l'ordinateur avec MCTS-UCT
// en tempsmax secondes
void ordijoue_mcts(Etat * etat, int tempsmax) {

  clock_t tic, toc;
  tic = clock();
  int temps;

  Coup ** coups;
  Coup * meilleur_coup ;
	
  // Créer l'arbre de recherche
  Noeud * racine = nouveauNoeud(NULL, NULL);	
  racine->etat = copieEtat(etat); 
	
  // créer les premiers noeuds:
  Noeud * enfant;
  coups = coups_possibles(racine->etat); 
  int k = 0;
  
  while ( coups[k] != NULL) {
    enfant = ajouterEnfant(racine, coups[k]);
    k++;
  }
  
  
	
	
  //meilleur_coup = coups[ rand()%k ]; // choix aléatoire
	
  /*  TODO :
      - supprimer la sélection aléatoire du meilleur coup ci-dessus
      - implémenter l'algorithme MCTS-UCT pour déterminer le meilleur coup ci-dessous
  */
  int iter = 0;
  int cmp,cmpNonDev;
  int i,vic;
  FinDePartie fp;
  Noeud ** nNonDev = (Noeud **) malloc((1+LARGEUR_MAX) * sizeof(Noeud*) ); 
  cmp=0;
  do {
    
    enfant = racine ;
  
    //Boucle tant que j'ai pas trouver un noeud non developes
    do{
      cmpNonDev=0;
        // je regarde si les fils du currrent sont developes
      for( i =0 ; i< enfant->nb_enfants;i++){
	if(enfant->enfants[i]->nb_enfants==0){
	  nNonDev[cmpNonDev]=enfant->enfants[i];
	  cmpNonDev++;
	}
      }
      if(cmpNonDev==0){ //si oui alors je change current par le maxB des fils
	enfant = meilleurNoeud(enfant);
      }
      else{//j'ai trouver des fils non developes j'en prend un au hazard
	enfant = nNonDev[rand()%(cmpNonDev)];
      }
    }while(cmpNonDev==0);
  
    
    
    fp = testFin(enfant->etat);//teste de testFin
    while (fp== NON){
	coups = coups_possibles(enfant->etat); 
	k = 0;
      
	while ( coups[k] != NULL) {
	  ajouterEnfant(enfant, coups[k]);
	  k++;
	}
      enfant = enfant -> enfants[rand()%(enfant->nb_enfants)];
      fp = testFin(enfant->etat);
   }
    

    
    vic = 0 ;
    if(fp==ORDI_GAGNE){
      
      vic = 1;
      cmp++;
      }else{
	vic = 0;
      }
      enfant->nb_victoires = enfant->nb_victoires+vic;
      enfant->nb_simus= enfant->nb_simus+1;
      while(enfant->parent !=NULL) {
	
	enfant = enfant->parent;
	enfant->nb_victoires = enfant->nb_victoires+vic;
	enfant->nb_simus= enfant->nb_simus+1;
      }	
    toc = clock(); 
    temps = (int)( ((double) (toc - tic)) / CLOCKS_PER_SEC );
    iter ++;
  
  } while ( temps < tempsmax );
  /* fin de l'algorithme  */
  
  // Jouer le meilleur premier coup
  jouerCoup(etat, meilleurCoup(racine) );
  
  printf("nombre de victoire : %d\n",racine->nb_victoires);
  printf("nombre de simulations : %d\n",iter);
  printf("probabilite de victoire : %f\n",(float)racine->nb_victoires/iter);
  //jouerCoup(etat, meilleur_coup );
  // Penser à libérer la mémoire :
  free (coups);
  freeNoeud(racine);
}

int main(void) {

  Coup * coup;
  FinDePartie fin;
	
  // initialisation
  Etat * etat = etat_initial(); 
	
  // Choisir qui commence : 
  printf("Qui commence (0 : humain, 1 : ordinateur) ? ");
  scanf("%d", &(etat->joueur) );
	
  // boucle de jeu
  srand(time(NULL));
  do {
    printf("\n");
    afficheJeu(etat);
		
    if ( etat->joueur == 0 ) {
      // tour de l'humain
			
      do {
	coup = demanderCoup(etat);
      } while ( !jouerCoup(etat, coup) );
									
    }
    else {
      // tour de l'Ordinateur
			
      ordijoue_mcts( etat, TEMPS );
			
    }
		
    fin = testFin( etat );
  }	while ( fin == NON ) ;

  printf("\n");
  afficheJeu(etat);
		
  if ( fin == ORDI_GAGNE )
    printf( "** L'ordinateur a gagné **\n");
  else if ( fin == MATCHNUL )
    printf(" Match nul !  \n");
  else
    printf( "** BRAVO, l'ordinateur a perdu  **\n");
  return 0;
}
