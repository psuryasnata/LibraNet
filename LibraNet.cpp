#include <bits/stdc++.h>
using namespace std;

class LibraNet
{
protected:
    int id;
    string title;
    string author;
    bool available;

public:
    LibraNet(int i, string t, string a){
        id = i;
        title = t;
        author = a;
        available = true;
    }

    int getId(){
        return id;
    }

    string getTitle(){
        return title;
    }

    bool isAvailable(){
        return available;
    }

    void setAvailable(bool val){
        available = val;
    }

    virtual void showinfo(){
        cout << id << " :- " << title << " by " << author;
        if (available){
            cout << " [Available]" << endl;
        }else{
            cout << " [Borrowed] " << endl;
        }
    }
    virtual ~LibraNet(){
    }
};


class Book : public LibraNet {
    int pageCount;
public:
    Book(int id, string t, string a, int pages) : LibraNet(id, t, a){
        pageCount = pages;
    }

    int getpageCount(){
        return pageCount; 
    }

    void showinfo() override {
        LibraNet::showinfo();
        cout << "Pages: " << pageCount << endl;
    }
};

class Playable{
public:
    virtual void play() = 0;
    virtual void pause() = 0;
};

class Audiobook : public LibraNet, public Playable{
    int duration;
public:
    Audiobook(int id, string t, string a, int d) : LibraNet(id, t, a) {
        duration = d;
    }

    void play() override{
        cout << "Playing audiobook: " << title << endl; 
    }

    void pause() override{
        cout << "Last Played audiobook: " << title << endl; 
    }

    void showinfo() override{
        LibraNet::showinfo();
        cout << "duration: " << duration << " mins" << endl;
    }
};

class Emagazine : public LibraNet{
    int issue;
public:
    Emagazine(int id, string t, string a, int i) : LibraNet(id, t, a){
        issue = i;
    }

    void archiveIssue(){
        setAvailable(false);
        cout << "Archived issue " << issue << " of " << title << endl;
    }

    void showinfo() override{
        LibraNet::showinfo();
        cout << "Issue No: " << issue << endl;
    }
};

struct BorrowRecord{
    int itemId;
    string user;
    time_t borrowDate;
    int durationDays;
};

class LibraryManager{
    map<int, LibraNet*> items;
    map<int, BorrowRecord> borrows;

public:
    void addItem(LibraNet* item){
        items[item->getId()] = item;
    }

    void borrowItem(int id, string user, int days){
        if (items.find(id) == items.end()){
            cout << "No such item!" << endl;
            return;
        }

        LibraNet* item = items[id];
        if (!item->isAvailable()){
            cout << "Item already borrowed!" << endl;
            return;
        }

        item->setAvailable(false);
        BorrowRecord rec ={
            id,
            user,
            time(NULL), 
            days};
        borrows[id] = rec;
        cout << user << " borrowed " << item->getTitle() << " for " << days << " days." <<endl;
    }

    void returnItem(int id){
        if (borrows.find(id) == borrows.end()) {
            cout << "No borrow record found!" << endl;
            return;
        }

        BorrowRecord rec = borrows[id];
        LibraNet* item = items[id];
        item->setAvailable(true);


        int fineperDay = 10; 
        time_t now = time(NULL);                     
        double secondsDiff = difftime(now, rec.borrowDate);  
        int daysPassed = (int)(secondsDiff / (60 * 60 * 24));

        int late = daysPassed - rec.durationDays;
        
        int fine;
        if (late > 0){
        fine = late * fineperDay;
        }else{
        fine = 0;
        }

        cout << "Returned " << item->getTitle() << ". Fine = Rs " << fine << endl;
        borrows.erase(id);
    }

    void showAll(){
        for (auto c : items){
            c.second->showinfo();
        }
    }
};


int main(){
    LibraryManager lib;

    Book* b1 = new Book(1, "War and Peace", "Leo Tolstoy", 300);
    Audiobook* a1 = new Audiobook(2, "Atomic Habits", "James Clear", 400);
    Emagazine* e1 = new Emagazine(3, "Tech Times Today", "Editor", 41);
    Book* b2 = new Book(4, "The Great Gatsby", "F. Scott Fitzgerald", 180);
    Book* b3 = new Book(5, "Moby Dick", "Herman Melville", 635);
    Book* b4 = new Book(6, "To Kill a Mockingbird", "Harper Lee", 281);

    lib.addItem(b1);
    lib.addItem(a1);
    lib.addItem(e1);
    lib.addItem(b2);
    lib.addItem(b3);
    lib.addItem(b4);

    
    lib.borrowItem(1, "Soumya", 2);
    lib.returnItem(1);


    cout << "\nBehaviors:- " << endl;
    cout << "Pages in book: " << b1->getpageCount() << endl;
    a1->play();
    e1->archiveIssue();

    cout << "\nAll Items:- " <<endl;
    lib.showAll();
    return 0;
}