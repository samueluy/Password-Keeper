#include <stdio.h>
#include <string.h>

#define STRING_SIZE 256
#define MAX_LINE 256

struct credentials{ // CHANGE THIS MABOI
	char account_name[STRING_SIZE];
	char username[STRING_SIZE];
	char password[STRING_SIZE];
};

void encrypt_algo(char input[STRING_SIZE], char temp_key[STRING_SIZE]){
	
    int i;
    int key=0;
    int temp=0;
    int count=0;
    char temp_text[STRING_SIZE];
    
    for(i=0; i<strlen(temp_key); i++)
    	key += temp_key[i];
    
    strcpy(temp_text, input);
    
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
        
        if(count==4){ // CHANGE THIS PARA MAGING MAS VERSETILE (maybe salt?)
            count=0;
            temp=0;
        }
    }
    strcpy(input, temp_text);
}

void encrypt(struct credentials *user, char key[STRING_SIZE]){

    encrypt_algo(user->account_name, key); // add struct to algo
    encrypt_algo(user->password, key);
    encrypt_algo(user->username, key);
}

/*void decrypt(struct credentials *user, char key[STRING_SIZE]){

    encrypt_algo(user->account_name, key); // add struct to algo
    encrypt_algo(user->password, key);
    encrypt_algo(user->username, key);
}*/

int main(void){
	
	char key[STRING_SIZE];
	
    struct credentials user;
    printf("Enter account name: ");
    scanf("%s", user.account_name);
   // fgets(user.account_name, STRING_SIZE, stdin);
   fflush(stdin);
    printf("Enter key: ");
    scanf("%s", key);
   // fgets(key, STRING_SIZE, stdin);

    encrypt(&user, key); // n+:ƒ
  //  decrypt(&user, key);
    
    
    printf("\n%s", user.account_name);

    return 0;
}
