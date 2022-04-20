#include<iostream>
#include<string>
#include<fstream>
#include <vector>
using namespace std;

struct item{
    pair<string,string> startEndDate;
    double wattage;
    item* next;

    item(): startEndDate("",""), wattage(0), next(nullptr){};
    item(pair<string,string> StartEndDate, double Wattage);
    //item(pair<string,string> StartEndDate, int Wattage) : startEndDate(StartEndDate), wattage(Wattage), next(nullptr){};
    item(item const &data) : startEndDate(data.startEndDate), wattage(data.wattage), next(data.next){};
};

item::item(pair<string, string> StartEndDate, double Wattage) {
    startEndDate = StartEndDate;
    wattage = Wattage;
    next = nullptr;

}

class HashTable{
private:
    int tableSize;
    int count;
    item** hashtable;

    int hash(pair<string,string> startEndDate);
    void tableDoubling();
    void rehash(int tableSize);

public:
    HashTable(){};
    HashTable(int num) : tableSize(num), count(0){
        hashtable = new item* [tableSize];
        for(int i = 0; i < tableSize; i++){
            hashtable[i] = nullptr;
        }
    }
    ~HashTable();

    void add(item data);
    void remove(pair<string,string> startEndDate);
    int search(pair<string,string> startEndDate);
    void display(string filename);
};

void HashTable::add(item data){
    count++;
    if(count > tableSize){
        tableDoubling();
    }
    int index = hash(data.startEndDate);
    item* newItem = new item(data);
    if(hashtable[index] == nullptr){
        hashtable[index] = newItem;
    }
    else{
        item* next = hashtable[index]->next;
        hashtable[index]->next = newItem;
        newItem->next = next;
    }
}
void HashTable::remove(pair<string,string> startEndDate){
    int index = hash(startEndDate);
    item* Ptr1 = hashtable[index];
    item* Ptr2 = nullptr;
    while(Ptr1 != nullptr && Ptr1->startEndDate != startEndDate){
        Ptr2 = Ptr1;
        Ptr1 = Ptr1->next;
    }
    if(Ptr1 == nullptr){
        cout << "Date Hour pair not found in the dataset\n";
        return;
    }
    else{
        if(Ptr2 == nullptr){
            hashtable[index] = Ptr1->next;
        }
        else{
            Ptr2->next = Ptr1->next;
        }
        delete Ptr1;
        Ptr1 = nullptr;
    }
}

int HashTable::search(pair<string, string> startEndDate) {
    int index = hash(startEndDate);
    item* Ptr = hashtable[index];
    while(Ptr != nullptr){
        if(Ptr->startEndDate == startEndDate){
            return Ptr->wattage;
        }
        Ptr = Ptr->next;
    }
    return -1;
}
int HashTable::hash(pair<string, string> startEndDate){
    string temp = startEndDate.first + startEndDate.second;
    int hash = 0;
    int index;
    for(char i : temp){
        hash = hash + (int) i;
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
    item** newTable = new item* [tableSize];
    for(int i = 0; i < tableSize; i++){
        newTable[i] = 0;
    }
    for(int i = 0; i < tableSize/2; i++){
        item* Ptr1 = hashtable[i];
        item* Ptr2 = hashtable[i];
        while(Ptr1 != nullptr){
            Ptr2 = Ptr1->next;
            int index = hash(Ptr1->startEndDate);
            if(newTable[index] == nullptr){
                newTable[index] = Ptr1;
                newTable[index]->next = nullptr;
            }
            else{
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
    for(int i = 0; i < tableSize; i++){
        item* Ptr1 = hashtable[i];
        while(Ptr1 != nullptr){
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
    for(int i = 0; i < tableSize; i++){
        out << "Bucket "<< i << ": ";
        item* Ptr1 = hashtable[i];
        while(Ptr1 != nullptr){
            out << "(" << Ptr1->startEndDate.first << " - " << Ptr1->startEndDate.second << ", " <<  Ptr1->wattage << "mW) ";
            Ptr1 = Ptr1->next;
        }
        out << endl;
    }
    out << endl;
    out.close();

}

vector<item> runfile(string path){
    vector<item> itemVector;
    itemVector.clear();
    ifstream file;
    file.open(path);
    if(!file.is_open()){
        cout << "Error: Could not open file " << path << endl;
        return itemVector;
    }
    string dataline;
    string testline;
    getline(file,testline);
    getline(file, dataline);
    string start, end, wattageSTR;
    start = dataline.substr(0, dataline.find(','));
    dataline = dataline.erase(0, dataline.find(',') +1);
    end = dataline.substr(0, dataline.find(','));
    dataline = dataline.erase(0, dataline.find(',') +1);
    wattageSTR = dataline;
    double wattage = stod(wattageSTR);
    while(!file.eof()){
        getline(file,dataline);
        dataline.substr(0, dataline.find(','));
        start = dataline.substr(0, dataline.find(','));
        dataline = dataline.erase(0, dataline.find(',') +1);
        end = dataline.substr(0, dataline.find(','));
        dataline = dataline.erase(0, dataline.find(',') +1);
        wattageSTR = dataline;
        if(wattageSTR != ""){
            wattage = stod(wattageSTR);
        }
        pair<string,string> temp(start,end);
        item Item(temp,wattage);
        itemVector.push_back(Item);
    }
    return itemVector;
}
int main(){
    HashTable Switzerland(9);
    vector<item> itemVector = runfile("../ch.csv");
    int y = 0;
    for(auto i : itemVector){
        y++;
        Switzerland.add(i);
    }
    Switzerland.display("Switzerland");

    HashTable Denmark(9);
    itemVector = runfile("../dk.csv");
    for(auto i : itemVector){
        y++;
        Denmark.add(i);
    }
    Denmark.display("Denmark");

    HashTable Spain(9);
    itemVector = runfile("../es.csv");
    for(auto i : itemVector){
        y++;
        Spain.add(i);
    }
    Spain.display("Spain");

    HashTable France(9);
    itemVector = runfile("../fr.csv");
    for(auto i : itemVector){
        y++;
        France.add(i);
    }
    France.display("France");

    HashTable Italy(9);
    itemVector = runfile("../it.csv");
    for(auto i : itemVector){
        y++;
        Italy.add(i);
    }
    Italy.display("Italy");

    HashTable Norway(9);
    itemVector = runfile("../no.csv");
    for(auto i : itemVector){
        y++;
        Norway.add(i);
    }
    Norway.display("Norway");

    HashTable Portugal(9);
    itemVector = runfile("../pt.csv");
    for(auto i : itemVector){
        y++;
        Portugal.add(i);
    }
    Portugal.display("Portugal");

    HashTable Sweden(9);
    itemVector = runfile("../se.csv");
    for(auto i : itemVector){
        y++;
        Sweden.add(i);
    }
    Sweden.display("Sweden");

    return 0;
}