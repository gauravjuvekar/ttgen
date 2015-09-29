SRCDIR=src
OBJDIR=obj
_create_obj_dir:=$(shell mkdir -p $(OBJDIR))
DEPSDIR=deps
_create_deps_dir:=$(shell mkdir -p $(DEPSDIR))
TARGET=project

override CFLAGS+=-Wall -Wextra -std=c11 $(shell pkg-config --cflags gtk+-3.0)
override LDLIBS+=$(shell pkg-config --libs gtk+-3.0)
override DEPFLAGS+=-MMD -MP -MT $@ -MF $(DEPSDIR)/$*.Td

objects=window.o interval.o
DEPS=$(objects:%.o=$(DEPSDIR)/%.d)

$(TARGET) : $(addprefix $(OBJDIR)/, $(objects))
	$(CC) $(CFLAGS) -o $(TARGET) $^ $(LDFLAGS) $(LDLIBS)

$(SRCDIR)/window.c: $(SRCDIR)/gui.ui.hex.in

$(OBJDIR)/%.o : $(SRCDIR)/%.c $(DEPSDIR)/%.d
	$(CC) $(CFLAGS) $(CPPFLAGS) $(DEPFLAGS) -c -o $@ $<
	mv -f $(DEPSDIR)/$*.Td $(DEPSDIR)/$*.d

$(SRCDIR)/%.hex.in : $(SRCDIR)/%
	xxd -i < $< > $@

$(DEPSDIR)/%.d: ;

-include $(DEPS)

.PHONY : clean
clean :
	rm --force --recursive -- $(OBJDIR) $(TARGET) $(DEPSDIR)
	rm --force -- src/*.hex.in
