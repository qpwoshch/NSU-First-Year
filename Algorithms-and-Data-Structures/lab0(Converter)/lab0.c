#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <locale.h>
#include <math.h>	
#include <stdio.h>
#include <assert.h>
#include <string.h>

int check1(int b1, int b2, char* input) {
	int check = 0;
	if (b1 > 16 || b1 < 2 || b2 > 16 || b2 < 2) {
		check = 1;
	}
	size_t size = strlen(input);
	int amountofpoints = 0;
	for (size_t i = 0; i < size - 1; i++) {
		int d = 0;
		if (input[i] == '.') {
			amountofpoints++;
		}
		else if (input[i] >= '0' && input[i] <= '9') {
			d = (input[i] - '0');
			if (d >= b1) {
				check = 1;
			}
		}
		else if (input[i] >= 'A' && input[i] <= 'Z') {
			d = (input[i] - 'A' + 10);
			if (d >= b1) {
				check = 1;
			}
		}
		else if (input[i] >= 'a' && input[i] <= 'z') {
			d = (input[i] - 'a' + 10);
			if (d >= b1) {
				check = 1;
			}
		}
		else {
			check = 1;
		}
	}
	if (input[0] == '.' || input[size - 2] == '.') {
		check = 1;
	}
	if (amountofpoints > 1) {
		check = 1;
	}
	return check;
}


double todecimal(char* input, int b1) {
	double decimal = 0;
	size_t size = strlen(input);
	int po = size-1;
	for (size_t i = 0; i < size; i++) {
		if (input[i] == '.') {
			po = i;
			break;
		}
	}
	for (size_t i = 0; i < size; i++) {
		if (input[i] == '.') {
			continue;
		}
		if (input[i] >= '0' && input[i] <= '9') {
			decimal += (input[i] - '0') * pow(b1, po - 1);
			po -= 1;
		}
		else if (input[i] >= 'A' && input[i] <= 'Z') {
			decimal += (input[i] - 'A' + 10) * pow(b1, po - 1);
			po -= 1;
		}
		else if (input[i] >= 'a' && input[i] <= 'z') {
			decimal += (input[i] - 'a' + 10) * pow(b1, po - 1);
			po -= 1;
		}
	}
	return decimal;
}


char* fromdecimal(double input, int b2) {
	long long int help = floor(input);
	int digit = 1;
	while (help >= b2) {
		digit++;
		help /= b2;
	}
	long long int input1 = floor(input);
	char* result = (char*)malloc(100);
	if (!result) {
		return NULL;
	}

	double prov = floor(input);
	long long int help1 = input1;
	for (int i = 0; i < digit; i++) {
		int h = help1 % b2;
		if (h >= 0 && h < 10) {
			char v = h;
			result[digit - 1 - i] = v + '0';
		}
		else if (h >= 10) {
			char v = h;
			result[digit - 1 - i] = v + 'A' - 10;
		}
		help1 /= b2;
	}
	if (prov == input) {
		result[digit] = '\0';
		return result;
	}
	else {
		result[digit] = '.';
		double fraction = input - prov;
		int decimalplaces = 1;
		while (decimalplaces != 15 && fraction - floor(fraction) != 0) {
			fraction *= b2;
			char hfraction = floor(fraction);
			if (floor(fraction) >= 0 && floor(fraction) < 10) {
				result[digit + decimalplaces] = hfraction + '0';
			}
			else if (floor(fraction) >= 10) {
				result[digit + decimalplaces] = hfraction + 'A' - 10;
			}
			decimalplaces++;
			fraction = fraction - floor(fraction);
		}
		result[digit + decimalplaces] = '\0';
		return result;
	}
}



int main() {
	setlocale(LC_ALL, "Rus");
	int b1;
	int b2;
	if (scanf("%d%d\n", &b1, &b2) != 2) {
		printf("bad input");
		return 0;
	}
	char input[15];
	if (fgets(input, 15, stdin) == NULL) {
		printf("bad input");
		return 0;
	}
	int check = check1(b1, b2, input);
	if (check == 1) {
		printf("bad input");
		return 0;
	}
	double decimal = todecimal(input, b1);
	char* answer = fromdecimal(decimal, b2);
	if (!answer) {
		printf("system error");
		return -1;
	}
	puts(answer);
	free(answer);
	return 0;
}


