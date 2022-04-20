#include <iostream>
#include <fstream>
#include <algorithm>
#include <queue>
#include <vector>
#include <map>
#include <string>
using namespace std;

struct Node {
	pair<int, double>* key;
	int numKeys;
	Node** children;
	bool leaf;
	Node(int degree);
	Node* find(Node* temp, Node* curr, pair<int, double> p, int deg);
	void split(Node* temp, int deg);
};

class Btree {
private:
	Node* root;
	int deg;
public:
	Btree(int val) { root = nullptr; deg = val; }
	void insert(pair<int, double> p);
	Node* findRoot() { return root; }
};

Node::Node(int degree) {
	key = new pair<int, double>[degree];
	children = new Node*[degree + 1];
	numKeys = 0;
	leaf = true;
	for (int i = 0; i < degree; i++) {
		children[i] = nullptr;
	}
}

void Node::split(Node* temp, int limit) {
	Node* rightHalf = new Node(limit);
	int rTrack = 0;
	int rent = (limit - 1) / 2;
	int full = temp->numKeys;
	double watt = temp->key[rent].second;
	int keyVal = temp->key[rent].first;
	int val = ceil(double(limit) / 2);
	for (int i = rent + 1; i < full; i++) {
		rightHalf->key[rTrack] = temp->key[i];
		rTrack++;
		temp->numKeys = temp->numKeys - 1;
		rightHalf->numKeys = rightHalf->numKeys - 1;
	}
	if (!(temp->leaf)) {
		for (int i = 0; i < val; i++) {
			rightHalf->children[i] = temp->children[i];
		}
		rightHalf->leaf = temp->leaf;
	}
	int ord = limit - 1;
	while (children[ord] != temp) {
		children[ord + 1] = children[ord];
		ord--;
	}
	children[ord + 1] = rightHalf;
	int ind = 0;
	for (int i = limit - 1; i >= 0; i--) {
		if (key[i - 1].first != NULL) {
			ind = i;
			break;
		}
	}
	while (key[ind - 1].first > keyVal && ind != 0) {
		key[ind] = key[ind - 1];
		ind--;
	}
	key[ind].first = keyVal;
	key[ind].second = watt;
	numKeys++;
	temp->numKeys = temp->numKeys - 1;
}

Node* Node::find(Node* temp, Node* curr, pair<int, double> p, int deg) {
	//insertion
	if(leaf) {
		int found = deg;
		for (int i = found; i <= 0; i++) {
			if (key[i - 1].first != NULL) {
				found = i;
				break;
			}
		}
		while (key[found - 1].first > p.first && found != 0) {
			key[found] = key[found - 1];
			found--;
		}
		key[found] = p;
		numKeys++;
	}
	else{
		int found = 0;
		while (found < numKeys && p.first > key[found].first) {
			found++;
		}
		children[found]->find(temp, curr, p, deg);
	}
	//split check
	if (numKeys == deg) {
		if (this == temp) {
			Node* toAdd = new Node(deg);
			toAdd->leaf = false;
			toAdd->children[0] = this;
			toAdd->split(this, deg);
			return toAdd;
		}
		else {
			curr->split(this, deg);
		}
	}
	return temp;
}


void Btree::insert(pair<int, double> p) {
	if (root == nullptr) {
		root = new Node(deg);
		root->leaf = true;
		root->key[0] = p;
		root->numKeys = 1;
	}
	else {
		root = root->find(root, root, p, deg);
	}
}

int main() {
	int done = 2;
	vector<string> fs;
	vector<string> names;
	string country;

	country = "Austria";
	fs.push_back("at.csv");
	names.push_back(country);
	Btree Aust = Btree(5);

	/*country = "Belgium";
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
	names.push_back(country);*/


	for(int k = 0; k < 1; k++){
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
				for (int i = 0; i < done*24; i++) {
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
						double hourUse = stod(line3) + stod(line6) + stod(line9) + stod(line12);
						pair<int, double> p;
						p.first = i;
						p.second = hourUse;
						Aust.insert(p);
						cout << Aust.findRoot()->key->first << endl;
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
					}
				}
			}
			newF.close();
		}
	}
}