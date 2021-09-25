/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * main.cpp
 * Copyright (C) 2012 Jon Fast <jon@PsyTP>
 * 
 * HuProj is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * HuProj is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <stack>
#include <cstdlib>
#include <cstring>
#include <vector>

using namespace std;

typedef struct HuNode {
    //data referenced here
    char data; //this is optional
    char LR; //indication of whether a left or right node
    double freq; //this is necessary for probabilities

    //data structure node references
    struct HuNode *lchild; //left child node
    struct HuNode *rchild; //right child node

} HuNode;

typedef struct HuTree {
    struct HuNode *root; //tree requires a root
    int length; //tree length, number of unique nodes
} HuTree;

typedef struct cNode {
    char data;
    string bitstring;
} cNode;

bool sortNodesByProbability(HuNode *i1, HuNode *i2) {
    if (i1->freq < i2->freq) {
        return true;
    } else {
        return false;
    }
}

void printNodeList(list<HuNode*> nodeList) {
    while (!nodeList.empty()) {
        HuNode *n = nodeList.back();
        cout << "Char: " << n->data << " Frequency: " << n->freq << endl;
        nodeList.pop_back();
    }
}

void traverseSubT(HuNode *n) {
    cout << "\tChar: " << n->data << " Frequency: " << n->freq << endl;
    if (n->lchild != nullptr)
        traverseSubT(n->lchild);
    if (n->rchild != nullptr)
        traverseSubT(n->rchild);
}

void printNodeListTraversal(list<HuNode*> nodeList) {
    while (!nodeList.empty()) {
        HuNode *n = nodeList.back();
        cout << "Char: " << n->data << " Frequency: " << n->freq << endl;
        if (n->lchild != nullptr) {
            traverseSubT(n->lchild);
        }
        if (n->rchild != nullptr) {
            traverseSubT(n->rchild);
        }
        nodeList.pop_back();
    }
}

int getNumUniqueNodes(list<HuNode*> nodeList) {
    //keep track of the sum
    int sum = 0;
    //pop nodes off list while empty, keeping track of how many we removed
    while (!nodeList.empty()) {
        nodeList.pop_front();
        sum++;
    }
    return sum;
}

void printCharList(list<char> charList) {
    list<char>::iterator it;
    for (it = charList.begin(); it != charList.end(); it++) {
        cout << " '" << *it << "', ";
    }
    cout << endl;
}

void printS(string printThis) {
    string::iterator it;

    for (it = printThis.begin(); it != printThis.end(); it++) {
        cout << " '" << *it << "', ";
    }
    cout << endl;
}

string readInFile(string directory) {
    //first create a string
    string line;
    string total;

    //filestream
    ifstream file(directory.c_str());

    // while open and not EOF
    if (file.is_open()) {
        while (file.good()) {
            //get line
            getline(file, line);
            //do something here (append to string)
            total += line;
            //clear the string
            line.clear();
        }
        file.close();
    } else {
        cout << "Unable to open file." << endl;
        total = "";
    }
    return total;
}

list<HuNode*> buildNodes(string expression) {

    //create a list of HuNodes
    list<HuNode*> nodeList;

    //create a list of chars along with an iterator
    list<char> charTypes;
    list<char>::iterator charIt = charTypes.begin();

    string expressionChars;
    int pos;
    //push items on the list
    for (int i = 0; i < expression.length(); i++) {
        if ((pos = expressionChars.find(expression[i])) < 0) {
            expressionChars += expression[i];
        }
    }
    for (int i = 0; i < expressionChars.length(); i++) {
        charTypes.push_back(expressionChars[i]);
    }
    //    cout << "Unsorted List:\n";
    //    printS(expressionChars);

    //remove duplicate elements
    charTypes.unique();
    //sort
    charTypes.sort();

    //    cout << "Sorted List:\n";
    //    printCharList(charTypes);

    //create an array of size charTypes
    const int charTypesSize = charTypes.size();
    std::vector<int> frequency;
    frequency.reserve(charTypesSize);

    //create the sum
    int counter;
    int charCounter;
    counter = 0;

    //go through each char in the list and determine its frequency
    for (charIt = charTypes.begin(); charIt != charTypes.end(); charIt++) {
        //get the current character being looked at
        char c = *charIt;
        charCounter = 0;

        //iterate through the original string and
        //if chars equal, then increase frequency
        for (int i = 0; i < expression.length(); i++) {
            if (c == expression[i]) {
                charCounter++;
            }
        }
        //add the associated frequency to the array
        frequency[counter] = charCounter;

        //increase the array counter
        counter++;
    }

    //    cout << "Sorted Frequency:" << endl;
    //    int freqSize = sizeof (frequency) / sizeof (frequency[0]);
    //    for (int i = 0; i < freqSize; i++) {
    //        cout << " '" << frequency[i] << "', ";
    //    }
    //    cout << endl;

    //counter reset
    counter = 0;

    //iterate through the charTypes list again to create tree nodes
    for (charIt = charTypes.begin(); charIt != charTypes.end(); charIt++) {
        HuNode *n = new HuNode;
        n->freq = frequency[counter];
        n->data = *charIt;
        n->lchild = nullptr;
        n->rchild = nullptr;
        n->LR = '*';
        nodeList.push_front(n);
        counter++;
    }

    //now we're dealing with a list of nodes
    return nodeList;
}

list<HuNode*> buildCode(list<HuNode*> temp) {
    //initialize temp storage
    HuNode *n1, *n2, *root;

    //sort temp in descending order
    temp.sort(sortNodesByProbability);

    //grab the first node
    n1 = temp.front();
    root = temp.front();
    temp.pop_front();

    //grab another node
    n2 = temp.front();
    temp.pop_front();

    //initialize the 3rd node
    HuNode *n3 = new HuNode;

    //attach nodes here
    n3->data = '*';
    n3->freq = n1->freq + n2->freq;
    n1->LR = '1';
    n2->LR = '0';
    n3->LR = '*';
    n3->lchild = n1; //by default, the lower node
    n3->rchild = n2; //by default, the higher node
    temp.push_back(n3);

    //print traversal here
    //printNodeListTraversal(temp);

    //return reformatted list
    return temp;
}

HuNode* buildCodeWrap(list<HuNode*> temp) {
    int length = 0;
    list<HuNode*>::iterator it;

    for (it = temp.begin(); it != temp.end(); it++) {
        HuNode *n = *it;
        length += n->freq;
    }

    int counter = 0;
    while (counter != length) {
        temp = buildCode(temp);
        counter = temp.back()->freq;
    }

    //print the list
    //printNodeListTraversal(temp);

    //create the root node
    HuNode *n = new HuNode;
    n = temp.front();

    //return the front node
    return n;
}

void printArray(char paths[], char currChar, int arrSize, list<cNode*> *thisList) {
    //cout << currChar << ": ";
    string currStr;
    for (int i = 0; i < arrSize; i++) {
        if (paths[i] == '*') {
            //do nothing
        } else {
            //cout << paths[i];
            currStr += paths[i];
        }
    }
    cNode *n = new cNode;
    n->bitstring = currStr;
    n->data = currChar;
    //do list stuff here
    thisList->push_front(n);
    currStr.clear();
    //cout << endl;
}

char printPaths(HuNode* node, char path[], int pathLen, list<cNode*> *thisList) {
    if (node == nullptr) {
        return '*';
    }

    //append this node to the path
    path[pathLen] = node->LR;
    pathLen++;

    if (node->lchild == nullptr && node->rchild == nullptr) {
        printArray(path, node->data, pathLen, thisList);
    } else {
        printPaths(node->lchild, path, pathLen, thisList);
        printPaths(node->rchild, path, pathLen, thisList);
    }
}

list<cNode*>* getPaths(HuNode* n) {
    //create some large char array to store paths
    char path[1000];
    //new list of structs for code list
    list<cNode*> *n1 = new list<cNode*>();
    //print the paths out recursively
    printPaths(n, path, 0, n1);
    //return the coded linked list
    return n1;
}

string encode(string inputStr, HuTree *n) {
    std::string *code = new std::string;
    //create the list nodes
    list<HuNode*> temp = buildNodes(inputStr);

    //get the number of unique nodes
    int nodes = getNumUniqueNodes(temp);

    //get the input strings length for verification
    int numChars = inputStr.length();

    //attach the tree root to the code tree
    n->root = buildCodeWrap(temp);
    n->length = nodes;

    //get the traversal of the tree, pre-order
    *code = "";

    //build bit strings
    list<cNode*> *paths = getPaths(n->root);

    //create the bit string for the string
    string::iterator sit;
    list<cNode*>::iterator it;

    //iterate through and create the bit string
    for (sit = inputStr.begin(); sit != inputStr.end(); sit++) {
        for (it = paths->begin(); it != paths->end(); it++) {
            //dereference the node
            cNode *n = *it;
            if (*sit == n->data) {
                *code += n->bitstring;
                *code += " ";
            }
        }
    }

    //time to get the bit strings
    return *code;
}

string decode(HuTree *n, string code) {
    HuNode *root = n->root;
    const int maxLength = 1000;
    string bitstrings[maxLength];

    int counter = 0;
    string::iterator it;
    string temp;
    for (it = code.begin(); it != code.end(); it++) {
        if (*it != ' ') {
            temp += *it;
        } else {
            bitstrings[counter] = temp;
            temp.clear();
            counter++;
        }
    }

    //debug
//    for (int i = 0; i < maxLength; i++) {
//        if (bitstrings[i] != "") {
//            cout << bitstrings[i] << ", ";
//        }
//    }
    
    string returnStatement = "";
    //get each bitstring
    for (int i = 0; i < maxLength; i++)
    {
        if (bitstrings[i] != "")
        {
            //perform traversals here
            string temp = bitstrings[i];
            string::iterator it;
            for (it = temp.begin(); it != temp.end(); it++)
            {
                if (*it == '1' && root->lchild != nullptr)
                {
                    root = root->lchild;
                }
                else if (*it == '0' && root->rchild != nullptr)
                {
                    root = root->rchild;
                }
            }
            
            returnStatement += root->data;
            root = n->root;
        }
    }
    return returnStatement;
}

void readOutToFile(string filename, string code)
{
    ofstream fileOut(filename.c_str());
    if (fileOut.is_open())
    {
        fileOut << code;
    }
    else
    {
        cout << "Unable to open file.";
    }
    
    fileOut.close();
}

int main() {
    string directory = "";
    string filename = "";
    
    cout << "Enter the root directory containing the file: " << endl;
    cin >> directory;
    cout << "Enter the filename of the file to be encoded: " << endl;
    cin >> filename;
    
    string::iterator it;
    it = directory.end();
    if (*it != '/')
    {
        directory += "/";
    }
    
    string inputStr = readInFile(directory + filename);
    
    HuTree *n = new HuTree();
    string code = encode(inputStr, n);
    
    filename = filename.substr(0, filename.length() - 4);
    filename += "_code.txt";
    readOutToFile(directory + filename, code);
    
    string returnStatement = decode(n, code);
    cout << returnStatement << endl;
    
    filename = filename.substr(0, filename.length() - 9);
    filename += "_result.txt";
    readOutToFile(directory + filename, returnStatement);
    
    return 0;
}