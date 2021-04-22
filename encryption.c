#include <stdio.h>
#include <string.h>

#define STRING_SIZE 256
#define MAX_LINE 256

struct credentials{ // CHANGE THIS MABOI
	char account_name[STRING_SIZE];
	char username[STRING_SIZE];
	char password[STRING_SIZE];
};

void algorithm(char input[STRING_SIZE], int key){

    int i;
    int temp;
    int count;
    char temp_text[STRING_SIZE];

    for(i=0; i<strlen(input); i++){
        temp_text[i] = input[i];
    }
    
    for(i=0; i<strlen(temp_text); i++){
        count++;
        if(count==1){
            temp_text[i] += key;
            temp=temp_text[i];
        }
            
        else{
            temp_text[i] += temp;
            temp=temp_text[i];
        }
        if(count==4){
            count=0;
            temp=0;
        }
    }
    
    strcpy(input, temp_text);

}

void encrypt(struct credentials *user, int key){

    char string[STRING_SIZE];

    algorithm(user->account_name, key); // add struct to algo
    
    

}


int main(void){

    int key=24;
    struct credentials user;
    printf("Enter account name: ");
    fgets(user.account_name, STRING_SIZE, stdin);

    encrypt(&user, key);
    
    printf("\n%s", user.account_name);

    return 0;
}
