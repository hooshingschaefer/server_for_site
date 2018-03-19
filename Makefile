

FLAGS = -g -O0 -Wall -Wextra -Wold-style-cast
GPP = g++ -std=gnu++17 ${FLAGS}
DEPFILE = Makefile.dep
EXEBIN = server
CPPSRC = server.cpp protocol.cpp
HEADERS = protocol.h
OBJECTS = ${CPPSRC:.cpp=.o}

all: ${EXEBIN}

${EXEBIN}: ${OBJECTS}
	${GPP} -o $@ ${OBJECTS}
%.o : %.cpp
	${GPP} -c ${CPPSRC} $<

clean:
	rm ${EXEBIN} ${OBJECTS}

