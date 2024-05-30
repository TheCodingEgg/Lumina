
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 512
#define MAX_VARS 100

typedef struct {
    char name[MAX_LINE_LENGTH];
    char value[MAX_LINE_LENGTH];
} Variable;

Variable variables[MAX_VARS];
int var_count = 0;

void say(const char *message) {
    printf("%s", message);
}

void inputc(const char *var_name) {
    char value[MAX_LINE_LENGTH];
    printf("Please enter input for %s: ", var_name);
    if (fgets(value, sizeof(value), stdin) != NULL) {
        // Strip newline character from the end of the input
        value[strcspn(value, "\n")] = '\0';
        for (int i = 0; i < var_count; i++) {
            if (strcmp(variables[i].name, var_name) == 0) {
                strncpy(variables[i].value, value, MAX_LINE_LENGTH);
                variables[i].value[MAX_LINE_LENGTH - 1] = '\0'; // Ensure null-termination
                return;
            }
        }
        fprintf(stderr, "Variable %s not found.\n", var_name);
    } else {
        fprintf(stderr, "Error reading input.\n");
    }
}

void sayinput(const char *var_name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, var_name) == 0) {
            printf("%s", variables[i].value);
            return;
        }
    }
    fprintf(stderr, "Variable %s not found.\n", var_name);
}

void set_variable(const char *var_name) {
    if (var_count < MAX_VARS) {
        strncpy(variables[var_count].name, var_name, MAX_LINE_LENGTH);
        variables[var_count].name[MAX_LINE_LENGTH - 1] = '\0'; // Ensure null-termination
        variables[var_count].value[0] = '\0'; // Initialize the value as an empty string
        var_count++;
    } else {
        fprintf(stderr, "Maximum number of variables reached.\n");
    }
}

void num_variable(const char *var_name, int value) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, var_name) == 0) {
            snprintf(variables[i].value, MAX_LINE_LENGTH, "%d", value);
            return;
        }
    }
    if (var_count < MAX_VARS) {
        strncpy(variables[var_count].name, var_name, MAX_LINE_LENGTH);
        variables[var_count].name[MAX_LINE_LENGTH - 1] = '\0'; // Ensure null-termination
        snprintf(variables[var_count].value, MAX_LINE_LENGTH, "%d", value);
        var_count++;
    } else {
        fprintf(stderr, "Maximum number of variables reached.\n");
    }
}

void inputn(const char *var_name) {
    int value;
    printf("Please enter input for %s: ", var_name);
    if (scanf("%d", &value) == 1) {
        getchar(); // Consume the newline character left by scanf
        num_variable(var_name, value);
    } else {
        fprintf(stderr, "Error reading input.\n");
    }
}

void sayinputn(const char *var_name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, var_name) == 0) {
            int value = atoi(variables[i].value);
            printf("%d", value);
            return;
        }
    }
    fprintf(stderr, "Variable %s not found.\n", var_name);
}

void nextl() {
    printf("\n");
}

void check_and_execute_command(const char *line) {
    char message[MAX_LINE_LENGTH];
    char var_name[MAX_LINE_LENGTH];
    int var_amount;
    
    // Ignore comments
    if (strncmp(line, "//", 2) == 0) {
        return;
    }

    // Check if the line matches the pattern say("message");
    if (sscanf(line, "say(\"%[^\"]\");", message) == 1) {
        say(message);
    } else if (sscanf(line, "inputc(%[a-zA-Z0-9_]);", var_name) == 1) {
        inputc(var_name);
    } else if (sscanf(line, "sayinput(%[a-zA-Z0-9_]);", var_name) == 1) {
        sayinput(var_name);
    } else if (sscanf(line, "set(%[a-zA-Z0-9_]);", var_name) == 1) {
        set_variable(var_name);
    } else if (sscanf(line, "num(%[a-zA-Z0-9_], %d);", var_name, &var_amount) == 2) {
        num_variable(var_name, var_amount);
    } else if (sscanf(line, "inputn(%[a-zA-Z0-9_]);", var_name) == 1) {
        inputn(var_name);
    } else if (sscanf(line, "sayinputn(%[a-zA-Z0-9_]);", var_name) == 1) {
        sayinputn(var_name);
    } else if (strcmp(line, "nextl();") == 0) {
        nextl();
    } else {
        fprintf(stderr, "No valid command found in line: %s\n", line);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        // Strip newline character from the end of the line
        line[strcspn(line, "\n")] = '\0';
        check_and_execute_command(line);
    }

    fclose(file);
    return 0;
}



