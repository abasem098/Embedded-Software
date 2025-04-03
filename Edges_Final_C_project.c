//====================================================================
//header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//====================================================================
//typedefs
typedef struct student s;
typedef struct node n;
//====================================================================
//function prototypes
void personal_intro(void);
void main_menu(void);
void addStudent(s *ptr);
void displayStudents(void);
void searchStudentByID(int id);
void updateStudent(int id);
void deleteStudent(int id);
float calculateAverageGPA(void);
void searchHighestGPA(void);
n* create_node(n *head, const s *ptr);
//====================================================================
//structures
struct student {
	char name[50];
	float GPA;
	int ID;
	int Age;
};
struct node {
	s data; //data type struct sd
	struct node *next; //pointer to structure
};
//====================================================================
//Global declarations
n *head = NULL;
int ID = 0;
float avg = 0;
//====================================================================
//main function
int main(void) {
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	personal_intro();
	main_menu();
	return 0;
}
//====================================================================
//function definitions
void main_menu(void) {
	int option = 0;
	for (int i = 0; i < 30; i++)
		printf("==");
	printf("\n");
	printf("Main Menu:\n");
	printf("Choose an option:\n");
	printf("1-Add a Student\n");
	printf("2-Display All Students\n");
	printf("3-Search for a Student by ID\n");
	printf("4-Update Student Information\n");
	printf("5-Delete a Student\n");
	printf("6-Calculate Average GPA\n");
	printf("7-Find Student with Highest GPA\n");
	printf("Press any key to exit\n");
	scanf("%d", &option);

	switch (option) {
	case 1:
		printf("1-Add a Student:-\n");
		s student_data;
		addStudent(&student_data);
		head = create_node(head, &student_data);
		main_menu();
		break;
	case 2:
		printf("2-Display All Students:-\n");
		displayStudents();
		main_menu();
		break;
	case 3:

		printf("3-Search for a Student by ID:-\n");
		printf("Enter student ID:\n");
		scanf("%d", &ID);
		searchStudentByID(ID);
		main_menu();
		break;
	case 4:
		printf("4-Update Student Information:-\n");
		printf("Enter student ID:\n");
		scanf("%d", &ID);
		updateStudent(ID);
		main_menu();
		break;
	case 5:
		printf("5-Delete a Student:-\n");
		printf("Enter student ID:\n");
		scanf("%d", &ID);
		deleteStudent(ID);
		main_menu();
		break;
	case 6:
		printf("6-Calculate Average GPA:-\n");
		avg = calculateAverageGPA();
		printf("average GPA of all students is %.2f\n", avg);
		main_menu();
		break;
	case 7:
		printf("7-Find Student with Highest GPA\n");
		searchHighestGPA();
		main_menu();
		break;
	default:
		return;

	}
}
//====================================================================
n* create_node(n *head, const s *ptr) {
	n *new = (n*) malloc(sizeof(n));
	//handle failed allocation case
	if (new == NULL) {
		printf("Allocation failed.\n");
		return NULL;
	}
	strcpy(new->data.name, ptr->name);
	new->data.Age = ptr->Age;
	new->data.ID = ptr->ID;
	new->data.GPA = ptr->GPA;
	new->next = NULL;
	//handle empty case
	if (head == NULL) {
		return new;
	}
	//you can't change value of head in the function, head is only changed in the main and passed as a parameter
	n *current = head;
	while (current->next != NULL) {
		current = current->next;
	}
	current->next = new;
	return head;
}
//====================================================================
void addStudent(s *ptr)
{

	//add students name,id,gpa,age
	while (getchar() != '\n')
		; // Only clear after previous scanf
	printf("Enter student name:");
	fgets(ptr->name, 50, stdin);
	ptr->name[strcspn(ptr->name, "\n")] = '\0'; // Remove newline character

	printf("Enter student age:");
	scanf("%d", &ptr->Age);

	printf("Enter student ID:");
	scanf("%d", &ptr->ID);

	printf("Enter student GPA:");
	scanf("%f", &ptr->GPA);
	if (ptr->GPA > 4 || ptr->GPA < 0) {
		printf("Error, GPA should be higher than 0 and no more than 4.\n");
		printf("Enter student GPA:");
		scanf("%f", &ptr->GPA);
	}
	printf("Student's data is saved successfully.\n");

}
//====================================================================
void displayStudents(void)
{
	if (head == NULL) {
		printf("List is empty!\n");
		return;
	}
	// to make a display function with void parameter you need a head as a global variable
	n *current = head;  // Start at the head

	// Traverse until we reach NULL (end of list)
	while (current != NULL) {
		printf("[Name: %s, Age: %d, ID: %d, GPA: %.2f]\n", current->data.name,
				current->data.Age, current->data.ID, current->data.GPA);
		current = current->next;  // Move to next node
	}

}
//====================================================================
void searchStudentByID(int id) {
	if (head == NULL) {
		printf("List is empty!\n");
		return;
	}
	n *current = head;
	while (current != NULL) {
		if (current->data.ID == id) {
			printf("[Name: %s, Age: %d, ID: %d, GPA: %.2f]\n",
					current->data.name, current->data.Age, current->data.ID,
					current->data.GPA);
			return;
		}
		current = current->next;
	}
	printf("Student isn't found\n");

}
//====================================================================
void updateStudent(int id) {
	if (head == NULL) {
		printf("List is empty!\n");
		return;
	}
	n *current = head;
	while (current != NULL) {
		if (current->data.ID == id) {
			//add students name,id,gpa,age
			while (getchar() != '\n')
				; // Only clear after previous scanf
			printf("Enter student name:");
			fgets(current->data.name, 50, stdin);
			current->data.name[strcspn(current->data.name, "\n")] = '\0'; // Remove newline character

			printf("Enter student age:");
			scanf("%d", &current->data.Age);

			printf("Enter student GPA:");
			scanf("%f", &current->data.GPA);
			printf("Student's data is updated successfully.\n");
			return;

		}
		current = current->next;
	}
	printf("Student isn't found\n");

}
//====================================================================
void deleteStudent(int id) {
	if (head == NULL) {
		printf("List is empty!\n");
		return;
	}
	// Special case: deleting first node
	if (head->data.ID == id) {
		n *temp = head;
		head = head->next;
		free(temp);
		printf("Student's data is deleted successfully.\n", id);
		return;
	}
	n *current = head;
	n *prev = NULL;

	while (current != NULL && current->data.ID != id) {
		prev = current;
		current = current->next;
	}
	if (current == NULL) {
		printf("Student isn't found!\n", id);
		return;
	}
	prev->next = current->next;
	free(current);
	printf("Student's data is deleted successfully.\n", id);

}
//====================================================================
float calculateAverageGPA(void) {
	if (head == NULL) {
		return 0;
	}
	n *current = head;
	float summation = 0, counter = 0;
	while (current != NULL) {
		counter++;
		summation += current->data.GPA;
		current = current->next;
	}
	float avg = summation / counter;
	return avg;
}
//====================================================================
void searchHighestGPA(void) {
	if (head == NULL) {
		printf("List is empty!\n");
		return;
	}

	n *current = head;
	n *highestStudent = head;  // Keep track of student with highest GPA
	float highest = head->data.GPA;  // Initialize with first student's GPA

	while (current != NULL) {
		if (current->data.GPA > highest) {
			highest = current->data.GPA;
			highestStudent = current;  // Save the pointer to this student
		}
		current = current->next;
	}
	printf("The student with highest GPA:\n");
	printf("Name: %s\n", highestStudent->data.name);
	printf("ID: %d\n", highestStudent->data.ID);
	printf("GPA: %.2f\n", highestStudent->data.GPA);
}
//====================================================================
void personal_intro(void) {
	for (int i = 0; i < 30; i++)
		printf("==");
	printf("\n");
	printf("Title:Edges C final project\n");
	printf("Name:Ahmed Basem");
	printf("\n");
	for (int i = 0; i < 30; i++)
		printf("==");
	printf("\n");
}

