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
	sudo mv $(BUILDDIR)/$(TARGET) $(INSTALLDIR)

uninstall: $(INSTALLDIR)/$(TARGET)
	sudo rm $(INSTALLDIR)/$(TARGET)

clean:
	rm -rf $(BUILDDIR)

.PHONY: uninstall install clean
