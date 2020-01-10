#ifndef AVLTREE_H
#define AVLTREE_H

#include <iostream>
#include <string>
#include <cstdio>
#include <algorithm>
#include <utility>
#include <vector>
#include <IndexInterface.h>
#include <string.h>
#include <stdio.h>
#include <ostream>
#include <fstream>

using namespace std;

template<class T>
struct Node{
    //pair<T,vector<T>> data; // each word has a vector of the docs that contain the word
    pair<T, vector<pair<T, int>>> data;
    Node * left = nullptr;
    Node * right = nullptr;
    int height = 0;
};

template<class T>
class AVLTree : public IndexInterface {
private:
    Node<string> * root = nullptr;
    int numNodes = 0;
    ofstream outFile;

public:
    AVLTree(){
        //outFile.open("/home/student/Desktop/index.txt");
    }
    ~AVLTree(){
        destroy(root);
    }
    void destroy(){
        destroy(root);
    }
    void destroy(Node<T> * node){
        if(node){
            destroy(node->left);
            destroy(node->right);
            delete node;
        }
    }

    int getNumNodes(){
        return numNodes;
    }

    int height(Node<T> * N)
    {
        if (N == nullptr)
            return 0;
        return N->height;
    }

    int max(int a, int b){
        return (a > b)? a : b;
    }

    Node<T> * newNode(T key){
        Node<T> * node = new Node<T>();
        node->data.first = key;
        node->left = nullptr;
        node->right = nullptr;
        node->height = 1; // new node is initially
                          // added at leaf
        return(node);
    }

    Node<T> * llRotate(Node<T> * parent){
        Node<T> * k1 = parent->left;
        parent->left = k1->right;
        k1->right = parent;
        parent->height = max(height(parent->left), height(parent->right)) + 1;
        k1->height = max(height(k1->left), height(k1->right)) + 1;
        if(parent == root){
            root = k1;
        }
        return k1;
    }

    Node<T> * rrRotate(Node<T> * parent){
        Node<T> * k1 = parent->right;
        parent->right = k1->left;
        k1->left = parent;
        parent->height = max(height(parent->left), height(parent->right)) + 1;
        k1->height = max(height(k1->left), height(k1->right)) + 1;
        if(parent == root){
            root = k1;
        }
        return k1;
    }

    Node<T> * rlRotate(Node<T> * parent){
        Node<T> * k = parent->right;
        parent->right = llRotate(k);
        return rrRotate(parent);
    }

    Node<T> * lrRotate(Node<T> * parent){
        Node<T> * k = parent->left;
        parent->left = rrRotate(k); //sets to ll case, works
        return llRotate(parent);

    }

    int getBalance(Node<T> * Node){
        if (Node == nullptr)
            return 0;
        return height(Node->left) - height(Node->right);
    }

    void addFirst(T data){
        addFirst(root, data);
    }

    Node<T> * addFirst(Node<T> * node, T data){
        if(root == nullptr){
            Node<T> * nNode = newNode(data);
            root = nNode;
        }
        if (node == nullptr){
            Node<T> * nNode = newNode(data);
            nNode->left = nullptr;
            nNode->right = nullptr;
            numNodes++;
            return(nNode);
        }

        if (data < node->data.first){
            node->left = addFirst(node->left, data);
        }
        else if (data > node->data.first){
            node->right = addFirst(node->right, data);
        }
        else{
            return node;
        }

        node->height = 1 + max(height(node->left),
                            height(node->right));

        int balance = getBalance(node);

        if (balance > 1 && data < node->left->data.first) // ll
            return llRotate(node);

        if (balance < -1 && data > node->right->data.first) // rr
            return rrRotate(node);

        if (balance > 1 && data > node->left->data.first) // lr
        {
            return lrRotate(node); //returns new pviot
        }

        if (balance < -1 && data < node->right->data.first)  // rl
        {
            return rlRotate(node);
        }

        return node;
    }

    void preOrder(){
        preOrder(root);
    }

    void preOrder(Node<T> * root){

        cout << "no" << endl;

        if(root != nullptr){
            cout << root->data.first << endl;
            preOrder(root->left);
            preOrder(root->right);
        }
    }

    void pof(){
        outFile.open("index.txt");
       // outFile.open("/home/student/Desktop/index.txt", ios::out | ofstream::app);
        outFile << getNumNodes() << endl;
        preOrderFile(root);
        outFile.close();
    }

    void preOrderFile(Node<T> * curr){

        if(curr !=nullptr){
            preOrderFile(curr->left);
            outFile << curr->data.first << "|";
            if(curr->data.second.size() != 0){
                for(int i = 0; i < curr->data.second.size(); i++){
                    outFile << curr->data.second[i].first << ":" << curr->data.second[i].second;
                    outFile << "-";
                }
            }
            outFile << endl;
            preOrderFile(curr->right);
        }
       // outFile.close();
    }

    void access(T data, vector<pair<T, int>> &docs){
        return access(data, root, docs);
    }

    void access(T data, Node<T> * curr, vector<pair<T, int>> &docs){ // return value can be changed for query
        if(strcmp(curr->data.first.c_str(), data.c_str()) < 0){ //neg if search is larger
            curr = curr->right;
            access(data, curr, docs);
        }
        else if(strcmp(curr->data.first.c_str(), data.c_str()) > 0){ // if search is larger
            curr = curr->left;
            access(data, curr, docs);
        }
        else if(strcmp(curr->data.first.c_str(), data.c_str()) == 0){
            docs = curr->data.second;
        }
        else{
        }
   }



    void addSec(T data, T doc){
        addSec(data, doc, root);
    }

    void addSec(T data, T newDoc, Node<T> * curr){
        if(strcmp(curr->data.first.c_str(), data.c_str()) < 0){ //neg if search is larger
            curr = curr->right;
            addSec(data, newDoc, curr);
        }
        else if(strcmp(curr->data.first.c_str(), data.c_str()) > 0){ // if search is larger
            curr = curr->left;
            addSec(data, newDoc, curr);
        }
        else if(strcmp(curr->data.first.c_str(), data.c_str()) == 0){ // this is where tne doc counting is gonna happen
            int temp = -1;
            //temp = curr->data.second.find(0, 101, newDoc); //find doesnt exist
            if(curr->data.second.size() == 0){
                curr->data.second.push_back(make_pair(newDoc, 1));
                return;
            }
            for(int i = 0; i < curr->data.second.size(); i++){
                if(curr->data.second[i].first == newDoc){
                    curr->data.second[i].second++;
                    return;
                }
                if(i == curr->data.second.size()-1){
                    pair<T,int> tempPair;
                    curr->data.second.push_back(make_pair(newDoc, 1));
                    return;
                }
            }

            return;
        }
        else
            return;
    }

    void addSecF(T data, T newDoc, Node<T> * curr, int value){
        if(strcmp(curr->data.first.c_str(), data.c_str()) < 0){ //neg if search is larger
            curr = curr->right;
            addSec(data, newDoc, curr);
        }
        else if(strcmp(curr->data.first.c_str(), data.c_str()) > 0){ // if search is larger
            curr = curr->left;
            addSec(data, newDoc, curr);
        }
        else if(strcmp(curr->data.first.c_str(), data.c_str()) == 0){ // this is where tne doc counting is gonna happen
                curr->data.second.push_back(make_pair(newDoc, value));
            return;
        }
        else
            return;
    }

    void add(T data, T data2){
        Node<T> * curr;
        curr = addFirst(root, data);
        addSec(data, data2, curr);
        return;
    }

    void addF(T data, T data2, int times){
        Node<T> * curr;
        curr = addFirst(root, data);
        addSecF(data, data2, curr, times);
        return;
    }

    void rf(){
      //  cout << "hi" << endl;
        ifstream file;
        file.open("index.txt");
        string line = "";
        getline(file, line);
        while(getline(file, line)){
            size_t wordIndex = line.find("|");
            string word = line.substr(0,wordIndex); //Here is the word
            string info =  line.substr(wordIndex+1, line.length()-1);
            char sentence[10000];
            strcpy(sentence, info.c_str());
            char * token = strtok(sentence, "-");

            while(token != NULL){

                string IDandFreq = token;
                token = strtok(NULL, "-");


                size_t IDIndex = IDandFreq.find(":");
                string ID = IDandFreq.substr(0,IDIndex); //Here is ID
                string frequencyStr = IDandFreq.substr(IDIndex+1, IDandFreq.length()-1);
                int frequency = stoi(frequencyStr); //Here is Freq
                    addF(word, ID, frequency);
             }

        }
        file.close();
    }

    bool isEmpty(){
        if(root == nullptr){
            return true;
        }
        else
            return false;
    }

};



#endif // AVLTREE_H
