/*
TODO:

(MAJOR) Check if all functions work

(MODIFY) if file is NULL
(MODIFY) Functions in a different file

(BACKUP) Create regular input for password
(FEATURE) encryption salt
(FEATURE) common file encryption (will change file name as well) (maybe make it hidden?)
(FEATURE) valid username (no space or other special character)

(FEATURE) Ascii art (Align to center)
(FEATURE) Login to register
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
//	scanf("%d", &nInput);

	while(!nValid){
		if(nInput > 0 && nInput <= nMaxInput)
			nValid=1;
		else{
		printf("\nInvalid!\nPlease enter a valid input: ");
		printf("Enter: ");
		nInput = getch() - '0';
	// using regular scanf
	//	scanf("%d", &nInput);
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
		passwrd[i-1]='\0';
}

int keyGen(char temp_key[STRING_SIZE]){
	int i, key=0;
	
	if(!strlen(temp_key)) // if user enters no key, 22 is the default key given by the programmer
		key = 22;
	else{
		for(i=0; i<strlen(temp_key); i++){
		key += temp_key[i];	
		}
	}
	
	key %= 30; // limit size of key
	
	if((key%5 == 1)) // mod 1 will not work with reset
		key+=1;
		
	return key;
}

void encrypt_algo(char input[STRING_SIZE], int nums[STRING_SIZE], int key){
	
    int i;
    int temp=0;
    int count=0;
    int reset = key % 5;
    
    
    for(i=0; i<strlen(input); i++) // get ascii values of user string input.
    	nums[i] = (int) input[i];
    
    for(i=0; i<strlen(input); i++){ // encrypt
        if(i==0){ // first character add key
        	nums[i] += key;
            temp=nums[i];
        }
        else if(count == reset){
        	nums[i] += key;
            temp=nums[i];
            count=0;
        }
        
        else{
            nums[i] += temp;
            temp=nums[i];
        }
        
        count++;
    }
}

void decrypt_algo(int nums[STRING_SIZE], int nums_length, int key){
	
    int i;
    int count=nums_length+1; // get string length for subtract key count
    int reset = key % 5;
	
    for(i=nums_length; i>=0; i--){
    	if(i == 0) // first character of string
        	nums[i] -= key;
        
        else if((count%reset) == 1)
        	nums[i] -= key;
        else
        	nums[i] -= nums[i-1];
    	
    	count--;
	}
	
	
	for(i=0; i<nums_length; i++)
		printf("%c", nums[i]);
}

void encrypt(struct credentials *user, int nums[STRING_SIZE], int key){

    encrypt_algo(user->account_name, nums, key); // add struct to algo
    encrypt_algo(user->password, nums,  key);
    encrypt_algo(user->username, nums, key);
}

void passwordKeeperArt(){
	printf("  ____                                     _\n |  _ \\ __ _ ___ _____      _____  _ __ __| |\n | |_) / _` / __/ __\\ \\ /\\ / / _ \\| '__/ _` |\n");
	printf(" |  __/ (_| \\__ \\__ \\\\ V  V / (_) | | | (_| |\n |_| __\\__,_|___/___/ \\_/\\_/ \\___/|_|  \\__,_|\n\n | |/ /___  ___ _ __   ___ _ __              \n");
	printf(" | ' // _ \\/ _ \\ '_ \\ / _ \\ '__|             \n | . \\  __/  __/ |_) |  __/ |                \n |_|\\_\\___|\\___| .__/ \\___|_|                \n");
	printf("               |_|                           \n");
}

int welcomeScreen(){
	
	system("cls");
	passwordKeeperArt();
	printf("[1] Login\n[2] Create a new account\n");
	
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
		scanf("%s", user.username);
	//	fgets(user.username, STRING_SIZE, stdin);
		
		printf("Enter password: ");
		enterPass(user.password);
		
		if(common != NULL){
			while(!feof(common) && !flag){
			count++;
			fscanf(common, "%s", line_buffer);
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
				printf("\nUsername/Password not found.\n[1] Try again?\n[2] Back\n[3] Exit\n");
				
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
		else{
			printf("\nNo account in the database. Please create an account.\n\nPress any key to continue...");
			getch();
			return 2;
		}
	}
		
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
		scanf(" %s", user.username);
		
		printf("Enter password: ");
		enterPass(user.password);
		
		printf("\nEnter file name: ");
		scanf(" %s", filename);
		
		while(!feof(common) && nValid==1){
			count++;
			fscanf(common, "%s", line_buffer);
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
			printf("Filename is already in use.\n[1] Try again?\n[2] Back\n[3] Exit\n");
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
			printf("Username is already in use.\n[1] Try again?\n[2] Back\n[3] Exit\n");
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
			fprintf(common, "%s ", user.username);
			fprintf(common, "%s ", user.password);
			fprintf(common, "%s ", filename);

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
	printf("\n[1] Display all passwords\n[2] Add a password\n[3] Change a password\n[4] Delete a password\n");
	printf("[5] Change your password for Password Keeper\n[6] Logout and exit Password Keeper\n");
	
	return validInput(6);
}

void displayCredentials(char filename[STRING_SIZE], int key){
	FILE *user_file;
	user_file = fopen(filename, "r");
	
	int i=0;
	int num_count=0, count=0, first=1;
	int current_num;
	int nums[STRING_SIZE];
	int account=1, username=0, password=0;
	
	system("cls");
	passwordKeeperArt();
	
	// add decrypt algorithm
	
	if(user_file != NULL){
		while(fscanf(user_file, "%d", &current_num) != EOF){
			nums[i] = current_num;
			i++;
			
			if(first){
				if(account){
					printf("\nAccount: ");
					account=0;
					username=1;
				}
				else if(username){
					printf("\nUsername: ");
					username=0;
					password=1;
				}
				else if(password){
					printf("\nPassword: ");
					password=0;
					account=1;
				}
				
				first=0;
		}
		
		if(current_num == 1434){ // marker for end of specific credential line, refresh first
			count++;
			first=1;
			decrypt_algo(nums, num_count, key);
			i=0;
			num_count=0;
			}
		else if(current_num == 14344){ // marker for end of account details, refresh first
			decrypt_algo(nums, num_count, key);
			printf("\n");
			count=0;
			first=1;
			i=0;
			num_count=0;
			}
		else
		//	printf("%c", current_num);
			num_count++;
		}
	}
	else
		printf("\nNo password has been created\n");
		
	printf("\nPress any key to continue...");
	getch();
	
	fclose(user_file);
}

void addPassword(char filename[STRING_SIZE], int key){ // check if unique application
	FILE *user_file;
	user_file = fopen(filename, "a+");
	
	int i;
	int unique_name=1, count=0;
	int nums[STRING_SIZE];
	struct credentials user;
	int current_num;
	
	system("cls");
	passwordKeeperArt();
	
	printf("\nEnter account name: ");
	scanf("%s", user.account_name);
	
	printf("Enter username: ");
	scanf("%s", user.username);

	printf("Enter password: ");
	enterPass(user.password);
	
	while(!feof(user_file)){
		count++;
		
		fscanf(user_file, "%d", &current_num);
/*		decrypt_algo(line_buffer, key);
		
		if(count==1 && strcmp(user.account_name, line_buffer)==0)
			unique_name=0;
		
		if(count==3)
			count=0;
*/
	}
	
	if(unique_name){
		encrypt_algo(user.account_name, nums, key);
		for(i=0; i<strlen(user.account_name); i++)
			fprintf(user_file, "%d ", nums[i]);
		
		fprintf(user_file, "%d ", 1434);
		
		encrypt_algo(user.username, nums, key);
		for(i=0; i<strlen(user.username); i++)
			fprintf(user_file, "%d ", nums[i]);
		
		fprintf(user_file, "%d ", 1434);
		
		encrypt_algo(user.password, nums, key);
		for(i=0; i<strlen(user.password); i++)
			fprintf(user_file, "%d ", nums[i]);
		
		fprintf(user_file, "%d ", 14344);
	}
	else{
		printf("\nAccount name already exists.\n\nPress any key to continue...");
		getch();
	}
	
	fclose(user_file);
}

void changePassword(char filename[STRING_SIZE], int key){
	FILE *user_file, *temp;
	user_file = fopen(filename, "r");
	
	system("cls");
	passwordKeeperArt();
	
	int valid_account_name=0, valid_password=0, line_number=0, count=0;
	char new_password[STRING_SIZE], line_buffer[STRING_SIZE];
	struct credentials user;
	
	printf("\nEnter account name: ");
	scanf("%s", user.account_name);
	
	printf("Enter password: ");
	enterPass(user.password);
	
	while(!feof(user_file) && !valid_password){ //stop loop if user->password is valid
		line_number++;
		count++;
		
		fscanf(user_file, "%s", line_buffer);
		//decrypt_algo(line_buffer, key);
		
		if(!valid_account_name && (count==1 && strcmp(user.account_name, line_buffer) == 0)) // check for username
			valid_account_name=1;
		
		if(valid_account_name && (count==3 && strcmp(user.password, line_buffer) == 0)){ // check for password below username
			valid_password=1;
			printf("\nEnter new password: ");
			enterPass(new_password);
		}
		if(count==3)
			count=0;
	}
	fclose(user_file);
	
	count=0;
	if(!valid_account_name || !valid_password){
		printf("\nInvalid account name/password\n\nPress any key to continue...");
		getch();
	}
	else{ // replace file with changed password
	//	encrypt_algo(new_password, key);
		user_file = fopen(filename, "r");
		temp = fopen("temp", "w");
			while(!feof(user_file)){
				fscanf(user_file, "%s", line_buffer);
				count++;
				
				if(line_number==count)
					fprintf(temp, "%s ", new_password);
				else
					fprintf(temp, "%s ", line_buffer);
			}
			fclose(user_file);
			fclose(temp);
			remove(filename);
			rename("temp", filename);
	}
}

void deletePassword(char filename[STRING_SIZE], int key){
	FILE *user_file, *temp;
	user_file = fopen(filename, "r");
	
	int valid_account_name=0, count=0, line_number=0;
	char line_buffer[STRING_SIZE];
	struct credentials user;
	
	system("cls");
	passwordKeeperArt();
	
	printf("\nEnter account name: ");
	scanf("%s", user.account_name);
	
	while(!feof(user_file) && !valid_account_name){
		line_number++;
		count++;
		
		fscanf(user_file, "%s", line_buffer);
		//decrypt_algo(line_buffer, key);
		
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

		while(!feof(user_file)){
				count++;
				
				fscanf(user_file, "%s", line_buffer);
				
				if(count < line_number || (line_number+2) < count) // delete account lines
					fprintf(temp, "%s", line_buffer);
				else;
		}
		
		fclose(user_file);
		fclose(temp);
		remove(filename);
		rename("temp", filename);
		
		printf("\"%s\" has been deleted\n\nPress any key to continue...", user.account_name);
		getch();
		
	}
}

void changePasswordKeeper(){ // add user number
	FILE *common, *temp;
	common = fopen("common", "r");
	
	int valid_password=0, count=0, line_number=0;
	char new_password[STRING_SIZE], line_buffer[STRING_SIZE];
	struct credentials user;
	
	system("cls");
	passwordKeeperArt();
	
	printf("\nEnter current password: ");
	enterPass(user.password);
	
	while(!feof(common) && !valid_password){ //stop loop if user->password is valid
		line_number++;
		count++;
		
		fscanf(common, "%s", line_buffer);
		if(!valid_password && (count==2 && strcmp(user.password, line_buffer)==0)){ // check for username
			valid_password=1;
			printf("\nEnter new password: ");
			enterPass(new_password);
		}
		if(count==3)
			count=0;
	}
	count=0;
	fclose(common);
	
	if(!valid_password){
		printf("\nInvalid password\n\nPress any key to continue...");
		getch();
		
	}
	else{ // replace file with changed password instead of old
		common = fopen("common", "r");
		temp = fopen("temp", "w");
			while(!feof(common)){
				count++;
				fscanf(common, "%s", line_buffer);
				if(line_number==count)
					fprintf(temp, "%s ", new_password);
				else
					fprintf(temp, "%s ", line_buffer);
			}
			fclose(common);
			fclose(temp);
			remove("common");
			rename("temp", "common");
			
			printf("Password changed!\n\nPress any key to continue...");
			getch();
	}
}

void endScreen(){
	
	system("cls");
	passwordKeeperArt();
	printf("\nThank you for using Password Keeper!\n\nSee you next time!");
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
				changePassword(filename, key);
				break;
			case 4:
				deletePassword(filename, key);
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
