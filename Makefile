CC = gcc
SRCDIR = src
BUILDDIR = build
TARGET = lltree
INSTALLDIR = /usr/local/bin

$(BUILDDIR)/$(TARGET): $(SRCDIR)/main.c | $(BUILDDIR)
	$(CC) $< -o $@

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

install: $(BUILDDIR)/$(TARGET)
	mv $(BUILDDIR)/$(TARGET) $(INSTALLDIR)

clean:
	rm -rf $(BUILDDIR)

.PHONY: install clean
