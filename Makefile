CXX = g++
CXXFLAGS = -std=c++14 -g -Wall -MMD -Werror=vla #-fsanitize=address
#BUILD = bin
OBJECTS = main.o grid.o observer.o building.o subject.o player.o tile.o grid_constants.o game.o components.o dice.o
DEPENDS = ${OBJECTS:.o=.d}
EXEC = constructor

${EXEC} : ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}
#	mv ${DEPENDS} bin/.
#	mv ${OBJECTS} bin/.

clean :
	rm ${DEPENDS} ${OBJECTS}

-include ${DEPENDS} # reads the .d files and reruns dependencies
