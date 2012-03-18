CXX = g++
OBJ = obj/
SRC = src/
BIN = bin/
POINTC = $(wildcard $(SRC)*/*.c) $(wildcard $(SRC)*.c) 
POINTCPP = $(wildcard $(SRC)*/*.cpp) $(wildcard $(SRC)*.cpp) 
POINTOP := $(POINTC:.c=.o) $(POINTCPP:.cpp=.o)
POINTO = $(patsubst src/%,$(OBJ)%,$(POINTOP)) #$(POINTOP:src=obj)

OPT := -Wall -pedantic -I$(SRC) 


ifeq ($(SHELL), sh.exe) 
OS := Win
else
OS := $(shell uname)
endif

ifeq ($(OS), Linux)
RM = rm
LIBS := -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system
endif
ifeq ($(OS), Darwin)
RM = rm
LIBS := -framework sfml-system -framework sfml-window -framework sfml-graphics -framework sfml-audio -framework sfml-network -framework OpenGL
endif
ifeq ($(OS), Win)
RM = del
LIBS := -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system
endif

default :
	@echo "C Files:" $(POINTC) ; \
	echo "C++ Files:" $(POINTCPP) ; \
	echo "O Files:" $(POINTO) ; \
	echo $(OPT)

.PHONY : default


all : test dirs

dirs : 
	@if [ ! -e  $(BIN) ] ; \
	then \
	echo "Creating directory $(BIN)" ; \
	mkdir $(BIN) ; \
	fi ; \
	if [ ! -e  $(OBJ)Core ] ; \
	then \
	echo "Creating directory $(OBJ)Core" ; \
	mkdir $(OBJ)Core ; \
	fi ; \
	if [ ! -e  $(OBJ)Audio ] ; \
	then \
	echo "Creating directory $(OBJ)Audio" ; \
	mkdir $(OBJ)Audio ; \
	fi ; \
	if [ ! -e  $(OBJ)Physics ] ; \
	then \
	echo "Creating directory $(OBJ)Physics" ; \
	mkdir $(OBJ)Physics ; \
	fi ; 
.PHONY : dirs

$(OBJ)%.o : $(SRC)%.cpp
	$(CXX) $(OPT) $^ -c -o $@
	
#$(OBJ)%.o : $(SRC)%.cpp
#$(C) $(OPT) $^ -c -o $@

test : $(POINTO)
	$(CXX) $(OPT) $^ -o $(BIN)$@ $(LIBS)
	
debug : debug_option all

debug_option :
#OPT = -g

run : all
	./$(BIN)test
.PHONY : run

clean:
	@$(RM) -vf $(OBJ)*.o $(BIN)test $(POINTO)
.PHONY : clean

doc:
	doxygen doc/Doxyfile ; \
	chmod u+x doc/html/index.html ; \
	open doc/html/index.html
.PHONY : doc

info:
	@echo "Lignes\tMots\tOctets"
	@cd src ; \
	find -E . -regex "(.+)\.(h|cpp|c|hpp)" -print0 | xargs -0 cat | wc ; \
	cd ..
.PHONY : info
#find -E src/ -regex "(.+)\.(h|cpp|c|hpp)" -exec cat {} \; | wc -l | (read A ; echo "Lignes dans le projet: $A")
#find -E src/ -regex "(.+)\.(h|cpp|c|hpp)" -exec cat {} \; | wc | (read L W B ; echo "Le projet a $L lignes, $W mots et pèse $B octets")


 