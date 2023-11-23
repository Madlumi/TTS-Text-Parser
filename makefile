ttsparse : build/main.o
	gcc -o out/main.c build/main.o 
build/main.o : src/main.c
	gcc -c src/main.c -o build/main.o
test : ttsparse 
	echo "testing"
clean:
	rm build/*
	rm out/*
