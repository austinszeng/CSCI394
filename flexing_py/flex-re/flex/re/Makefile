all: oh-one-one no-ab calculator

oh-one-one: oh-one-one.ll
	flex -o oh-one-one.cc oh-one-one.ll
	g++ -std=c++17 -g -o oh-one-one oh-one-one.cc

no-ab: no-ab.ll
	flex -o no-ab.cc no-ab.ll
	g++ -std=c++17 -g -o no-ab no-ab.cc

calculator: calculator.ll
	flex -o calculator.cc calculator.ll
	g++ -std=c++17 -g -o calculator calculator.cc

clean:
	touch oh-one-one no-ab calculator foo~
	rm *~ oh-one-one no-ab calculator
	touch oh-one-one.cc no-ab.cc calculator.cc
	rm  oh-one-one.cc no-ab.cc calculator.cc
