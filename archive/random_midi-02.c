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
int octaves_num = 9;

typedef struct {
	char tonal;			// "root" note of the bar
	char num_of_notes; 	// max 4
	char notes[4];		// midi numbers of the notes
	char durations[4];	// note durations in ?
} bar;

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
	
	// arg[0] = program name
	// arg[1] = --root
	// arg[2] = root
	// arg[3] = --steps-hex
	// arg[4] = stepshex
	// arg[5] = --bars
	// arg[6] = bars
	
	// error check arguments
	// array with compare results
	int strcmp_array[3] = 	{
							strcmp(argv[1],"--root"),
							strcmp(argv[3],"--steps-hex"),
							strcmp(argv[5],"--bars")
							};
	
	// check if any of the compares is not zero	
	if (strcmp_array[0]!=0 || strcmp_array[1]!=0 || strcmp_array[2]!=0) {
		printf(	"usage:\n%s "
				"--root <root_note> --steps-hex <stepshex> "
				"--bars <number_of_bars>\n",argv[0]);
		exit(1);
	}
	
	
	// PARSE ROOT NOTE START ===========================================
	// =================================================================
	// get arg[2] root note
	char * strtol_pointer;
	long int strtol_out = strtol(argv[2], &strtol_pointer, 10);
	
	// case proper number but not zero
	// (strtol returns zero if it does
	// not find a number)
	if (strtol_out>=1 && strtol_out<=11) {
		root = strtol_out;
	}
	// case zero
	else if (strtol_out==0 && argv[2][0]=='0') {
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
	// get arg[4] hex number
	step_num = strlen(argv[4]);
	printf ("steps count: %d\n",step_num);
	
	// minimum steps 4, max 12
	if (step_num<=12 && step_num>=4) {
		// decode string
		// printf ("size of argv4[i] %lu\n", sizeof(argv[2][i]));
		
		for (i=0; i<step_num; i++) {
			//printf ("\n");
			//printf ("step %d: %u\n",i,argv[2][i]);
			char step = argv[4][i];
			
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
	if (argv[6]) {
		bar_num = strtol(argv[6], (char**)0, 10);
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
	// for octaves_num octaves
	int octave;
	int step;
	int note=0;
	notes_root_num = step_num * octaves_num;
	
	for (octave=0; octave<octaves_num; octave++) {
		
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
	
	// print a new line
	printf("\n");
	
	// array to hold bar structs
	bar * bars_struct;
	bars_struct = (bar *) malloc (sizeof(bar)*bar_num);
	
	// cycle length
	int cycle_length = 8;
	
	// starting octave
	int start_octave = 2;
	
	// find starting note
	int start_note = notes_root[start_octave*step_num];
	
	// for each bar set tonal note
	// according to position in cycle length
	for (i=0; i<bar_num; i++) {
		
		// case 1: First bar of the cycle
		// must be the root note.
		// i%cycle_length == 0
		if (i%cycle_length == 0) {
			printf ("BAR %2d - first\n", i);
			printf ("--%d\n", (bars_struct[i].tonal=start_note));
		}
		
		// case 3: before last bar
		// must not be the last step of the sequence
		// aka random
		// i%cycle_length == cycle_length - 2
		else if (i%cycle_length == cycle_length - 2) {
			printf ("BAR %2d - before last\n", i);
		}
		
		// case 4: last bar
		// must be the last step of the sequence
		// i%cycle_length == cycle_length - 1
		else if (i%cycle_length == cycle_length - 1) {
			printf ("BAR %2d - last\n", i);
		}
		
		// case 2: generic bar inside cycle
		// must not be the same as the previous
		// ie bar_struct[i-1]
		else {
			printf ("BAR %2d - generic\n", i);
		}
		
	}
	
	// for each bar generate chords (tonal note + others)
	
	// for each chord generate notes
	
	// arrays or structs?
	
	
	free (bars_struct);
	return 0;
}

