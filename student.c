#include "student.h"

#include <stdlib.h>
#include <string.h>

void init_student_list(StudentList *list) {
    list->head = NULL;
}

void clear_student_list(StudentList *list) {
    Student *cur = list->head;

    while (cur != NULL) {
        Student *next = cur->next;
        free(cur);
        cur = next;
    }

    list->head = NULL;
}

int count_students(const StudentList *list) {
    int count = 0;
    const Student *cur = list->head;

    while (cur != NULL) {
        count++;
        cur = cur->next;
    }

    return count;
}

Student *find_student(StudentList *list, int id) {
    Student *cur = list->head;

    while (cur != NULL) {
        if (cur->id == id) {
            return cur;
        }
        cur = cur->next;
    }

    return NULL;
}

int add_student(StudentList *list, int id, const char *name, int score) {
    Student *new_student;
    Student *cur;

    if (id <= 0 || score < 0 || score > 100 || name[0] == '\0') {
        return 0;
    }

    if (find_student(list, id) != NULL) {
        return 0;
    }

    new_student = malloc(sizeof(Student));
    if (new_student == NULL) {
        return 0;
    }

    new_student->id = id;
    strncpy(new_student->name, name, MAX_NAME_LEN);
    new_student->name[MAX_NAME_LEN] = '\0';
    new_student->score = score;
    new_student->next = NULL;

    if (list->head == NULL) {
        list->head = new_student;
        return 1;
    }

    cur = list->head;
    while (cur->next != NULL) {
        cur = cur->next;
    }
    cur->next = new_student;

    return 1;
}

int delete_student(StudentList *list, int id) {
    (void)list;
    (void)id;
    return 0;
}

int update_student(StudentList *list, int id, int score) {
    (void)list;
    (void)id;
    (void)score;
    return 0;
}

void sort_students_by_name(StudentList *list) {
    (void)list;
}

void sort_students_by_score(StudentList *list) {
    (void)list;
}
