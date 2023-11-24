ttsparse : build/ttsp.o
	gcc -o out/ttsp build/ttsp.o 
build/ttsp.o : src/ttsp.c
	gcc -c src/ttsp.c -o build/ttsp.o
build/test.o : tests/test.c
	gcc -c tests/test.c -o build/test.o
test : build/ttsp.o build/test.o
	gcc -o out/test  build/test.o
	./out/test
clean:
	rm build/*
	rm out/*
