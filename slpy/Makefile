slpy: slpy-lex.o slpy-parse.o slpy.o slpy-ast.o slpy-util.o
	g++ -g -o slpy slpy-util.o slpy-lex.o slpy-ast.o slpy-parse.o slpy.o 

slpy-ast.o: slpy-ast.cc slpy-ast.hh slpy-util.hh
	g++ -std=c++17 -g -c slpy-ast.cc

slpy-lex.o: slpy-lex.cc slpy-lex.hh slpy-util.hh
	g++ -std=c++17 -g -c slpy-lex.cc

slpy-util.o: slpy-util.cc slpy-util.hh
	g++ -std=c++17 -g -c slpy-util.cc

slpy-parse.o: slpy-parse.cc slpy-parse.hh slpy-ast.hh slpy-util.hh slpy-lex.hh
	g++ -std=c++17 -g -c slpy-parse.cc

slpy.o: slpy.cc slpy-lex.hh slpy-ast.hh slpy-parse.hh slpy-util.hh
	g++ -std=c++17 -g -c slpy.cc

clean:
	touch slpy foo~ foo.o
	rm *~ *.o slpy


