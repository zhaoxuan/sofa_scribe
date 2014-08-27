OS=$(shell uname -s)
CC=g++
ifeq ($(OS), Darwin)
	CXXFLAGS= -std=c++11
else
	CXXFLAGS=
endif
LIBS= -lthrift
INCLUDE= -I./src -I/usr/local/include/thrift
LIB_DIR= -L/usr/local/lib -L/usr/lib
SOURCE= scribeClient.cpp
GENCPP= src/scribe_types.cpp \
		src/scribe.cpp \
		src/scribe_constants.cpp \
		src/zipkinCore_types.cpp \
		src/zipkinCore_constants.cpp \
		src/formatters.cpp \
		src/base64.cpp


default:
	$(CC) $(CXXFLAGS) -o main $(INCLUDE) $(LIB_DIR) $(LIBS) $(SOURCE) $(GENCPP)
