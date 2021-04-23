#include <stdio.h>
#include <string.h>

#define STRING_SIZE 256
#define MAX_LINE 256

struct credentials{ // CHANGE THIS MABOI
	char account_name[STRING_SIZE];
	char username[STRING_SIZE];
	char password[STRING_SIZE];
};

void encrypt_algo(char input[STRING_SIZE], int key){
	
    int i;
    int temp=0;
    int count=0;
    char temp_text[STRING_SIZE];
    
    strcpy(temp_text, input);
    
    for(i=0; i<strlen(temp_text); i++){
        if(i==0){
        	temp_text[i] += key;
            temp=temp_text[i];
		}/* THIS PART IS NOT WORKING sad
        else if(count==1){
            temp_text[i] += key;
            temp=temp_text[i];
        }*/
        else{
            temp_text[i] += temp;
            temp=temp_text[i];
        }
        
        if(count==4){ // CHANGE THIS PARA MAGING MAS VERSETILE (maybe salt?)
            count=0;
            //temp=0;
        }
        count++;
    }
    strcpy(input, temp_text);
}

void decrypt_algo(char input[STRING_SIZE], int key){
	
    int i;
    int temp=0;
    int count=strlen(input)-1; // get string length for subtract key count
    char temp_text[STRING_SIZE];
    
    strcpy(temp_text, input);
    for(i=strlen(temp_text)-1; i>=0; i--){
        if(i == 0){
        	temp_text[i] -= key;
		}
       /* else if(count%5==0){
            temp_text[i] -= key;
            temp=temp_text[i];
        }*/
        else{
        	temp=temp_text[i-1];
            temp_text[i] -= temp;
        }
        count--;
    }
    strcpy(input, temp_text);
}

void encrypt(struct credentials *user, int key){

    encrypt_algo(user->account_name, key); // add struct to algo
    encrypt_algo(user->password, key);
    encrypt_algo(user->username, key);
}

void decrypt(struct credentials user, int key){

    decrypt_algo(user.account_name, key); // add struct to algo
   // decrypt_algo(user.password, key);
   // decrypt_algo(user.username, key);
    
   printf("\nDecrypt: %s", user.account_name);
}

int main(void){
	
	int key = 10;
	
    struct credentials user;
   /* printf("Enter account name: ");
    scanf("%s", user.account_name);
   // fgets(user.account_name, STRING_SIZE, stdin);
    fflush(stdin);
    printf("Enter key: ");
    scanf("%s", key);
   // fgets(key, STRING_SIZE, stdin);*/
   	strcpy(user.account_name, "helloo");
    encrypt(&user, key);
    
    printf("\nEncrypt: %s", user.account_name);
    decrypt(user, key);
    

    return 0;
}
