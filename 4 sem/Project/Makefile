override CXXFLAGS += $(shell wx-config --cxxflags)
override CXXFLAGS += $(shell pkg-config --cflags opencv)
override LDLIBS += $(shell wx-config --libs)
override LDLIBS += $(shell pkg-config --libs opencv)

all: calc

exprtk.hpp.gch: exprtk.hpp
	$(CXX) exprtk.hpp $(CXXFLAGS)

precompiled_headers.h.gch: precompiled_headers.h exprtk.hpp.gch
	$(CXX) precompiled_headers.h $(CXXFLAGS)

libwxconvertcv.a: WxConvertCv.h WxConvertCv.cpp precompiled_headers.h.gch
	$(CXX) -c WxConvertCv.cpp $(CXXFLAGS)
	$(AR) -rc libwxconvertcv.a WxConvertCv.o
	ranlib libwxconvertcv.a
	rm WxConvertCv.o

libhandreco.a: override LDLIBS += -lwxconvertcv
libhandreco.a: handwritten-recognition.h handwritten-recognition.cpp libwxconvertcv.a precompiled_headers.h.gch
	$(CXX) -c handwritten-recognition.cpp $(CXXFLAGS)
	$(AR) -rc libhandreco.a handwritten-recognition.o
	ranlib libhandreco.a
	rm handwritten-recognition.o

libtrainsvm.a: trainSVM.h trainSVM.cpp precompiled_headers.h.gch
	$(CXX) -c trainSVM.cpp $(CXXFLAGS)
	$(AR) -rc libtrainsvm.a trainSVM.o
	ranlib libtrainsvm.a
	rm trainSVM.o

calc: override LDLIBS += -L./
calc: override LDLIBS += -lhandreco
calc: override LDLIBS += -lwxconvertcv
calc: override LDLIBS += -ltrainsvm
calc: calculator-gui.cpp  libhandreco.a libwxconvertcv.a libtrainsvm.a precompiled_headers.h.gch
	$(CXX) calculator-gui.cpp $(CXXFLAGS) $(LDLIBS) -o calc
