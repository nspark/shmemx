#
# "Basic" Makefile
#

.PHONY		: all check
.SECONDARY	: $(EXES) $(OBJS) $(LIBS)

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
OBJS		= $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(LIBSRC))
LIBS		= $(LIBDIR)/lib$(LIBNAME).a
EXES		= $(patsubst $(TSTDIR)/%.c,$(BINDIR)/%,$(EXESRC))
TSTS		= $(patsubst $(TSTDIR)/%.c,$(BINDIR)/%.tt,$(EXESRC))

ifdef CC
ENV		= OSHMEM_CC=$(CC)
endif
OSHCC		= oshcc
CFLAGS		= -O3 -std=c11 -Wall -Wextra -pedantic -Werror
CPPFLAGS	= -D_POSIX_VERSION=200809L -D_XOPEN_VERSION=700 -I$(INCDIR)
LDFLAGS		= -L$(LIBDIR) -l$(LIBNAME)

ifdef NPES
_NPES		= $(NPES)
else
_NPES		= 16
endif

LAUNCHCMD	= oshrun
LAUNCHOPT	= -n $(_NPES)

all:		$(DIRS) $(EXES)

check:		$(TSTS)

clean:
		rm -rf $(BINDIR)/*

CLEAN:
		rm -rf $(DIRS)

$(OBJDIR)/%.o	: $(SRCDIR)/%.c
		$(ENV) $(OSHCC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

$(LIBDIR)/%.a	: $(OBJS)
		ar r $@ $<

$(BINDIR)/%	: $(TSTDIR)/%.c $(LIBDIR)/lib$(LIBNAME).a
		$(ENV) $(OSHCC) $(CPPFLAGS) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(BINDIR)/%.tt	: $(BINDIR)/%
		$(LAUNCHCMD) $(LAUNCHOPT) $<

$(BINDIR)	:
		mkdir -p $(BINDIR)

$(LIBDIR)	:
		mkdir -p $(LIBDIR)

$(OBJDIR)	:
		mkdir -p $(OBJDIR)
