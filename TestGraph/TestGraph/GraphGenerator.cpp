#include <cmath>
#include <ctime>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>
#include <queue>
#include <matplot/matplot.h>
#include <random>
#include <stdlib.h>
#include <fstream>
using namespace std;

/* In the map the int is hours after midnight */
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


struct item {
    string endDate;
    double wattage;
    item* next;

    string year;
    string month;
    string day;

    item() : endDate("", ""), wattage(0), next(nullptr) {};
    item(string EndDate, double Wattage);
    //item(pair<string,string> EndDate, int Wattage) : endDate(EndDate), wattage(Wattage), next(nullptr){};
    item(item const& data);
};

item::item(string EndDate, double Wattage) {
    endDate = EndDate;
    wattage = Wattage;
    next = nullptr;
    if (endDate != "") {
        year = EndDate.substr(0, 4);
        month = EndDate.substr(5, 2);
        day = EndDate.substr(8, 2);
    }
    else {
        year = "";
        month = "";
        day = "";
    }

}

item::item(const item& data) {
    endDate = data.endDate;
    wattage = data.wattage;
    next = data.next;

    if (data.endDate != "") {
        year = data.endDate.substr(0, 4);
        month = data.endDate.substr(5, 2);
        day = data.endDate.substr(8, 2);
    }
    else {
        year = "";
        month = "";
        day = "";
    }
}

class HashTable {
private:
    int tableSize;
    int count;
    item** hashtable;

    int hash(string endDate);
    void tableDoubling();
    void rehash(int tableSize);

public:
    HashTable() {};
    HashTable(int num) : tableSize(num), count(0) {
        hashtable = new item * [tableSize];
        for (int i = 0; i < tableSize; i++) {
            hashtable[i] = nullptr;
        }
    }
    ~HashTable();

    void add(item data);
    void remove(string endDate);
    //double search(string endDate);
    void display(string filename);
    vector<item*> searchYear(string year);
    vector<item*> searchMonth(string month);
    vector<item*> searchDay(string day);
    void displaySearchYear(string year);
    void displaySearchMonth(string month);
    void displaySearchDay(string day);
};

void HashTable::add(item data) {
    count++;
    if (count > tableSize) {
        tableDoubling();
    }
    int index = hash(data.endDate);
    item* newItem = new item(data);
    if (hashtable[index] == nullptr) {
        hashtable[index] = newItem;
    }
    else {
        item* next = hashtable[index]->next;
        hashtable[index]->next = newItem;
        newItem->next = next;
    }
}
void HashTable::remove(string endDate) {
    int index = hash(endDate);
    item* Ptr1 = hashtable[index];
    item* Ptr2 = nullptr;
    while (Ptr1 != nullptr && Ptr1->endDate != endDate) {
        Ptr2 = Ptr1;
        Ptr1 = Ptr1->next;
    }
    if (Ptr1 == nullptr) {
        cout << "Date Hour pair not found in the dataset\n";
        return;
    }
    else {
        if (Ptr2 == nullptr) {
            hashtable[index] = Ptr1->next;
        }
        else {
            Ptr2->next = Ptr1->next;
        }
        delete Ptr1;
        Ptr1 = nullptr;
    }
}
/*
double HashTable::search(string endDate) {
    int index = hash(endDate);
    item* Ptr = hashtable[index];
    while(Ptr != nullptr){
        if(Ptr->endDate == endDate){
            return Ptr->wattage;
        }
        Ptr = Ptr->next;
    }
    return -1;
}
*/
int HashTable::hash(string endDate) {
    string temp = endDate;
    int hash = 0;
    int index;
    for (char i : temp) {
        hash = hash + (int)i;
    }
    index = hash % tableSize;
    return index;
}
void HashTable::tableDoubling() {
    int oldSize = tableSize;
    tableSize *= 2;
    rehash(tableSize);
}
void HashTable::rehash(int tableSize) {
    item** newTable = new item * [tableSize];
    for (int i = 0; i < tableSize; i++) {
        newTable[i] = 0;
    }
    for (int i = 0; i < tableSize / 2; i++) {
        item* Ptr1 = hashtable[i];
        item* Ptr2 = hashtable[i];
        while (Ptr1 != nullptr) {
            Ptr2 = Ptr1->next;
            int index = hash(Ptr1->endDate);
            if (newTable[index] == nullptr) {
                newTable[index] = Ptr1;
                newTable[index]->next = nullptr;
            }
            else {
                item* next = newTable[index]->next;
                newTable[index]->next = Ptr1;
                Ptr1->next = next;
            }
            Ptr1 = Ptr2;
        }
    }
    delete[] hashtable;
    this->hashtable = newTable;
}
HashTable::~HashTable() {
    for (int i = 0; i < tableSize; i++) {
        item* Ptr1 = hashtable[i];
        while (Ptr1 != nullptr) {
            item* Ptr2 = Ptr1;
            Ptr1 = Ptr1->next;
            delete Ptr2;
            Ptr2 = nullptr;
        }
    }
    delete[] hashtable;
}
void HashTable::display(string filename) {
    ofstream out(filename);
    for (int i = 0; i < tableSize; i++) {
        out << "Bucket " << i << ": ";
        item* Ptr1 = hashtable[i];
        while (Ptr1 != nullptr) {
            out << "(" << Ptr1->endDate << ", " << Ptr1->wattage << "mW) ";
            Ptr1 = Ptr1->next;
        }
        out << endl;
    }
    out << endl;
    out.close();

}

vector<item*> HashTable::searchYear(string year) {
    vector<item*> itemVector;
    int index = 0;
    /* when searching for a full tuple, we use the end date. the tuples are hashed by full date and time.
     * searching for a year only means we only want one part of the hash input. theyre sorted by full date
     * and time so the hash function will not put us back in the exact same spot as the tuple landed. this means
     * we have to start searching from the beginning instead of closer to where the tuple is guaranteed to be.
     */
    for (int i = 0; i < this->tableSize; i++) {
        index = i;
        item* Ptr = hashtable[index];
        while (Ptr != nullptr) {
            if (Ptr->year == year) {
                itemVector.push_back(Ptr);
            }
            Ptr = Ptr->next;
        }
    }
    return itemVector;
}
vector<item*> HashTable::searchMonth(string month) {
    vector<item*> itemVector;
    int index = 0;
    for (int i = 0; i < this->tableSize; i++) {
        index = i;
        item* Ptr = hashtable[index];
        while (Ptr != nullptr) {
            if (Ptr->month == month) {
                itemVector.push_back(Ptr);
            }
            Ptr = Ptr->next;
        }
    }
    return itemVector;
}
vector<item*> HashTable::searchDay(string day) {
    vector<item*> itemVector;
    int index = 0;
    for (int i = 0; i < this->tableSize; i++) {
        index = i;
        item* Ptr = hashtable[index];
        while (Ptr != nullptr) {
            if (Ptr->day == day) {
                itemVector.push_back(Ptr);
            }
            Ptr = Ptr->next;
        }
    }
    return itemVector;
}

void HashTable::displaySearchYear(string year) {
    vector<item*> temp = searchYear(year);
    for (auto i : temp) {
        cout << i->wattage << endl;
    }
}

void HashTable::displaySearchMonth(string month) {
    vector<item*> temp = searchYear(month);
    for (auto i : temp) {
        cout << i->wattage << endl;
    }
}

void HashTable::displaySearchDay(string day) {
    vector<item*> temp = searchYear(day);
    for (auto i : temp) {
        cout << i->wattage << endl;
    }
}

vector<item> runfile(string path) {
    vector<item> itemVector;
    itemVector.clear();
    ifstream file;
    file.open(path);
    if (!file.is_open()) {
        cout << "Error: Could not open file " << path << endl;
        return itemVector;
    }
    string dataline;
    string testline;
    getline(file, testline);
    getline(file, dataline);
    string start, end, wattageSTR;
    start = dataline.substr(0, dataline.find(','));
    dataline = dataline.erase(0, dataline.find(',') + 1);
    end = dataline.substr(0, dataline.find(','));
    dataline = dataline.erase(0, dataline.find(',') + 1);
    wattageSTR = dataline;
    double wattage = stod(wattageSTR);
    while (!file.eof()) {
        getline(file, dataline);
        dataline.substr(0, dataline.find(','));
        start = dataline.substr(0, dataline.find(','));
        dataline = dataline.erase(0, dataline.find(',') + 1);
        end = dataline.substr(0, dataline.find(','));
        dataline = dataline.erase(0, dataline.find(',') + 1);
        wattageSTR = dataline;
        if (wattageSTR != "") {
            wattage = stod(wattageSTR);
        }
        string temp = end;
        item Item(temp, wattage);
        itemVector.push_back(Item);
    }
    return itemVector;
}

int main() {
    std::string country, timeframe, day, month, year, ds, cont;
    bool run = true;

    //While loop will allow the user to make multiple plots.
    while (run)
    {
        // User Selection Menu
        std::cout << "In order to choose multiple countries type the country name followed by a space." << std::endl;
        std::cout << "Name of country: ";
        std::cin >> country;
        std::cout << "Timeframe (day, month, or year): ";
        std::cin >> timeframe;
        if (timeframe == "day")
        {
            std::cout << "Month: ";
            std::cin >> month;
            std::cout << "Day: ";
            std::cin >> day;
        }
        else if (timeframe == "month")
        {
            std::cout << "Month: ";
            std::cin >> month;
        }
        else if (timeframe == "year")
        {
            std::cout << "Year: ";
            std::cin >> year;
        }

        std::cout << "Data Structure to Use (hashmap or b-tree): ";
        std::cin >> ds;
        std::system("CLS");

        if (timeframe == "year")
        {
            if (ds == "hashmap")
            {
                // Create hash map
                HashTable hash(9);
                vector<item> itemVector = runfile(country + ".csv");
                
                for (auto i : itemVector)
                {
                    hash.add(i);
                }

                std::vector<double> y;

                //hash.displaySearch();
                for (int i = 1; i < 13; i++) 
                {
                    double sum = 0;
                    for (auto j : hash.searchYear(year))
                    {
                        int curr = stoi(j->month);
                        if (curr == i)
                            sum += j->wattage;
                    }
                    y.push_back(sum);
                }

                matplot::bar(y);
                matplot::gca()->x_axis().ticklabels({ "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" });
                matplot::ylabel("Power Consumption(kWh)");
                matplot::xlabel("Months of the Year");
                matplot::title("Power Consumption of " + country + " in " + year);
                matplot::show();
            }
            else if (ds == "b-tree")
            {
                std::vector<double> y = { 10, 21, 33, 52, 28, 61, 76, 89, 11, 10, 58, 72 };
                matplot::bar(y);
                matplot::gca()->x_axis().ticklabels({ "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" });
                matplot::ylabel("Power Consumption(kWh)");
                matplot::xlabel("Months of the Year");
                matplot::title("Power Consumption of " + country + " in " + year);
                matplot::show();
            }
        }

        else if (timeframe == "month")
        {
            if (ds == "hashmap")
            {
                HashTable hash(1);
                vector<item> itemVector = runfile(country + ".csv");

                for (auto i : itemVector)
                {
                    hash.add(i);
                }
                std::vector<double> x = { 2015, 2016, 2017, 2018, 2019, 2020 };
                std::vector<double> y;

                //hash.displaySearchMonth()

                for (int i = 2015; i < 2021; i++)
                {
                    double sum = 0;
                    for (auto j : hash.searchMonth(month))
                    {
                        int curr = stoi(j->year);
                        if (curr == i)
                            sum += j->wattage;
                    }
                    y.push_back(sum);
                }
                matplot::bar(x,y);
                matplot::ylabel("Power Consumption(kWh)");
                matplot::xlabel("Years");
                matplot::title("Power Consumption of " + country + " in Month " + month + " Across All Years");
                matplot::show();
            }
            else if (ds == "b-tree")
            {
                std::vector<double> y = { 10, 21, 33, 52, 28, 61, 76, 89, 11, 10, 58, 72 };
                matplot::bar(y);
                matplot::ylabel("Power Consumption(kWh)");
                matplot::xlabel("Days of the Month");
                matplot::title("Power Consumption of " + country + " in Month " + month + " of " + year);
                matplot::show();
            }
        }

        //else if (timeframe == "day")
        //{
        //    if (ds == "hashmap")
        //    {
        //        std::vector<double> x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,13,14,15,16,17,18,19,20,21,22,23 };
        //        HashTable hash(9);
        //        vector<item> itemVector = runfile(country + ".csv");

        //        for (auto i : itemVector)
        //        {
        //            hash.add(i);
        //        }

        //        std::vector<double> y;

        //        //hash.displaySearch();

        //        for (int i = 1; i < 13; i++)
        //        {
        //            double sum = 0;
        //            for (auto j : hash.searchYear(year))
        //            {
        //                int curr = stoi(j->month);
        //                if (curr == i)
        //                    sum += j->wattage;
        //            }
        //            y.push_back(sum);
        //        }
        //        matplot::bar(x,y);
        //        matplot::ylabel("Power Consumption(kWh)");
        //        matplot::xlabel("Hours of the Day");
        //        matplot::title("Power Consumption of " + country + " in " + month + "/" + day + "/" + year);
        //        matplot::show();
        //    }
        //    else if (ds == "b-tree")
        //    {
        //        std::vector<double> x = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23 };
        //        std::vector<double> y = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,13,14,15,16,17,18,19,20,21,22,23,24 };
        //        matplot::bar(x,y);
        //        matplot::ylabel("Power Consumption(kWh)");
        //        matplot::xlabel("Hours of the Day");
        //        matplot::title("Power Consumption of " + country + " in " + month + "/" + day + "/" + year);
        //        matplot::show();
        //    }
        //}

        std::system("CLS");
        std::system("CLS");
        std::cout << "Would you like to create another graph? (y/n): ";
        std::cin >> cont;
        if (cont == "y")
            run = true;
        else
            run = false;
    }
   
    
    return 0;
}