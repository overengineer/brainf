/*
Brainf**k interpreter by Alper Said Soylu
There are decimal and ascii modes.
Both tape and cells are circular.
Cells are 8 bit.
*/
#include <stdio.h>
#include <conio.h>
#include <cstdlib>
#include <string.h>

class Bf {
public:
	Bf();
	int inputf(FILE* stream, size_t fsize);
	int execute(int tapesize);
	void print();
	void setmode(bool m);
	~Bf();
private:
	bool d;
	char* script;
	int script_end;
	int tape_end;
	unsigned char* tape;
};

int main(void)
{
	Bf bf;
	FILE* stream;
	char path[100];
	size_t fsize;
	bool c;
	while (1) {
		//add ascii and decimal modes
		puts("\ndecimal mode(0) or ascii mode(1)?:");
		scanf_s("%d",(int*)&c);
		bf.setmode(c);
		puts("\nread from file(0) or write on console(1)?:");
		scanf_s("%d", (int*)&c);
		puts("enter max script size:");
		if (!scanf_s("%d", &fsize)) {
			fprintf(stderr, "function: %s \t message: fscanf_s error\n", __FUNCTION__);
			break;
		}
		if (c) {
			puts("enter your code:");
			stream = stdin;
		}
		else {
			puts("enter file directory:");
			if (!scanf_s("%s", path,100)) {
				fprintf(stderr, "function: %s \t message: fscanf_s error\n", __FUNCTION__);
				break;
			}
			if (fopen_s(&stream, path, "r")) {
				fprintf(stderr, "function: %s \t message: fopen_s error\n", __FUNCTION__);
				break;
			}
		}
		if (bf.inputf(stream,fsize)) {
			break;
		}
		if(bf.execute(100000)) {
			break;
		}
		bf.print();
		if (!c) {
			fclose(stream);
		}
	}
	perror("error message"); system("PAUSE");return -1;
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
	if (d) {
		unsigned char* tptr;
		for (tptr = tape;tptr != &tape[tape_end] && (*tptr);printf("%d ", (*tptr)), tptr++);
	}
	else {
		printf("\n%s\n\n", tape);
	}
}
void Bf::setmode(bool  m) {
	d = m;
}

int Bf::inputf(FILE* stream, size_t fsize)
{
	int i;
	if (fsize == 0) {
		fprintf(stderr, "function: %s \t message: zero value assigned to argument fsize\n", __FUNCTION__);
		return -1;
	}
	if (stream == NULL) {
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
	if (!fscanf_s(stream,"%s",script, scriptsize)) {
		fprintf(stderr, "function: %s \t message: fscanf_s error\n", __FUNCTION__);
		return -1;
	}
	for (i = 0;script[i] != '\0';i++);
	script_end = i;
	return 0;
}

int Bf::execute(int tapesize)
{
	tape_end = tapesize;
	if (tapesize == 0) {
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
	tape = (unsigned char*)malloc(sizeof(unsigned char)*(tapesize + 1));
	if (tape == NULL) {
		fprintf(stderr, "function: %s \t message: couldnt allocate memory\n", __FUNCTION__);
		return -1;
	}
	for (i = 0;i<tapesize;i++) {
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
				scanf_s("%d", (int*)tptr);
			}
			else {
				*tptr = _getch();
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
						fprintf(stderr, "function: %s \t message: unbalanced brackets\n", __FUNCTION__);
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
					fprintf(stderr, "function: %s \t message: unbalanced brackets\n", __FUNCTION__);
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


