CC = gcc
SRCDIR = src
BUILDDIR = build
TARGET = lltree
INSTALLDIR = /usr/local/bin

SOURCES = $(SRCDIR)/lltree.c $(SRCDIR)/fdet.c
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SOURCES))

$(BUILDDIR)/$(TARGET): $(OBJECTS) | $(BUILDDIR)
	$(CC) $(OBJECTS) -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) -c $< -o $@

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

install: $(BUILDDIR)/$(TARGET)
	sudo mv $(BUILDDIR)/$(TARGET) $(INSTALLDIR)

uninstall: $(INSTALLDIR)/$(TARGET)
	sudo rm $(INSTALLDIR)/$(TARGET)

clean:
	rm -rf $(BUILDDIR)

.PHONY: uninstall install clean
