bus : main.o bus.o
	gcc -pedantic-errors -g -o $@ $^
main.o : main.c
	gcc -pedantic-errors -g -c -o $@ $^
bus.o : bus.c
	gcc -pedantic-errors -g -c -o $@ $^
