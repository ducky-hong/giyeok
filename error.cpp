#include <stdio.h>
#include <stdlib.h>
#include "error.h"

void error(Error e) {
	switch (e) {
	case ERROR_PARSING_WRONGIDENTIFIER:
		printf("ERROR: Wrong identifier\n");
		exit(0);
	case ERROR_PARSING_WRONGFUNCTIONDEFINITION:
		printf("ERROR: Wrong function definition\n");
		exit(0);
	case ERROR_PARSING_WRONGFUNCTIONCLOSURE:
		printf("ERROR: Wrong function definition\n");
		exit(0);
	case ERROR_PARSING_HASTWOVARARG:
		printf("ERROR: A function can have only one variable arguments\n");
		exit(0);
	case ERROR_PARSING_WRONGCLASSDEFINITION:
		printf("ERROR: Wrong class definition\n");
		exit(0);
	case ERROR_PARSING_WRONGUSEDEFINITION:
		printf("ERROR: Wrong use definition\n");
		exit(0);
	case ERROR_INTEGRATION_WRONGDOT:
		printf("ERROR: Dot should be followed by identifier\n");
		exit(0);
	case ERROR_INTEGRATION_CANNOTFINDSYMBOL:
		printf("ERROR: Cannot find symbol\n");
		exit(0);
	case ERROR_RUNTIME_PARAMNEEDSVARIABLE:
		printf("ERROR: Argument declared with VAR should be given a variable\n");
		exit(0);
	}
}
