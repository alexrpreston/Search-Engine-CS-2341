#ifndef DOCUMENTPARSER_H
#define DOCUMENTPARSER_H

#include <string>
#include <vector>
#include "AVLTree.h"
#include "HashMap.h"
using namespace std;
class documentParser
{
public:
    documentParser();
    documentParser(char * folderPath);
    documentParser(char * persistentIndexPath, IndexInterface * &II);
    documentParser(IndexInterface * &II);
    bool isStopWord(string &word);
    void makeLowerCase(string &word);
    void getFileNames(char * filePath);
    void getAddedFileNames(string filePath);
    void stemWord(string &word);
    string stemQueryWord(string word);
    void readDocumentsHTMLData(string filePath);
    void readAddedDocumentsHTMLData(string filePath);
    void makeStopWords();
    void removeTags(string &html);
    void addOpinions(string filePath);
    void dataTypes();
    string getRelevantInfo(string filePath);
    string getFirst300Words(string filePath);
    string folderPath = "";
    AVLTree<string> tree;
    HashMap<string> map;
    IndexInterface * II = &tree;
    bool avlT = true;
    string word = "";
    vector<string>stopWords;
    vector<string>expandedOpinion;
    string wordToFind = "";
    int wordToFindTotalOccurances = 0;
    int wordToFindDocumentOccurances = 0;
    bool appearsInDoc = false;
    string filePath = "";
    string dataType = "";
    int avergeWordsPerDocument = 0;
    int totalDocumentsParsed = 0;

    double getAvergeWordsPerDocument() const;
    void addAvergeWordsPerDocument(int value);
    int getTotalDocumentsParsed() const;
    void addTotalDocumentsParsed(int value);
};

#endif // DOCUMENTPARSER_H
