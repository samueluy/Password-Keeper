#include <stdio.h>

#include "HEADER.h"
#include "FUNCTIONS.c"

int main(void){
	int nInput, nSuccess=2, key;
	char filename[STRING_SIZE], temp_key[STRING_SIZE];
	
	while(nSuccess == 2){
		nInput = welcomeScreen();
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
		nInput = mainMenu();
		switch (nInput){
			case 1:
				displayCredentials(filename, key);
				break;
			case 2:
				addPassword(filename, key);
				break;
			case 3:
				changePassword(filename, key, 1);
				break;
			case 4:
				deletePassword(filename, key);
				break;
			case 5:
				changePassword("common", MAIN_KEY, 0);
				break;
				
			case 6: break;
		}
	}
	
	endScreen();
	
	return 0;
}
