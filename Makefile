CC = gcc
# flags
CFLAGS = -g -Wall


#OpenSSH library
LIBS = -lcrypto

#target executable
TARGET = avalancha

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) $(TARGET).c -o $(TARGET) $(LIBS)  

  clean:
	$(RM) $(TARGET)



