#include<iostream>
#include<string>
#include<fstream>
#include <vector>
using namespace std;

struct item{
    string endDate;
    double wattage;
    item* next;

    string year;
    string month;
    string day;

    item(): endDate("",""), wattage(0), next(nullptr){};
    item(string EndDate, double Wattage);
    //item(pair<string,string> EndDate, int Wattage) : endDate(EndDate), wattage(Wattage), next(nullptr){};
    item(item const &data);
};

item::item(string EndDate, double Wattage) {
    endDate = EndDate;
    wattage = Wattage;
    next = nullptr;
    if(endDate != ""){
        year = EndDate.substr(0,4);
        month = EndDate.substr(5, 2);
        day = EndDate.substr(8, 2);
    }
    else{
        year = "";
        month = "";
        day = "";
    }

}

item::item(const item &data) {
    endDate = data.endDate;
    wattage = data.wattage;
    next = data.next;

    if(data.endDate != ""){
        year = data.endDate.substr(0,4);
        month = data.endDate.substr(5, 2);
        day = data.endDate.substr(8, 2);
    }
    else{
        year = "";
        month = "";
        day = "";
    }
}

class HashTable{
private:
    int tableSize;
    int count;
    item** hashtable;

    int hash(string endDate);
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
    void remove(string endDate);
    //double search(string endDate);
    void display(string filename);
    vector<item*> searchYear(string year);
    vector<item*> searchMonth(string month);
    vector<item*> searchDay(string day);
    void displaySearch();
};

void HashTable::add(item data){
    count++;
    if(count > tableSize){
        tableDoubling();
    }
    int index = hash(data.endDate);
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
void HashTable::remove(string endDate){
    int index = hash(endDate);
    item* Ptr1 = hashtable[index];
    item* Ptr2 = nullptr;
    while(Ptr1 != nullptr && Ptr1->endDate != endDate){
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
int HashTable::hash(string endDate){
    string temp = endDate;
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
            int index = hash(Ptr1->endDate);
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
            out << "(" << Ptr1->endDate << ", " <<  Ptr1->wattage << "mW) ";
            Ptr1 = Ptr1->next;
        }
        out << endl;
    }
    out << endl;
    out.close();

}

vector<item*> HashTable::searchYear(string year) {
    //string date = year + "-01-01 00:00:00+00:00";
    vector<item*> itemVector;
    //itemVector.clear();
    int index = 0;
    /* when searching for a full tuple, we use the end date. the tuples are hashed by full date and time.
     * searching for a year only means we only want one part of the hash input. theyre sorted by full date
     * and time so the hash function will not put us back in the exact same spot as the tuple landed. this means
     * we have to start searching from the beginning instead of closer to where the tuple is guaranteed to be.
     */
    for(int i =0; i < this->tableSize; i++){
        index = i;
        item* Ptr = hashtable[index];
        while(Ptr != nullptr){
            if(Ptr->year == year){
                itemVector.push_back(Ptr);
            }
            Ptr = Ptr->next;
        }
    }
    return itemVector;
    /*
    item* Ptr = hashtable[index];
    while(Ptr != nullptr){
        if(Ptr->year == year){
            itemVector.push_back(Ptr);
        }
        Ptr = Ptr->next;
    }
     */
}
vector<item*> HashTable::searchMonth(string month) {
    vector<item*> itemVector;
    itemVector.clear();
    int index = 0;
    item* Ptr = hashtable[index];
    while(Ptr != nullptr){
        if(Ptr->month == month){
            itemVector.push_back(Ptr);
        }
        Ptr = Ptr->next;
    }
    return itemVector;
}
vector<item*> HashTable::searchDay(string day) {
    vector<item*> itemVector;
    itemVector.clear();
    int index = 0;
    item* Ptr = hashtable[index];
    while(Ptr != nullptr){
        if(Ptr->day == day){
            itemVector.push_back(Ptr);
        }
        Ptr = Ptr->next;
    }
    return itemVector;
}

void HashTable::displaySearch() {
    vector<item *> temp = searchYear("2020");
    for(auto i: temp){
        cout <<i->wattage << endl;
    }
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
        string temp = end;
        item Item(temp,wattage);
        itemVector.push_back(Item);
    }
    return itemVector;
}
int main(){

    //testing how to delimit end date into year month and date
    //string temp = "2020-01-04 00:00:00+00:00";
    //string year = temp.substr(0,temp.find('-'));
    //cout << year; good
    //string month = temp.substr(temp.find('-')+1, temp.find('-')-2);
    //cout << month << "\n"; good
    //string day = temp.substr(temp.find_last_of('-')+1);
    //string year = temp.substr(0,4);
    //cout << year << endl;
    //string month = temp.substr(5,2);
    //cout << month << endl;
    //string day = temp.substr(8,2);
    //cout << day << endl;

    //day = temp.substr(0,1);//temp.substr(temp.find('-')+4, temp.find_last_of('-'));
    //cout << day << "\n";

    HashTable Switzerland(9);
    vector<item> itemVector = runfile("../ch.csv");
    int y = 0;
    for(auto i : itemVector){
        y++;
        Switzerland.add(i);
    }

    Switzerland.displaySearch();
    /*
    vector<item*> pointers = Switzerland.searchYear("2020"); // put all years 2020 in item* vector
    for(auto i: pointers){
        cout << i->wattage << "hello" << endl;
    }
     */

    //Switzerland.display("Switzerland");
/*
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

*/
    return 0;
}