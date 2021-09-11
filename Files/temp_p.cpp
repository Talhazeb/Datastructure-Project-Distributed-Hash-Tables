#include <iostream>
#include<fstream>
#include <time.h>

#include"sha1.h"
#include <boost/multiprecision/cpp_int.hpp>

using namespace std;
using namespace boost::multiprecision;

//===========================CIRCULAR LINKED LIST STORING MACHINE IDS==================================//
template<typename T>
class Node
{
public:
    T id;   //Machine ID

    Node<T>* next;
    Node()
    {
        id = 0;
        next = NULL;
    }
};


template<typename T>
class List
{
private:
    Node<T>* head;
    int count = 0;
public:
    List()
    {
        head = NULL;
    }

    //-------Circular Linked List Insertion-----------//
    void insert(T i, int s)
    {
        if (count >= pow(s, 2))
        {
            cout << "\n error";
            return;
        }
        count++;
        Node<T>* ptr;
        Node<T>* newnode;
        newnode = new Node<T>;

        newnode->id = i;

        if (head == NULL) 
        {
            head = newnode;
            newnode->next = head;
        }
        if (head != NULL)
        {
            ptr = head;

            while (ptr->next != head)
                ptr = ptr->next;

            ptr->next = newnode;
            newnode->next = head;
        }
    }

    void Delete(T i) {
        Node<T>* ptr;
        Node<T>* ptrtr = NULL;

        ptr = head;

        while (ptr->id != i)
        {
            ptrtr = ptr;
            ptr = ptr->next;
        }
        ptrtr->next = ptr->next;

        delete ptr;
    }

    //---------Nearest Active Machine Finder---------------//
    int find(T h_k, int num)
    {
        Node<T>* ptr_n;
        Node<T>* ptr_p;

        ptr_n = head;
        ptr_p = head;

        for (int i = 0; i < num; i++)
        {
            if (i == 0)
            {
                if (ptr_n->id == h_k)
                {
                    return ptr_n->id;
                }
                else
                {
                    ptr_n = ptr_n->next;
                }
            }
            else
            {
                if (ptr_n->id == h_k)
                {
                    return ptr_n->id;
                }
                else if (ptr_n->id > h_k && h_k > ptr_p->id)
                {
                    return ptr_n->id;
                }
                else if (h_k > ptr_n->id && ptr_n->next == head)
                {
                    return head->id;
                }
                else
                {
                    ptr_p = ptr_n;
                    ptr_n = ptr_n->next;
                }
            }
        }

    }

    void Display()
    {
        Node<T>* ptr;

        ptr = head;
        do
        {
            cout << ptr->id << endl;

            ptr = ptr->next;
        } while (ptr != head);
    }
};

//=======================AVL TREE FOR INDEXING=================================//

template<typename T, typename T1>
class AVL_Tree
{
    struct node
    {
        T Key;
        T1 Value;
        node* left;
        node* right;
        int height;
    };


    node* root;

    int height(node* tree)
    {
        if (tree == NULL)
            return -1;
        else
            return tree->height;
    }


    node* InsertInAVL(T k, T1 V, node* tree)
    {

        if (tree == NULL)
        {
            tree = new node;
            tree->Key = k;
            tree->Value = V;
            tree->height = 0;
            tree->left = tree->right = NULL;
        }
        else if (k < tree->Key)
        {
            tree->left = InsertInAVL(k, V, tree->left);
            if (height(tree->left) - height(tree->right) == 2)
            {
                if (k < tree->left->Key)
                    tree = singleRightRotate(tree);
                else
                    tree = Double_R_R(tree);
            }
        }
        else if (k > tree->Key)
        {
            tree->right = InsertInAVL(k, V, tree->right);
            if (height(tree->right) - height(tree->left) == 2)
            {
                if (k > tree->right->Key)
                    tree = singleLeftRotate(tree);
                else
                    tree = Double_L_R(tree);
            }
        }

        tree->height = max(height(tree->left), height(tree->right)) + 1;
        return tree;   
    }

    node* singleRightRotate(node*& tree)
    {
        node* temp = tree->left;
        tree->left = temp->right;
        temp->right = tree;
        tree->height = max(height(tree->left), height(tree->right)) + 1;
        temp->height = max(height(temp->left), tree->height) + 1;
        return temp;
    }

    node* singleLeftRotate(node*& tree)
    {
        node* temp = tree->right;
        tree->right = temp->left;
        temp->left = tree;
        tree->height = max(height(tree->left), height(tree->right)) + 1;
        temp->height = max(height(tree->right), tree->height) + 1;

        return temp;
    }

    node* Double_L_R(node*& tree)
    {
        tree->right = singleRightRotate(tree->right);
        return singleLeftRotate(tree);
    }

    node* Double_R_R(node*& tree)
    {
        tree->left = singleLeftRotate(tree->left);
        return singleRightRotate(tree);
    }

    node* findMin(node* tree)
    {
        if (tree == NULL)
            return NULL;
        else if (tree->left == NULL)
            return tree;
        else
            return findMin(tree->left);
    }

    node* find_maximum(node* tree)
    {
        if (tree == NULL)
            return NULL;
        else if (tree->right == NULL)
            return tree;
        else
            return find_maximum(tree->right);
    }

    node* remove(T x, T1 V, node* tree)
    {
        node* temp;

        //--------------------No Element
        if (tree == NULL)
            return NULL;

        //----------------------Resmue Searching  
        else if (x < tree->Key)
            tree->left = remove(x, V, tree->left);
        else if (x > tree->Key)
            tree->right = remove(x, V, tree->right);

        //---------------------------Two Children
        else if (tree->left && tree->right)
        {
            temp = findMin(tree->right);
            tree->Key = temp->Key;
            tree->Value = temp->Value;
            tree->right = remove(tree->Key, tree->Value, tree->right);
        }

        //--------------------------One or Zero Child
        else
        {
            temp = tree;
            if (tree->left == NULL)
                tree = tree->right;
            else if (tree->right == NULL)
                tree = tree->left;
            delete temp;
        }
        if (tree == NULL)
            return tree;

        tree->height = max(height(tree->left), height(tree->right)) + 1;


        if (height(tree->left) - height(tree->right) == 2)
        {

            if (height(tree->left->left) - height(tree->left->right) == 1)
                return singleLeftRotate(tree);

            else
                return Double_L_R(tree);
        }

        else if (height(tree->right) - height(tree->left) == 2)
        {

            if (height(tree->right->right) - height(tree->right->left) == 1)
                return singleRightRotate(tree);

            else
                return Double_R_R(tree);
        }
        return tree;
    }

    int getBalance(node* tree)
    {
        if (tree == NULL)
            return 0;
        else
            return height(tree->left) - height(tree->right);
    }

    void inorder(node* tree, T P)
    {
        
        if (tree == NULL)
            return;
        string a;
        inorder(tree->left, P);
        ifstream fi;

        fi.open(P + ".txt", ios::in);
        while (getline(fi, a))
        {
            cout << a << " ";
            cout << endl;
        }

        cout << endl;
        inorder(tree->right, P);
    }

public:
    AVL_Tree()
    {  
        root = NULL;
    }

    void insert2(T x, T1 V)
    {
        root = InsertInAVL(x, V, root);
    }


    void remove3(T x, T1 V)
    {
        root = remove(x, V, root);
    }

    void find(T P, T1 L)
    {
        node* nodePtr = root;

        while (nodePtr)
        {
            if (nodePtr->Value == L)
                break;
            else if (L < nodePtr->Value)
                nodePtr = nodePtr->left;
            else
                nodePtr = nodePtr->right;
        }

        cout << "NodePtr: " << nodePtr->Value << endl;

        if (nodePtr == NULL) 
        {
            cout << "\nData not found";
            return;
        }

        string l;
        ifstream fin;
        for (int i = 0; i < L; i++)
        {
            fin.open(P + ".txt", ios::in);
            getline(fin, l);
        }

        string line1;
        getline(fin, line1);
        cout << line1;
    }

    void display(T Pe)
    {
        inorder(root, Pe);
        cout << endl;
    }
};

//======================RANDOM STRING GENERATOR=======================
char generate_random_string()
{
    const char All_string[] =
                                 "0123456789"
                                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    int strLen = sizeof(All_string) - 1;

    return All_string[rand() % strLen];
}

//========================SHA 1 HASHING OF MACHINES=======================

int hasher_random(int id_s)
{
    SHA1 sha;
    string rand_id;

    for (unsigned int i = 0; i < 40; i++)
    {
        rand_id += generate_random_string();
    }

    string hex_string = sha(rand_id);

    sha.reset();

    int ss = hex_string.length();

    for (int i = 0; i < ss; i++)
    {
        if (hex_string[i] >= 97 && hex_string[i] <= 122)
        {
            hex_string[i] = hex_string[i] - 32;
        }
    }

    int256_t base = 1;
    int256_t decimal = 0;

    //===========HEX TO INT256 CONVERSION============//
    for (int i = ss - 1; i >= 0; i--)
    {
        if (hex_string[i] >= '0' && hex_string[i] <= '9')
        {
            decimal += (hex_string[i] - 48) * base;

            base = base * 16;
        }

        else if (hex_string[i] >= 'A' && hex_string[i] <= 'F')
        {
            decimal += (hex_string[i] - 55) * base;

            base = base * 16;
        }
    }

    int pp = pow(2, id_s);

    //=================ID BY MOD=================
    int256_t temp_id_256 = decimal % pp;

    //=============CONVERSION FROM INT265 TO INT===================
    int ret_hash = temp_id_256.template convert_to<int>();

    return ret_hash;

}

//==================SHA-1 HASHING OF KEY======================
int hasher_key_random(string k, int id_s)
{

    SHA1 sha;

    string hex_string = sha(k);

    sha.reset();

    int ss = hex_string.length();

    for (int i = 0; i < ss; i++)
    {
        if (hex_string[i] >= 97 && hex_string[i] <= 122)
        {
            hex_string[i] = hex_string[i] - 32;
        }
    }

    int256_t base = 1;
    int256_t decimal = 0;

    for (int i = ss - 1; i >= 0; i--)
    {
        if (hex_string[i] >= '0' && hex_string[i] <= '9')
        {
            decimal += (hex_string[i] - 48) * base;

            base = base * 16;
        }

        else if (hex_string[i] >= 'A' && hex_string[i] <= 'F')
        {
            decimal += (hex_string[i] - 55) * base;

            base = base * 16;
        }
    }

    int pp = pow(2, id_s);

    int256_t temp_id_256 = decimal % pp;

    int ret_hash = temp_id_256.template convert_to<int>();

    return ret_hash;

}

//========================BUBBLE SORTING FOR MACHINES===================
void Sort_bubble(int arr[], int s)
{
    for (int i = 0; i < s - 1; ++i)
    {
        for (int j = 0; j < s - i - 1; ++j)
        {
            //Ascending order
            if (arr[j] > arr[j + 1])
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

//===========================MAIN============================
int main()
{
    srand(time(0));
    int count = 0;

    int* arr = new int[count];
    
    int line = 0;
    int num_machine;
    int id_space;

    int p_b;

    cout << ".........::::::::::::: DISTRIBUTED NETWORK STORAGE ::::::::::::::::::...............\n\n";

    cout << "Input Identifier Space(0 bits - 160 bits) : ";
    cin >> id_space;

    while (id_space < 0 || id_space>160)
    {
        cout << "Input out of Range!\n\n";
        cout << "Input Identifier Space(0 bits - 160 bits) : ";
        cin >> id_space;
    }

    p_b = pow(2, id_space);

    cout << "\nInput number of Active Machines (Range: 0 - " << p_b << ") : ";
    cin >> num_machine;

    while (num_machine < 0 || num_machine>p_b)
    {
        cout << "Input out of Range!\n\n";
        cout << "\nInput number of Active Machines (Range: 0 - " << p_b << ") : ";
        cin >> id_space;
    }

    List <int>L;
    int MID;

    AVL_Tree<string, int> B;
    int opt;
    string k;
    string v;
    int temp = 0;

    /*for (int j = 0; j < id_space; j++)
    {
        L.insert(j, id_space);
    }*/

    arr = new int[num_machine];
    for (int i = 0; i < num_machine; i++)
    {
        arr[i] = 0;
    }

    cout << "\n...::: Assigning Machine IDS :::...\n";
    for (int i = 0; i < num_machine; i++)
    {
        cout << "\nAssigning ID to Machine " << i + 1 << " : \n";
        cout << "-----------------------------------\n";
        cout << "1. Assign Manually\n";
        cout << "2. Assign Automatically\n";
        cout << "\nInput Option : ";
        cin >> opt;

        if (opt == 1)
        {
            cout << "\nEnter Machine " << i + 1 << " id : ";
            cin >> MID;

            //arr[i] = MID;
            if (count == 0)
            {
                count++;

                while (MID < 0 || MID >= p_b)
                {
                    cout << "Input out of Range!\n\n";
                    cout << "\nEnter Machine " << i + 1 << " id : ";
                    cin >> MID;
                }

                arr[i] = MID;
            }
            else
            {
                count++;
                for (int j = 0; j < count - 1; j++)
                {
                    if (arr[i] == arr[j])
                    {
                        cout << "Error! Value less or equal to another ID\n";
                    }

                }
                arr[i] = MID;
            }
        }

        else if (opt == 2)
        {
            if (count == 0)
            {
                count++;
                MID = hasher_random(id_space);
                arr[i] = MID;
                cout << "--------------------------------------------\n";
                cout << "Auto Assingned ID to Machine " << i + 1 << " : " << arr[i] << endl;
                cout << "--------------------------------------------\n";
            }
            else
            {
                MID = hasher_random(id_space);
                arr[i] = MID;
                count++;
                for (int y = 0; y < count - 1; y++)
                {
                    if (arr[i] == arr[y])
                    {
                        i--;
                        count--;
                        break;
                    }
                }
                cout << "--------------------------------------------\n";
                cout << "Auto Assingned ID to Machine " << i + 1 << " : " << arr[i] << endl;
                cout << "--------------------------------------------\n";
            }
        }
    }


    Sort_bubble(arr, num_machine);
        
    for (int i = 0; i < num_machine; i++)
    {
        L.insert(arr[i], id_space);
    }
    cout << endl;
    
    

    int* line_arr = new int[num_machine];
    for (int m = 0; m < num_machine; m++)
    {
        line_arr[m] = 0;
    }



    cout << "Machines Added:::" << endl;
    L.Display();

    string key;
    string value;
    bool exi = false;
    int opt2;
    bool check;
    int hashed_key;
    string filename;

    int found_mach;

    string fil;
    string lin;

    int li;



    while (exi == false)
    {

        cout << "\n\n...:::Functionalities:::...\n";
        cout << "1. Insert Data (Key, Value Pair)\n";
        cout << "2. Remove data by filename and line\n";
        cout << "3. Remove Data by Key\n";
        cout << "4. Print Routing Table of any Machine\n";
        cout << "5. Print AVL tree of any Machine\n";
        cout << "6. Remove any Machine\n";
        cout << "7. Add new Machine\n";
        cout << "8. Search Data \n";
        cout << "9. Exit \n";
        cout << "Input Option: ";

        cin >> opt2;

        while (opt2 != 1 && opt2 != 2 && opt2 != 3 && opt2 != 4 && opt2 != 5 && opt2 != 6 && opt2 != 7 && opt2 != 8 && opt2 != 9)
        {
            cout << "Invalid option, Select Again!\n";
            cout << "Input Option: ";
            cin >> opt2;
        }

        if (opt2 == 1)
        {
            ofstream f;
            cout << "\nInput KEY: ";
            cin >> key;
            cout << "Input VALUE: ";
            cin >> value;


            hashed_key = hasher_key_random(key, id_space);

            cout << "hashed key = " << hashed_key;
            found_mach = L.find(hashed_key, num_machine);

            cout << "\nMachine found = " << found_mach << endl;

            filename = to_string(found_mach);


            f.open(filename + ".txt", ios::app);
            f << key << " " << value << endl;



            cout << "Line : " << line << endl;
            B.insert2(filename, line);

            line++;


            //line = 0;

        }

        else if (opt2 == 2)
        {
            string fil;
            int li;
            ifstream i;
            ofstream t;
            int c = 0;
            cout << "\nInput Filepath: ";
            cin >> fil;
            cout << "Input Line number: ";
            cin >> li;

            i.open(fil + ".txt", ios::in);

            t.open("temp.txt", ios::out);
            char ch;
            while (i.get(ch))
            {

                if (ch == '\n')
                    c++;


                if (c != li)
                    t << ch;
            }
            t.close();
            i.close();
            remove((fil + ".txt").c_str());
            rename("temp.txt", (fil + ".txt").c_str());
            B.remove3(fil, li);
            B.insert2(fil, li);
        }

        else if (opt2 == 5)
        {
            string po;
            cout << "\nEnter the file path of which you want to print AVL tree : ";
            cin >> po;
            cout << "\nAvl Tree of the machine : " << endl;
            B.display(po);

        }

        else if (opt2 == 6)
        {
            int remove_machine;
            bool found = false;

            cout << "Input ID of Machine to be removed: ";
            cin >> remove_machine;
            for (int n = 0; n < num_machine; n++)
            {
                if (remove_machine == arr[n])
                {
                    found = true;
                    cout << "\nMachine Found!\n";
                }
            }
            if (found == true)
            {
                ifstream i;

                ofstream t;

                ifstream t_m;

                ifstream n_m;

                ofstream new_m;

                int temp_lin = 0;

                int c = 0;

                cout << "\nMachine to remove: " << remove_machine << endl;
                    
                string rm = to_string(remove_machine);
                
                i.open(rm + ".txt", ios::in);

                t.open("temp.txt", ios::out);
                char ch;
                while (i.get(ch))
                {

                    if (ch == '\n')
                    {
                        //B.remove3(rm, c);
                        c++;
                    }

                    t << ch;

                }
                t.close();
                i.close();

                remove((rm + ".txt").c_str());

                
                int next_machine;
                for (int n = 0; n < num_machine; n++)
                {
                    if (remove_machine == arr[n])
                    {
                        if (arr[n + 1] == arr[num_machine-1])
                        {
                            next_machine = arr[0];
                        }
                        else
                            next_machine = arr[n + 1];

                    }
                }

                string n_m_s = to_string(next_machine);

                n_m.open(n_m_s + ".txt", ios::in);

                int line_count_m = 0;
                while (n_m.get(ch))
                {

                    if (ch == '\n')
                    {
                        //B.remove3(rm, c);
                        line_count_m++;
                    }

                }

                //cout << "Line count :" << line_count_m << endl;
                cout << "NExt MAchine: "<< n_m_s << endl;

                t_m.open("temp.txt", ios::in);

                new_m.open(n_m_s + ".txt", ios::out);
                int hehe = 0;

                while (t_m.get(ch))
                {

                    if (ch == '\n')
                    {
                        //B.remove3(rm, c);
                        hehe++;
                        //line_count_m++;
                        B.insert2(n_m_s, line_count_m);
                    }

                    if(hehe>=line_count_m)
                        new_m << ch;

                }
                t_m.close();
                new_m.close();


                remove("temp.txt");

                //B.remove3(fil, li);
                //B.insert2(fil, li);
            }
            else
            {
                cout << "\nMachine not Found!\n";
            }
        }

        if (opt2 == 8) {
            string Pi;
            int LN;
            cout << "\n Enter the File Path :";
            cin >> Pi;
            cout << "\n Enter the line Number :";
            cin >> LN;
            B.find(Pi, LN);
        }

        if (opt2 == 9)
        {
            exi = true;
            exit(0);
        }
    }
}

