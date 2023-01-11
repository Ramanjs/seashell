# _*_ Makefile _*_

shell: shell.c lsss catss mkdirss rmss datess utils.o
	gcc -g shell.c utils.o -l readline -o shell

lsss: ls.o utils.o
	gcc ls.o utils.o -o lsss

ls.o: ls.c
	gcc -c -w ls.c -o ls.o

catss: cat.o utils.o
	gcc cat.o utils.o -o catss

cat.o: cat.c
	gcc -c cat.c -o cat.o

mkdirss: mkdir.o utils.o
	gcc mkdir.o utils.o -o mkdirss

mkdir.o: mkdir.c
	gcc -c mkdir.c -o mkdir.o

rmss: rm.o utils.o
	gcc rm.o utils.o -o rmss

rm.o: rm.c
	gcc -c rm.c -o rm.o
	
datess: date.o utils.o
	gcc date.o utils.o -o datess

date.o: date.c
	gcc -c date.c -o date.o

utils.o: utils.c
	gcc -c utils.c -o utils.o

clean:
	rm -f shell lsss catss mkdirss rmss datess 
	rm -f ls.o cat.o rm.o mkdir.o date.o utils.o
