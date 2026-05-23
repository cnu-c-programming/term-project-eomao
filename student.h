#ifndef STUDENT_H
#define STUDENT_H

#define MAX_NAME_LEN 31

typedef struct Student {
    int id;
    char name[MAX_NAME_LEN + 1];
    int score;
    struct Student *next;
} Student;

typedef struct {
    Student *head;
} StudentList;

void init_student_list(StudentList *list);
void clear_student_list(StudentList *list);
int count_students(const StudentList *list);

Student *find_student(StudentList *list, int id);
int add_student(StudentList *list, int id, const char *name, int score);
int delete_student(StudentList *list, int id);
int update_student(StudentList *list, int id, int score);

void sort_students_by_name(StudentList *list);
void sort_students_by_score(StudentList *list);

#endif
