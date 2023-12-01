CFLAGS = -Wall -Werror -Wextra -std=c11
GFLAGS = -fprofile-arcs -ftest-coverage
FILES = functions/*.c
LIBS = -L. -lcheck -lm

all: gcov_report

test: s21_matrix.a all_tests/tests.c $(FILES)
	gcc $(CFLAGS) -c $(FILES)
	gcc $(CFLAGS) -c all_tests/tests.c
	gcc $(CFLAGS) *.o -o tests $(LIBS) s21_matrix.a
	./tests

s21_matrix.a: $(FILES)
	gcc $(CFLAGS) -c $(FILES)
	ar crs s21_matrix.a *.o

gcov_report: $(FILES) all_tests/tests.c
	gcc $(CFLAGS) $(GFLAGS) -c $(FILES)
	gcc $(CFLAGS) -c all_tests/tests.c
	gcc $(GFLAGS) *.o -o tests $(LIBS)
	./tests
	lcov -t "a.out" -o REPORT.info -c -d .
	genhtml -o html REPORT.info
	open html/index.html
	
rebuild:
	make clean
	make all

clean:
	rm -f tests ./*.o ./*.a report *.gcda *.gcno *info 
	rm -rf html

check:
	clang-format -style=Google -n $(FILES) *.h all_tests/tests.c

clang:
	clang-format -style=Google -i $(FILES) *.h all_tests/tests.c