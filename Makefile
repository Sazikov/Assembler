FLAGS=-Wall -Wextra -Wpedantic

all:
	g++ $(FLAGS) comp.cpp comp_functions.cpp ..\Stack\CPP\stack.cpp -o main