#include "command.h"
#include "file_io.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int parse_positive_int(const char *text, int *number) {
    char *end;
    long value;

    value = strtol(text, &end, 10);
    if (text[0] == '\0' || *end != '\0' || value <= 0) {
        return 0;
    }

    *number = (int)value;
    return 1;
}

#ifdef ADMIN_MODE
static int parse_score(const char *text, int *score) {
    char *end;
    long value;

    value = strtol(text, &end, 10);
    if (text[0] == '\0' || *end != '\0' || value < 0 || value > 100) {
        return 0;
    }

    *score = (int)value;
    return 1;
}
#endif

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

    if (!parse_positive_int(args, &id)) {
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

static CommandResult handle_reload(StudentList *list, const char *csv_path) {
    int loaded;

    clear_student_list(list);
    loaded = load_students(csv_path, list);
    if (loaded < 0) {
        return COMMAND_ERROR;
    }

    printf("Reloaded %d students from %s.\n", loaded, csv_path);
    return COMMAND_CONTINUE;
}

#ifdef ADMIN_MODE
static CommandResult handle_save(StudentList *list, const char *csv_path) {
    int saved;

    saved = save_students(csv_path, list);
    if (saved < 0) {
        return COMMAND_ERROR;
    }

    printf("Saved %d students to %s.\n", saved, csv_path);
    return COMMAND_CONTINUE;
}

static CommandResult handle_add(StudentList *list, const char *args) {
    char id_text[32];
    char name[MAX_NAME_LEN + 1];
    char score_text[32];
    char extra[32];
    int id;
    int score;
    int count;

    count = sscanf(args, "%31s %31s %31s %31s", id_text, name, score_text, extra);
    if (count < 3) {
        printf("Error: missing argument.\n");
        return COMMAND_ERROR;
    }
    if (count > 3) {
        printf("Error: invalid argument.\n");
        return COMMAND_ERROR;
    }

    if (!parse_positive_int(id_text, &id)) {
        printf("Error: invalid id.\n");
        return COMMAND_ERROR;
    }
    if (!parse_score(score_text, &score)) {
        printf("Error: invalid score.\n");
        return COMMAND_ERROR;
    }
    if (strchr(name, ',') != NULL) {
        printf("Error: invalid name.\n");
        return COMMAND_ERROR;
    }
    if (find_student(list, id) != NULL) {
        printf("Error: duplicate ID.\n");
        return COMMAND_ERROR;
    }

    if (!add_student(list, id, name, score)) {
        printf("Error: cannot add student.\n");
        return COMMAND_ERROR;
    }

    printf("Student added.\n");
    return COMMAND_CONTINUE;
}

static CommandResult handle_delete(StudentList *list, const char *args) {
    int id;

    if (!parse_positive_int(args, &id)) {
        printf("Error: invalid id.\n");
        return COMMAND_ERROR;
    }

    if (!delete_student(list, id)) {
        printf("Error: student not found.\n");
        return COMMAND_ERROR;
    }

    printf("Student deleted.\n");
    return COMMAND_CONTINUE;
}

static CommandResult handle_update(StudentList *list, const char *args) {
    char id_text[32];
    char score_text[32];
    char extra[32];
    int id;
    int score;
    int count;

    count = sscanf(args, "%31s %31s %31s", id_text, score_text, extra);
    if (count < 2) {
        printf("Error: missing argument.\n");
        return COMMAND_ERROR;
    }
    if (count > 2) {
        printf("Error: invalid argument.\n");
        return COMMAND_ERROR;
    }

    if (!parse_positive_int(id_text, &id)) {
        printf("Error: invalid id.\n");
        return COMMAND_ERROR;
    }
    if (!parse_score(score_text, &score)) {
        printf("Error: invalid score.\n");
        return COMMAND_ERROR;
    }
    if (find_student(list, id) == NULL) {
        printf("Error: student not found.\n");
        return COMMAND_ERROR;
    }

    if (!update_student(list, id, score)) {
        printf("Error: cannot update student.\n");
        return COMMAND_ERROR;
    }

    printf("Student updated.\n");
    return COMMAND_CONTINUE;
}
#endif

CommandResult execute_command(StudentList *list, const char *csv_path, const char *line) {
    char command[32];
    char args[224];
    int count;

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

    if (strcmp(command, "reload") == 0) {
        return handle_reload(list, csv_path);
    }

#ifdef ADMIN_MODE
    if (strcmp(command, "save") == 0) {
        return handle_save(list, csv_path);
    }

    if (strcmp(command, "add") == 0) {
        if (count < 2) {
            printf("Error: missing argument.\n");
            return COMMAND_ERROR;
        }
        return handle_add(list, args);
    }

    if (strcmp(command, "delete") == 0) {
        if (count < 2) {
            printf("Error: missing id.\n");
            return COMMAND_ERROR;
        }
        return handle_delete(list, args);
    }

    if (strcmp(command, "update") == 0) {
        if (count < 2) {
            printf("Error: missing argument.\n");
            return COMMAND_ERROR;
        }
        return handle_update(list, args);
    }
#endif

    printf("Unknown command or permission denied.\n");
    return COMMAND_ERROR;
}
