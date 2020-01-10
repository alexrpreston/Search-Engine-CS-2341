#ifndef USERINTERFACE_H
#define USERINTERFACE_H
#include "documentparser.h"
#include "queryprocessor.h"
class userInterface
{
public:
    userInterface();
    void maintenanceMode(documentParser &parser, queryProcessor &processor);
    void interactiveMode(documentParser &parser, queryProcessor &processor);
    void mainMenu(documentParser &parser, queryProcessor &processor);
    documentParser buffer();
    AVLTree<string> tree;
    HashMap<string> map;
    IndexInterface * II = &tree; //Every class needs to modify this II
};

#endif // USERINTERFACE_H
