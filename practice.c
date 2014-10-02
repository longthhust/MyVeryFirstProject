#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "practice.h"

int main() {
	FILE *fp;
	FILE *fp2;
	char* file_name = "students.txt";
	char* file_name2 = "log.txt";
	int studentId;
	char name[30];
	char class[10];
	char department[10];
	int r;
	StudentNode temp;
	StudentNode* result;
	int i;
	int loop;
	int userChoice;


	//Initialize index array
	for (i = 0; i<20; i++) {
		indexArray[i].r = i;
		indexArray[i].node = NULL;
		cur[i] = NULL;
	}

	printf("Begin of program\n\n");

	//Try to open file to read
	if ((fp = fopen(file_name, "r")) == NULL) {
		printf("No such file\n");
		exit(1);
	}


	//Add students to their corespoding indexes
	while (fscanf(fp, "%d:%[^:]:%[^:]:%[^:\n]", &studentId, name, class, department) == 4) {
		addStudent(studentId, name, class, department);
	}

	//Link indexes together
	linkIndexes();


	//Menu
	loop = 1;
	while (loop) {
		printf ("******************************\n* What do you wanna do?      *\n*     1- Add student         *\n*     2- Delete student      *\n*     3- Find student        *\n*     4- Edit student        *\n*     5- Student list        *\n*     6- Exit                *\n******************************");
		printf("\nYour choice: ");
		scanf("%d%*c", &userChoice);
		switch (userChoice) {
			case 1: 
				studentId = askForId();
				temp = findStudentById(studentId);
				if (temp == NULL) { 
					strcpy(name, askForName());
					strcpy(class, askForClass());
					strcpy(department, askForDepartment());
					addStudent (studentId, name, class, department);
					linkIndexes();
					addStudentToFile(studentId, name, class, department, file_name);
					makeLog(studentId, "APPEND", file_name2);
					printf("Add success!\n");
				} else
					printf("ID already existed!\n");
				break;
			case 2:
				studentId = askForId();
				if (deleteStudent(studentId) == 1) {
					deleteStudentFromFile(studentId, file_name);
					makeLog(studentId, "DELETE", file_name2);
					printf("Delete success!\n");
				} else
					printf("Can't find any student with given ID!\n");
				break;
			case 3:
				printf("\nFind by which criteria?\n1- Student ID\n2- Name\n3- Class\n4- Department\n5- Back to menu\nYour choice: ");
				scanf("%d%*c", &userChoice);
				switch (userChoice) {
					case 1:
						studentId = askForId();
						temp = findStudentById(studentId);
						printf("\nResult:\n");
						if (temp != NULL) {
							printf("%-10d | %-30s | %-10s | %-10s\n", temp->studentId, temp->name, temp->class, temp->department);
						} else 
							printf("Can't find any student with given Id!\n");
						break;
					case 2:
						strcpy(name, askForName());
						result = findStudentByName(name);
						i = 0;
						printf("\nResult:\n");
						while (result[i] != NULL) {
							printf("%-10d | %-30s | %-10s | %-10s\n", result[i]->studentId, result[i]->name, result[i]->class, result[i]->department);
							i++;
						}
						if (i == 0) 
							printf("Can't find any student with given name!\n");
						break;
					case 3:
						strcpy(class, askForClass());
						result = findStudentByClass(class);
						i = 0;
						printf("\nResult:\n");
						while (result[i] != NULL) {
							printf("%-10d | %-30s | %-10s | %-10s\n", result[i]->studentId, result[i]->name, result[i]->class, result[i]->department);
							i++;
						}
						if (i == 0) 
							printf("Can't find any student with given class!\n");
						break;
					case 4:
						strcpy(department, askForDepartment());
						result = findStudentByDepartment(department);
						i = 0;
						printf("\nResult:\n");
						while (result[i] != NULL) {
							printf("%-10d | %-30s | %-10s | %-10s\n", result[i]->studentId, result[i]->name, result[i]->class, result[i]->department);
							i++;
						}
						if (i == 0) 
							printf("Can't find any student with given department!\n");
						break;
					case 5:
						break;
					default:
						printf("\nNo suitable action found!\n");
						break;
				}
				break;
			case 4:
				studentId = askForId();
				temp = findStudentById(studentId);
				if (temp != NULL) {
					printf("\n%-10d | %-30s | %-10s | %-10s\n", temp->studentId, temp->name, temp->class, temp->department);
					deleteStudent(studentId);
					deleteStudentFromFile(studentId, file_name);
					strcpy(name, askForName());
					strcpy(class, askForClass());
					strcpy(department, askForDepartment());
					addStudent(studentId, name, class, department);
					linkIndexes();
					addStudentToFile(studentId, name, class, department, file_name);
					makeLog(studentId, "MODIFY", file_name2);
					printf("Edit success!\n");
				} else
					printf("Can't find any student with given ID!\n");
				break;
			case 5:
				//Print student list
				temp = firstNode();
				if (temp == NULL) {
					printf("\nNo student!\n");
				} else {
					printf("\n%-5s| %-10s | %-30s | %-10s | %-10s\n","Count", "StudentID", "Name", "Class", "Department");
					printf("\n");
					i = 0;
					while (temp != NULL) {
						i++;
						printf("%-5d| %-10d | %-30s | %-10s | %-10s\n",i , temp->studentId, temp->name, temp->class, temp->department);
						temp = temp->next;
					}
				}
				break;
			case 6:
				loop = 0;
				break;
			default:
				printf("\nNo suitable action found!\n");
		}
	}

	printf("\nEnd of program\n");


	//Release resources
	fclose(fp);
	return 0;
}

int askForId() {
	int id;
	printf("\nEnter a student ID: ");
	scanf("%d%*c", &id);
	return id;
}

char* askForName() {
	char* name = (char*) malloc(sizeof(char)*30);
	printf("\nEnter a name: ");
	scanf("%[^\n]%*c", name);
	return name;
}

char* askForClass() {
	char *class = (char*) malloc(sizeof(char)*10);;
	printf("\nEnter a class: ");
	scanf("%[^\n]%*c", class);
	return class;
}

char* askForDepartment() {
	char *department = (char*) malloc(sizeof(char)*10);;
	printf("\nEnter a department: ");
	scanf("%[^\n]%*c", department);
	return department;
}

StudentNode firstNode() {
	StudentNode temp = indexArray[0].node;
	int i = 0;
	while (temp == NULL && i <= 19) {
		i++;
		temp = indexArray[i].node;
	}
	return temp;
}

StudentNode* findStudentByName(char* name) {
	StudentNode* nodeArray = (StudentNode*) malloc(sizeof(StudentNode)*100);
	StudentNode temp = firstNode();
	int count = 0;
	while (temp != NULL) {
		if (strcmp(temp->name, name) == 0) {
			nodeArray[count++]=temp;
		}
		temp = temp->next;
	}
	nodeArray[count] = NULL;
	return nodeArray;
}

StudentNode* findStudentByDepartment(char* department) {
	StudentNode* nodeArray = (StudentNode*) malloc(sizeof(StudentNode)*100);
	StudentNode temp = firstNode();
	int count = 0;
	while (temp != NULL) {
		if (strcmp(temp->department, department) == 0) {
			nodeArray[count++]=temp;
		}
		temp = temp->next;
	}
	nodeArray[count] = NULL;
	return nodeArray;
}

StudentNode* findStudentByClass(char* class) {
	StudentNode* nodeArray = (StudentNode*) malloc(sizeof(StudentNode)*100);
	StudentNode temp = firstNode();
	int count = 0;
	while (temp != NULL) {
		if (strcmp(temp->class, class) == 0) {
			nodeArray[count++]=temp;
		}
		temp = temp->next;
	}
	nodeArray[count] = NULL;
	return nodeArray;
}

StudentNode findStudentById(int id) {
	int r = id % 20;
	StudentNode temp = indexArray[r].node;
	while (temp != NULL) {
		if (temp->studentId == id) {
			return temp;
		}
		temp = temp->next;
	}
	return NULL;
}

void addStudent (int id, char* name, char* class, char* department) {
	int r = id % 20;
	StudentNode newStudent = (StudentNode) malloc (sizeof(Student));
	newStudent->studentId = id;
	strcpy(newStudent->name, name);
	strcpy(newStudent->class, class);
	strcpy(newStudent->department, department);
	newStudent->next = NULL;
	if (cur[r] == NULL) {
		indexArray[r].node = newStudent;
		cur[r] = newStudent;
	}
	else {
		cur[r]->next = newStudent;
		cur[r] = newStudent;
	}
}

void addStudentToFile(int id, char* name, char* class, char* department, char* fn){
	FILE *fp;
	if ((fp = fopen(fn, "a")) == NULL) {
		printf("No such file\n");
		exit(1);
	}
	fprintf(fp, "\n%d:%s:%s:%s", id, name, class, department);
	fclose(fp);
}

void makeLog(int id, char* action, char* fn){
	time_t rawtime;
	struct tm * timeinfo;
	time (&rawtime);
	timeinfo = localtime (&rawtime);
	char* t;

	FILE *fp;
	if ((fp = fopen(fn, "a")) == NULL) {
		printf("No such file\n");
		exit(1);
	}
	t = asctime(timeinfo);
	t[strlen(t)-1] = '\0';
	fprintf(fp, "Time: %s | Action: %s | ID: %d\n", t, action, id);
	fclose(fp);
}

int deleteStudent(int id) {
	int r = id % 20;
	StudentNode delete;
	StudentNode temp = indexArray[r].node;
	if (temp == NULL) return 0;
	if (temp->studentId == id) {
		if (temp == firstNode()) {
			if (temp == cur[r]) {
				indexArray[r].node = NULL;
				cur[r] = NULL;
			} else
				indexArray[r].node = temp->next;
			free(temp);
			return 1;
		}
		while (indexArray[--r].node == NULL);
		cur[r]->next = temp->next;
		free(temp);
		return 1;
	}
	while ((temp->next != NULL) && (temp->next != cur[r]->next)) {
		if (temp->next->studentId == id) {
			delete = temp->next;
			if (delete == cur[r])
				cur[r] = temp;
			temp->next = delete->next;
			free(delete);
			return 1;
		}
		temp = temp->next;
	}
	return 0;
}


void deleteStudentFromFile(int id, char* fn){
	FILE *fp;
	FILE *fp2;
	char str[100];
	char buf[1024];
	if ((fp = fopen(fn, "r")) == NULL) {
		printf("No such file\n");
		exit(1);
	}
	if ((fp2 = fopen("replica.txt", "w")) == NULL) {
		printf("No such file\n");
		exit(1);
	}
	sprintf(str, "%d", id);
	while (fgets(buf, sizeof(buf), fp) != NULL) {
		if((strncmp(buf, str, strlen(str)) == 0) && (buf[strlen(str)] == ':'))
			continue;
		if (strcmp(buf,"\n") == 0)
			continue;
		fputs(buf, fp2);
	}
	fclose(fp);
	fclose(fp2);
	remove(fn);
	rename("replica.txt", fn);
}

void linkIndexes() {
	int j = 0;
	int k = 0;
	while (j <= 18) {
		if (indexArray[j].node != NULL) {
			k = j + 1;
			while (k <= 19) {
				if (indexArray[k].node != NULL) {
					cur[j]->next = indexArray[k].node;
					break;
				}
				k++;
			}
			j = k;
		} else {
			j++;
		}
	}
}










