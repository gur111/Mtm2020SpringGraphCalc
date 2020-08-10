CXX=/usr/local/bin/g++
# CXX=g++
SERVER_PYTHON=/usr/local/include/python3.6m
LOCAL_PYTHON=/usr/include/python3.8
PYTHON_INCLUDE=$(SERVER_PYTHON)


$(shell mkdir -p bin)


COMP_FLAGS = -std=c++11 -pedantic-errors -Wall -Werror -fPIC
DEBUG_FLAGS = -g
PRODUCTION_FLAGS = -DNDEBUG
RELEASE_TYPE_FLAGS = $(PRODUCTION_FLAGS) 

EXCEPTIONS = Exceptions.o
BIN_TREE = BinTree.o
CALC_RUNNER = CalcRunner.o
GRAPH = Graph.o
GRAPH_CALC = GraphCalc.o
MAIN = Main.o
PARSER = Parser.o
STORAGE_MANAGER = StorageManager.o
TREE_RUNNER = TreeRunner.o
GRAPH_SWIG = GraphSwig.o
UTILS = Utils.o
BIN_DIR = ./bin

OBJ_NAMES = $(EXCEPTIONS) $(BIN_TREE) $(CALC_RUNNER) $(GRAPH) $(GRAPH_CALC) $(MAIN) $(PARSER) $(STORAGE_MANAGER) $(TREE_RUNNER) $(UTILS)
OBJS=$(addprefix $(BIN_DIR)/, $(OBJ_NAMES))
PROG = gcalc


$(PROG): $(OBJ_NAMES)
	$(CXX) $(RELEASE_TYPE_FLAGS) $(OBJS) -o $@


$(EXCEPTIONS): Exceptions.cpp Exceptions.h Constants.h
	$(CXX) -c $(RELEASE_TYPE_FLAGS) $(COMP_FLAGS) $*.cpp -o $(BIN_DIR)/$*.o

$(BIN_TREE): BinTree.cpp BinTree.h Constants.h
	$(CXX) -c $(RELEASE_TYPE_FLAGS) $(COMP_FLAGS) $*.cpp -o $(BIN_DIR)/$*.o

$(CALC_RUNNER): CalcRunner.cpp CalcRunner.h Constants.h
	$(CXX) -c $(RELEASE_TYPE_FLAGS) $(COMP_FLAGS) $*.cpp -o $(BIN_DIR)/$*.o

$(GRAPH): Graph.cpp Graph.h Constants.h
	$(CXX) -c $(RELEASE_TYPE_FLAGS) $(COMP_FLAGS) $*.cpp -o $(BIN_DIR)/$*.o

$(GRAPH_CALC): GraphCalc.cpp GraphCalc.h Constants.h
	$(CXX) -c $(RELEASE_TYPE_FLAGS) $(COMP_FLAGS) $*.cpp -o $(BIN_DIR)/$*.o

$(MAIN): Main.cpp Constants.h
	$(CXX) -c $(RELEASE_TYPE_FLAGS) $(COMP_FLAGS) $*.cpp -o $(BIN_DIR)/$*.o

$(PARSER): Parser.cpp Parser.h Constants.h
	$(CXX) -c $(RELEASE_TYPE_FLAGS) $(COMP_FLAGS) $*.cpp -o $(BIN_DIR)/$*.o

$(STORAGE_MANAGER): StorageManager.cpp StorageManager.h Constants.h
	$(CXX) -c $(RELEASE_TYPE_FLAGS) $(COMP_FLAGS) $*.cpp -o $(BIN_DIR)/$*.o

$(TREE_RUNNER): TreeRunner.cpp TreeRunner.h Constants.h
	$(CXX) -c $(RELEASE_TYPE_FLAGS) $(COMP_FLAGS) $*.cpp -o $(BIN_DIR)/$*.o

$(UTILS): Utils.cpp Utils.h Constants.h
	$(CXX) -c $(RELEASE_TYPE_FLAGS) $(COMP_FLAGS) $*.cpp -o $(BIN_DIR)/$*.o

$(GRAPH_SWIG): GraphSwig.h GraphSwig.cpp $(GRAPH)
	$(CXX) -c $(RELEASE_TYPE_FLAGS) $(COMP_FLAGS) $*.cpp -o $(BIN_DIR)/$*.o
	

libgraph.a: $(EXCEPTIONS) $(GRAPH) $(UTILS) $(GRAPH_SWIG)
	ar -rs $@ $(addprefix $(BIN_DIR)/, $^)

swig: libgraph.a $(GRAPH_SWIG)
	swig -python graph.i
	$(CXX) $(COMP_FLAGS) graph_wrap.c $(BIN_DIR)/$(EXCEPTIONS) $(BIN_DIR)/$(GRAPH) $(BIN_DIR)/$(UTILS) $(BIN_DIR)/$(GRAPH_SWIG) \
    -I$(PYTHON_INCLUDE) -fPIC -shared -o _graph.so

tar:
	# cp mtm_final_test/gur_input.txt ./test_in.txt
	# cp mtm_final_test/gur_output.txt ./test_out.txt
	zip gcalc.zip graph.i design.pdf Makefile test_in.txt test_out.txt *.cpp *.h
	# rm ./test_in.txt ./test_out.txt


clean:
	rm -rf $(BIN_DIR) $(PROG)
	rm -f gcalc
	rm -f *.py
	rm -f *.so
