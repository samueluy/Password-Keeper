#include <stdio.h>

#include "HEADER.h"

struct credentials{
	char account_name[STRING_SIZE];
	char username[STRING_SIZE];
	char password[STRING_SIZE];
};

int validInput(int nMaxInput){
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

void enterPass(char passwrd[STRING_SIZE]){
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
	
	if((key%5 == 1) || (key%5 == 0))// mod 1 and 0 will not work with reset
		key+=2;
		
	return key;
}

void encrypt_algo(char input[STRING_SIZE], int nums[STRING_SIZE], int key){	
    int i, temp=0, count=0;
    int reset = key % 5;
    
    key += strlen(input); // salt
    
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

void store_credentials(char input[STRING_SIZE], char filename[STRING_SIZE], int key, int which){
	FILE *user_file;
	user_file = fopen(filename, "a+");
	
	int i;
	int nums[STRING_SIZE];
	
	if(user_file != NULL){
		encrypt_algo(input, nums, key);
	for(i=0; i<strlen(input); i++)
		fprintf(user_file, "%d ", nums[i]);
	
	if(which==1)
		fprintf(user_file, "%d ", END_LINE);
	else if(which==2)
		fprintf(user_file, "%d ", END_GROUP);
	
	fclose(user_file);
	}
	else{
		printf("Error! Could not open file. Please contact administrator.\n\nPress any key to continue...");
		getch();
	}
}

void decrypt_algo(int nums[STRING_SIZE], int nums_length, int key, char decrypted[STRING_SIZE]){
    int i;
    int count=nums_length+1; // get string length for subtract key count
    int reset = key % 5;
	
	key += nums_length; // SALT
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
		decrypted[i] = nums[i];
	
	decrypted[nums_length] = '\0'; // set last to NULL
}

int check_same(char input[STRING_SIZE], char filename[STRING_SIZE], int which, int key, int *line_count){
	FILE *user_file;
	user_file = fopen(filename, "r");
	
	int current_num, length=0, same_count=0, detail_count, i=0, y;
	int nums[STRING_SIZE], temp[STRING_SIZE];
	
	encrypt_algo(input, nums, key); // get the values of encrypted input
	
	if(user_file != NULL){
		while(fscanf(user_file, "%d", &current_num) != EOF){
			temp[i] = current_num;
			if(current_num == END_LINE || current_num == END_GROUP){
				detail_count++;
				*line_count += 1;
				if(strlen(input) > length)
					length = strlen(input);
				for(y=0; y<length; y++){
					if(nums[y] == temp[y])
						same_count++;
					else
						y=length;
				}
				if(same_count == length && which==detail_count){
					fclose(user_file);
					return 1;
				}
					
				i=-1;
				length=-1;
				same_count=0;
				if(current_num == END_GROUP)
					detail_count=0;
			}
			i++;
			length++;
		}
		fclose(user_file);
		return 0;
	}
	else
		return -1;
	fclose(user_file);
	return -1; // failed
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

int login(char filename[STRING_SIZE], char temp_key[STRING_SIZE]){
	FILE *common;
	
	int nSelect, current_number, line_count, username_line_count, valid_username, nValid, nContinue=0, i, y;
	int file_nums[STRING_SIZE], encrypted_password[STRING_SIZE], saved_pass[STRING_SIZE];;
	char decrypted_password[STRING_SIZE], decrypted_filename[STRING_SIZE];
	struct credentials user;
	
	system("cls");
	passwordKeeperArt();
	while(!nContinue){
		common = fopen("common", "r");
		valid_username=0, nValid=0, line_count=0, i=0, y=0, username_line_count=0; // reset all values
		
		printf("\nEnter username: ");
		scanf("%s", user.username);
		
		printf("Enter password: ");
		enterPass(user.password);
		encrypt_algo(user.password, encrypted_password, MAIN_KEY);
		
		if(common != NULL){
			valid_username = check_same(user.username, "common", 1, MAIN_KEY, &username_line_count);
			if(valid_username){
				while(fscanf(common, "%d", &current_number) != EOF){
					if(current_number == END_LINE || current_number == END_GROUP)
						line_count++;

				if(username_line_count == line_count){ // Password of inputted account name
					saved_pass[i-1] = current_number;
					i++;
				}
				if(username_line_count+1 == line_count){ // Filename of inputted account name
					file_nums[y-1] = current_number;
					y++;
				}
			}
			
			decrypt_algo(saved_pass, i-1, MAIN_KEY, decrypted_password);
			if(!strcmp(decrypted_password, user.password))
				nValid = 1;
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
				decrypt_algo(file_nums, y-1, MAIN_KEY, decrypted_filename);
				strcpy(filename, decrypted_filename);
				printf("\nEnter key: ");
				enterPass(temp_key);
				nContinue=1;
				fclose(common);	
			}
		}
		else{
			printf("\nNo account in the database. Please create an account.\n\nPress any key to continue...");
			getch();
			return 2;
		}
	}
	return 1;
}

int newAccount(char filename[STRING_SIZE], char temp_key[STRING_SIZE]){
	FILE *common;
	
	int nSelect, taken_username, taken_filename, nContinue=0;
	int line_count;
	struct credentials user;
	
	while(!nContinue){
		system("cls");
		passwordKeeperArt();
		common = fopen("common", "a+");
		taken_username=1, taken_filename=1; // Reset all values
		
		printf("\nEnter username: ");
		scanf(" %s", user.username);
		
		printf("Enter password: ");
		enterPass(user.password);
		
		printf("\nEnter file name: ");
		scanf("%s", filename);
		
		taken_username = check_same(user.username, "common", 1, MAIN_KEY, &line_count);
		taken_filename = check_same(filename, "common", 2, MAIN_KEY, &line_count);
		
		if(taken_username){
			system("cls");
			passwordKeeperArt();
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
		else if(taken_filename){
			system("cls");
			passwordKeeperArt();
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
		else{
			printf("Enter key: ");
			enterPass(temp_key);
			
			nContinue = 1;
			store_credentials(user.username, "common", MAIN_KEY, 1);
			store_credentials(user.password, "common", MAIN_KEY, 1);
			store_credentials(filename, "common", MAIN_KEY, 2);
		}
	}
	
	fclose(common);
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
	
	int current_num, account=1, username=0, password=0, num_count=0, count=0, first=1, i;
	int nums[STRING_SIZE];
	char decrypted[STRING_SIZE];
	
	system("cls");
	passwordKeeperArt();
	
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
		
		if(current_num == END_LINE){ // marker for end of specific credential line, refresh first
			count++;
			first=1;
			decrypt_algo(nums, num_count, key, decrypted);
			printf("%s", decrypted);
			i=0;
			num_count=0;
			}
		else if(current_num == END_GROUP){ // marker for end of account details, refresh first
			decrypt_algo(nums, num_count, key, decrypted);
			printf("%s", decrypted);
			printf("\n");
			count=0;
			first=1;
			i=0;
			num_count=0;
			}
		else
			num_count++;
		}
	}
	else
		printf("\nNo password has been created\n");
		
	printf("\nPress any key to continue...");
	getch();
	
	fclose(user_file);
}

void addPassword(char filename[STRING_SIZE], int key){
	FILE *user_file;
	user_file = fopen(filename, "a+");
	
	int user_line, unique_name=0;
	struct credentials user;
	
	system("cls");
	passwordKeeperArt();
	
	printf("\nEnter account name: ");
	scanf("%s", user.account_name);
	
	printf("Enter username: ");
	scanf("%s", user.username);

	printf("Enter password: ");
	enterPass(user.password);
	
	unique_name = check_same(user.account_name, filename, 1, key, &user_line);
	
	if(!unique_name){
		store_credentials(user.account_name, filename, key, 1);
		store_credentials(user.username, filename, key, 1);
		store_credentials(user.password, filename, key, 2);
	}
	else{
		printf("\nAccount name already exists.\n\nPress any key to continue...");
		getch();
	}
	
	fclose(user_file);
}

void changePassword(char filename[STRING_SIZE], int key , int which){
	FILE *user_file, *temp;
	
	system("cls");
	passwordKeeperArt();
	
	int current_number, new_password_length=0, account_line_count=0, line_count=0, done=0, valid_account_name=0, i;
	int nums[STRING_SIZE];
	char new_password[STRING_SIZE];
	struct credentials user;
	
	printf("\nEnter account name: ");
	scanf("%s", user.account_name);

	valid_account_name = check_same(user.account_name, filename, 1, key, &account_line_count);
	account_line_count += which;
	
	if(!valid_account_name){
		remove("temp");
		printf("\nInvalid account name\n\nPress any key to continue...");
	}
	else{
		printf("Enter new password: ");
		enterPass(new_password);
		new_password_length = strlen(new_password);
		encrypt_algo(new_password, nums, key);
		
		user_file = fopen(filename, "r");
		
		temp = fopen("temp", "w");
		if(user_file == NULL){
			printf("\nUser file not found. Please contact administrator\n\nPress any key to continue...");
			getch();
		}
		else{
			while(fscanf(user_file, "%d", &current_number) != EOF){
				if(current_number == END_LINE || current_number == END_GROUP){
						line_count++;
				}
		
				if(account_line_count == line_count){ // Password of inputted account name
					if(!done){
						fprintf(temp, "%d ", END_LINE);
						for(i=0; i<new_password_length; i++)
							fprintf(temp, "%d ", nums[i]);
						done=1;
					}
				}
				else
					fprintf(temp, "%d ", current_number);
			}
			fclose(user_file);
			fclose(temp);
			remove(filename);
			rename("temp", filename);
			
			printf("\n\nPassword successfuly changed!\n\nPress any key to continue...");
			getch();
		}
	}
}

void deletePassword(char filename[STRING_SIZE], int key){
	FILE *user_file, *temp;
	
	int current_number, account_line_count=0, valid_account_name=0, deleted=0, line_count=0;
	struct credentials user;
	
	system("cls");
	passwordKeeperArt();
	
	printf("\nEnter account name: ");
	scanf("%s", user.account_name);
	
	valid_account_name = check_same(user.account_name, filename, 1, key, &account_line_count);
	
	if(!valid_account_name){
		printf("Invalid account name\n\nPress any key to continue...");
		getch();
	}
	else{
		user_file = fopen(filename, "r");
		temp = fopen("temp", "w");
		if(user_file != NULL){
			while(fscanf(user_file, "%d", &current_number) != EOF){
				if(current_number == END_LINE || current_number == END_GROUP)
					line_count++;
				
				if(account_line_count-1 == line_count);
				else if(deleted){
					deleted=0;
					line_count++;
					fprintf(temp, "%d ", current_number);
				}
				else if(account_line_count == line_count); // Password of inputted account name
				else if(account_line_count+1 == line_count);
				else if(account_line_count+2 == line_count){
					deleted=1;
					if(account_line_count != 1)
						fprintf(temp, "%d ", END_GROUP);
				}
				else
					fprintf(temp, "%d ", current_number);
			}
			fclose(user_file);
			fclose(temp);
			remove(filename);
			rename("temp", filename);
			
			printf("\"%s\" has been deleted\n\nPress any key to continue...", user.account_name);
			getch();
			
			fclose(user_file);
		}
		else{
			printf("Error! Could not open file. Please contact administrator.\n\nPress any key to continue...");
			getch();	
		}
	}
}

void endScreen(){
	system("cls");
	passwordKeeperArt();
	printf("\nThank you for using Password Keeper!\n\nSee you next time!");
}
