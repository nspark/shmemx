#
# Basic Chapel Makefile
#

.SECONDARY	= $(EXES)

SRCDIR		= src
INCDIR		= include
TSTDIR		= test
OBJDIR		= obj
LIBDIR		= lib
BINDIR		= bin

SRCS		= $(wildcard $(TSTDIR)/*.c)
EXES		= $(patsubst $(TSTDIR)/%.c,$(BINDIR)/%,$(SRCS))
TSTS		= $(patsubst $(TSTDIR)/%.c,$(BINDIR)/%.tt,$(SRCS))

ENV		= OSHMEM_CC=gcc-4.9
CC		= oshcc
CFLAGS		= -O3 -std=c11 -Wall -Wextra -pedantic -Werror
CPPFLAGS	= -D_POSIX_VERSION=200809L -D_XOPEN_VERSION=700 -I$(INCDIR)

LAUNCHCMD	= oshrun
LAUNCHOPT	= --mca sshmem mmap -n 16

all:		$(EXES)

test:		$(TSTS)

clean:
		rm -rf $(BINDIR)/*

CLEAN:
		rm -rf $(BINDIR) $(LIBDIR) $(OBJDIR)

$(BINDIR)/%	: $(TSTDIR)/%.c $(BINDIR)
		$(ENV) $(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $<

$(BINDIR)/%.tt	: $(BINDIR)/%
		$(LAUNCHCMD) $(LAUNCHOPT) $<

$(BINDIR)	:
		mkdir -p $(BINDIR)

$(LIBDIR)	:
		mkdir -p $(LIBDIR)

$(OBJDIR)	:
		mkdir -p $(OBJDIR)
