#    This file is part of FreeControl Package
#    Copyright (C) 2009, 2010  Quique Rodiguez <quesoruso74@yahoo.com>
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.

proyecto: freecontrol.o MemoryManager LadderExec
	gcc -o freecontrol freecontrol.o

freecontrol.o: freecontrol.c
	gcc -c freecontrol.c

MemoryManager.o: MemoryManager.c Common.c
	gcc -c MemoryManager.c

MemoryManager: MemoryManager.o 
	gcc -o MemoryManager MemoryManager.o

LadderExec.o: LadderExec.c Common.c
	gcc -c LadderExec.c

LadderExec: LadderExec.o
	gcc -o LadderExec LadderExec.o

clean:
	rm -f *.o freecontrol MemoryManager LadderExec *.log Data/MemoryMap.Communication


