#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define STRING_SIZE 256
#define MAX_LINE 256 
#define MAIN_KEY 214 // Programmer defined key for common file
#define END_LINE 1434 // End marker for each credential line
#define END_GROUP 14344 // End marker for each account in file

struct credentials;

/* Encryption algorithm:
Encrypts the user input and places its values in an array of integers

input = String that is provided by the user. This is what the user wants to be encrypted
nums = The array of encrypted integers from the input of the user
key = Key that is entered by the user (or the programmer i.e. common file) upon log in
*/
void encrypt_algo(char input[STRING_SIZE], int nums[STRING_SIZE], int key);

/* Decryption algorithm:
Decrypts an array of integer and provides the decrypted string in an array

nums = Array of integers that is provided by the program. This contains the list of encrypted numbers
nums_length = The number of items inside the array of integers (nums) provided previously
key = Key that is entered by the user (or the programmer i.e. common file) upon log in
decrypted = Array of characters that contains the decrypted values of the previously provided array of integers
*/
void decrypt_algo(int nums[STRING_SIZE], int nums_length, int key, char decrypted[STRING_SIZE]);

/* Check if similar:
Check if, and so where is the similar string that is inputted by the user

input = String that is provided by the user. This is what the user wants to search for similarities
filename = Name of the file that the user wants to search for similarities
key = Key that is entered by the user (or the programmer i.e. common file) upon log in
*line_count = Value of markers passed (i.e END_LINE || END_GROUP, *line_count += 1)

Return 0 = No similarities
Return 1 = Found similarities
*/
int check_same(char input[STRING_SIZE], char filename[STRING_SIZE], int key, int *line_count);

/* Store Credentials:
Prints the encrypted value of the inputs to a given file.

input = String that is provided by the user. This is what the user wants to store in the file
filename = Name of the file that the user wants to store in
key = Key that is entered by the user (or the programmer i.e. common file) upon log in
which = Stores a certain MARKER at the end of the stored input. 0 = nothing, 1 = END_LINE, 2 = END_GROUP
*/
void store_credentials(char input[STRING_SIZE], char filename[STRING_SIZE], int key, int which);

/* Login:
Prompts the user to input his/her common file login credentials

filename = After logging in, the filename of the user will be placed in this array
temp_key = After successfully logging in, the user will input their key as a string that will then be converted into an integer value

Return 1 = Prompt the user to Login again
Return 2 = Return to the Welcome Screen
Return 3 = Exit the program
*/
int login(char filename[STRING_SIZE], char temp_key[STRING_SIZE]);

/* Create New Account: 
Creates a new account that will be stored in the common file

filename = After creating an account, the filename of the user will be placed in this array
temp_key = After successfully creating an account, the user will input their key as a string that will then be converted into an integer value
*/
int newAccount(char filename[STRING_SIZE], char temp_key[STRING_SIZE]);

/* Main Menu:
Displays possible options that the program allows

Return 1 = displayCredentials
Return 2 = addPassword
Return 3 = changePassword
Return 4 = deletePassword
Return 5 = Change password of the user in the common file
Return 6 = Exit the program
*/
int mainMenu();

/* Display Credentials:
Decrypts and displays all the accounts located in the user's file

filename = The filename of the user
key = Key of the user
*/
void displayCredentials(char filename[STRING_SIZE], int key);

/* Add Password:
Creates a new account for a specific user that will be stored in their file

filename = The filename of the user
key = Key of the user
*/
void addPassword(char filename[STRING_SIZE], int key);

/* Change Password:
Changes a specific password of an account, or common file, of a user

filename = The filename of the user
key = Key of the user
which = selects between common file or user file | 0 = Common, 1 = User File
*/
void changePassword(char filename[STRING_SIZE], int key , int which);

/* Delete Password
Deletes a speciffic account in the file of the user

filename = The filename of the user
key = Key of the user
*/
void deletePassword(char filename[STRING_SIZE], int key);

/* Valid Input:
Asks the user for an input until their input is valid. Used for integer-navigated menus

1 - nMaxInput

nMaxInput = Maximum integer value of the menu
Returns the valid integer input of the user
*/
int validInput(int nMaxInput);

/* Enter Password:
(BUG) Backspace not working

As the user types the password, this will hide it using '*' instead of the text itself for additional security

password = The password that the user inputs will be stored in this character array.
*/
void enterPass(char passwrd[STRING_SIZE]); // Make password print '*' instead of the password itself for security

/* Key Generator:
Converts a string input into an integer value

temp_key = String that the user wants to be converted into an integer-valued key

Return integer value of the string
*/
int keyGen(char temp_key[STRING_SIZE]);

/* Password Keeper Art
Displays an ASCII banner of the program name
*/
void passwordKeeperArt();

/* Welcome Screen
First screen that will appear upon starting the program

Return 1 = Login
Return 2 = Create a new account
*/
int welcomeScreen();

/* End Screen:
Goodbye message before exiting the program
*/
void endScreen();

