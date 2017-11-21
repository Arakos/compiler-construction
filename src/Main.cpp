//============================================================================
// Name        : Main.cpp
// Author      : Spas
// Version     :
// Copyright   : Your copyright notice
// Description : Compiler Construction practical work ...
//============================================================================

#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include "Lexer.h"
#include "TreeNode.h"
#include "Parser.h"

using namespace std;


std::string readFile(std::string filename) {
	std::ifstream t(filename);
	std::string str = "";
	if(t) {
		t.seekg(0, std::ios::end);
		str.reserve(t.tellg());
		t.seekg(0, std::ios::beg);

		str.assign((std::istreambuf_iterator<char>(t)),
					std::istreambuf_iterator<char>());
	} else {
		cout << "Cannot open file: " << filename << endl;
		str = "";
	}
	return str;
}

bool parse(string input) {

	if(input.length() <= 0) {
		cout << "cannot parse empty input!" << endl;
		return false;
	}
	Lexer* l = new Lexer(input);
	Parser* p = new Parser( l );
	TreeNode* root = p->createAST();
	bool error = root->isErrorNode();

	cout << "Abstract Syntax Tree:" << endl << root->toString() << endl << endl;
	cout << "Symbole Table:" << endl << p->getSymbolTableAsString() << endl << endl << endl << endl;

	delete root;
    delete p;

	return !error;
}

void runTests() {
	std::fstream input("tests.txt");
	std::vector<string> testfiles;
	for( std::string line; getline( input, line ); ) {
	    if(line.length() > 0) {
	    	testfiles.push_back(line);
	    }
	}

	for(string filename : testfiles) {
		string tmp = filename;
//        if (tmp.find("\r")){
//            tmp.erase(tmp.size() - 1);
//        }
		string input = readFile(tmp);
		if( parse(input) ) {
			cout << "Test '" + filename + "' successful" << endl << BREAKER << endl;
		} else {
			cout << "Test '" + filename + "' failed" << endl << BREAKER << endl;
		}
	}
}

int main() {

	cout << "Hallo, bin Compiler Davi's, seit 16 Tagen Leiter für Grammatikzufriedenheit bei 1 & 1. Und ich compiliere erst, wenn das Programm läuft." << endl << endl;

	string userInput = "";

	while(userInput != "exit") {
		cout << BREAKER << endl;
		cout    << "1. Enter (1) to parse a file by given name" << endl
				<< "2. Enter (2) to parse the files specified in the 'tests.txt' file" << endl
				<< "3. Enter code to parse it right away"<< endl
				<< "4. Enter (0) to exit" << endl;;
		getline(cin, userInput);

		if(userInput == "1") {
			cout << "Filename: ";
			getline(cin, userInput);
			string tmp = readFile(userInput);
			if(!tmp.empty()) {
				parse(tmp);
			}

		} else if(userInput == "2") {
			runTests();

		} else if(userInput == "0") {
			break;

		} else {
			if(userInput == "Hallo,") { 	// fukn eastereggs o.O
				userInput += " bin Marcell D'avis, 1 & 1 :)";
			}
			parse(userInput);
		}
	}
	cout << "Exit successful!" << endl;
	return 0;
}
