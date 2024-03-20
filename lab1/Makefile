# .PHONY: all
# all: test_reverse
# test_reverse: test_reverse.o reverse.o reverse.h
# 	gcc test_reverse.o reverse.o -o test_reverse
# .PHONY: clear
# clean:
# 	rm test_reverse
# .PHONY: format
# format:
# 	clang-format -style=Google -i reverse.h reverse.c test_reverse.c

# reverse.o: reverse.c
# 	gcc  -c reverse.c -o reverse.o

# test_reverse.o: test_reverse.c
# 	gcc  -c test_reverse.c -o test_reverse.o

.PHONY: all
all: test_reverse
test_reverse: test_reverse.o reverse.o reverse.h
	gcc test_reverse.o reverse.o -o test_reverse
.PHONY: clear
clean:
	rm test_reverse
.PHONY: format
format:
	clang-format -style=Google -i reverse.h reverse.c test_reverse.c

test_reverse.o: %.o : %.c
	gcc $< -o $@
# reverse.o: reverse.c
# 	gcc  -c reverse.c -o reverse.o

# test_reverse.o: test_reverse.c
# 	gcc  -c test_reverse.c -o test_reverse.o