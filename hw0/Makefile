word_count: wc
wc: wc.o
	gcc -ggdb3 wc.o -o wc	
wc.o: wc.c
	gcc -ggdb3 -c wc.c

map: map.o
	gcc -ggdb3 map.o -o map
map.o: map.c
	gcc -ggdb3 -c map.c

clean:
	rm -rf *0  wc 
