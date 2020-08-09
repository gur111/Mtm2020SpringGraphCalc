# CXX=/usr/local/bin/g++
CXX=g++

$(shell mkdir -p bin)


COMP_FLAGS = -std=c++11 -pedantic-errors -Wall -Werror
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
UTILS = Utils.o
BIN_DIR = ./bin

OBJS = $(EXCEPTIONS) $(BIN_TREE) $(CALC_RUNNER) $(GRAPH) $(GRAPH_CALC) $(MAIN) $(PARSER) $(STORAGE_MANAGER) $(TREE_RUNNER) $(UTILS)
PROG = gcalc


$(PROG): $(OBJS)
	$(CXX) $(RELEASE_TYPE_FLAGS) $(OBJS) -o $@


$(BIN_DIR)/$(EXCEPTIONS): Exceptions.cpp Exceptions.h Constants.h
	$(CXX) -c $(RELEASE_TYPE_FLAGS) $(COMP_FLAGS) $(BIN_DIR)/$*.c

$(BIN_DIR)/$(BIN_TREE): BinTree.cpp BinTree.h Constants.h
	$(CXX) -c $(RELEASE_TYPE_FLAGS) $(COMP_FLAGS) $(BIN_DIR)/$*.c

$(BIN_DIR)/$(CALC_RUNNER): CalcRunner.cpp CalcRunner.h Constants.h
	$(CXX) -c $(RELEASE_TYPE_FLAGS) $(COMP_FLAGS) $(BIN_DIR)/$*.c

$(BIN_DIR)/$(GRAPH): Graph.cpp Graph.h Constants.h
	$(CXX) -c $(RELEASE_TYPE_FLAGS) $(COMP_FLAGS) $(BIN_DIR)/$*.c

$(BIN_DIR)/$(GRAPH_CALC): GraphCalc.cpp GraphCalc.h Constants.h
	$(CXX) -c $(RELEASE_TYPE_FLAGS) $(COMP_FLAGS) $(BIN_DIR)/$*.c

$(BIN_DIR)/$(MAIN): Main.cpp Main.h Constants.h
	$(CXX) -c $(RELEASE_TYPE_FLAGS) $(COMP_FLAGS) $(BIN_DIR)/$*.c

$(BIN_DIR)/$(PARSER): Parser.cpp Parser.h Constants.h
	$(CXX) -c $(RELEASE_TYPE_FLAGS) $(COMP_FLAGS) $(BIN_DIR)/$*.c

$(BIN_DIR)/$(STORAGE_MANAGER): StorageManager.cpp StorageManager.h Constants.h
	$(CXX) -c $(RELEASE_TYPE_FLAGS) $(COMP_FLAGS) $(BIN_DIR)/$*.c

$(BIN_DIR)/$(TREE_RUNNER): TreeRunner.cpp TreeRunner.h Constants.h
	$(CXX) -c $(RELEASE_TYPE_FLAGS) $(COMP_FLAGS) $(BIN_DIR)/$*.c

$(BIN_DIR)/$(UTILS): Utils.cpp Utils.h Constants.h
	$(CXX) -c $(RELEASE_TYPE_FLAGS) $(COMP_FLAGS) $(BIN_DIR)/$*.c




clean:
	rm -rf $(BIN_DIR) $(PROG)
