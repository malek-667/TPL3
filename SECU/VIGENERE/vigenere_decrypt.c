#include <stdio.h>
#include <stdlib.h>

char decrypt(char key, char letter) {
	int i,u;

	int fl = 'A';

	if ((letter - key) < 0)
		return (letter - key + 26)% 26 + fl;
	else
		return (letter - key)% 26 + fl;
}

int main(int argc, char  *argv[]) {
	int fl = 'A';
	int ll = 'Z';
	
	FILE* fileR = NULL;
	FILE* fileW = NULL;
	int currentChar;
	int iterattor = 0;

	if (argc == 4) {

		fileR = fopen(argv[2], "r");
		fileW = fopen(argv[3], "w");

		if (fileR != NULL) {

            do {
                currentChar = fgetc(fileR);

                if (currentChar <= ll && currentChar >= fl) {
                    fputc(decrypt(argv[1][iterattor], currentChar), fileW);

				iterattor++;
				if (argv[1][iterattor] == '\0') {
					iterattor = 0;
				}

                } else {
                    if (currentChar != EOF)
                    	fputc(currentChar, fileW);
                }


            } while (currentChar != EOF);
        } else {
            printf("Impossible d'ouvrir le fichier %s\n", fileR);
            return 2;
        }
	} else {
		printf("USAGE: %s cle_decalage fichier_a_dechiffrer fichier_sortie\n", argv[0]);
		return 1;
	}

	fclose(fileR);
	fclose(fileW);

	return 0;
}