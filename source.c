#include <stdio.h>
#include <malloc.h>
#include <stdio_ext.h>

struct marks {
    int *estimate;
    int *exam;
};

struct info {
    char *name;
    struct marks *sem;
};

struct group {
    struct info *student;
    int averageExam;
    int averageEstimate;
    int isModify;
    int lastStudent;
} *list;

int groupMax = 0;

void enter();
void printAll();
int search(int minmax);

int main()
{
    char *action;
    int EXIT = 1;
    list = (struct group*)malloc(sizeof(struct group));
    list[0].lastStudent = 0;

    action = (char*)malloc(3 * sizeof(char));

    while (EXIT) {
        printf("Select the action:\n");
        printf("    \\e - Add student\n    \\p - Print all\n    \\x - Search for average min\n    \\n - Search for average max\n    \\q - Exit\n");
        fgets(action, 3, stdin);

        if (*action != '\\') {
            printf("Wrong command, try agan\n\n");
            continue;
        }

        switch (action[1]) {
            case 'e':
                enter();
                break;
            case 'p':
                printAll();
                break;
            case 'x':
                //printf("Group %d, average: %d", search(0), (list + search(0))->averageExam);
                break;
            case 'n':
                //printf("Group %d, average: %d", search(1), (list + search(1))->averageExam);
                break;
            case 'q':
                EXIT = 0;
                break;
            default:
                printf("Wrong command, try agan\n\n");
                break;
        }
        __fpurge(stdin);
    }

    return 0;
}

void enter()
{
    int groupNum, semNum, studentID, i, est;

    printf("Enter group:\n");
    scanf("%d",&groupNum);
    if (!groupNum) {
        printf("Error\n");
        return;
    }
    if (groupNum > groupMax) {
        list = (struct group*)realloc(list, sizeof(struct group) * groupNum);
        for (i = groupMax; i < groupNum; i++) {
            list[i].student = (struct info*)malloc(sizeof(struct info));
            list[i].lastStudent = 0;
        }
        groupMax = groupNum;
    }
    groupNum--;

    printf("Enter name:\n");
    studentID = list[groupNum].lastStudent++;
    list[groupNum].student = (struct info*)realloc(list[groupNum].student, sizeof(struct info) * (studentID + 1));
    list[groupNum].student[studentID].name = (char*)malloc(30 * sizeof(char));
    __fpurge(stdin);
    fgets(list[groupNum].student[studentID].name, 30, stdin);

    printf("Enter estimates, 0 to stop entry:\n");
    list[groupNum].student[studentID].sem = (struct marks*)malloc(8 * sizeof(struct marks));
    for (semNum = 0; semNum < 8; semNum++) {
        list[groupNum].student[studentID].sem[semNum].estimate = (int*)malloc(sizeof(int));
        printf("    %d semestr: ", semNum + 1);
        for (i = 1, est = 1; est; i++) {  // <7
            scanf("%d",&est);
            list[groupNum].student[studentID].sem[semNum].estimate[i - 1] = est;
            list[groupNum].student[studentID].sem[semNum].estimate = (int*)realloc(list[groupNum].student[studentID].sem[semNum].estimate, sizeof(int) * i);
        }
    }

    printf("Enter exams, 0 to stop entry:\n");
    for (semNum = 0; semNum < 8; semNum++) {
        (((list + groupNum)->student + studentID)->sem + semNum)->exam = (int*)malloc(sizeof(int));
        printf("    %d semestr: ", semNum + 1);
        for (i = 1, est = 1; est; i++) {  // <7
            scanf("%d", &est);
            list[groupNum].student[studentID].sem[semNum].exam[i - 1] = est;
            list[groupNum].student[studentID].sem[semNum].exam = (int*)realloc(list[groupNum].student[studentID].sem[semNum].exam, sizeof(int) * i);
        }
    }
}

void printAll()
{
    int currGroup, currID, currSem, i;
    for (currGroup = 0; currGroup < groupMax; currGroup++) {
        printf("Group #%d: \n", currGroup + 1);
        for (currID = 0; currID < (list + currGroup)->lastStudent; currID++) {
            printf("    %s",((list + currGroup)->student + currID)->name);
            for (currSem = 0; currSem < 8; currSem++) {
                printf("        %d semestr: ", currSem + 1);

                for (i = 0; list[currGroup].student[currID].sem[currSem].estimate[i]; i++) {
                    printf("%d ", list[currGroup].student[currID].sem[currSem].estimate[i]);
                }

                printf("    exams: ");
                for (i = 0; list[currGroup].student[currID].sem[currSem].exam[i]; i++) {
                    printf("%d ", list[currGroup].student[currID].sem[currSem].exam[i]);
                }
                printf("\n");
            }
        }
    }
    printf("\n");
}

int search(int minmax)
{
    return 1;
}
