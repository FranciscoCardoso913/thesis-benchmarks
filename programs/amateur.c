#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>



/**
 * Verifies if input given is actually a long integer and assigns it to "n".
 */
long verifyInput(char* arg, long *n) {
    char** endptr;
    *n = strtol(arg, endptr, 10); 
    return **endptr == '\0';
}

/**
 * Core logic extracted into its own function to test interprocedural boundaries.
 * * CORAL CONTRACT: We demand that 'theStr' and 'alreadySeen' 
 * are strictly NOT_NULL when this function is called.
 */
void extract_fragments(int fd, char* theStr, long* alreadySeen, long numberFrags, long maxFragSize, long tam) {
    int arraySize = 0;
    
    while (numberFrags != 0) {
        long aTry;
        int alreadyStored;
        while (1) {
            alreadyStored = 0;
            aTry = rand() % (tam - maxFragSize + 1); 
            for (int i = 0; i < arraySize; i++) {
                if (aTry == alreadySeen[i]) {
                    alreadyStored = 1;
                    break;
                }
            }
            if (!alreadyStored) break;
        }
        lseek(fd, aTry, SEEK_SET);
        read(fd, theStr, maxFragSize);

        theStr[maxFragSize - 1] = '\0'; 

        long idx = 0;
        printf(">");
        
        while (theStr[idx] != '\0') {
            if (theStr[idx] == '\n') printf(" ");
            else printf("%c", theStr[idx]);
            idx++;
        }
        printf("<\n");
        
        alreadySeen[arraySize++] = aTry;
        numberFrags--;
    }
}

int main (int argc, char** argv) {
    if (argc != 4) {
        fprintf(stderr, "Usage: samples file numberfrags maxfragsize\n");
        return EXIT_FAILURE;
    }

    if( argv == NULL) return -1;

    long numberFrags, maxFragSize;
    char * argv_2 = argv[2];
    char * argv_3 = argv[3];
    if (argv_2 == NULL) return -1;
  
    if (!verifyInput(argv_2, &numberFrags) ) {
        fprintf(stderr, "numberfrags and maxfragsize must be integers\n");
        return EXIT_FAILURE;
    }
    if (argv_3 == NULL) return -1;
    if (!verifyInput(argv_3, &maxFragSize)) {
        fprintf(stderr, "numberfrags and maxfragsize must be integers\n");
        return EXIT_FAILURE;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("Couldn't read file");
        return EXIT_FAILURE;
    }

    long tam = lseek(fd, 0, SEEK_END); 

    char *theStr=  (char*) malloc(maxFragSize * sizeof(char)); ;
    long *alreadySeen =  (long*) malloc((numberFrags) * sizeof(long));


    srand(0);

    // 4. Execute core logic
    // CORAL VIOLATION: main() is passing MAYBE_NULL pointers to extract_fragments()
    // which explicitly requires NOT_NULL. CORAL will throw a PreconditionViolationError!
    extract_fragments(fd, theStr, alreadySeen, numberFrags, maxFragSize, tam);

    // 5. Cleanup
    free(theStr);
    free(alreadySeen);
    close(fd);

    return 0;
}

