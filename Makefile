CC = gcc
CFLAGS = -Wall -fPIC -c -O3
STATICFLAGS = -Wall -c -O3
SRC_DIR = ./src/
INCLUDE_DIR = ./inc/
OBJ_DIR = ./bin/
HEADERS = utils.h \
			doublell.h \
			socket.h
SRCS = buffer.c \
		doublell.c \
		socket.c
OBJS = $(SRCS:%.c=%.o)
OBJS_LOC = $(OBJS:%=$(OBJ_DIR)%)
LIB_DYNAMIC = libscott.so
LIB_STATIC = $(LIB_DYNAMIC:%.so=%.a)
INSTALL = install
INSTALL_PATH?=/usr/

.PHONY: all
all: $(LIB_DYNAMIC)

$(LIB_DYNAMIC): $(OBJS_LOC)
	$(CC) -shared $(OBJS_LOC) -o $(LIB_DYNAMIC)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $< -o $@ -I $(INCLUDE_DIR)

.PHONY: static
static: CFLAGS = $(STATICFLAGS)
static: $(LIB_STATIC)

$(LIB_STATIC): $(OBJS_LOC)
	ar -cvq $(LIB_STATIC) $(OBJS_LOC)

.PHONY: install
install: all static
	mkdir -p $(INSTALL_PATH)lib
	mkdir -p $(INSTALL_PATH)include/scott
	$(INSTALL) $(LIB_DYNAMIC) $(LIB_STATIC) -t $(INSTALL_PATH)lib
	$(INSTALL) $(HEADERS:%=$(INCLUDE_DIR)%) -t $(INSTALL_PATH)include/scott
	ln -sf $(INSTALL_PATH)lib/$(LIB_DYNAMIC) /lib64/

.PHONY: uninstall
uninstall:
	rm -f $(INSTALL_PATH)lib/$(LIB_DYNAMIC)
	rm -f $(INSTALL_PATH)lib/$(LIB_STATIC)
	for file in $(HEADERS); do \
		rm -rf $(INSTALL_PATH)include/scott/$$file ; \
	done
	rm -d $(INSTALL_PATH)include/scott/

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR)
	rm -f $(LIB_DYNAMIC) $(LIB_STATIC)
