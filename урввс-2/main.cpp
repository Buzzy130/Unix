#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h> // Include the time.h header for time function

int main() {
    FILE* file;
    file = fopen("results.txt", "w");
    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    srand((unsigned int)time(NULL)); // Cast the result of time to unsigned int
    int n = rand() % 10 + 1; // Generate a random number of calculations from 1 to 10
    for (int i = 0; i < n; i++) {
        double x = ((double)rand() / RAND_MAX) * 2 - 1; // Generate a random number x from [-1; 1]
        double result = sin(x); // Calculate sin(x)
        fprintf(file, "%f\n", result); // Write the result to the file
    }
    fclose(file);

    pid_t pid = fork();
    if (pid < 0) {
        printf("Fork failed\n");
        return 1;
    }
    else if (pid == 0) {
        // Child process
        file = fopen("results.txt", "r");
        if (file == NULL) {
            printf("Error opening file\n");
            return 1;
        }
        double result;
        while (fscanf(file, "%lf", &result) != EOF) {
            printf("%f\n", result); // Output the result to the screen
        }
        fclose(file);
    }
    else {
        // Parent process
        sleep(1); // Wait a bit to give the child time to open the file
        file = fopen("results.txt", "a");
        if (file == NULL) {
            printf("Error opening file\n");
            return 1;
        }
        fprintf(file, "STOP\n"); // Send the keyword
        fclose(file);
        wait(NULL); // Wait for the child process to finish
    }

    return 0;
}

