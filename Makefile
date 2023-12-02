PROJECT	:= window_manager
VERSION := 0.0.1

SRCDIR	:= src
TESTDIR := tests
LIBDIR	:= lib
INCDIR	:= include

BUILDDIR:= build
OBJDIR 	:= $(BUILDDIR)/obj
BINDIR	:= $(BUILDDIR)/bin

SRCEXT	:= cpp
HEADEXT	:= h
OBJEXT	:= o

BUILDTYPE?=Debug

CC:=clang++

CWARN:=-Wall -Wextra -Weffc++ -Wcast-align -Wcast-qual -Wchar-subscripts\
-Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral\
-Wformat-security -Wformat=2 -Winline -Wnon-virtual-dtor -Woverloaded-virtual\
-Wpacked -Wpointer-arith -Wredundant-decls -Wsign-promo -Wstrict-overflow=2\
-Wswitch-default -Wswitch-enum -Wundef -Wunreachable-code -Wunused\
-Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing\
-Wno-old-style-cast -Wno-varargs

CDEBUG:=-D _DEBUG -ggdb3 -fcheck-new -fsized-deallocation -fstack-protector\
-fstrict-overflow -fno-omit-frame-pointer\
-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,${strip \
}float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,${strip \
}null,return,returns-nonnull-attribute,shift,${strip \
}signed-integer-overflow,undefined,unreachable,vla-bound,vptr

CMACHINE:=-mavx512f -march=native -mtune=native

CFLAGS:=-std=c++17 -fPIE $(CMACHINE) $(CWARN)
INCFLAGS:= -I$(SRCDIR) -I$(INCDIR)
LFLAGS  := -Llib/ $(addprefix -l, $(LIBS))\
			-lsfml-graphics -lsfml-window -lsfml-system

ifeq ($(BUILDTYPE), Release)
	CFLAGS:=-O3 $(CFLAGS)
else
	CFLAGS:=-O0 $(CDEBUG) $(CFLAGS)
endif

CFORMAT  :=clang-format
FMTFLAGS :=--dry-run -Werror -ferror-limit=1

CTIDY     :=clang-tidy
TIDYCHECKS:=-*,clang-analyzer-*,-bugprone-*,-cppcoreguidelines-*
TIDYFLAGS :=--checks=$(TIDYCHECKS) --header-filter="" --quiet\
--warnings-as-errors=*

TIDYFLAGS +=$(foreach flag, $(CFLAGS),--extra-arg=$(flag))

HEADERS := $(shell find -L $(SRCDIR) -type f -name "*.$(HEADEXT)")
SOURCES := $(shell find -L $(SRCDIR) -type f -name "*.$(SRCEXT)")
OBJECTS	:= $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))
TESTS	:= $(shell find $(TESTDIR) -type f -name "*$(SRCEXT)")
TESTOBJS:= $(patsubst %,$(OBJDIR)/%,$(TESTS:.$(SRCEXT)=.$(OBJEXT)))
LIBS	:= $(patsubst lib%.a, %, $(shell find $(LIBDIR) -type f))

all: $(BINDIR)/$(PROJECT)

remake: cleaner all

init:
	@mkdir -p $(SRCDIR)
	@mkdir -p $(INCDIR)
	@mkdir -p $(LIBDIR)
	@mkdir -p $(OBJDIR)
	@mkdir -p $(BINDIR)

build_lib: $(OBJECTS)
	@mkdir -p dist/include
	@mkdir -p dist/lib
	@ar rcs dist/lib/lib$(PROJECT).a $^
	@find $(SRCDIR) -type f -name *.$(HEADEXT) -exec\
		bash -c 'cp -p --parents {} dist/include' \;
	@tar -czf dist/$(PROJECT)-$(VERSION)-linux-x86_64.tar.gz dist/*
	@rm -r dist/include
	@rm -r dist/lib

# Build test objects
$(OBJDIR)/$(TESTDIR)/%.$(OBJEXT): $(TESTDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCFLAGS) -I$(TESTDIR) -c $< -o $@

# Build source objects
$(OBJDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@echo Building $@ "... "
	@$(CTIDY) $(TIDYFLAGS) $<
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@

# Build project binary
$(BINDIR)/$(PROJECT): $(OBJECTS)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $^ $(LFLAGS) -o $(BINDIR)/$(PROJECT)

# Build test binary
$(BINDIR)/$(PROJECT)_tests: $(filter-out %/main.o,$(OBJECTS)) $(TESTOBJS)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $^ $(LFLAGS) -o $(BINDIR)/$(PROJECT)_tests

format:
	@$(foreach source, $(shell find $(SRCDIR) -type f,\
		$(CFORMAT) -i $(source);)

clean:
	@rm -rf $(OBJDIR)

cleaner: clean
	@rm -rf $(BINDIR)

run: $(BINDIR)/$(PROJECT)
	$(BINDIR)/$(PROJECT) $(ARGS)

test: $(BINDIR)/$(PROJECT)_tests
	 $(BINDIR)/$(PROJECT)_tests $(ARGS)

.PHONY: all remake clean cleaner

