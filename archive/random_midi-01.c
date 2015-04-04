/*
 * main.c
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
#include <stdlib.h>
#include <string.h>

int notes_all[128];
int root;
int steps[12];
int step_num;
int notes_root[128];
int notes_root_num;
int bar_num;

int main(int argc, char **argv) {
	
	// variable i for general loop use.
	int i;
	
	// help a little with args
	// printf ("Argc: %d\n", argc);
	
	// args debug
	/*
	int a;
	for (a=0; a<argc; a++) {
		printf ("arg %d: %s\n", a,argv[a]);
	}
	*/
	
	
	// PARSE ROOT NOTE START ===========================================
	// =================================================================
	// get arg[1] root note
	char * strtol_pointer;
	long int strtol_out = strtol(argv[1], &strtol_pointer, 10);
	
	// case proper number but not zero
	// (strtol returns zero if it does
	// not find a number)
	if (strtol_out>=1 && strtol_out<=11) {
		root = strtol_out;
	}
	// case zero
	else if (strtol_out==0 && argv[1][0]=='0') {
		root = 0;
	}
	// case wrong
	else {
		printf ("argument 1 (root note) not between 0 and 11. "
		"Exiting...\n\n");
		exit(1);
	}
	printf ("root note: %d\n",root);
	// PARSE ROOT NOTE END =============================================
	// =================================================================
	
	
	// PARSE STEPS START ===============================================
	// =================================================================
	// get arg[2] hex number
	step_num = strlen(argv[2]);
	printf ("steps count: %d\n",step_num);
	
	// minimum steps 4, max 12
	if (step_num<=12 && step_num>=4) {
		// decode string
		// printf ("size of argv2[i] %lu\n", sizeof(argv[2][i]));
		
		for (i=0; i<step_num; i++) {
			//printf ("\n");
			//printf ("step %d: %u\n",i,argv[2][i]);
			char step = argv[2][i];
			
			// CASE 1
			// step is number
			if (step>='0' && step <='7') {
				char step_string[2] = {step,'\0'};				
				steps[i]=atoi(step_string);
			}
			
			// CASE 2
			// step is letter a to d
			else if (step>='a' && step<='d') {				
				steps[i]=step-89;
			}
			else {
				printf("step #%d is wrong. Exiting...\n\n",i);
				exit(1);
			}
			printf ("step %d = %u\n",i,steps[i]);		
		}
	}
	
	else {
		printf ("steps count not between 4 and 12. Exiting...\n\n");
		exit(1);
	}
	// PARSE STEPS END =================================================
	// =================================================================
	
	
	// PARSE NUMBER OF BARS START ======================================
	// =================================================================
	if (argv[3]) {
		bar_num = strtol(argv[3], (char**)0, 10);
		// printf ("bar_num: %d\n",bar_num);
		if (bar_num<=0 || bar_num > 256) {
			printf ("bars number <0 or >256 or not valid. Exiting...\n\n");
			exit(1);
		}
		printf ("Number of bars: %d\n", bar_num);
	}
	else {
		printf ("Missing argument 3, number of bars. Exiting...\n\n");
		exit(1);
	}
	
	// PARSE NUMBER OF BARS END ========================================
	// =================================================================
	
	
	// NOTE AND SCALE GENERATION =======================================
	// =================================================================	
	// generate notes 0-127
	for (i=0; i<128; i++) {
		notes_all[i]=i;
	}
	
	// generates notes of scale
	// for 8 octaves
	int octave;
	int step;
	int note=0;
	notes_root_num = step_num * 8;
	
	for (octave=0; octave<8; octave++) {
		
		printf ("\nOCTAVE    #%d\n", octave);
		printf (  "------------\n");
		
		printf ("[");
		for (step=0; step<step_num; step++) {
			notes_root[note] = root + octave*12 + steps[step];
			printf ("%3d ",notes_root[note]);
			note++;
			
		}
		printf ("]\n");
	}
	// NOTE AND SCALE GENERATION END ===================================
	// =================================================================
	
	
	// generate bars
	
	// for each bar generate chords (tonal note + others)
	
	// for each chord generate notes
	
	// arrays or structs?
	
	return 0;
}

