#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>


// Gloabal variables
int n[4] = {3, 5, 50, 80};
int vecnum[4] = {1, 2, 3, 4};
char datadir[100] = "../inputfiles/";
char timelog[100] = "./timetaken.log";


// Function signatures
double** read_input_matrix(int n);
double* read_input_vector(int n, int vecnum);
bool is_eigenvector(double** mat, double* vec, int n, double *lambda);
void matrix_vector_product(double** mat, double* vec, int n, double* product);
void free_matrix(double** mat, int n);
void write_eigenvalue_to_file(int n, int vecnum, double lambda);


int main() {
    int i, j;
    FILE* timelogfile = fopen(timelog, "w");
    if (!timelogfile) {
        printf("Error opening file: %s\n", timelog);
        exit(1);
    }

    for (i=0; i<4; i++) {
        // Read input matrix
        double** mat = read_input_matrix(n[i]);

        double total_time_taken = 0.0;
        clock_t start, end;

        for (j=0; j<4; j++) {
            // Read input vector
            double* vec = read_input_vector(n[i], vecnum[j]);

            // Check if it is an eigenvector
            double lambda;
            start = clock();
            bool is_eigen = is_eigenvector(mat, vec, n[i], &lambda);
            end = clock();
            total_time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;

            if (is_eigen) {
                // Print to screen
                printf("vec_%06d_%06d.in : Yes : %.6f\n", n[i], vecnum[j], lambda);
                // Append to file
                write_eigenvalue_to_file(n[i], vecnum[j], lambda);
            } else {
                // Print to screen
                printf("vec_%06d_%06d.in : Not an eigenvector\n", n[i], vecnum[j]);
            }
            free(vec);
        }
        free_matrix(mat, n[i]);
        double avg_time_taken = (double)total_time_taken / 4;
        fprintf(timelogfile, "n = %d, avg time taken = %.8f\n", n[i], avg_time_taken);

    }
    fclose(timelogfile);
    return 0;
}


double** read_input_matrix(int n) {
    int i; 

    // Memory allocation
    double** mat = (double**)malloc(sizeof(double*) * n);
    for (i = 0; i < n; i++) {
        mat[i] = (double*)malloc(sizeof(double) * n);
    }

    // Filepath
    char filepath[256];
    sprintf(filepath, "%smat_%06d.in", datadir, n);
    FILE* inputfile = fopen(filepath, "r");
    if (!inputfile) {
        printf("Error opening file: %s\n", filepath);
        free_matrix(mat, n);
        exit(1);
    }

    // Reading the file
    char* line = NULL;
    size_t len = 0;
    int row = 0;

    // Read matrix data line by line
    while (getline(&line, &len, inputfile) != -1 && row < n) {
        char* token = strtok(line, ","); // Tokenization
        int col = 0;
        while (token && col < n) { 
            mat[row][col++] = atof(token); // string to double
            token = strtok(NULL, ",");
        }
        row++;
    }

    // Cleanup
    free(line);
    fclose(inputfile);

    return mat;
}


double* read_input_vector(int n, int vecnum) {
    // Memory allocation
    double* vec = (double*)malloc(sizeof(double) * n);

    // Filepath
    char filepath[256];
    sprintf(filepath, "%svec_%06d_%06d.in", datadir, n, vecnum);
    FILE* inputfile = fopen(filepath, "r");
    if (!inputfile) {
        printf("Error opening file: %s\n", filepath);
        free(vec);
        exit(1);
    }

    // Reading the file
    char* line = NULL;
    size_t len = 0;
    int i = 0;
    getline(&line, &len, inputfile);
    char* token = strtok(line, ","); // Tokenization
    while (token && i < n) {
        vec[i++] = atof(token); // string to double
        token = strtok(NULL, ",");
    }

    // Cleanup
    free(line);
    fclose(inputfile);

    return vec;
}


bool is_eigenvector(double** mat, double* vec, int n, double* lambda) {
    double* product = (double*)malloc(sizeof(double) * n);
    matrix_vector_product(mat, vec, n, product);

    // Check for first non-zero value in the vector
    int non_zero_index = -1, i;
    for (i=0; i<n; i++) {
        if (vec[i] != 0) {
            non_zero_index = i;
            break;
        }
    }

    // Zero vector cannot be eigen vector
    if (non_zero_index == -1) {
        return false;
    }

    // Forward pass starting from first non-zero element
    double temp = (double)product[non_zero_index] / vec[non_zero_index];
    for (i=non_zero_index+1; i<n; i++) {
        if (vec[i] != 0) { // Only check for non-zero elements
            if (fabs(product[i] - temp * vec[i]) > 1e-6) {
                free(product);
                return false;
            }
        }
    }

    *lambda = temp;
    free(product);
    return true;
}


void matrix_vector_product(double** mat, double* vec, int n, double* product) {
    int i, j;
    for (i=0; i<n; i++) {
        product[i] = 0;
        for (j=0; j<n; j++) {
            product[i] += mat[i][j] * vec[j];
        }
    }
    return;
}


void free_matrix(double** mat, int n) {
    int i;
    for (i=0; i<n; i++) {
        free(mat[i]);
    }
    free(mat);
    return;
}


void write_eigenvalue_to_file(int n, int vecnum, double lambda) {
    // Filepath
    char filepath[256];
    sprintf(filepath, "%svec_%06d_%06d.in", datadir, n, vecnum);
    FILE* outfile = fopen(filepath, "a");
    if (!outfile) {
        printf("Error opening file: %s\n", filepath);
        exit(1);
    }

    // Appending to file
    fprintf(outfile, "\nEigenvalue: %.6f\n", lambda);
    fclose(outfile);
    return;
}
