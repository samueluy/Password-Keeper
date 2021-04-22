#include <stdio.h>
#include <string.h>

void shuffle(char string[256]){
    int i=0;
    char temp[256];

    for(i=0; i<strlen(string); i++){
        temp[i] = string[i]+1;
    }

    strcpy(string, temp);
}
int main(void){
    
    char string[256];

    printf("Enter string: ");
    fgets(string, 256, stdin);

    shuffle(string);

    printf("%s", string);

    return 0;
}