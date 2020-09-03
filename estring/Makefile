CC = g++
CFLAGS = -g -Wall -O

TOPDIR = $(PWD)
OBJDIR = $(TOPDIR)/obj
BINDIR = $(TOPDIR)/bin
TESTDIR = $(TOPDIR)/test
BIN = sso cow eager
INCDIR = ./include

OBJLINK = --std=c99

sso: $(TESTDIR)/test_cow_string.cpp
	@echo "cow"
	$(CC) $^ -o $(BINDIR)/$@ -I $(INCDIR)

cow: $(TESTDIR)/test_cow_string.cpp
	@echo "cow"
	$(CC) $^ -o $(BINDIR)/$@ -I $(INCDIR)

eager: $(TESTDIR)/test_eager_string.cpp
	@echo "eager"
	$(CC) $^ -o $(BINDIR)/$@ -I $(INCDIR)

clean:
	rm -f $(OBJDIR)/*.o $(BINDIR)/*
