calculator-gui: override LDLIBS+=$(shell wx-config --libs)
calculator-gui: override CXXFLAGS+=$(shell wx-config --cflags)
all: calculator-gui

calc:
	g++ -std=c++11 calculator-gui.cpp handwritten-recognition.cpp WxConvertCv.cpp `wx-config --cflags --libs` `pkg-config --cflags --libs opencv` -g2 -D__DEBUG -o calc
