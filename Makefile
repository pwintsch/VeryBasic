CC = clang++
CFLAGS = -std=c++11 -g -v
OBJ = ./*.cpp

verybasic: $(OBJ)
	$(CC) $(CFLAGS) -o $./obj/verybasic $(OBJ)

clean:
	rm -f ./obj/*.o verybasic
