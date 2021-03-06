/**
 * A simple main function for running the Lyra Password Hashing Scheme (PHS).
 * 
 * Author: The Lyra PHC team (http://www.lyra-kdf.net/) -- 2014.
 * 
 * This software is hereby placed in the public domain.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ''AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include "Lyra.h"
#include "Sponge.h"

/**
 * Generates the test vectors for Lyra.
 *
 * @param t Parameter to determine the processing time (T)
 * @param nRows  Memory cost parameter (defines the number of rows of the memory matrix, R)
 */
int testVectors(unsigned int  tCost, unsigned int  nRows) {
    //=================== Basic variables, with default values =======================//
    int kLen = 64;
    unsigned char *pwd;
    int pwdLen = 11;
    unsigned char *salt;
    int saltLen = 16;
	
	srand(time(NULL));
	int i;
    int countSample;
    int indexSalt = 0;
    //==========================================================================/

    unsigned char *K = malloc(kLen);

    /* Generating vectors with the input size varying from 0 to 128 bytes,
     * and values varying from 0 to 127. The salt size is fixed in 16 bytes, 
	 * and its value varies from 0 to 256.
	 */
    for (countSample = 0; countSample <= 128; countSample++) {
        pwdLen = countSample;
        int count;
        pwd = malloc(sizeof (pwd) * pwdLen);
        for (count = 0; count < pwdLen; count++) {
            pwd[count] = count;
        }

        salt = malloc(sizeof (salt) * saltLen);
        for (count = 0; count < saltLen; count++) {
            salt[count] = saltLen * indexSalt + count;
        }
        indexSalt++;
        if (indexSalt == saltLen)
            indexSalt = 0;

        lyra(K, kLen, (unsigned char*) pwd, pwdLen, (unsigned char*) salt, saltLen, tCost, nRows, N_COLS);

        printf("\ninlen: %d\n", pwdLen);
        printf("t_cost: %d\n", tCost);
        printf("m_cost: %d\n", nRows);
        printf("outlen: %d\n", kLen);

        printf("In: ");
        for (i = 0; i < pwdLen; i++) {
            printf("%02x ", pwd[i]);
        }
        printf("\n");

        printf("Salt: ");
        for (i = 0; i < saltLen; i++) {
            printf("%02x ", salt[i]);
        }
        printf("\n");

        printf("Out: ");
        for (i = 0; i < kLen; i++) {
            printf("%02x ", K[i]);
        }
        printf("\n");
    }
    
	/* Generating vectors with the input size varying from 0 to 128 bytes,
     * and values varying from 128 to 255. The salt size is fixed in 16 bytes, 
	 * and its value varies from 0 to 256.
	 */
    for (countSample = 128; countSample < 256; countSample++) {
        pwdLen = countSample - 127;
        int count;
        pwd = malloc(sizeof (pwd) * pwdLen);
        for (count = 0; count < pwdLen; count++) {
            pwd[count] = count + 128;
        }

        salt = malloc(sizeof (salt) * saltLen);
        for (count = 0; count < saltLen; count++) {
            salt[count] = saltLen * indexSalt + count;
        }
        indexSalt++;
        if (indexSalt == saltLen)
            indexSalt = 0;

        lyra(K, kLen, (unsigned char*) pwd, pwdLen, (unsigned char*) salt, saltLen, tCost, nRows, N_COLS);

        printf("\ninlen: %d\n", pwdLen);
        printf("t_cost: %d\n", tCost);
        printf("m_cost: %d\n", nRows);
        printf("outlen: %d\n", kLen);

        printf("In: ");
        for (i = 0; i < pwdLen; i++) {
            printf("%02x ", pwd[i]);
        }
        printf("\n");

        printf("Salt: ");
        for (i = 0; i < saltLen; i++) {
            printf("%02x ", salt[i]);
        }
        printf("\n");

        printf("Out: ");
        for (i = 0; i < kLen; i++) {
            printf("%02x ", K[i]);
        }
        printf("\n");
    }
    return 0;
}

int main(int argc, char *argv[]) {
    //=================== Basic variables, with default values =======================//
    int kLen = 64;
    int tCost = 0;
    int nRows = 0;
    char *pwd = "Lyra sponge";
    int pwdLen = 11;
    char *salt = "saltsaltsaltsalt";
    int saltLen = 16;
    //==========================================================================/
    
    switch (argc) {
        case 2:
            if (strcmp(argv[1], "--help") == 0) {
                printf("Usage: \n");
                printf("       Lyra pwd salt kLen tCost nRows \n\n");
                printf("Inputs:\n");
                printf(" - pwd: the password\n");
                printf(" - salt: the salt\n");
                printf(" - kLen: output size\n");
                printf(" - tCost: the time cost parameter\n");
                printf(" - nRows: the number of rows parameter\n");
                printf("\n");
                printf("Or:\n");
                printf("       Lyra tCost nRows --testVectors (to generate test vectors and test Lyra operation)\n\n");
                return 0;
            } else {
                printf("Invalid options.\nFor more information, try \"Lyra --help\".\n");
                return 0;
            }
            break;
        case 6:
            pwd = argv[1];
            pwdLen = strlen(pwd);
            salt = argv[2];
            saltLen = strlen(salt);
            kLen = atoi(argv[3]);
            tCost = atoi(argv[4]);
            nRows = atoi(argv[5]);
            break;
        case 4:
            if (strcmp(argv[3], "--testVectors") == 0) {
                tCost = atoi(argv[1]);
                nRows = atoi(argv[2]);
                testVectors(tCost, nRows);
                return 0;
            } else {
                printf("Invalid options.\nFor more information, try \"Lyra --help\".\n");
                return 0;
            }
            break;
        default:
            printf("Invalid options.\nTry \"Lyra --help\" for help.\n");
            return 0;
    }
    
    unsigned char *K = malloc(kLen);

    printf("Inputs: \n");
    printf("\tPassword: %s\n", pwd);
    printf("\tPassword Size: %d\n", pwdLen);
    printf("\tSalt: %s\n", salt);
    printf("\tOutput Size: %d\n", kLen);
    printf("------------------------------------------------------------------------------------------------------------------------------------------\n");

    printf("Parameters: \n");
    printf("\tT: %d\n", tCost);
    printf("\tR: %d\n", nRows);
    printf("\tC: %d\n", N_COLS);
    printf("\tMemory: %ld bits\n", ((long) (N_COLS * nRows * BLOCK_LEN_BYTES)));
    printf("------------------------------------------------------------------------------------------------------------------------------------------\n");


    switch (lyra(K, kLen, (unsigned char*) pwd, pwdLen, (unsigned char*) salt, saltLen, tCost, nRows, N_COLS)) {
	case 0:
	    printf("Output: \n");

	    printf("\n\tK: ");
	    int i;
	    for (i = 0; i < kLen; i++) {
		printf("%x|", K[i]);
	    }
	    break;
	case -1:
	    printf("Error: unable to allocate memory (R too large?)\n");
	    break;
	default:
	    printf("Unexpected error\n");
	    break;
    }
     printf("\n");
    printf("------------------------------------------------------------------------------------------------------------------------------------------\n");

    return 0;
}

