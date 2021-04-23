CAVEDIR = /usr/local/CAVE
CAVELIBDIR = $(CAVEDIR)/lib
CAVEINCDIR = $(CAVEDIR)/include
SNDINCDIR = /usr/local/CAVE/include
SNDLIBDIR = /usr/local/CAVE/lib

# The Wavefront library is needed for the dcuWFAvatar class
WAVELIBDIR = $(HOME)/lib
WAVEINCDIR = $(HOME)/include

OPTorDEBUG = -O

CFLAGS = -I$(CAVEINCDIR) -I$(SNDINCDIR) -I$(WAVEINCDIR) $(OPTorDEBUG) \
		-DOPENGL -UIRISGL  -DTEXTURE_OBJECT  -32
C++FLAGS = $(CFLAGS)
LFLAGS = $(OPTorDEBUG)


LIBOBJS = dcuAudioEnv.o \
	dcuSample.o dcuLoopSample.o dcuRandomSample.o dcuTriggerSample.o \
	dcuWandTriggerSample.o \
	dcuVolume.o dcuPointVolume.o dcuSphereVolume.o dcuBoxVolume.o \
	dcuTransform.o dcuGrabber.o \
	dcuTrigger.o \
	dcuDump.o \
	dcuImage.o dcuSharedImage.o dcuTexture.o dcuTextureSet.o \
	dcuAvatar.o dcuWFAvatar.o dcuTexAvatar.o \
	misc.o glmisc.o


libdcu.a: $(LIBOBJS)
	rm -f libdcu.a
	ar ru libdcu.a $(LIBOBJS)
