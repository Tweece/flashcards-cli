#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define QUESTION 0
#define ANSWER 1

unsigned int randomVariable;
char* readfile(char*);
char getonechar();
int countquestions(char*);
int countuntilnext(char*);
unsigned int randomize();

int main() {
	time_t t;
	srand((unsigned) time(&t));
	randomVariable = (unsigned) time(&t);
	char filename[21] = {0}; 
	//fgets(filename, 10, stdin);
	printf("Open file: ");
	scanf("%s", filename); getchar(); // still using scanf, will switch to fgets once I figure it out
	char *mainfile = readfile(filename); 
	char *mainfileStart = mainfile; // creating a copy for easier freeing
	printf("\"%x\"\n", mainfile);
	///printf("%s", mainfile); 
	int i = 0, numberofquestions = countquestions(mainfile), currentquestion = 0, currentanswer = 0, next = 0;
	char* flashcards[numberofquestions][2]; // creating the main array
	while(*mainfile && *(mainfile+1)) { // checking + 1 for null byte cause the starting sequence is "|$" for question and "|#" for answer
		///printf("Current character = \'%c\' and another one is \'%c\'\n", *mainfile, *(mainfile+1));
		if((*mainfile == '|') && (*(mainfile+1) == '$')) { // checking for the start question sequence
			mainfile += 2; // moving to the start of the question
			next = countuntilnext(mainfile); // finding the length of the question
			///printf("Next: \"%d\"\n", next);
			while(*(mainfile + next - 1) == '\n' || *(mainfile + next - 1) == ' ') next--; //cutting the selected part if space or newline is at the end
			while(*(mainfile) == ' ' || *(mainfile) == '\n') {mainfile++; next--;} //moving forward if starts with space or newline
			flashcards[currentquestion][QUESTION] = malloc((next + 1) * sizeof(char)); // allocating memory
			for(char* j = mainfile + next; mainfile < j;++flashcards[currentquestion][QUESTION], ++mainfile) {*flashcards[currentquestion][QUESTION] = *mainfile; }
			*flashcards[currentquestion][QUESTION] = '\0'; // closing the "string"
			flashcards[currentquestion][QUESTION] -= next; // setting this question's pointer back to the start of it
			printf("\'%s\'", flashcards[currentquestion][QUESTION]);
			currentquestion++; mainfile--; // going back one character for safety
		}
		if((*mainfile == '|') && (*(mainfile+1) == '#')) {
			mainfile += 2; // doing literally the same stuff but with answer
			next = countuntilnext(mainfile);
			///printf("Next: \"%d\"\n", next);
			while(*(mainfile + next - 1) == '\n' || *(mainfile + next - 1) == ' ') next--; //cutting the selected part if space or newline is at the end
			while(*(mainfile) == ' ' || *(mainfile) == '\n') {mainfile++; next--;} //moving forward if starts with space or newline
			flashcards[currentanswer][ANSWER] = malloc((next + 1) * sizeof(char));
			for(char* j = mainfile + next; mainfile < j;++flashcards[currentanswer][ANSWER], ++mainfile) {*flashcards[currentanswer][ANSWER] = *mainfile; }
			*flashcards[currentanswer][ANSWER] = '\0';
			flashcards[currentanswer][ANSWER] -= next;
			printf("\'%s\'", flashcards[currentanswer][ANSWER]);
			currentanswer++; mainfile--;
		}
		printf("\"%x\"\n", mainfile);
		mainfile++;
	}
	getchar();
	free(mainfileStart); // freeing the file cause it's no longer useful
	///for(i = 0; i < numberofquestions; i++) { printf("Flashcard number \"%d\"\n\tQuestion: %s\n\tAnswer: %s\n\n", i, flashcards[i][QUESTION], flashcards[i][ANSWER]); }
	char option;
	for(;;) {
		printf("Flashcards\nChoose one of the options below:\n1. Random\n2. Test mode\n3. Display all\nQ. close program\n"); option = getonechar();
		if(option == '1') {
			char whichone = 2; // used for some 
			printf("Choose one\nQ - Questions\nA - Answers\n"); option = getonechar();
			if(option == 'q' || option == 'Q') {option = 'Q'; whichone = 0;}
			else if(option == 'a' || option == 'A') {option = 'A'; whichone = 1;}
			if(whichone == 2) {
				printf("Wrong input\n");
			} else {
				unsigned int randnumber;
				option = ' ';
				while(option != 'q' && option != 'Q') {
					////
					randnumber = randomize()%numberofquestions;
					////
					printf("%c) %s", 81 - (whichone * 16), flashcards[randnumber][whichone]); option = getonechar();
					if(option == 'q' || option == 'Q') break; 
					printf("%c) %s\n", 65 + (whichone * 16), flashcards[randnumber][(whichone == 0)]); option = getonechar(); // the (whichone == 0) just changes 1 to 0 and 0 to 1
				} option = ' '; // resseting user's input
			}
		}
		if(option == '2') {
			unsigned int numsTest;
			scanf("%d", numsTest); getchar();
			if(numsTest > numberofquestions) printf("There can't be more answers that there are in the file");
			else {
				int numbers[numsTest];
				int randomizing = 0;
				char whichone = 2;
				//while(randomizing < numsTest) {for(int i = 0; i < numsTest;)}
				printf("Choose one\nQ - Questions\nA - Answers\n"); option = getonechar();
				if(option == 'q' || option == 'Q') {option = 'Q'; whichone = 0;}
				else if(option == 'a' || option == 'A') {option = 'A'; whichone = 1;}
				if(whichone == 2) {
					printf("Wrong input\n");
				} else {
					unsigned int randnumber;
					option = ' ';
					while(option != 'q' && option != 'Q') {
						randnumber = rand()%numberofquestions;
						printf("%c) %s", 81 - (whichone * 16), flashcards[randnumber][whichone]); option = getonechar();
						if(option == 'q' || option == 'Q') break; 
						for(int i = 1; i <= numsTest; i++ ) {
							randnumber = rand()%numberofquestions;
							printf("%d) %s\n", 65 + (whichone * 16), flashcards[randnumber][(whichone == 0)]); option = getonechar(); // the (whichone == 0) just changes 1 to 0 and 0 to 1
						}
					} option = ' '; // resseting user's input
				}
			}
		}
		if(option == '3') {
			for(i = 0; i < numberofquestions; i++) {
				printf("Flashcard no. \"%d\"\n\tQuestion: %s\n\tAnswer: %s\n\n", i, flashcards[i][QUESTION], flashcards[i][ANSWER]);
				}
		}
		if(option == 'q' || option == 'Q') {
			for(int ii = 0; ii < 2; ii++) for(i = 0; i < numberofquestions; i++) free(flashcards[i][ii]); // freeing memory
			return 0;
		}
	}
	return 0;
}

char* readfile(char* name) {
	FILE *fp;
	char *array;
	int ch; // current character
	size_t curpos = 0, length;
	fp = fopen(name, "r");
	if(!fp) return 0;
	fseek(fp, 0, SEEK_END); // going to the end of the file
	length = ftell(fp); // checking the length of the file
	array = malloc((length + 1) * sizeof(char));
	rewind(fp);
	while((ch = getc(fp)) != EOF) {
		array[curpos++] = ch; // pasting char by char
	}
	array[curpos] = '\0';
	fclose(fp);
	return array;
}

char getonechar() {
	char a = getchar();
	if(a != '\n') while (getchar() != '\n' && getchar()!=EOF);
	return a;
}

int countquestions(char* text) {
	int i = 0;
	while(*text && *(text+1)) { // null byte check
		if((*text == '|') && (*(text+1) == '$')) i++; // searching for questions
		text++;
	}
	return i;
}
int countuntilnext(char* text) {
	int i = 0;
	while(*text && *(text+1)) {
		if((*text == '|') && ((*(text+1) == '$') || (*(text+1) == '#'))) { // looking for next question / answer sequence
			 return i;
		}
        i++; text++;
	}
	return i;
}

unsigned int randomize() {
	randomVariable = randomVariable * 20743437;
	return randomVariable;
}