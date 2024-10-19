#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sqlite3.c"
#include <Windows.h>



void add(int rc, sqlite3* database, char* error_message) {
	printf("Введите Имя\n> ");
	char name[10];
	if (scanf("%s", &name) != 1) {
		return;
	}
	printf("Введите Фамилию\n> ");
	char surname[10];
	if (scanf("%s", &surname) != 1) {
		return;
	}
	printf("Введите возраст\n> ");
	int age;
	if (scanf("%d", &age) != 1) {
		return;
	}
	while (getchar() != '\n');
	printf("Введите причину почему он/она бесит\n> ");
	char reason[50];
	if (fgets(reason, sizeof(reason), stdin) == NULL) {
		return;
	}
	reason[strcspn(reason, "\n")] = '\0';
	char sql[200];
	snprintf(sql, sizeof(sql), "INSERT INTO PeopleWhoPissMeOff (name, surname, age, reason) VALUES ('%s', '%s', %d, '%s')", name, surname, age, reason);
	rc = sqlite3_exec(database, sql, 0, 0, &error_message);
	if (rc != SQLITE_OK) {
		printf("SQL error: %s\n", error_message);
		sqlite3_free(error_message);
		sqlite3_close(database);
		return;
	}
}

void deletePerson(int rc, sqlite3* database, char* error_message) {
	printf("Введите имя человека, которого хотите удалить\n> ");
	char name[50];
	if (scanf("%s", name) != 1) {
		return;
	}
	printf("Введите фамилию человека, которого хотите удалить\n> ");
	char surname[50];
	if (scanf("%s", surname) != 1) {
		return;
	}
	char sql[100];
	snprintf(sql, sizeof(sql), "DELETE FROM PeopleWhoPissMeOff WHERE name='%s' AND surname='%s'", name, surname);
	rc = sqlite3_exec(database, sql, 0, 0, &error_message);
	if (rc != SQLITE_OK) {
		printf("SQL error: %s\n", error_message);
		sqlite3_free(error_message);
		sqlite3_close(database);
		return;
	}
}



int filter(void* notUsed, int colCount, char** columns, char** colNames) {
	for (int i = 0; i < colCount; i++)
	{
		if (columns[i] != NULL) {
			printf("%s = %s\n", colNames[i], columns[i]);
		}
		else {
			printf("%s = NULL\n", colNames[i]);
		}
	}
	printf("\n");
	return 0;
}

int printTable(void* notUsed, int colCount, char** columns, char** colNames) {
	for (int i = 0; i < colCount; i++)
	{
		if (columns[i] != NULL) {
			printf("%s = %s\n", colNames[i], columns[i]);
		}
		else {
			printf("%s = NULL\n", colNames[i]);
		}
	}
	printf("\n");
	return 0;
}

int printTableInFile(void* notUsed, int colCount, char** columns, char** colNames) {
	FILE* output_file = fopen("output.txt", "a");
	if (output_file == NULL) {
		printf("Ошибка открытия файла для записи.\n");
		return 1;
	}
	for (int i = 0; i < colCount; i++)
	{
		if (columns[i] != NULL) {
			fprintf(output_file, "%s = %s\n", colNames[i], columns[i]);
		}
		else {
			fprintf(output_file, "%s = NULL\n", colNames[i]);
		}

	}
	fprintf(output_file, "\n");
	fclose(output_file);
	return 0;
}

void save(int rc, sqlite3* database, char* error_message) {
	printf("Данные в файле должны быть предоставленны следующим образом:\nname\nsurname\nage\nreason\nname\nи тд.\n");
	FILE* input_file;
	input_file = fopen("input.txt", "r");
	if (input_file == NULL) {
		printf("Ошибка открытия файла для записи.\n");
		return;
	}
	while (!feof(input_file)) {
		char name[10];
		char surname[20];
		int age;
		char reason[50];
		if (fscanf(input_file, "%s\n", &name) == NULL) {
			return;
		}
		if (fscanf(input_file, "%s\n", &surname) == NULL) {
			return;
		}
		if (fscanf(input_file, "%d\n", &age) == NULL) {
			return;
		}
		if (fgets(reason, sizeof(reason), input_file) == NULL) {
			return;
		}
		reason[strcspn(reason, "\n")] = '\0';
		char sql[200];
		snprintf(sql, sizeof(sql), "INSERT INTO PeopleWhoPissMeOff (name, surname, age, reason) VALUES ('%s', '%s', %d, '%s')", name, surname, age, reason);
		rc = sqlite3_exec(database, sql, 0, 0, &error_message);
		if (rc != SQLITE_OK) {
			printf("SQL error: %s\n", error_message);
			sqlite3_free(error_message);
			sqlite3_close(database);
			return;
		}

	}
}

void description() {
	printf("Вам предоставлена база данных людей, которые меня бесят:\n");
	printf("данная база данных может:");
	printf("ниже предоставлены команды, которые вы можете использовать:\n");
	printf("1) добавление\n2) удаление по имени-фамилии\n3) отфильтровать записи\n4) вывести запись\n5) загрузить записи из файла\n6) сохранить записи в файл\n7) завершить программу\n8) вывести доступные команды\n");
}





int main() {
	setlocale(LC_ALL, "Ru");
	sqlite3* database;
	char* error_message = 0;
	int rc = sqlite3_open("database.db", &database);
	if (rc != SQLITE_OK) {
		sqlite3_close(database);
		return 1;
	}
//новое создание таблицы:
/*	char* sql = "DROP TABLE IF EXISTS PeopleWhoPissMeOff;\nCREATE TABLE PeopleWhoPissMeOff(id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, surname TEXT, age INTEGER, reason TEXT);";
	rc = sqlite3_exec(database, sql, 0, 0, &error_message);
	if (rc != SQLITE_OK) {
		printf("SQL error: %s\n", error_message);
		sqlite3_free(error_message);
		sqlite3_close(database);
		return 1;
	}
*/
	description();
	int checkExit = 1;

	do {
		printf("Введите действие\n> ");
		int a;
		char b[100];
		if (fgets(b, sizeof(b), stdin) == NULL) {
			return 0;
		}
		sscanf(b, "%d", &a);
		if (a != 1 && a != 2 && a != 3 && a != 4 && a != 5 && a != 6 && a != 7 && a != 8) {
			printf("Ошибка, прочтите документацию и попробуйте еще раз\n");
			return -1;
		}
		if (a == 1) {
			add(rc, database, error_message);
		}
		if (a == 2) {
			deletePerson(rc, database, error_message);
		}
		if (a == 3) {
			printf("Введите критерий по которому вы хотит вывести. Доступны:\n1)name\n2)surname\n3)age\n4)reason\n> ");
			char сriteria[10];
			if (scanf("%s", &сriteria) != 1) {
				return 0;
			}
			if (strcmp(сriteria, "name") == 0) {
				printf("Введите имя\n> ");
				char name[10];
				if (scanf("%s", &name) != 1) {
					return 0;
				}
				char sql[100];
				snprintf(sql, sizeof(sql), "SELECT name, surname, age, reason FROM PeopleWhoPissMeOff WHERE name = '%s'", name);
				rc = sqlite3_exec(database, sql, printTable, 0, &error_message);
				if (rc != SQLITE_OK)
				{
					printf("SQL error: %s\n", error_message);
					sqlite3_free(error_message);
					sqlite3_close(database);
					return 1;
				}
			}
			else if (strcmp(сriteria, "surname") == 0) {
				printf("Введите фмилию\n> ");
				char surname[20];
				if (scanf("%s", &surname) != 1) {
					return 0;
				}
				char sql[100];
				snprintf(sql, sizeof(sql), "SELECT name, surname, age, reason FROM PeopleWhoPissMeOff WHERE surname = '%s'", surname);
				rc = sqlite3_exec(database, sql, printTable, 0, &error_message);
				if (rc != SQLITE_OK)
				{
					printf("SQL error: %s\n", error_message);
					sqlite3_free(error_message);
					sqlite3_close(database);
					return 1;
				}
			}
			else if (strcmp(сriteria, "age") == 0) {
				printf("Введите возраст\n> ");
				int age = 0;
				if (scanf("%d", &age) != 1) {
					return 0;
				}
				char sql[100];
				snprintf(sql, sizeof(sql), "SELECT name, surname, age, reason FROM PeopleWhoPissMeOff WHERE age = %d", age);
				rc = sqlite3_exec(database, sql, printTable, 0, &error_message);
				if (rc != SQLITE_OK)
				{
					printf("SQL error: %s\n", error_message);
					sqlite3_free(error_message);
					sqlite3_close(database);
					return 1;
				}
			}
			else if (strcmp(сriteria, "reason") == 0) {
				char reason[50];
				if (fgets(reason, sizeof(reason), stdin) == NULL) {
					return 0;
				}
				reason[strcspn(reason, "\n")] = '\0';
				char sql[100];
				snprintf(sql, sizeof(sql), "SELECT name, surname, age, reason FROM PeopleWhoPissMeOff WHERE reason = '%s'", reason);
				rc = sqlite3_exec(database, sql, printTable, 0, &error_message);
				if (rc != SQLITE_OK)
				{
					printf("SQL error: %s\n", error_message);
					sqlite3_free(error_message);
					sqlite3_close(database);
					return 1;
				}
			}
			else {
				printf("Такого критерия нет, введите другой\n");
			}
		}
		if (a == 4) {
			char* sql = "SELECT * FROM PeopleWhoPissMeOff";
			rc = sqlite3_exec(database, sql, printTable, 0, &error_message);
			if (rc != SQLITE_OK)
			{
				printf("SQL error: %s\n", error_message);
				sqlite3_free(error_message);
				sqlite3_close(database);
				return 1;
			}
		}
		if (a == 6) {
			char* sql = "SELECT * FROM PeopleWhoPissMeOff";
			rc = sqlite3_exec(database, sql, printTableInFile, 0, &error_message);
			if (rc != SQLITE_OK)
			{
				printf("SQL error: %s\n", error_message);
				sqlite3_free(error_message);
				sqlite3_close(database);
				return 1;
			}
		}
		if (a == 5) {
			save(rc, database, error_message);
		}
		if (a == 7) {
			checkExit = 0;
		}
		if (a == 8) {
			description();
		}
	} while (checkExit == 1);
	sqlite3_close(database);
	return 0;
}

