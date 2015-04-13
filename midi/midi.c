/*
 * swap.c
 * 
 * Copyright 2015 sotiris <sotiris@kant>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <stdio.h>
#include <stdint.h>
#include <byteswap.h>
#include "midi.h"



int main(int argc, char **argv)
{
	int a = 3;
	printf ("%#o - %#o\n", a, bswap_16(a));
	
	struct midi_header head1;
	head1.mthd[0]='M';
	head1.mthd[1]='T';
	head1.mthd[2]='h';
	head1.mthd[3]='d';
	
	head1.header_length = 6;
	head1.format = 0;
	head1.n_tracks = 1;
	head1.division = 96;
	
	printf ("%d\n", head1.n_tracks);
	
	return 0;
}

