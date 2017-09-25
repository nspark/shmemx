#
# "Basic" Makefile
#

.PHONY:		all check clean CLEAN
.SECONDARY:	$(EXES) $(OBJS) $(LIBS)

SRCDIR		= src
INCDIR		= include
TSTDIR		= test
OBJDIR		= obj
LIBDIR		= lib
BINDIR		= bin
DIRS		= $(BINDIR) $(LIBDIR) $(OBJDIR)

LIBNAME		= shmemx
LIBSRC		= $(wildcard $(SRCDIR)/*.c)
EXESRC		= $(wildcard $(TSTDIR)/*.c)
INCS		= $(wildcard $(INCDIR)/*.h)
OBJS		= $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(LIBSRC))
LIBS		= $(LIBDIR)/lib$(LIBNAME).a
EXES		= $(patsubst $(TSTDIR)/%.c,$(BINDIR)/%,$(EXESRC))
TSTS		= $(patsubst $(TSTDIR)/%.c,$(BINDIR)/%.tt,$(EXESRC))

CC		= oshcc
CFLAGS		= -O3 -std=c11 -Wall -Wextra -pedantic -Werror
CPPFLAGS	= -D_POSIX_VERSION=200809L -D_XOPEN_VERSION=700 -I$(INCDIR)
LDFLAGS		= -L$(LIBDIR) -l$(LIBNAME)

NPES		= 16

LAUNCHCMD	= oshrun
LAUNCHOPT	= -n $(NPES)

all:		$(DIRS) $(EXES)

check:		$(TSTS)

clean:
		rm -rf $(BINDIR)/* $(LIBDIR)/*.a $(OBJDIR)/*.o

CLEAN:
		rm -rf $(DIRS)

$(OBJDIR)/%.o	: $(SRCDIR)/%.c
		$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

$(LIBDIR)/%.a	: $(OBJS)
		ar r $@ $<

$(BINDIR)/%	: $(TSTDIR)/%.c $(LIBDIR)/lib$(LIBNAME).a $(INCS)
		$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(BINDIR)/%.tt	: $(BINDIR)/%
		$(LAUNCHCMD) $(LAUNCHOPT) $<

$(BINDIR)	:
		mkdir -p $(BINDIR)

$(LIBDIR)	:
		mkdir -p $(LIBDIR)

$(OBJDIR)	:
		mkdir -p $(OBJDIR)
