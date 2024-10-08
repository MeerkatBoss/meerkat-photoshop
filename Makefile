PROJECT	:= window_manager
VERSION := 0.0.1

SRCDIR	  := src
TESTDIR   := tests
LIBDIR	  := lib
INCDIR	  := include
EXPORTDIR := $(SRCDIR)/Export
COMMONDIR := $(SRCDIR)/Common

BUILDDIR:= build
OBJDIR 	:= $(BUILDDIR)/obj
BINDIR	:= $(BUILDDIR)/bin
EXPORTOBJDIR := $(OBJDIR)/Export
EXPORTBINDIR := $(BINDIR)/Export
COMMONOBJDIR := $(OBJDIR)/Common

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

CMACHINE:= # -mavx512f -march=native -mtune=native

CFLAGS:=-std=c++17 -fPIE -fPIC $(CMACHINE) $(CWARN) -DSFML_STATIC
INCFLAGS:= -I$(SRCDIR) -I$(INCDIR) -I$(COMMONDIR)
LFLAGS  := -lsfml-graphics -lsfml-window -lsfml-system -ldl

ifeq ($(BUILDTYPE), Release)
	CFLAGS:=-O2 -DNASSERT $(CFLAGS)
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

SOURCES := $(shell find -L $(SRCDIR) -type f -name "*.$(SRCEXT)" -not -path "$(EXPORTDIR)/*")
EXPORTS := $(shell find $(EXPORTDIR) -type f -name "*.$(SRCEXT)")
OBJECTS	:= $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))
COMMONOBJS := $(filter $(COMMONOBJDIR)/%,$(OBJECTS))
EXPORTOBJS := $(patsubst $(EXPORTDIR)/%,$(EXPORTOBJDIR)/%,$(EXPORTS:.$(SRCEXT)=.$(OBJEXT)))
EXPORTBINS := $(patsubst $(EXPORTDIR)/%,$(EXPORTBINDIR)/%,$(EXPORTS:.$(SRCEXT)=.so))
MAINSRC := src/Main.cpp
MAINOBJ := obj/Main.o

all: export $(BINDIR)/$(PROJECT)

export: $(EXPORTBINS)
	@$(foreach bin,$^,mkdir -p Plugins/$(basename $(notdir $(bin)))/;)
	@$(foreach bin,$^,cp -f -t Plugins/$(basename $(notdir $(bin)))/ $(bin);)

remake: cleaner all

init:
	@mkdir -p $(SRCDIR)
	@mkdir -p $(INCDIR)
	@mkdir -p $(LIBDIR)
	@mkdir -p $(OBJDIR)
	@mkdir -p $(BINDIR)
	@mkdir -p $(TESTDIR)

# Build source objects
$(OBJDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@echo Building $@
	@$(CTIDY) $(TIDYFLAGS) $<
	@mkdir -p $(dir $@)
	@$(CC) $(if $(filter $<,$(TRACED)),,-DNLOG_TRACE)\
		$(CFLAGS) $(INCFLAGS) -c $< -o $@

# Build plugin binaries
$(EXPORTBINDIR)/%.so: $(EXPORTOBJDIR)/%.$(OBJEXT) $(COMMONOBJS)
	@echo Building plugin $(basename $(notdir $@))
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -shared $^ -o $@

# Build project binary
$(BINDIR)/$(PROJECT): $(OBJECTS)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $^ $(LFLAGS) -o $(BINDIR)/$(PROJECT)

format:
	@$(foreach source, $(shell find $(SRCDIR) -type f,\
		$(CFORMAT) -i $(source);)

clean:
	@rm -rf $(OBJDIR)

cleaner: clean
	@rm -rf $(BINDIR)

run: $(BINDIR)/$(PROJECT)
	$(BINDIR)/$(PROJECT) $(ARGS)

.PHONY: all remake clean cleaner run format

