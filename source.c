#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define arraySize 30
#define stringSize 30
#define semestrSize 8

const int MAXMARK = 10;
const int MINMARK = 1;

struct students_t {
    char name[stringSize];
    int marks[arraySize];
    int marksSize;
    int exams[semestrSize][arraySize];
    int examsSize[semestrSize];
    float average;
};

struct groups_t {
    struct students_t student[arraySize];
    int nextStudent;
};

void addStudent(struct groups_t *group, int grNum);
int inputGroup(int *lastGroup);
void inputName(struct students_t *student);
int inputMarks(int *marks);
void inputExams(struct students_t *student);
float searchAverage(struct students_t student);

int printAll(struct groups_t *group, int lastGroup);

float searchMinAverageExam(struct groups_t *group, int lastGroup);
float searchMaxAverageExam(struct groups_t *group, int lastGroup);

void checkHelp(char **argv);

int main(int argc, char **argv)
{
    if (argc > 1) {
        checkHelp(argv);
    }
    int i, grNum, lastGroup = 0;
    float average;
    char command[stringSize];
    struct groups_t group[arraySize];
    for (i = 0; i < arraySize; i++) {
        group[i].nextStudent = 0;
    }

    while (1) {
        printf("\nSelect the action:\n");
        printf("    \\e - Add student\n    \\p - Print all\n    \\x - Search for average min\n");
        printf("    \\n - Search for average max\n    \\q - Exit\n");
        fgets(command, stringSize, stdin);
        if (command[0] != '\\') {
            printf("Wrong command!\n");
            continue;
        }
        switch (command[1]) {
        case 'e':
            grNum = inputGroup(&lastGroup);
            addStudent(group, grNum);
            break;
        case 'p':
            if (!(printAll(group, lastGroup))) {
                printf("Nothing is entered.\n");
            }
            break;
        case 'x':
            average = searchMinAverageExam(group, lastGroup);
            if (!average) {
                printf("Neither exam is not entered.\n");
                break;
            }
            printf("Min is %0.3f\n", average);
            break;
        case 'n':
            average = searchMaxAverageExam(group, lastGroup);
            if (!average) {
                printf("Neither exam is not entered.\n");
                break;
            }
            printf("Max is %0.3f\n", average);
            break;
        case 'q':
            return 0;
        default:
            printf("Wrong command!\n");
        }
    }
    return 0;
}

void addStudent(struct groups_t *group, int grNum)
{
    int studNum;
    studNum = group[grNum].nextStudent;
    inputName(&group[grNum].student[studNum]);

    printf("Enter marks: ");
    group[grNum].student[studNum].marksSize = inputMarks(group[grNum].student[studNum].marks);
    inputExams(&(group[grNum].student[studNum]));
    group[grNum].student[studNum].average = searchAverage(group[grNum].student[studNum]);
    group[grNum].nextStudent++;
}

int inputGroup(int *lastGroup)
{
    int grNum;
    char groupStr[stringSize];
    do {
        printf("Enter group number: ");
        fgets(groupStr, stringSize, stdin);
        grNum = atoi(groupStr);
        if (grNum > arraySize || grNum < 1) {
            printf("Wrong number.\n");
            continue;
        }
        break;
    } while (1);
    if (grNum >= *lastGroup) {
        *lastGroup = grNum;
    }
    return --grNum;
}

void inputName(struct students_t *student)
{
    do {
        printf("Enter student's name: ");
        fgets(student->name, stringSize, stdin);
        if (student->name[0] == '\n') {
            continue;
        }
        return;
    } while (1);
}

int inputMarks(int *marks)
{
    int i, check = 1;
    char *tempPointer, *buffer;
    buffer = (char *) malloc(arraySize * sizeof(char));
    tempPointer = buffer;
    do {
        fgets(tempPointer, arraySize, stdin);
        for (i = 0;; i++) {
            while (tempPointer[0] == ' ') {
                tempPointer++;
            }
            if (!tempPointer[1]) {
                check = 0;
                break;
            }
            marks[i] = strtol(tempPointer, &tempPointer, 10);
            if ((marks[i] < MINMARK) || (marks[i] > MAXMARK)) {
                printf("Marks are not valid, try again: ");
                break;
            }
        }
    } while (check);
    free(buffer);
    return i;
}

void inputExams(struct students_t *student)
{
    int semNum;
    printf("Enter exams:\n");
    for (semNum = 0; semNum < semestrSize; semNum++) {
        printf("    %d semestr: ", semNum + 1);
        student->examsSize[semNum] = inputMarks(student->exams[semNum]);
    }
}

float searchAverage(struct students_t student)
{
    int i, j, count = 0, sum = 0;
    float result;
    for (i = 0; i < semestrSize; i++) {
        for (j = 0; j < student.examsSize[i]; j++) {
            sum += student.exams[i][j];
            count++;
        }
    }
    if (count == 0) {
        return 0;
    }
    result = (float) sum / (float) count;
    return result;
}

int printAll(struct groups_t *group, int lastGroup)
{
    int grNum, studNum, semNum, i, check = 0;
    for (grNum = 0; grNum < lastGroup; grNum++) {
        printf("\nGroup #%d", grNum + 1);
        for (studNum = 0; studNum < group[grNum].nextStudent; studNum++) {
            check = 1;
            printf("\n    %s", group[grNum].student[studNum].name);
            printf("        marks: ");
            for (i = 0; i < group[grNum].student[studNum].marksSize; i++) {
                printf("%d ", group[grNum].student[studNum].marks[i]);
            }
            printf("\n        exams:");
            for (semNum = 0; semNum < semestrSize; semNum++) {
                printf("\n            semestr #%d: ", semNum);
                for (i = 0; i < group[grNum].student[studNum].examsSize[semNum]; i++) {
                    printf("%d ", group[grNum].student[studNum].exams[semNum][i]);
                }
            }
        }
    }
    return check;
}

float searchMinAverageExam(struct groups_t *group, int lastGroup)
{
    int grNum, studNum;
    float tempMin = MAXMARK + 1;
    for (grNum = 0; grNum < lastGroup; grNum++) {
        for (studNum = 0; studNum < group[grNum].nextStudent; studNum++) {
            if (group[grNum].student[studNum].average == 0) {
                continue;
            }
            if (group[grNum].student[studNum].average < tempMin) {
                tempMin = group[grNum].student[studNum].average;
            }
        }
    }
    if (tempMin == MAXMARK + 1) {
        return 0;
    }
    return tempMin;
}

float searchMaxAverageExam(struct groups_t *group, int lastGroup)
{
    int grNum, studNum;
    float tempMax = MINMARK - 1;
    for (grNum = 0; grNum < lastGroup; grNum++) {
        for (studNum = 0; studNum < group[grNum].nextStudent; studNum++) {
            if (group[grNum].student[studNum].average == 0) {
                continue;
            }
            if (group[grNum].student[studNum].average > tempMax) {
                tempMax = group[grNum].student[studNum].average;
            }
        }
    }
    return tempMax;
}

void checkHelp(char **argv)
{
    if (!strcmp(argv[1], "-h")) {
        printf("================================================================================\n"
               "                                     MANUAL:\n\n"
               "Commands: \\e \\p \\x \\n \\q\n\n"
               "\\e - Structure input\n"
               "    1) Enter the group number. Number must be > 0 and < 30\n"
               "    2) Enter the name of the student. The name must not be an empty string.\n"
               "    3) Enter marks through spaces. Marks should be integers from 1 to 10. "
               "If marks are missing, enter an empty string.\n"
               "    4) Enter semester exams. Same as marks.\n\n"
               "\\p - Print whole structure.\n"
               "    Displays all entries.\n\n"
               "\\x - Searching for the maximum average of exam\n"
               "    Displays the maximum average of exams.\n\n"
               "\\n - Searching for the minimum average of exam\n"
               "    Displays the minimum average of exams.\n\n"
               "\\q - Exit\n"
               "    Close the program.\n"
               "================================================================================\n");
        exit(0);
    }
}
