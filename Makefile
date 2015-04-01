### CSCI561 - Fall 2014 - Sample Makefile

### Example for a C++ agent:
agent: alphabet.cpp gameplay.cpp greedy.cpp main_call.cpp minimax.cpp
	g++ alphabet.cpp gameplay.cpp greedy.cpp main_call.cpp minimax.cpp -o agent

run: agent
	./agent
