#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "HashMap.h"

using namespace std;

struct Entry
{
    string id;
    string product, brand, asin;
    vector<string> category;
    string upc, listPrice, sellPrice, quantity, modelNumber;

    friend ostream& operator<<(ostream& os, const Entry& rhs);
};

ostream& operator<<(ostream& os, const Entry& rhs)
{
    os << "Product Name: " << rhs.product << ", Categories: ";
    for(string s : rhs.category)
        os << s << ", ";
    os << "Price: " << rhs.sellPrice << "Model Num: " << rhs.modelNumber << endl;
    return os;
}

HashMap<string, Entry> idMap;
HashMap<string, vector<string>> categoryMap;

void printHelp()
{
    cout << "Supported list of commands: " << endl;
    cout << " 1. find <inventoryid> - Finds if the inventory exists. If exists, prints details. If not, prints 'Inventory not found'." << endl;
    cout << " 2. listInventory <category_string> - Lists just the id and name of all inventory belonging to the specified category. If the category doesn't exists, prints 'Invalid Category'.\n"
         << endl;
    cout << " Use :quit to quit the REPL" << endl;
}

bool validCommand(string line)
{
    return (line == ":help") ||
           (line.rfind("find", 0) == 0) ||
           (line.rfind("listInventory") == 0);
}

void evalCommand(string line)
{
    if (line == ":help")
    {
        printHelp();
    }
    // if line starts with find
    else if (line.rfind("find", 0) == 0)
    {
        Entry e = *idMap.Find(line.substr(5));
        if(!e.id.empty())
            cout << e << endl;

    }
    // if line starts with listInventory
    else if (line.rfind("listInventory") == 0)
    {
        vector<string> entries = *categoryMap.Find(line.substr(14));
        for(string s : entries)
        {
            cout << idMap.Find(s) << endl;
        }
    }
}

void AddCSV()
{
    ifstream infile("marketing_sample_for_amazon_com-ecommerce__20200101_20200131__10k_data.csv");
    string tmp;
    getline(infile, tmp);
    while(!infile.eof())
    {
        string line;
        while(getline(infile, line))
        {
            stringstream ss(line);
            std::string token;
            std::vector<std::string> fields;
            while (std::getline(ss, token, ','))
            {
                fields.push_back(token);
            }
            Entry* e = new Entry();
            e->id = fields[0];
            e->product = fields[1];
            e->brand = fields[2];
            e->asin = fields[3];
            stringstream css(fields[4]);
            e->upc = fields[5];
            e->listPrice = fields[6];
            e->sellPrice = fields[7];
            e->quantity = fields[8];
            e->modelNumber = fields[9];
            while (std::getline(css, token, '|'))
            {
                if(token[0] == ' ')
                {
                    token = token.substr(1);
                }
                if(token[token.length()-1] == ' ')
                {
                    token = token.substr(0, token.length()-1);
                }
                e->category.push_back(token);
                vector<string>* v = categoryMap.Find(token);
                if(!v)
                {
                    v = new vector<string>();
                    v->push_back(e->id);
                    categoryMap.Insert(token, v);
                }
                else
                {
                    v->push_back(e->id);
                }
            }
            idMap.Insert(e->id, e);
        }
    }
}

void bootStrap()
{
    cout << "\n Welcome to Amazon Inventory Query System" << endl;
    cout << " enter :quit to exit. or :help to list supported commands." << endl;
    cout << "\n> ";
    AddCSV();
}

int main(int argc, char const *argv[])
{
    string line;
    bootStrap();
    while (getline(cin, line) && line != ":quit")
    {
        if (validCommand(line))
        {
            evalCommand(line);
        }
        else
        {
            cout << "Command not supported. Enter :help for list of supported commands" << endl;
        }
        cout << "> ";
    }
    return 0;
}