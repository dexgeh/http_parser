
test: *.c *.h
	cc -std=c99 -o test http_parser.c test.c

clean:
	rm test
