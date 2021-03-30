INCLUDE_DIR			=	./src/include
BUILD_DIR			=	./build/server ./build/player ./build/tests
INSTALL_DIR 		= 	./install
TESTS_DIR			=	./src/test/

SERVER_SRC			= 	$(wildcard ./src/*.c)
PLAYER_SRC 			= 	$(wildcard ./src/player/*.c)
COMMON_SRC			=	$(wildcard ./src/common/*.c)
TESTS_SRC			=	$(wildcard ./src/tests/*.c) $(COMMON_SRC) $(shell echo $(SERVER_SRC) | sed 's/.\/src\/main.c//')

SERVER_OBJ			=	$(SERVER_SRC:./src/%.c=./build/server/%.o)
PLAYER_OBJ			=	$(PLAYER_SRC:./src/player/%.c=./build/player/%.o)
PLAYER_COMMON_OBJ	=	$(COMMON_SRC:./src/common/%.c=./build/player/%.o)
SERVER_COMMON_OBJ	=	$(COMMON_SRC:./src/common/%.c=./build/server/%.o)
COMMON_OBJ			=	$(COMMON_SRC:%.c=%.o)

PLAYER_LIB			=	$(PLAYER_OBJ:%.o=%.so)
SERVER_BIN			=	./build/server/server
TESTS_BIN			=	./build/tests/alltests
INSTALL_LIB			=	$(PLAYER_LIB:./build/player/%.so=$(INSTALL_DIR)/%_1.so) $(PLAYER_LIB:./build/player/%.so=$(INSTALL_DIR)/%_2.so)
INSTALL_BIN			=	$(SERVER_BIN:./build/server/%=$(INSTALL_DIR)/%) $(TESTS_BIN:./build/tests/%=$(INSTALL_DIR)/%)

DEP					=	$(PLAYER_OBJ:%.o=%.d) $(PLAYER_COMMON_OBJ:%.o=%.d) $(SERVER_OBJ:%.o=%.d) $(SERVER_COMMON_OBJ:%.o=%.d)

CC 					= 	gcc
ifdef GSL_PATH
CPPFLAGS 			= 	-I${INCLUDE_DIR} -I${GSL_PATH}/include -lm -L$(GSL_PATH)/lib/ -lgsl -lgslcblas
LDFLAGS 			= 	`$(GSL_PATH)/bin/gsl-config --cflags --libs` -ldl
else
CPPFLAGS 			= 	-I${INCLUDE_DIR} -I$ -lm
LDFLAGS 			= 	`gsl-config --cflags --libs` -ldl
endif
CFLAGS 				= 	-Wall -Wextra -std=c99 -g
DEPFLAGS			= 	-MT $@ -MMD -MP -MF $(@D)/$*.d
LIBFLAGS 			= 	-shared
DYNAMICFLAGS		=	-fPIC
TESTSFLAGS			=	-lgcov

COLOR				=	"\033["
GREEN				=	";32m"
CYAN				=	";36m"
PURPLE				=	";35m"
GREY				=	"1;30m"
BOLD				=	";1"
ITALIC				=	";3"
NOCOLOR				=	"\033[0m"


############################## MAIN RULES ##############################

all : build

build: BUILD_start $(BUILD_DIR) $(PLAYER_COMMON_OBJ) $(SERVER_COMMON_OBJ) $(PLAYER_LIB) $(SERVER_BIN) $(TESTS_BIN)
	@echo $(COLOR)$(BOLD)$(GREEN)BUILD DONE$(NOCOLOR)

install: INSTALL_start build $(INSTALL_LIB) $(INSTALL_BIN) INSTALL_end

test:	TESTS_start $(TESTS_BIN) TESTS_end

clean:
	rm -rf ./build $(INSTALL_BIN) $(INSTALL_LIB)


############################## HELPERS RULES ##############################

.PHONY: all build test install clean %_start %_end

.SECONDARY: $(SERVER_OBJ) $(SERVER_COMMON_OBJ) $(PLAYER_OBJ) $(PLAYER_COMMON_OBJ) $(PLAYER_LIB) $(SERVER_BIN)

$(BUILD_DIR):
	@mkdir -p $@

%_start:
	@echo $(COLOR)$(BOLD)$(GREEN)$* START$(NOCOLOR)

%_end: 
	@echo $(COLOR)$(BOLD)$(GREEN)$* DONE$(NOCOLOR)

%_copy:
	@echo $(COLOR)$(GREY)"\t$* \tCOPIED"$(NOCOLOR)

%.o: %.c

$(DEP):
include $(wildcard $(DEP))


############################## BUILD RULES ################################

build/player/%.so: build/player/%.o
	@echo $(COLOR)$(ITALIC)$(PURPLE)"\tCompilation Player lib :	"$*$(NOCOLOR)
	@$(CC) $(CPPFLAGS) $(CFLAGS) $(LIBFLAGS) $(PLAYER_COMMON_OBJ) $< -o $@ $(LDFLAGS)

$(SERVER_BIN): $(SERVER_OBJ)
	@echo $(COLOR)$(ITALIC)$(PURPLE)"\tCompilation Server BINARY"$(NOCOLOR)
	@$(CC) $(CPPFLAGS) $(CFLAGS) $(SERVER_COMMON_OBJ) $(SERVER_OBJ) -o $@ $(LDFLAGS)

build/player/%.o: src/player/%.c
	@echo $(COLOR)$(ITALIC)$(CYAN)"\tCompilation Player type :	$*"$(NOCOLOR)
	@$(CC) $(DEPFLAGS) $(CPPFLAGS) $(CFLAGS) $(DYNAMICFLAGS) -c $< -o $@

build/player/%.o: src/common/%.c
	@echo $(COLOR)$(ITALIC)$(CYAN)"\tCompilation Common file :	$*"$(NOCOLOR)
	@$(CC) -MT $(addprefix build/player/,$(notdir $@)) -MMD -MP -MF build/player/$*.d $(CPPFLAGS) $(CFLAGS) $(DYNAMICFLAGS) -c $(addsuffix .c, $(basename $<)) -o $(addprefix build/player/,$(notdir $*.o))

build/server/%.o: src/common/%.c
	@echo $(COLOR)$(ITALIC)$(CYAN)"\tCompilation Common file :	$*"$(NOCOLOR)
	@$(CC) -MT $(addprefix build/server/,$(notdir $@)) -MMD -MP -MF build/server/$*.d $(CPPFLAGS) $(CFLAGS) -c $(addsuffix .c, $(basename $<)) -o $(addprefix build/server/,$(notdir $*.o))

build/server/%.o: src/%.c
	@echo $(COLOR)$(ITALIC)$(CYAN)"\tCompilation Server file :	$*"$(NOCOLOR)
	@$(CC) $(DEPFLAGS) $(CPPFLAGS) $(CFLAGS) -c $< -o $@


############################## INSTALL RULES ##############################

install/%_1.so: build/player/%.so %_1_copy
	@cp $< $@
install/%_2.so: build/player/%.so %_2_copy
	@cp $< $@
install/%: build/server/% %_copy
	@cp $< $@
install/%: build/tests/% %_copy
	@cp $< $@


############################## TESTS RULES ##############################

$(TESTS_BIN): $(BUILD_DIR)
	@echo $(COLOR)$(ITALIC)$(PURPLE)"\tCompilation AllTests BINARY"$(NOCOLOR)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(TESTSFLAGS) $(TESTS_SRC) -o $@ $(LDFLAGS)

# SRC_DIR = src
# INSTALL_DIR = install
# PLAYER_DIR = ${SRC_DIR}/player
# BUILD_DIR = build

# BIN = server #alltest

# CC = gcc
# CPPFLAGS = -I ${SRC_DIR}
# CFLAGS = -Wall -Wextra -std=c99 -g
# LDFLAGS = `gsl-config --cflags --libs` -ldl
# LIBFLAGS = -shared -fPIC

# PLAYER_SRC=${wildcard ${PLAYER_DIR}/*.c}
# LIB=${PLAYER_SRC:${PLAYER_DIR}/%.c=${BUILD_DIR}/%.so}

# SERVER_SRC=${wildcard ${SRC_DIR}/*.c}

# LIB_SRC=${wildcard ${BUILD_DIR}/*.so}
# COPY_LIB1=${LIB_SRC:${BUILD_DIR}/%.so=${INSTALL_DIR}/%_1.so}
# COPY_LIB2=${LIB_SRC:${BUILD_DIR}/%.so=${INSTALL_DIR}/%_2.so}

# $(info ${LIB})

# .PHONY: all build test install clean

# all: build install

# build: ${BIN}

# alltest: build

# clean:
# 	rm -rf build
# 	rm -rf install/*
# 	rm -rf ${SRC_DIR}/*~
# 	rm -rf ${SRC_DIR}/player_bis.c

# server: build_dir ${LIB} ${SERVER_SRC}
# 	${CC} ${SERVER_SRC} ${BUILD_DIR}/*.so -o ${BUILD_DIR}/server ${LDFLAGS}

# ${BUILD_DIR}/%.so: ${PLAYER_DIR}/%.c
# 	${CC} ${LIBFLAGS} $< ${SRC_DIR}/utils.c -o $@ ${LDFLAGS}

# build_dir:
# 	mkdir -p build

# install: ${COPY_LIB1} ${COPY_LIB2}
# 	cp ${BUILD_DIR}/server ./install/

# ${INSTALL_DIR}/%_1.so: ${BUILD_DIR}/%.so
# 	cp $< $@

# ${INSTALL_DIR}/%_2.so: ${BUILD_DIR}/%.so
# 	cp $< $@