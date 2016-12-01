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

CXX=$(shell command -v clang++-3.9 || command -v clang++-3.8 || command -v clang++)
CXXFLAGS=-std=c++14 -Ofast -Wall -Wextra
INC=-I. -I../inc -I../uri/include -I../uri/GSL
SRC=../uri/src/percent_encoding.cpp ../uri/src/uri.cpp

all: mime_types status_codes version methods header message status_line \
		 request_line request response

mime_types: mime_types_test.cpp test_machine.o
	$(CXX) $(CXXFLAGS) $(INC) -omime_types mime_types_test.cpp test_machine.o

status_codes: status_codes_test.cpp test_machine.o
	$(CXX) $(CXXFLAGS) $(INC) -ostatus_codes status_codes_test.cpp test_machine.o

version: version_test.cpp test_machine.o
	$(CXX) $(CXXFLAGS) $(INC) -oversion version_test.cpp test_machine.o

methods: methods_test.cpp test_machine.o
	$(CXX) $(CXXFLAGS) $(INC) -omethods methods_test.cpp test_machine.o

header: header_test.cpp test_machine.o
	$(CXX) $(CXXFLAGS) $(INC) -oheader header_test.cpp test_machine.o

message: message_test.cpp test_machine.o
	$(CXX) $(CXXFLAGS) $(INC) -omessage message_test.cpp test_machine.o

status_line: status_line_test.cpp test_machine.o
	$(CXX) $(CXXFLAGS) $(INC) -ostatus_line status_line_test.cpp test_machine.o

request_line: request_line_test.cpp test_machine.o
	$(CXX) $(CXXFLAGS) $(INC) -orequest_line request_line_test.cpp test_machine.o $(SRC)

request: request_test.cpp test_machine.o
	$(CXX) $(CXXFLAGS) $(INC) -orequest request_test.cpp test_machine.o $(SRC)

response: response_test.cpp test_machine.o
	$(CXX) $(CXXFLAGS) $(INC) -oresponse response_test.cpp test_machine.o

test_machine.o: test_machine.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c test_machine.cpp

clean:
	rm -f mime_types status_codes version methods header message status_line \
	      request_line request response
