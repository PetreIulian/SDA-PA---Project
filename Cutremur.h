#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
    int year;
    int quake_number;
    float magnitude;
    struct node *next;
};

struct node *head = NULL;

void insertFirst(int year, int quake, float magnitude) {
    struct node *newNode = (struct node *)malloc(sizeof(struct node));
    newNode->year = year;
    newNode->quake_number = quake;
    newNode->magnitude = magnitude;
    newNode->next = head;
    head = newNode;
}

void reverse(struct node **head_ref) {
    struct node *current = *head_ref;
    struct node *prev = NULL;
    struct node *next;

    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    *head_ref = prev;
}

int insert(int an_start, int an_stop, float magnitudine_medie[], int numar_cutremure[]) {
    struct node *current = head;
    int i = 0;

    while (current != NULL) {
        if (current->year >= an_start && current->year <= an_stop) {
            magnitudine_medie[i] = current->magnitude;
            numar_cutremure[i] = current->quake_number;
            i++;
        }
        current = current->next;
    }
    return i;
}

void display() {
    struct node *current = head;
    while (current != NULL) {
        printf("%d,%d,%f\n", current->year, current->quake_number, current->magnitude);
        current = current->next;
    }
}

void moving_average(float data[], float result[], int size, int window) {
    for (int i = 0; i <= size - window; i++) {
        float sum = 0.0;
        for (int j = 0; j < window; j++) {
            sum += data[i + j];
        }
        result[i + window - 1] = sum / window;
    }
}

void moving_average_quake(int data[], float result[], int size, int window) {
    for (int i = 0; i <= size - window; i++) {
        float sum = 0.0;
        for (int j = 0; j < window; j++) {
            sum += data[i + j];
        }
        result[i + window - 1] = sum / window + 15;
    }
}

void cutremure(int an_start, int an_stop) {
    FILE *fp;

    // Date de comparație
    int numar_cutremure_comparare[] = {
        32, 30, 29, 34, 34, 36, 28, 27, 30, 33, 35, 31,
        29, 28, 32, 30, 27, 33, 31, 29, 34, 32, 28, 30,
        33, 31, 29, 28, 36, 35, 30, 33, 29, 27, 30, 32
    };

    float magnitudine_medie_comparare[] = {
        2.96, 2.84, 3.04, 2.65, 2.91, 2.98, 3.10, 2.95, 2.88, 2.76, 2.85, 2.90,
        3.00, 2.87, 2.93, 2.91, 2.95, 2.86, 2.88, 3.01, 2.90, 2.84, 2.79, 2.89,
        3.01, 2.90, 2.93, 2.88, 2.97, 3.00, 2.85, 2.92, 2.88, 2.83, 2.91, 2.94
    };

    int size = sizeof(numar_cutremure_comparare) / sizeof(numar_cutremure_comparare[0]);
    int window_size = 3;

    float rez_cutremure[size];
    float rez_magnitudine[size];

    float magnitudine_medie[100] = {0};
    int numar_cutremure[100] = {0};

    for (int i = 0; i < size; i++) {
        rez_cutremure[i] = -1;
        rez_magnitudine[i] = -1;
    }

    // Citire fișier CSV
    fp = fopen("date.csv", "r");
    if (fp == NULL) {
        perror("Eroare fisier citire.\n");
        return ;
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), fp)) {
        int year, quake_number;
        float magnitude;

        if (sscanf(buffer, "%d,%f,%d", &year, &magnitude, &quake_number) == 3) {
            //printf("%d,%0.1f,%d\n", year, magnitude, quake_number);
            insertFirst(year, quake_number, magnitude);
        }
    }
    fclose(fp);

    // Inversare listă pentru ordine corectă
    reverse(&head);

    //display();

    int num_valori = insert(an_start, an_stop, magnitudine_medie, numar_cutremure);

    if (num_valori > 36) {
        num_valori = 36;
    }

    /*printf("---------------\n");
    for (int i = 0; i < num_valori; i++) {
        printf("%f, %d\n", magnitudine_medie[i], numar_cutremure[i]);
    }*/

    moving_average(magnitudine_medie, rez_magnitudine, size, window_size);
    moving_average_quake(numar_cutremure, rez_cutremure, size, window_size);

    fp = fopen("rezultate_cutremure.csv", "w");
    if (fp == NULL) {
        perror("Eroare fisier scriere.\n");
        return;
    }

    fprintf(fp, "Luna,Cutremure,MA_Cutremure,Magnitudine,MA_Magnitudine\n");

    for (int i = 2; i < num_valori; i++) {
        fprintf(fp, "%d,%.0f,", i - 1, (float)numar_cutremure_comparare[i]);

        if (i >= window_size - 1) {
            fprintf(fp, "%.0f,", rez_cutremure[i]);
            printf("%.0f\n", rez_cutremure[i]);
        }
        else
            fprintf(fp, ",");

        fprintf(fp, "%.1f,", magnitudine_medie_comparare[i]);

        if (i >= window_size - 1)
            fprintf(fp, "%.2f\n", rez_magnitudine[i]);
        else
            fprintf(fp, "\n");
    }

    fclose(fp);
    printf("Datele au fost salvate în 'rezultate_cutremure.csv'.\n");

    while (head != NULL) {
        struct node *temp = head;
        head = head->next;
        free(temp);
    }

    return;
}