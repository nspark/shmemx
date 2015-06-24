#
# Basic Chapel Makefile
#

.SECONDARY	= $(EXES)

SRCDIR		= src
INCDIR		= include
OBJDIR		= obj
LIBDIR		= lib
BINDIR		= bin

SRCS		= $(wildcard $(SRCDIR)/*.c)
EXES		= $(patsubst $(SRCDIR)/%.c,$(BINDIR)/%,$(SRCS))
TSTS		= $(patsubst $(SRCDIR)/%.c,$(BINDIR)/%.tst,$(SRCS))

ENV		= OSHMEM_CC=gcc-4.9
CC		= oshcc
CFLAGS		= -O3 -std=c11 -Wall -Wextra -pedantic -Werror
CPPFLAGS	= -D_POSIX_VERSION=200809L -D_XOPEN_VERSION=700 -I$(INCDIR)

LAUNCHCMD	= oshrun
LAUNCHOPT	= --mca sshmem mmap -n 4

all:		$(EXES)

test:		$(TSTS)

clean:
		rm -rf $(BINDIR)/*

CLEAN:
		rm -rf $(BINDIR) $(LIBDIR) $(OBJDIR)

$(BINDIR)/%	: $(SRCDIR)/%.c $(BINDIR)
		$(ENV) $(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $<

$(BINDIR)/%.tst	: $(BINDIR)/%
		$(LAUNCHCMD) $(LAUNCHOPT) $<

$(BINDIR)	:
		mkdir -p $(BINDIR)

$(LIBDIR)	:
		mkdir -p $(LIBDIR)

$(OBJDIR)	:
		mkdir -p $(OBJDIR)
