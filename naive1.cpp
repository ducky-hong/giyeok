#include "Parser.h"
#include "Runner.h"

void loadLibrary();

Parser *prsr;

int main(int argc, char **argv) {
	Eblock *program;
	Runner *runner;

	prsr=new Parser();
	loadLibrary();
	if (argc<2) program=prsr->parseFile("test.gy");
	else program=prsr->parseFile(argv[1]);
	if (!program) {
		printf("File not found : %s\n", argv[1]);
	} else {
		runner=new Runner();
		program->run(runner);
	}
	return 0;
}
