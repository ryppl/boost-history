CC=g++
CFLAGSREL=-Wall -pedantic -ftemplate-depth-255 -O3
CFLAGSDBG=-Wall -pedantic -ftemplate-depth-255
INCLUDE=-I../../../../../boost -I../../../..

all: test

clean:
	rm *_dbg
	rm *_rel

test: build
	./test_ptree_dbg 
	./test_ptree_rel
	./test_cmdline_dbg 
	./test_cmdline_rel
	./test_ini_dbg 
	./test_ini_rel
	./test_info_dbg 
	./test_info_rel
	./test_xml_dbg 
	./test_xml_rel

build: debug release

debug: test_ptree_dbg test_cmdline_dbg test_ini_dbg  test_info_dbg test_xml_dbg
release: test_ptree_rel test_cmdline_rel test_ini_rel test_info_rel test_xml_rel

test_ptree_dbg: ../test_property_tree.cpp
	$(CC) $(CFLAGSDBG) $(INCLUDE) $< -o $@
test_ptree_rel: ../test_property_tree.cpp
	$(CC) $(CFLAGSREL) $(INCLUDE) $< -o $@
test_cmdline_dbg: ../test_cmdline_parser.cpp
	$(CC) $(CFLAGSDBG) $(INCLUDE) $< -o $@
test_cmdline_rel: ../test_cmdline_parser.cpp
	$(CC) $(CFLAGSREL) $(INCLUDE) $< -o $@
test_ini_dbg: ../test_ini_parser.cpp
	$(CC) $(CFLAGSDBG) $(INCLUDE) $< -o $@
test_ini_rel: ../test_ini_parser.cpp
	$(CC) $(CFLAGSREL) $(INCLUDE) $< -o $@
test_info_dbg: ../test_info_parser.cpp
	$(CC) $(CFLAGSDBG) $(INCLUDE) $< -o $@
test_info_rel: ../test_info_parser.cpp
	$(CC) $(CFLAGSREL) $(INCLUDE) $< -o $@
test_xml_dbg: ../test_xml_parser_spirit.cpp
	$(CC) $(CFLAGSDBG) $(INCLUDE) $< -o $@
test_xml_rel: ../test_xml_parser_spirit.cpp
	$(CC) $(CFLAGSREL) $(INCLUDE) $< -o $@
