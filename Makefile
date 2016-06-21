CC = gcc
# flags
CFLAGS = -g -Wall
LDFLAGS=-L/usr/local/opt/openssl/lib
CPPFLAGS=-I/usr/local/opt/openssl/include

#OpenSSH library
LIBS = -lcrypto

#target executable
TARGET = avalancha

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(TARGET).c -o $(TARGET) $(LIBS) $(LDFLAGS) 

  clean:
	$(RM) $(TARGET)



