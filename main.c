#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SEC 512

typedef struct partition{
    char Nom[5];
    short int type;
    char SGF[10];
    long int taille;
    int numSecDebut;
    long int cluster;
    long int adrTabAlloc;

} partition;

void afficher_partition(partition P)
{
  printf("\n Partition : %s",P.Nom);
  if (P.type==0) printf("\n Type : principale");
  else printf("\n Type : �tendue");

  printf("\n Syst�me de fichier : %s",P.SGF);
  
  if (P.taille>=1024*1024) printf("\n Taille : %ld",P.taille/(1024*1024));
  else printf("\n Taille : %ld",P.taille/1024);
  
  printf("\n Secteur debut : %d",P.numSecDebut);
  printf("\n Taille du cluster : %ld",P.cluster/1024);

  printf("\n Adresse de la table d'allocation MFT/FAT : %ld",P.adrTabAlloc);

}

int convertOctetToInt(char octet[2])
{
    int i;
    int pos[2]={0};

    for(i=0;i<2;i++)
    {
      switch(octet[i])
      {
    case '0': pos[i]=0; break;
    case '1': pos[i]=1; break;
    case '2': pos[i]=2; break;
    case '3': pos[i]=3; break;
    case '4': pos[i]=4; break;
    case '5': pos[i]=5; break;
    case '6': pos[i]=6; break;
    case '7': pos[i]=7; break;
    case '8': pos[i]=8; break;
    case '9': pos[i]=9; break;
    case 'A': pos[i]=10; break;
    case 'B': pos[i]=11; break;
    case 'C': pos[i]=12; break;
    case 'D': pos[i]=13; break;
    case 'E': pos[i]=14; break;
    case 'F': pos[i]=15; break;
    default: break;
      }
    }

    return (pos[0]+16*pos[1]);
}
void Afficher_secteur(unsigned char secteur[MAX_SEC],int taille) /// En mode console
{
    int j,i=0;

    //system("cls");
    printf("\n\n");
    printf("\n Affichage Secteur %d\n",0);
    while(i<taille)
    {
      printf("\n----------------------------------------------------\n");
      printf("%d :",i);
      for (j=i; j<i+16; j++)
      {
          printf("%02x ", secteur[j]);// afficher les octets en hexa
      }
      i+=16;
    }
     printf("\n\n Fin de secteur \n \n");
}

int main()
{
    FILE *dispositif = NULL;
    unsigned char secteur[MAX_SEC];
    int numSecteur=0;

    /// Lecture du secteur 0
    dispositif = fopen("\\\\.\\C:","rb");// ouvrir le dispositif

    if(dispositif == NULL)
    {
        printf("Dispositif non ouvert \n ");
    }

    else{
    if (fseek(dispositif,0,SEEK_SET)!=0) // positioner la tete
    {
    printf("Erreur de deplacement de tete de lecture \n");
    }
    else {
    fread(secteur , 512 ,1 , dispositif);// lire le secteur concerner
    }
    }
    fclose(dispositif);

    Afficher_secteur(secteur,512);
    getchar();

    /// R�cup�ration des diff�rentes sections
    /*** variables ***/
    char boot[447];
    char descripteurs[4][16];
    int i,j;

    /// R�cup�ration BOOT
    for (i=0; i<446; i++)
    {
        boot[i]=secteur[i];
    }
    Afficher_secteur(boot,446);

    /// R�cup�ration descripteurs
    for(i=0;i<4;i++)
    {
        for(j=0;j<16;j++)
        {
            descripteurs[i][j]=secteur[16*i+j+446];
        }

        printf("\n DESCRIPTEUR DE PARTITION : %d \n",i);
        Afficher_secteur(descripteurs[i],16);
    }

    partition Part[4];

    /// Remplissage des descripteurs de partition
    for (i=0; i<4; i++)
    {
	// Nom de la partition
	sprintf(Part[i].Nom,"X");

	// Type de la partition
	if(descripteurs[i][0]==480) Part[i].type=0; // Partition principale
	else Part[i].type=1; // Partition etendue

	// Syst�me de fichier
	sprintf(Part[i].SGF,"None");

	// Taille de la partition
	Part[i].taille=0;
	
	// Secteur debut
	Part[i].numSecDebut=-1;

        // Taille du cluster
	Part[i].cluster=0;

 	// Adresse de la table MFT/FAT
	Part[i].adrTabAlloc=-1;

	afficher_partition(Part[i]);
    }

    		
    getchar();
    getchar();
    return 0;
}
