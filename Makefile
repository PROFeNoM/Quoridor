INCLUDE_DIR			=	./src/include
BUILD_DIR			=	./build/server ./build/player ./build/tests
INSTALL_DIR 		= 	./install
TESTS_DIR			=	./src/test/
DOCS_DIR			=	./docs/

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
CPPFLAGS 			= 	-I${INCLUDE_DIR}
LDFLAGS 			= 	`gsl-config --cflags --libs` -ldl
endif
CFLAGS 				= 	-Wall -Wextra -std=c99
DEPFLAGS			= 	-MT $@ -MMD -MP -MF $(@D)/$*.d
LIBFLAGS 			= 	-shared
DYNAMICFLAGS		=	-fPIC
TESTSFLAGS			=	-g -O0 -fprofile-arcs -ftest-coverage

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
	rm -rf ./build $(INSTALL_BIN) $(INSTALL_LIB) *.gcno *.gcda

mrproper: clean
	rm -rf $(DOCS_DIR)

docs: ./doxyfile
	@doxygen ./doxyfile

############################## HELPERS RULES ##############################

.PHONY: all build test install clean docs %_start %_end

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
	@$(CC) $(CFLAGS) -I./src/tests/ $(CPPFLAGS) $(TESTSFLAGS) $(TESTS_SRC) -o $@ $(LDFLAGS)
