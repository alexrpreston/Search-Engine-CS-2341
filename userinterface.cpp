#include "userinterface.h"
using namespace std;
#include "documentparser.h"
#include "iostream"
#include "IndexInterface.h"
#include "queryprocessor.h"
#include "AVLTree.h"
#include "HashMap.h"
#include "stdlib.h"
userInterface::userInterface(){
    documentParser parser(II);
    queryProcessor processor(II);
    mainMenu(parser, processor);

}

void userInterface::mainMenu(documentParser &parser, queryProcessor &processor){
    int choice = 0;
    do{
        cout << "1. Maintence mode" << "\n";
        cout << "2. Interactive mode" << "\n";
        cout << "3. Exit " << "\n" ;
        cout << "Enter an option: ";
        cin >> choice;
        system("clear");
        switch(choice){
            case 1:
                maintenanceMode(parser, processor);
                break;
            case 2:
                interactiveMode(parser, processor);
                break;
            case 3:
                break;
        default:
                cout << "Not a valid option" << "\n\n\n\n";
        }
    }while(choice != 3);
}

void userInterface::maintenanceMode(documentParser &parser, queryProcessor &processor){
    int choice = 0;
    do{
        cout << "1. Add Opinions to the index." << "\n";
        cout << "2. Clear the Index." << "\n";
        cout << "3. Parse the corpus and populate index" << "\n";
        cout << "4. Go back to main menu." << "\n";
        cout << "5. Build Index from txt file" << "\n";
        cout << "6. Write Persistent Index" << "\n";
        cout << "7. Exit " << "\n";
        cout << "Enter an option: ";
        cin >> choice;
        system("clear");
        if(choice == 1){
                string folderPath;
                cout << "Enter a folder path: ";
                cin >> folderPath;
                char * formatedFilePath = &folderPath[0];
                parser.addOpinions(formatedFilePath);
                processor.parser.folderPath = formatedFilePath;
                cout << "Opinions added to corpus" << endl;
        }
        if(choice == 2){

        }
        if(choice == 3){
            parser.getFileNames("/home/student/Desktop/scotus-med/");
        }
        if(choice == 4){
            mainMenu(parser, processor);
        }
        if(choice == 5){
            processor.II->rf();
            mainMenu(parser, processor);
        }
        if(choice == 6){
            processor.II->pof();
            mainMenu(parser, processor);
        }
        
        
    }while(choice != 7);
}

void userInterface::interactiveMode(documentParser &parser, queryProcessor &processor){
    int choice = 0;
    do{
        cout << "1. Choose Data Structure Type." << "\n";
        cout << "2. Enter a Query." << "\n";
        cout << "3. Print Basic Statistics" << "\n";
        cout << "4. Go back to main menu." << "\n";
        cout << "5. Exit " << "\n";
        cout << "Enter an option: ";
        cin >> choice;
        system("clear");
        if(choice == 1){
            char dataStrChoice = ' ';
            if(parser.avlT){
                cout << "The search engine is currently running the AVL tree." << endl;
            }
            else{
                cout << "The search engine is currently running the Hash map." << endl;
            }
            cout << "Would you like to switch (Y/N): ";
            cin >> dataStrChoice;
            if(dataStrChoice == 'Y'){
                parser.dataTypes();
            }
            mainMenu(parser, processor);
        }
        if(choice == 2){
            string query = "";
            int searchResult = -1;
            cout << "=== Search the Corpus ===" << "\n\n";
            cout << "Enter Query: ";
            cin.ignore();
            getline(cin, query);
            processor.querySearch(query);
            cout << "\n\nEnter a number of the search results \n"
                    "you would like to expand, between " << "1 and " <<  processor.finalDocuments.size() << ":";
            cin >> searchResult;
            string expansion = processor.expand(searchResult-1);
            cout <<"\n";
            cout << expansion << endl;
            cout <<"\n";

        }
        if(choice == 3){
            cout << "Total number of opinions indexed: " << parser.getTotalDocumentsParsed() << "\n";
            cout << "Average number of words indexed per opinion: " << parser.getAvergeWordsPerDocument() << "\n";
        }
        if(choice == 4){
            mainMenu(parser, processor);
        }


    }while(choice != 5);
}


