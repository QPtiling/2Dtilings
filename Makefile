CPP=g++
OBJ=graph.o src.o

GRAPH: $(OBJ)
	$(CPP) $(OBJ) -o $@

.cpp.o:
	$(CPP) -c $(CPPFLAGS) -o $@ $<

fig:	
	gnuplot tiling.gl

clean:
	rm -rf *.o *~ sites bonds

