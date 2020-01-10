#include "documentparser.h"
#include "iomanip"
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <dirent.h>
#include <algorithm>
#include <cctype>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <cstdio>
#include <locale>
#include <codecvt>
#include <chrono>
#include "../IndexInterface/Libraries/OleanderStemmingLibrary/include/olestem/stemming/english_stem.h"
#include "../IndexInterface/Libraries/rapidjson/include/rapidjson/document.h"
#include "../IndexInterface/Libraries/rapidjson/include/rapidjson/writer.h"
#include "../IndexInterface/Libraries/rapidjson/include/rapidjson/stringbuffer.h"
#include "../IndexInterface/Libraries/rapidjson/include/rapidjson/filereadstream.h"

using namespace rapidjson;

documentParser::documentParser(){
}

documentParser::documentParser(char * folderPath){ //Only used once to parse the files then gets stored the index.txt
    makeStopWords();
    getFileNames(folderPath);
}

documentParser::documentParser(char * persistentIndexPath, IndexInterface * &II){
    this->II = II; //This sets the AVL tree we created in the UI to the one we read all our files into.

    makeStopWords();
    getFileNames(persistentIndexPath);

}

documentParser::documentParser(IndexInterface *&II){
    this->II = II; //This sets the AVL tree we created in the UI to the one we read all our files into.
    makeStopWords();
}




void documentParser::dataTypes(){
    II->destroy();
    if(avlT){
        avlT == false;
    }
    else{
        avlT == true;
    }

    if(!avlT){
        II = &tree;
    }
    else{
        II = &map; //map doesnt extend II rn
    }
    II->rf(); //^^
}

string documentParser::getRelevantInfo(string filePath){
    string path = "/home/student/Desktop/scotus-med/";
    if(filePath[0] == '_'){
        cout << filePath << endl;
        filePath = filePath.substr(1, filePath.length());
        cout << filePath << endl;
        cout << folderPath << endl;
        path = folderPath;
    }
    string jsonEXT = ".json";
    string name = path + filePath + jsonEXT;
    FILE * fp = fopen(name.c_str(), "rb");
    char readBuffer[653666];
    FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    Document d;
    d.ParseStream(is);
    Value& s = d["html_with_citations"];
    string html = s.GetString();
    size_t endOfInfoIndex = html.find("Decided");
    html = html.substr(5, endOfInfoIndex+26);
    removeTags(html);
    cout <<  html;
}

string documentParser::getFirst300Words(string filePath){
    string path = "/home/student/Desktop/scotus-med/";
    if(filePath[0] == '_'){
        filePath = filePath.substr(1, filePath.length());
        path = folderPath;
    }
    int totalwords = 0;
    string expandedDoc = "";

    string jsonEXT = ".json";
    string name = path + filePath + jsonEXT;
    FILE * fp = fopen(name.c_str(), "rb");
    char readBuffer[6553666];
    FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    Document d;
    d.ParseStream(is);
    Value& s = d["html_with_citations"];
    string html = s.GetString();
    size_t endOfInfoIndex = html.find("Decided");
    html = html.substr(endOfInfoIndex+26, 1500);
    removeTags(html);

    char sentence[655565];
    strcpy(sentence, html.c_str());
    char * token = strtok(sentence, " ");
    while(token != NULL){
        string buffer = "";
        for(int i = 0; i < strlen(token); i++){
                if(isalpha(tolower(token[i]))) buffer += char(tolower(token[i]));
        }

        token = strtok(NULL, " ");

        word = buffer.c_str();
        if(totalwords < 300) expandedDoc = expandedDoc + " " + word;
        totalwords++;
     }
    expandedDoc += "..........";
    return expandedDoc;
}

int documentParser::getTotalDocumentsParsed() const{
    return totalDocumentsParsed;
}

void documentParser::addTotalDocumentsParsed(int value){
    totalDocumentsParsed += value;
}

double documentParser::getAvergeWordsPerDocument() const{
    double avg = avergeWordsPerDocument / getTotalDocumentsParsed();
    return avg;
}

void documentParser::addAvergeWordsPerDocument(int value){
    avergeWordsPerDocument += value;
}

bool documentParser::isStopWord(string &word){
    int left = 0;
    int right = stopWords.size()-1;
    makeLowerCase(word);
    if(word == wordToFind){
        appearsInDoc = true;
        wordToFindTotalOccurances++;
    }
    while(left <= right){
            int middle = left + (right - left) / 2;

            if(stopWords.at(middle) == word){
                return true;
            }

            if(stopWords.at(middle) < word){
                left = middle + 1;
            }

            else {
               right = middle - 1;
            }
    }

    return false;
}

void documentParser::makeLowerCase(string &word){
    transform(word.begin(), word.end(), word.begin(), ::tolower);
}

void documentParser::getFileNames(char *filePath){
    char * dir = filePath;
    this->filePath = filePath;
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    if((dp = opendir(dir)) == NULL) {
        return;
    }
    chdir(dir);
    while((entry = readdir(dp)) != NULL) {
        lstat(entry->d_name,&statbuf);
        if(S_ISDIR(statbuf.st_mode)) {
            if(strcmp(".",entry->d_name) == 0 ||
                strcmp("..",entry->d_name) == 0)
                continue;
            readDocumentsHTMLData(entry->d_name);
            addTotalDocumentsParsed(1);
        }
        readDocumentsHTMLData(entry->d_name);
        addTotalDocumentsParsed(1);
    }
    closedir(dp);
    chdir("../CSE2341-F19-Alex-Isaac/Sprint5/build-IndexInterface-Desktop_Qt_5_10_0_GCC_64bit-Debug/");
}

void documentParser::getAddedFileNames(string filePath){
    string added = "";
    const char * dir = filePath.c_str();
    this->filePath = filePath;
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    if((dp = opendir(dir)) == NULL) {
        return;
    }
    chdir(dir);
    while((entry = readdir(dp)) != NULL) {
        lstat(entry->d_name,&statbuf);
        if(S_ISDIR(statbuf.st_mode)) {
            if(strcmp(".",entry->d_name) == 0 ||
                strcmp("..",entry->d_name) == 0)
                continue;
            readAddedDocumentsHTMLData(entry->d_name);
            addTotalDocumentsParsed(1);
        }

        readAddedDocumentsHTMLData(entry->d_name);
        addTotalDocumentsParsed(1);
    }
    closedir(dp);
    chdir("../CSE2341-F19-Alex-Isaac/Sprint5/build-IndexInterface-Desktop_Qt_5_10_0_GCC_64bit-Debug/");
}

void documentParser::stemWord(string &unstemmedWord){
    stemming::english_stem<> StemEnglish;
    std::wstring stemmedword(L" ");
    std::string ANSIWord(unstemmedWord);
    wchar_t* UnicodeTextBuffer = new wchar_t[ANSIWord.length()+1];
    std::wmemset(UnicodeTextBuffer, 0, ANSIWord.length()+1);
    std::mbstowcs(UnicodeTextBuffer, ANSIWord.c_str(), ANSIWord.length());
    stemmedword = UnicodeTextBuffer;
    StemEnglish(stemmedword);
    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type,wchar_t>converter;

    unstemmedWord = converter.to_bytes(stemmedword);
    delete [] UnicodeTextBuffer;

}

string documentParser::stemQueryWord(string word){
    stemming::english_stem<> StemEnglish;
    std::wstring stemmedword(L" ");
    std::string ANSIWord(word);
    wchar_t* UnicodeTextBuffer = new wchar_t[ANSIWord.length()+1];
    std::wmemset(UnicodeTextBuffer, 0, ANSIWord.length()+1);
    std::mbstowcs(UnicodeTextBuffer, ANSIWord.c_str(), ANSIWord.length());
    stemmedword = UnicodeTextBuffer;
    StemEnglish(stemmedword);
    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type,wchar_t>converter;

    word = converter.to_bytes(stemmedword);
    delete [] UnicodeTextBuffer;
    return word;
}

void documentParser::readAddedDocumentsHTMLData(string filePath){
        string name = this->folderPath + filePath;
        FILE * fp = fopen(name.c_str(), "rb");
        char readBuffer[6553666];
        FileReadStream is(fp, readBuffer, sizeof(readBuffer));
        Document d;
        d.ParseStream(is);
        Value& s = d["html_with_citations"];


        fclose(fp);
        string shortFileName =  filePath.substr(0,filePath.size()-5);
        shortFileName = "_" + shortFileName;
        string html = s.GetString();

        removeTags(html);
        char sentence[655565];
        strcpy(sentence, html.c_str());
        char * token = strtok(sentence, " ");

       while(token != NULL){
           string buffer = "";
           for(int i = 0; i < strlen(token); i++){
                   if(isalpha(tolower(token[i]))) buffer += char(tolower(token[i]));
           }
           token = strtok(NULL, " ");

           word = buffer.c_str();
           if(word != ""){
               if(!isStopWord(word)){
                   stemWord(word);
                   addAvergeWordsPerDocument(1);
                   II->add(word, shortFileName);
               }
           }
        }
       if(appearsInDoc == true){
           wordToFindDocumentOccurances++;
           appearsInDoc = false;

        }
           if(appearsInDoc == true){
               wordToFindDocumentOccurances++;
               appearsInDoc = false;
           }
}


void documentParser::readDocumentsHTMLData(string filePath){
        string name = this->filePath + filePath;
        FILE * fp = fopen(name.c_str(), "rb");
        char readBuffer[6553666];
        FileReadStream is(fp, readBuffer, sizeof(readBuffer));
        Document d;
        d.ParseStream(is);
        Value& s = d["html_with_citations"];


        fclose(fp);
        string shortFileName =  filePath.substr(0,filePath.size()-5);
        string html = s.GetString();

        removeTags(html);
        char sentence[655565];
        strcpy(sentence, html.c_str());
        char * token = strtok(sentence, " ");

       while(token != NULL){
           string buffer = "";
           for(int i = 0; i < strlen(token); i++){
                   if(isalpha(tolower(token[i]))) buffer += char(tolower(token[i]));
           }
           token = strtok(NULL, " ");

           word = buffer.c_str();
           if(word != ""){
               if(!isStopWord(word)){
                   stemWord(word);
                   addAvergeWordsPerDocument(1);
                   II->add(word, shortFileName);
               }
           }
        }
       if(appearsInDoc == true){
           wordToFindDocumentOccurances++;
           appearsInDoc = false;

        }
           if(appearsInDoc == true){
               wordToFindDocumentOccurances++;
               appearsInDoc = false;
           }
}

void documentParser::makeStopWords(){
    stopWords.clear();
    fstream stopWordFile;
    stopWordFile.open("../IndexInterface/stopWords.txt");

    while(!stopWordFile.eof()){
        string stopWord;
        getline(stopWordFile, stopWord, '\n');
        stopWords.push_back(stopWord);
    }
}

//Starts search for tag at the end of html string to save time.
void documentParser::removeTags(string &html){
    int openBracket = 0;
    int closeBracket = 0;
    for(int i = html.length()-1; i > 0; i--){
        if(html[i] == '>'){
            closeBracket = i;
        }
        if(html[i] == '<'){
            openBracket = i;
            if(closeBracket != html.npos){
                html.erase(openBracket, closeBracket - openBracket+1);
            }
        }
    }
}

void documentParser::addOpinions(string folderPath){
    this->folderPath = folderPath;
    makeStopWords();
    getAddedFileNames(folderPath);
}
