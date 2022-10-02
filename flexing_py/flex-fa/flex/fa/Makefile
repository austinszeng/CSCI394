all: oh-one-one balanced

oh-one-one: oh-one-one.ll FALexer.hh
	flex -o oh-one-one.cc oh-one-one.ll
	g++ -std=c++17 -g -o oh-one-one oh-one-one.cc

balanced: balanced.ll balanced.hh
	flex -o balanced.cc balanced.ll
	g++ -std=c++17 -g -o balanced balanced.cc

clean:
	touch oh-one-one balanced foo~
	rm *~ oh-one-one balanced
	touch oh-one-one.cc balanced.cc
	rm oh-one-one.cc balanced.cc
