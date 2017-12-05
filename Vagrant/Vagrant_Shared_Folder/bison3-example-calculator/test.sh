#! /bin/sh

# This shell script is part of the C++ Bison Example
# (tests the calc++ program).
# Some changes made by Prof. R. C. Moore (fbi.h-da.de)

# See https://www.gnu.org/software/bison/manual/html_node/A-Complete-C_002b_002b-Example.html

# Copyright (C) 2005-2012 Free Software Foundation, Inc.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

#test -z "$VERBOSE" && {
#   exec > /dev/null 2>&1
#   set -x
#}

echo && echo -e "\nTEST: Running a four line cac++ program..."
cat > input <<EOF
package main;

import "test";
import "hallo";
import "jaGG";

func myFunc () { 
	var hallo = 1 + 1;
	hallo = 42;
	
	var davis = 1;
	
	var definition;
	
	var marcell = "hallo";
	
}
EOF

./calc++ input

echo && echo -e "\nTEST: Running the same four line cac++ program with input from a pipe (stdin)..."
cat input | ./calc++ -

echo && echo -e "\nTEST: Running the same four line cac++ program with parse tracing turned on..."
./calc++ -p -s input

cat > input <<EOF
package main;

import "file1";
import "file2";

func main() {
	var x = (1 + 2) * (3 + 4 * (5 + 6));
	var y = 1 + 2 * 3 + 4 * 5;
	var z = 1 + 2 + 3 + 4 + 5 ;
}
EOF

./calc++ -p -s input

rm input

#make tests > test.txt 2>&1
