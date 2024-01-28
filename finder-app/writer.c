// finder-app/writer.c

#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

int main(int argc, char *argv[]) {
    // Check the number of command-line arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <writefile> <writestr>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Extract command-line arguments
    char *writefile = argv[1];
    char *writestr = argv[2];

    // Open the file for writing
    FILE *file = fopen(writefile, "w");
    if (file == NULL) {
        // Log error using syslog and exit
        syslog(LOG_ERR, "Error creating/writing file %s", writefile);
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    // Write the string to the fileee
    fprintf(file, "%s\n", writestr);

    // Log the write operation using syslog
    syslog(LOG_DEBUG, "Writing '%s' to '%s'", writestr, writefile);

    // Close the file
    fclose(file);

    // Close syslog connection
    closelog();

    return 0;
}
