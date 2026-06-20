# Makefile of LUNAR

# Copyright (C) 1988,1989,1991,1992,2001 Fung F. Lee and Ricky Yeung

# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.


# CFLAGS= -O
CC = gcc
all: lunar

clean:
	rm -f *.o lunar

PREFIX = /usr
BINDIR  = $(PREFIX)/bin
DATADIR = $(PREFIX)/share
MANDIR  = $(PREFIX)/share/man

install: lunar lunar.1
	install -m755 -d $(DESTDIR)$(BINDIR)
	install -m755 -p lunar $(DESTDIR)$(BINDIR)/
	install -m755 -d $(DESTDIR)$(DATADIR)/lunar
	install -m644 -p lunar.bitmap $(DESTDIR)$(DATADIR)/lunar/
	install -m755 -d $(DESTDIR)$(MANDIR)/man1
	install -m644 -p lunar.1 $(DESTDIR)$(MANDIR)/man1/

.PHONY: install clean
