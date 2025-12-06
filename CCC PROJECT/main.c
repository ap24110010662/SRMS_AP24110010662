#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define STUDENT_FILE "students.txt"
#define CREDENTIAL_FILE "credentials.txt"
#define LOG_FILE "login_logs.txt"

struct Student {
    char roll[20];    // changed from int to char[]
    char name[50];
    float marks;
};

char currentRole[20];
char currentUser[50];

/* Function declarations */
int loginSystem();
void logEvent(char *user, char *status);
void getMaskedPassword(char *password);

void mainMenu();
void adminMenu();
void userMenu();
void staffMenu();
void guestMenu();

void addStudent();
void displayStudents();
void searchStudent();
void searchByName();
void updateStudent();
void deleteStudent();

void resetPassword(); // ADMIN ONLY


/* ================= MAIN (LOGIN LOOP) =================== */
int main() {
    while (1) {
        printf("\n==============================\n");
        printf("       STUDENT SYSTEM\n");
        printf("==============================\n");

        if (loginSystem()) {
            mainMenu(); // After logout → go to login screen again
        }

        // Ask if user wants to exit completely
        char ch;
        printf("\nDo you want to exit the program? (y/n): ");
        scanf(" %c", &ch);

        if (ch == 'y' || ch == 'Y') {
            printf("Exiting program...\n");
            break;
        }
    }

    return 0;
}


/* ================= LOGIN =================== */
int loginSystem() {
    char username[50], password[50];
    char fileUser[50], filePass[50], fileRole[20];

    printf("\n===== LOGIN =====\n");
    printf("Username: ");
    scanf("%s", username);

    printf("Password: ");
    getMaskedPassword(password);

    FILE *fp = fopen(CREDENTIAL_FILE, "r");
    if (!fp) {
        printf("Error: credentials.txt not found!\n");
        return 0;
    }

    while (fscanf(fp, "%s %s %s", fileUser, filePass, fileRole) == 3) {
        if (strcmp(username, fileUser) == 0 && strcmp(password, filePass) == 0) {
            strcpy(currentRole, fileRole);
            strcpy(currentUser, fileUser);

            logEvent(username, "SUCCESS");

            fclose(fp);
            return 1;
        }
    }

    logEvent(username, "FAILED");
    fclose(fp);
    printf("Invalid username or password!\n");
    return 0;
}


/* ========== PASSWORD MASK () ============= */
void getMaskedPassword(char *password) {
    int i = 0;
    char ch;

    while (1) {
        ch = getch();
        if (ch == 13) {  // Enter key
            password[i] = '\0';
            printf("\n");
            break;
        }
        else if (ch == 8 && i > 0) {  // Backspace
            i--;
            printf("\b \b");
        }
        else {
            password[i++] = ch;
            printf("*");
        }
    }
}


/* ========== LOGIN LOGGING ============= */
void logEvent(char *user, char *status) {
    FILE *log = fopen(LOG_FILE, "a");
    if (!log) return;

    fprintf(log, "User: %s | Status: %s\n", user, status);
    fclose(log);
}


/* ================= MAIN MENU =================== */
void mainMenu() {
    if (strcmp(currentRole, "ADMIN") == 0)
        adminMenu();
    else if (strcmp(currentRole, "USER") == 0)
        userMenu();
    else if (strcmp(currentRole, "STAFF") == 0)
        staffMenu();
    else
        guestMenu();
}


/* ================= ADMIN MENU =================== */
void adminMenu() {
    int choice;

    do {
        printf("\n===== ADMIN MENU =====\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Search Student (Roll)\n");
        printf("4. Search by Name\n");
        printf("5. Update Student\n");
        printf("6. Delete Student\n");
        printf("7. Reset Password (Admin Only)\n");
        printf("8. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: searchByName(); break;
            case 5: updateStudent(); break;
            case 6: deleteStudent(); break;
            case 7: resetPassword(); break;
            case 8: return;
            default: printf("Invalid choice!\n");
        }
    } while (1);
}


/* USER → Display + Search */
void userMenu() {
    int choice;

    do {
        printf("\n===== USER MENU =====\n");
        printf("1. Display Students\n");
        printf("2. Search by Roll\n");
        printf("3. Search by Name\n");
        printf("4. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: displayStudents(); break;
            case 2: searchStudent(); break;
            case 3: searchByName(); break;
            case 4: return;
            default: printf("Invalid!\n");
        }
    } while (1);
}


/* STAFF → Add + Display + Search */
void staffMenu() {
    int choice;

    do {
        printf("\n===== STAFF MENU =====\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Search by Roll\n");
        printf("4. Search by Name\n");
        printf("5. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: searchByName(); break;
            case 5: return;
            default: printf("Invalid!\n");
        }
    } while (1);
}


/* Guest → display + search only */
void guestMenu() {
    int choice;

    do {
        printf("\n===== GUEST MENU =====\n");
        printf("1. Display Students\n");
        printf("2. Search by Roll\n");
        printf("3. Search by Name\n");
        printf("4. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: displayStudents(); break;
            case 2: searchStudent(); break;
            case 3: searchByName(); break;
            case 4: return;
            default: printf("Invalid!\n");
        }
    } while (1);
}


/* ================= CRUD FUNCTIONS =================== */

void addStudent() {
    struct Student s;
    FILE *fp = fopen(STUDENT_FILE, "a");

    if (!fp) {
        printf("Error opening student file!\n");
        return;
    }

    printf("Enter Roll: ");
    scanf(" %s", s.roll);           // changed to string
    printf("Enter Name: ");
    scanf(" %s", s.name);
    printf("Enter Marks: ");
    scanf(" %f", &s.marks);

    fprintf(fp, "%s %s %.2f\n", s.roll, s.name, s.marks); // string roll
    fclose(fp);

    printf("Student Added Successfully!\n");
}


void displayStudents() {
    struct Student s;
    FILE *fp = fopen(STUDENT_FILE, "r");

    if (!fp) {
        printf("No student file found!\n");
        return;
    }

    printf("\n--- Student List ---\n");
    while (fscanf(fp, "%s %s %f", s.roll, s.name, &s.marks) == 3) {
        printf("Roll: %s | Name: %s | Marks: %.2f\n", s.roll, s.name, s.marks);
    }

    fclose(fp);
}


void searchStudent() {
    char r[20];
    int found = 0;
    struct Student s;

    printf("Enter Roll to Search: ");
    scanf("%s", r);

    FILE *fp = fopen(STUDENT_FILE, "r");

    if (!fp) {
        printf("No student file found!\n");
        return;
    }

    while (fscanf(fp, "%s %s %f", s.roll, s.name, &s.marks) == 3) {
        if (strcmp(s.roll, r) == 0) {        // string compare
            printf("FOUND: %s %s %.2f\n", s.roll, s.name, s.marks);
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (!found)
        printf("Student Not Found!\n");
}


/* ========== SEARCH BY NAME ============= */
void searchByName() {
    char searchName[50];
    struct Student s;
    int found = 0;

    printf("Enter name to search: ");
    scanf("%s", searchName);

    FILE *fp = fopen(STUDENT_FILE, "r");

    if (!fp) {
        printf("No student file found!\n");
        return;
    }

    while (fscanf(fp, "%s %s %f", s.roll, s.name, &s.marks) == 3) {
        if (strcmp(searchName, s.name) == 0) {
            printf("FOUND: %s %s %.2f\n", s.roll, s.name, s.marks);
            found = 1;
        }
    }

    if (!found)
        printf("No student with that name.\n");

    fclose(fp);
}


void updateStudent() {
    char r[20];
    int found = 0;
    struct Student s;
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!fp || !temp) {
        printf("File error!\n");
        return;
    }

    printf("Enter Roll to Update: ");
    scanf("%s", r);

    while (fscanf(fp, "%s %s %f", s.roll, s.name, &s.marks) == 3) {
        if (strcmp(s.roll, r) == 0) {
            found = 1;
            printf("New Name: ");
            scanf("%s", s.name);
            printf("New Marks: ");
            scanf("%f", &s.marks);
        }
        fprintf(temp, "%s %s %.2f\n", s.roll, s.name, s.marks);
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found)
        printf("Student Updated!\n");
    else
        printf("Student Not Found!\n");
}


void deleteStudent() {
    char r[20];
    int found = 0;
    struct Student s;

    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!fp || !temp) {
        printf("File error!\n");
        return;
    }

    printf("Enter Roll to Delete: ");
    scanf("%s", r);

    while (fscanf(fp, "%s %s %f", s.roll, s.name, &s.marks) == 3) {
        if (strcmp(s.roll, r) != 0)
            fprintf(temp, "%s %s %.2f\n", s.roll, s.name, s.marks);
        else
            found = 1;
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found)
        printf("Student Deleted!\n");
    else
        printf("Not Found!\n");
}


/* ========== RESET PASSWORD (ADMIN ONLY) ============= */
void resetPassword() {
    if (strcmp(currentRole, "ADMIN") != 0) {
        printf("Only ADMIN can reset passwords!\n");
        return;
    }

    char user[50], newPass[50], confirmPass[50];
    char fileUser[50], filePass[50], fileRole[20];

    printf("Enter username to reset: ");
    scanf("%s", user);

    FILE *fp = fopen(CREDENTIAL_FILE, "r");
    FILE *temp = fopen("temp_cred.txt", "w");

    if (!fp || !temp) {
        printf("Error opening credentials file!\n");
        return;
    }

    int found = 0;

    while (fscanf(fp, "%s %s %s", fileUser, filePass, fileRole) == 3) {
        if (strcmp(fileUser, user) == 0) {
            found = 1;

            printf("Enter New Password: ");
            scanf("%s", newPass);
            printf("Confirm Password: ");
            scanf("%s", confirmPass);

            if (strcmp(newPass, confirmPass) != 0) {
                printf("Password Mismatch!\n");
                fclose(fp);
                fclose(temp);
                remove("temp_cred.txt");
                return;
            }

            fprintf(temp, "%s %s %s\n", fileUser, newPass, fileRole);
        }
        else {
            fprintf(temp, "%s %s %s\n", fileUser, filePass, fileRole);
        }
    }

    fclose(fp);
    fclose(temp);

    remove(CREDENTIAL_FILE);
    rename("temp_cred.txt", CREDENTIAL_FILE);

    if (found)
        printf("Password Reset Successful!\n");
    else
        printf("User Not Found!\n");
}