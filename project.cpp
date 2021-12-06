#include <iostream> 
#include <fstream> 
#include <string>
#include <limits>
#include <math.h>
#include <sstream> 
#include <Windows.h>
using namespace std;


#define SIZEMAX 10500
const int MAX_STRING_LENGTH = 1024;
int g_m = 0;


class dataNode
{
private:
    string path = "\0";
    int line = 0;
    string txtpath = "\0";
    dataNode* nextptr;
    dataNode* preptr;

public:
    dataNode() //default constructor
    {
        path = txtpath = "\0";
        line = -1;
        nextptr = preptr = NULL;
    }

    dataNode(int l, string p, string t) //parameterized constructor
    {
        path = p;
        txtpath = t;
        line = l;
        nextptr = preptr = NULL;
    }

    dataNode(const dataNode& obj) //copy constructor
    {
        path = obj.path;
        line = obj.line;
        txtpath = obj.txtpath;
        nextptr = obj.nextptr;
        preptr = obj.preptr;
    }

    dataNode& operator=(const dataNode& obj) //overloaded operator =
    {
        path = obj.path;
        line = obj.line;
        txtpath = obj.txtpath;
        nextptr = obj.nextptr;
        preptr = obj.preptr;
        return *this;
    }

    string gettxtpath()
    {
        return txtpath;
    }

    ~dataNode() {} //Destructor

    friend class my_data; //defining friend class so that access to private class members is possible
};

class my_data //linked list to store file path and line number
{
private:
    dataNode* head;
    dataNode* tail;
    int count;

public:
    my_data() //default constructor
    {
        head = tail = NULL;
        count = 0;
    }

    my_data(const my_data& obj) //parameterized constructor
    {
        head = obj.head;
        tail = obj.tail;
        count = obj.count;
    }

    ~my_data()
    {} //Destructor

    dataNode* get_head()
    {
        return head;
    }

    void setnull() //sets head and tail of doubly linked list to NULL
    {
        head = tail = NULL;
        count = 0;
    }

    my_data& operator=(const my_data& obj) //overloaded = operator
    {
        dataNode* temp2 = obj.head;
        while (temp2 != NULL)
        {
            insert(temp2->line, temp2->path, temp2->txtpath);
            temp2 = temp2->nextptr;
        }
        count = obj.count;
        return *this;
    }

    void insert(int l, string p, string t) //inserting in linked list
    {
        dataNode* temp = new dataNode(l, p, t);
        if (!head) //if head is NULL
        {
            head = temp;
            tail = temp;
        }
        else //if head is not NULL
        {
            dataNode* curr = new dataNode(l, p, t);
            curr = head;
            while (curr != NULL && curr->nextptr != NULL)
            {
                curr = curr->nextptr;
            }
            temp->nextptr = NULL;
            tail->nextptr = temp;
            temp->preptr = tail;
            tail = temp;

        }
        count++;
    }

    void remove(int l, string p, string t) //deleting some element from linked list
    {
        if (head) //if head is not NULL i-e linked list is not empty
        {
            dataNode* current = head;
            dataNode* prev = current;
            dataNode* temp;
            while (current != NULL && (current->line != l || current->path != p))
            {
                prev = current; //finds previous node to the one we need to delete
                current = current->nextptr;

            }
            if (current->nextptr != NULL) //if node to be deleted is the last one in the linked list
            {
                temp = current;
                prev->nextptr = current->nextptr;
                current->nextptr->preptr = prev;
                delete temp;
                count--;
            }
            else
            {
                prev->nextptr = NULL;
                current->preptr = NULL;
                delete current;
            }
        }

    }

    bool isEmpty() //checks if linked list is empty
    {
        if (!head)
        {
            return true;
        }
        return false;
    }

    string display() //displays all elements in linked list
    {
        string path = "\0";
        if (head)
        {
            dataNode* tempnode = head;
            while (tempnode)
            {
                ifstream file;
                file.open(tempnode->txtpath);
                if (file.good())
                {
                    string x;
                    cout << "\nPath: " << tempnode->path << "\nLine: " << tempnode->line << "\nText File path: " << tempnode->txtpath;
                    getline(file, x);
                    cout << "\n1. ID: " << x;
                    getline(file, x);
                    cout << "\n2. Year: " << x;
                    getline(file, x);
                    cout << "\n3. 113 Cause Name: " << x;
                    getline(file, x);
                    cout << "\n4. Cause: " << x;
                    getline(file, x);
                    cout << "\n5. State: " << x;
                    getline(file, x);
                    cout << "\n6. Deaths: " << x;
                    getline(file, x);
                    cout << "\n7. Age Adjusted Death Rate: " << x;
                    cout << "\n\n";
                    file >> path;
                }
                else
                {
                    cout << "\nPath: " << tempnode->path << "\nLine: " << tempnode->line << "\nText File path: " << tempnode->txtpath;
                    cout << "\nCould not open text file\n";
                }
                file.close();
                if (tempnode->nextptr)
                {
                    if (tempnode->nextptr->line == tempnode->line && tempnode->nextptr->txtpath == tempnode->txtpath)
                    {
                        return path;
                    }
                    tempnode = tempnode->nextptr;
                }
                else
                {
                    break;
                }                
            }
            return path;
        }
        else
        {
            cout << "\nError\nLinked list is empty\n";
            return path;
        }
    }
};

template<class T> class DSDB; //forward declaration so that friend declaration of template class is possible
template <class T>class Node
{
private:
    bool leaf = 0; //shows if that particular node is a leaf node or not
    int size = 0;
    my_data* data; //linked list to store data details
    T* key;
    Node<T>** child; //pointer of pointers to store children of node
    Node<T>* next;
    Node<T>* prev;

public:
    Node<T>() //default constructor
    {
        leaf = 0;
        size = 0;
        key = new T[g_m - 1];
        data = new my_data[g_m - 1];
        child = new Node<T> * [g_m];
        prev = next = NULL;
    }

    friend class DSDB<T>; //friend class declaration so that access to private members is possible
};


template <class T> class DSDB //defining B+ tree class
{
private:
    Node<T>* root;

public:
    DSDB<T>() //default constructor
    {
        cout << "\nEnter order (m) of B+ Tree\n";
        cin >> g_m;
        while (g_m <= 3 || g_m > 10000 || cin.fail())
        {
            if (g_m >= 5000)
            {
                cout << "\nError\nValue of 'm' must be an integer value that is greater than or equal to 3, and less than or equal to 10,000\nPlease enter the value of 'm' again\n";
            }
            else
            {
                cout << "\nError\nValue of 'm' must be an integer value that is greater than or equal to 3\nPlease enter the value of 'm' again\n";
            }
            cin.clear();
            cin.ignore((numeric_limits<streamsize>::max)(), '\n');
            cin >> g_m;
        }
        root = NULL;
    }

    int check_for_same(T key, T* arr, int size) //checking for duplicates
    {
        for (int i = 0; i < size; i++)
        {
            if (key == *(arr + i))
            {
                return i;
            }
        }
        return -1;
    }

    void point_search(T x, int choice) //searching for a singular data element with provided key
    {
        if (root) //if root is not NULL i-e tree is not empty
        {
            Node<T>* current = root; //creates temp node, and points it to root of B+ tree
            while (!current->leaf) //keeps searching until you come upon a leaf node
            { //checks if key being searched for is less than or greater than key number 'i' in node
                for (int i = 0; i < current->size; i++)
                {
                    if (x < current->key[i])
                    { //if searched key is less than key number 'i', points temporary node to that same index in child pointer array
                        current = current->child[i];
                        break;
                    }
                    if (i >= current->size - 1) //if reached the end of that node's keys
                    {
                        current = current->child[i + 1]; //assigns temp node pointer to the last child in children array
                        break;
                    }
                }
            }
            for (int i = 0; i < current->size; i++) //if you come upon a valid leaf node
            {
                if (current->key[i] == x) //simple search condition, checks if key 'i' of that node is equal to passed key
                {
                    if (choice == 1)
                    {
                        cout << "\nKey= " << current->key[i]; //displays data details
                        current->data[i].display();
                        return;
                    }
                    cout << "\nKey= " << current->key[i]; //displays data details
                    current->data[i].display();
                    return;
                }
                /*This search is somewhat similar to the binary search method, and I used that for reference*/
            }
            cout << "Not found\n";
        }
        else
        {
            cout << "Empty Tree\n";
            return;
        }
    }

    void range_search(T x, T x2, int choice) //searching for a singular data element with provided key
    {
        if (root) //if root is not NULL i-e tree is not empty
        {
            Node<T>* current = root; //creates temp node, and points it to root of B+ tree
            while (!current->leaf) //keeps searching until you come upon a leaf node
            { //checks if key being searched for is less than or greater than key number 'i' in node
                for (int i = 0; i < current->size; i++)
                {
                    if (x < current->key[i])
                    { //if searched key is less than key number 'i', points temporary node to that same index in child pointer array
                        current = current->child[i];
                        break;
                    }
                    if (i >= current->size - 1) //if reached the end of that node's keys
                    {
                        current = current->child[i + 1]; //assigns temp node pointer to the last child in children array
                        break;
                    }
                }
            }
            for (int i = 0; i < current->size; i++) //if you come upon a valid leaf node
            {
                if (current->key[i] == x) //simple search condition, checks if key 'i' of that node is equal to passed key
                {
                    if (choice == 1)
                    {
                        int iter = i, iter2 = x2;
                        bool flag = 0;
                        cout << "\nKey= " << current->key[iter];; //displays data details
                        string path = current->data[iter].display();
                        cout << "\n\n\n";
                        iter++;
                        while (iter <= iter2 && !flag)
                        {
                            ifstream file;
                            file.open(path);
                            if (file.good())
                            {
                                cout << "\n\n";
                                string x;
                                getline(file, x);
                                cout << "\n1. ID: " << x;
                                getline(file, x);
                                cout << "\n2. Year: " << x;
                                getline(file, x);
                                cout << "\n3. 113 Cause Name: " << x;
                                getline(file, x);
                                cout << "\n4. Cause: " << x;
                                getline(file, x);
                                cout << "\n5. State: " << x;
                                getline(file, x);
                                cout << "\n6. Deaths: " << x;
                                getline(file, x);
                                cout << "\n7. Age Adjusted Death Rate: " << x;
                                file >> path;
                                iter++;
                            }
                            else
                            {
                                flag = 1;
                            }
                            file.close();
                        }
                    }

                    else
                    {
                        string path = "\0";
                        bool flag = 0;
                        ifstream file;
                        if (choice == 2)
                        {
                            path = "C:/B+Year/";
                        }
                        else if (choice == 5)
                        {
                            path = "C:/B+Deaths/";
                        }
                        else if (choice == 6)
                        {
                            path = "C:/B+AgeDeath/";
                        }
                        path += "1.txt";
                        while (!flag)
                        {
                            file.open(path);
                            if (file.good())
                            {
                                string id = "\0", year = "\0", cause113 = "\0", cause = "\0", state = "\0", deaths = "\0", age_deaths = "\0";
                                cout << "\n\n";
                                getline(file, id);
                                getline(file, year);
                                getline(file, cause113);
                                getline(file, cause);
                                getline(file, state);
                                getline(file, deaths);
                                getline(file, age_deaths);
                                file >> path;

                                if (choice == 2)
                                {
                                    stringstream ss(year);
                                    int year_n = 0;
                                    ss >> year_n;

                                    if (year_n >= x && year_n <= x2)
                                    {
                                        cout << "\n1. ID: " << id;
                                        cout << "\n2. Year: " << year;
                                        cout << "\n3. 113 Cause Name: " << cause113;
                                        cout << "\n4. Cause: " << cause;
                                        cout << "\n5. State: " << state;
                                        cout << "\n6. Deaths: " << deaths;
                                        cout << "\n7. Age Adjusted Death Rate: " << age_deaths;
                                    }
                                }
                                else if (choice = 5)
                                {
                                    stringstream ss(deaths);
                                    int deaths_n = 0;
                                    ss >> deaths_n;

                                    if (deaths_n >= x && deaths_n <= x2)
                                    {
                                        cout << "\n1. ID: " << id;
                                        cout << "\n2. Year: " << year;
                                        cout << "\n3. 113 Cause Name: " << cause113;
                                        cout << "\n4. Cause: " << cause;
                                        cout << "\n5. State: " << state;
                                        cout << "\n6. Deaths: " << deaths;
                                        cout << "\n7. Age Adjusted Death Rate: " << age_deaths;
                                    }
                                }
                                else if (choice == 6)
                                {
                                    stringstream ss(age_deaths);
                                    int age_deaths_n = 0;
                                    ss >> age_deaths_n;

                                    if (age_deaths_n >= x && age_deaths_n <= x2)
                                    {
                                        cout << "\n1. ID: " << id;
                                        cout << "\n2. Year: " << year;
                                        cout << "\n3. 113 Cause Name: " << cause113;
                                        cout << "\n4. Cause: " << cause;
                                        cout << "\n5. State: " << state;
                                        cout << "\n6. Deaths: " << deaths;
                                        cout << "\n7. Age Adjusted Death Rate: " << age_deaths;
                                    }
                                }
                            }
                            else
                            {
                                flag = 1;
                            }
                            file.close();
                        }

                    }
                    return;
                }
                /*This search is somewhat similar to the binary search method, and I used that for reference*/
            }
            cout << "Not found\n";
        }
        else
        {
            cout << "Empty Tree\n";
            return;
        }
    }

    void update(T key, int field, string old, string new_val)
    {
        if (root) //if root is not NULL i-e tree is not empty
        {
            Node<T>* current = root; //creates temp node, and points it to root of B+ tree
            while (!current->leaf) //keeps searching until you come upon a leaf node
            { //checks if key being searched for is less than or greater than key number 'i' in node
                for (int i = 0; i < current->size; i++)
                {
                    if (key < current->key[i])
                    { //if searched key is less than key number 'i', points temporary node to that same index in child pointer array
                        current = current->child[i];
                        break;
                    }
                    if (i >= current->size - 1) //if reached the end of that node's keys
                    {
                        current = current->child[i + 1]; //assigns temp node pointer to the last child in children array
                        break;
                    }
                }
            }
            for (int i = 0; i < current->size; i++) //if you come upon a valid leaf node
            {
                if (current->key[i] == key) //simple search condition, checks if key 'i' of that node is equal to passed key
                {
                    cout << "\nOLD VALUE OF TUPLE:\n";
                    string pathnxt = current->data[i].display();
                    cout << "\n\n\n";
                    ifstream file;
                    string path = current->data[i].get_head()->gettxtpath();
                    file.open(path);
                    if (file.good())
                    {
                        string id = "\0", year = "\0", cause113 = "\0", cause = "\0", state = "\0", deaths = "\0", age_deaths = "\0";
                        cout << "\n\n";
                        getline(file, id);
                        getline(file, year);
                        getline(file, cause113);
                        getline(file, cause);
                        getline(file, state);
                        getline(file, deaths);
                        getline(file, age_deaths);

                        if (field == 1) //Year
                        {
                            if (year == old)
                            {
                                file.close();
                                ofstream file1;
                                file1.open(path);
                                if (file1.good())
                                {
                                    year = new_val;
                                    file1 << id << "\n" << year << "\n" << cause113 << "\n" << cause << "\n" << state << "\n" << deaths << "\n" << age_deaths << "\n" << pathnxt;

                                    cout << "\nNEW VALUE OF TUPLE:\n";
                                    cout << "\n1. ID: " << id;
                                    cout << "\n2. Year: " << year;
                                    cout << "\n3. 113 Cause Name: " << cause113;
                                    cout << "\n4. Cause: " << cause;
                                    cout << "\n5. State: " << state;
                                    cout << "\n6. Deaths: " << deaths;
                                    cout << "\n7. Age Adjusted Death Rate: " << age_deaths;
                                }
                                else
                                {
                                    cout << "\nCould not open text file\n";
                                }
                                file1.close();
                            }
                            else
                            {
                                cout << "\nValue of field specified was not correct\n";
                            }
                        }

                        else if (field == 2) //Cause
                        {
                            if (cause == old)
                            {
                                file.close();
                                ofstream file1;
                                file1.open(path);
                                if (file1.good())
                                {
                                    cause = new_val;
                                    file1 << id << "\n" << year << "\n" << cause113 << "\n" << cause << "\n" << state << "\n" << deaths << "\n" << age_deaths << "\n" << pathnxt;

                                    cout << "\nNEW VALUE OF TUPLE:\n";
                                    cout << "\n1. ID: " << id;
                                    cout << "\n2. Year: " << year;
                                    cout << "\n3. 113 Cause Name: " << cause113;
                                    cout << "\n4. Cause: " << cause;
                                    cout << "\n5. State: " << state;
                                    cout << "\n6. Deaths: " << deaths;
                                    cout << "\n7. Age Adjusted Death Rate: " << age_deaths;
                                }
                                else
                                {
                                    cout << "\nCould not open text file\n";
                                }
                                file1.close();
                            }
                            else
                            {
                                cout << "\nValue of field specified was not correct\n";
                            }
                        }

                        else if (field == 3) //State
                        {
                            if (state == old)
                            {
                                file.close();
                                ofstream file1;
                                file1.open(path);
                                if (file1.good())
                                {
                                    state = new_val;
                                    file1 << id << "\n" << year << "\n" << cause113 << "\n" << cause << "\n" << state << "\n" << deaths << "\n" << age_deaths << "\n" << pathnxt;

                                    cout << "\nNEW VALUE OF TUPLE:\n";
                                    cout << "\n1. ID: " << id;
                                    cout << "\n2. Year: " << year;
                                    cout << "\n3. 113 Cause Name: " << cause113;
                                    cout << "\n4. Cause: " << cause;
                                    cout << "\n5. State: " << state;
                                    cout << "\n6. Deaths: " << deaths;
                                    cout << "\n7. Age Adjusted Death Rate: " << age_deaths;
                                }
                                else
                                {
                                    cout << "\nCould not open text file\n";
                                }
                                file1.close();
                            }
                            else
                            {
                                cout << "\nValue of field specified was not correct\n";
                            }
                        }

                        else if (field == 4) //Deaths
                        {
                            if (deaths == old)
                            {
                                file.close();
                                ofstream file1;
                                file1.open(path);
                                if (file1.good())
                                {
                                    deaths = new_val;
                                    file1 << id << "\n" << year << "\n" << cause113 << "\n" << cause << "\n" << state << "\n" << deaths << "\n" << age_deaths << "\n" << pathnxt;

                                    cout << "\nNEW VALUE OF TUPLE:\n";
                                    cout << "\n1. ID: " << id;
                                    cout << "\n2. Year: " << year;
                                    cout << "\n3. 113 Cause Name: " << cause113;
                                    cout << "\n4. Cause: " << cause;
                                    cout << "\n5. State: " << state;
                                    cout << "\n6. Deaths: " << deaths;
                                    cout << "\n7. Age Adjusted Death Rate: " << age_deaths;
                                }
                                else
                                {
                                    cout << "\nCould not open text file\n";
                                }
                                file1.close();
                            }
                            else
                            {
                                cout << "\nValue of field specified was not correct\n";
                            }
                        }

                        else if (field == 5) //Age Adjusted Death Rate
                        {
                            if (age_deaths == old)
                            {
                                file.close();
                                ofstream file1;
                                file1.open(path);
                                if (file1.good())
                                {
                                    age_deaths = new_val;
                                    file1 << id << "\n" << year << "\n" << cause113 << "\n" << cause << "\n" << state << "\n" << deaths << "\n" << age_deaths << "\n" << pathnxt;

                                    cout << "\nNEW VALUE OF TUPLE:\n";
                                    cout << "\n1. ID: " << id;
                                    cout << "\n2. Year: " << year;
                                    cout << "\n3. 113 Cause Name: " << cause113;
                                    cout << "\n4. Cause: " << cause;
                                    cout << "\n5. State: " << state;
                                    cout << "\n6. Deaths: " << deaths;
                                    cout << "\n7. Age Adjusted Death Rate: " << age_deaths;
                                }
                                else
                                {
                                    cout << "\nCould not open text file\n";
                                }
                                file1.close();
                            }
                            else
                            {
                                cout << "\nValue of field specified was not correct\n";
                            }
                        }
                    }
                    else
                    {
                        cout << "\nCould not open text file\n";
                    }
                    file.close();
                }
            }
        }
        else
        {
            cout << "Empty Tree\n";
            return;
        }
    }

    void deletion(T x, int choice)
    {
        string path = "\0";
        bool flag = 0;
        ifstream file;
        if (choice == 1)
        {
            path = "C:/B+ID/";
        }
        else if (choice == 2)
        {
            path = "C:/B+Year/";
        }
        else if (choice == 3)
        {
            path = "C:/B+Cause/";
        }
        else if (choice == 4)
        {
            path = "C:/B+State/";
        }
        else if (choice == 5)
        {
            path = "C:/B+Deaths/";
        }
        else if (choice == 6)
        {
            path = "C:/B+AgeDeath/";
        }
        path += "1.txt";

        file.open(path);
        int i = 2;
        while (!flag)
        {
            if (file.good())
            {
                string tempcstr = path;
                const char* temppath = tempcstr.c_str();
                string id = "\0", year = "\0", cause113 = "\0", cause = "\0", state = "\0", deaths = "\0", age_deaths = "\0";
                getline(file, id);
                getline(file, year);
                getline(file, cause113);
                getline(file, cause);
                getline(file, state);
                getline(file, deaths);
                getline(file, age_deaths);
                file >> path;
                file.close();
                if (id.length() == 0)
                {
                    flag = 1;
                }

                if (choice == 1)
                {
                    stringstream ss(id);
                    T delid = 0;
                    ss >> delid;

                    if (delid == x)
                    {
                        remove(temppath);
                        cout << "\nSuccessfully deleted record and file at " << temppath << "\n";
                    }
                }
                else if (choice == 2)
                {
                    stringstream ss(id);
                    T delid = 0;
                    ss >> delid;
                    if (delid == x)
                    {
                        remove(temppath);
                        cout << "\nSuccessfully deleted record and file at " << temppath << "\n";
                    }
                }
                else if (choice == 3)
                {
                    stringstream ss;
                    ss << x;
                    string keystr = ss.str();

                    if (cause == keystr)
                    {
                        remove(temppath);
                        cout << "\nSuccessfully deleted record and file at " << temppath << "\n";
                    }
                }
                else if (choice == 4)
                {
                    stringstream ss;
                    ss << x;
                    string keystr = ss.str();

                    if (state == keystr)
                    {
                        remove(temppath);
                        cout << "\nSuccessfully deleted record and file at " << temppath << "\n";
                    }
                }
                else if (choice == 5)
                {
                    stringstream ss(id);
                    T delid = 0;
                    ss >> delid;
                    if (delid == x)
                    {
                        remove(temppath);
                        cout << "\nSuccessfully deleted record and file at " << temppath << "\n";
                    }
                }
                else if (choice == 6)
                {
                    stringstream ss(id);
                    T delid = 0;
                    ss >> delid;
                    if (delid == x)
                    {
                        remove(temppath);
                        cout << "\nSuccessfully deleted record and file at " << temppath << "\n";
                    }
                }
                file.open(path);
            }
            else
            {
                flag = 1;
            }
        }

        file.close();
    }

    void insert(T x, int l, string p, string t) //code to insert in leaf nodes of B+ tree
    {
        bool flag = 0;
        if (!root) //if root is NULL i-e tree is empty
        {
            root = new Node<T>; //assigns root as a new node
            root->key[root->size] = x; //sets key at 0th index to the inserted key
            root->data[root->size].insert(l, p, t); //inserts data in linked list
            root->leaf = 1;
            root->size++;
            root->next = NULL;
            root->prev = NULL;
            return;
        }
        //if tree is not empty
        Node<T>* current = getRoot(); //sets pointer to root
        Node<T>* ancestor = NULL;

        Node<T>* nodefordup = getRoot();
        int index = 0;
        if (nodefordup->leaf) //checks for duplicates if current node is leaf
        {
            for (index = 0; index < nodefordup->size; index++)
            {
                if (x == nodefordup->key[index])
                {
                    flag = 1;
                    break;
                }
            }
        }
        if (flag)
        {
            nodefordup->data[index].insert(l, p, t);
            return;
        }

        while (!current->leaf) //if current node is not leaf
        {
            /*searches for what location of child pointer to point current node to, in order to figure out where to go next
            this searching method is the same as we defined above*/
            ancestor = current;
            for (int i = 0; i < current->size; i++)
            {
                if (x == current->key[i])
                {
                    current = current->child[i]; //if duplicate occurs
                    flag = 1; //sets bool flag to 1
                    break;
                }
                if (i == current->size - 1)
                {
                    current = current->child[i + 1];
                    break;
                }
                if (x < current->key[i])
                {
                    current = current->child[i];
                    break;
                }
            }
        }
        if (flag) //if duplicate occurs, inserts data into linked list of same key
        {
            int i = 0;
            while (x > current->key[i] && i < current->size)
            {
                i += 1;
            }
            current->data[i].insert(l, p, t);
            return;
        }
        else if (current->size < (g_m - 1)) //if there is space to insert more nodes in that node
        {
            int i = 0;
            while (x > current->key[i] && i < current->size)
            {
                i += 1;
            }
            for (int j = current->size; j > i; j--)
            {
                current->key[j] = current->key[j - 1];
                current->data[j] = current->data[j - 1];

            }
            Node<T>* parentnode = return_ancestor(root, current); //gets the ancestor of the node in question
            int j = 0;
            if (parentnode) //LINKING LEAF NODES
            {
                for (j = 0; j < parentnode->size; j++) //checks parent to see what index of children node the current node in question is, then links all the children of that parent
                {
                    if (parentnode->child[j] == current)
                    {
                        break;
                    }
                }
                if (j == (g_m - 1))
                {
                    current->prev = parentnode->child[j - 1];
                    parentnode->child[j - 1]->next = current;
                    current->next = NULL;
                }
                else if (j == 0)
                {
                    current->prev = NULL;
                    current->next = parentnode->child[j + 1];
                    parentnode->child[j + 1]->prev = current;
                }
                else
                {
                    current->next = parentnode->child[j + 1];
                    parentnode->child[j + 1]->prev = current;
                    current->prev = parentnode->child[j - 1];
                    parentnode->child[j - 1]->next = current;
                }
            }
            current->key[i] = x; //inserts key at index found earlier
            current->data[i].insert(l, p, t); //inserts  data at same index
            current->size++; //increments size of node by 1
            current->child[current->size] = current->child[current->size - 1]; //shifts children array by 1
            current->child[current->size - 1] = NULL; //sets the child pointer we assigned above to NULL
            return;
        }

        else //if there is no more space to insert in that node
        {
            Node<T>* temp = new Node<T>;
            my_data tempdata[SIZEMAX];
            T temp2[SIZEMAX];
            for (int i = 0; i < g_m - 1; i++)
            {
                *(temp2 + i) = current->key[i];
                *(tempdata + i) = current->data[i];
            }

            int i = 0, j = 0;
            while ((x > * (temp2 + i)) && (i < g_m - 1))
            {
                i += 1;
            }
            for (int j = g_m; j > i; j--)
            {
                swap_dec1d(temp2, j);
                swap_dec1d_class(tempdata, j);
            }

            *(temp2 + i) = x;
            tempdata[i].insert(l, p, t);
            temp->leaf = 1;
            current->size = (g_m) / 2; //sets size of node after shifting of elements to new size, according to rules of B+ tree
            temp->size = (g_m)-(g_m / 2);
            current->child[current->size] = temp;
            temp->child[temp->size] = current->child[g_m - 1];
            current->child[g_m - 1] = NULL;
            for (i = 0; i < current->size; i++)
            {
                current->key[i] = *(temp2 + i);
                current->data[i] = *(tempdata + i);
            }
            for (i = 0, j = current->size; i < temp->size; i++, j++)
            {
                temp->key[i] = *(temp2 + j);
                temp->data[i] = *(tempdata + j);
            }
            if (current == root) //if node is root node of B+ Tree
            {
                Node<T>* temproot = new Node<T>; //defines new root for that B+ Tree
                temproot->key[0] = temp->key[0]; //sets first key and first data elements of new root to the temp node we created before
                temproot->data[0] = temp->data[0];

                temproot->child[0] = current; //sets first child of new root to the current node
                temproot->child[1] = temp; //sets second child of new root to the temp node we created before
                temproot->leaf = 0; //sets the new root to be a non leaf node
                temproot->size = 1; //sets size of the new root as 1, as we inserted temp node at key[0] of new root
                root = temproot; //sets root data member of the class to the new root, so that this new root will be referred to as the root of the B+ tree


                Node<T>* parentnode = return_ancestor(root, current); //finds ancestor of the current node in question
                int j = 0;

                if (parentnode) //LINKING LEAF NODES
                {
                    //linking, like we did above
                    for (j = 0; j < parentnode->size; j++)
                    {
                        if (parentnode->child[j] == current)
                        {
                            break;
                        }
                    }

                    if (j == 0)
                    {
                        current->prev = NULL;
                        current->next = parentnode->child[j + 1];
                        parentnode->child[j + 1]->prev = current;
                    }
                    else if (j == (g_m - 1))
                    {
                        current->prev = parentnode->child[j - 1];
                        parentnode->child[j - 1]->next = current;
                        current->next = NULL;
                    }
                    else
                    {
                        current->next = parentnode->child[j + 1];
                        parentnode->child[j + 1]->prev = current;
                        current->prev = parentnode->child[j - 1];
                        parentnode->child[j - 1]->next = current;
                    }
                }

            }
            else
            {
                make_insertion(temp->key[0], temp->data[0], ancestor, temp); //inserts at the internal nodes of the tree, after previous conditions fail. This is the only one that remains

                Node<T>* parentnode = return_ancestor(root, current); //gets ancestor of current node
                int j = 0;

                if (parentnode) //LINKING LEAF NODES
                {
                    //linking, like we did before
                    for (j = 0; j < parentnode->size; j++)
                    {
                        if (parentnode->child[j] == current)
                        {
                            break;
                        }
                    }
                    if (j == 0)
                    {
                        current->prev = NULL;
                        current->next = parentnode->child[j + 1];
                        parentnode->child[j + 1]->prev = current;
                    }
                    else if (j == (g_m - 1))
                    {
                        current->prev = parentnode->child[j - 1];
                        parentnode->child[j - 1]->next = current;
                        current->next = NULL;
                    }
                    else
                    {
                        current->next = parentnode->child[j + 1];
                        parentnode->child[j + 1]->prev = current;
                        current->prev = parentnode->child[j - 1];
                        parentnode->child[j - 1]->next = current;
                    }
                }
            }
        }
    }

    void make_insertion(T x, my_data y, Node<T>* current, Node<T>* descendant) //inserting in the inner nodes of the B+ tree. this is usually done after new data is inserted,
                                                                               //and old data needs to be shifted
    {
        if (current->size < g_m - 1) //if size of current node is not more than the order specified i-e there is space for insertion
        {
            int i = 0;
            while (x > current->key[i] && i < current->size)
            {
                i++;
            }
            for (int j = current->size; j > i; j--)
            {
                current->key[j] = current->key[j - 1]; //shifts keys by one
                current->data[j] = current->data[j - 1]; //shifts data by one
            }
            for (int j = current->size + 1; j > i + 1; j--)
            {
                current->child[j] = current->child[j - 1]; //shifts children by one
            }

            current->key[i] = x; //sets key of current node to the one passed to the function
            current->data[i] = y; //does same for the data

            current->size += 1; //increases size of node by 1
            current->child[i + 1] = descendant; //sets child of current node as the next possible one, to the one passed in the function
        }
        else //if size of current node is equal to or more than the order specified i-e no more space for insertion
        {
            Node<T>* add_node = new Node<T>; //defines node to be added
            Node<T>* child_temp[SIZEMAX]; //temp array to hold children
            T temp_key[SIZEMAX]; //temp array to store keys
            my_data tempdata[SIZEMAX]; //temp array to store data

            for (int i = 0; i < g_m - 1; i++)
            {
                send_same_idx(temp_key, current, i); //stores key of current node in temp arr
                send_same_idx_data(tempdata, current, i); //stores data of current node in temp arr
            }
            for (int i = 0; i < g_m; i++)
            {
                send_same_idx_child(child_temp, current, i); //stores child of current node in temp arr
            }

            int i = 0;
            while (x > * (temp_key + i) && i < g_m - 1) //searches for what index to insert key and data in temp array. increments the indnex if key to be added is larger than current one in temp array
            {
                i += 1;
            }
            for (int j = g_m; j > i; j--)
            {
                swap_dec1d_class(tempdata, j); //shifts temp array for data one index to the left
                swap_dec1d(temp_key, j); //does same for temp array for key
            }
            *(temp_key + i) = x; //adds data to temp array for keys, based on the index we found earlier
            *(tempdata + i) = y; //does same for temp data array

            for (int j = g_m + 1; j > i + 1; j--)
            {
                swap_dec(child_temp, j); //shifts temp children array to the left, but starts from the end of the array, so effectively shifting it to the right
            }

            *(child_temp + (i + 1)) = descendant; //sets temp child array based on index we found earlier, plus one, to the descendant we passed to the function
            add_node->leaf = 0; //sets leaf status of the node to be added as false

            current->size = (g_m) / 2; //adjusts sizes of nodes based on rules of B+ Tree
            add_node->size = (g_m - 1) - ((g_m) / 2);

            int j = 0;
            for (i = 0, j = current->size + 1; i < add_node->size; i++, j++)
            {
                add_node->data[i] = *(tempdata + j); //sets data element of the node to be added to the temp array
                add_node->key[i] = *(temp_key + j); //does same for keys
            }

            for (i = 0, j = current->size + 1; i < add_node->size + 1; i++, j++)
            {
                add_node->child[i] = *(child_temp + j); //does same for children
            }

            if (current == root) //checks if current root in question is the root node
            {
                Node<T>* temproot = new Node<T>; //if it is the root, defines a new temp  root, and does same procedures we did above for a new root, when we inserted in leaf nodes
                temproot->key[0] = current->key[current->size];
                temproot->data[0] = current->data[current->size];
                temproot->child[0] = current;
                temproot->child[1] = add_node;
                temproot->leaf = 0;
                temproot->size = 1;
                root = temproot;
            }
            else
            {
                make_insertion(current->key[current->size], current->data[current->size], return_ancestor(getRoot(), current), add_node); //if root is not reached, makes recursive call again and again
                                                                                                                                         //till we get to the root, so that whole tree is according to
                                                                                                                                         //rules of B+ Tree
            }
        }
    }

    Node<T>* return_ancestor(Node<T>* current, Node<T>* findparentof) //recursive function to find the ancestor of a given node. takeks two nodes as argument,
                                                                      //one to use for traversal, and the other to find the ancestor of
    {
        Node<T>* parent; //defines a node as parent, this will be what we use to find the ancestor of given node
        parent = NULL; //initially sets it to NULL
        if ((current->leaf) || (current->child[0]->leaf)) //if passed node is NULL, or first child is also a leaf node 
        {
            return NULL; //returns NULL, as no parent can be possible in that sense
        }

        for (int i = 0; i < current->size + 1; i++) //iterates through a node
        {
            if (current->child[i] == findparentof) //checks if a nodes child is equal to the node we want to find the ancesotr of
            {
                parent = current; //if it is, we point the parent node to that node, and return the parent node
                return parent;
            }
            else
            { //if a nodes child is not equal to the passed node that we want to find the ancestor of
                parent = return_ancestor(current->child[i], findparentof); // function calls itself again, this time passing thatt nodes child that is not equal to the passed node, as argument,
                                                                           // and saves its return argumentn to the parent node
                if (parent) //if parent is not NULL
                {
                    return parent; //returns it
                }
            }
        }
        return parent; //returns the parentn
    }

    Node<T>* getRoot() //returns the root
    {
        return root;
    }

    //utility functions
    inline void swap_dec(Node<T>** x, int j)
    {
        *(x + j) = *(x + (j - 1));
    }

    inline void swap_dec1d(T* x, int j)
    {
        *(x + j) = *(x + (j - 1));
    }

    inline void swap_dec1d_class(my_data* x, int j)
    {
        *(x + j) = *(x + (j - 1));
    }

    inline void send_same_idx(T* x, Node<T>* y, int i)
    {
        *(x + i) = y->key[i];
    }

    inline void send_same_idx_data(my_data* x, Node<T>* y, int i)
    {
        *(x + i) = y->data[i];
    }

    inline void send_same_idx_child(Node<T>** x, Node<T>* y, int i)
    {
        *(x + i) = y->child[i];
    }
};

template <class T> class file_reader //class for reading input files, and storing nodes in files
{
private:
    void CreateFolder(wchar_t* path) //creating folders
    {
        if (!CreateDirectory(path, NULL))
        {
            cout << "\nCould not create directory or directory is already created\n";
            return;
        }
    }
public:
    void readfile(int choice, DSDB<T>& node, int limit) //reads csv files and creates relevant text files in relevant folders, according to the choices specified by the user
    {
        cin.clear();
        cin.ignore();
        string fpath;
        cout << "\nEnter path where .CSV files are located\nOnly enter the path where the folder containing the .CSV files are\n" <<
            "\nIf the .CSV files are in the same folder as the .cpp file of the project, just enter '.' , without the quotation marks\n" <<
            "E.g. If you want to set path as D drive, enter 'D:'\nIf you enter 'D:', final path would be 'D:/NCHS_-_Leading_Causes_of_Death__United_States/\n";
        getline(cin, fpath);

        if (choice == 1)
        {
            wchar_t dir[1024] = L"C:/B+ID";
            CreateFolder(dir); //creating folder based on indexing type
            ifstream file;
            int i = 0;
            for (int j = 1; j <= limit; j++)
            {
                string path;
                if (j < 10)
                {
                    path = fpath + "/NCHS_-_Leading_Causes_of_Death__United_States/DeathRate_0";
                }
                else
                {
                    path = fpath + "/NCHS_-_Leading_Causes_of_Death__United_States/DeathRate_";
                }
                stringstream j1;
                string jstr;
                j1 << j;
                j1 >> jstr;

                path += jstr;
                path += ".csv";
                cout << path << "\n";
                file.open(path);
                string x;

                int line = 0;
                if (file.good())
                {
                    cout << "\nFile open\n";
                    while (!file.eof())
                    {
                        getline(file, x);
                        cout << x << "\n";
                        if (line != 0)
                        {
                            string txtpath = "C:/B+ID/";
                            string tmptxt = txtpath;
                            string istr;
                            stringstream s1;
                            s1 << i;
                            s1 >> istr;
                            txtpath += istr;
                            txtpath += ".txt";
                            ofstream outfile;
                            outfile.open(txtpath);
                            T id = 0;
                            int flag = 0;
                            if (outfile.good())
                            {
                                int j = 0, k = 0, c_count = 0;
                                string temp = "\0";
                                temp.resize(MAX_STRING_LENGTH);
                                while (x[j])
                                {
                                    if (x[j + 1] == '\0')
                                    {
                                        temp[k] = x[j];
                                        outfile << temp << "\n";
                                        outfile << tmptxt << i + 1 << ".txt";
                                        temp.clear();
                                        temp.resize(MAX_STRING_LENGTH);
                                        k = 0;
                                        j++;
                                        continue;
                                    }

                                    if (x[j] != ',')
                                    {
                                        if (j - 1 >= 0)
                                        {
                                            if (x[j] == 'V' && x[j - 1] == '(')
                                            {
                                                flag = 1;
                                            }
                                            if (x[j] == 'I' && x[j - 1] == '(')
                                            {
                                                flag = 2;
                                            }
                                            if (x[j] == 'N' && x[j - 1] == '"')
                                            {
                                                flag = 3;
                                            }
                                            if (x[j] == '*' && x[j - 1] == '(')
                                            {
                                                flag = 4;
                                            }
                                        }
                                        temp[k] = x[j];
                                        k++, j++;
                                    }
                                    else
                                    {
                                        c_count++;
                                        if (c_count <= 2)
                                        {
                                            stringstream str(temp);
                                            int numeric_data = 0;
                                            str >> numeric_data;
                                            outfile << numeric_data << "\n";
                                            if (c_count == 1)
                                            {
                                                id = numeric_data;
                                            }
                                            temp.clear();
                                            temp.resize(MAX_STRING_LENGTH);
                                            k = 0;
                                            j++;
                                        }
                                        else
                                        {
                                            if (flag == 1 && c_count == 3)
                                            {
                                                temp[k] = x[j];
                                                k++, j++;
                                                continue;
                                            }
                                            else if ((flag == 2 || flag == 3) && (c_count == 3 || c_count == 4 || c_count == 5))
                                            {
                                                temp[k] = x[j];
                                                k++, j++;
                                                continue;
                                            }
                                            else if (flag == 4 && (c_count == 3 || c_count == 4))
                                            {
                                                temp[k] = x[j];
                                                k++, j++;
                                                continue;
                                            }
                                            outfile << temp << "\n";
                                            temp.clear();
                                            temp.resize(MAX_STRING_LENGTH);
                                            k = 0;
                                            j++;
                                        }
                                    }
                                }
                            }
                            outfile.close();
                            id += 1;
                            node.insert(id, line, path, txtpath);
                        }
                        i++, line++;
                    }
                }
                else
                {
                    cout << "\nCould not create file\n";
                }
                file.close();
            }
        }

        else if (choice == 2)
        {
            wchar_t dir[1024] = L"C:/B+Year";
            CreateFolder(dir); //creating folder based on indexing type
            ifstream file;
            int i = 0;
            for (int j = 1; j <= limit; j++)
            {
                string path;
                if (j < 10)
                {
                    path = fpath + "/NCHS_-_Leading_Causes_of_Death__United_States/DeathRate_0";
                }
                else
                {
                    path = fpath + "/NCHS_-_Leading_Causes_of_Death__United_States/DeathRate_";
                }
                stringstream j1;
                string jstr;
                j1 << j;
                j1 >> jstr;

                path += jstr;
                path += ".csv";
                cout << path << "\n";
                file.open(path);
                string x;

                int line = 0;
                if (file.good())
                {
                    cout << "\nFile open\n";
                    while (!file.eof())
                    {
                        getline(file, x);
                        cout << x << "\n";
                        if (line != 0)
                        {
                            string txtpath = "C:/B+Year/";
                            string tmptxt = txtpath;
                            string istr;
                            stringstream s1;
                            s1 << i;
                            s1 >> istr;
                            txtpath += istr;
                            txtpath += ".txt";
                            ofstream outfile;
                            outfile.open(txtpath);
                            T id = 0;
                            int flag = 0;
                            if (outfile.good())
                            {
                                int j = 0, k = 0, c_count = 0;
                                string temp = "\0";
                                temp.resize(MAX_STRING_LENGTH);
                                while (x[j])
                                {
                                    if (x[j + 1] == '\0')
                                    {
                                        temp[k] = x[j];
                                        outfile << temp << "\n";
                                        outfile << tmptxt << i + 1 << ".txt";
                                        temp.clear();
                                        temp.resize(MAX_STRING_LENGTH);
                                        k = 0;
                                        j++;
                                        continue;
                                    }

                                    if (x[j] != ',')
                                    {
                                        if (j - 1 >= 0)
                                        {
                                            if (x[j] == 'V' && x[j - 1] == '(')
                                            {
                                                flag = 1;
                                            }
                                            if (x[j] == 'I' && x[j - 1] == '(')
                                            {
                                                flag = 2;
                                            }
                                            if (x[j] == 'N' && x[j - 1] == '"')
                                            {
                                                flag = 3;
                                            }
                                            if (x[j] == '*' && x[j - 1] == '(')
                                            {
                                                flag = 4;
                                            }
                                        }
                                        temp[k] = x[j];
                                        k++, j++;
                                    }
                                    else
                                    {
                                        c_count++;
                                        if (c_count <= 2)
                                        {
                                            stringstream str(temp);
                                            int numeric_data = 0;
                                            str >> numeric_data;
                                            outfile << numeric_data << "\n";
                                            if (c_count == 2)
                                            {
                                                id = numeric_data;
                                            }
                                            temp.clear();
                                            temp.resize(MAX_STRING_LENGTH);
                                            k = 0;
                                            j++;
                                        }
                                        else
                                        {
                                            if (flag == 1 && c_count == 3)
                                            {
                                                temp[k] = x[j];
                                                k++, j++;
                                                continue;
                                            }
                                            else if ((flag == 2 || flag == 3) && (c_count == 3 || c_count == 4 || c_count == 5))
                                            {
                                                temp[k] = x[j];
                                                k++, j++;
                                                continue;
                                            }
                                            else if (flag == 4 && (c_count == 3 || c_count == 4))
                                            {
                                                temp[k] = x[j];
                                                k++, j++;
                                                continue;
                                            }
                                            outfile << temp << "\n";
                                            temp.clear();
                                            temp.resize(MAX_STRING_LENGTH);
                                            k = 0;
                                            j++;
                                        }
                                    }
                                }
                            }
                            outfile.close();
                            node.insert(id, line, path, txtpath);
                        }
                        i++, line++;
                    }
                }
                else
                {
                    cout << "\nCould not create file\n";
                }
                file.close();
            }
        }

        else if (choice == 3)
        {
            wchar_t dir[1024] = L"C:/B+Cause";
            CreateFolder(dir); //creating folder based on indexing type
            ifstream file;
            int i = 0;
            for (int j = 1; j <= limit; j++)
            {
                string path;
                if (j < 10)
                {
                    path = fpath + "/NCHS_-_Leading_Causes_of_Death__United_States/DeathRate_0";
                }
                else
                {
                    path = fpath + "/NCHS_-_Leading_Causes_of_Death__United_States/DeathRate_";
                }
                stringstream j1;
                string jstr;
                j1 << j;
                j1 >> jstr;

                path += jstr;
                path += ".csv";
                cout << path << "\n";
                file.open(path);
                string x;

                int line = 0;
                if (file.good())
                {
                    cout << "\nFile open\n";
                    while (!file.eof())
                    {
                        getline(file, x);
                        cout << x << "\n";
                        if (line != 0)
                        {
                            string txtpath = "C:/B+Cause/";
                            string tmptxt = txtpath;
                            string istr;
                            stringstream s1;
                            s1 << i;
                            s1 >> istr;
                            txtpath += istr;
                            txtpath += ".txt";
                            ofstream outfile;
                            outfile.open(txtpath);
                            string id;
                            int flag = 0;
                            if (outfile.good())
                            {
                                int j = 0, k = 0, c_count = 0;
                                string temp = "\0";
                                temp.resize(MAX_STRING_LENGTH);
                                while (x[j])
                                {
                                    if (x[j + 1] == '\0')
                                    {
                                        temp[k] = x[j];
                                        outfile << temp << "\n";
                                        outfile << tmptxt << i + 1 << ".txt";
                                        temp.clear();
                                        temp.resize(MAX_STRING_LENGTH);
                                        k = 0;
                                        j++;
                                        continue;
                                    }

                                    if (x[j] != ',')
                                    {
                                        if (j - 1 >= 0)
                                        {
                                            if (x[j] == 'V' && x[j - 1] == '(')
                                            {
                                                flag = 1;
                                            }
                                            if (x[j] == 'I' && x[j - 1] == '(')
                                            {
                                                flag = 2;
                                            }
                                            if (x[j] == 'N' && x[j - 1] == '"')
                                            {
                                                flag = 3;
                                            }
                                            if (x[j] == '*' && x[j - 1] == '(')
                                            {
                                                flag = 4;
                                            }
                                        }
                                        temp[k] = x[j];
                                        k++, j++;
                                    }
                                    else
                                    {
                                        c_count++;
                                        if (c_count <= 2)
                                        {
                                            stringstream str(temp);
                                            int numeric_data = 0;
                                            str >> numeric_data;
                                            outfile << numeric_data << "\n";
                                            temp.clear();
                                            temp.resize(MAX_STRING_LENGTH);
                                            k = 0;
                                            j++;
                                        }
                                        else
                                        {
                                            if (flag == 1 && c_count == 3)
                                            {
                                                temp[k] = x[j];
                                                k++, j++;
                                                continue;
                                            }
                                            else if ((flag == 2 || flag == 3) && (c_count == 3 || c_count == 4 || c_count == 5))
                                            {
                                                temp[k] = x[j];
                                                k++, j++;
                                                continue;
                                            }
                                            else if (flag == 4 && (c_count == 3 || c_count == 4))
                                            {
                                                temp[k] = x[j];
                                                k++, j++;
                                                continue;
                                            }

                                            if (flag == 1 && c_count == 5)
                                            {
                                                id = temp;
                                            }
                                            else if ((flag == 2 || flag == 3) && c_count == 7)
                                            {
                                                id = temp;
                                            }
                                            else if (flag == 4 && c_count == 6)
                                            {
                                                id = temp;
                                            }
                                            else if (flag == 0 && c_count == 4)
                                            {
                                                id = temp;
                                            }
                                            outfile << temp << "\n";
                                            temp.clear();
                                            temp.resize(MAX_STRING_LENGTH);
                                            k = 0;
                                            j++;
                                        }
                                    }
                                }
                            }
                            outfile.close();
                            stringstream ss(id);
                            T num;
                            ss >> num;
                            node.insert(num, line, path, txtpath);
                        }
                        i++, line++;
                    }
                }
                else
                {
                    cout << "\nCould not create file\n";
                }
                file.close();
            }
        }

        else if (choice == 4)
        {
            wchar_t dir[1024] = L"C:/B+State";
            CreateFolder(dir); //creating folder based on indexing type
            ifstream file;
            int i = 0;
            for (int j = 1; j <= limit; j++)
            {
                string path;
                if (j < 10)
                {
                    path = fpath + "/NCHS_-_Leading_Causes_of_Death__United_States/DeathRate_0";
                }
                else
                {
                    path = fpath + "/NCHS_-_Leading_Causes_of_Death__United_States/DeathRate_";
                }
                stringstream j1;
                string jstr;
                j1 << j;
                j1 >> jstr;

                path += jstr;
                path += ".csv";
                cout << path << "\n";
                file.open(path);
                string x;

                int line = 0;
                if (file.good())
                {
                    cout << "\nFile open\n";
                    while (!file.eof())
                    {
                        getline(file, x);
                        cout << x << "\n";
                        if (line != 0)
                        {
                            string txtpath = "C:/B+State/";
                            string tmptxt = txtpath;
                            string istr;
                            stringstream s1;
                            s1 << i;
                            s1 >> istr;
                            txtpath += istr;
                            txtpath += ".txt";
                            ofstream outfile;
                            outfile.open(txtpath);
                            string id;
                            int flag = 0;
                            if (outfile.good())
                            {
                                int j = 0, k = 0, c_count = 0;
                                string temp = "\0";
                                temp.resize(MAX_STRING_LENGTH);
                                while (x[j])
                                {
                                    if (x[j + 1] == '\0')
                                    {
                                        temp[k] = x[j];
                                        outfile << temp << "\n";
                                        outfile << tmptxt << i + 1 << ".txt";
                                        temp.clear();
                                        temp.resize(MAX_STRING_LENGTH);
                                        k = 0;
                                        j++;
                                        continue;
                                    }

                                    if (x[j] != ',')
                                    {
                                        if (j - 1 >= 0)
                                        {
                                            if (x[j] == 'V' && x[j - 1] == '(')
                                            {
                                                flag = 1;
                                            }
                                            if (x[j] == 'I' && x[j - 1] == '(')
                                            {
                                                flag = 2;
                                            }
                                            if (x[j] == 'N' && x[j - 1] == '"')
                                            {
                                                flag = 3;
                                            }
                                            if (x[j] == '*' && x[j - 1] == '(')
                                            {
                                                flag = 4;
                                            }
                                        }
                                        temp[k] = x[j];
                                        k++, j++;
                                    }
                                    else
                                    {
                                        c_count++;
                                        if (c_count <= 2)
                                        {
                                            stringstream str(temp);
                                            int numeric_data = 0;
                                            str >> numeric_data;
                                            outfile << numeric_data << "\n";
                                            temp.clear();
                                            temp.resize(MAX_STRING_LENGTH);
                                            k = 0;
                                            j++;
                                        }
                                        else
                                        {
                                            if (flag == 1 && c_count == 3)
                                            {
                                                temp[k] = x[j];
                                                k++, j++;
                                                continue;
                                            }
                                            else if ((flag == 2 || flag == 3) && (c_count == 3 || c_count == 4 || c_count == 5))
                                            {
                                                temp[k] = x[j];
                                                k++, j++;
                                                continue;
                                            }
                                            else if (flag == 4 && (c_count == 3 || c_count == 4))
                                            {
                                                temp[k] = x[j];
                                                k++, j++;
                                                continue;
                                            }

                                            if (flag == 1 && c_count == 6)
                                            {
                                                id = temp;
                                            }
                                            else if ((flag == 2 || flag == 3) && c_count == 8)
                                            {
                                                id = temp;
                                            }
                                            else if (flag == 4 && c_count == 7)
                                            {
                                                id = temp;
                                            }
                                            else if (flag == 0 && c_count == 5)
                                            {
                                                id = temp;
                                            }
                                            outfile << temp << "\n";
                                            temp.clear();
                                            temp.resize(MAX_STRING_LENGTH);
                                            k = 0;
                                            j++;
                                        }
                                    }
                                }
                            }
                            outfile.close();
                            stringstream ss(id);
                            T num;
                            ss >> num;
                            node.insert(num, line, path, txtpath);
                        }
                        i++, line++;
                    }
                }
                else
                {
                    cout << "\nCould not create file\n";
                }
                file.close();
            }
        }

        else if (choice == 5)
        {
            wchar_t dir[1024] = L"C:/B+Deaths";
            CreateFolder(dir); //creating folder based on indexing type
            ifstream file;
            int i = 0;
            for (int j = 1; j <= limit; j++)
            {
                string path;
                if (j < 10)
                {
                    path = fpath + "/NCHS_-_Leading_Causes_of_Death__United_States/DeathRate_0";
                }
                else
                {
                    path = fpath + "/NCHS_-_Leading_Causes_of_Death__United_States/DeathRate_";
                }
                stringstream j1;
                string jstr;
                j1 << j;
                j1 >> jstr;

                path += jstr;
                path += ".csv";
                cout << path << "\n";
                file.open(path);
                string x;

                int line = 0;
                if (file.good())
                {
                    cout << "\nFile open\n";
                    while (!file.eof())
                    {
                        getline(file, x);
                        cout << x << "\n";
                        if (line != 0)
                        {
                            string txtpath = "C:/B+Deaths/";
                            string tmptxt = txtpath;
                            string istr;
                            stringstream s1;
                            s1 << i;
                            s1 >> istr;
                            txtpath += istr;
                            txtpath += ".txt";
                            ofstream outfile;
                            outfile.open(txtpath);
                            int id = 0;
                            int flag = 0;
                            if (outfile.good())
                            {
                                int j = 0, k = 0, c_count = 0;
                                string temp = "\0";
                                temp.resize(MAX_STRING_LENGTH);
                                while (x[j])
                                {
                                    if (x[j + 1] == '\0')
                                    {
                                        temp[k] = x[j];
                                        outfile << temp << "\n";
                                        outfile << tmptxt << i + 1 << ".txt";
                                        temp.clear();
                                        temp.resize(MAX_STRING_LENGTH);
                                        k = 0;
                                        j++;
                                        continue;
                                    }

                                    if (x[j] != ',')
                                    {
                                        if (j - 1 >= 0)
                                        {
                                            if (x[j] == 'V' && x[j - 1] == '(')
                                            {
                                                flag = 1;
                                            }
                                            if (x[j] == 'I' && x[j - 1] == '(')
                                            {
                                                flag = 2;
                                            }
                                            if (x[j] == 'N' && x[j - 1] == '"')
                                            {
                                                flag = 3;
                                            }
                                            if (x[j] == '*' && x[j - 1] == '(')
                                            {
                                                flag = 4;
                                            }
                                        }
                                        temp[k] = x[j];
                                        k++, j++;
                                    }
                                    else
                                    {
                                        c_count++;
                                        if (c_count <= 2)
                                        {
                                            stringstream str(temp);
                                            int numeric_data = 0;
                                            str >> numeric_data;
                                            outfile << numeric_data << "\n";
                                            temp.clear();
                                            temp.resize(MAX_STRING_LENGTH);
                                            k = 0;
                                            j++;
                                        }
                                        else
                                        {
                                            if (flag == 1 && c_count == 3)
                                            {
                                                temp[k] = x[j];
                                                k++, j++;
                                                continue;
                                            }
                                            else if ((flag == 2 || flag == 3) && (c_count == 3 || c_count == 4 || c_count == 5))
                                            {
                                                temp[k] = x[j];
                                                k++, j++;
                                                continue;
                                            }
                                            else if (flag == 4 && (c_count == 3 || c_count == 4))
                                            {
                                                temp[k] = x[j];
                                                k++, j++;
                                                continue;
                                            }

                                            if (flag == 1 && c_count == 7)
                                            {
                                                stringstream str(temp);
                                                int numeric_data = 0;
                                                str >> numeric_data;
                                                id = numeric_data;
                                            }
                                            else if ((flag == 2 || flag == 3) && c_count == 9)
                                            {
                                                stringstream str(temp);
                                                int numeric_data = 0;
                                                str >> numeric_data;
                                                id = numeric_data;
                                            }
                                            else if (flag == 4 && c_count == 8)
                                            {
                                                stringstream str(temp);
                                                int numeric_data = 0;
                                                str >> numeric_data;
                                                id = numeric_data;
                                            }
                                            else if (flag == 0 && c_count == 6)
                                            {
                                                stringstream str(temp);
                                                int numeric_data = 0;
                                                str >> numeric_data;
                                                id = numeric_data;
                                            }
                                            outfile << temp << "\n";
                                            temp.clear();
                                            temp.resize(MAX_STRING_LENGTH);
                                            k = 0;
                                            j++;
                                        }
                                    }
                                }
                            }
                            stringstream ss(id);
                            T num;
                            ss >> num;
                            node.insert(num, line, path, txtpath);
                        }
                        i++, line++;
                    }
                }
                else
                {
                    cout << "\nCould not create file\n";
                }
                file.close();
            }
        }

        else if (choice == 6)
        {
            wchar_t dir[1024] = L"C:/B+AgeDeath";
            CreateFolder(dir); //creating folder based on indexing type
            ifstream file;
            int i = 0;
            for (int j = 1; j <= limit; j++)
            {
                string path;
                if (j < 10)
                {
                    path = fpath + "/NCHS_-_Leading_Causes_of_Death__United_States/DeathRate_0";
                }
                else
                {
                    path = fpath + "/NCHS_-_Leading_Causes_of_Death__United_States/DeathRate_";
                }
                stringstream j1;
                string jstr;
                j1 << j;
                j1 >> jstr;

                path += jstr;
                path += ".csv";
                cout << path << "\n";
                file.open(path);
                string x;

                int line = 0;
                if (file.good())
                {
                    cout << "\nFile open\n";
                    while (!file.eof())
                    {
                        getline(file, x);
                        cout << x << "\n";
                        if (line != 0)
                        {
                            string txtpath = "C:/B+AgeDeath/";
                            string tmptxt = txtpath;
                            string istr;
                            stringstream s1;
                            s1 << i;
                            s1 >> istr;
                            txtpath += istr;
                            txtpath += ".txt";
                            ofstream outfile;
                            outfile.open(txtpath);
                            double id = 0;
                            int flag = 0;
                            if (outfile.good())
                            {
                                int j = 0, k = 0, c_count = 0;
                                string temp = "\0";
                                temp.resize(MAX_STRING_LENGTH);
                                while (x[j])
                                {
                                    if (x[j + 1] == '\0')
                                    {
                                        temp[k] = x[j];

                                        stringstream str(temp);
                                        double numeric_data = 0.0;
                                        str >> numeric_data;
                                        id = numeric_data;
                                        stringstream ss(id);
                                        T num;
                                        ss >> num;
                                        node.insert(num, line, path, txtpath);


                                        outfile << temp << "\n";
                                        outfile << tmptxt << i + 1 << ".txt";
                                        temp.clear();
                                        temp.resize(MAX_STRING_LENGTH);
                                        k = 0;
                                        j++;
                                        continue;
                                    }

                                    if (x[j] != ',')
                                    {
                                        if (j - 1 >= 0)
                                        {
                                            if (x[j] == 'V' && x[j - 1] == '(')
                                            {
                                                flag = 1;
                                            }
                                            if (x[j] == 'I' && x[j - 1] == '(')
                                            {
                                                flag = 2;
                                            }
                                            if (x[j] == 'N' && x[j - 1] == '"')
                                            {
                                                flag = 3;
                                            }
                                            if (x[j] == '*' && x[j - 1] == '(')
                                            {
                                                flag = 4;
                                            }
                                        }
                                        temp[k] = x[j];
                                        k++, j++;
                                    }
                                    else
                                    {
                                        c_count++;
                                        if (c_count <= 2)
                                        {
                                            stringstream str(temp);
                                            int numeric_data = 0;
                                            str >> numeric_data;
                                            outfile << numeric_data << "\n";
                                            temp.clear();
                                            temp.resize(MAX_STRING_LENGTH);
                                            k = 0;
                                            j++;
                                        }
                                        else
                                        {
                                            if (flag == 1 && c_count == 3)
                                            {
                                                temp[k] = x[j];
                                                k++, j++;
                                                continue;
                                            }
                                            else if ((flag == 2 || flag == 3) && (c_count == 3 || c_count == 4 || c_count == 5))
                                            {
                                                temp[k] = x[j];
                                                k++, j++;
                                                continue;
                                            }
                                            else if (flag == 4 && (c_count == 3 || c_count == 4))
                                            {
                                                temp[k] = x[j];
                                                k++, j++;
                                                continue;
                                            }
                                            outfile << temp << "\n";
                                            temp.clear();
                                            temp.resize(MAX_STRING_LENGTH);
                                            k = 0;
                                            j++;
                                        }
                                    }
                                }
                            }
                        }
                        i++, line++;
                    }
                }
                else
                {
                    cout << "\nCould not create file\n";
                }
                file.close();
            }
        }

        cout << "\n\nText files for B+ Tree are located in the following directory:\n1. C:/B+ID if indexing by ID\n2. C:/B+Year if indexing by Year\n3. C:/B+Cause if indexing by Cause\n"
            << "4. C:/B+State if indexing by State\n5. C:/B+Deaths\n6. C:/B+AgeDeath\n";
        cout << "\nThe text files have the following attributes in them:\n1. ID\n2. Year of Death\n3. 113 Cause Name\n"
            << "4. Cause Name\n5. State\n6. Number of Deaths\n7. Age-adjusted Death Rate\n8. Disk Location of the next element of the tree, for the B+ Tree\n\n";
    }
};


int main()
{
    int choice = INT_MAX, selection = INT_MAX, num = 0;
    cout << "\n______________________________________________________________________________________\n"
        << "Data Structures project created by:\n1. Ali Kamal (19I-1865)\n2. Shehroz Khan (19I-1751)\n3. Razi Haider (19I-1762)"
        << "\n______________________________________________________________________________________\n\n\n";


    cout << "\n______________________________________________________________________________________";
    cout << "\nPlease make a selection to choose what column you want to perform indexing on\nEnter the corresponding numbers in front of the choices\n";
    cout << "1- ID\n2- Year\n3- Cause\n4- State\n5- Deaths\n6- Age Adjusted Death Rate\n";
    cout << "______________________________________________________________________________________\n";
    cin >> choice;
    while (choice < 1 || choice>6 || cin.fail())
    {
        cin.clear();
        cin.ignore((numeric_limits<streamsize>::max)(), '\n');
        cout << "\nChoice must be between 1 and 6\n";
        cout << "\n______________________________________________________________________________________\n";
        cout << "\nPlease make a selection to choose what column you want to perform indexing on\nEnter the corresponding numbers in front of the choices";
        cout << "1- ID\n2- Year\n3- Cause\n4- State\n5- Deaths\n6- Age Adjusted Death Rate\n";
        cout << "\______________________________________________________________________________________\n";
        cin >> choice;
    }

    if (choice == 1 || choice == 2 || choice == 5)
    {
        DSDB<int> node;
        file_reader<int> f1;

        cout << "\nPlease enter how many .CSV files you want to read (between 1 and 10)\n";
        cin >> num;
        while (num < 1 || num>10 || cin.fail())
        {
            cin.clear();
            cin.ignore((numeric_limits<streamsize>::max)(), '\n');
            cout << "\nNumber of .CSV files must be between 1 and 10\nPlease enter the number of .CSV files you want to read again\n";
            cin >> num;
        }
        f1.readfile(choice, node, num);

        cout << "\nPlease enter what operation you want to perform\nPlease enter the number corresponding to the operation\n";
        do
        {
            cout << "\n______________________________________________________________________________________\n";
            cout << "1. Point Search\n2. Range Search\n3. Update key and value\n4. Delete key\n-1. EXIT THE PROGRAM";
            cout << "\n______________________________________________________________________________________\n";
            cin >> selection;
            while (selection < -1 || selection == 0 || selection>4)
            {
                cin.clear();
                cin.ignore((numeric_limits<streamsize>::max)(), '\n');
                cout << "\nPlease enter a valid choice only\n";
                cout << "\n______________________________________________________________________________________\n";
                cout << "1. Point Search\n2. Range Search\n3. Update key and value\n4. Delete key\n-1. EXIT THE PROGRAM";
                cout << "\n______________________________________________________________________________________\n";
                cin >> selection;
            }
            if (selection == 1)
            {
                int search_key = 0;
                cout << "\nPlease enter key for 'Point Search'\n";
                cin >> search_key;
                while (cin.fail())
                {
                    cin.clear();
                    cin.ignore((numeric_limits<streamsize>::max)(), '\n');
                    cout << "\nPlease only enter numeric values\n";
                    cin >> search_key;
                }
                node.point_search(search_key, selection);
            }
            else if (selection == 2)
            {
                int search_key = 0, search_key2 = 0;
                cout << "\nPlease enter key #1 for 'Range Search'\n";
                cin >> search_key;
                while (cin.fail())
                {
                    cin.clear();
                    cin.ignore((numeric_limits<streamsize>::max)(), '\n');
                    cout << "\nPlease only enter numeric values\n";
                    cin >> search_key;
                }
                cout << "\nPlease enter key #2 for 'Range Search'\n";
                cin >> search_key2;
                while (cin.fail())
                {
                    cin.clear();
                    cin.ignore((numeric_limits<streamsize>::max)(), '\n');
                    cout << "\nPlease only enter numeric values\n";
                    cin >> search_key;
                }

                if (search_key2 < search_key)
                {
                    cout << "\nSearch key # 2 entered was smaller than Search key # 1\nSwapping their values\n";
                    swap(search_key2, search_key);
                }
                node.range_search(search_key, search_key2, 1);
            }
            else if (selection == 3)
            {
                int k = 0, c = 0;
                string old = "\0", new_val = "\0";
                cout << "\nEnter key you want to update the value of\n";
                cin >> k;
                while (cin.fail() || k <= 0)
                {
                    cin.clear();
                    cin.ignore((numeric_limits<streamsize>::max)(), '\n');
                    cout << "\nKey must be a numeric value greater than 0\nPlease enter key again\n";
                    cin >> k;
                }
                if (choice == 1)
                {
                    cout << "\nChoose what field you want to change\nPlease enter the corresponding numbers besides the fields given\n";
                    cout << "1. Year\n2. Cause Name\n3. State\n4. Deaths\n5. Age Adjusted Death Rate\n";
                    cin >> c;
                    while (cin.fail() || (c < 1 || c>5))
                    {
                        cin.clear();
                        cin.ignore((numeric_limits<streamsize>::max)(), '\n');
                        cout << "\nChoice must be a numeric value between 1 and 5\nPlease enter choice again\n";
                        cin >> c;
                    }
                    cin.clear();
                    cin.ignore();
                    cout << "\nEnter old value of the field you want to change\n";
                    getline(cin, old);
                    cout << "\nEnter new value of the field you want to change\n";
                    getline(cin, new_val);
                }

                else if (choice == 2)
                {
                    cout << "\nChoose what field you want to change\nPlease enter the corresponding numbers besides the fields given\n";
                    cout << "1. Year\n2. Cause Name\n3. State\n4. Deaths\n5. Age Adjusted Death Rate\n";
                    cin >> c;
                    while (cin.fail() || (c <= 1 || c > 5))
                    {
                        cin.clear();
                        cin.ignore((numeric_limits<streamsize>::max)(), '\n');
                        if (c == 1)
                        {
                            cout << "\nYou cannot choose the same field that you're indexing on";
                        }
                        cout << "\nChoice must be a numeric value between 2 and 5\nPlease enter choice again\n";
                        cin >> c;
                    }
                    cin.clear();
                    cin.ignore();
                    cout << "\nEnter old value of the field you want to change\n";
                    getline(cin, old);
                    cout << "\nEnter new value of the field you want to change\n";
                    getline(cin, new_val);
                }

                else if (choice == 5)
                {
                    cout << "\nChoose what field you want to change\nPlease enter the corresponding numbers besides the fields given\n";
                    cout << "1. Year\n2. Cause Name\n3. State\n4. Deaths\n5. Age Adjusted Death Rate\n";
                    cin >> c;
                    while (cin.fail() || (c < 1 || c > 5) || c == 4)
                    {
                        cin.clear();
                        cin.ignore((numeric_limits<streamsize>::max)(), '\n');
                        if (c == 4)
                        {
                            cout << "\nYou cannot choose the same field that you're indexing on";
                        }
                        cout << "\nChoice must be a numeric value between 1 and 5\nPlease enter choice again\n";
                        cin >> c;
                    }
                    cin.clear();
                    cin.ignore();
                    cout << "\nEnter old value of the field you want to change\n";
                    getline(cin, old);
                    cout << "\nEnter new value of the field you want to change\n";
                    getline(cin, new_val);
                }

                node.update(k, c, old, new_val);
            }
            else if (selection == 4)
            {
                int delid = 0;
                cout << "\nEnter key to delete\n";
                cin >> delid;
                while (cin.fail() || delid <= 0)
                {
                    cin.clear();
                    cin.ignore((numeric_limits<streamsize>::max)(), '\n');
                    cout << "\nChoice must be a numeric value between greater than 0\nPlease enter choice again\n";
                    cin >> delid;
                }
                node.deletion(delid, choice);
            }
        } while (selection != -1);
        cout << "\n\nEXITING THE PROGRAM\n\n";
        exit(0);
    }

    else if (choice == 3 || choice == 4)
    {
        DSDB<string> node;
        file_reader<string> f1;

        cout << "\nPlease enter how many .CSV files you want to read (between 1 and 10)\n";
        cin >> num;
        while (num < 1 || num>10 || cin.fail())
        {
            cin.clear();
            cin.ignore((numeric_limits<streamsize>::max)(), '\n');
            cout << "\nNumber of .CSV files must be between 1 and 10\nPlease enter the number of .CSV files you want to read again\n";
            cin >> num;
        }
        f1.readfile(choice, node, num);

        cout << "\nPlease enter what operation you want to perform\nPlease enter the number corresponding to the operation\n";
        do
        {
            cout << "\n______________________________________________________________________________________\n";
            cout << "1. Point Search\n2. Range Search\n3. Update key and value\n4. Delete key\n-1. EXIT THE PROGRAM";
            cout << "\n______________________________________________________________________________________\n";
            cin >> selection;
            while (selection < -1 || selection == 0 || selection>4)
            {
                cin.clear();
                cin.ignore((numeric_limits<streamsize>::max)(), '\n');
                cout << "\nPlease enter a valid choice only\n";
                cout << "\n______________________________________________________________________________________\n";
                cout << "1. Point Search\n2. Range Search\n3. Update key and value\n4. Delete key\n-1. EXIT THE PROGRAM";
                cout << "\n______________________________________________________________________________________\n";
                cin >> selection;
            }
            if (selection == 1)
            {
                string search_key = 0;
                cout << "\nPlease enter key to search\n";
                getline(cin, search_key);
                node.point_search(search_key, selection);
            }
            else if (selection == 2)
            {
                cout << "\nUnfortunately, 'Range Search' only works if indexing by ID, Year, Deaths, or Age Adjusted Death Rate\nPlease index by a numeric value to perform range search\n";
            }
            else if (selection == 3)
            {
                string k;
                int c = 0;
                string old = "\0", new_val = "\0";
                cout << "\nEnter key you want to update the value of\n";
                getline(cin, k);

                if (choice == 3)
                {
                    cout << "\nChoose what field you want to change\nPlease enter the corresponding numbers besides the fields given\n";
                    cout << "1. Year\n2. Cause Name\n3. State\n4. Deaths\n5. Age Adjusted Death Rate\n";
                    cin >> c;
                    while (cin.fail() || (c < 1 || c>5) || c == 2)
                    {
                        cin.clear();
                        cin.ignore((numeric_limits<streamsize>::max)(), '\n');
                        if (c == 2)
                        {
                            cout << "\nYou cannot choose the same field that you're indexing on";
                        }
                        cout << "\nChoice must be a numeric value between 1 and 5\nPlease enter choice again\n";
                        cin >> c;
                    }
                    cin.clear();
                    cin.ignore();
                    cout << "\nEnter old value of the field you want to change\n";
                    getline(cin, old);
                    cout << "\nEnter new value of the field you want to change\n";
                    getline(cin, new_val);
                }

                else if (choice == 4)
                {
                    cout << "\nChoose what field you want to change\nPlease enter the corresponding numbers besides the fields given\n";
                    cout << "1. Year\n2. Cause Name\n3. State\n4. Deaths\n5. Age Adjusted Death Rate\n";
                    cin >> c;
                    while (cin.fail() || (c < 1 || c>5) || c == 3)
                    {
                        cin.clear();
                        cin.ignore((numeric_limits<streamsize>::max)(), '\n');
                        if (c == 3)
                        {
                            cout << "\nYou cannot choose the same field that you're indexing on";
                        }
                        cout << "\nChoice must be a numeric value between 1 and 5\nPlease enter choice again\n";
                        cin >> c;
                    }
                    cin.clear();
                    cin.ignore();
                    cout << "\nEnter old value of the field you want to change\n";
                    getline(cin, old);
                    cout << "\nEnter new value of the field you want to change\n";
                    getline(cin, new_val);
                }

                node.update(k, c, old, new_val);
            }
            else if (selection == 4)
            {
                string delid;
                cout << "\nEnter key to delete\n";
                getline(cin, delid);
                node.deletion(delid, choice);
            }
        } while (selection != -1);
        cout << "\n\nEXITING THE PROGRAM\n\n";
        exit(0);
    }

    else
    {
        DSDB<double> node;
        file_reader<double> f1;

        cout << "\nPlease enter how many .CSV files you want to read (between 1 and 10)\n";
        cin >> num;
        while (num < 1 || num>10 || cin.fail())
        {
            cin.clear();
            cin.ignore((numeric_limits<streamsize>::max)(), '\n');
            cout << "\nNumber of .CSV files must be between 1 and 10\nPlease enter the number of .CSV files you want to read again\n";
            cin >> num;
        }
        f1.readfile(choice, node, num);

        cout << "\nPlease enter what operation you want to perform\nPlease enter the number corresponding to the operation\n";
        do
        {
            cout << "\n______________________________________________________________________________________\n";
            cout << "1. Point Search\n2. Range Search\n3. Update key and value\n4. Delete key\n-1. EXIT THE PROGRAM";
            cout << "\n______________________________________________________________________________________\n";
            cin >> selection;
            while (selection < -1 || selection == 0 || selection>4)
            {
                cin.clear();
                cin.ignore((numeric_limits<streamsize>::max)(), '\n');
                cout << "\nPlease enter a valid choice only\n";
                cout << "\n______________________________________________________________________________________\n";
                cout << "1. Point Search\n2. Range Search\n3. Update key and value\n4. Delete key\n-1. EXIT THE PROGRAM";
                cout << "\n______________________________________________________________________________________\n";
                cin >> selection;
            }
            if (selection == 1)
            {
                double search_key = 0;
                cout << "\nPlease enter key to search\n";
                cin >> search_key;
                while (cin.fail())
                {
                    cin.clear();
                    cin.ignore((numeric_limits<streamsize>::max)(), '\n');
                    cout << "\nPlease only enter numeric values\n";
                    cin >> search_key;
                }
                node.point_search(search_key, selection);
            }
            else if (selection == 2)
            {
                double search_key = 0, search_key2 = 0;
                cout << "\nPlease enter key #1 for 'Range Search'\n";
                cin >> search_key;
                while (cin.fail())
                {
                    cin.clear();
                    cin.ignore((numeric_limits<streamsize>::max)(), '\n');
                    cout << "\nPlease only enter numeric values\n";
                    cin >> search_key;
                }
                cout << "\nPlease enter key #2 for 'Range Search'\n";
                cin >> search_key2;
                while (cin.fail())
                {
                    cin.clear();
                    cin.ignore((numeric_limits<streamsize>::max)(), '\n');
                    cout << "\nPlease only enter numeric values\n";
                    cin >> search_key;
                }

                if (search_key2 < search_key)
                {
                    cout << "\nSearch key # 2 entered was smaller than Search key # 1\nSwapping their values\n";
                    swap(search_key2, search_key);
                }
                node.range_search(search_key, search_key2, 1);
            }
            else if (selection == 3)
            {
                double k = 0.0;
                int c = 0;
                string old = "\0", new_val = "\0";
                cout << "\nEnter key you want to update the value of\n";
                cin >> k;
                while (cin.fail() || k <= 0)
                {
                    cin.clear();
                    cin.ignore((numeric_limits<streamsize>::max)(), '\n');
                    cout << "\nKey must be a numeric value greater than 0\nPlease enter key again\n";
                    cin >> k;
                }
                cout << "\nChoose what field you want to change\nPlease enter the corresponding numbers besides the fields given\n";
                cout << "1. Year\n2. Cause Name\n3. State\n4. Deaths\n5. Age Adjusted Death Rate\n";
                cin >> c;
                while (cin.fail() || (c < 1 || c >= 5))
                {
                    cin.clear();
                    cin.ignore((numeric_limits<streamsize>::max)(), '\n');
                    if (c == 5)
                    {
                        cout << "\nYou cannot choose the same field that you're indexing on";
                    }
                    cout << "\nChoice must be a numeric value between 1 and 4\nPlease enter choice again\n";
                    cin >> c;
                }
                cin.clear();
                cin.ignore();
                cout << "\nEnter old value of the field you want to change\n";
                getline(cin, old);
                cout << "\nEnter new value of the field you want to change\n";
                getline(cin, new_val);

                node.update(k, c, old, new_val);
            }
            else if (selection == 4)
            {
                double delid = 0.0;
                cout << "\nEnter key to delete\n";
                cin >> delid;
                while (cin.fail() || delid <= 0)
                {
                    cin.clear();
                    cin.ignore((numeric_limits<streamsize>::max)(), '\n');
                    cout << "\nChoice must be a numeric value between greater than 0\nPlease enter choice again\n";
                    cin >> delid;
                }
                node.deletion(delid, choice);
            }
        } while (selection != -1);
        cout << "\n\nEXITING THE PROGRAM\n\n";
        exit(0);
    }


    return 0;
}