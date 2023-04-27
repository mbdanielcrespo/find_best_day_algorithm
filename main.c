#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int date;
    int type; // 0 for start, 1 for end
    char *transport;
} Endpoint;

int compare(const void *a, const void *b) {
    Endpoint *e1 = (Endpoint *)a;
    Endpoint *e2 = (Endpoint *)b;

    if (e1->date != e2->date)
        return e1->date - e2->date;
    else
        return e1->type - e2->type;
}

int date_to_int(const char *date) {
    int day, month, year;
    sscanf(date, "%d.%d.%d", &day, &month, &year);
    return day + month * 30 + year * 365;
}

void int_to_date(int date, char *date_str) {
    int year = date / 365;
    int month = (date % 365) / 30;
    int day = (date % 365) % 30;
    sprintf(date_str, "%02d.%02d.%04d", day, month, year);
}

int compare_strings(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

void read_input_from_file(const char *filename, int *n, Endpoint **endpoints_ptr) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fscanf(file, "%d", n);
    // Alloc memory for the transport structure
    *endpoints_ptr = malloc(2 * (*n) * sizeof(Endpoint));
    char *start_date = malloc(11 * sizeof(char));
    char *end_date = malloc(11 * sizeof(char));

    for (int i = 0; i < *n; i++) 
    {
        // Alocate memory for the transport variable of Enpoints
        (*endpoints_ptr)[2 * i].transport = malloc(100 * sizeof(char));
        (*endpoints_ptr)[2 * i + 1].transport = malloc(100 * sizeof(char));

        // Read input
        fscanf(file, "%s %s %s", start_date, end_date, (*endpoints_ptr)[2 * i].transport);

        // Replace underscores with spaces in transport
        for (int j = 0; (*endpoints_ptr)[2 * i].transport[j]; j++) 
        {
            if ((*endpoints_ptr)[2 * i].transport[j] == '_') {
                (*endpoints_ptr)[2 * i].transport[j] = ' ';
            }
        }
        
        // Save data to Endpoint structure
        (*endpoints_ptr)[2 * i].date = date_to_int(start_date);
        (*endpoints_ptr)[2 * i].type = 0;
        (*endpoints_ptr)[2 * i + 1].date = date_to_int(end_date);
        (*endpoints_ptr)[2 * i + 1].type = 1;
        strcpy((*endpoints_ptr)[2 * i + 1].transport, (*endpoints_ptr)[2 * i].transport);
        //printf("Start date -> %d    End date -> %d    Transport -> %s\n", (*endpoints_ptr)[2 * i].date, (*endpoints_ptr)[2 * i + 1].date, (*endpoints_ptr)[2 * i].transport);
        printf("Start date -> %s    End date -> %s    Transport -> %s\n", start_date, end_date, (*endpoints_ptr)[2 * i].transport);
    }
    free(start_date);
    free(end_date);
    fclose(file);
}

int main() {
    // Uncomment the following lines if you want to read input from stdin
    /*
    scanf("%d", &n);
    for (i = 0; i < n; i++) {
        char start_date[11], end_date[11];
        scanf("%s %s %s", start_date, end_date, endpoints[2 * i].transport);

        endpoints[2 * i].date = date_to_int(start_date);
        endpoints[2 * i].type = 0;
        endpoints[2 * i + 1].date = date_to_int(end_date);
        endpoints[2 * i + 1].type = 1;
        strcpy(endpoints[2 * i + 1].transport, endpoints[2 * i].transport);
    }
    */

    int n, i;
    int count = 0, max_count = 0, max_date = -1;
    Endpoint *endpoints;

    read_input_from_file("input2.txt", &n, &endpoints);
    qsort(endpoints, 2 * n, sizeof(Endpoint), compare);

    for (int i = 0; i < 2 * n; i++) {
        if (endpoints[i].type == 0) {
            count++;
        } else {
            count--;
        }

        if (count > max_count) {
            max_count = count;
            max_date = endpoints[i].date;
        }
    }

    const char **overlapping_transports = (const char **)malloc(sizeof(char *) * max_count);
    int overlapping_count = 0;

    count = 0;
    for (int i = 0; i < 2 * n; i++) {
        if (endpoints[i].type == 0) {
            count++;
        } else {
            count--;
        }

        if (count == max_count && endpoints[i].date == max_date) {
            for (int j = i - (max_count - 1) * 2; j <= i; j += 2) {
                overlapping_transports[overlapping_count++] = endpoints[j].transport;
            }
            break;
        }
    }


    char best_day[11];
    int_to_date(max_date, best_day);
    printf("Best day: %s\n", best_day);

    qsort(overlapping_transports, overlapping_count, sizeof(char *), compare_strings);
    
    for (int i = 0; i < max_count; i++) {
        printf("Num -> %d  Transportes -> %s\n", i, overlapping_transports[i]);
    }
    for (int i = 0; i < 2 * n; i++) {
        free(endpoints[i].transport);
    }
    free(overlapping_transports);
    return 0;
}