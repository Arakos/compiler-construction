####################################################################
# This Makefile started out as a copy of the one in the flex manual.
# Cf. http://flex.sourceforge.net/manual/Makefiles-and-Flex.html#Makefiles-and-Flex
#
# It replaces the amazingly complex Makefile that comes with the C++calc example
# found in the Bison manual.
#
# This is Verison 0.2 of the Makefile (as of 17 April 2013, 22:00
# The previous (unnumbered) version only worked with MAKEFLAGS=-j3
# (and it's strange that it worked then).
#
####################################################################
#      "Variables", die hier benutzt werden:
#      Vgl. http://www.makelinux.net/make3/make3-CHP-2-SECT-2.html
# $@ = The filename representing the target.
# $< = The filename of the first prerequisite.
# $* = The stem of the filename of the target (i.e. without .o, .cpp...)
# $^ = The names of all the prerequisites, with spaces between them.
####################################################################
# Uncomment only one of the next two lines (choose your c++ compiler)
#CC=g++
CC=clang++

LEX=flex
YACC=bison
YFLAGS=-d 

LLVM_FLAGS := `llvm-config --cxxflags --ldflags --system-libs --libs all  \
                   | sed s/no-maybe/no/ `
CFLAGS :=  -g -std=c++11 -Wall -fexceptions $(LLVM_FLAGS) -rdynamic -O3
# Wo -d wichtig ist, weil damit Header-Dateien erzeugt werden
#         (*.hh - und nicht nur Quellcode in *.cc)
# aber -v nicht so wichtig ist, weil damit "nur" die  Datei
#         calc++-parser.output erzeugt wird, die zwar informativ aber nicht
#         unbedingt notwendig (sie wird nicht weiterverwendet).

HEADERS=calc++-parser.hh calc++-scanner.hh

calc++ : calc++.o calc++-scanner.o calc++-parser.o calc++-driver.o

calc++.o : calc++.cc calc++-driver.hh calc++-parser.hh

%.o: %.cc
	$(CC) $(CFLAGS) -o $@ -c $<

calc++-scanner.cc: calc++-scanner.ll
	$(LEX) $(LFLAGS) -o calc++-scanner.cc calc++-scanner.ll

calc++-parser.cc calc++-parser.hh: calc++-parser.yy
	$(YACC) $(YFLAGS) -o calc++-parser.cc calc++-parser.yy

clean:
	$(RM) *~ *.o  calc++  calc++-scanner.cc calc++-parser.cc  calc++-scanner.hh calc++-parser.hh calc++-parser.output location.hh stack.hh position.hh

tests: test.sh calc++
	./test.sh
