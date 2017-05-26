/*
Brainf**k interpreter by Alper Said Soylu
There are decimal and ascii modes.
Cells are 8 bit.
*/
#include <stdio.h>
#include <conio.h>
#include <cstdlib>
#include <string.h>

class Bf {
public:
	Bf();
	int inputf(FILE* bf_sstream, size_t fsize);
	int execute(FILE* bf_istream,size_t tapesize);
	void print();
	void setmode(bool m);
	~Bf();
private:
	bool d;
	bool f;
	char* script;
	size_t script_end;
	size_t tape_end;
	unsigned char* tape;
};

int main(void)
{
	Bf bf;
	FILE* bf_sstream;
	FILE* bf_istream;
	char path[100];
	size_t fsize,tsize;
	bool d,c,f;
	while (1) {
		//add ascii and decimal modes
		puts("\nascii mode(0) or decimal mode(1)?:");
		scanf_s("%d",(int*)&d);
		bf.setmode(d);
		puts("\nread source code from file(0) or write on console(1)?:");
		scanf_s("%d", (int*)&c);
		puts("\nget input from stdin(0) or file(1)?:");
		scanf_s("%d", (int*)&f);
		puts("enter max script size:");
		if (!scanf_s("%d", &fsize)) {
			fprintf(stderr, "function: %s \t message: scanf_s error\n", __FUNCTION__);
			break;
		}
		puts("enter tape size:");
		if (!scanf_s("%d", &tsize)) {
			fprintf(stderr, "function: %s \t message: scanf_s error\n", __FUNCTION__);
			break;
		}
		if (f) {
			puts("enter bf input stream directory:");
			if (!scanf_s("%s", path, 100)) {
				fprintf(stderr, "function: %s \t message: scanf_s error\n", __FUNCTION__);
				break;
			}
			if (fopen_s(&bf_istream, path, "r")) {
				fprintf(stderr, "function: %s \t message: fopen_s error: directory: %s\n", __FUNCTION__, path);
				break;

			}
		}
		else {
			puts("bf input stream redirected to stdin");
			bf_istream = stdin;
		}
		if (c) {
			puts("enter your code:");
			bf_sstream = stdin;
		}
		else {
			puts("enter bf source code file directory:");
			if (!scanf_s("%s", path,100)) {
				fprintf(stderr, "function: %s \t message: scanf_s error\n", __FUNCTION__);
				break;
			}
			if (fopen_s(&bf_sstream, path, "r")) {
				fprintf(stderr, "function: %s \t message: fopen_s error: directory: %s\n", __FUNCTION__,path);
				break;
			}
		}
		
		if (bf.inputf(bf_sstream,fsize)) {
			break;
		}
		if(bf.execute(bf_istream,tsize)) {
			break;
		}
		bf.print();
		if (!c) {
			fclose(bf_sstream);
		}
		if (f) {
			fclose(bf_istream);
		}
	}
	perror("error message"); 
	_getch();
	return -1;
}

Bf::Bf() {
	script = NULL;
	tape = NULL;
	tape_end = 10000;
}

Bf::~Bf() {
	if(script)
	free(script);
	if(tape)
	free(tape);
}

void Bf::print()
{
	puts("\ntape:");
	/*if (d) {*/
		unsigned char* tptr;
		for (tptr = tape;tptr != &tape[tape_end] && (*tptr);printf("%d ", (*tptr)), tptr++);/*
	}
	else {
		printf("%s\n\n", tape);
	}*/
}
void Bf::setmode(bool  m) {
	d = m;
}

int Bf::inputf(FILE* bf_sstream, size_t fsize)
{
	int i;
	if (fsize == 0) {
		fprintf(stderr, "function: %s \t message: zero value assigned to argument fsize\n", __FUNCTION__);
		return -1;
	}
	if (bf_sstream == NULL) {
		fprintf(stderr, "function: %s \t message:  null pointer assigned to argument stream\n", __FUNCTION__);
		return -1;
	}
	if (script) {
		free(script);
		script = NULL;
	}
	size_t scriptsize = fsize + 1;
	script = (char*)malloc(scriptsize* sizeof(char));
	if (script == NULL) {
		fprintf(stderr, "function: %s \t message: couldnt allocate memory\n", __FUNCTION__);
		return -1;
	}
	if (!fscanf_s(bf_sstream,"%s",script, scriptsize)) {
		fprintf(stderr, "function: %s \t message: fscanf_s error \n", __FUNCTION__);
		return -1;
	}
	for (i = 0;script[i] != '\0';i++);
	script_end = i;
	return 0;
}

int Bf::execute(FILE* bf_istream, size_t tsize)
{
	tape_end = tsize;
	if (tsize == 0) {
		fprintf(stderr, "function: %s \t message: zero value assigned to argument tapesize\n", __FUNCTION__);
		return -1;
	}
	unsigned char *tptr, *tend;
	char *sptr;
	int bcount, i;
	if (script == NULL) {
		fprintf(stderr, "function: %s \t message: null string\n", __FUNCTION__);
		return -1;
	}
	if (tape) {
		free(tape);
		tape = NULL;
	}
	tape = (unsigned char*)malloc(sizeof(unsigned char)*(tsize + 1));
	if (tape == NULL) {
		fprintf(stderr, "function: %s \t message: couldnt allocate memory\n", __FUNCTION__);
		return -1;
	}

	for (i = 0;i<tsize;i++) {
		tape[i] = 0;
	};
	tape[i] = '\0';
	sptr = script;
	tptr = tape;
	tend = &tape[i];
	printf("execution:\n");
	for (;1;)
	{
		switch (*sptr) {
		case '!':
		case '\0':
			return 0;
		case ',':
			printf("\nenter input: \t");
			if (d) {
				if (!fscanf_s(bf_istream,"%d", (int*)tptr)) {
					fprintf(stderr, "function: %s, script offset: %d, message: scanf_s error\n", __FUNCTION__, sptr - script);
					return -1;
				}
			}
			else {
				*tptr = fgetc(bf_istream);
				_putch(*tptr);
				if (*tptr == EOF) {
					fprintf(stderr, "function: %s, script offset: %d, message: end of input file reached \n", __FUNCTION__, sptr - script);
					return -1;
				}
			}
			sptr++;
			break;
		case '.':
			if (d) {
				printf("%d", *tptr);
			}
			else {
				_putch(*tptr);
			}
			sptr++;
			break;
		case '>':
			if (tptr == tend){
				tptr = tape;
				fprintf(stderr, "function: %s, script offset: %d, message: tape access violation\n", __FUNCTION__,sptr-script);
				return -1;
			}
			else{
				tptr++;
			}
			sptr++;
			break;
		case '<':
			if (tptr == tape) {
				tptr = tend;
				fprintf(stderr, "function: %s, script offset: %d, message: tape access violation\n", __FUNCTION__, sptr-script);
				return -1;
			}
			else {
				tptr--;
			}
			sptr++;
			break;
		case '+':
			if (*tptr == UCHAR_MAX) {
				*tptr = 0;
			}
			else {
				(*tptr)++;
			}
			sptr++;
			break;
		case '-':
			if (*tptr == 0) {
				*tptr = UCHAR_MAX;
			}
			else {
				(*tptr)--;
			}
			sptr++;
			break;
		case '[':
			if (*tptr == 0) {
				bcount = 1;
				while (bcount) {
					sptr++;
					if (*sptr == '\0') {
						fprintf(stderr, "function: %s, script offset: %d, message: unbalanced bracets\n", __FUNCTION__, sptr-script);
						return -1;
					}
					if (*sptr == '[')
						bcount++;
					else if (*sptr == ']')
						bcount--;
				}
			}
			sptr++;
			break;
		case ']':
			bcount = -1;
			while (bcount) {
				sptr--;
				if (sptr == script) {
					fprintf(stderr, "function: %s, script offset: %d, message: unbalanced bracets\n", __FUNCTION__, sptr-script);
					return -1;
				}
				if (*sptr == '[')
					bcount++;
				else if (*sptr == ']')
					bcount--;
			}
			break;
		default:
			sptr++;
			break;
		}
	}
}


