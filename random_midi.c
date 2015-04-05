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

typedef struct {
	int root_chord;	// "root" note of the bar
	int num_of_notes; 	// max 4
	int notes[4];		// midi numbers of the notes
	int durations[4];	// note durations in ?
} bar;

int parse_root (char * arg);
int parse_steps (char * arg);
int parse_steps_R (char * arg, int * steps_p, int * step_num_p);
int parse_bars (char * arg);
int rand_hop_gen ();
int rand_hop_0 ();
int get_oct (int note);
int populate_bar (int note, bar * stru);
void print_struct (bar * stru);
int check_note (int note);

char names[12][5] = {"DO","DO+","RE","RE+","MI","FA","FA+","SOL","SOL+","LA","LA+","SI"};
int notes_all[128];		// array with ALL midi notes
int root;				// root note
int steps[12];			// array with steps counting from root
int step_num;			// number of steps - length of steps array
int notes_root[128];	// array with SCALE notes
int notes_root_num;		// number of total scale notes
int bar_num;			// bars number - length of song
int octaves_num = 9;	// span in octaves
int cycle_length = 8;	// cycle length
int start_octave = 4;	// starting octave
int limit_oct_up = 3;
int limit_oct_down = 2;

int seed1 = 666;



int main(int argc, char **argv) {
	
	srand(seed1);
	
	// variable i for general loop use.
	int i;
	
	// arg[0] = program name
	// arg[1] = --root
	// arg[2] = root
	// arg[3] = --steps-hex
	// arg[4] = stepshex
	// arg[5] = --bars
	// arg[6] = bars
	// arg[7] = --seed
	// arg[8] = seed
	
	char usage[] = 	"--root <root_chord> --steps-hex <stepshex> "
					"--bars <number_of_bars>\n";
	
	// check argc number must be 7
	if (argc!=7) {
		printf(	"usage:\n%s "
				"%s",argv[0],usage);
		exit(1);
	}
	
	// error check arguments typing
	// array with compare results
	int strcmp_array[3] = 	{
							strcmp(argv[1],"--root"),
							strcmp(argv[3],"--steps-hex"),
							strcmp(argv[5],"--bars")
							};
							
	// check if any of the compares is not zero	
	if (strcmp_array[0]!=0 || strcmp_array[1]!=0 || strcmp_array[2]!=0) 
	{
		printf(	"usage:\n%s "
				"%s",argv[0],usage);
		exit(1);
	}
	
	
	// PARSE ROOT NOTE START ===========================================
	// =================================================================
	// get arg[2] root note
	//
	root = parse_root(argv[2]);
	//
	// PARSE ROOT NOTE END =============================================
	// =================================================================
	
	
	// PARSE STEPS START ===============================================
	// =================================================================
	// get arg[4] hex number
	//
	// WARNING !!!!
	// the following function
	// sets external vars
	// step_num & steps[]
	// by reference passing!
	parse_steps_R(argv[4],steps,&step_num);
	//
	// PARSE STEPS END =================================================
	// =================================================================
	
	
	// PARSE NUMBER OF BARS START ======================================
	// =================================================================
	//
	bar_num = parse_bars(argv[6]);
	//
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
	
	// memory to hold bar structs
	bar * bars_struct;
	bars_struct = (bar *) malloc (sizeof(bar)*bar_num);
	
	
	
	// find starting note
	// int start_note = notes_root[start_octave*step_num];
	
	int curr_bar_ni;
	int curr_bar_n;
	int prev_bar_o;
	int prev_bar_ni;
	
	// for each bar set root_chord note
	// according to position in cycle length
	for (i=0; i<bar_num; i++) {
		
		// CASES START =================================================
		
		// case 1: First bar of the cycle
		// must be the root note.
		// i%cycle_length == 0
		if (i%cycle_length == 0) {
			
			
			// first bar of SONG
			if (i==0) {
				
				curr_bar_ni = start_octave*step_num;
				curr_bar_n=notes_root[curr_bar_ni];
			}
			
			// generic first bar of cycle
			else {
				// find position of prev bar note octave
				prev_bar_o = get_oct(notes_root[prev_bar_ni]);
				curr_bar_ni = prev_bar_o*step_num;
				curr_bar_n=notes_root[curr_bar_ni];
				}
			printf("---------------------------------------\n");	
			}
		
		
		// case 3: before last bar
		// must not be the last step of the sequence
		// aka random
		// i%cycle_length == cycle_length - 2
		else if (i%cycle_length == cycle_length - 2) {
			
			// request generic random hop
			// but CHECK if the resulting
			// note is the one before root.
			// HOW?
			// if it is request a new random.
			
			curr_bar_ni = prev_bar_ni+rand_hop_gen();
			curr_bar_n = notes_root[curr_bar_ni];
		}
		
		// case 4: last bar
		// must be the last step of the sequence
		// i%cycle_length == cycle_length - 1
		else if (i%cycle_length == cycle_length - 1) {
			
			prev_bar_o = get_oct(notes_root[prev_bar_ni]);
			curr_bar_ni = (prev_bar_o * step_num)+step_num-1;
			curr_bar_n = notes_root[curr_bar_ni];
		}
		
		// case 2: generic bar inside cycle
		// must not be the same as the previous
		// ie bar_struct[i-1]
		else {
			
			curr_bar_ni = prev_bar_ni+rand_hop_gen();
			curr_bar_n = notes_root[curr_bar_ni];
		}
		
		// CASES END ===================================================
		
		// from now on we have a note!
		
		// print some info
		printf ("BAR %2d - %d", i, i%cycle_length);
		printf (" Note number %2d", (curr_bar_n));
		printf (" (%4s)", names[curr_bar_n%12]);
		
		printf (" Note index %2d", (curr_bar_ni));
		printf (" Octave %2d\n",get_oct(curr_bar_n));
		
		// save cur to previous
		prev_bar_ni = curr_bar_ni;
		
		// TODO
		// feed note and struct pointer to function that
		// will write note to struct
		// will generate other notes
		// will generate durations
		// will return zero if everything ok
		
		populate_bar(curr_bar_n, &bars_struct[i]);
		
		print_struct(&bars_struct[i]);
		
		// correct out of range
		// by checking if out of bounds
		// if so, change previous note value
		// and correct next bar
		// 1. case down
		if (prev_bar_ni < step_num*limit_oct_down) {
			prev_bar_ni = prev_bar_ni + 1*step_num;
			printf ("CHANGE OCTAVE - out of range DOWN ----\n");
		}
		// 2. case UP
		else if (prev_bar_ni > step_num*(octaves_num-limit_oct_up)) {
			prev_bar_ni = prev_bar_ni - 1*step_num;
			printf ("CHANGE OCTAVE - out of range UP ----\n");
		}
		
	}
	
	free (bars_struct);
	return 0;
}


int parse_root (char * arg) {
	char * strtol_pointer;
	long int strtol_out = strtol(arg, &strtol_pointer, 10);
	
	// case proper number but not zero
	// (strtol returns zero if it does
	// not find a number)
	if (strtol_out>=1 && strtol_out<=11) {
		root = strtol_out;
	}
	// case zero
	else if (strtol_out==0 && arg[0]=='0') {
		root = 0;
	}
	// case wrong
	else {
		printf ("argument 1 (root note) not between 0 and 11. "
		"Exiting...\n\n");
		exit(1);
	}
	printf ("root note: %d\n",root);
	return root;
}

int parse_steps (char * arg) {
	step_num = strlen(arg);
	printf ("steps count: %d\n",step_num);
	
	// minimum steps 4, max 12
	if (step_num<=12 && step_num>=4) {
		
		int i;
		for (i=0; i<step_num; i++) {
			char step = arg[i];
			
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
	
return 0;

}

int parse_steps_R (char * arg, int * steps_p, int * step_num_p) {
	*step_num_p = strlen(arg);
	printf ("steps count: %d\n",*step_num_p);
	
	// minimum steps 4, max 12
	if (*step_num_p<=12 && *step_num_p>=4) {
		
		int i;
		for (i=0; i<*step_num_p; i++) {
			char step = arg[i];
			
			// CASE 1
			// step is number
			if (step>='0' && step <='7') {
				char step_string[2] = {step,'\0'};				
				steps_p[i]=atoi(step_string);
			}
			
			// CASE 2
			// step is letter a to d
			else if (step>='a' && step<='d') {				
				steps_p[i]=step-89;
			}
			else {
				printf("step #%d is wrong. Exiting...\n\n",i);
				exit(1);
			}
			printf ("step %d = %u\n",i,steps_p[i]);		
		}
	}
	
	else {
		printf ("steps count not between 4 and 12. Exiting...\n\n");
		exit(1);
	}
	
return 0;

}

int parse_bars (char * arg) {
	
	int bn;
	if (arg) {
		bn = strtol(arg, (char**)0, 10);
		if (bn<=0 || bn > 256) {
			printf ("bars number <0 or >256 or not valid. Exiting...\n\n");
			exit(1);
		}
		printf ("Number of bars: %d\n", bn);
	}
	else {
		printf ("Missing argument 3, number of bars. Exiting...\n\n");
		exit(1);
	}
	
	return bn;
	
}

int rand_hop_0 () {
	// hop after first bar of each cycle
	// always goes up.
	// 0<hop0<step_num
	// calling function must check if
	// note after hoping is out of range
	
	return 0;
}

int rand_hop_gen () {
	// hop at midway bars
	// ie generic hop
	// -step_num<hopg<stepnum
	// calling function must check RANGE
	int rand_hop; 
	
	// rand_hop must NOT be zero
	do {
		rand_hop = rand()%(2*step_num) - step_num;
	} while (rand_hop==0);
	
	// printf ("  --Hop %d\n",rand_hop);
	return rand_hop;
}

int get_oct (int note) {
	return note/12;
}

int check_note (int note) {
	// if the note is in the steps
	// and notes_root it returns 0
	// else returns 1
	int i = 0;
	// printf ("got note %d\n",note);
	note = note%12;
	// printf ("trs note %d\n",note);
	for (i=0; i<step_num; i++) {
		if (notes_root[i]==note) {
			return i;
		}
	}
	return -1;
}

int populate_bar (int note, bar * stru) {
	// feed note and struct pointer to function that
	// will write note to struct
	// will generate other notes
	// will generate durations
	// will return zero if everything ok
	int notes_num = 4;
    int temp_notes[notes_num];
	int shuffled_notes[notes_num];
	
	// printf ("Note passed: %2d \n",note);
	
	// root is root
	temp_notes[0] = note;
	stru->root_chord = temp_notes[0];
	// third - check if in scale
	// major+4 or minor+5
	// if not in scale choose one
	// of the two randomly
	int major = note + 4;
	int minor = note + 3;
	int perf_fifth = note + 7;
	int third;
	int fifth;
	
	// set third according to 
	// whether major or minor
	// is included in scale
	if (check_note(major)>=0) {
		third = major;
		//printf("check: %d found major - ",check_note(major));
	}
	else if (check_note(minor)>=0){
		third = minor;
		//printf("check: %d found minor - ",check_note(minor));
	}
	else {
		// else pick one of the two
		third = note + ((rand()%2)+5);
		//printf("found rando - ");
	}
	temp_notes[1] = third;
	
	// set fifth
	temp_notes[2] = perf_fifth;
	
	// set random forth note
	// loop until finding a note that belongs to the scale
	do 	{
	temp_notes[3] = note + steps[rand()%step_num];
	} while (check_note(temp_notes[3]) <= 0 );
	
	// TODO
	// SHUFFLE NOTES
    // choose one of the four and put it in first place
    //

	// print them
	// put them in structure
	int i = 0;
	for (i=0; i<notes_num; i++) {
		printf("%-5s(%2d) ",names[ temp_notes[i]%12 ], temp_notes[i]);
		stru->notes[i] = temp_notes[i];
	}
	printf("\n");
	
	return 0;
	
}

void print_struct (bar * stru) {
	printf ("Root:%3d n2:%3d n3:%3d n4%3d\n", 	stru->root_chord,
												stru->notes[1],
												stru->notes[2],
												stru->notes[3]);	
}
