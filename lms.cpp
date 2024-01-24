#include <bits/stdc++.h>
#include <ctime>
#include <chrono>
#include <thread>  
#include <limits> 
#include <cstdlib> 
using namespace std;
#define ar array
#define ll long long
#define ld long double
#define sza(x) ((int)x.size())
#define all(a) (a).begin(), (a).end()
#define endl '\n'
#define pb push_back
const int MAX_N = 1e5 + 5;
const ll MOD = 1e9 + 7;
const ll INF = 1e18;
const ld EPS = 1e-9;
class BookDetails;
class LibraryPatron;
class LibraryTransaction{
private:
    // LibraryPatron* patron;
    BookDetails* book;
    time_t date;
    time_t dueDate;
    bool Late;
public:
    LibraryTransaction(BookDetails *b, time_t t){
        // patron = pat;
        book = b;
        date = t;
        const int days = 14;//book holding time period
        int conv = days*24*60*60;
        dueDate = date + conv;
    }
    // void DueDate(){
    //     const int days = 14;
    //     dueDate = date + (days*24*60*60);
    // }
    const time_t& getDueDate() const{
        return dueDate;
    }
    void checkLate(bool yep){
        Late = yep;
    }
    bool valLate() const{
        return Late;
    }
};
class BookDetails{
private:
    string title;
    string author;
    int publicationYear;
    int availableCopies;
    int totalCopies;
    bool reserved;
    vector<LibraryTransaction> transactions;
public:
    BookDetails(const string t, const string a, int py, int tc){
        title = t;
        author = a;
        publicationYear=py;
        totalCopies=tc;
        availableCopies = tc;
        reserved=false;
    }
    void checkout(time_t transactionDate){
        if(availableCopies){
            availableCopies--;
            LibraryTransaction transaction(this, transactionDate);
            transactions.pb(transaction);
            cout<<"Book Checked out: " <<title<<endl;
            cout<<"Due Date: "<<ctime(&transaction.getDueDate());
        }
        else cout<<"All copies already issued."<<endl;
    }
    void checkin(){
        if(availableCopies<totalCopies) availableCopies ++;
        else cout<<"Invalid."<<endl;
        cout<<"Book Checked in: "<<title<<endl;
    }
    const string &getTitle() const{
        return title;
    }
    const string &getAuthor() const{
        return author;
    }
    int getPublicationYear() const{
        return publicationYear;
    }
    int getAvailableCopies() const{
        return availableCopies;
    }
    int getTotalCopies() const{
        return totalCopies;
    }
    void ReserveBook(){
        if(availableCopies){
            if(!reserved){
                reserved = true;
                cout<<"Book reserved: "<<title<<endl;
            }
            else{
                cout<<"Reservation isn't needed. Book is available."<<endl;
            }
        }
        else{
            cout<<"Book is not available currently for issue."<<endl;
        }
    }
    bool isReserve() const{
        return reserved;
    }
    const vector<LibraryTransaction>& getTransactions() const{
        return transactions;
    }

};
class LibraryPatron{
private:
    int patronID;
    string name;
    vector<BookDetails*> checkedOutBooks;
    vector<LibraryTransaction> transactions;
public:
    LibraryPatron(int pI, const string &n){
        patronID = pI;
        name = n;
    }
    // LibraryPatron(int patronID, const std::string& name)
    //     : patronID(patronID), name(name)
    // {}
    void CheckOutBook(BookDetails &book, time_t transactionDate){
        // cout<<checkedOutBooks.size()<<endl;
        checkedOutBooks.pb(&book);
        LibraryTransaction transaction(&book, transactionDate);
        transactions.pb(transaction);
        // cout<<"Due Date: "<<ctime(&transaction.getDueDate());
        book.checkout(transactionDate);
        // cout<<checkedOutBooks.size()<<endl;
    }
    void ReturnBook(BookDetails &book, time_t returnDate){
        // cout<<checkedOutBooks.size()<<endl;
        if(find(checkedOutBooks.begin(),checkedOutBooks.end(),&book)!=checkedOutBooks.end()){
            checkedOutBooks.erase(find(checkedOutBooks.begin(),checkedOutBooks.end(),&book));
            // cout<<checkedOutBooks.size()<<endl;
            (book).checkin();
            LibraryTransaction& transaction = transactions[sza(transactions)-1];
            // cout<<"--"<<endl;
            // cout<<name<<endl;
            // cout<<ctime(&returnDate)<<endl;
            // cout<<ctime(&transaction.getDueDate())<<endl;
            if(transaction.getDueDate()<returnDate){
                transaction.checkLate(true);
                int daysLate = (returnDate-transaction.getDueDate())/(24*60*60);
                cout<<"Book was returned late. Fine: Rs."<<daysLate*2<<endl;
            }
        }
        else cout<<"Book wasn't checked out."<<endl;
    }
    void ReserveBook(BookDetails& book){
        book.ReserveBook();
    }
    const string& getName() const{
        return name;
    }

};

//----------------------For Display part -------------------------------------
class ShowLib {
private:
    string branch;
    vector<BookDetails*> books;
public:
    ShowLib(const string& branch) : branch(branch) {}

    void AddBook(BookDetails* book) {
        books.pb(book);
    }

    void DisplayBookList() const {
        cout << "Books available in " << branch << " branch:" << endl;
        for (const auto& book : books) {
            cout << "- " << book->getTitle() << " by " << book->getAuthor();
            if (book->isReserve()) {
                cout << " (Reserved)";
            }
            cout << endl;
        }
    }

};

//------------------------Test driver code ----------------------------
int main() {
    time_t currentTime = time(0);

    BookDetails book1("Beautiful Things", "Hunter Biden", 2021, 1);
    BookDetails book2("The Audacity Of Hope", "Barack Obama", 2006, 3);
    BookDetails book3("Ajatshatru","Jay Shankar Prasad",2020,5);
    ShowLib branch("Bookshelf");    
    branch.AddBook(&book1);
    branch.AddBook(&book2);
    branch.AddBook(&book3);
    branch.DisplayBookList();
    LibraryPatron patron1(42, "Patron1");
    LibraryPatron patron2(69, "Patron2");
    patron1.CheckOutBook(book1, currentTime);
    patron2.CheckOutBook(book1, currentTime);
    // this_thread::sleep_for(chrono::seconds(10));
    time_t returnTime = currentTime +604800*27; 
    patron1.ReturnBook(book1, returnTime);
    patron1.ReserveBook(book3);
    patron2.ReserveBook(book3);
    branch.DisplayBookList();
    return 0;
}