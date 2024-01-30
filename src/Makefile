CFLAGS = -Wall -Werror -Wextra -std=c11
CC = gcc

CHECK_FLAGS = $(shell pkg-config --cflags --libs check)
GCOV_FLAGS = -fprofile-arcs -ftest-coverage -fPIC
VALGRIND_FLAGS = --log-file="valgrind.txt" --track-origins=yes --trace-children=yes --leak-check=full --leak-resolution=med
LIBRARY = s21_3dviewer.a

SRC= $(wildcard *.c)
OBJ = $(patsubst %.c,%.o, ${SRC})
SRC_TEST = $(wildcard tests/*.c)
OBJ_TEST = test.o

all: clean $(LIBRARY) test gcov_report

$(LIBRARY): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC)
	ar rc $(LIBRARY) $(OBJ)
	rm $(OBJ)

install:
	$(MAKE) -C build
	open ./build/qt.app

uninstall: clean
	rm -rf build/qt.app build.gz

test: $(LIBRARY) $(OBJ_TEST)
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(OBJ_TEST) -o test $(LIBRARY) $(CHECK_FLAGS)
	./test

$(OBJ_TEST):
	$(CC) -c $(SRC_TEST)

clang:
	clang-format --style=google -i $(SRC) qt/*.cpp

gcov_report: clean $(OBJ_TEST)
	$(CC) $(CFLAGS) -o test $(OBJ_TEST) --coverage $(SRC) $(CHECK_FLAGS)
	./test
	gcov -b -l -p -c s21_*.gcno
	gcovr -o gcov_report.html --html --html-details

clean: 
	rm -rf *.o test *.a tests/*.o *.gcda *.gcno *.gcov report *.info build/*.o build/*.cpp build/*.h

valgrind:
	valgrind $(VALGRIND_FLAGS) ./test
	grep -n "ERROR SUMMARY" valgrind.txt

dist: install clean
	tar -zcvf 3DViewer.gz build

dvi:
	open readme.txt