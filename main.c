#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Struct Pasien
struct Patient {
    char name[100];           // Nama Pasien
    int age;                  // Umur Pasien
    char gender[10];          // Gender Pasien (Pria/Wanita)
    char bpjs_number[20];     // Nomor BPJS
    char address[200];        // Tempat Tinggal
    char disease[200];        // Penyakit Pasien
    int queue_number;         // Nomor Urut Pasien
    struct Patient *next;     // Pointer ke pasien berikutnya (Linked List)
};

// Struct Dokter
struct Doctor {
    char name[100];           // Nama Dokter
    char specialization[100]; // Spesialisasi Dokter
};

// Struct Klinik
struct Clinic {
    char name[100];           // Nama Klinik
    struct Doctor *doctors;   // Daftar Dokter di Klinik
    int doctor_count;         // Jumlah Dokter di Klinik
    int patient_count;        // Jumlah Pasien Terdaftar di Klinik
};

// Struct untuk menyimpan riwayat pasien yang dilayani
struct ServedPatient {
    struct Patient *patient; // Pointer ke pasien yang dilayani
    struct ServedPatient *next; // Pointer ke riwayat berikutnya
};

// Head untuk stack riwayat pasien
struct ServedPatient *servedPatientStack = NULL;
// Mendeklarasikan klinik secara global
struct Clinic clinics[] = {
    {"Klinik Topangado", (struct Doctor[]){{"Dokter Spesialis Gigi", "Gigi"}, {"Dokter Spesialis Kulit", "Kulit"}, {"Dokter Spesialis Mata", "Mata"}, {"Dokter Spesialis Gizi", "Gizi"}}, 4, 0},
    {"Klinik Kasih Ibu", (struct Doctor[]){{"Dokter Spesialis Kandungan", "Kandungan"}, {"Dokter Spesialis Anak", "Anak"}, {"Dokter Spesialis Bedah", "Bedah"}, {"Dokter Spesialis Gizi", "Gizi"}}, 4, 0},
    {"Klinik Bona Mitra", (struct Doctor[]){{"Dokter Spesialis Ortopedi", "Ortopedi"}, {"Dokter Spesialis Jantung", "Jantung"}, {"Dokter Spesialis Immunologi", "Immunologi"}, {"Dokter Spesialis Kulit", "Kulit"}, {"Dokter Spesialis Mata", "Mata"}}, 5, 0},
    {"Klinik Sehat Selalu", (struct Doctor[]){{"Dokter Spesialis Gigi", "Gigi"}, {"Dokter Spesialis Jantung", "Jantung"}, {"Dokter Spesialis Gizi", "Gizi"}, {"Dokter Spesialis Mata", "Mata"}}, 4, 0},
    {"Klinik Bunda Rahma", (struct Doctor[]){{"Dokter Spesialis Anak", "Anak"}, {"Dokter Spesialis Gigi", "Gigi"}, {"Dokter Spesialis Gizi", "Gizi"}, {"Dokter Spesialis Mata", "Mata"}, {"Dokter Spesialis Kulit", "Kulit"}}, 5, 0},
    {"Klinik Jayadi", (struct Doctor[]){{"Dokter Spesialis Jantung", "Jantung"}, {"Dokter Spesialis Immunologi", "Immunologi"}, {"Dokter Spesialis Bedah", "Bedah"}}, 3, 0},
    {"Klinik Pratama Kita", (struct Doctor[]){{"Dokter Spesialis Ortopedi", "Ortopedi"}, {"Dokter Spesialis Immunologi", "Immunologi"}, {"Dokter Spesialis Anak", "Anak"}, {"Dokter Spesialis Bedah", "Bedah"}}, 4, 0}
};
int clinicCount = sizeof(clinics) / sizeof(clinics[0]);

// Head untuk linked list pasien
struct Patient *patientHead = NULL;
int patientQueueCounter = 1; // Counter untuk nomor urut pasien

struct Feedback {
    char message[500]; // Pesan feedback
    char type[20];     // Jenis feedback (Dokter/Pelayan/Kasir)
    char timestamp[20]; // Timestamp for when the feedback was given
    struct Feedback *next; // Pointer ke feedback berikutnya
};

// Head untuk linked list feedback
struct Feedback *feedbackHead = NULL;

// Fungsi untuk menambahkan pasien baru
int countPatients(); // Forward declaration agar bisa dipakai di addPatient()

void addPatient() {
    struct Patient *newPatient = (struct Patient *)malloc(sizeof(struct Patient));

    printf("\n=== Registrasi Pasien Baru ===\n");
    printf("Nama: ");
    getchar(); // Membersihkan buffer sebelum membaca input string
    fgets(newPatient->name, sizeof(newPatient->name), stdin);
    newPatient->name[strcspn(newPatient->name, "\n")] = '\0'; // Menghapus newline

    printf("Umur: ");
    scanf("%d", &newPatient->age);
    getchar(); // Membersihkan buffer sebelum membaca input string berikutnya

    printf("Gender (Pria/Wanita): ");
    fgets(newPatient->gender, sizeof(newPatient->gender), stdin);
    newPatient->gender[strcspn(newPatient->gender, "\n")] = '\0'; // Menghapus newline

    printf("Nomor BPJS: ");
    fgets(newPatient->bpjs_number, sizeof(newPatient->bpjs_number), stdin);
    newPatient->bpjs_number[strcspn(newPatient->bpjs_number, "\n")] = '\0';

    printf("Alamat: ");
    fgets(newPatient->address, sizeof(newPatient->address), stdin);
    newPatient->address[strcspn(newPatient->address, "\n")] = '\0';

    printf("Konsultasi dengan dokter: ");
    fgets(newPatient->disease, sizeof(newPatient->disease), stdin);
    newPatient->disease[strcspn(newPatient->disease, "\n")] = '\0';

    // Mengatur nomor urut berdasarkan jumlah pasien yang ada
    newPatient->queue_number = countPatients() + 1; // Set nomor urut
    newPatient->next = NULL;

    if (patientHead == NULL) {
        patientHead = newPatient;
    } else {
        struct Patient *temp = patientHead;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newPatient;
    }

    printf("\nPasien berhasil diregistrasi dengan nomor urut: %d\n", newPatient->queue_number);
}

// Define the diseases, age groups, and gender groups
const char *diseases[] = {"Gigi", "Ortopedi", "Immunologi", "Jantung", "Bedah", "Anak", "Kandungan", "Kulit", "Mata", "Gizi"};
const char *ageGroups[] = {"0-18", "19-30", "31-50", "51 ke atas"};
const char *genders[] = {"Pria", "Wanita"}; // Gender groups

// Function to display disease statistics
void displayDiseaseStatistics() {
    struct ServedPatient *temp = servedPatientStack;
    int diseaseCount[10][4] = {0}; // 10 diseases and 4 age groups

    // Count occurrences of each disease in each age group
    while (temp != NULL) {
        int age = temp->patient->age;
        int ageGroupIndex = -1;

        // Determine the age group index
        if (age >= 0 && age <= 18) {
            ageGroupIndex = 0;
        } else if (age >= 19 && age <= 30) {
            ageGroupIndex = 1;
        } else if (age >= 31 && age <= 50) {
            ageGroupIndex = 2;
        } else if (age > 50) {
            ageGroupIndex = 3;
        }

        // Count the disease occurrences
        for (int i = 0; i < 10; i++) {
            if (strcasecmp(temp->patient->disease, diseases[i]) == 0 && ageGroupIndex != -1) {
                diseaseCount[i][ageGroupIndex]++;
            }
        }
        temp = temp->next;
    }

    // Display the results
    printf("\n=== Statistik Penyakit Berdasarkan Kelompok Umur ===\n");
    for (int j = 0; j < 4; j++) { // For each age group
        printf("\nKelompok Umur: %s\n", ageGroups[j]);
        int maxCount = 0, minCount = INT_MAX;
        char *mostCommonDisease = NULL, *leastCommonDisease = NULL;

        for (int i = 0; i < 10; i++) { // For each disease
            if (diseaseCount[i][j] > maxCount) {
                maxCount = diseaseCount[i][j];
                mostCommonDisease = (char *)diseases[i];
            }
            if (diseaseCount[i][j] < minCount && diseaseCount[i][j] > 0) {
                minCount = diseaseCount[i][j];
                leastCommonDisease = (char *)diseases[i];
            }
        }

        if (mostCommonDisease != NULL) {
            printf("Penyakit Paling Umum: %s (Jumlah: %d)\n", mostCommonDisease, maxCount);
        } else {
            printf("Tidak ada data untuk kelompok umur ini.\n");
        }

        if (leastCommonDisease != NULL) {
            printf("Penyakit Paling Sedikit: %s (Jumlah: %d)\n", leastCommonDisease, minCount);
        } else {
            printf("Tidak ada data untuk kelompok umur ini.\n");
        }
    }
    // Ask user if they want to save the statistics to a file
    char saveToFile;
    printf("\nApakah Anda ingin menyimpan statistik penyakit ke dalam file (Y/N)? ");
    scanf(" %c", &saveToFile); // Note the space before %c to consume any leftover newline

    if (saveToFile == 'Y' || saveToFile == 'y') {
        // Open file for writing
        FILE *file = fopen("disease_statistics.txt", "w");
        if (file == NULL) {
            printf("Error opening file for writing.\n");
            return;
        }

        // Write statistics to the file
        fprintf(file, "=== Statistik Penyakit Berdasarkan Kelompok Umur ===\n");
        for (int j = 0; j < 4; j++) { // For each age group
            fprintf(file, "\nKelompok Umur: %s\n", ageGroups[j]);
            for (int i = 0; i < 10; i++) { // For each disease
                if (diseaseCount [i][j] > 0) {
                    fprintf(file, "Penyakit: %s (Jumlah: %d)\n", diseases[i], diseaseCount[i][j]);
                }
            }
        }

        fclose(file);
        printf("Statistik penyakit telah disimpan ke dalam file 'disease_statistics.txt'.\n");
    } else {
        printf("Statistik tidak disimpan.\n");
    }
}

void giveFeedback() {
    struct Feedback *newFeedback = (struct Feedback *)malloc(sizeof(struct Feedback));

    printf("\n=== Feedback ===\n");

    // Validate feedback type
    while (1) {
        printf("Pilih jenis feedback (Dokter/Pelayan/Kasir): ");
        fgets(newFeedback->type, sizeof(newFeedback->type), stdin);
        newFeedback->type[strcspn(newFeedback->type, "\n")] = '\0'; // Menghapus newline

        // Check if the type is valid
        if (strcasecmp(newFeedback->type, "Dokter") == 0 ||
            strcasecmp(newFeedback->type, "Pelayan") == 0 ||
            strcasecmp(newFeedback->type, "Kasir") == 0) {
            break; // Valid input, exit loop
        } else {
            printf("Jenis feedback tidak valid. Silakan coba lagi.\n");
        }
    }

    printf("Masukkan pesan feedback: ");
    fgets(newFeedback->message, sizeof(newFeedback->message), stdin);
    newFeedback->message[strcspn(newFeedback->message, "\n")] = '\0'; // Menghapus newline

    // Add timestamp
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    sprintf(newFeedback->timestamp, "%02d-%02d-%04d %02d:%02d:%02d",
            t->tm_mday, t->tm_mon + 1, t->tm_year + 1900,
            t->tm_hour, t->tm_min, t->tm_sec);

    newFeedback->next = feedbackHead; // Menambahkan feedback ke linked list
    feedbackHead = newFeedback;

    printf("Feedback berhasil diberikan. Terima kasih!\n");
}

// Function to display all feedback
void printFeedback() {
    struct Feedback *temp = feedbackHead;

    if (temp == NULL) {
        printf("\nTidak ada feedback yang diberikan.\n");
        return;
    }

    printf("\n=== Daftar Feedback ===\n");
    while (temp != NULL) {
        printf("Jenis: %s\n", temp->type);
        printf("Pesan: %s\n", temp->message);
        printf("Waktu: %s\n", temp->timestamp); // Display timestamp
        printf("---------------------------\n");
        temp = temp->next;
    }
}

// Fungsi untuk mencetak daftar pasien
void printPatients() {
    struct Patient *temp = patientHead;

    if (temp == NULL) {
        printf("\nTidak ada pasien yang terdaftar.\n");
        return;
    }

    printf("\n=== Daftar Pasien Terdaftar ===\n");
    while (temp != NULL) {
        printf("Nomor Urut: %d\n", temp->queue_number);
        printf("Name: %s\n", temp->name);
        printf("Age: %d\n", temp->age);
        printf("Gender: %s\n", temp->gender); // Menampilkan gender
        printf("BPJS Number: %s\n", temp->bpjs_number);
        printf("Address: %s\n", temp->address);
        printf("Disease: %s\n", temp->disease);
        printf("---------------------------\n");
        temp = temp->next;
    }
}

void recommendClinics(char *disease); // Forward declaration

void servePatient() {
    if (patientHead == NULL) {
        printf("\nTidak ada pasien yang dapat dilayani.\n");
        return;
    }

    struct Patient *temp = patientHead;
    patientHead = patientHead->next; // Menghapus pasien dari daftar
    printf("\nPasien yang dilayani:\n");
    printf("Nomor Urut: %d\n", temp->queue_number);
    printf("Name: %s\n", temp->name);
    printf("Age: %d\n", temp->age);
    printf("BPJS Number: %s\n", temp->bpjs_number);
    printf("Address: %s\n", temp->address);
    printf("Disease: %s\n", temp->disease);
    printf("---------------------------\n");

    // Rekomendasi klinik berdasarkan penyakit
    recommendClinics(temp->disease);

    // Menanyakan nama klinik yang dipilih oleh servant
    char clinicName[100];
    printf("\nMasukkan nama klinik untuk pasien: ");
     // Membersihkan buffer sebelum membaca input string
    fgets(clinicName, sizeof(clinicName), stdin);
    clinicName[strcspn(clinicName, "\n")] = '\0'; // Menghapus newline

    // Mencari klinik berdasarkan nama
    int found = 0;
    for (int i = 0; i < clinicCount; i++) {
        // Gunakan strcasecmp agar case-insensitive
        if (strcasecmp(clinics[i].name, clinicName) == 0) {
            printf("Pasien akan ditangani di klinik: %s\n", clinics[i].name);

            // Menambah jumlah antrian di klinik
            clinics[i].patient_count++;

            // Menambah antrian pada dokter yang relevan berdasarkan penyakit pasien
            for (int j = 0; j < clinics[i].doctor_count; j++) {
                if (strcmp(clinics[i].doctors[j].specialization, temp->disease) == 0) {
                    printf("Dokter yang dituju: %s\n", clinics[i].doctors[j].name);
                    break;
                }
            }
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Klinik dengan nama '%s' tidak ditemukan.\n", clinicName);
    }

    // Menyimpan pasien yang dilayani ke dalam stack
    struct ServedPatient *newServedPatient = (struct ServedPatient *)malloc(sizeof(struct ServedPatient));
    newServedPatient->patient = temp;
    newServedPatient->next = servedPatientStack;
    servedPatientStack = newServedPatient; // Push ke stack

    // Pastikan jangan double free, cukup free(temp) jika memang dialokasikan di luar
    free(temp);
}

void printServedPatients() {
    struct ServedPatient *temp = servedPatientStack;

    if (temp == NULL) {
        printf("\nTidak ada riwayat pasien yang dilayani.\n");
        return;
    }

    printf("\n=== Riwayat Pasien yang Dilayani ===\n");
    while (temp != NULL) {
        printf("Nomor Urut: %d\n", temp->patient->queue_number);
        printf("Name: %s\n", temp->patient->name);
        printf("Age: %d\n", temp->patient->age);
        printf("BPJS Number: %s\n", temp->patient->bpjs_number);
        printf("Address: %s\n", temp->patient->address);
        printf("Disease: %s\n", temp->patient->disease);
        printf("Gender: %s\n", temp->patient->gender);
        printf("---------------------------\n");
        temp = temp->next;
    }
}

void countPatientsByDisease() {
    char disease[200];
    struct ServedPatient *temp = servedPatientStack; // Menggunakan stack pasien yang dilayani
    int count = 0;

    printf("\n=== Statistik Penyakit ===\n");
    printf("Masukkan nama penyakit: ");
    fgets(disease, sizeof(disease), stdin);
    disease[strcspn(disease, "\n")] = '\0'; // Menghapus newline

    // Loop untuk menghitung jumlah pasien dengan penyakit tertentu
    while (temp != NULL) {
        if (strcasecmp(temp->patient->disease, disease) == 0) {
            count++;
        }
        temp = temp->next;
    }

    // Menampilkan hasil pencarian
    if (count > 0) {
        printf("Jumlah pasien yang dilayani dengan penyakit '%s': %d\n", disease, count);
    } else {
        printf("Tidak ada pasien yang dilayani dengan penyakit '%s'.\n", disease);
    }
      // Ask user if they want to save the statistics to a file
    char saveToFile;
    printf("\nApakah Anda ingin menyimpan statistik penyakit ke dalam file (Y/N)? ");
    scanf(" %c", &saveToFile); // Note the space before %c to consume any leftover newline

    if (saveToFile == 'Y' || saveToFile == 'y') {
        // Open file for writing
        FILE *file = fopen("disease_statistics.txt", "w");
        if (file == NULL) {
            printf("Error opening file for writing.\n");
            return;
        }

        // Write statistics to the file
        fprintf(file, "=== Statistik Penyakit Pasien ===\n");
        fprintf(file, "Penyakit: %s\n", disease);
        fprintf(file, "Jumlah pasien yang dilayani: %d\n", count);

        // Close the file
        fclose(file);

        printf("Statistik penyakit telah disimpan ke dalam 'disease_statistics.txt'.\n");
    } else {
        printf("Proses penyimpanan dibatalkan. Statistik penyakit tidak disimpan.\n");
    }
}

void searchPatientByName() {
    char name[100];
    struct Patient *temp = patientHead;

    printf("\n=== Cari Pasien Berdasarkan Nama ===\n");
    printf("Masukkan Nama Pasien: ");
    getchar(); // Membersihkan buffer sebelum membaca input string
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0'; // Menghapus newline

    while (temp != NULL) {
        if (strcasecmp(temp->name, name) == 0) { // case-insensitive
            printf("\nPasien ditemukan:\n");
            printf("Nomor Urut: %d\n", temp->queue_number);
            printf("Name: %s\n", temp->name);
            printf("Age: %d\n", temp->age);
            printf("Gender: %s\n", temp->gender);
            printf("BPJS Number: %s\n", temp->bpjs_number);
            printf("Address: %s\n", temp->address);
            printf("Disease: %s\n", temp->disease);
            printf("---------------------------\n");
            return;
        }
        temp = temp->next;
    }

    printf("Pasien dengan nama %s tidak ditemukan.\n", name);
}

void processPayment() {
    char bpjs_number[20];
    struct Patient *temp = patientHead;

    printf("\n=== Proses Pembayaran ===\n");
    printf("Masukkan Nomor BPJS Pasien: ");
    getchar(); // Membersihkan buffer sebelum membaca input string
    fgets(bpjs_number, sizeof(bpjs_number), stdin);
    bpjs_number[strcspn(bpjs_number, "\n")] = '\0'; // Menghapus newline

    while (temp != NULL) {
        if (strcmp(temp->bpjs_number, bpjs_number) == 0) {
            printf("\nPasien ditemukan:\n");
            printf("Nomor Urut: %d\n", temp->queue_number);
            printf("Nama: %s\n", temp->name);
            printf("Umur: %d\n", temp->age);
            printf("Gender: %s\n", temp->gender);
            printf("Nomor BPJS: %s\n", temp->bpjs_number);
            printf("Alamat: %s\n", temp->address);
            printf("Penyakit: %s\n", temp->disease);
            printf("---------------------------\n");

            char confirmation;
            printf("Apakah pasien ingin melanjutkan pembayaran? (y/n): ");
            scanf(" %c", &confirmation);

            if (confirmation == 'y' || confirmation == 'Y') {
                printf("Proses pembayaran untuk pasien %s berhasil.\n", temp->name);
                return; // Pembayaran selesai
            } else {
                printf("Pembayaran dibatalkan.\n");
                return;
            }
        }
        temp = temp->next;
    }

    printf("Pasien dengan nomor BPJS %s tidak ditemukan.\n", bpjs_number);
}

void recommendClinics(char *disease) {
    int recommendedClinics[clinicCount];
    int recommendedCount = 0;

    for (int i = 0; i < clinicCount; i++) {
        for (int j = 0; j < clinics[i].doctor_count; j++) {
            if (strcmp(clinics[i].doctors[j].specialization, disease) == 0) {
                recommendedClinics[recommendedCount++] = i;
                break;
            }
        }
    }

    if (recommendedCount > 0) {
        printf("\nRekomendasi klinik dengan dokter spesialis %s:\n", disease);
        for (int i = 0; i < recommendedCount; i++) {
            int index = recommendedClinics[i];
            printf("%d. Klinik: %s (Jumlah antrian: %d)\n", i + 1, clinics[index].name, clinics[index].patient_count);
        }
    } else {
        printf("\nTidak ada klinik yang sesuai untuk penyakit %s.\n", disease);
    }
}

void countPatientsByAgeGroup() {
    struct ServedPatient *temp = servedPatientStack;
    int ageGroup1 = 0; // 0-18
    int ageGroup2 = 0; // 19-30
    int ageGroup3 = 0; // 31-50
    int ageGroup4 = 0; // 51 and above

    while (temp != NULL) {
        if (temp->patient->age >= 0 && temp->patient->age <= 18) {
            ageGroup1++;
        } else if (temp->patient->age >= 19 && temp->patient->age <= 30) {
            ageGroup2++;
        } else if (temp->patient->age >= 31 && temp->patient->age <= 50) {
            ageGroup3++;
        } else if (temp->patient->age > 50) {
            ageGroup4++;
        }
        temp = temp->next;
    }

    // Display the statistics
    printf("\n=== Age Statistics ===\n");
    printf("Number of patients aged 0-18 years: %d\n", ageGroup1);
    printf("Number of patients aged 19-30 years: %d\n", ageGroup2);
    printf("Number of patients aged 31-50 years: %d\n", ageGroup3);
    printf("Number of patients aged 51 years and above: %d\n", ageGroup4);

    // Ask user if they want to save the statistics to a file
    char saveToFile;
    printf("\nApakah Anda ingin menyimpan statistik umur ke dalam file (Y/N)? ");
    scanf(" %c", &saveToFile); // Note the space before %c to consume any leftover newline

    if (saveToFile == 'Y' || saveToFile == 'y') {
        // Open file for writing
        FILE *file = fopen("age_statistics.txt", "w");
        if (file == NULL) {
            printf("Error opening file for writing.\n");
            return;
        }

        // Write statistics to the file
        fprintf(file, "=== Statistik Umur Pasien ===\n");
        fprintf(file, "Jumlah pasien berusia 0-18 tahun: %d\n", ageGroup1);
        fprintf(file, "Jumlah pasien berusia 19-30 tahun: %d\n", ageGroup2);
        fprintf(file, "Jumlah pasien berusia 31-50 tahun: %d\n", ageGroup3);
        fprintf(file, "Jumlah pasien berusia 51 tahun ke atas: %d\n", ageGroup4);

        // Close the file
        fclose(file);

        printf("Statistik umur telah disimpan ke dalam 'age_statistics.txt'.\n");
    } else {
        printf("Proses penyimpanan dibatalkan. Statistik umur tidak disimpan.\n");
    }
}

void printServiceDensity() {
    int count = 0;
    struct Patient *temp = patientHead;

    while (temp != NULL) {
        count++;
        temp = temp->next;
    }
    printf("\n=== Kepadatan Layanan ===\n");
    printf("Jumlah pasien terdaftar: %d\n", count);
    printf("---------------------------\n");
}

void editPatientData() {
    int queue_number;
    struct Patient *temp = patientHead;

    printf("\n=== Edit Data Pasien ===\n");
    printf("Masukkan Nomor Urut Pasien yang ingin diedit: ");
    scanf("%d", &queue_number);

    while (temp != NULL) {
        if (temp->queue_number == queue_number) {
            printf("Editing data for Pasien: %s\n", temp->name);
            printf("Umur (sekarang: %d): ", temp->age);
            scanf("%d", &temp->age);
            getchar(); // Membersihkan buffer

            printf("Gender (sekarang: %s): ", temp->gender);
            fgets(temp->gender, sizeof(temp->gender), stdin);
            temp->gender[strcspn(temp->gender, "\n")] = '\0'; // Menghapus newline

            printf("Nomor BPJS (sekarang: %s): ", temp->bpjs_number);
            fgets(temp->bpjs_number, sizeof(temp->bpjs_number), stdin);
            temp->bpjs_number[strcspn(temp->bpjs_number, "\n")] = '\0';

            printf("Alamat (sekarang: %s): ", temp->address);
            fgets(temp->address, sizeof(temp->address), stdin);
            temp->address[strcspn(temp->address, "\n")] = '\0';

            printf("Penyakit (sekarang: %s): ", temp->disease);
            fgets(temp->disease, sizeof(temp->disease), stdin);
            temp->disease[strcspn(temp->disease, "\n")] = '\0';

            printf("Data pasien berhasil diperbarui.\n");
            return;
        }
        temp = temp->next;
    }

    printf("Pasien dengan nomor urut %d tidak ditemukan.\n", queue_number);
}

void sortServedPatientsByName() {
    if (servedPatientStack == NULL) {
        printf("\nTidak ada pasien yang telah dilayani untuk diurutkan.\n");
        return;
    }

    // Create an array to hold the served patients for sorting
    int count = 0;
    struct ServedPatient *temp = servedPatientStack;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }

    struct Patient *servedPatientsArray[count];
    temp = servedPatientStack;
    for (int i = 0; i < count; i++) {
        servedPatientsArray[i] = temp->patient;
        temp = temp->next;
    }

    // Bubble sort the served patients by name
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcasecmp(servedPatientsArray[j]->name, servedPatientsArray[j + 1]->name) > 0) {
                // Swap
                struct Patient *tempPatient = servedPatientsArray[j];
                servedPatientsArray[j] = servedPatientsArray[j + 1];
                servedPatientsArray[j + 1] = tempPatient;
            }
        }
    }

    // Ask user if they want to save the sorted data to a file
    char saveToFile;
    printf("\nApakah Anda ingin menyimpan daftar pasien yang telah dilayani dan terurut ke dalam file (Y/N)? ");
    scanf(" %c", &saveToFile); // Note the space before %c to consume any leftover newline

    if (saveToFile == 'Y' || saveToFile == 'y') {
        // Open file for writing
        FILE *file = fopen("sorted_served_patients.txt", "w");
        if (file == NULL) {
            printf("Error opening file for writing.\n");
            return;
        }

        // Write sorted served patient data to the file
        fprintf(file, "=== Daftar Pasien yang Telah Dilayani dan Terurut Berdasarkan Nama ===\n");
        for (int i = 0; i < count; i++) {
            fprintf(file, "Nomor Urut: %d\n", servedPatientsArray[i]->queue_number);
            fprintf(file, "Nama: %s\n", servedPatientsArray[i]->name);
            fprintf(file, "Umur: %d\n", servedPatientsArray[i]->age);
            fprintf(file, "Gender: %s\n", servedPatientsArray[i]->gender);
            fprintf(file, "Nomor BPJS: %s\n", servedPatientsArray[i]->bpjs_number);
            fprintf(file, "Alamat: %s\n", servedPatientsArray[i]->address);
            fprintf(file, "Penyakit: %s\n", servedPatientsArray[i]->disease);
            fprintf(file, "---------------------------\n");
        }

        // Close the file
        fclose(file);

        printf("\nDaftar pasien yang telah dilayani telah diurutkan berdasarkan nama dan disimpan ke dalam 'sorted_served_patients.txt'.\n");
    } else {
        printf("Proses penyimpanan dibatalkan. Daftar pasien tidak disimpan.\n");
    }
}

void printDoctors() {
    struct Doctor doctors[] = {
        {"Dokter Spesialis Gigi", "Gigi"},
        {"Dokter Spesialis Ortopedi", "Ortopedi"},
        {"Dokter Spesialis Immunologi", "Immunologi"},
        {"Dokter Spesialis Jantung", "Jantung"},
        {"Dokter Spesialis Bedah", "Bedah"},
        {"Dokter Spesialis Anak", "Anak"},
        {"Dokter Spesialis Kandungan", "Kandungan"},
        {"Dokter Spesialis Kulit", "Kulit"},
        {"Dokter Spesialis Mata", "Mata"},
        {"Dokter Spesialis Gizi", "Gizi"}
    };

    printf("\n=== Daftar Dokter ===\n");
    for (int i = 0; i < (int)(sizeof(doctors) / sizeof(doctors[0])); i++) {
        printf("%s\n", doctors[i].name);
    }
    printf("---------------------------\n");
}

void printClinics() {
    printf("\n=== Daftar Klinik dan Dokter ===\n");
    for (int i = 0; i < clinicCount; i++) {
        printf("%s (Dokter: ", clinics[i].name);
        for (int j = 0; j < clinics[i].doctor_count; j++) {
            printf("%s", clinics[i].doctors[j].specialization);
            if (j < clinics[i].doctor_count - 1) {
                printf(", ");
            }
        }
        printf(") [Jumlah Antrian: %d]\n", clinics[i].patient_count);
    }
    printf("---------------------------\n");
}

void sortPatientsByAge() {
    if (patientHead == NULL) {
        printf("\nTidak ada pasien yang terdaftar untuk diurutkan.\n");
        return;
    }

    struct Patient *current, *next;
    char tempName[100], tempGender[10], tempBPJS[20], tempAddress[200], tempDisease[200];
    int tempAge, tempQueueNumber;

    // Bubble sort
    for (current = patientHead; current != NULL; current = current->next) {
        for (next = current->next; next != NULL; next = next->next) {
            if (current->age > next->age) {
                // Swap
                strcpy(tempName, current->name);
                strcpy(current->name, next->name);
                strcpy(next->name, tempName);

                tempAge = current->age;
                current->age = next->age;
                next->age = tempAge;

                strcpy(tempGender, current->gender);
                strcpy(current->gender, next->gender);
                strcpy(next->gender, tempGender);

                strcpy(tempBPJS, current->bpjs_number);
                strcpy(current->bpjs_number, next->bpjs_number);
                strcpy(next->bpjs_number, tempBPJS);

                strcpy(tempAddress, current->address);
                strcpy(current->address, next->address);
                strcpy(next->address, tempAddress);

                strcpy(tempDisease, current->disease);
                strcpy(current->disease, next->disease);
                strcpy(next->disease, tempDisease);

                tempQueueNumber = current->queue_number;
                current->queue_number = next->queue_number;
                next->queue_number = tempQueueNumber;
            }
        }
    }

    // Ask user if they want to save the sorted data to a file
    char saveToFile;
    printf("\nApakah Anda ingin menyimpan daftar pasien yang terurut berdasarkan umur ke dalam file (Y/N)? ");
    scanf(" %c", &saveToFile); // Note the space before %c to consume any leftover newline

    if (saveToFile == 'Y' || saveToFile == 'y') {
        // Open file for writing
        FILE *file = fopen("sorted_patients_by_age.txt", "w");
        if (file == NULL) {
            printf("Error opening file for writing.\n");
            return;
        }

        // Write sorted patient data to the file
        struct Patient *temp = patientHead;
        fprintf(file, "=== Daftar Pasien Terurut Berdasarkan Umur ===\n");
        while (temp != NULL) {
            fprintf(file, "Nomor Urut: %d\n", temp->queue_number);
            fprintf(file, "Nama: %s\n", temp->name);
            fprintf(file, "Umur: %d\n", temp->age);
            fprintf(file, "Gender: %s\n", temp->gender);
            fprintf(file, "Nomor BPJS: %s\n", temp->bpjs_number);
            fprintf(file, "Alamat: %s\n", temp->address);
            fprintf(file, "Penyakit: %s\n", temp->disease);
            fprintf(file, "---------------------------\n");
            temp = temp->next;
        }

        // Close the file
        fclose(file);

        printf("\nDaftar pasien telah diurutkan berdasarkan umur dan disimpan ke dalam 'sorted_patients_by_age.txt'.\n");
    } else {
        printf("Proses penyimpanan dibatalkan. Daftar pasien tidak disimpan.\n");
    }
}

void searchPatientByBPJS() {
    char bpjs_number[20];
    struct Patient *temp = patientHead;

    printf("\n=== Cari Pasien Berdasarkan Nomor BPJS ===\n");
    printf("Masukkan Nomor BPJS: ");
    getchar(); // Membersihkan buffer sebelum membaca input string
    fgets(bpjs_number, sizeof(bpjs_number), stdin);
    bpjs_number[strcspn(bpjs_number, "\n")] = '\0';

    while (temp != NULL) {
        if (strcmp(temp->bpjs_number, bpjs_number) == 0 ) {
            printf("\nPasien ditemukan:\n");
            printf("Nomor Urut: %d\n", temp->queue_number);
            printf("Name: %s\n", temp->name);
            printf("Age: %d\n", temp->age);
            printf("Gender: %s\n", temp->gender);
            printf("BPJS Number: %s\n", temp->bpjs_number);
            printf("Address: %s\n", temp->address);
            printf("Disease: %s\n", temp->disease);
            printf("---------------------------\n");
            return;
        }
        temp = temp->next;
    }

    printf("Pasien dengan nomor BPJS %s tidak ditemukan.\n", bpjs_number);
}

void displayDiseaseStatisticsByGender() {
    struct ServedPatient *temp = servedPatientStack;
    int diseaseCount[10][2] = {0}; // 10 diseases and 2 genders

    // Count occurrences of each disease for each gender
    while (temp != NULL) {
        for (int i = 0; i < 10; i++) {
            if (strcasecmp(temp->patient->disease, diseases[i]) == 0) {
                if (strcasecmp(temp->patient->gender, "Pria") == 0) {
                    diseaseCount[i][0]++; // Count for male
                } else if (strcasecmp(temp->patient->gender, "Wanita") == 0) {
                    diseaseCount[i][1]++; // Count for female
                }
            }
        }
        temp = temp->next;
    }

    // Display the results for each gender
    for (int j = 0; j < 2; j++) { // For each gender
        printf("\n=== Statistik Penyakit untuk Gender: %s ===\n", genders[j]);
        int maxCount = 0, minCount = INT_MAX;
        char *mostCommonDisease = NULL, *leastCommonDisease = NULL;

        for (int i = 0; i < 10; i++) { // For each disease
            if (diseaseCount[i][j] > maxCount) {
                maxCount = diseaseCount[i][j];
                mostCommonDisease = (char *)diseases[i];
            }
            if (diseaseCount[i][j] < minCount && diseaseCount[i][j] > 0) {
                minCount = diseaseCount[i][j];
                leastCommonDisease = (char *)diseases[i];
            }
        }

        if (mostCommonDisease != NULL) {
            printf("Penyakit Paling Umum: %s (Jumlah: %d)\n", mostCommonDisease, maxCount);
        } else {
            printf("Tidak ada data untuk gender ini.\n");
        }

        if (leastCommonDisease != NULL) {
            printf("Penyakit Paling Sedikit: %s (Jumlah: %d)\n", leastCommonDisease, minCount);
        } else {
            printf("Tidak ada data untuk gender ini.\n");
        }
    }

    // Ask user if they want to save the statistics to a file
    char saveToFile;
    printf("\nApakah Anda ingin menyimpan statistik penyakit berdasarkan gender ke dalam file (Y/N)? ");
    scanf(" %c", &saveToFile); // Note the space before %c to consume any leftover newline

    if (saveToFile == 'Y' || saveToFile == 'y') {
        // Open file for writing
        FILE *file = fopen("disease_statistics_by_gender.txt", "w");
        if (file == NULL) {
            printf("Error opening file for writing.\n");
            return;
        }

        // Write statistics to the file
        fprintf(file, "=== Statistik Penyakit Berdasarkan Gender ===\n");
        for (int j = 0; j < 2; j++) { // For each gender
            fprintf(file, "\nGender: %s\n", genders[j]);
            for (int i = 0; i < 10; i++) { // For each disease
                if (diseaseCount[i][j] > 0) {
                    fprintf(file, "Penyakit: %s (Jumlah: %d)\n", diseases[i], diseaseCount[i][j]);
                }
            }
        }

        fclose(file);
        printf("Statistik penyakit berdasarkan gender telah disimpan ke dalam file 'disease_statistics_by_gender.txt'.\n");
    } else {
        printf("Statistik tidak disimpan.\n");
    }
}

void countPatientsByGender() {
    struct ServedPatient *temp = servedPatientStack;
    int maleCount = 0, femaleCount = 0;

    while (temp != NULL) {
        if (strcasecmp(temp->patient->gender, "Pria") == 0) {
            maleCount++;
        } else if (strcasecmp(temp->patient->gender, "Wanita") == 0) {
            femaleCount++;
        }
        temp = temp->next;
    }

    // Display the statistics
    printf("\n=== Statistik Gender ===\n");
    printf("Jumlah pasien Pria: %d\n", maleCount);
    printf("Jumlah pasien Wanita: %d\n", femaleCount);

    // Ask user if they want to save the statistics to a file
    char saveToFile;
    printf("\nApakah Anda ingin menyimpan statistik gender ke dalam file (Y/N)? ");
    scanf(" %c", &saveToFile); // Note the space before %c to consume any leftover newline

    if (saveToFile == 'Y' || saveToFile == 'y') {
        // Open file for writing
        FILE *file = fopen("gender_statistics.txt", "w");
        if (file == NULL) {
            printf("Error opening file for writing.\n");
            return;
        }

        // Write statistics to the file
        fprintf(file, "=== Statistik Gender Pasien ===\n");
        fprintf(file, "Jumlah pasien Pria: %d\n", maleCount);
        fprintf(file, "Jumlah pasien Wanita: %d\n", femaleCount);

        // Close the file
        fclose(file);

        printf("Statistik gender telah disimpan ke dalam 'gender_statistics.txt'.\n");
    } else {
        printf("Proses penyimpanan dibatalkan. Statistik gender tidak disimpan.\n");
    }
}

int countPatients() {
    int count = 0;
    struct Patient *temp = patientHead;

    while (temp != NULL) {
        count++;
        temp = temp->next;
    }
    return count;
}

// Function to check the current queue of patients
void checkCurrentQueue() {
    struct Patient *temp = patientHead;

    if (temp == NULL) {
        printf("\nTidak ada pasien yang terdaftar dalam antrian.\n");
        return;
    }

    printf("\n=== Antrian Pasien Sekarang ===\n");
    while (temp != NULL) {
        printf("Nomor Urut: %d\n", temp->queue_number);
        printf("Nama: %s\n", temp->name);
        printf("Umur: %d\n", temp->age);
        printf("Gender: %s\n", temp->gender);
        printf("Nomor BPJS: %s\n", temp->bpjs_number);
        printf("Alamat: %s\n", temp->address);
        printf("Penyakit: %s\n", temp->disease);
        printf("---------------------------\n");
        temp = temp->next;
    }
}

// Modify the loginAsCashier function to include the new option
void loginAsCashier() {
    int choice;
    while (1) {
        printf("\n=== Menu Kasir ===\n");
        printf("1. Proses Pembayaran\n");
        printf("2. Cari Pasien Berdasarkan Nama\n");
        printf("3. Tampilkan Feedback\n");
        printf("4. Cek Antrian Sekarang\n"); // New option added
        printf("5. Logout\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                processPayment();
                break;
            case 2:
                searchPatientByName();
                break;
            case 3:
                printFeedback();
                break;
            case 4:
                checkCurrentQueue(); // Call the new function
                break;
            case 5:
                printf("Logout Successful!\n");
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}
void loginAsServant() {
    char username[50], password[50];

    printf("\n=== Login as Servant ===\n");
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    if (strcmp(username, "admin") == 0 && strcmp(password, "admin") == 0) {
        printf("Login Successful!\n");

        int choice;
        while (1) {
            printf("\n=== Menu Pelayan ===\n");
            printf("1. Tampilkan Daftar Pasien\n");
            printf("2. Layani Pasien\n");
            printf("3. Tampilkan Kepadatan Layanan\n");
            printf("4. Tampilkan Daftar Dokter\n");
            printf("5. Tampilkan Daftar Klinik dan Dokter\n");
            printf("6. Cari Pasien Berdasarkan Nomor BPJS\n");
            printf("7. Edit Data Pasien\n");
            printf("8. Data Statistik Pasien\n");
            printf("9. Logout\n");
            printf("10. Tampilkan history pelayanan\n");
            printf("Choose an option: ");
            scanf("%d", &choice);
            getchar();

            switch (choice) {
                case 1:
                    printPatients();
                    break;
                case 2:
                    servePatient();
                    break;
                case 3:
                    printServiceDensity();
                    break;
                case 4:
                    printDoctors();
                    break;
                case 5:
                    printClinics();
                    break;
                case 6:
                    searchPatientByBPJS();
                    break;
                case 7:
                    editPatientData();
                    break;
                case 8:
                {
                    int statChoice;
                    while (1) {
                        printf("\n=== Statistik Pasien ===\n");
                        printf("1. Tampilkan Pasien Berdasarkan Urutan Nama\n");
                        printf("2. Tampilkan Pasien Berdasarkan Urutan Umur\n");
                        printf("3. Tampilkan Statistik Umur\n");
                        printf("4. Tampilkan Statistik Gender\n");
                        printf("5. Tampilkan Statistik Penyakit\n");
                        printf("6. Tampilkan Statistik Penyakit Berdasarkan Kelompok Umur\n"); // New option added
                        printf("7. Tampilkan Statistik Penyakit Berdasarkan Gender\n");
                        printf("8. Kembali ke Menu Utama\n");
                        printf("Choose an option: ");
                        scanf("%d", &statChoice);
                        getchar();

                        switch (statChoice) {
                            case 1:
                                sortServedPatientsByName();
                                printPatients();
                                break;
                            case 2:
                                sortPatientsByAge();
                                printPatients();
                                break;
                            case 3:
                                countPatientsByAgeGroup();
                                break;
                            case 4:
                                countPatientsByGender();
                                break;
                            case 5:
                                countPatientsByDisease();
                                break;
                            case 6:
                                displayDiseaseStatistics(); // Call the new function
                                break;
                            case 7:
                                displayDiseaseStatisticsByGender(); // Call the new function
                                break;
                            case 8:
                                printf("Kembali ke Menu Utama.\n");
                                goto mainMenu;

                            default:
                                printf("Pilihan tidak valid. Silakan coba lagi.\n");
                        }
                    }
                }
                mainMenu:
                    break;
                case 9:
                    printf("Logout Successful!\n");
                    return;
                case 10:
                    printServedPatients();
                    break;
                default:
                    printf("Pilihan tidak valid. Silakan coba lagi.\n");
            }
        }
    } else {
        printf("Username atau password salah.\n");
    }
}

int main() {
    int choice;
    patientQueueCounter = countPatients() + 1;

    // Pasien pertama
    struct Patient *manualPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(manualPatient->name, "Abil");
    manualPatient->age = 19;
    strcpy(manualPatient->gender, "Pria");
    strcpy(manualPatient->bpjs_number, "999");
    strcpy(manualPatient->address, "Kampung Bulang");
    strcpy(manualPatient->disease, "Gigi");
    manualPatient->queue_number = 1;
    manualPatient->next = NULL;

    // Pasien kedua
    struct Patient *secondPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(secondPatient->name, "Rafi");
    secondPatient->age = 25;
    strcpy(secondPatient->gender, "Pria");
    strcpy(secondPatient->bpjs_number, "888");
    strcpy(secondPatient->address, "Kota Baru");
    strcpy(secondPatient->disease, "Jantung");
    secondPatient->queue_number = 2;
    secondPatient->next = NULL;
    manualPatient->next = secondPatient;

    // Pasien ketiga
    struct Patient *thirdPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(thirdPatient->name, "Dinda");
    thirdPatient->age = 30;
    strcpy(thirdPatient->gender, "Wanita");
    strcpy(thirdPatient->bpjs_number, "777");
    strcpy(thirdPatient->address, "Jakarta");
    strcpy(thirdPatient->disease, "Kulit");
    thirdPatient->queue_number = 3;
    thirdPatient->next = NULL;
    secondPatient->next = thirdPatient;

    // Pasien keempat
    struct Patient *fourthPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(fourthPatient->name, "Rizky");
    fourthPatient->age = 22;
    strcpy(fourthPatient->gender, "Pria");
    strcpy(fourthPatient->bpjs_number, "666");
    strcpy(fourthPatient->address, "Bandung");
    strcpy(fourthPatient->disease, "Mata");
    fourthPatient->queue_number = 4;
    fourthPatient->next = NULL;
    thirdPatient->next = fourthPatient;

    // Pasien kelima
    struct Patient *fifthPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(fifthPatient->name, "Fadhil");
    fifthPatient->age = 45;
    strcpy(fifthPatient->gender, "Pria");
    strcpy(fifthPatient->bpjs_number, "555");
    strcpy(fifthPatient->address, "Surabaya");
    strcpy(fifthPatient->disease, "Gizi");
    fifthPatient->queue_number = 5;
    fifthPatient->next = NULL;
    fourthPatient->next = fifthPatient;

    // Pasien keenam
    struct Patient *sixthPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(sixthPatient->name, "Aisyah");
    sixthPatient->age = 35;
    strcpy(sixthPatient->gender, "Wanita");
    strcpy(sixthPatient->bpjs_number, "444");
    strcpy(sixthPatient->address, "Yogyakarta");
    strcpy(sixthPatient->disease, "Kandungan");
    sixthPatient->queue_number = 6;
    sixthPatient->next = NULL;
    fifthPatient->next = sixthPatient;

    // Pasien ketujuh
    struct Patient *seventhPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(seventhPatient->name, "Naufal");
    seventhPatient->age = 28;
    strcpy(seventhPatient->gender, "Pria");
    strcpy(seventhPatient->bpjs_number, "333");
    strcpy(seventhPatient->address, "Medan");
    strcpy(seventhPatient->disease, "Anak");
    seventhPatient->queue_number = 7;
    seventhPatient->next = NULL;
    sixthPatient->next = seventhPatient;

    // Pasien kedelapan
    struct Patient *eighthPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(eighthPatient->name, "Citra");
    eighthPatient->age = 40;
    strcpy(eighthPatient->gender, "Wanita");
    strcpy(eighthPatient->bpjs_number, "222");
    strcpy(eighthPatient->address, "Palembang");
    strcpy(eighthPatient->disease, "Bedah");
    eighthPatient->queue_number = 8;
    eighthPatient->next = NULL;
    seventhPatient->next = eighthPatient;

    // Pasien kesembilan
    struct Patient *ninthPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(ninthPatient->name, "Yusuf");
    ninthPatient->age = 50;
    strcpy(ninthPatient->gender, "Pria");
    strcpy(ninthPatient->bpjs_number, "111");
    strcpy(ninthPatient->address, "Makassar");
    strcpy(ninthPatient->disease, "Ortopedi");
    ninthPatient->queue_number = 9;
    ninthPatient->next = NULL;
    eighthPatient->next = ninthPatient;

    // Pasien kesepuluh
    struct Patient *tenthPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(tenthPatient->name, "Nabila");
    tenthPatient->age = 27;
    strcpy(tenthPatient->gender, "Wanita");
    strcpy(tenthPatient->bpjs_number, "110");
    strcpy(tenthPatient->address, "Semarang");
    strcpy(tenthPatient->disease, "Jantung");
    tenthPatient->queue_number = 10;
    tenthPatient->next = NULL;
    ninthPatient->next = tenthPatient;

    struct Patient *current = tenthPatient;

    // Pasien ke-11
    struct Patient *eleventhPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(eleventhPatient->name, "Andre");
    eleventhPatient->age = 34;
    strcpy(eleventhPatient->gender, "Pria");
    strcpy(eleventhPatient->bpjs_number, "109");
    strcpy(eleventhPatient->address, "Malang");
    strcpy(eleventhPatient->disease, "Kulit");
    eleventhPatient->queue_number = 11;
    eleventhPatient->next = NULL;
    current->next = eleventhPatient;
    current = eleventhPatient;

    // Pasien ke-12
    struct Patient *twelfthPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(twelfthPatient->name, "Tania");
    twelfthPatient->age = 24;
    strcpy(twelfthPatient->gender, "Wanita");
    strcpy(twelfthPatient->bpjs_number, "108");
    strcpy(twelfthPatient->address, "Padang");
    strcpy(twelfthPatient->disease, "Mata");
    twelfthPatient->queue_number = 12;
    twelfthPatient->next = NULL;
    current->next = twelfthPatient;
    current = twelfthPatient;

    // Pasien ke-13
    struct Patient *thirteenthPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(thirteenthPatient->name, "Dewi");
    thirteenthPatient->age = 32;
    strcpy(thirteenthPatient->gender, "Wanita");
    strcpy(thirteenthPatient->bpjs_number, "107");
    strcpy(thirteenthPatient->address, "Bali");
    strcpy(thirteenthPatient->disease, "Gizi");
    thirteenthPatient->queue_number = 13;
    thirteenthPatient->next = NULL;
    current->next = thirteenthPatient;
    current = thirteenthPatient;

    // Pasien ke-14
    struct Patient *fourteenthPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(fourteenthPatient->name, "Arif");
    fourteenthPatient->age = 29;
    strcpy(fourteenthPatient->gender, "Pria");
    strcpy(fourteenthPatient->bpjs_number, "106");
    strcpy(fourteenthPatient->address, "Pontianak");
    strcpy(fourteenthPatient->disease, "Kandungan");
    fourteenthPatient->queue_number = 14;
    fourteenthPatient->next = NULL;
    current->next = fourteenthPatient;
    current = fourteenthPatient;

    // Pasien ke-15
    struct Patient *fifteenthPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(fifteenthPatient->name, "Farah");
    fifteenthPatient->age = 36;
    strcpy(fifteenthPatient->gender, "Wanita");
    strcpy(fifteenthPatient->bpjs_number, "105");
    strcpy(fifteenthPatient->address, "Manado");
    strcpy(fifteenthPatient->disease, "Anak");
    fifteenthPatient->queue_number = 15;
    fifteenthPatient->next = NULL;
    current->next = fifteenthPatient;
    current = fifteenthPatient;

    // Pasien ke-16
    struct Patient *sixteenthPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(sixteenthPatient->name, "Gilang");
    sixteenthPatient->age = 38;
    strcpy(sixteenthPatient->gender, "Pria");
    strcpy(sixteenthPatient->bpjs_number, "104");
    strcpy(sixteenthPatient->address, "Palangkaraya");
    strcpy(sixteenthPatient->disease, "Bedah");
    sixteenthPatient->queue_number = 16;
    sixteenthPatient->next = NULL;
    fifteenthPatient->next = sixteenthPatient;

    // Pasien ke-17
    struct Patient *seventeenthPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(seventeenthPatient->name, "Dian");
    seventeenthPatient->age = 26;
    strcpy(seventeenthPatient->gender, "Wanita");
    strcpy(seventeenthPatient->bpjs_number, "103");
    strcpy(seventeenthPatient->address, "Ambon");
    strcpy(seventeenthPatient->disease, "Ortopedi");
    seventeenthPatient->queue_number = 17;
    seventeenthPatient->next = NULL;
    sixteenthPatient->next = seventeenthPatient;

    // Pasien ke-18
    struct Patient *eighteenthPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(eighteenthPatient->name, "Kirana");
    eighteenthPatient->age = 29;
    strcpy(eighteenthPatient->gender, "Wanita");
    strcpy(eighteenthPatient->bpjs_number, "102");
    strcpy(eighteenthPatient->address, "Kendari");
    strcpy(eighteenthPatient->disease, "Jantung");
    eighteenthPatient->queue_number = 18;
    eighteenthPatient->next = NULL;
    seventeenthPatient->next = eighteenthPatient;

    // Pasien ke-19
    struct Patient *nineteenthPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(nineteenthPatient->name, "Hasan");
    nineteenthPatient->age = 41;
    strcpy(nineteenthPatient->gender, "Pria");
    strcpy(nineteenthPatient->bpjs_number, "101");
    strcpy(nineteenthPatient->address, "Palu");
    strcpy(nineteenthPatient->disease, "Kulit");
    nineteenthPatient->queue_number = 19;
    nineteenthPatient->next = NULL;
    eighteenthPatient->next = nineteenthPatient;

    // Pasien ke-20
    struct Patient *twentiethPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(twentiethPatient->name, "Budi");
    twentiethPatient->age = 33;
    strcpy(twentiethPatient->gender, "Pria");
    strcpy(twentiethPatient->bpjs_number, "100");
    strcpy(twentiethPatient->address, "Ternate");
    strcpy(twentiethPatient->disease, "Mata");
    twentiethPatient->queue_number = 20;
    twentiethPatient->next = NULL;
    nineteenthPatient->next = twentiethPatient;

    // Pasien ke-21
    struct Patient *twentyFirstPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(twentyFirstPatient->name, "Aulia");
    twentyFirstPatient->age = 25;
    strcpy(twentyFirstPatient->gender, "Wanita");
    strcpy(twentyFirstPatient->bpjs_number, "099");
    strcpy(twentyFirstPatient->address, "Sorong");
    strcpy(twentyFirstPatient->disease, "Gigi");
    twentyFirstPatient->queue_number = 21;
    twentyFirstPatient->next = NULL;
    twentiethPatient->next = twentyFirstPatient;

    // Pasien ke-22
    struct Patient *twentySecondPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(twentySecondPatient->name, "Siti");
    twentySecondPatient->age = 30;
    strcpy(twentySecondPatient->gender, "Wanita");
    strcpy(twentySecondPatient->bpjs_number, "098");
    strcpy(twentySecondPatient->address, "Bitung");
    strcpy(twentySecondPatient->disease, "Mata");
    twentySecondPatient->queue_number = 22;
    twentySecondPatient->next = NULL;
    twentyFirstPatient->next = twentySecondPatient;

    // Pasien ke-23
    struct Patient *twentyThirdPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(twentyThirdPatient->name, "Hendra");
    twentyThirdPatient->age = 27;
    strcpy(twentyThirdPatient->gender, "Pria");
    strcpy(twentyThirdPatient->bpjs_number, "097");
    strcpy(twentyThirdPatient->address, "Tegal");
    strcpy(twentyThirdPatient->disease, "Kulit");
    twentyThirdPatient->queue_number = 23;
    twentyThirdPatient->next = NULL;
    twentySecondPatient->next = twentyThirdPatient;

    // Pasien ke-24
    struct Patient *twentyFourthPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(twentyFourthPatient->name, "Anisa");
    twentyFourthPatient->age = 28;
    strcpy(twentyFourthPatient->gender, "Wanita");
    strcpy(twentyFourthPatient->bpjs_number, "096");
    strcpy(twentyFourthPatient->address, "Cilacap");
    strcpy(twentyFourthPatient->disease, "Jantung");
    twentyFourthPatient->queue_number = 24;
    twentyFourthPatient->next = NULL;
    twentyThirdPatient->next = twentyFourthPatient;

    // Pasien ke-25
    struct Patient *twentyFifthPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(twentyFifthPatient->name, "Ahmad");
    twentyFifthPatient->age = 35;
    strcpy(twentyFifthPatient->gender, "Pria");
    strcpy(twentyFifthPatient->bpjs_number, "095");
    strcpy(twentyFifthPatient->address, "Purwokerto");
    strcpy(twentyFifthPatient->disease, "Bedah");
    twentyFifthPatient->queue_number = 25;
    twentyFifthPatient->next = NULL;
    twentyFourthPatient->next = twentyFifthPatient;

    // Pasien ke-26
    struct Patient *twentySixthPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(twentySixthPatient->name, "Indah");
    twentySixthPatient->age = 33;
    strcpy(twentySixthPatient->gender, "Wanita");
    strcpy(twentySixthPatient->bpjs_number, "094");
    strcpy(twentySixthPatient->address, "Tasikmalaya");
    strcpy(twentySixthPatient->disease, "Gizi");
    twentySixthPatient->queue_number = 26;
    twentySixthPatient->next = NULL;
    twentyFifthPatient->next = twentySixthPatient;

    // Pasien ke-27
    struct Patient *twentySeventhPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(twentySeventhPatient->name, "Rina");
    twentySeventhPatient->age = 31;
    strcpy(twentySeventhPatient->gender, "Wanita");
    strcpy(twentySeventhPatient->bpjs_number, "093");
    strcpy(twentySeventhPatient->address, "Cirebon");
    strcpy(twentySeventhPatient->disease, "Anak");
    twentySeventhPatient->queue_number = 27;
    twentySeventhPatient->next = NULL;
    twentySixthPatient->next = twentySeventhPatient;

    // Pasien ke-28
    struct Patient *twentyEighthPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(twentyEighthPatient->name, "Sari");
    twentyEighthPatient->age = 29;
    strcpy(twentyEighthPatient->gender, "Wanita");
    strcpy(twentyEighthPatient->bpjs_number, "092");
    strcpy(twentyEighthPatient->address, "Banjarmasin");
    strcpy(twentyEighthPatient->disease, "Gigi");
    twentyEighthPatient->queue_number = 28;
    twentyEighthPatient->next = NULL;
    twentySeventhPatient->next = twentyEighthPatient;

    // Pasien ke-29
    struct Patient *twentyNinthPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(twentyNinthPatient->name, "Dewi");
    twentyNinthPatient->age = 34;
    strcpy(twentyNinthPatient->gender, "Wanita");
    strcpy(twentyNinthPatient->bpjs_number, "091");
    strcpy(twentyNinthPatient->address, "Jambi");
    strcpy(twentyNinthPatient->disease, "Jantung");
    twentyNinthPatient->queue_number = 29;
    twentyNinthPatient->next = NULL;
    twentyEighthPatient->next = twentyNinthPatient;

    // Pasien ke-30
    struct Patient *thirtiethPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(thirtiethPatient->name, "Rudi");
    thirtiethPatient->age = 37;
    strcpy(thirtiethPatient->gender, "Pria");
    strcpy(thirtiethPatient->bpjs_number, "090");
    strcpy(thirtiethPatient->address, "Bali");
    strcpy(thirtiethPatient->disease, "Kulit");
    thirtiethPatient->queue_number = 30;
    thirtiethPatient->next = NULL;
    twentyNinthPatient->next = thirtiethPatient;

    // Pasien ke-31
    struct Patient *thirtyFirstPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(thirtyFirstPatient->name, "Lina");
    thirtyFirstPatient->age = 26;
    strcpy(thirtyFirstPatient->gender, "Wanita");
    strcpy(thirtyFirstPatient->bpjs_number, "089");
    strcpy(thirtyFirstPatient->address, "Makassar");
    strcpy(thirtyFirstPatient->disease, "Mata");
    thirtyFirstPatient->queue_number = 31;
    thirtyFirstPatient->next = NULL;
    thirtiethPatient->next = thirtyFirstPatient;

    // Pasien ke-32
    struct Patient *thirtySecondPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(thirtySecondPatient->name, "Eko");
    thirtySecondPatient->age = 40;
    strcpy(thirtySecondPatient->gender, "Pria");
    strcpy(thirtySecondPatient->bpjs_number, "088");
    strcpy(thirtySecondPatient->address, "Surabaya");
    strcpy(thirtySecondPatient->disease, "Anak");
    thirtySecondPatient->queue_number = 32;
    thirtySecondPatient->next = NULL;
    thirtyFirstPatient->next = thirtySecondPatient;

    // Pasien ke-33
    struct Patient *thirtyThirdPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(thirtyThirdPatient->name, "Tina");
    thirtyThirdPatient->age = 22;
    strcpy(thirtyThirdPatient->gender, "Wanita");
    strcpy(thirtyThirdPatient->bpjs_number, "087");
    strcpy(thirtyThirdPatient->address, "Bandung");
    strcpy(thirtyThirdPatient->disease, "Gizi");
    thirtyThirdPatient->queue_number = 33;
    thirtyThirdPatient->next = NULL;
    thirtySecondPatient->next = thirtyThirdPatient;

    // Pasien ke-34
    struct Patient *thirtyFourthPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(thirtyFourthPatient->name, "Fajar");
    thirtyFourthPatient->age = 39;
    strcpy(thirtyFourthPatient->gender, "Pria");
    strcpy(thirtyFourthPatient->bpjs_number, "086");
    strcpy(thirtyFourthPatient->address, "Yogyakarta");
    strcpy(thirtyFourthPatient->disease, "Bedah");
    thirtyFourthPatient->queue_number = 34;
    thirtyFourthPatient->next = NULL;
    thirtyThirdPatient->next = thirtyFourthPatient;

    // Pasien ke-35
    struct Patient *thirtyFifthPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(thirtyFifthPatient->name, "Rina");
    thirtyFifthPatient->age = 30;
    strcpy(thirtyFifthPatient->gender, "Wanita");
    strcpy(thirtyFifthPatient->bpjs_number, "085");
    strcpy(thirtyFifthPatient->address, "Palembang");
    strcpy(thirtyFifthPatient->disease, "Ortopedi");
    thirtyFifthPatient->queue_number = 35;
    thirtyFifthPatient->next = NULL;
    thirtyFourthPatient->next = thirtyFifthPatient;

    // Pasien ke-36
    struct Patient *thirtySixthPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(thirtySixthPatient->name, "Siti");
    thirtySixthPatient->age = 27;
    strcpy(thirtySixthPatient->gender, "Wanita");
    strcpy(thirtySixthPatient->bpjs_number, "084");
    strcpy(thirtySixthPatient->address, "Medan");
    strcpy(thirtySixthPatient->disease, "Jantung");
    thirtySixthPatient->queue_number = 36;
    thirtySixthPatient->next = NULL;
    thirtyFifthPatient->next = thirtySixthPatient;

    // Pasien ke-37
    struct Patient *thirtySeventhPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(thirtySeventhPatient->name, "Budi");
    thirtySeventhPatient->age = 32;
    strcpy(thirtySeventhPatient->gender, "Pria");
    strcpy(thirtySeventhPatient->bpjs_number, "083");
    strcpy(thirtySeventhPatient->address, "Jakarta");
    strcpy(thirtySeventhPatient->disease, "Kulit");
    thirtySeventhPatient->queue_number = 37;
    thirtySeventhPatient->next = NULL;
    thirtySixthPatient->next = thirtySeventhPatient;

    // Pasien ke-38
    struct Patient *thirtyEighthPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(thirtyEighthPatient->name, "Diana");
    thirtyEighthPatient->age = 29;
    strcpy(thirtyEighthPatient->gender, "Wanita");
    strcpy(thirtyEighthPatient->bpjs_number, "082");
    strcpy(thirtyEighthPatient->address, "Bandung");
    strcpy(thirtyEighthPatient->disease, "Anak");
    thirtyEighthPatient->queue_number = 38;
    thirtyEighthPatient->next = NULL;
    thirtySeventhPatient->next = thirtyEighthPatient;

    // Pasien ke-39
    struct Patient *thirtyNinthPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(thirtyNinthPatient->name, "Andi");
    thirtyNinthPatient->age = 34;
    strcpy(thirtyNinthPatient->gender, "Pria");
    strcpy(thirtyNinthPatient->bpjs_number, "081");
    strcpy(thirtyNinthPatient->address, "Semarang");
    strcpy(thirtyNinthPatient->disease, "Gizi");
    thirtyNinthPatient->queue_number = 39;
    thirtyNinthPatient->next = NULL;
    thirtyEighthPatient->next = thirtyNinthPatient;

    // Pasien ke-40
    struct Patient *fortiethPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(fortiethPatient->name, "Rizki");
    fortiethPatient->age = 36;
    strcpy(fortiethPatient->gender, "Pria");
    strcpy(fortiethPatient->bpjs_number, "080");
    strcpy(fortiethPatient->address, "Yogyakarta");
    strcpy(fortiethPatient->disease, "Mata");
    fortiethPatient->queue_number = 40;
    fortiethPatient->next = NULL;
    thirtyNinthPatient->next = fortiethPatient;

    // Pasien ke-41
    struct Patient *fortyFirstPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(fortyFirstPatient->name, "Nina");
    fortyFirstPatient->age = 31;
    strcpy(fortyFirstPatient->gender, "Wanita");
    strcpy(fortyFirstPatient->bpjs_number, "079");
    strcpy(fortyFirstPatient->address, "Bali");
    strcpy(fortyFirstPatient->disease, "Bedah");
    fortyFirstPatient->queue_number = 41;
    fortyFirstPatient->next = NULL;
    fortiethPatient->next = fortyFirstPatient;

    // Pasien ke-42
    struct Patient *fortySecondPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(fortySecondPatient->name, "Fahmi");
    fortySecondPatient->age = 28;
    strcpy(fortySecondPatient->gender, "Pria");
    strcpy(fortySecondPatient->bpjs_number, "078");
    strcpy(fortySecondPatient->address, "Surabaya");
    strcpy(fortySecondPatient->disease, "Anak");
    fortySecondPatient->queue_number = 42;
    fortySecondPatient->next = NULL;
    fortyFirstPatient->next = fortySecondPatient;

    // Pasien ke-43
    struct Patient *fortyThirdPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(fortyThirdPatient->name, "Rina");
    fortyThirdPatient->age = 35;
    strcpy(fortyThirdPatient->gender, "Wanita");
    strcpy(fortyThirdPatient->bpjs_number, "077");
    strcpy(fortyThirdPatient->address, "Medan");
    strcpy(fortyThirdPatient->disease, "Jantung");
    fortyThirdPatient->queue_number = 43;
    fortyThirdPatient->next = NULL;
    fortySecondPatient->next = fortyThirdPatient;

    // Pasien ke-44
    struct Patient *fortyFourthPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(fortyFourthPatient->name, "Siti");
    fortyFourthPatient->age = 29;
    strcpy(fortyFourthPatient->gender, "Wanita");
    strcpy(fortyFourthPatient->bpjs_number, "076");
    strcpy(fortyFourthPatient->address, "Jakarta");
    strcpy(fortyFourthPatient->disease, "Kulit");
    fortyFourthPatient->queue_number = 44;
    fortyFourthPatient->next = NULL;
    fortyThirdPatient->next = fortyFourthPatient;

    // Pasien ke-45
    struct Patient *fortyFifthPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(fortyFifthPatient->name, "Budi");
    fortyFifthPatient->age = 33;
    strcpy(fortyFifthPatient->gender, "Pria");
    strcpy(fortyFifthPatient->bpjs_number, "075");
    strcpy(fortyFifthPatient->address, "Bali");
    strcpy(fortyFifthPatient->disease, "Anak");
    fortyFifthPatient->queue_number = 45;
    fortyFifthPatient->next = NULL;
    fortyFourthPatient->next = fortyFifthPatient;

    // Pasien ke-46
    struct Patient *fortySixthPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(fortySixthPatient->name, "Dewi");
    fortySixthPatient->age = 30;
    strcpy(fortySixthPatient->gender, "Wanita");
    strcpy(fortySixthPatient->bpjs_number, "074");
    strcpy(fortySixthPatient->address, "Surabaya");
    strcpy(fortySixthPatient->disease, "Gizi");
    fortySixthPatient->queue_number = 46;
    fortySixthPatient->next = NULL;
    fortyFifthPatient->next = fortySixthPatient;

    // Pasien ke-47
    struct Patient *fortySeventhPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(fortySeventhPatient->name, "Rizki");
    fortySeventhPatient->age = 32;
    strcpy(fortySeventhPatient->gender, "Pria");
    strcpy(fortySeventhPatient->bpjs_number, "073");
    strcpy(fortySeventhPatient->address, "Bandung");
    strcpy(fortySeventhPatient->disease, "Mata");
    fortySeventhPatient->queue_number = 47;
    fortySeventhPatient->next = NULL;
    fortySixthPatient->next = fortySeventhPatient;

    // Pasien ke-48
    struct Patient *fortyEighthPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(fortyEighthPatient->name, "Tina");
    fortyEighthPatient->age = 28;
    strcpy(fortyEighthPatient->gender, "Wanita");
    strcpy(fortyEighthPatient->bpjs_number, "072");
    strcpy(fortyEighthPatient->address, "Yogyakarta");
    strcpy(fortyEighthPatient->disease, "Anak");
    fortyEighthPatient->queue_number = 48;
    fortyEighthPatient->next = NULL;
    fortySeventhPatient->next = fortyEighthPatient;

    // Pasien ke-49
    struct Patient *fortyNinthPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(fortyNinthPatient->name, "Andi");
    fortyNinthPatient->age = 36;
    strcpy(fortyNinthPatient->gender, "Pria");
    strcpy(fortyNinthPatient->bpjs_number, "071");
    strcpy(fortyNinthPatient->address, "Palembang");
    strcpy(fortyNinthPatient->disease, "Jantung");
    fortyNinthPatient->queue_number = 49;
    fortyNinthPatient->next = NULL;
    fortyEighthPatient->next = fortyNinthPatient;

    // Pasien ke-50
    struct Patient *fiftiethPatient = (struct Patient *)malloc(sizeof(struct Patient));
    strcpy(fiftiethPatient->name, "Siti");
    fiftiethPatient->age = 29;
    strcpy(fiftiethPatient->gender, "Wanita");
    strcpy(fiftiethPatient->bpjs_number, "070");
    strcpy(fiftiethPatient->address, "Jakarta");
    strcpy(fiftiethPatient->disease, "Gizi");
    fiftiethPatient->queue_number = 50;
    fiftiethPatient->next = NULL;
    fortyNinthPatient->next = fiftiethPatient;


    if (patientHead == NULL) {
        patientHead = manualPatient;
    } else {
        struct Patient *temp = patientHead;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = manualPatient;
    }

while (1) {
        printf("\n=== Welcome to Hospital Management System ===\n");
        printf("1. Register as Patient\n");
        printf("2. Login as Servant\n");
        printf("3. Login as Cashier\n");
        printf("4. Exit\n");
        printf("5. Give Feedback\n");
        printf("6. Panduan Daftar Dokter dan Deskripsinya\n"); // New option added
        printf("Choose an option: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                addPatient();
                break;
            case 2:
                loginAsServant();
                break;
            case 3:
                loginAsCashier();
                break;
            case 4:
                printf("Exiting the program. Goodbye!\n");
                exit(0);
            case 5:
                giveFeedback();
                break;
            case 6:
                printf("1. Dokter spesialis gigi adalah dokter gigi yang telah menempuh pendidikan lanjutan dalam bidang tertentu dari kedokteran gigi. Mereka memiliki keahlian mendalam untuk menangani kasus-kasus yang lebih kompleks dibandingkan dengan dokter gigi umum. Setiap spesialisasi memiliki fokus dan tugas khusus yang berkaitan dengan perawatan dan kesehatan gigi, mulut, dan jaringan pendukungnya.\n2. Dokter spesialis ortopedi adalah dokter yang memiliki keahlian khusus dalam menangani gangguan pada sistem muskuloskeletal, yaitu sistem tubuh yang meliputi tulang, sendi, otot, ligamen, tendon, saraf, dan jaringan penghubung lainnya. Mereka berperan dalam diagnosis, pengobatan, pencegahan, dan rehabilitasi cedera atau penyakit yang memengaruhi pergerakan tubuh.\n3. Dokter spesialis imunologi adalah dokter yang memiliki keahlian dalam mendiagnosis, merawat, dan menangani gangguan yang berkaitan dengan sistem imun. Sistem imun adalah pertahanan tubuh yang melindungi dari infeksi, penyakit, dan zat asing. Dokter spesialis imunologi menangani berbagai kondisi yang disebabkan oleh gangguan fungsi imun, baik yang terlalu aktif, terlalu lemah, atau menyerang tubuh sendiri (autoimun).\n4. Dokter spesialis jantung, atau dikenal juga sebagai kardiolog, adalah dokter yang memiliki keahlian khusus dalam mendiagnosis, merawat, dan menangani penyakit serta gangguan yang berkaitan dengan jantung dan pembuluh darah (sistem kardiovaskular). Mereka menjalani pelatihan tambahan setelah pendidikan kedokteran umum untuk menguasai bidang kardiologi.\n5. Dokter spesialis kandungan, atau dikenal sebagai dokter spesialis obstetri dan ginekologi (obgyn), adalah dokter yang menangani kesehatan reproduksi wanita, termasuk kehamilan, persalinan, serta penyakit dan kondisi yang memengaruhi organ reproduksi. Dokter ini memiliki dua fokus utama: obstetri (kehamilan dan persalinan) dan ginekologi (kesehatan reproduksi wanita secara umum).\n6. Dokter spesialis bedah adalah dokter yang memiliki keahlian khusus dalam diagnosis dan penanganan penyakit, cedera, atau kondisi kesehatan tertentu melalui tindakan pembedahan. Mereka terlatih untuk melakukan prosedur bedah di berbagai bagian tubuh guna memperbaiki, mengangkat, atau mengganti jaringan yang rusak, serta untuk menyelamatkan nyawa atau meningkatkan kualitas hidup pasien.\n7. Dokter spesialis anak, atau dikenal sebagai pediatri, adalah dokter yang memiliki keahlian dalam menangani kesehatan bayi, anak-anak, dan remaja hingga usia 18 tahun (atau sesuai dengan batas usia di negara masing-masing). Mereka berfokus pada diagnosis, pengobatan, dan pencegahan penyakit serta memastikan tumbuh kembang anak berjalan optimal.\n8. Dokter spesialis mata, atau dikenal sebagai oftalmologis, adalah dokter yang memiliki keahlian khusus dalam mendiagnosis, mengobati, dan menangani penyakit serta gangguan pada mata. Mereka menangani berbagai masalah mata, mulai dari gangguan penglihatan ringan hingga penyakit serius yang memerlukan pembedahan.\n9. Dokter spesialis kulit, atau dikenal sebagai dermatologis, adalah dokter yang memiliki keahlian dalam diagnosis, pengobatan, dan pencegahan penyakit kulit serta gangguan yang mempengaruhi rambut, kuku, dan membran mukosa. Spesialis kulit menangani berbagai masalah mulai dari kondisi kulit ringan hingga penyakit kulit yang lebih serius.\n10. Dokter spesialis gizi, atau dikenal sebagai spesialis gizi klinik, adalah dokter yang memiliki keahlian dalam mendiagnosis, mengobati, dan menangani gangguan yang berkaitan dengan asupan makanan dan nutrisi. Mereka bekerja untuk membantu pasien mencapai keseimbangan gizi yang optimal dan menangani berbagai kondisi yang berhubungan dengan pola makan, baik itu kekurangan atau kelebihan gizi, serta penyakit yang berhubungan dengan diet.");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}
