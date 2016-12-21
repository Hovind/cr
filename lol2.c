#include <stdio.h>
 
int func(char **arg){
	(*arg)[1] = 'a';
	return 1;
}
 
int main(void) {
	// your code goes here
	char cstring[] = "hello";
	char *cp = cstring;
	char **cpp = &cp;
	printf("Before: %s\n", cstring);
	func(cpp);
	printf("After: %s\n", cstring);
	return 0;
}
 