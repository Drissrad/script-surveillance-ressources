#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define MAX_LINE_LENGTH 100
#define MAX_FILES 100

float calculate_average(FILE *file, const char *usage_type) {
    char line[MAX_LINE_LENGTH];
    float total = 0;
    int count = 0;

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        strtok(line, ","); 

        float usage;
        if (strcmp(usage_type, "cpu") == 0) {
            usage = atof(strtok(NULL, ","));
        } else {
            usage = atof(strtok(NULL, "G"));
        }

        total += usage;
        count++;
    }

    if (count == 0) {
        fprintf(stderr, "No data found in file\n");
        exit(1);
    }

    return total / count;
}

int is_within_date_range(const char *filename, const char *start_date, const char *end_date, const char *usage_type) {
    char date[9];
    int number_of_characters = 0;
    
    if(strcmp(usage_type, "cpu") == 0 || strcmp(usage_type, "ram") == 0) {
        number_of_characters = 10;
    } else {
        number_of_characters = 11;
    }

    strncpy(date, filename + number_of_characters, 8);
    date[8] = '\0'; 

    if ((start_date == NULL || strcmp(date, start_date) >= 0) && (end_date == NULL || strcmp(date, end_date) <= 0)) {
        printf("Within Date: %s\n", date);
        return 1; 
    } else {
        printf("Outside Date: %s\n", date);
        return 0; 
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2 && argc != 4) {
        fprintf(stderr, "Usage: %s <usage_type> [<start_date> <end_date>]\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *usage_type = argv[1];
    char *start_date = NULL;
    char *end_date = NULL;

    if (argc == 4) {
        start_date = argv[2];
        end_date = argv[3];
    }

    char *prefix;
    if (strcmp(usage_type, "ram") == 0) {
        prefix = "ram_usage_";
    } else if (strcmp(usage_type, "cpu") == 0) {
        prefix = "cpu_usage_";
    } else if (strcmp(usage_type, "disk") == 0) {
        prefix = "disk_usage_";
    } else {
        fprintf(stderr, "Invalid usage type. Must be 'ram', 'cpu', or 'disk'.\n");
        return EXIT_FAILURE;
    }

    char *dates[MAX_FILES];
    float averages[MAX_FILES];
    int file_count = 0;

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(".")) == NULL) {
        perror("Error opening directory");
        return EXIT_FAILURE;
    }

    while ((ent = readdir(dir)) != NULL) {
        if (strstr(ent->d_name, prefix) != NULL && strstr(ent->d_name, ".txt") != NULL) {
            if ((start_date == NULL || end_date == NULL) || is_within_date_range(ent->d_name, start_date, end_date, usage_type)) {
                printf("Reading data from file: %s\n", ent->d_name);
                FILE *file = fopen(ent->d_name, "r");
                if (file == NULL) {
                    fprintf(stderr, "Error opening file %s\n", ent->d_name);
                    continue;
                }

                float average = calculate_average(file, usage_type);
                printf("Average usage for %s: %.2f\n", ent->d_name, average);

                    int number_of_characters = 0;
    
		    if(strcmp(usage_type, "cpu") == 0 || strcmp(usage_type, "ram") == 0) {
			number_of_characters = 10;
		    } else {
			number_of_characters = 11;
		    }
                char *date = strdup(ent->d_name + number_of_characters);
                date[strlen(date) - 4] = '\0'; 

                dates[file_count] = date;
                averages[file_count] = average;
                file_count++;

                fclose(file);
            }
        }
    }

    closedir(dir);

    FILE *gnuplot_pipe = popen("gnuplot -persist", "w");
    if (gnuplot_pipe == NULL) {
        fprintf(stderr, "Error opening pipe to gnuplot\n");
        return EXIT_FAILURE;
    }

    fprintf(gnuplot_pipe, "set xlabel 'Date'\n");
    fprintf(gnuplot_pipe, "set ylabel 'Average Usage'\n");
    fprintf(gnuplot_pipe, "set xtics rotate by -45\n");
    fprintf(gnuplot_pipe, "set title '%s Usage'\n", usage_type);
    fprintf(gnuplot_pipe, "set xtics nomirror\n");

    fprintf(gnuplot_pipe, "plot '-' using 2:xticlabels(1) with linespoints title 'Average Usage'\n");

    for (int i = 0; i < file_count; i++) {
        fprintf(gnuplot_pipe, "%s %.2f\n", dates[i], averages[i]);
    }

    fprintf(gnuplot_pipe, "e\n");

    pclose(gnuplot_pipe);

    for (int i = 0; i < file_count; i++) {
        free(dates[i]);
    }

    return 0;
}
