//==================================================//
// Name         : Project 2					        //
// Author       : Coty Mugford                      //
// Date         : 2/20/2024                         //
//==================================================//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "CSVparser.hpp"

using namespace std;

//Course Structure
struct Course {
	string courseNumber;
	string courseName;
	string prerequisiteOne;
	string prerequisiteTwo;
	Course() {
	}
};

//Internal Node Struture for Binary Tree
struct Node {
	Course course;
	Node *left;
	Node *right;

	//default constructor
	Node() {
		left = nullptr;
		right = nullptr;
	}

	Node(Course aCourse) :
		Node() {
		this->course = aCourse;
	}
};

//Initialize BST class functions
class BinarySearchTree {

private:

	Node* root;
	void addNode(Node* node, Course course);
	void inOrder(Node* node);

public:

	BinarySearchTree();
	virtual ~BinarySearchTree();
	void inOrder();
	void insertNode(Course course);
	Course nodeSearch(string courseNumber);
};

//Initialize root to null
BinarySearchTree::BinarySearchTree() {
	root = nullptr;
}

//Destructor
BinarySearchTree::~BinarySearchTree() {

}

//Sets sort function to root
void BinarySearchTree::inOrder() {
	inOrder(root);
}


void BinarySearchTree::insertNode(Course course) {
	//If the root is null set it to the new course
	if (root == nullptr) {
		root = new Node(course);
	}
	//Else call addNode function to insert new course
	else {
		this->addNode(root, course);
	}
}

Course BinarySearchTree::nodeSearch(string courseNumber) {
	//Sets start node to root
	Node* current = root;

	//While the current node is not null
	while (current != nullptr) {
		//If the matching courseNumber is found return the course
		if (current->course.courseNumber.compare(courseNumber) == 0) {
			return current->course;
		}

		//If the searched for courseNumber is less than the current node check next node to the left
		if (courseNumber.compare(current->course.courseNumber) < 0) {
			current = current->left;
		}

		//Else check the next node to the right
		else {
			current = current->right;
		}
	}

	Course course;
	return course;
}

void BinarySearchTree::addNode(Node* node, Course course) {
	// if node is larger then add to left
	if (node->course.courseNumber.compare(course.courseNumber) > 0) {
		// if no left node
		if (node->left == nullptr) {
			// this node becomes left
			node->left = new Node(course);
		}
		// else recurse down the left node
		else {
			this->addNode(node->left, course);
		}
	}
	// else add to right
	else {
		// if no right node
		if (node->right == nullptr) {
			// this node becomes right
			node->right = new Node(course);
		}
		//else
		else {
			// recurse down the left node
			this->addNode(node->right, course);
		}
	}
}

void BinarySearchTree::inOrder(Node* node) {

	//If the node is not null print each node in order  from smallest to largest
	if (node != nullptr) {
		inOrder(node->left);
		cout << node->course.courseNumber << ": "
			<< node->course.courseName << endl;
		inOrder(node->right);
		
	}
}

//Basic displayCourse function
void displayCourse(Course course) {
	cout << course.courseNumber << ": "
		<< course.courseName << endl;
	if (course.prerequisiteOne != "") {
		cout << "Prerequisites: " << course.prerequisiteOne;
	}
	if (course.prerequisiteTwo != "") {
		cout << ", " << course.prerequisiteTwo;
	}
	cout << endl;
	return;
}

//Loads the courses into the binary search tree from the CSV file using the CSVparser file
void loadCourses(string csvPath, BinarySearchTree* bst) {
	
	cout << "Loading CSV file " << csvPath << endl;

	// initialize the CSV Parser using the given path
	csv::Parser file = csv::Parser(csvPath);

	try {
		// loop to read rows of a CSV file
		for (unsigned int i = 0; i < file.rowCount(); i++) {

			// Create a data structure and add to the collection of courses
			Course course;
			course.courseNumber = file[i][0];
			course.courseName = file[i][1];
			if (file[i][2] != "") {
				course.prerequisiteOne = file[i][2];
			}
			if (file[i][3] != "") {
				course.prerequisiteTwo = file[i][3];
			}

			// push this course to the end
			bst->insertNode(course);
		}
	}
	catch (csv::Error& e) {
		std::cerr << e.what() << std::endl;
	}

}

int main() {

	//Initializes our structures and classes
	BinarySearchTree* bst;
	bst = new BinarySearchTree();
	Course course;

	//Initialize the file path and prompt user for file
	string csvPath = "";
	cout << "Please enter the file you would like to load (include path): " << endl;
	getline(cin, csvPath);

	string courseNum;
	int choice = 0;
	//Menu that keeps looping until user enters 9 and exits the program
	while (choice != 9) {

		cout << endl << "1. Load Courses" << endl
			<< "2. Display Course List" << endl
			<< "3. Search for Specific Course" << endl
			<< "9. Exit" << endl
			<< "Enter Choice: ";
		cin >> choice;
		cout << endl;

		//Switch statement that that allows user to access menu items via input (choice)
		switch (choice) {
		case 1:

			//Calls the loadCourses function to load courses into binary search tree
			loadCourses(csvPath, bst);
			break;

		case 2:

			//Calls the inOrder function to print out to user the course list
			bst->inOrder();
			break;

		case 3:

			//Prompts user for desired course number
			cout << "Enter the course number you are looking for: ";
			cin >> courseNum;
			//Allows for user to search using lower case letters
			for (char& num : courseNum) {
				num = toupper(num);
			}
			//Sets course equal to the user entered number
			course = bst->nodeSearch(courseNum);

			//If the course is found display the course and its prerequisites
			if (!course.courseNumber.empty()) {
				displayCourse(course);
				break;
			}
			//Else tell the user the course was not found
			else {
				cout << "Course Number " << courseNum << " was not found." << endl;
				break;
			}

		case 9:
			cout << "Good Bye";
			return 0;

		default:

			cout << choice << " is not a valid option." << endl;

		}
	}
}