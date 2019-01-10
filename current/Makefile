#############################################################################################
#                                                                                           #
#     This is Makefile.                                                                     #
#     Copyleft, 2013-2018, <feedback@7cats.biz>, Babushkin Alexander.                       #
#                                                                                           #
#     This program is free software; you can redistribute it and/or modify it under the     #
#  terms of the GNU General Public License as published by the Free Software Foundation;    #
#  either version 3 of the License, or (at your option) any later version.                  #
#                                                                                           #
#     This program is distributed in the hope that it will be useful, but WITHOUT ANY       #
#  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A          #
#  PARTICULAR PURPOSE.  See the GNU General Public License for more details.                #
#                                                                                           #
#############################################################################################
#                                                                                           #
#                                                                                           #
#                                                                                           #
# ------------------------+---------------------------------------------------------------- #
#                         |                                                                 #
#     Command             |      Description                                                #
#                         |                                                                 #
# ------------------------+---------------------------------------------------------------- #
#                         |                                                                 #
#     $>make              |                                                                 #
#     or                  |                                                                 #
#     $>make all          |                                                                 #
#                         |                                                                 #
# ------------------------+---------------------------------------------------------------- #
#                         |                                                                 #
#     $>make debug        |                                                                 #
#                         |                                                                 #
# ------------------------+---------------------------------------------------------------- #
#                         |                                                                 #
#     $>make checksum     |                                                                 #
#     $>make checking     |                                                                 #
#                         |                                                                 #
# ------------------------+---------------------------------------------------------------- #
#                         |                                                                 #
#     $>make help         |                                                                 #
#     $>make license      |                                                                 #
#     $>make warranty     |                                                                 #
#                         |                                                                 #
# ------------------------+---------------------------------------------------------------- #
#                         |                                                                 #
#     $>make clean        |                                                                 #
#                         |                                                                 #
# ------------------------+---------------------------------------------------------------- #
#                         |                                                                 #
#     $>make cleanall     |                                                                 #
#                         |                                                                 #
# ------------------------+---------------------------------------------------------------- #
#                         |                                                                 #
#     $>make install      |                                                                 #
#                         |                                                                 #
# ------------------------+---------------------------------------------------------------- #
#                         |                                                                 #
#     $>make uninstall    |                                                                 #
#                         |                                                                 #
# ------------------------+---------------------------------------------------------------- #
#                                                                                           #
#############################################################################################

VERSION := 0.1
PROJECT := TransferFiles
TARGET1 := transferfiles
TARGETS := $(TARGET1)
OBJECTS1:= main.o clock.o options.o network.o

ARCHIVE := $(PROJECT)-$(VERSION)
INSTALL := /usr/local/bin/$(PROJECT)

CHKSUM  := sha256sum
CHKFILE := checksum.txt

CPUTYPE := $(shell uname --machine)
OSTYPE  := $(shell uname --kernel-name)

CFLAGS  := -std=c99 -Os

.PHONY: all debug dist checksum checking list help license warranty clean cleanall install uninstall

#############################################################################################
#                  Make all the top-level targets the makefile knows about.                 #
#############################################################################################

$(TARGET1): $(OBJECTS1)
	@echo Linking ...
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c network.h options.h clock.h base.h
	@echo Compiling $<
	$(CC) $(CFLAGS) -c -o $@ $<

clock.o: clock.c clock.h base.h
	@echo Compiling $<
	$(CC) $(CFLAGS) -c -o $@ $<

options.o: options.c options.h base.h
	@echo Compiling $<
	$(CC) $(CFLAGS) -c -o $@ $<

network.o: network.c network.h base.h
	@echo Compiling $<
	$(CC) $(CFLAGS) -c -o $@ $<

#############################################################################################
#                  Make all the top-level targets the makefile knows about.                 #
#############################################################################################

all:    $(TARGETS)

#############################################################################################
#                                                                                           #
#############################################################################################

debug:  CFLAGS := -std=c99 -gstabs -Og -D APP_DEBUG=stabs
debug:  all

#############################################################################################
#                    Create a distribution file of the source files.                        #
#############################################################################################

dist:   checksum $(ARCHIVE)src.tgz

$(ARCHIVE)src.tgz: $(ARCHIVE)src.tar.gz
	@echo
	@echo Renaming...
	cd .. && mv --verbose --force $< $@

$(ARCHIVE)src.tar.gz: $(ARCHIVE)src.tar
	@echo
	@echo Creating gzip-archive $@ ...
	cd .. && gzip --verbose --force --best $<

$(ARCHIVE)src.tar:
	@echo
	@echo Creating tar-archive $@ ...
	cd .. && tar --verbose --create --recursion --force-local --file $@ *

#############################################################################################
#                       Create a checksum file of the source files.                         #
#############################################################################################

checksum: cleanall $(CHKFILE)

$(CHKFILE):
	@echo
	@echo Creating $@ ...
	$(CHKSUM) --text * > $@
	cat $@

#############################################################################################
#                          Testing a checksum of the source files.                          #
#############################################################################################

checking:
	$(CHKSUM) --check $(CHKFILE)

#############################################################################################
#                                                                                           #
#############################################################################################

list:
	-ls --format=long --color=always --almost-all --recursive --group-directories-first --sort=extension --time-style=long-iso --file-type

#############################################################################################
#                                                                                           #
#############################################################################################

help:
	-cat README.txt

license:
	-cat COPYING

warranty:
	-cat COPYING

#############################################################################################
#                Delete all files that are normally created by running make.                #
#############################################################################################

clean:
	@echo Cleaning...
	-rm --verbose --force *~ .fuse* *.o *.code *.lst

cleanall: clean
	-rm --verbose --force $(TARGETS) $(CHKFILE) *.log *.dat

#############################################################################################
#    Copy the executable file into a directory that users typically search for commands.    #
#############################################################################################

install:
	@echo Installing...
	-mkdir --verbose -p $(INSTALL)
	-install --verbose --target-directory=$(INSTALL) $(TARGETS)

#############################################################################################
#                                                                                           #
#############################################################################################

uninstall:
	@echo Uninstalling...
	-rm --verbose --force --recursive --dir $(INSTALL)

#############################################################################################
#                                                                                 - EOF -   #
#############################################################################################
