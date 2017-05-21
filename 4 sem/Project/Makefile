CXXFLAGS += $(shell wx-config --cxxflags)
CXXFLAGS += $(shell pkg-config --cflags opencv)
LDLIBS += $(shell wx-config --libs)
LDLIBS += $(shell pkg-config --libs opencv)

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

calc: calculator-gui.cpp
calc: LDLIBS += -L./
calc: LDLIBS += -lhandreco
calc: LDLIBS += -lwxconvertcv
calc:
	$(CXX) calculator-gui.cpp $(CXXFLAGS) $(LDLIBS) -o calc
