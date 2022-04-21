#include <iostream>
#include <fstream>
#include <algorithm>
#include <queue>
#include <vector>
#include <map>
#include <string>
using namespace std;

class Node {
public:
	string name;
	map<int, vector<pair<int, double>>> mp;
	Node* right;
	Node* left;
	Node() { name = ""; right = nullptr, left = nullptr; }
};

int findHeight(Node* root) {
	if (root == nullptr) {
		return 0;
	}
	else {
		return 1 + max(findHeight(root->left), findHeight(root->right));
	}
}

Node* rightRoto(Node* root) {
	Node* newR = root->left;
	Node* temp = root->left->right;
	newR->right = root;
	root->left = temp;
	return newR;
}

Node* leftRoto(Node* root) {
	Node* newR = root->right;
	Node* temp = root->right->left;
	newR->left = root;
	root->right = temp;
	return newR;
}

Node* insertNode(Node* root, string& key, map<int, vector<pair<int, double>>> vals) {
	if (root == nullptr) {
		Node* temp = new Node();
		temp->name = key;
		temp->mp = vals;
		return temp;
	}
	else if (root->name > key) {
		root->left = insertNode(root->left, key, vals);
	}
	else if (root->name < key) {
		root->right = insertNode(root->right, key, vals);
	}
	int balance = findHeight(root->right) - findHeight(root->left);
	//LL if left side bigger and newly inserted is left child of left root
	if (balance < -1 && key < root->left->name) {
		return rightRoto(root);
	}
	//LR if left side bigger and newly inserted is right child of left root
	else if (balance < -1 && key > root->left->name) {
		root->left = leftRoto(root->left);
		return rightRoto(root);
	}
	//RR if right side bigger and newly inserted is right child of right root
	else if (balance > 1 && key > root->right->name) {
		return leftRoto(root);
	}
	//RL if right side bigger and newly inserted is left child of right root
	else if (balance > 1 && key < root->right->name) {
		root->right = rightRoto(root->right);
		return leftRoto(root);
	}
	return root;
};

Node* findNode(Node* root, string& toFind) {
	if (root->name == toFind) {
		return root;
	}
	else {
		if (toFind > root->name) {
			return findNode(root->right, toFind);
		}
		if (toFind < root->name) {
			return findNode(root->left, toFind);
		}
	}
}

void print(Node* root) {
	if (root == nullptr) {
		return;
	}
	queue<Node*> q;
	q.push(root);
	while (!q.empty()) {
		Node* toP = q.front();
		cout << toP->name << " ";
		q.pop();
		if (toP->left != nullptr) {
			q.push(toP->left);
		}
		if (toP->right != nullptr) {
			q.push(toP->right);
		}
	}
}

int main() {
	int done = 2039;
	Node* root = nullptr;
	vector<string> fs;
	vector<string> names;
	string country;

	country = "Austria";
	fs.push_back("at.csv");
	names.push_back(country);

	country = "Belgium";
	fs.push_back("be.csv");
	names.push_back(country);

	country = "Switzerland";
	fs.push_back("ch.csv");
	names.push_back(country);

	country = "Germany";
	fs.push_back("de.csv");
	names.push_back(country);

	country = "Denmark";
	fs.push_back("dk.csv");
	names.push_back(country);

	country = "Spain";
	fs.push_back("es.csv");
	names.push_back(country);

	country = "France";
	fs.push_back("fr.csv");
	names.push_back(country);

	country = "United Kingdom";
	fs.push_back("gb.csv");
	names.push_back(country);

	country = "Ireland";
	fs.push_back("ie.csv");
	names.push_back(country);

	country = "Italy";
	fs.push_back("it.csv");
	names.push_back(country);

	country = "Luxembourg";
	fs.push_back("lu.csv");
	names.push_back(country);

	country = "Netherlands";
	fs.push_back("nl.csv");
	names.push_back(country);

	country = "Norway";
	fs.push_back("no.csv");
	names.push_back(country);

	country = "Portugal";
	fs.push_back("pt.csv");
	names.push_back(country);

	country = "Sweden";
	fs.push_back("se.csv");
	names.push_back(country);


	for (int k = 0; k < 14; k++) {
		map<int, vector<pair<int, double>>> vals;
		string newNode = names[k];
		string line1, line2, line3, line4, line5, line6, line7, line8, line9, line10, line11, line12;
		fstream newF(fs[k]);
		cout << newNode << " ";
		while (newF.is_open()) {
			string start, end, load;
			getline(newF, start, ',');
			getline(newF, end, ',');
			getline(newF, load, '\n');
			if (newNode == "Austria" || newNode == "Belgium" || newNode == "Germany" || newNode == "Luxembourg" || newNode == "Netherlands") {
				int count = 0;
				for (int i = 0; i < done; i++) {
					for (int j = 0; j < 24; j++) {
						getline(newF, line1, ',');
						getline(newF, line2, ',');
						getline(newF, line3, '\n');
						getline(newF, line4, ',');
						getline(newF, line5, ',');
						getline(newF, line6, '\n');
						getline(newF, line7, ',');
						getline(newF, line8, ',');
						getline(newF, line9, '\n');
						getline(newF, line10, ',');
						getline(newF, line11, ',');
						getline(newF, line12, '\n');
						count += 4;
						double hourUse = stod(line3) + stod(line6) + stod(line9) + stod(line12);
						pair<int, double> p;
						p.first = j;
						p.second = hourUse;
						vals[i].push_back(p);
					}
				}
			}
			else if (newNode == "United Kingdom" || newNode == "Ireland") {
				for (int i = 0; i < done; i++) {
					for (int j = 0; j < 24; j++) {
						getline(newF, line1, ',');
						getline(newF, line2, ',');
						getline(newF, line3, '\n');
						getline(newF, line4, ',');
						getline(newF, line5, ',');
						getline(newF, line6, '\n');
						double hourUse = stod(line3) + stod(line6);
						pair<int, double> p;
						p.first = j;
						p.second = hourUse;
						vals[i].push_back(p);
					}
				}
			}
			else {
				for (int i = 0; i < done; i++) {
					for (int j = 0; j < 24; j++) {
						getline(newF, line1, ',');
						getline(newF, line2, ',');
						getline(newF, line3, '\n');
						double hourUse = stod(line3);
						pair<int, double> p;
						p.first = j;
						p.second = hourUse;
						vals[i].push_back(p);
					}
				}
			}
			newF.close();
		}
		root = insertNode(root, newNode, vals);
	}

}