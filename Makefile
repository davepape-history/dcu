CAVEDIR = /usr/local/CAVE
CAVELIBDIR = $(CAVEDIR)/lib
CAVEINCDIR = $(CAVEDIR)/include
SNDINCDIR = /usr/local/audio
SNDLIBDIR = /usr/local/audio

# The Wavefront library is only needed for demo5, demoGrab, and demoGrab2
WAVELIBDIR = $(HOME)/lib
WAVEINCDIR = $(HOME)/include

OPTorDEBUG = -O

CFLAGS = -Iinclude -I$(CAVEINCDIR) -I$(SNDINCDIR) -I$(WAVEINCDIR) $(OPTorDEBUG) \
		-DOPENGL -UIRISGL  -DTEXTURE_OBJECT  -32
C++FLAGS = $(CFLAGS)
LFLAGS = $(OPTorDEBUG)

LIBS = -Llib -ldcu \
	-L$(CAVELIBDIR) -lcave_ogl \
	-L$(SNDLIBDIR) -lsnd \
	-L$(WAVELIBDIR) -lwave_ogl \
	-lGL -lGLU -lXi -lX11 -lm -limage




demo0: demo0.o
	$(C++) $(LFLAGS) demo0.o -o demo0 $(LIBS)

demo1: demo1.o
	$(C++) $(LFLAGS) demo1.o -o demo1 $(LIBS)

demo2: demo2.o
	$(C++) $(LFLAGS) demo2.o -o demo2 $(LIBS)

demo3: demo3.o
	$(C++) $(LFLAGS) demo3.o -o demo3 $(LIBS)

demo4: demo4.o
	$(C++) $(LFLAGS) demo4.o -o demo4 $(LIBS)

demo5: demo5.o
	$(C++) $(LFLAGS) demo5.o -o demo5 $(LIBS)

demoGrab: demoGrab.o
	$(C++) $(LFLAGS) demoGrab.o -o demoGrab $(LIBS)

demoGrab2: demoGrab2.o
	$(C++) $(LFLAGS) demoGrab2.o -o demoGrab2 $(LIBS)

compass: compass.o
	$(C++) $(LFLAGS) compass.o -o compass $(LIBS)
