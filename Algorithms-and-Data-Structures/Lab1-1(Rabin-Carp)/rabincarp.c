#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>


long long int hash(const unsigned char* str, int firstcharacterofstring, int lastchacterofstring) {
	long long int hash_value = 0;
	int tri = 3;
	int i = 0;
	for (int j = firstcharacterofstring; j <= lastchacterofstring; j++) {
		hash_value += (str[j] % tri) * pow(tri, i);
		i++;
	}
	return hash_value;
}

void RabinKarp(const unsigned char* string, const unsigned char* substring, int sumelement) {
	int lenstring = strlen((const char*)string) - 1;
	int lensubstring = strlen((const char*)substring) - 1;
	if (lenstring < lensubstring) {
		return;
	}
	long long int hashsubstring = hash((const unsigned char*)substring, 0, lensubstring);
	for (int startofstring = 0; startofstring <= lenstring - lensubstring; startofstring++) {
		int endofstring = startofstring + lensubstring;
		long long int hashstring = hash(string, startofstring, endofstring);
		if (hashstring == hashsubstring) {
			for (int firstcharactertocheck = 0; firstcharactertocheck <= lensubstring; firstcharactertocheck++) {
				printf("%d ", firstcharactertocheck + startofstring + 1 + sumelement);
				if (substring[firstcharactertocheck] != string[firstcharactertocheck + startofstring]) {
					break;
				}
			}
		}
	}
}

int main() {
	unsigned char* string = malloc(50);
	string[0] = '\n';
	unsigned char* substring = malloc(30);
	if (!fgets((char*)substring, 30, stdin)) {
		free(substring);
		free(string);
		return 0;
	}
	int lensubstring = strlen((const char*)substring) - 1;
	long long int hashsubstring;
	hashsubstring = hash(substring, 0, lensubstring-1);
	printf("%lld ", hashsubstring);
	int restrictionsubstring = 0;
	while (substring[restrictionsubstring] != '\n') {
		restrictionsubstring++;
	}
	substring[restrictionsubstring] = '\0'; 
	int sumelement = 0;
	while (fgets((char*)string, 50, stdin) != NULL) {
		int restrictionstring = 0;
		while (string[restrictionstring] != '\n' && string[restrictionstring] != '\0') {
			restrictionstring++;
		}
		string[restrictionstring] = '\0';
		RabinKarp(string, substring, sumelement);
		sumelement += strlen((const char*)substring);
	}
	free(string);
	free(substring);
	return 0;
}









