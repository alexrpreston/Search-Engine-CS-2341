#ifndef INDEXINTERFACE_H
#define INDEXINTERFACE_H

#include <iostream>
#include <vector>

using namespace std;

class IndexInterface{
public:
    virtual ~IndexInterface(){}
    virtual void access(string data, vector<pair<string, int>> &docs) = 0;
    virtual void destroy() = 0;
    virtual void add(string, string) = 0;
    virtual void rf() = 0;
    virtual void pof() = 0;
    virtual bool isEmpty() = 0;
};


#endif // INDEXINTERFACE_H
