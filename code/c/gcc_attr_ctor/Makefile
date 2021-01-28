default: a

a:
	gcc -o main main.c

b:
	gcc -shared -fPIC -o libfoo.so foolib.c
	gcc -o main2 main2.c -I. -L. -lfoo

c:
	gcc -fPIC -c foolib.c
	ld -shared -o libfoo.so foolib.o
	gcc -o main2 main2.c -I. -L. -lfoo

d:
	gcc -shared -fPIC -o libfoo.so foolib.c
	gcc -o main3 main3.c -I. -ldl

clean:
	rm -f *.o main main2 main3 libfoo.so