#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include<iostream>
using namespace std;
class fileParse
{

public:
	vector<string> coursesInfo;
	vector<string> courseTitles;

	void fileReader(const string fileToRead) {

		string courseCode;
		string courseName;


		// Open the file for reading
		ifstream file(fileToRead);

		// Read the file line by line and store each line in the vector
		string line;
		while (getline(file, line)) {
			coursesInfo.push_back(line);
		}


		// Loop through the vector and add each course title to the courseTitles vector
		for (const auto& courseInfo : coursesInfo) {
			// Split the course information into individual lines
			vector<string> courseBreakdown;
			stringstream ss(courseInfo);
			string item;
			while (getline(ss, item, ',')) {
				courseBreakdown.push_back(item);
			}
			// Check the formatting of the course code
			if (courseBreakdown[0].size() != 7) {
				throw invalid_argument("Invalid formatting for course code");
			}
			courseCode = courseBreakdown[0];

			if (courseBreakdown[1].empty()) {
				throw invalid_argument("Course name cannot be empty");
			}

			courseName = courseBreakdown[1];
			courseTitles.push_back(courseCode);

		}

		// Loop through the vector again and check the prerequisites for each course
		for (const auto& courseInfo : coursesInfo) {
			// Split the course information into individual lines
			vector<string> courseBreakdown;
			stringstream ss(courseInfo);
			string item;
			while (getline(ss, item, ',')) {
				courseBreakdown.push_back(item);
			}


			// Loop through the prerequisites and check their formatting and validity
			for (int i = 2; i < courseBreakdown.size(); ++i) {
				if (courseBreakdown[i].empty()) {
					continue;
				}
				else if (courseBreakdown[i].size() != 7) {
					throw invalid_argument("Invalid course prereq formatting");
				}


				// Check if the prerequisite is a valid course
				const string& prereq = courseBreakdown[i];
				if (find(courseTitles.begin(), courseTitles.end(), prereq) == courseTitles.end()) {

					for (auto title : courseTitles) {
						cout << title << endl;
					}
				}

			}


		}

		// Process the courseTitles vector as needed
	}


};




#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>


using namespace std;

//Structure that will hold course information
struct Course {
	string code;
	string name;
	vector <string> preReqs;
	int id;

};

//Structure for tree nodes
struct Node {
	Course course;
	Node* left;
	Node* right;


	//default constructor
	Node() {
		left = nullptr;
		right = nullptr;
	}
	//intialize with a course
	Node(Course aCourse) {
		this->course = aCourse;
		left = nullptr;
		right = nullptr;
	}

};
class BinarySearchTree {
private:
	Node* root;

	void addCourse(Node* node, Course course);
	void inOrder(Node* node);


public:
	BinarySearchTree();
	virtual ~BinarySearchTree();
	void Insert(Course course);
	void InOrder();
	void loadBids(string fileName);
	Course Search(string courseCode);
};




 //Default constructor

BinarySearchTree::BinarySearchTree() {
	root = nullptr;
}

//Destructor
BinarySearchTree::~BinarySearchTree() {
	// recurse from root deleting every node
}


void BinarySearchTree::InOrder() {
	inOrder(root);
}


void BinarySearchTree::loadBids(string fileName) {
	ifstream readFile(fileName); //opens our course information file
	
	
	//Ensure the file is opened succesfully
	if (!readFile){
		cerr << "Error opening input file" << endl;
	}
	string line;
	vector<string> lines;

	

	//Reads each line in file
	while (getline(readFile, line)) {
		
		
		;
		lines.push_back(line);

	}
	//takes each line and breaks it down
	for (auto line : lines) {
		vector<string> tokens;
		string token;
		stringstream ss(line);
		Course course;

		while (getline(ss, token, ',')) {
			tokens.push_back(token);
		}
		
		for (int i = 0; i < tokens.size(); i++) {
			if (i == 0) {
				course.code = tokens.at(i);
				course.id = stoi(tokens.at(i).substr(tokens.at(i).size() - 3));
			
			}
			if (i == 1) {
				
				course.name = tokens.at(i);
				;
			}
			if (i > 1) {
				course.preReqs.push_back(tokens.at(i));
				
			}
		
		

		}
		
		this->Insert(course);

	}


}


void BinarySearchTree::addCourse(Node* node, Course courses) {
	
	if (node->course.code.compare(courses.code) > 0) {
		if (node->left == nullptr) {
			node->left = new Node(courses);
		}
		else {
			this->addCourse(node->left, courses);
		}
	}
	else {
		if (node->right == nullptr) {
			
			node->right = new Node(courses);
		}
		else {
		
			this->addCourse(node->right, courses);
		}
	}
}


void  BinarySearchTree:: inOrder(Node* node){
	//This is a recursive call that will use recursion to make it to the bottom left node and print in order of l, curr, r
	 
	if (node != nullptr) {
		// l
		inOrder(node->left); 

		//curr
		cout << node->course.code << " : " << node->course.name ;
		
		cout << endl;

		//r
		inOrder(node->right);
	}
	
}

void BinarySearchTree:: Insert(Course course) {
	//If the root is empty we will inset the course to the root
	
	if (root == nullptr) {
		root = new Node(course);
	}

	//else add the course using addCourse() method
	else {
		this->addCourse(root, course);
	}

	

}

Course BinarySearchTree::Search(string courseCode) {
	try {
		Node* currentNode = this->root;
		while (currentNode != nullptr) {
			if (currentNode->course.code.compare(courseCode) == 0) {
				// Print course code, name, and prerequisites (if any)
				cout << currentNode->course.code << ", " << currentNode->course.name << endl << "Prerequisites: ";

				// Loop through each prerequisite and print it
				for (int i = 0; i < currentNode->course.preReqs.size(); i++) {
					cout << currentNode->course.preReqs[i];

					// Print a comma and space between prerequisites (except for the last one)
					if (i < currentNode->course.preReqs.size() - 1) {
						cout << ", ";
					}
				}

				// Print a newline character to separate output
				cout << endl;

				// Return the course associated with the current node
				return currentNode->course;
			}
			else if (currentNode->course.code.compare(courseCode) < 0) {
				currentNode = currentNode->right;
			}
			else {
				currentNode = currentNode->left;
			}
		}
		// Throw an exception if the course was not found
		throw invalid_argument("Course not found");
	}
	catch (const invalid_argument& e) {
		// Print an error message if the course was not found
		cout << "Error: " << e.what() << endl;
	}
}

int main(int argc, char** argv) {

	BinarySearchTree* bst;
	bst = new BinarySearchTree();
	bool done = false;

	cout << "Welcome to the course planner." << endl << endl;
	while (!done) {
		int choice;
		string courseToFind;
		fileParse fileReader;
		string fileName;


		cout << "1. Load Data Structure." << endl;
		cout << "2. Print Course List." << endl;
		cout << "3. Print Course." << endl;
		cout << "9. Exit" << endl << endl;
		cout << "What would you like to do? ";


		cin >> choice;
		//We will validate the input before starting switch statements
		
		if (cin.fail()) {
			cout << "Do not type characters. Your options are 1, 2, 3 or 9" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}
		else if (choice != 1 && choice != 2 && choice != 3 && choice != 9) {
			cout << "Invalid number try 1, 2, 3 or 9" << endl;
		}
		
		
		switch (choice) {
		case 1:
			cout << "What is the name of the CSV file?";
			cin >> fileName;

			try {
				// Verify all course information is valid before loading structure
				fileReader.fileReader(fileName);

				// After verification of information we load it into the BST
				bst->loadBids(fileName);
				break;
			}
			catch (const invalid_argument& e) {
				cerr << "Error: " << e.what() << endl;
				break;
			}


		case 2:
			//Prints our BST in order from lowest to highest value
			bst->InOrder();

			break;

		case 3:
			//Ask user for course to find


			cout << "What course do you want to know about? ";
			cin >> courseToFind;

			// Convert courseToFind to all uppercase letters
			for (int i = 0; i < courseToFind.length(); i++) {
				courseToFind[i] = toupper(courseToFind[i]);
			}

			//using that Search for the course with our BST Search method
			bst->Search(courseToFind);
			break;

		case 9:
			done = true;
			break;

		}

	}
}
