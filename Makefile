CC=gcc
CFLAGS=-Werror -Wextra -Wall -std=c11
OS_NAME=$(shell uname)
BUILD_DIR=build
BRANCH_INFO       =--rc lcov_branch_coverage=1
REMOVE_DIRECTIVES =clean_mode=1
SRC_DIR           =./
BUILD_DIR         =build
PROJ_NAME	  =3D_vier
TEST_FILES        =3D_vier/doing.c parsing.c
TEST_DIR          =3D_vier/test
TEMP_DIR          =3D_vier/test/temp
REPORT_DIR_GCOV   =3D_vier/test/report_gcov
REPORT_DIR_LCOV   =3D_vier/test/report_lcov
ADD_FILE 	  =parsing.c parsing.h Makefile 3DViewer_v1_info.texi

ifeq ($(OS_NAME), Linux)
	MV_DIR = $(TEST_DIR)
	TEST_FLAGS  =--coverage $(shell pkg-config --libs check)
	LEAK_CHECK  =valgrind --tool=memcheck --leak-check=yes --log-file="report.txt" ./$(TEST_DIR)/test.out
	CFLAGS_LIBS =-lm -lsubunit -lpthread -lrt
	OPEN=./$(BUILD_DIR)/$(PROJ_NAME)
	TAR=$(PROJ_NAME)
else
	TAR=$(PROJ_NAME).app
	OPEN=open $(BUILD_DIR)/$(PROJ_NAME).app
	MV_DIR = .
	TEST_FLAGS  =--coverage -lm -lcheck
	LEAK_CHECK  =CK_FORK=no leaks --atExit -- ./$(TEST_DIR)/test.out
	CFLAGS_LIBS =-lm -L /usr/local/lib #-lcheck
endif

all: install start

install:
	qmake CONFIG+=release -o $(BUILD_DIR)/Makefile 3D_vier/3D_vier.pro
	-cd $(BUILD_DIR); make
	-cd $(BUILD_DIR); make clean
	-rm -rf $(BUILD_DIR)/Makefile
	-rm -rf $(BUILD_DIR)/.qmake.stash
	-rm -rf $(BUILD_DIR)/qmake_qmake_qm_files.qrc
	-rm -rf $(BUILD_DIR)/.qm

start:
	$(OPEN)

uninstall: clean 
	rm -rf build
	

leaks: test
	$(LEAK_CHECK)
	grep "ERROR SUMMARY:" report.txt 

clean: clean_test
	rm -rf $(BUILD_DIR)/$(PROJ_NAME)_src.tar.gz
	rm -rf $(BUILD_DIR)/$(TAR).tar.gz
	
clean_test:
	rm -rf $(TEST_DIR)/*.c
	rm -rf $(TEST_DIR)/*.out
	rm -rf $(TEST_DIR)/*.gcda
	rm -rf $(TEST_DIR)/*.gcno
	rm -rf $(TEMP_DIR)
	rm -rf $(REPORT_DIR_LCOV)
	rm -rf $(REPORT_DIR_GCOV)
	rm -rf report.txt
	rm -rf *.gcda

dist: install clean
	rm -rf $(BUILD_DIR)/$(PROJ_NAME).tar.gz
	tar -cvf $(BUILD_DIR)/$(PROJ_NAME).tar.gz $(BUILD_DIR)/$(TAR)
	tar -cvf $(BUILD_DIR)/$(PROJ_NAME)_src.tar.gz $(PROJ_NAME) $(ADD_FILE)

dvi:
	mkdir -p $(BUILD_DIR)
	makeinfo -o $(BUILD_DIR) --html --no-warn --force ./3DViewer_v1_info.texi
	open $(BUILD_DIR)/index.html

test.c: 
	@checkmk $(REMOVE_DIRECTIVES) $(TEST_DIR)/test > $(TEST_DIR)/test.c

test: test.c
	$(CC) $(FLAGS) $(TEST_FILES) $(TEST_DIR)/test.c $(TEST_FLAGS) -o $(TEST_DIR)/test.out
	./$(TEST_DIR)/test.out
	mkdir -p $(TEMP_DIR)
	mv $(MV_DIR)/*.gcda $(TEMP_DIR)/
	mv $(MV_DIR)/*.gcno $(TEMP_DIR)/

gcov_report: test
	mkdir -p $(REPORT_DIR_GCOV)
	mv $(TEMP_DIR)/*.gcda $(REPORT_DIR_GCOV)
	mv $(TEMP_DIR)/*.gcno $(REPORT_DIR_GCOV)
	rm -fR $(TEMP_DIR)
	gcovr -r $(REPORT_DIR_GCOV) -r . --html-details -o test_report_gcov.html
	mv test_report_gcov.* $(REPORT_DIR_GCOV)
	open $(REPORT_DIR_GCOV)/test_report_gcov.html

lcov_report: test
	mkdir -p $(REPORT_DIR_LCOV)
	mv $(TEMP_DIR)/*.gcda $(REPORT_DIR_LCOV)
	mv $(TEMP_DIR)/*.gcno $(REPORT_DIR_LCOV)
	rm -fR $(TEMP_DIR)
	lcov -t $(TEST_DIR)/test.out -o $(REPORT_DIR_LCOV)/t_lcov.info -c -d .
	genhtml -o $(REPORT_DIR_LCOV) $(REPORT_DIR_LCOV)/t_lcov.info
	open $(REPORT_DIR_LCOV)/index.html

style:
	clang-format --style=Google -n *.c
	clang-format --style=Google -n *.h
	clang-format --style=Google -n 3D_vier/*.c
	clang-format --style=Google -n 3D_vier/*.h
	clang-format --style=Google -n 3D_vier/*.cpp
	clang-format --style=Google -n 3D_vier/*.hpp
	
style-format:
	clang-format --style=Google -i *.c
	clang-format --style=Google -i *.h
	clang-format --style=Google -i 3D_vier/*.c
	clang-format --style=Google -i 3D_vier/*.h
	clang-format --style=Google -i 3D_vier/*.cpp
	clang-format --style=Google -i 3D_vier/*.hpp
