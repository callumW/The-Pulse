GLFW_PATH=/usr/local/Cellar/glfw/3.3.2/
GLM_PATH=/usr/local/Cellar/glm/0.9.9.5/

SRC= \
src/main.cpp \
src/glad.c \
src/utils.cpp \
src/Shader.cpp \
src/Camera.cpp \
src/SpriteRenderer.cpp \
src/Texture.cpp

BOX_2D_PATH= /Users/cwilson/Libs/box2d/1025f9a
BOX_2D_INCLUDES= -I${BOX_2D_PATH}/include
BOX_2D_FLAGS= -L${BOX_2D_PATH} -lbox2d

INCLUDES= \
-Iinclude \
-I${GLFW_PATH}/include \
-I${GLM_PATH}/include \
${BOX_2D_INCLUDES}

LIBS= \
-L${GLFW_PATH}/lib -lglfw \
${BOX_2D_FLAGS}

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
