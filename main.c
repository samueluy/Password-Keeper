/*
TODO:

(FEATURE) encryption salt
(FEATURE) common file encryption (will change file name as well) (maybe make it hidden?)
(FEATURE) valid username (no space or other special character)
(BUG) encryption fix (Key does not change everyting) (if 1 char, won't work)
(BUG) repeated username is allowed
(BUG) cant detect if unique if key is wrong

(FEATURE) Ascii art (Align to center)
(FEATURE) Login to register
(FEATURE) clear screen (finish adding to all) + (Display credentials dissapperaing)
(FEATURE) add a back navigate thingy
(FEATURE) press any key to continue

(FEATURE) invalid input clear screen
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

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
	nInput = getch() - '0';

	while(!nValid){
		if(nInput > 0 && nInput <= nMaxInput)
			nValid=1;
		else{
		printf("\nInvalid!\nPlease enter a valid input: ");
		printf("Enter: ");
		nInput = getch() - '0';
		}
	}
	
	return nInput;
}

void enterPass(char passwrd[STRING_SIZE]){ // Make password print '*' instead of the password itself for security
	int i=0;
	do{
			passwrd[i]=getch();
			if(passwrd[i]!='\r')
				printf("*");
			i++;
		}
		while(passwrd[i-1]!='\r');
		passwrd[i-1]='\n';
}

int keyGen(char temp_key[STRING_SIZE]){
	int i, key=0;
	
	for(i=0; i<strlen(temp_key); i++){
		key += temp_key[i];
	}
	
	return key;
}

void encrypt_algo(char input[STRING_SIZE], int key){ // for some reason, yung first lang nag iiba
	
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
}
void passwordKeeperArt(){
	printf("  ____                                     _\n");
	printf(" |  _ \\ __ _ ___ _____      _____  _ __ __| |\n");
	printf(" | |_) / _` / __/ __\\ \\ /\\ / / _ \\| '__/ _` |\n");
	printf(" |  __/ (_| \\__ \\__ \\\\ V  V / (_) | | | (_| |\n");
	printf(" |_| __\\__,_|___/___/ \\_/\\_/ \\___/|_|  \\__,_|\n\n");
	printf(	" | |/ /___  ___ _ __   ___ _ __              \n");
	printf(" | ' // _ \\/ _ \\ '_ \\ / _ \\ '__|             \n");
	printf(" | . \\  __/  __/ |_) |  __/ |                \n");
	printf(" |_|\\_\\___|\\___| .__/ \\___|_|                \n");
	printf("               |_|                           \n");
}

int welcomeScreen(){
	
	system("cls");
	passwordKeeperArt();
	printf("[1] Login\n");
	printf("[2] Create a new account\n");
	
	return validInput(2);
}

int login(char filename[STRING_SIZE], char temp_key[STRING_SIZE]){ // goto main menu, make key do something
	FILE *common;
	
	int nSelect=0, nValidUser=0, nValid=0, count=0, flag=0, nContinue=0;
	char line_buffer[STRING_SIZE];
	struct credentials user;
	
	system("cls");
	passwordKeeperArt();
	while(!nContinue){
		common = fopen("common", "r");
		nValidUser=0,nValid=0,count=0,flag=0; // reset all values
		
		printf("\nEnter username: ");
		fgets(user.username, STRING_SIZE, stdin);
		
		printf("Enter password: ");
		enterPass(user.password);
				
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
			system("cls");
			passwordKeeperArt();
			printf("\nUsername/Password not found.\n");
			printf("[1] Try again?\n");
			printf("[2] Back\n");
			printf("[3] Exit\n");

			nSelect = validInput(3);
			if(nSelect == 1){
				system("cls");
				passwordKeeperArt();
			}
			else if(nSelect == 2){
				fclose(common);
				return 2;
			}
			else{
				fclose(common);
				return 3;
			}
		}
		else{
			printf("\nEnter key: ");
			enterPass(temp_key);
			nContinue=1;
		}
		
	fclose(common);
	}
	
	strtok(filename, "\n");

	return 1;
}

int newAccount(char filename[STRING_SIZE], char temp_key[STRING_SIZE]){ // TODO: create key
	FILE *common, *user_file;
	
	int nSelect=0, nValid=1, nValidUsername=1, nValidFilename=1, count=0, nContinue=0;
	char line_buffer[STRING_SIZE];
	struct credentials user;
	
	system("cls");
	passwordKeeperArt();
	while(!nContinue){
		common = fopen("common", "a+");
		nValid=1,nValidUsername=1,nValidFilename=1,count=0; // Reset all values
		
		printf("\nEnter username: ");
		fgets(user.username, STRING_SIZE, stdin);
		
		printf("Enter password: ");
		enterPass(user.password);
		
		printf("\nEnter file name: ");
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
			printf("[2] Back\n");
			printf("[3] Exit\n");
			nSelect = validInput(3);
				if(nSelect == 1);
				else if(nSelect == 2){
					fclose(common);
					return 2;
				}
				else{
					fclose(common);
					return 3;
				}
		}

		else if(!nValidUsername){
			printf("Username is already in use.\n");
			printf("[1] Try again?\n");
			printf("[2] Back \n");
			printf("[3] Exit\n");
			nSelect = validInput(3);
				if(nSelect == 1);
				else if(nSelect == 2){
					fclose(common);
					return 2;
				}
				else{
					fclose(common);
					return 3;
				}
		}

		else{
			printf("Enter key: ");
			enterPass(temp_key);
			
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
	
	system("cls");
	passwordKeeperArt();
	printf("\n[1] Display all passwords\n");
	printf("[2] Add a password\n");
	printf("[3] Change a password\n");
	printf("[4] Delete a password\n");
	printf("[5] Change your password for Password Keeper\n");
	printf("[6] Logout and exit Password Keeper\n");
	
	return validInput(6);
}

void displayCredentials(char filename[STRING_SIZE], int key){
	FILE *user_file;
	user_file = fopen(filename, "r");
	
	int count=0;
	char line_buffer[STRING_SIZE];
	
	system("cls");
	passwordKeeperArt();
	
	while(fgets(line_buffer, MAX_LINE, user_file) != NULL){
		strtok(line_buffer, "\n");
		count++;
		if(count==1){
			printf("Account: "); // change bring to center.
			decrypt_algo(line_buffer, key);
			printf("%s", line_buffer);
		}
		else if(count==2){
			printf("Username: "); // change bring to center.
			decrypt_algo(line_buffer, key);
			printf("%s", line_buffer);
		}
			
		else if(count==3){
			printf("Password: "); // change bring to center.
			decrypt_algo(line_buffer, key);
			printf("%s", line_buffer);
			printf("\n");
			count=0;
		}
	}
	
	printf("[1] Back\n");
	validInput(1);
	
	fclose(user_file);
}

void addPassword(char filename[STRING_SIZE], int key){ // check if unique application
	FILE *user_file;
	user_file = fopen(filename, "a+");
	
	int unique_name=1, count=0;
	char line_buffer[STRING_SIZE];
	struct credentials user;
	
	system("cls");
	passwordKeeperArt();
	
	printf("\nEnter account name: ");
	fgets(user.account_name, STRING_SIZE, stdin);

	printf("Enter username: ");
	fgets(user.username, STRING_SIZE, stdin);

	printf("Enter password: ");
		enterPass(user.password);
	
	while(fgets(line_buffer, MAX_LINE, user_file) != NULL){
		count++;
		line_buffer[strcspn(line_buffer,"\n")]=0;
		decrypt_algo(line_buffer, key);
		
		if(count==1 && strcmp(user.account_name, line_buffer)==0)
			unique_name=0;
		
		if(count==3)
			count=0;
	}
	
	if(unique_name){
		encrypt(&user, key);
		fprintf(user_file, "%s\n", user.account_name);
		fprintf(user_file, "%s\n", user.username);
		fprintf(user_file, "%s\n", user.password);
	}
	else{
		printf("\nAccount name already exists.\n");
		printf("Press any key to continue: ");
		getch();
	}
	
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
	
	system("cls");
	passwordKeeperArt();
	printf("\nThank you for using Password Keeper!\n");
	printf("See you next time!");
}


int main(void){
	
	int nInput, nSuccess=2, key;
	char filename[STRING_SIZE], temp_key[STRING_SIZE];
	
	
	while(nSuccess == 2){
		nInput = welcomeScreen(); // open welcome screen. return user input to nInput
		switch (nInput){
			case 1:
				nSuccess = login(filename, temp_key);
				break;
			case 2:
				nSuccess = newAccount(filename, temp_key);
				break;
		}
	}
	
	if(nSuccess == 3)
		nInput = 6;

	key = keyGen(temp_key);
	
	while(nInput!=6 && nSuccess){
		
		nInput = mainMenu(); // open main menu. return user input to nInput
	
		switch (nInput){ // main menu choices
			case 1:
				displayCredentials(filename, key);
				break;
			case 2:
				addPassword(filename, key);
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
				
			case 6: break;
		}
	}
	
	endScreen();
	
//	fclose(common);

	return 0;
}
