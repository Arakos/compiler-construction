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

echo && echo "tests/test1.go"
cat tests/test1.go | ./calc++ -

echo && echo "tests/test2.go"
cat tests/test2.go | ./calc++ -

echo && echo "tests/test3.go"
cat tests/test3.go | ./calc++ -

echo && echo "tests/test4.go"
cat tests/test4.go | ./calc++ -

echo && echo "tests/test5.go"
cat tests/test5.go | ./calc++ -

echo && echo "tests/fail1.go"
cat tests/fail1.go | ./calc++ -

echo && echo "tests/fail2.go"
cat tests/fail2.go | ./calc++ -

echo && echo "tests/fail3.go"
cat tests/fail3.go | ./calc++ -

echo && echo "tests/fail4.go"
cat tests/fail4.go | ./calc++ -

echo && echo "tests/fail5.go"
cat tests/fail5.go | ./calc++ -


#make tests > test.txt 2>&1
