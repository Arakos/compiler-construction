#include <iostream>
#include "calc++-driver.hh"


int
main (int argc, char *argv[])
{
  int res = 0;
  calcxx_driver driver;
  if(!0) {
	  std::cout << "Hallo, !0 is true, und ich bin Marcell D'Avis, 1 & 1" << std::endl << "Starting D'Avis parse ..." << std::endl;
  }
  
  for (int i = 1; i < argc; ++i) {
    if (argv[i] == std::string ("-p"))
      driver.trace_parsing = true;
    else if (argv[i] == std::string ("-s"))
      driver.trace_scanning = true;
    else if (!driver.parse (argv[i]))
      std::cout << "Das Programm läuft!" << std::endl;
    else {
      std::cout << "Ich kompiliere erst, wenn das Programm läuft!" << std::endl;
	  res = 1;
	}
  }
  
  if(res == 0) {
	  std::cout << std::endl;
	  std::map<std::string, std::string> variables = driver.variables;
	  for(std::map<std::string, std::string>::iterator it = variables.begin(); it != variables.end(); ++it) {
		  std::cout << it->first << ": " << it->second << std::endl;
	  }
	  
	  if(driver.tree)
		std::cout << "Tree:\n" << driver.tree->toString() << endl;
  }
  
  return 0;
}

