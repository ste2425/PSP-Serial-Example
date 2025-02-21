TARGET = User
OBJS = main.o kernel.o

INCDIR = 
CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR = ../libs
LDFLAGS =

LIBS = -lpspkubridge

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = User

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
