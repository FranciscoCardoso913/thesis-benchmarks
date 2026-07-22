#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h> // Required for open() and O_RDONLY


#pragma coral null __endptr{*__endptr: maybe-null -> not-null} : maybe-null -> not-null
#pragma coral null __nptr : maybe-null -> not-null
long strtol(const char *__restrict__ __nptr, char **__restrict__ __endptr, int __base);


#pragma coral null __buf : not-null -> not-null
ssize_t read(int __fd, void *__buf, size_t __nbytes);

/**
 * Verifies if input given is actually a long integer and assigns it to "n".
 */
#pragma coral null arg : not-null -> not-null
#pragma coral null n : not-null -> not-null
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
#pragma coral null theStr : not-null -> not-null
#pragma coral null alreadySeen : not-null -> not-null
void extract_fragments(int fd, char* theStr, long* alreadySeen, long numberFrags, long maxFragSize, long tam) {
    int arraySize = 0;
    
    while (numberFrags != 0) {
        long aTry;
        int alreadyStored;

        // Generate "numberfrags" different numbers
        while (1) {
            alreadyStored = 0;
            aTry = rand() % (tam - maxFragSize + 1); 
            
            // If alreadySeen was NULL, this would segfault!
            for (int i = 0; i < arraySize; i++) {
                if (aTry == alreadySeen[i]) {
                    alreadyStored = 1;
                    break;
                }
            }
            if (!alreadyStored) break;
        }
        // Gets the string it should read using low-level lseek and read
        lseek(fd, aTry, SEEK_SET);
        read(fd, theStr, maxFragSize);

        
        
        // (Fix: amateur code didn't null-terminate the string)
        theStr[maxFragSize - 1] = '\0'; 

        long idx = 0;
        printf(">");
        
        // Prints the string replacing newlines with spaces
        // If theStr was NULL, this would segfault!
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
    // 1. Check basic arguments
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

    // 2. Open file using POSIX open() and check for errors
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("Couldn't read file");
        return EXIT_FAILURE;
    }

    // Get the size of the file using lseek
    long tam = lseek(fd, 0, SEEK_END); 
    
    // 3. Allocate memory
    // FLAW: malloc returns MAYBE_NULL, but the amateur forgets to check it!
    char *theStr ;

    while (1){
        theStr =  (char*) malloc(maxFragSize * sizeof(char));
        if (theStr != NULL) break;
    }

    
    long *alreadySeen;

    while (1){
        alreadySeen =  (long*) malloc((numberFrags) * sizeof(long));  
        if (alreadySeen != NULL) break;
    }

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
