#include "common.h"

unsigned int tm = 250;

unsigned int opt_uint(const char *arg)
{
	long val;

	val = strtol(arg, (char **)&arg, 0);
	if((*arg != '\0') || (val < 0) || (val >= UINT_MAX))
		fprintf(stderr, "Invalid number."), exit(1);

	return val;
}

int main(int argc, char **argv)
{
	bool max = false;
	bool debug = false;
	unsigned int i = 1, f_min = CPU_MIN, f_max = CPU_MAX;

	while(argv[i] != NULL) {
		if(argv[i][0] == '-') {
			if(strcmp(argv[i] + 1, "min") == 0) {
				f_min = opt_uint(argv[i+1]);
				if(!cpu_chk(f_min))
					fprintf(stderr, "Invalid frequency '%u'.", f_min);

				i += 2;
			}
			else if(strcmp(argv[i] + 1, "max") == 0) {
				f_max = opt_uint(argv[i+1]);
				if(!cpu_chk(f_max))
					fprintf(stderr, "Invalid frequency '%u'.", f_max);

				i += 2;
			}
			else if(strcmp(argv[i] + 1, "debug") == 0)
				debug = true, i++;
			else
				fprintf(stderr, "Invalid option '%s'.\n", argv[i] + 1), exit(1);
		}
		else
			fprintf(stderr, "Invalid argument '%s'.\n", *argv), exit(1);
	}

	cpu_set(CPU_MIN);
	cpu_util();

	while(true) {
		float util;

		usleep(1000*tm);
		util = cpu_util();

		if(max) {
			if(util < 0.5) {
				cpu_set(f_min);
				max = false;

				if(debug)
					printf("%s: cpu set to %u\n", dbgtime(), f_min);
			}
		}
		else {
			if(util > 0.5) {
				cpu_set(f_max);
				max = true;

				if(debug)
					printf("%s: cpu set to %u\n", dbgtime(), f_max);
			}
		}
	}

	return 0;
}