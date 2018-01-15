/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;
    int n0, n1, n2, n3, n4, n5, n6, n7;

    for (i = 0; i <= 7; i++) {
      n0 = A[i][0];
      n1 = A[i][1];
      n2 = A[i][2];
      n3 = A[i][3];
      n4 = A[i][4];
      n5 = A[i][5];
      n6 = A[i][6];
      n7 = A[i][7];
      B[0][i] = n0;
      B[1][i] = n1;
      B[2][i] = n2;
      B[3][i] = n3;
      B[4][i] = n4;
      B[5][i] = n5;
      B[6][i] = n6;
      B[7][i] = n7;
      for (j = 0; j <= 7; j++) {
        B[j][i] = A[i][j];  
      }
    }
    
    for (i = 0; i <= 7; i++) {
      for (j = 8; j <= 15; j++) {
        B[j][i] = A[i][j];
      }
    }

    for (i = 0; i <= 7; i++) {
      for (j = 16; j <= 23; j++) {
        B[j][i] = A[i][j];
      }
    }

    for (i = 0; i <= 7; i++) {
      for (j = 24; j <= 31; j++) {
        B[j][i] = A[i][j];
      }
    }
    
    for (i = 8; i <= 15; i++) {
      for (j = 0; j <= 7; j++) {
        B[j][i] = A[i][j];
      }
    }
    for (i = 8; i <= 15; i++) {
      for (j = 8; j <= 15; j++) {
        B[j][i] = A[i][j];
      }
    }
    for (i = 8; i <= 15; i++) {
      for (j = 16; j <= 23; j++) {
        B[j][i] = A[i][j];
      }
    }
    for (i = 8; i <= 15; i++) {
      for (j = 24; j <= 31; j++) {
        B[j][i] = A[i][j];
      }
    }
    
    for (i = 16; i <= 23; i++) {
      for (j = 0; j <= 7; j++) {
        B[j][i] = A[i][j];
      }
    }

    for (i = 16; i <= 23; i++) {
      for (j = 8; j <= 15; j++) {
        B[j][i] = A[i][j];
      }
    }

    for (i = 16; i <= 23; i++) {
      for (j = 16; j <= 23; j++) {
        B[j][i] = A[i][j];
      }
    }

    for (i = 16; i <= 23; i++) {
      for (j = 24; j <= 31; j++) {
        B[j][i] = A[i][j];
      }
    }

    for (i = 24; i <= 31; i++) {
      for (j = 0; j <= 7; j++) {
        B[j][i] = A[i][j];
      }
    }

    for (i = 24; i <= 31; i++) {
      for (j = 8; j <= 15; j++) {
        B[j][i] = A[i][j];
      }
    }

    for (i = 24; i <= 31; i++) {
      for (j = 16; j <= 23; j++) {
        B[j][i] = A[i][j];
      }
    }

    for (i = 24; i <= 31; i++) {
      for (j = 24; j <= 31; j++) {
        B[j][i] = A[i][j];
      }
    }

}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/* 
 Tranpose eight at a time:
 A(0,0) ~ A(0,7) -> B(0,0) B(1,0) ... B(7,0)
 A(1,0) ~ A(1,7) -> B(0,1) B(1,1) ... B(7,1)
 ...
 A(7,0) ~ A(7,7) -> B(0,7) B(1,7) ... B(7,7)
 
 A(0,8) ~ A(0,15) -> B(8,0) B(9,0) ... B(15,0)
 A(1,8) ~ A(1,15) -> B(8,1) B(9,1) ... B(15,1)
 ...
 A(7,8) ~ A(7
 */
char transpose_eight_desc[] = "Transpose eight";
void transpose_eight(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i <= 7; i++) {
      for (j = 0; j <= 7; j++) {
        B[j][i] = A[i][j];  
      }
    }
    
    for (i = 0; i <= 7; i++) {
      for (j = 8; j <= 15; j++) {
        B[j][i] = A[i][j];
      }
    }

    for (i = 0; i <= 7; i++) {
      for (j = 16; j <= 23; j++) {
        B[j][i] = A[i][j];
      }
    }

    for (i = 0; i <= 7; i++) {
      for (j = 24; j <= 31; j++) {
        B[j][i] = A[i][j];
      }
    }
    
    for (i = 8; i <= 15; i++) {
      for (j = 0; j <= 7; j++) {
        B[j][i] = A[i][j];
      }
    }
    for (i = 8; i <= 15; i++) {
      for (j = 8; j <= 15; j++) {
        B[j][i] = A[i][j];
      }
    }
    for (i = 8; i <= 15; i++) {
      for (j = 16; j <= 23; j++) {
        B[j][i] = A[i][j];
      }
    }
    for (i = 8; i <= 15; i++) {
      for (j = 24; j <= 31; j++) {
        B[j][i] = A[i][j];
      }
    }
    
    for (i = 16; i <= 23; i++) {
      for (j = 0; j <= 7; j++) {
        B[j][i] = A[i][j];
      }
    }

    for (i = 16; i <= 23; i++) {
      for (j = 8; j <= 15; j++) {
        B[j][i] = A[i][j];
      }
    }

    for (i = 16; i <= 23; i++) {
      for (j = 16; j <= 23; j++) {
        B[j][i] = A[i][j];
      }
    }

    for (i = 16; i <= 23; i++) {
      for (j = 24; j <= 31; j++) {
        B[j][i] = A[i][j];
      }
    }

    for (i = 24; i <= 31; i++) {
      for (j = 0; j <= 7; j++) {
        B[j][i] = A[i][j];
      }
    }

    for (i = 24; i <= 31; i++) {
      for (j = 8; j <= 15; j++) {
        B[j][i] = A[i][j];
      }
    }

    for (i = 24; i <= 31; i++) {
      for (j = 16; j <= 23; j++) {
        B[j][i] = A[i][j];
      }
    }

    for (i = 24; i <= 31; i++) {
      for (j = 24; j <= 31; j++) {
        B[j][i] = A[i][j];
      }
    }

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

