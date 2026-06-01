#include "command.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int parse_id(const char *text, int *id) {
    char *end;
    long value;

    value = strtol(text, &end, 10);
    if (text[0] == '\0' || *end != '\0' || value <= 0) {
        return 0;
    }

    *id = (int)value;
    return 1;
}

static CommandResult handle_list(StudentList *list) {
    Student *cur = list->head;

    if (cur == NULL) {
        printf("No students found.\n");
        return COMMAND_CONTINUE;
    }

    printf("ID   Name       Score\n");
    while (cur != NULL) {
        printf("%-4d %-10s %d\n", cur->id, cur->name, cur->score);
        cur = cur->next;
    }

    return COMMAND_CONTINUE;
}

static CommandResult handle_find(StudentList *list, const char *args) {
    int id;
    Student *student;

    if (!parse_id(args, &id)) {
        printf("Error: invalid id.\n");
        return COMMAND_ERROR;
    }

    student = find_student(list, id);
    if (student == NULL) {
        printf("Error: student not found.\n");
        return COMMAND_ERROR;
    }

    printf("ID: %d\n", student->id);
    printf("Name: %s\n", student->name);
    printf("Score: %d\n", student->score);

    return COMMAND_CONTINUE;
}

CommandResult execute_command(StudentList *list, const char *csv_path, const char *line) {
    char command[32];
    char args[224];
    int count;

    (void)csv_path;

    if (line[0] == '\0') {
        return COMMAND_CONTINUE;
    }

    count = sscanf(line, "%31s %223[^\n]", command, args);
    if (count <= 0) {
        return COMMAND_CONTINUE;
    }

    if (strcmp(command, "exit") == 0) {
        printf("Goodbye.\n");
        return COMMAND_EXIT;
    }

    if (strcmp(command, "list") == 0) {
        return handle_list(list);
    }

    if (strcmp(command, "find") == 0) {
        if (count < 2) {
            printf("Error: missing id.\n");
            return COMMAND_ERROR;
        }
        return handle_find(list, args);
    }

    printf("Unknown command or permission denied.\n");
    return COMMAND_ERROR;
}
