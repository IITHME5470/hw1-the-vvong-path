#include <stdio.h>
#include <stdlib.h>

// Function signatures
void read_input_data(char* filename, int* n);
double** create_array(int n);
void free_array(double** arr, int n);
void print_to_file(double** arr, int n, int format_flag);

// Global variables
char inputdir[100] = "./input/";
char outputdir[100] = "./output/";


int main() {
    // Read from input file
    int n;
    read_input_data("input.in", &n);

    // Dynamic allocation and initialization of array
    double** arr = create_array(n);

    // Format flag
    int format_flag;
    printf("Enter format flag (ASCII - 0, Binary - 1): ");
    scanf("%d", &format_flag);

    // Print to file
    print_to_file(arr, n, format_flag);

    // Free array
    free_array(arr, n);
    return 0;
}


void read_input_data(char* filename, int* n) {
    // Filepath
    char filepath[100];
    sprintf(filepath, "%s%s", inputdir, filename);
    FILE* inputfile = fopen(filepath, "r");
    if (!inputfile) {
        printf("Error opening file: %s\n", filepath);
        exit(1);
    }

    fscanf(inputfile, "%d", n);
    fclose(inputfile);
    return;
}


double** create_array(int n) {
    double** arr = (double**)malloc(sizeof(double*) * n);
    int i, j;
    for (i=0; i<n; i++) {
        arr[i] = (double*)malloc(sizeof(double) * n);
        for (j=0; j<n; j++) {
            arr[i][j] = i + j;
        }
    }
    return arr;
}


void free_array(double** arr, int n) {
    int i;
    for (i=0; i<n; i++) {
        free(arr[i]);
    }
    free(arr);
}


void print_to_file(double** arr, int n, int format_flag) {
    // Filepath
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "%sarray_%06d_%s.out", outputdir, n, format_flag == 0 ? "asc" : "bin");
    FILE* outfile = fopen(filepath, format_flag == 0 ? "w" : "wb");
    if (!outfile) {
        printf("Error opening file: %s\n", filepath);
        free_array(arr, n);
        exit(1);
    }

    if (format_flag == 0) {
        int i, j;
        for (i=0; i<n; i++) {
            for (j=0; j<n; j++) {
                fprintf(outfile, "%.15lf ", arr[i][j]);
            }
            fprintf(outfile, "\n");
        }
    } else if (format_flag == 1) {
        int i;
        for (i=0; i<n; i++) {
            fwrite(arr[i], sizeof(double), n, outfile);
        }
    }
    fclose(outfile);
    return;
}
