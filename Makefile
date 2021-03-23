SRC_DIR = src
INSTALL_DIR = install
PLAYER_DIR = ${SRC_DIR}/player
BUILD_DIR = build

BIN = server #alltest

CC = gcc
CPPFLAGS = -I ${SRC_DIR}
CFLAGS = -Wall -Wextra -std=c99 -g
LDFLAGS = `gsl-config --cflags --libs` -ldl
LIBFLAGS = -shared -fPIC

PLAYER_SRC=${wildcard ${PLAYER_DIR}/*.c}
LIB=${PLAYER_SRC:${PLAYER_DIR}/%.c=${BUILD_DIR}/%.so}

SERVER_SRC=${wildcard ${SRC_DIR}/*.c}

LIB_SRC=${wildcard ${BUILD_DIR}/*.so}
COPY_LIB1=${LIB_SRC:${BUILD_DIR}/%.so=${INSTALL_DIR}/%_1.so}
COPY_LIB2=${LIB_SRC:${BUILD_DIR}/%.so=${INSTALL_DIR}/%_2.so}

$(info ${LIB})

.PHONY: all build test install clean

all: build install

build: ${BIN}

alltest: build

clean:
	rm -rf build
	rm -rf install/*
	rm -rf ${SRC_DIR}/*~
	rm -rf ${SRC_DIR}/player_bis.c

server: build_dir ${LIB} ${SERVER_SRC}
	${CC} ${SERVER_SRC} ${BUILD_DIR}/*.so -o ${BUILD_DIR}/server ${LDFLAGS}

${BUILD_DIR}/%.so: ${PLAYER_DIR}/%.c
	${CC} ${LIBFLAGS} $< ${SRC_DIR}/annex_function.c -o $@ ${LDFLAGS}

build_dir:
	mkdir -p build

install: ${COPY_LIB1} ${COPY_LIB2}
	cp ${BUILD_DIR}/server ./install/

${INSTALL_DIR}/%_1.so: ${BUILD_DIR}/%.so
	cp $< $@

${INSTALL_DIR}/%_2.so: ${BUILD_DIR}/%.so
	cp $< $@
