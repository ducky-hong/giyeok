#include "Parser.h"
#include "Runner.h"

void loadLibrary();
void printprogram(GYstring, Eblock*);

Parser *prsr;

int main(int argc, char **argv) {
	Eblock *program;
	Runner *runner;
	bool pp=false;
	GYstring filename;

	prsr=new Parser();
	loadLibrary();
	if (argc<2) filename="test.gy";
	else {
		int i;

		for (i=1; i<argc; i++)
			if (argv[i][0]=='-') {
				if (strcmp(argv[1], "-pp")==0)
					pp=true;
			} else filename=argv[i];
	}
	program=prsr->parseFile(filename);
	if (!program) {
		printf("File not found : %s\n", filename);
	} else {
		if (pp) printprogram(filename, program);
		runner=new Runner();
		program->run(runner);
	}
	return 0;
}
