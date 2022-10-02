all: foo baz

foo: foo.ll foo.hh
	flex -o foo.cc foo.ll
	g++ -std=c++17 -g -o foo foo.cc

baz: baz.ll baz.hh
	flex -o baz.cc baz.ll
	g++ -std=c++17 -g -o baz baz.cc

clean:
	touch foo~ foo foo.cc baz baz.cc
	rm *~ foo foo.cc baz baz.cc

