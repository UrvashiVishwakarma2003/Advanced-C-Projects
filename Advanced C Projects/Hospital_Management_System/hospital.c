/*
================================================================================
DESCRIPTION:
Complete hospital management system for managing patients, doctors,
appointments, and billing.

FEATURES:
- Add new patients
- Add new doctors
- Book appointments
- View all patients
- View all doctors
- View appointments
- Generate bill
- Discharge patient
- Search patient by ID or name

================================================================================
*/

#include <stdio.h>      // For printf, scanf - input/output
#include <stdlib.h>     // For system, exit - system functions
#include <string.h>     // For strcpy, strcmp - string operations
#include <conio.h>      // For getch - keyboard input
#include <time.h>       // For time - date functions

/*
================================================================================
STRUCTURES
================================================================================
*/

// Structure for date
struct Date
{
    int day;
    int month;
    int year;
};

// Structure for patient
struct Patient
{
    int id;                 // Patient ID
    char name[50];          // Patient name
    int age;                // Patient age
    char gender;            // M or F
    char address[100];      // Address
    char phone[12];         // Phone number
    char disease[100];      // Disease/Diagnosis
    struct Date admitDate;  // Admission date
    struct Date dischargeDate; // Discharge date
    int isAdmitted;         // 1=Admitted, 0=Discharged
    int billAmount;         // Total bill amount
};

// Structure for doctor
struct Doctor
{
    int id;                 // Doctor ID
    char name[50];          // Doctor name
    char specialization[50]; // Department/Specialization
    char phone[12];         // Phone number
    int fee;                // Consultation fee
    int available;          // 1=Available, 0=Not available
};

// Structure for appointment
struct Appointment
{
    int id;                 // Appointment ID
    int patientId;          // Patient ID
    int doctorId;           // Doctor ID
    struct Date date;       // Appointment date
    char time[10];          // Appointment time
    int status;             // 1=Scheduled, 2=Completed, 3=Cancelled
};

/*
================================================================================
GLOBAL VARIABLES
================================================================================
*/

struct Patient patients[200];       // Array to store patients
int patientCount = 0;               // Total patients
struct Doctor doctors[50];          // Array to store doctors
int doctorCount = 0;                // Total doctors
struct Appointment appointments[500]; // Array to store appointments
int appointmentCount = 0;           // Total appointments
int nextPatientId = 1001;           // Next patient ID
int nextDoctorId = 2001;            // Next doctor ID
int nextAppointmentId = 3001;       // Next appointment ID

/*
================================================================================
FUNCTION: getCurrentDate()
PURPOSE: Get current date
================================================================================
*/

void getCurrentDate(struct Date *date)
{
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    date->day = tm->tm_mday;
    date->month = tm->tm_mon + 1;
    date->year = tm->tm_year + 1900;
}

/*
================================================================================
FUNCTION: saveDataToFile()
PURPOSE: Save all data to files
================================================================================
*/

void saveDataToFile()
{
    // Save patients
    FILE *file = fopen("patients.dat", "wb");
    if(file != NULL)
    {
        fwrite(&patientCount, sizeof(int), 1, file);
        fwrite(patients, sizeof(struct Patient), patientCount, file);
        fwrite(&nextPatientId, sizeof(int), 1, file);
        fclose(file);
    }
    
    // Save doctors
    file = fopen("doctors.dat", "wb");
    if(file != NULL)
    {
        fwrite(&doctorCount, sizeof(int), 1, file);
        fwrite(doctors, sizeof(struct Doctor), doctorCount, file);
        fwrite(&nextDoctorId, sizeof(int), 1, file);
        fclose(file);
    }
    
    // Save appointments
    file = fopen("appointments.dat", "wb");
    if(file != NULL)
    {
        fwrite(&appointmentCount, sizeof(int), 1, file);
        fwrite(appointments, sizeof(struct Appointment), appointmentCount, file);
        fwrite(&nextAppointmentId, sizeof(int), 1, file);
        fclose(file);
    }
}

/*
================================================================================
FUNCTION: loadDataFromFile()
PURPOSE: Load all data from files
================================================================================
*/

void loadDataFromFile()
{
    // Load patients
    FILE *file = fopen("patients.dat", "rb");
    if(file != NULL)
    {
        fread(&patientCount, sizeof(int), 1, file);
        fread(patients, sizeof(struct Patient), patientCount, file);
        fread(&nextPatientId, sizeof(int), 1, file);
        fclose(file);
    }
    
    // Load doctors
    file = fopen("doctors.dat", "rb");
    if(file != NULL)
    {
        fread(&doctorCount, sizeof(int), 1, file);
        fread(doctors, sizeof(struct Doctor), doctorCount, file);
        fread(&nextDoctorId, sizeof(int), 1, file);
        fclose(file);
    }
    
    // Load appointments
    file = fopen("appointments.dat", "rb");
    if(file != NULL)
    {
        fread(&appointmentCount, sizeof(int), 1, file);
        fread(appointments, sizeof(struct Appointment), appointmentCount, file);
        fread(&nextAppointmentId, sizeof(int), 1, file);
        fclose(file);
    }
}

/*
================================================================================
FUNCTION: addPatient()
PURPOSE: Add new patient
================================================================================
*/

void addPatient()
{
    system("cls");
    printf("\n========================================\n");
    printf("           ADD NEW PATIENT\n");
    printf("========================================\n");
    
    struct Patient newPatient;
    
    newPatient.id = nextPatientId++;
    
    printf("\nPatient ID: %d\n", newPatient.id);
    printf("Enter Patient Name: ");
    fflush(stdin);
    fgets(newPatient.name, 50, stdin);
    newPatient.name[strcspn(newPatient.name, "\n")] = 0;
    
    printf("Enter Age: ");
    scanf("%d", &newPatient.age);
    
    printf("Enter Gender (M/F): ");
    scanf(" %c", &newPatient.gender);
    
    printf("Enter Address: ");
    fflush(stdin);
    fgets(newPatient.address, 100, stdin);
    newPatient.address[strcspn(newPatient.address, "\n")] = 0;
    
    printf("Enter Phone Number: ");
    scanf("%s", newPatient.phone);
    
    printf("Enter Disease/Diagnosis: ");
    fflush(stdin);
    fgets(newPatient.disease, 100, stdin);
    newPatient.disease[strcspn(newPatient.disease, "\n")] = 0;
    
    getCurrentDate(&newPatient.admitDate);
    newPatient.isAdmitted = 1;
    newPatient.billAmount = 0;
    
    patients[patientCount] = newPatient;
    patientCount++;
    saveDataToFile();
    
    printf("\n========================================\n");
    printf("     PATIENT ADMITTED SUCCESSFULLY!\n");
    printf("========================================\n");
    printf("Patient ID: %d\n", newPatient.id);
    printf("Admit Date: %02d/%02d/%d\n", newPatient.admitDate.day, 
           newPatient.admitDate.month, newPatient.admitDate.year);
    
    printf("\nPress any key...");
    getch();
}

/*
================================================================================
FUNCTION: viewAllPatients()
PURPOSE: Display all patients
================================================================================
*/

void viewAllPatients()
{
    system("cls");
    printf("\n========================================\n");
    printf("           ALL PATIENTS\n");
    printf("========================================\n");
    
    if(patientCount == 0)
    {
        printf("\nNo patients found!\n");
    }
    else
    {
        printf("\n================================================================================\n");
        printf("ID     | Name                | Age | Gender | Phone      | Status\n");
        printf("================================================================================\n");
        
        for(int i = 0; i < patientCount; i++)
        {
            printf("%-6d | %-19s | %-3d | %-6c | %-10s | %s\n", 
                   patients[i].id,
                   patients[i].name,
                   patients[i].age,
                   patients[i].gender,
                   patients[i].phone,
                   patients[i].isAdmitted ? "ADMITTED" : "DISCHARGED");
        }
        printf("================================================================================\n");
        printf("Total Patients: %d\n", patientCount);
    }
    
    printf("\nPress any key...");
    getch();
}

/*
================================================================================
FUNCTION: searchPatient()
PURPOSE: Search patient by ID or name
================================================================================
*/

void searchPatient()
{
    system("cls");
    printf("\n========================================\n");
    printf("           SEARCH PATIENT\n");
    printf("========================================\n");
    
    int choice;
    printf("\nSearch by:\n");
    printf("1. Patient ID\n");
    printf("2. Patient Name\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    
    if(choice == 1)
    {
        int id;
        printf("\nEnter Patient ID: ");
        scanf("%d", &id);
        
        int found = 0;
        for(int i = 0; i < patientCount; i++)
        {
            if(patients[i].id == id)
            {
                printf("\n========================================\n");
                printf("           PATIENT DETAILS\n");
                printf("========================================\n");
                printf("Patient ID: %d\n", patients[i].id);
                printf("Name: %s\n", patients[i].name);
                printf("Age: %d\n", patients[i].age);
                printf("Gender: %c\n", patients[i].gender);
                printf("Address: %s\n", patients[i].address);
                printf("Phone: %s\n", patients[i].phone);
                printf("Disease: %s\n", patients[i].disease);
                printf("Admit Date: %02d/%02d/%d\n", patients[i].admitDate.day,
                       patients[i].admitDate.month, patients[i].admitDate.year);
                printf("Status: %s\n", patients[i].isAdmitted ? "ADMITTED" : "DISCHARGED");
                printf("Bill Amount: Rs.%d\n", patients[i].billAmount);
                printf("========================================\n");
                found = 1;
                break;
            }
        }
        
        if(!found)
        {
            printf("\nPatient not found!\n");
        }
    }
    else if(choice == 2)
    {
        char name[50];
        printf("\nEnter Patient Name: ");
        fflush(stdin);
        fgets(name, 50, stdin);
        name[strcspn(name, "\n")] = 0;
        
        int found = 0;
        printf("\n================================================================================\n");
        printf("ID     | Name                | Age | Phone      | Status\n");
        printf("================================================================================\n");
        
        for(int i = 0; i < patientCount; i++)
        {
            if(strstr(patients[i].name, name) != NULL)
            {
                printf("%-6d | %-19s | %-3d | %-10s | %s\n", 
                       patients[i].id,
                       patients[i].name,
                       patients[i].age,
                       patients[i].phone,
                       patients[i].isAdmitted ? "ADMITTED" : "DISCHARGED");
                found = 1;
            }
        }
        
        if(!found)
        {
            printf("\nNo patient found with name: %s\n", name);
        }
        printf("================================================================================\n");
    }
    
    printf("\nPress any key...");
    getch();
}

/*
================================================================================
FUNCTION: addDoctor()
PURPOSE: Add new doctor
================================================================================
*/

void addDoctor()
{
    system("cls");
    printf("\n========================================\n");
    printf("           ADD NEW DOCTOR\n");
    printf("========================================\n");
    
    struct Doctor newDoctor;
    
    newDoctor.id = nextDoctorId++;
    
    printf("\nDoctor ID: %d\n", newDoctor.id);
    printf("Enter Doctor Name: ");
    fflush(stdin);
    fgets(newDoctor.name, 50, stdin);
    newDoctor.name[strcspn(newDoctor.name, "\n")] = 0;
    
    printf("Enter Specialization: ");
    fgets(newDoctor.specialization, 50, stdin);
    newDoctor.specialization[strcspn(newDoctor.specialization, "\n")] = 0;
    
    printf("Enter Phone Number: ");
    scanf("%s", newDoctor.phone);
    
    printf("Enter Consultation Fee: ₹");
    scanf("%d", &newDoctor.fee);
    
    newDoctor.available = 1;
    
    doctors[doctorCount] = newDoctor;
    doctorCount++;
    saveDataToFile();
    
    printf("\n========================================\n");
    printf("     DOCTOR ADDED SUCCESSFULLY!\n");
    printf("========================================\n");
    printf("Doctor ID: %d\n", newDoctor.id);
    
    printf("\nPress any key...");
    getch();
}

/*
================================================================================
FUNCTION: viewAllDoctors()
PURPOSE: Display all doctors
================================================================================
*/

void viewAllDoctors()
{
    system("cls");
    printf("\n========================================\n");
    printf("           ALL DOCTORS\n");
    printf("========================================\n");
    
    if(doctorCount == 0)
    {
        printf("\nNo doctors found!\n");
    }
    else
    {
        printf("\n================================================================================\n");
        printf("ID     | Name                | Specialization      | Fee     | Status\n");
        printf("================================================================================\n");
        
        for(int i = 0; i < doctorCount; i++)
        {
            printf("%-6d | %-19s | %-20s | Rs.%-6d | %s\n", 
                   doctors[i].id,
                   doctors[i].name,
                   doctors[i].specialization,
                   doctors[i].fee,
                   doctors[i].available ? "AVAILABLE" : "NOT AVAILABLE");
        }
        printf("================================================================================\n");
        printf("Total Doctors: %d\n", doctorCount);
    }
    
    printf("\nPress any key...");
    getch();
}

/*
================================================================================
FUNCTION: bookAppointment()
PURPOSE: Book appointment with doctor
================================================================================
*/

void bookAppointment()
{
    system("cls");
    printf("\n========================================\n");
    printf("           BOOK APPOINTMENT\n");
    printf("========================================\n");
    
    int patientId, doctorId;
    
    printf("\nEnter Patient ID: ");
    scanf("%d", &patientId);
    
    int patientIndex = -1;
    for(int i = 0; i < patientCount; i++)
    {
        if(patients[i].id == patientId)
        {
            patientIndex = i;
            break;
        }
    }
    
    if(patientIndex == -1)
    {
        printf("\nPatient not found!\n");
        printf("\nPress any key...");
        getch();
        return;
    }
    
    printf("\nAvailable Doctors:\n");
    printf("================================================================================\n");
    printf("ID     | Name                | Specialization      | Fee\n");
    printf("================================================================================\n");
    
    for(int i = 0; i < doctorCount; i++)
    {
        if(doctors[i].available)
        {
            printf("%-6d | %-19s | %-20s | Rs.%d\n", 
                   doctors[i].id,
                   doctors[i].name,
                   doctors[i].specialization,
                   doctors[i].fee);
        }
    }
    printf("================================================================================\n");
    
    printf("\nEnter Doctor ID: ");
    scanf("%d", &doctorId);
    
    int doctorIndex = -1;
    for(int i = 0; i < doctorCount; i++)
    {
        if(doctors[i].id == doctorId && doctors[i].available)
        {
            doctorIndex = i;
            break;
        }
    }
    
    if(doctorIndex == -1)
    {
        printf("\nDoctor not found or not available!\n");
        printf("\nPress any key...");
        getch();
        return;
    }
    
    struct Appointment newAppointment;
    
    newAppointment.id = nextAppointmentId++;
    newAppointment.patientId = patientId;
    newAppointment.doctorId = doctorId;
    
    printf("\nEnter Appointment Date (DD MM YYYY): ");
    scanf("%d %d %d", &newAppointment.date.day, &newAppointment.date.month, &newAppointment.date.year);
    
    printf("Enter Time (HH:MM): ");
    scanf("%s", newAppointment.time);
    
    newAppointment.status = 1;
    
    appointments[appointmentCount] = newAppointment;
    appointmentCount++;
    saveDataToFile();
    
    printf("\n========================================\n");
    printf("     APPOINTMENT BOOKED SUCCESSFULLY!\n");
    printf("========================================\n");
    printf("Appointment ID: %d\n", newAppointment.id);
    printf("Doctor: %s\n", doctors[doctorIndex].name);
    printf("Patient: %s\n", patients[patientIndex].name);
    printf("Date: %02d/%02d/%d\n", newAppointment.date.day, newAppointment.date.month, newAppointment.date.year);
    printf("Time: %s\n", newAppointment.time);
    
    printf("\nPress any key...");
    getch();
}

/*
================================================================================
FUNCTION: viewAppointments()
PURPOSE: View all appointments
================================================================================
*/

void viewAppointments()
{
    system("cls");
    printf("\n========================================\n");
    printf("           ALL APPOINTMENTS\n");
    printf("========================================\n");
    
    if(appointmentCount == 0)
    {
        printf("\nNo appointments found!\n");
    }
    else
    {
        printf("\n================================================================================\n");
        printf("App ID | Patient Name        | Doctor Name         | Date       | Time  | Status\n");
        printf("================================================================================\n");
        
        for(int i = 0; i < appointmentCount; i++)
        {
            char patientName[50] = "Unknown";
            char doctorName[50] = "Unknown";
            
            for(int j = 0; j < patientCount; j++)
            {
                if(patients[j].id == appointments[i].patientId)
                {
                    strcpy(patientName, patients[j].name);
                    break;
                }
            }
            
            for(int j = 0; j < doctorCount; j++)
            {
                if(doctors[j].id == appointments[i].doctorId)
                {
                    strcpy(doctorName, doctors[j].name);
                    break;
                }
            }
            
            char status[15];
            if(appointments[i].status == 1) strcpy(status, "SCHEDULED");
            else if(appointments[i].status == 2) strcpy(status, "COMPLETED");
            else strcpy(status, "CANCELLED");
            
            printf("%-6d | %-19s | %-19s | %02d/%02d/%d | %-5s | %s\n", 
                   appointments[i].id,
                   patientName,
                   doctorName,
                   appointments[i].date.day,
                   appointments[i].date.month,
                   appointments[i].date.year,
                   appointments[i].time,
                   status);
        }
        printf("================================================================================\n");
        printf("Total Appointments: %d\n", appointmentCount);
    }
    
    printf("\nPress any key...");
    getch();
}

/*
================================================================================
FUNCTION: generateBill()
PURPOSE: Generate bill for patient
================================================================================
*/

void generateBill()
{
    system("cls");
    printf("\n========================================\n");
    printf("           GENERATE BILL\n");
    printf("========================================\n");
    
    int patientId;
    printf("\nEnter Patient ID: ");
    scanf("%d", &patientId);
    
    int patientIndex = -1;
    for(int i = 0; i < patientCount; i++)
    {
        if(patients[i].id == patientId)
        {
            patientIndex = i;
            break;
        }
    }
    
    if(patientIndex == -1)
    {
        printf("\nPatient not found!\n");
        printf("\nPress any key...");
        getch();
        return;
    }
    
    printf("\n========================================\n");
    printf("           HOSPITAL BILL\n");
    printf("========================================\n");
    printf("Patient ID: %d\n", patients[patientIndex].id);
    printf("Patient Name: %s\n", patients[patientIndex].name);
    printf("Age: %d\n", patients[patientIndex].age);
    printf("Gender: %c\n", patients[patientIndex].gender);
    printf("Disease: %s\n", patients[patientIndex].disease);
    printf("Admit Date: %02d/%02d/%d\n", patients[patientIndex].admitDate.day,
           patients[patientIndex].admitDate.month, patients[patientIndex].admitDate.year);
    printf("========================================\n");
    
    int roomCharges = 500 * 1; // Assume 1 day for demo
    int doctorFee = 500;
    int medicineCharges = 1000;
    int otherCharges = 200;
    
    int totalBill = roomCharges + doctorFee + medicineCharges + otherCharges;
    patients[patientIndex].billAmount = totalBill;
    
    printf("Room Charges (per day): Rs.%d\n", roomCharges);
    printf("Doctor Consultation: Rs.%d\n", doctorFee);
    printf("Medicine Charges: Rs.%d\n", medicineCharges);
    printf("Other Charges: Rs.%d\n", otherCharges);
    printf("========================================\n");
    printf("TOTAL BILL: Rs.%d\n", totalBill);
    printf("========================================\n");
    
    saveDataToFile();
    
    printf("\nPress any key...");
    getch();
}

/*
================================================================================
FUNCTION: dischargePatient()
PURPOSE: Discharge patient from hospital
================================================================================
*/

void dischargePatient()
{
    system("cls");
    printf("\n========================================\n");
    printf("           DISCHARGE PATIENT\n");
    printf("========================================\n");
    
    int patientId;
    printf("\nEnter Patient ID: ");
    scanf("%d", &patientId);
    
    int patientIndex = -1;
    for(int i = 0; i < patientCount; i++)
    {
        if(patients[i].id == patientId)
        {
            patientIndex = i;
            break;
        }
    }
    
    if(patientIndex == -1)
    {
        printf("\nPatient not found!\n");
        printf("\nPress any key...");
        getch();
        return;
    }
    
    if(patients[patientIndex].isAdmitted == 0)
    {
        printf("\nPatient is already discharged!\n");
        printf("\nPress any key...");
        getch();
        return;
    }
    
    printf("\nPatient Name: %s\n", patients[patientIndex].name);
    printf("Age: %d\n", patients[patientIndex].age);
    printf("Disease: %s\n", patients[patientIndex].disease);
    
    char confirm;
    printf("\nDischarge this patient? (Y/N): ");
    scanf(" %c", &confirm);
    
    if(confirm == 'Y' || confirm == 'y')
    {
        getCurrentDate(&patients[patientIndex].dischargeDate);
        patients[patientIndex].isAdmitted = 0;
        
        // Generate bill before discharge
        generateBill();
        
        saveDataToFile();
        
        printf("\n========================================\n");
        printf("     PATIENT DISCHARGED SUCCESSFULLY!\n");
        printf("========================================\n");
        printf("Discharge Date: %02d/%02d/%d\n", patients[patientIndex].dischargeDate.day,
               patients[patientIndex].dischargeDate.month, patients[patientIndex].dischargeDate.year);
    }
    else
    {
        printf("\nDischarge cancelled.\n");
    }
    
    printf("\nPress any key...");
    getch();
}

/*
================================================================================
FUNCTION: mainMenu()
PURPOSE: Display main menu
================================================================================
*/

void mainMenu()
{
    int choice;
    
    do
    {
        system("cls");
        printf("\n========================================\n");
        printf("     HOSPITAL MANAGEMENT SYSTEM\n");
        printf("========================================\n");
        printf("\n");
        printf(" PATIENT MANAGEMENT\n");
        printf(" 1. Add New Patient\n");
        printf(" 2. View All Patients\n");
        printf(" 3. Search Patient\n");
        printf(" 4. Discharge Patient\n");
        printf("\n");
        printf(" DOCTOR MANAGEMENT\n");
        printf(" 5. Add New Doctor\n");
        printf(" 6. View All Doctors\n");
        printf("\n");
        printf(" APPOINTMENT MANAGEMENT\n");
        printf(" 7. Book Appointment\n");
        printf(" 8. View Appointments\n");
        printf("\n");
        printf(" BILLING\n");
        printf(" 9. Generate Bill\n");
        printf("\n");
        printf(" 10. Exit\n");
        printf("\n");
        printf("========================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice)
        {
            case 1: addPatient(); break;
            case 2: viewAllPatients(); break;
            case 3: searchPatient(); break;
            case 4: dischargePatient(); break;
            case 5: addDoctor(); break;
            case 6: viewAllDoctors(); break;
            case 7: bookAppointment(); break;
            case 8: viewAppointments(); break;
            case 9: generateBill(); break;
            case 10:
                printf("\nThank you for using Hospital Management System!\n");
                break;
            default:
                printf("\nInvalid choice!\n");
                printf("\nPress any key...");
                getch();
        }
        
    } while(choice != 10);
}

/*
================================================================================
FUNCTION: main()
PURPOSE: Program entry point
================================================================================
*/

int main()
{
    loadDataFromFile();
    mainMenu();
    return 0;
}