CC = g++
CFLAGS = -c
LDFLAGS =
SOURCES = main.cpp websocket.cpp sha1.cpp base64.cpp GameState.cpp Player.cpp Latency.cpp Parse.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = snake-server

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *o $(EXECUTABLE)
