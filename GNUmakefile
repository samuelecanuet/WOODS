# $Id: GNUmakefile,v 1.12 2010/09/17 18:45:43 maire Exp $
# --------------------------------------------------------------
# GNUmakefile for examples module.  Gabriele Cosmo, 06/04/98.
# --------------------------------------------------------------

name := woods
G4TARGET := $(name)
G4EXLIB := true

ifndef G4INSTALL
  G4INSTALL = .
endif

#Maud
#set G4WORKDIR to "." : project directory
#else default G4WORKDIR is set to HOME/geant4_workdir
G4WORKDIR = .
#PATH = ${PATH}:G4WORKDIR/bin/Darwin-g++

.PHONY: all
all: lib bin

CPPFLAGS += $(shell root-config --cflags)
EXTRALIBS+= $(shell root-config --libs)

#### G4ANALYSIS_USE := true

include $(G4INSTALL)/config/architecture.gmk

include $(G4INSTALL)/config/binmake.gmk

# added to clean compilation and backup files
clean_all:	clean
		@echo "Cleaning files"
		@\rm -Rfv bin tmp *.bck */*.bck vrmlfiles/*.wrl
