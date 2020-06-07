# GPL License - see http://opensource.org/licenses/gpl-license.php
# Copyright 2006 *nixCoders team - don't forget to credit us

NAME = ghf
VERSION = $(shell cat version.def)

CFLAGS = -Wall -pipe -march=i686 $(shell pkg-config --cflags libelf 2> /dev/null)
LDFLAGS = -lz -ldl -L. -lghf -lstdc++

# Release/Debug flags
ifdef RELEASE
	LDFLAGS += /usr/lib/libelf.a /usr/lib/libbfd.a /usr/lib/libopcodes.a /usr/lib/libcppunit.a
else
	CFLAGS += -ggdb
	LDFLAGS += -lelf -lbfd -lopcodes -lcppunit
endif

LIB_SRC = asm.c elf.c utils.c vfunction.cpp
LIB_OBJS = $(patsubst %.c, %.o, $(filter %.c, $(LIB_SRC))) $(patsubst %.cpp, %.o, $(filter %.cpp, $(LIB_SRC)))
LIB_HEADERS = $(NAME).h
LIB = lib$(NAME).a

TEST_SRC = ghf-test.cpp
TEST_OBJS = $(TEST_SRC:.cpp=.o)
TEST_HEADERS = ghf-test.h
TEST_PROG = $(TEST_OBJS:.o=)

DIST_FILES += $(LIB_SRC) $(LIB_HEADERS) $(TEST_SRC) $(TEST_HEADERS) Makefile version.def
DIST_FOLDER = $(NAME)
DIST_FILE = $(NAME)-$(VERSION).tar.gz

all: $(LIB) $(TEST_PROG)

$(LIB): $(LIB_OBJS)
	$(AR) r $(LIB) $(LIB_OBJS)
	ranlib $(LIB)

clean:
	rm -rf $(LIB_OBJS) $(LIB) $(TEST_OBJS) $(TEST_PROG) $(DIST_FILE) $(DIST_FOLDER)

%.o: %.c $(LIB_HEADERS) Makefile
	$(CC) $(CFLAGS) -c -o $@ $<
%.o: %.cpp $(LIB_HEADERS) Makefile
	$(CXX) $(CFLAGS) -c -o $@ $<
$(TEST_OBJS): $(TEST_SRC) $(TEST_HEADERS) $(LIB) Makefile
	$(CXX) $(CFLAGS) -c -o $@ $<


$(TEST_PROG): $(TEST_OBJS)
	$(CC) $(TEST_OBJS) -o $(TEST_PROG) $(LDFLAGS)
	@./$(TEST_PROG)

dist:
	make clean all RELEASE=1
	mkdir $(DIST_FOLDER)
	cp -R $(DIST_FILES) $(DIST_FOLDER)
	tar -czf $(DIST_FILE) $(DIST_FOLDER)
	rm -rf $(DIST_FOLDER)
