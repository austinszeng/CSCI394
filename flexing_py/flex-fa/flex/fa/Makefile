all: oh-one-one balanced end-oh-one-one even-ab balanced-bracks

even-oh-one: even-oh-one.ll FALexer.hh
	flex -o even-oh-one.cc even-oh-one.ll
	g++ -std=c++17 -g -o even-oh-one even-oh-one.cc

balanced-bracks: balanced-bracks.ll balanced-bracks.hh
	flex -o balanced-bracks.cc balanced-bracks.ll
	g++ -std=c++17 -g -o balanced-bracks balanced-bracks.cc

even-ab: even-ab.ll FALexer.hh
	flex -o even-ab.cc even-ab.ll
	g++ -std=c++17 -g -o even-ab even-ab.cc

end-oh-one-one: end-oh-one-one.ll FALexer.hh
	flex -o end-oh-one-one.cc end-oh-one-one.ll
	g++ -std=c++17 -g -o end-oh-one-one end-oh-one-one.cc

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
