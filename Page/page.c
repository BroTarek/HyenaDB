#include<stdio.h>
#include<stdint.h>

typedef struct{
uint16_t page_checksum;
uint16_t page_flags;
void* page_lower;
void* page_upper;
void* page_special;
uint16_t page_size;/*8kb*/
}PageHeader;


int main(){
	int s=66;
	int* addressOfS=&s;
	printf("%ls",addressOfS);
	return 0;
}
