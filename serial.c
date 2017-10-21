// serial.c
// Writes information from a file serially to the selected COM port
//
// Written by Alvin Wong October 2017 for ELEC3117 project
//

#include <windows.h>
#include <time.h>
#include <stdio.h>

int main()
{
    // Declare variables and structures
    HANDLE hSerial;
    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts = {0};

    // Open serial port
    fprintf(stderr, "Opening serial port...");
    hSerial = CreateFile(
                "COM6", GENERIC_READ|GENERIC_WRITE, 0, NULL,
                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
    if (hSerial == INVALID_HANDLE_VALUE)
    {
            fprintf(stderr, "Error\n");
            return 1;
    }
    else fprintf(stderr, "OK\n");

    // Set device parameters (38400 baud, 1 start bit,
    // 1 stop bit, no parity)
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (GetCommState(hSerial, &dcbSerialParams) == 0)
    {
        fprintf(stderr, "Error getting device state\n");
        CloseHandle(hSerial);
        return 1;
    }

    dcbSerialParams.BaudRate = 9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    if(SetCommState(hSerial, &dcbSerialParams) == 0)
    {
        fprintf(stderr, "Error setting device parameters\n");
        CloseHandle(hSerial);
        return 1;
    }

    // Set COM port timeout settings
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    if(SetCommTimeouts(hSerial, &timeouts) == 0)
    {
        fprintf(stderr, "Error setting timeouts\n");
        CloseHandle(hSerial);
        return 1;
    }

    // Specify path of file
    char *path = "C:\\Users\\sv3nt\\AppData\\Local\\Packages\\IntelligentKioskSample_csqyw2evbp0ke\\LocalState\\test.txt";

    // Variable initialisations
    int prevSize = 0;
    int init = 0;
    int diff = 0;
    char *bytes_to_send;
    char *prevBytes;
    clock_t start, end;
    double elapsed = 0;
    start = clock();
    end = clock();
    while (1) {
        // Reads from the file every 10 seconds
        elapsed = (double)(end - start)/CLOCKS_PER_SEC;
        if (init = 0 || elapsed > 10) {
            start = clock();
            FILE *fp = fopen(path, "r");
            fseek(fp, 0L, SEEK_END);
            int size = ftell(fp);
            fseek(fp, 0L, SEEK_SET);
            // Allocate memory for bytes to send
            bytes_to_send = malloc(size*sizeof(char));
            fread(bytes_to_send, 1, size, fp);
            fclose(fp);

            // Checks if file has been modified
            diff = 0;
            if (init = 0 || prevSize != size) {
                diff = 1;
                printf("diff changed 1\n");
            } else {
                for (int i = 0; i < size; i++) {
                    if (prevBytes[i] != bytes_to_send[i]) {
                        diff = 1;
                        printf("diff changed 2\n");
                        break;
                    }
                }
            }
            // If file has been modified, send serial data
            if (diff == 1) {
                for (int i = 0; i < size; i++) {
                    fprintf(stderr, "Sending %d over\n", bytes_to_send[i]);
                }
                // Send specified text
                DWORD bytes_written, total_bytes_written = 0;
                fprintf(stderr, "Sending bytes...");
                if(!WriteFile(hSerial, bytes_to_send, size, &bytes_written, NULL))
                {
                    fprintf(stderr, "Error\n");
                    CloseHandle(hSerial);
                    return 1;
                }
                fprintf(stderr, "%d bytes written\n", bytes_written);

                // Store into previous array
                free(prevBytes);
                prevBytes = malloc(size*sizeof(char));
                for (int i = 0; i < size; i++) {
                    prevBytes[i] = bytes_to_send[i];
                }
                prevSize = size;
                diff = 0;
                init = 1;
            }
            free(bytes_to_send);
        }
        end = clock();
    }

    fprintf(stderr, "Closing serial port...");
    if (CloseHandle(hSerial) == 0)
    {
        fprintf(stderr, "Error\n");
        return 1;
    }
    fprintf(stderr, "OK\n");

    return 0;
}
