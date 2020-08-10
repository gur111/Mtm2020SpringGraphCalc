set -e
swig -c++ -python graph.i -outdir ./bin
g++ -std=c++11 -pedantic-errors -Wall -Werror Graph_wrap.cxx Utils.cpp graph.cpp Exceptions.cpp GraphSwig.cpp \
    -I/usr/include/python3.8  -fPIC -shared -o _graph.so