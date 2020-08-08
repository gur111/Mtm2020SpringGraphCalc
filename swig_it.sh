set -e
swig -c++ -python Graph.i
g++ -c -DNDEBUG -pedantic-errors -Wall -Werror Graph.cpp Exceptions.cpp GraphSwig.cpp
ar -rs libgraph.a Graph.o GraphSwig.o Exceptions.o
g++ -DNDEBUG -pedantic-errors -Wall -Werror   \
    -I/usr/include/python3.6m -fPIC -shared libgraph.a Graph_wrap.cxx -o _graph.so
# ld -shared libgraph.a Graph_wrap.o -L. -L/usr/local/include/python3.8/ -o _graph.so
