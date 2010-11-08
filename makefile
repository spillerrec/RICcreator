CPP = C:/MinGW/bin/g++.exe
CFLAGS = -g -O2
OBJECTS = $(OBJECT_PATH)/main.o

OBJECT_PATH = bin/obj
PROGRAM_PATH = bin
SOURCE_PATH = src


RICLIB_PATH = $(SOURCE_PATH)/riclib
RICLIB_OBJECT_PATH = $(OBJECT_PATH)/riclib
RICLIB_OBJECTS = $(RICLIB_OBJECT_PATH)/ricfile.o $(RICLIB_OBJECT_PATH)/ricObjectRead.o $(RICLIB_OBJECT_PATH)/ricObjectWrite.o $(RICLIB_OBJECT_PATH)/ricPixel.o $(RICLIB_OBJECT_PATH)/nxtCanvas.o


$(PROGRAM_PATH)/main.exe : $(RICLIB_OBJECTS) $(OBJECTS)
	$(CPP) $(CFLAGS) $(RICLIB_OBJECTS) $(OBJECTS) -o $(PROGRAM_PATH)/main.exe
	
	
######riclib######


$(RICLIB_OBJECT_PATH)/ricfile.o : $(RICLIB_PATH)/ricfile.h $(RICLIB_PATH)/ricfile.cpp $(RICLIB_PATH)/ricObject.h
	$(CPP) $(CFLAGS) -c $(RICLIB_PATH)/ricfile.cpp -o $(RICLIB_OBJECT_PATH)/ricfile.o

$(RICLIB_OBJECT_PATH)/ricObjectRead.o : $(RICLIB_PATH)/ricObject.h $(RICLIB_PATH)/ricObjectRead.cpp
	$(CPP) $(CFLAGS) -c $(RICLIB_PATH)/ricObjectRead.cpp -o $(RICLIB_OBJECT_PATH)/ricObjectRead.o

$(RICLIB_OBJECT_PATH)/ricObjectWrite.o : $(RICLIB_PATH)/ricObject.h $(RICLIB_PATH)/ricObjectWrite.cpp
	$(CPP) $(CFLAGS) -c $(RICLIB_PATH)/ricObjectWrite.cpp -o $(RICLIB_OBJECT_PATH)/ricObjectWrite.o

$(RICLIB_OBJECT_PATH)/ricPixel.o : $(RICLIB_PATH)/ricPixel.h $(RICLIB_PATH)/ricPixel.cpp
	$(CPP) $(CFLAGS) -c $(RICLIB_PATH)/ricPixel.cpp -o $(RICLIB_OBJECT_PATH)/ricPixel.o

$(RICLIB_OBJECT_PATH)/nxtCanvas.o : $(RICLIB_PATH)/nxtCanvas.h $(RICLIB_PATH)/nxtCanvas.cpp $(RICLIB_PATH)/ricPixel.h
	$(CPP) $(CFLAGS) -c $(RICLIB_PATH)/nxtCanvas.cpp -o $(RICLIB_OBJECT_PATH)/nxtCanvas.o


$(OBJECT_PATH)/main.o : $(SOURCE_PATH)/main.cpp $(RICLIB_PATH)/ricfile.h $(RICLIB_PATH)/nxtCanvas.h $(RICLIB_PATH)/ricPixel.h
	$(CPP) $(CFLAGS) -c $(SOURCE_PATH)/main.cpp -o $(OBJECT_PATH)/main.o
