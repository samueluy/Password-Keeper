/*
TODO:
encryption
for loop for all arrays to make them all 0
password input '*"
No need enter
Ascii art (Joshua)
*/

#include <stdio.h>
#include <string.h>

#define STRING_SIZE 256
#define MAX_LINE 256

struct credentials{ // CHANGE THIS MABOI
	char account_name[STRING_SIZE];
	char username[STRING_SIZE];
	char password[STRING_SIZE];
};

int validInput(int nMaxInput){ // Asks the user for input till valid (use for integer-navigated menus)
	int nInput=0, nValid=0;
	
	printf("Enter: ");
	scanf("%d", &nInput);
	while(!nValid){
		if(nInput > 0 && nInput <= nMaxInput)
			nValid=1;
		else{
		printf("Invalid!\nPlease enter a valid input: ");
		getc(stdin);
		scanf("%d", &nInput);
		}
	}
	
	return nInput;
}

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
    decrypt_algo(user.password, key);
    decrypt_algo(user.username, key);
    
   printf("\nDecrypt: %s", user.account_name);
}
int welcomeScreen(){
	
	printf("[1] Login\n");
	printf("[2] Create a new account\n");
	
	return validInput(2);
}

int login(char filename[STRING_SIZE]){ // goto main menu, make key do something
	FILE *common;
	
	int nSelect=0, nValidUser=0, nValid=0, count=0, flag=0, nContinue=0;
	char key[STRING_SIZE], line_buffer[STRING_SIZE];
	struct credentials user;
	
	while(!nContinue){
		common = fopen("common", "r");
		nValidUser=0,nValid=0,count=0,flag=0; // reset all values
		
		printf("Enter username: ");
		getc(stdin);
		fgets(user.username, STRING_SIZE, stdin);
		
		printf("Enter password: ");
		fgets(user.password, STRING_SIZE, stdin);
		printf("%s\n%s", user.username, user.password);
		
		while(fgets(line_buffer, MAX_LINE, common) != NULL && !flag){
			count++;
			
			if(count==1 && strcmp(user.username, line_buffer) == 0) // check every username
				nValidUser=1;
				
			if(count==2 && nValidUser){ // check password if username is valid
				if(strcmp(user.password, line_buffer) == 0)
					nValid=1;
				else
					flag=1; // exit loop if username does not match password
			}
			
			if(nValid && count==3){ // copy line_buffer to filename to use in main
				strcpy(filename, line_buffer);
				flag=1;
			}
			if(count==3) // reset count -> move to next user
				count=0;
		}
		
		if(!nValid){
			printf("Username/Password not found.\n");
			printf("[1] Try again?\n");
			printf("[2] Exit\n");

			nSelect = validInput(2);
			if(nSelect == 1);
			else{
				fclose(common);
				return 0;
			}
		}
		else{
			printf("Enter key: ");
			fgets(key, STRING_SIZE, stdin);
			nContinue=1;
		}
		
	fclose(common);
	}
	
	strtok(filename, "\n");

	return 1;
}

int newAccount(char filename[STRING_SIZE]){ // TODO: create key
	FILE *common, *user_file;
	
	int nSelect=0, nValid=1, nValidUsername=1, nValidFilename=1, count=0, nContinue=0;
	char line_buffer[STRING_SIZE];
	struct credentials user;
	
	while(!nContinue){
		common = fopen("common", "a+");
		nValid=1,nValidUsername=1,nValidFilename=1,count=0; // Reset all values
		printf("Enter username: ");
		getc(stdin);
		fgets(user.username, STRING_SIZE, stdin);
		
		printf("Enter password: ");
		fgets(user.password, STRING_SIZE, stdin);
		
		printf("Enter file name: ");
		fgets(filename, STRING_SIZE, stdin);
		
		while(fgets(line_buffer, MAX_LINE, common) != NULL && nValid==1){
			count++;
			if(count==1 && strcmp(user.username, line_buffer) == 0){ // check every username
				nValidUsername=0;
				nValid=0;
			}
			
			if(count==3 && strcmp(filename, line_buffer) == 0){ // check every filename
				nValidFilename=0;
				nValid=0;
			}
			
			if(count == 3) // reset count to 0
				count = 0;
		}
		
		if(!nValidFilename){
			printf("Filename is already in use.\n");
			printf("[1] Try again?\n");
			printf("[2] Exit\n");
			nSelect = validInput(2);
				if(nSelect == 1);
				else{
					fclose(common);
					return 0;
				}
		}

		else if(!nValidUsername){
			printf("Username is already in use.\n");
			printf("[1] Try again?\n");
			printf("[2] Exit\n");
			nSelect = validInput(2);
				if(nSelect == 1);
				else{
					fclose(common);
					return 0;
				}
			
		}

		else{
			nContinue = 1;
			fprintf(common, "%s", user.username);
			fprintf(common, "%s", user.password);
			fprintf(common, "%s", filename);
			
			strtok(filename, "\n");
			user_file = fopen(filename, "w"); // create user file
			fclose(user_file);
		}

		fclose(common);
	}
	
	return 1;
}

int mainMenu(){
	
	printf("[1] Display all passwords\n");
	printf("[2] Add a password\n");
	printf("[3] Change a password\n");
	printf("[4] Delete a password\n");
	printf("[5] Change your password for Password Keeper\n");
	printf("[6] Logout and exit Password Keeper\n");
	
	return validInput(6);
}

void displayCredentials(struct credentials user, int key, char filename[STRING_SIZE]){
	FILE *user_file;
	user_file = fopen(filename, "r");
	
	int count=0;
	char line_buffer[STRING_SIZE];
	

	while(fgets(line_buffer, MAX_LINE, user_file) != NULL){
		count++;
		if(count==1)
			printf("Account: %s", line_buffer); // change bring to center.
		else if(count==2)
			printf("Username: %s", line_buffer);
		else if(count==3){
			printf("Password: %s\n", line_buffer);
			count=0;
		}
	}
	fclose(user_file);
}

void addPassword(struct credentials *user, int key, char filename[STRING_SIZE]){ // check if unique application
	FILE *user_file;
	user_file = fopen(filename, "a+");
	
	int unique_name=1, count=0;
	char line_buffer[STRING_SIZE];
	struct credentials user;
	
	printf("Enter account name: ");
	getc(stdin);
	fgets(user.account_name, STRING_SIZE, stdin);

	printf("Enter username: ");
	fgets(user.username, STRING_SIZE, stdin);

	printf("Enter password: ");
	fgets(user.password, STRING_SIZE, stdin);
	
	while(fgets(line_buffer, MAX_LINE, user_file) != NULL){
		count++;
		if(count==1 && strcmp(user.account_name, line_buffer)==0)
			unique_name=0;
		
		if(count==3)
			count=0;
	}
	
	if(unique_name){
		encrypt(*user, key);
		fprintf(user_file, "%s", user.account_name);
		fprintf(user_file, "%s", user.username);
		fprintf(user_file, "%s", user.password);
	}
	else
		printf("Account name already exists.\n");
	
	fclose(user_file);
}

void changePassword(char filename[STRING_SIZE]){
	FILE *user_file, *temp;
	user_file = fopen(filename, "r");
	
	int valid_account_name=0, valid_password=0, line_number=0, count=0;
	char new_password[STRING_SIZE], line_buffer[STRING_SIZE];
	struct credentials user;
	
	printf("Enter account name: ");
	getc(stdin);
	fgets(user.account_name, STRING_SIZE, stdin);
	
	printf("Enter password: ");
	fgets(user.password, STRING_SIZE, stdin);
	
	while(fgets(line_buffer, MAX_LINE, user_file) != NULL && !valid_password){ //stop loop if user->password is valid
		line_number++;
		count++;
		if(!valid_account_name && (count==1 && strcmp(user.account_name, line_buffer)==0)) // check for username
			valid_account_name=1;
		
		if(valid_account_name && (count==3 && strcmp(user.password, line_buffer) == 0)){ // check for password below username
			valid_password=1;
			printf("Enter new password: ");
			fgets(new_password, STRING_SIZE, stdin);
		}
		
		if(count==3)
			count=0;
	}
	fclose(user_file);
	
	count=0;
	if(!valid_account_name || !valid_password)
		printf("Invalid account name/password\n");
	else{ // replace file with changed password instead of old
		user_file = fopen(filename, "r");
		temp = fopen("temp", "w");
			while(fgets(line_buffer, MAX_LINE, user_file) != NULL){
				count++;
				if(line_number==count)
					fprintf(temp, "%s", new_password);
				else
					fprintf(temp, "%s", line_buffer);
			}
			fclose(user_file);
			fclose(temp);
			remove(filename);
			rename("temp", filename);
	}
}

void deletePassword(char filename[STRING_SIZE]){
	FILE *user_file, *temp;
	user_file = fopen(filename, "r");
	
	int valid_account_name=0, count=0, line_number=0;
	char line_buffer[STRING_SIZE];
	struct credentials user;
	
	printf("Enter account name: ");
	fgets(user.account_name, STRING_SIZE, stdin);
	
	while(fgets(line_buffer, MAX_LINE, user_file) != NULL && !valid_account_name){
		line_number++;
		count++;
		if(!valid_account_name && (count==1 && strcmp(user.account_name, line_buffer)==0)) // check for username
			valid_account_name=1;
			
		if(count==3)
			count=0;
	}
	fclose(user_file);
	
	count=0; // reset count
	if(!valid_account_name)
		printf("Invalid account name\n");
	else{
		user_file = fopen(filename, "r");
		temp = fopen("temp", "w");

		while(fgets(line_buffer, MAX_LINE, user_file) != NULL){
				count++;
				if(count < line_number || (line_number+2) < count) // delete account lines
					fprintf(temp, "%s", line_buffer);
				else;
		}
		
		fclose(user_file);
		fclose(temp);
		remove(filename);
		rename("temp", filename);
	}
}

void changePasswordKeeper(){ // add user number
	FILE *common, *temp;
	common = fopen("common", "r");
	
	int valid_password=0, count=0, line_number=0;
	char new_password[STRING_SIZE], line_buffer[STRING_SIZE];
	struct credentials user;
	
	printf("Enter current password: ");
	fgets(user.password, STRING_SIZE, stdin);
	
	while(fgets(line_buffer, MAX_LINE, common) != NULL && !valid_password){ //stop loop if user->password is valid
		line_number++;
		count++;
		if(!valid_password && (count==2 && strcmp(user.password, line_buffer)==0)){ // check for username
			valid_password=1;
			printf("Enter new password: ");
			fgets(new_password, STRING_SIZE, stdin);
		}
		if(count==3)
			count=0;
	}
	count=0;
	fclose(common);
	
	if(!valid_password)
		printf("Invalid password\n");
	else{ // replace file with changed password instead of old
		common = fopen("common", "r");
		temp = fopen("temp", "w");
			while(fgets(line_buffer, MAX_LINE, common) != NULL){
				count++;
				if(line_number==count)
					fprintf(temp, "%s", new_password);
				else
					fprintf(temp, "%s", line_buffer);
			}
			fclose(common);
			fclose(temp);
			remove("common");
			rename("temp", "common");
	}
}

void endScreen(){
	
	printf("Thank you for using Password Keeper!\n");
	printf("See you next time!");
}


int main(void){
	// FILE *common, *user_file;
	//	user_file = fopen(filename, "r");
	
	int nInput, nSuccess;
	char filename[STRING_SIZE];
	
	nInput = welcomeScreen(); // open welcome screen. return user input to nInput
	
	switch (nInput){
		case 1:
			nSuccess = login(filename);
			break;
		case 2:
			nSuccess = newAccount(filename);
			break;
	}
	while(nInput!=6 && nSuccess){
		
		nInput = mainMenu(); // open main menu. return user input to nInput
	
		switch (nInput){ // main menu choices
			case 1:
				displayCredentials(filename);
				break;
			case 2:
				addPassword(filename);
				break;
			case 3:
				changePassword(filename);
				break;
			case 4:
				deletePassword(filename);
				break;
			case 5:
				changePasswordKeeper();
				break;
			case 6:
				endScreen();
				break;
		}
	}
	
//	fclose(common);

	return 0;
}
