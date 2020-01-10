
#ifndef QUERYPROCESSOR_H
#define QUERYPROCESSOR_H


#include "string"
#include "documentparser.h"
#include "vector"
#include "HashMap.h"
#include "AVLTree.h"
#include "chrono"
using namespace std;

class queryProcessor
{
public:
    queryProcessor(IndexInterface * &II);

    void querySearch(string query);
    void singleQuery(string query);
    void orQuery(string query);
    void andQuery(string query);
    void notQuery(string query);
    void spliceQueryWords(string query);
    void spliceNotWords(string notQueryWords);
    void removeRepeats();
    void mergeAllDocuments();
    void getNotQueryDocs();
    void removeNotQueryDocs();
    void sortFinalDocsByFrequency();
    void printDocuemnts();
    vector<string> folders;
    string expand(int num);
    vector<vector<pair<string, int>>> allDocuments;
    vector<pair<string, int>> finalDocuments;
    vector<pair<string,int>> notQueryDocs;
    documentParser (formatedFilePath);
    vector<string> splicedNotWords;
    vector<string> splicedQueryWords;
    AVLTree<string> tree;
    HashMap<string> map;
    IndexInterface * II = &tree;
    documentParser parser;


private:
    string query = "";

};

#endif // QUERYPROCESSOR_H
