# $Id: Makefile,v 1.4 2011/07/22 14:46:58 david Exp $
#
# Copyright (C) 2008-2011 INESC ID Lisboa.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
#
# $Log: Makefile,v $
# Revision 1.4  2011/07/22 14:46:58  david
# Minor cleanup.
#
#
EIGEN=/afs/l2f.inesc-id.pt/home/ferreira/face-recognition/eigen
EFJ=../efj
QTDIR=/usr/lib64/qt4
INC=-I. -I/usr/include/opencv -I$(EFJ) -I$(EIGEN) -I$(QTDIR)/include/QtCore -I$(QTDIR)/include/QtGui
CXXFLAGS = -fPIC -fpic -m64 -pipe -std=c++0x

all: libfacedetector.so

libfacedetector.so: FaceDetector.o
	g++ -shared -o $@ $^

%.o: %.cpp
	g++ $(INC) $(CXXFLAGS) -c $< -o $@

clean:
	-rm FaceDetector.o libfacedetector.so
