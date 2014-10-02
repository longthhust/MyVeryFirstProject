typedef struct _Student {
	int studentId;
	char name[30];
	char class[10];
	char department[10];
	struct _Student* next;
} Student;

typedef Student* StudentNode;

typedef struct _ArrayNode {
	int r;
	StudentNode node;
} ArrayNode;

int askForId();
char* askForName();
char* askForClass();
char* askForDepartment();
StudentNode firstNode();
StudentNode* findStudentByName(char* name);
StudentNode* findStudentByClass(char* class);
StudentNode* findStudentByDepartment(char* department);
StudentNode findStudentById(int id);
void addStudent (int id, char* name, char* class, char* department);
void addStudentToFile(int id, char* name, char* class, char* department, char* fn);
void makeLog(int id, char* action, char* fn);
int deleteStudent(int id);
void deleteStudentFromFile(int id, char* fn);
void linkIndexes();

StudentNode cur[20];
ArrayNode indexArray[20];