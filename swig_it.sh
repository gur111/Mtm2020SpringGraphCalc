set -e
swig -c++ -python Graph.i
g++ -std=c++11 -pedantic-errors -Wall -Werror Graph_wrap.cxx Utils.cpp Graph.cpp Exceptions.cpp GraphSwig.cpp \
    -I/usr/include/python3.8  -fPIC -shared -o _Graph.so