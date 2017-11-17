//
// Created by Daniel on 17.11.2017.
//

#ifndef COMPILERCONSTRUCTION_SYMBOLTABLE_H
#define COMPILERCONSTRUCTION_SYMBOLTABLE_H

#include <iostream>
#include <vector>


using namespace std;

class SymbolTable;

struct Symbol{
    string name = "";
    string type = "";
    double value = 0;
    bool isconst = false;
    int scope;

};
class SymbolTable{

public:
    SymbolTable(){
    vector<Symbol> s;
    SymTable.push_back(s);
        scope = 0;
    }

    ~SymbolTable(){
    }
    int ReturnScope(){
        return scope;
    }
    void NewScope(){
        scope++;
        vector<Symbol> s;
        SymTable.push_back(s);
    }
    void NewSubScope(){

    }
    void Insert(Symbol newsym){
        SymTable.at(scope).push_back(newsym);
    }
    void Delete(Symbol delsym){
        int size = SymTable.at(scope).size();
        Symbol tmpsym;
        for(int i=0; i<size; i++) {
            tmpsym = SymTable.at(scope).at(i);
            if(delsym.name == tmpsym.name){
                SymTable.at(scope).erase(SymTable.at(scope).begin() + i );
                return;
            }
        }
    }
    bool Lookup(Symbol Sym){
        int size = SymTable.at(scope).size();
        Symbol tmpsym;
        for(int i=0; i<size; i++) {
            tmpsym = SymTable.at(scope).at(i);
            if(Sym.name == tmpsym.name){
                return true;
            }
        }
        return false;
    }
    Symbol GetAttribute(string Sname, int findscope){
        vector<Symbol>Symvector;
        Symvector.swap(SymTable.at(findscope));
        for(int i= 0; i<Symvector.size(); i++){
            Symbol s = Symvector.at(i);
            if (s.name == Sname){
                return s;
            }
        }

    }
    bool PutAttribute(string Sname, int findscope, int value){
        vector<Symbol> Sym;
        Sym.swap(SymTable.at(findscope));
        for(int i= 0; i<Sym.size(); i++){
            Symbol s = Sym.at(i);
            if(s.name == Sname){
                SymTable.at(findscope).at(i).value = value;
                return true;
            }

        }
        return false;

    }
    void printSymbolTable(){
        for(int i=0; i<SymTable.size();i++){
            if(!SymTable.at(i).empty())
            cout << "Scope " << i << endl;
            for (int k = 0;k<SymTable.at(i).size();k++){
                cout << SymTable.at(i).at(k).type << " "
                     << SymTable.at(i).at(k).name << " "
                     << SymTable.at(i).at(k).value << endl;
            }
        }
    }
private:
    vector <vector <Symbol>> SymTable;
    int scope;
};

#endif //COMPILERCONSTRUCTION_SYMBOLTABLE_H
