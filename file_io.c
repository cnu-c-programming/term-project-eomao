#include "file_io.h"

#include <stdio.h>
#include <string.h>

#define CSV_LINE_SIZE 256
#define CSV_HEADER "id,name,score"

static void trim_newline(char *line) {
    size_t len = strlen(line);

    while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
        line[len - 1] = '\0';
        len--;
    }
}

static int parse_student_line(char *line, int *id, char *name, int *score) {
    char extra;

    if (strchr(line, ',') == NULL) {
        return 0;
    }

    if (sscanf(line, "%d,%31[^,],%d%c", id, name, score, &extra) != 3) {
        return 0;
    }

    return 1;
}

int load_students(const char *path, StudentList *list) {
    FILE *fp;
    char line[CSV_LINE_SIZE];
    int count = 0;

    fp = fopen(path, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: cannot open CSV file.\n");
        return -1;
    }

    if (fgets(line, sizeof(line), fp) == NULL) {
        fprintf(stderr, "Error: invalid CSV header.\n");
        fclose(fp);
        return -1;
    }

    trim_newline(line);
    if (strcmp(line, CSV_HEADER) != 0) {
        fprintf(stderr, "Error: invalid CSV header.\n");
        fclose(fp);
        return -1;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        int id;
        int score;
        char name[MAX_NAME_LEN + 1];

        trim_newline(line);
        if (line[0] == '\0') {
            continue;
        }

        if (!parse_student_line(line, &id, name, &score)) {
            fprintf(stderr, "Error: invalid CSV data.\n");
            clear_student_list(list);
            fclose(fp);
            return -1;
        }

        if (!add_student(list, id, name, score)) {
            fprintf(stderr, "Error: invalid CSV data.\n");
            clear_student_list(list);
            fclose(fp);
            return -1;
        }

        count++;
    }

    fclose(fp);
    return count;
}

int save_students(const char *path, const StudentList *list) {
    (void)path;
    (void)list;
    return 0;
}
