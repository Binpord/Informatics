override CXXFLAGS += $(shell wx-config --cxxflags)
override CXXFLAGS += $(shell pkg-config --cflags opencv)
override LDLIBS += $(shell wx-config --libs)
override LDLIBS += $(shell pkg-config --libs opencv)

precompiled: precompiled_headers.h
	$(CXX) precompiled_headers.h $(CXXFLAGS)

WxConvertCv: WxConvertCv.h WxConvertCv.cpp
	$(CXX) -c WxConvertCv.cpp $(CXXFLAGS)
	$(AR) -rc libwxconvertcv.a WxConvertCv.o
	ranlib libwxconvertcv.a
	rm WxConvertCv.o

exprtk: exprtk.hpp
	$(CXX) exprtk.hpp $(CXXFLAGS)

handwritten-recognition: handwritten-recognition.h handwritten-recognition.cpp
	$(CXX) -c handwritten-recognition.cpp $(CXXFLAGS)
	$(AR) -rc libhandreco.a handwritten-recognition.o
	ranlib libhandreco.a
	rm handwritten-recognition.o

trainSVM: trainSVM.h trainSVM.cpp
	$(CXX) -c trainSVM.cpp $(CXXFLAGS)
	$(AR) -rc libtrainsvm.a trainSVM.o
	ranlib libtrainsvm.a
	rm trainSVM.o

calc: calculator-gui.cpp
calc: override LDLIBS += -L./
calc: override LDLIBS += -lhandreco
calc: override LDLIBS += -lwxconvertcv
calc: override LDLIBS += -ltrainsvm
calc:
	$(CXX) calculator-gui.cpp $(CXXFLAGS) $(LDLIBS) -o calc
