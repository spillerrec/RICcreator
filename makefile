CPP = C:/Development/MinGW/bin/g++.exe
CFLAGS = -g -O2
OBJECTS = $(OBJECT_PATH)/main.o

OBJECT_PATH = bin/obj
PROGRAM_PATH = bin
SOURCE_PATH = src


RICLIB_PATH = $(SOURCE_PATH)/riclib
RICLIB_OBJECT_PATH = $(OBJECT_PATH)/riclib
RICLIB_OBJECTS = $(RICLIB_OBJECT_PATH)/ricfile.o $(RICLIB_OBJECT_PATH)/ricObjectRead.o


$(PROGRAM_PATH)/main.exe : $(RICLIB_OBJECTS) $(OBJECTS)
	$(CPP) $(CFLAGS) $(RICLIB_OBJECTS) $(OBJECTS) -o $(PROGRAM_PATH)/main.exe
	
	
######riclib######


$(RICLIB_OBJECT_PATH)/ricfile.o : $(RICLIB_PATH)/ricfile.h $(RICLIB_PATH)/ricfile.cpp $(RICLIB_PATH)/ricObject.h
	$(CPP) $(CFLAGS) -c $(RICLIB_PATH)/ricfile.cpp -o $(RICLIB_OBJECT_PATH)/ricfile.o

$(RICLIB_OBJECT_PATH)/ricObjectRead.o : $(RICLIB_PATH)/ricObject.h $(RICLIB_PATH)/ricObjectRead.cpp
	$(CPP) $(CFLAGS) -c $(RICLIB_PATH)/ricObjectRead.cpp -o $(RICLIB_OBJECT_PATH)/ricObjectRead.o



$(OBJECT_PATH)/main.o : $(SOURCE_PATH)/main.cpp $(RICLIB_PATH)/ricfile.h
	$(CPP) $(CFLAGS) -c $(SOURCE_PATH)/main.cpp -o $(OBJECT_PATH)/main.o
