CC=g++
#FLAGS=-std=c++17 -fvar-tracking -g -O0 -Wall 
FLAGS=-std=c++17 -g -O3 -Wall 

OCT_STUFF=oct_main.cpp oct.cpp cut_covering.cpp gammoid.cpp matroid.cpp gaussian.cpp field.cpp graph.cpp basic.cpp
OCT_INFO_STUFF=oct_info_main.cpp oct.cpp cut_covering.cpp gammoid.cpp matroid.cpp gaussian.cpp field.cpp graph.cpp basic.cpp
OCT_CH_STUFF=check_oct_approx.cpp oct.cpp cut_covering.cpp gammoid.cpp matroid.cpp gaussian.cpp field.cpp graph.cpp basic.cpp

TESTS_FIELD=tests/field.cpp field.cpp
TESTS_GAMMOID=tests/gammoid.cpp gammoid.cpp matroid.cpp gaussian.cpp field.cpp graph.cpp basic.cpp
TESTS_GAUSS=tests/gauss.cpp gaussian.cpp field.cpp graph.cpp basic.cpp
TESTS_GRAPH=tests/graph.cpp graph.cpp basic.cpp

all: $(OCT_STUFF)
	$(CC) $(FLAGS) -lpthread -lgmp -lgmpxx -o oct $(OCT_STUFF)

gammoid_tests:
	$(CC) $(FLAGS) -lpthread -lgtest -lgmp -lgmpxx -o tests/gammoid $(TESTS_GAMMOID)

field_tests:
	$(CC) $(FLAGS) -lpthread -lgtest -lgmp -lgmpxx -o tests/field $(TESTS_FIELD)

gauss_tests:
	$(CC) $(FLAGS) -lpthread -lgtest -lgmp -lgmpxx -o tests/gauss $(TESTS_GAUSS)

graph_tests:
	$(CC) $(FLAGS) -lpthread -lgtest -lgmp -lgmpxx -o tests/graph $(TESTS_GRAPH)

oct_info: $(OCT_INFO_STUFF)
	$(CC) $(FLAGS) -lpthread -lgmp -lgmpxx -o oct_info $(OCT_INFO_STUFF)

oct_checker: $(OCT_CH_STUFF)
	$(CC) $(FLAGS) -lpthread -lgmp -lgmpxx -o oct_checker $(OCT_CH_STUFF)

