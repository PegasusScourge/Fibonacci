/**

Creates a fibonacci sequence in memory and then allows you to query it

*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <Windows.h>

#include "PeggyUtil.h"

#define CON_BUFFER_LEN 200

/* Function defs*/
void fib_displayMenu(void);
void fib_createSequence(unsigned long long *buf, int bufMaxLen, int toIndex);

/* Var defs */
unsigned long long *fibBuf = NULL;
int fibBufLen = 10;

int main()
{
	int fibSequenceLength = 10;

	// Allocate the Fibonacci buffer
	fibBuf = malloc(fibBufLen * sizeof(unsigned long long));

	if (fibBuf == NULL) {
		perror("Failed to get memory for the fibonacci sequence\n");
		abort();
	}

	fib_createSequence(fibBuf, fibBufLen, fibSequenceLength);

	// Allocate the buffer for reading from the console
	char conBuffer[CON_BUFFER_LEN];

	int charsGot = 0;
	int switchVal = 0;

	bool running = true;
	while (running) {
		fib_displayMenu();
		printf("> ");
		charsGot = getConsoleLine(conBuffer, CON_BUFFER_LEN);

		switchVal = atoi(conBuffer);

		switch (switchVal) {
		case 1:
			printf("Set the new Fibonacci sequence limit (index): ");
			charsGot = getConsoleLine(conBuffer, CON_BUFFER_LEN);

			int newLim = atoi(conBuffer);
			if (newLim <= 0) {
				printf("Can't define a limit of 0 or below!\n");
				continue; // Continue to next loop
			}
			else if (newLim > fibBufLen) {
				// We need to allocate more memory space
				unsigned long long* newBuf = realloc(fibBuf, newLim * sizeof(unsigned long long));
				if (newBuf == NULL) {
					// Memory allocation error occured
					perror("Failed to allocate more memory to fibonacci sequence list");
					abort();
				}

				// Transfer the allocated space
				fibBuf = newBuf;
				fibBufLen = newLim;
			}
			fibSequenceLength = newLim;
			fib_createSequence(fibBuf, fibBufLen, fibSequenceLength);
			break;
		case 2:
			printf("Index: ");
			charsGot = getConsoleLine(conBuffer, CON_BUFFER_LEN);

			int index = atoi(conBuffer);
			if (index < 0 || index > fibSequenceLength) {
				printf("Invalid index location: sequence is %i indexes long\n", fibSequenceLength);
				Sleep(2000);
				continue;
			}

			printf("Index %i has value of %llu\n", index, fibBuf[index]);
			Sleep(2000);
			break;
		case 3:
			printf("Output file name: ");
			charsGot = getConsoleLine(conBuffer, CON_BUFFER_LEN);

			printf("Writing to file %s\n", conBuffer);
			FILE* file = NULL;
			file = fopen(conBuffer, "w");

			if (file == NULL) {
				perror("Failed to open file");
				continue;
			}

			fprintf(file, "Fibonacci Sequence\n---------------------------------\n");
			for (int i = 0; i < fibSequenceLength; i++) {
				fprintf(file, "[%i] %llu\n", i, fibBuf[i]);
			}

			fclose(file);
			printf("File written!\n");
			Sleep(1000);

			break;
		case 4:
		default:
			running = false;
			break;
		}
	}

	printf("Exiting...\n");

	exit(EXIT_SUCCESS);
}

void fib_displayMenu(void) {

	system("cls");

	printf("###############################################\n");
	printf("# Fibonacci : Created by PegasusScourge       #\n");
	printf("###############################################\n\n");

	printf("Options:\n");
	printf(" - 1: Specify a Fibonacci sequence limit\n");
	printf(" - 2: Query the Fibonacci sequence at an index\n");
	printf(" - 3: Output the Fibonacci sequence to a file\n");
	printf(" - 4: Exit\n");
}

void fib_createSequence(unsigned long long *buf, int bufMaxLen, int toIndex) {

	if (toIndex < bufMaxLen) {
		printf("WARN: Creating sequence that wanted to get to index %i but the buffer is only %i indexes long!\n", toIndex, bufMaxLen);
	}

	printf("[0] 0 [1] 1 ");
	// Set the initial fibonacci numbers
	buf[0] = 0;
	buf[1] = 1;

	for (int i = 2; i < toIndex && i < bufMaxLen; i++) {
		buf[i] = buf[i - 1] + buf[i - 2];
		printf("[%i] %llu ", i, buf[i]);
	}
	printf("\n");

	printf("Created sequence\n");

	Sleep(1000);
}