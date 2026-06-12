/*
 * main.c - Mini Student Shell
 *
 * Usage:
 *   ./admin_shell <students.csv> [-f commands.txt]
 *   ./client_shell <students.csv> [-f commands.txt]
 */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "student.h"
#include "file_io.h"
#include "command.h"

#define LINE_SIZE 256

static const char *program_name(void) {
#ifdef ADMIN_MODE
    return "Admin";
#else
    return "Client";
#endif
}

static const char *prompt_text(void) {
#ifdef ADMIN_MODE
    return "admin> ";
#else
    return "client> ";
#endif
}

static void print_usage(const char *exe_name) {
    fprintf(stderr, "Usage: %s <csv_file> [-f command_file]\n", exe_name);
}

static void trim_newline(char *line) {
    size_t len = strlen(line);

    while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
        line[len - 1] = '\0';
        len--;
    }
}

static char *trim_spaces(char *line) {
    char *start = line;
    char *end;

    while (isspace((unsigned char)*start)) {
        start++;
    }

    end = start + strlen(start);
    while (end > start && isspace((unsigned char)end[-1])) {
        end--;
    }
    *end = '\0';

    return start;
}

void run_shell(const char *csv_path) {
    StudentList list;
    char line[LINE_SIZE];
    int loaded;

    init_student_list(&list);
    loaded = load_students(csv_path, &list);
    if (loaded < 0) {
        clear_student_list(&list);
        return;
    }

    printf("[%s Program]\n", program_name());
    printf("Loaded %d students from %s.\n", loaded, csv_path);

    while (1) {
        CommandResult result;

        printf("%s", prompt_text());
        fflush(stdout);

        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }

        char *command_line;

        trim_newline(line);
        command_line = trim_spaces(line);
        result = execute_command(&list, csv_path, command_line);

        if (result == COMMAND_EXIT) {
            break;
        }
    }

    clear_student_list(&list);
}

void run_command_file(const char *cmd_file, const char *csv_path) {
    StudentList list;
    FILE *fp;
    char line[LINE_SIZE];
    int line_no = 0;
    int loaded;

    init_student_list(&list);
    loaded = load_students(csv_path, &list);
    if (loaded < 0) {
        clear_student_list(&list);
        return;
    }

    printf("[%s Program]\n", program_name());
    printf("Loaded %d students from %s.\n", loaded, csv_path);

    fp = fopen(cmd_file, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: cannot open command file.\n");
        clear_student_list(&list);
        return;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        CommandResult result;
        char *command_line;

        line_no++;
        trim_newline(line);
        command_line = trim_spaces(line);

        if (command_line[0] == '\0' || command_line[0] == '#') {
            continue;
        }

        printf("[command file:%d] %s\n", line_no, command_line);
        result = execute_command(&list, csv_path, command_line);

        if (result == COMMAND_EXIT) {
            break;
        }
        if (result == COMMAND_ERROR) {
            printf("Skipped line %d.\n", line_no);
        }
    }

    fclose(fp);
    clear_student_list(&list);
}

int main(int argc, char *argv[]) {
    const char *csv_path = NULL;
    const char *cmd_file = NULL;

    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Error: missing command file.\n");
                print_usage(argv[0]);
                return 1;
            }
            cmd_file = argv[++i];
        } else if (csv_path == NULL) {
            csv_path = argv[i];
        } else {
            fprintf(stderr, "Error: too many arguments.\n");
            print_usage(argv[0]);
            return 1;
        }
    }

    if (csv_path == NULL) {
        print_usage(argv[0]);
        return 1;
    }

    if (cmd_file != NULL) {
        run_command_file(cmd_file, csv_path);
    } else {
        run_shell(csv_path);
    }

    return 0;
}
