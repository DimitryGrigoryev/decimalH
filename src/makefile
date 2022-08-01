
CC = gcc
CC2 = gcc
LDFLAGS = #-std=c11 -D_GNU_SOURCE -Wall -Wextra -Werror -ggdb3
OS := $(shell uname -s)

ifeq ($(OS), Darwin)
	CC2 += -D OS_MAC
else
	CC2 += -D OS_LINUX
endif

default: all

all: clean s21_decimal.a

past: clean s21_decimal.a
ifeq ($(OS), Darwin)
	$(CC) $(LDFLAGS) -o past past.c s21_decimal.a
else
	$(CC) $(LDFLAGS) -o past past.c s21_decimal.a -lm
endif
	./past

test:: s21_decimal.a unit_test.c
ifeq ($(OS), Darwin)
	$(CC) $(LDFLAGS) -o test unit_test.c s21_decimal.a -lcheck
else
	$(CC) unit_test.c s21_decimal.c -o test -lcheck -lsubunit -lrt -lpthread -lm
endif
test::
	./test

s21_decimal.a: s21_decimal.o
	ar rcs s21_decimal.a s21_decimal.o
	ranlib $@
	cp $@ lib$@

s21_decimal.o: s21_decimal.c s21_decimal.h
	$(CC2) $(LDFLAGS) -c -o s21_decimal.o s21_decimal.c

clean:
	rm -rf test *.a *.o *.out *.cfg fizz *.gc* *.info report mtest past s21_decimal

rebuild: clean all

check:
	cppcheck *.h *.c
	cp ../materials/linters/CPPLINT.cfg CPPLINT.cfg
	python3 ../materials/linters/cpplint.py s21_decimal.c *.h
ifeq ($(OS), Darwin)
	CK_FORK=no leaks --atExit -- ./test
else
	valgrind --leak-check=full ./test
endif

gcov_report: s21_decimal.a
ifeq ($(OS), Darwin)
	$(CC2) --coverage unit_test.c s21_decimal.c -o test -lcheck
else
	$(CC2) --coverage unit_test.c s21_decimal.c -o test -lcheck -lsubunit -lrt -lpthread -lm
endif
	./test
	lcov -t "test" -o test.info -c -d ./
	genhtml -o report test.info
	open ./report/index-sort-f.html
