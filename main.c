#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BRANCH_SIZE 50
#define NAME_SIZE 50

struct Student {
    int roll;
    char name[NAME_SIZE];
    char branch[BRANCH_SIZE];
    float marks;
};

int isRollNumberExist(FILE *file, int roll) {
    struct Student student;
    rewind(file);

    while (fread(&student, sizeof(struct Student), 1, file) == 1) {
        if (student.roll == roll) {
            return 1;
        }
    }

    return 0;
}

void addStudent(FILE *file) {
    struct Student student;
    printf("Enter Page Number: ");
    scanf("%d", &student.roll);

    if (isRollNumberExist(file, student.roll)) {
        printf("Page Number %d already exists. This page Number is already checked.\n", student.roll);
        return;
    }

    fwrite(&student, sizeof(struct Student), 1, file);
    printf("Page number added successfully!\n");
}


void displayStudent(struct Student student) {
    printf("  %-20d \n", student.roll);
}

void displayStudents(FILE *file) {
    struct Student student;
    rewind(file);

    printf("\n--- Page Numbers added ---\n");
    printf("|        Page         |\n");
    printf("|---------------------|\n");

    while (fread(&student, sizeof(struct Student), 1, file) == 1) {
        displayStudent(student);
    }
}

void deleteStudentByRoll(FILE *file, int roll) {
    struct Student student;
    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Error creating a temporary file.\n");
        return;
    }

    rewind(file);
    while (fread(&student, sizeof(struct Student), 1, file) == 1) {
        if (student.roll != roll) {
            fwrite(&student, sizeof(struct Student), 1, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove("student_data.txt");
    rename("temp.txt", "student_data.txt");

    file = fopen("student_data.txt", "r+");
    if (file == NULL) {
        printf("Error reopening the file.\n");
        return;
    }

    printf("Page Number %d deleted successfully!\n", roll);
}


void editStudentDataByRoll(FILE *file, int roll) {
    struct Student student;
    int found = 0;

    rewind(file);

    while (fread(&student, sizeof(struct Student), 1, file) == 1) {
        if (student.roll == roll) {
            found = 1;

            printf("Page Found!\n");
            printf("Edit Student Data:\n");
            
            printf("Enter new Page Number: ");
            int newRoll;
            scanf("%d", &newRoll);
            if (newRoll != 0) {
                student.roll = newRoll;
            }

            fseek(file, -(long)sizeof(struct Student), SEEK_CUR);
            fwrite(&student, sizeof(struct Student), 1, file);
            printf("Student data updated successfully!\n");
            break;
        }
    }

    if (!found) {
        printf("Student with Roll Number %d not found.\n", roll);
    }
}


int main() {
    FILE *studentFile;
    studentFile = fopen("student_data.txt", "a+");

    if (studentFile == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }

    int choice;
    while (1) {
        printf("\n--- Student Data Manager ---\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Delete Student by Roll Number\n");
        printf("4. Edit Student Data by Roll Number\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addStudent(studentFile);
                break;
            case 2:
                displayStudents(studentFile);
                break;
            case 3:
                {
                    int roll;
                    printf("Enter Page Number to delete: ");
                    scanf("%d", &roll);
                    deleteStudentByRoll(studentFile, roll);

                    // Reassign the file pointer after reopening the file
                    fclose(studentFile);
                    studentFile = fopen("student_data.txt", "r+");
                    if (studentFile == NULL) {
                        printf("Error reopening the file.\n");
                        return 1;
                    }
                }
                break;
            case 4:
                {
                    int roll;
                    printf("Enter Page Number to edit data: ");
                    scanf("%d", &roll);
                    editStudentDataByRoll(studentFile, roll);
                }
                break;
            case 5:
                fclose(studentFile);
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}