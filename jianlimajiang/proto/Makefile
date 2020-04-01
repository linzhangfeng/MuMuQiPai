CC = gcc
CXX = g++
CFLAGS = -ggdb -O0 -Wall
CXXFLAGS = -ggdb -O0 -fno-strict-aliasing -Wno-write-strings -frtti -fexceptions -Wall
INCPATH = -I../include
LD = gcc
LFLAGS = 
LIBS = -L../lib  -lprotobuf -lstdc++
	
STATIC_LIB = libcomproto.a
OBJECTS_LIB = gameproto.pb.o loginproto.pb.o redisproto.pb.o jianligameproto.pb.o

all: $(STATIC_LIB)

$(STATIC_LIB): $(OBJECTS_LIB)	
	ar -r $(STATIC_LIB) $(OBJECTS_LIB)
	mv $(STATIC_LIB) ../lib
	
makeproto:  
	protoc --cpp_out=./ *.proto
	
.SUFFIXES: .cpp .cc .cxx .c
.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

.pb.cc.o:
	$(CXX) -c $(CFLAGS) $(INCPATH) -o $@ $<
	
clean:
	rm -rf ./*.pb.*
