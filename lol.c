#include <stdio.h>

char * lolfunc() {
	return "Per";
}
int main() {
	char * hei = lolfunc();
	int asd = 3;
	printf("%s is the string, %i is the number.\n", hei, asd);
	return 0;
}
