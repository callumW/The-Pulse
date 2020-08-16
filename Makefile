GLFW_PATH=/usr/local/Cellar/glfw/3.3.2/
GLM_PATH=/usr/local/Cellar/glm/0.9.9.5/

SRC= \
src/main.cpp \
src/glad.c \
src/utils.cpp \
src/Shader.cpp \
src/Camera.cpp

INCLUDES= \
-Iinclude \
-I${GLFW_PATH}/include \
-I${GLM_PATH}/include

LIBS= \
-L${GLFW_PATH}/lib -lglfw \

LD_FLAGS = ${LIBS}

CFLAGS= -Wall -g

BINOUT=the-pulse

.PHONY: all
all:
	clang++ -std=c++14 ${CFLAGS} ${INCLUDES} ${SRC} ${LD_FLAGS} -o ${BINOUT}

.PHONY: clean
clean:
	rm -f ${BINOUT}
	rm -rf *.dSYM
