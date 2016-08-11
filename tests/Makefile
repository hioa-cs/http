# This file is a part of the IncludeOS unikernel - www.includeos.org
#
# Copyright 2015 Oslo and Akershus University College of Applied Sciences
# and Alfred Bratterud
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

CPP=$(shell command -v clang++ || command -v clang++-3.8 || command -v clang++-3.7 || command -v clang++-3.6)
CFLAGS=-std=c++14 -Ofast -Wall -Wextra
INC=-I. -I../inc -I../uri/include -I../uri/GSL/include
SRC=../uri/src/percent_encoding.cpp ../uri/src/uri.cpp

all: request response
	
request: request_test.cpp test_machine.o
	$(CPP) $(CFLAGS) $(INC) -orequest request_test.cpp test_machine.o $(SRC)

response: response_test.cpp test_machine.o
	$(CPP) $(CFLAGS) $(INC) -oresponse response_test.cpp test_machine.o

test_machine.o: test_machine.cpp
	$(CPP) $(CFLAGS) $(INC) -c test_machine.cpp

clean:
	rm -f request
	rm -f response
	rm -f test_machine.o
