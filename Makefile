CC = g++
CFLAGS = -std=c++17 -Wall 
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

SOURCES = main.cpp grid.cpp
HEADERS = grid.hpp cell.hpp
OBJECTS = $(SOURCES:.cpp=.o)

EXECUTABLE = final

all: $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS) 
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXECUTABLE) $(LDFLAGS)
	chmod +x $(EXECUTABLE)

%.o: %.cpp $(HEADERS) 
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

# final: main.o
# 	$(CC) $(CFLAGS) $(LDFLAGS) main.o -o final
# 	chmod +x final

# main.o: main.cpp
# 	$(CC) $(CFLAGS) $(LDFLAGS) -c main.cpp

# clean: 
# 	rm main.o render_grid.o
