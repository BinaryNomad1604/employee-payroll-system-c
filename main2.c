#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Employee
{
    int id;
    char name[50];
    float basicSalary;
};

void addEmployee();
void displayEmployee();
void searchEmployee();
void deleteEmployee();
void updateEmployee();

int main()
{
    int choice;

    while (1)
    {
        printf("\n===== Employee Payroll System =====\n");
        printf("1. Add Employee\n");
        printf("2. Display Employees\n");
        printf("3. Search Employee\n");
        printf("4. Delete Employee\n");
        printf("5. Update Employee\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addEmployee();
            break;

        case 2:
            displayEmployee();
            break;

        case 3:
            searchEmployee();
            break;

        case 4:
            deleteEmployee();
            break;

        case 5:
            updateEmployee();
            break;

        case 6:
            printf("Exiting program...\n");
            exit(0);

        default:
            printf("Invalid choice!\n");
        }
    }

    return 0;
}

void addEmployee()
{
    struct Employee emp;
    FILE *fp;

    fp = fopen("employee.dat", "ab");
    if (fp == NULL)
    {
        printf("File error!\n");
        return;
    }

    printf("\nEnter Employee ID: ");
    scanf("%d", &emp.id);

    getchar(); // clear buffer
    printf("Enter Employee Name (no spaces): ");
    scanf("%49s", emp.name);

    printf("Enter Basic Salary: ");
    scanf("%f", &emp.basicSalary);

    fwrite(&emp, sizeof(emp), 1, fp);
    fclose(fp);

    printf("Employee Added Successfully!\n");
}

void displayEmployee()
{
    struct Employee emp;
    FILE *fp;

    fp = fopen("employee.dat", "rb");
    if (fp == NULL)
    {
        printf("No records found!\n");
        return;
    }

    printf("\nID\tName\tSalary\tHRA\tDA\tTax\tNet Salary\n");

    while (fread(&emp, sizeof(emp), 1, fp))
    {
        float hra = emp.basicSalary * 0.20;
        float da = emp.basicSalary * 0.10;
        float tax = (emp.basicSalary + hra + da) * 0.05;
        float netSalary = emp.basicSalary + hra + da - tax;

        printf("%d\t%s\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n",
               emp.id, emp.name, emp.basicSalary, hra, da, tax, netSalary);
    }

    fclose(fp);
}

void searchEmployee()
{
    struct Employee emp;
    FILE *fp;
    int searchId, found = 0;

    printf("Enter Employee ID to search: ");
    scanf("%d", &searchId);

    fp = fopen("employee.dat", "rb");
    if (fp == NULL)
    {
        printf("No records found!\n");
        return;
    }

    while (fread(&emp, sizeof(emp), 1, fp))
    {
        if (emp.id == searchId)
        {
            float hra = emp.basicSalary * 0.20;
            float da = emp.basicSalary * 0.10;
            float tax = (emp.basicSalary + hra + da) * 0.05;
            float netSalary = emp.basicSalary + hra + da - tax;

            printf("\nEmployee Found:\n");
            printf("ID: %d\nName: %s\nSalary: %.2f\nHRA: %.2f\nDA: %.2f\nTax: %.2f\nNet Salary: %.2f\n",
                   emp.id, emp.name, emp.basicSalary, hra, da, tax, netSalary);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Employee with ID %d not found.\n", searchId);

    fclose(fp);
}

void deleteEmployee()
{
    struct Employee emp;
    FILE *fp, *temp;
    int deleteId, found = 0;

    printf("Enter Employee ID to delete: ");
    scanf("%d", &deleteId);

    fp = fopen("employee.dat", "rb");
    if (fp == NULL)
    {
        printf("No records found!\n");
        return;
    }

    temp = fopen("temp.dat", "wb");
    if (temp == NULL)
    {
        printf("File error!\n");
        fclose(fp);
        return;
    }

    while (fread(&emp, sizeof(emp), 1, fp))
    {
        if (emp.id != deleteId)
        {
            fwrite(&emp, sizeof(emp), 1, temp);
        }
        else
        {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);

    remove("employee.dat");
    rename("temp.dat", "employee.dat");

    if (found)
        printf("Employee deleted successfully.\n");
    else
        printf("Employee with ID %d not found.\n", deleteId);
}

void updateEmployee()
{
    struct Employee emp;
    FILE *fp;
    int updateId, found = 0;

    printf("Enter Employee ID to update: ");
    scanf("%d", &updateId);

    fp = fopen("employee.dat", "rb+");
    if (fp == NULL)
    {
        printf("No records found!\n");
        return;
    }

    while (fread(&emp, sizeof(emp), 1, fp))
    {
        if (emp.id == updateId)
        {
            getchar(); // clear buffer
            printf("Enter new Name (no spaces): ");
            scanf("%49s", emp.name);

            printf("Enter new Basic Salary: ");
            scanf("%f", &emp.basicSalary);

            fseek(fp, -sizeof(emp), SEEK_CUR);
            fwrite(&emp, sizeof(emp), 1, fp);
            found = 1;
            printf("Employee updated successfully.\n");
            break;
        }
    }

    if (!found)
        printf("Employee with ID %d not found.\n", updateId);

    fclose(fp);
}
