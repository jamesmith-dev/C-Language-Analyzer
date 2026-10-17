// use buffer to temporarily contain chars from input file
// use fgetc!!
//to compile: gcc main.c
//to run: ./a.out "input_file".txt
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *ifp;
int temp;
char identiferLengthError[] = "Lexical Analyzer Error: maximum identifier length is 11";
char numberLengthError[] = "Lexical Analyzer Error: maximum number length is 5";
char digitIdentifierError[] = "Lexical Analyzer Error: identifiers cannot begin with digits";
char invalidSymbolError[] = "Lexical Analyzer Error: invalid symbol";
char invalidIdentifierError[] = "Lexical Analyzer Error: identifiers cannot be named 'null' or 'main' ";

void parseNum(char c, char *buffer);
int keywordNum(char *buffer);
void parseWord(char *c, char *buffer);
void parseSymbol(char c);

typedef enum token_type {
  identifier = 1,                // identifier
  number = 2,                    // number
  keyword_const = 3,             // const
  keyword_var = 4,               // var
  keyword_procedure = 5,         // procedure
  keyword_call = 6,              // call
  keyword_begin = 7,             // begin
  keyword_end = 8,               // end
  keyword_if = 9,                // if
  keyword_then = 10,             // then
  keyword_else = 11,             // else
  keyword_while = 12,            // while
  keyword_do = 13,               // do
  keyword_read = 14,             // read
  keyword_write = 15,            // write
  keyword_def = 16,              // def
  period = 17,                   // .
  assignment_symbol = 18,        // :=
  minus = 19,                    // -
  semicolon = 20,                // ;
  left_curly_brace = 21,         // {
  right_curly_brace = 22,        // }
  equal_to = 23,                 // ==
  not_equal_to = 24,             // <>
  less_than = 25,                // <
  less_than_or_equal_to = 26,    // <=
  greater_than = 27,             // >
  greater_than_or_equal_to = 28, // >=
  plus = 29,                     // +
  times = 30,                    // *
  division = 31,                 // /
  left_parenthesis = 32,         // (
  right_parenthesis = 33,        // )
} token_type;

void parseNum(char c, char *buffer){
  int i = 0;
  //only goes in if it is a number or a character
	
	while (isdigit(c) || isalpha(c)){
    //if i>4 then one of the constraints is broken	
    if((i == 1) && isalpha(c)){
				printf("%s\n", digitIdentifierError);
					while ((isalpha(c) || isdigit(c))){
      			c = fgetc(ifp);
      		}	  
				fseek(ifp, -1L, SEEK_CUR);
				return;
			}
		
		if(i > 4){
			
			printf("%s\n", numberLengthError);
			while ((isalpha(c) || isdigit(c))){
      	c = fgetc(ifp);
      }	  
			fseek(ifp, -1L, SEEK_CUR);
			return;		
		}	
		
			buffer[i]=c;
			i++;
		c = fgetc(ifp);
   //  	if(isalpha(c = fgetc(ifp))){
			// 	if((strcmp(buffer, "main") == 0) || (strcmp(buffer, "null") == 0)){
			// 		printf("%s\n",invalidIdentifierError);
			// 		return;
			// 	}
			// 	printf("%*s\t%d\n", 12, buffer, 1);
   //    	return;
				
			// }
    	
			
  }	
	fseek(ifp, -1L, SEEK_CUR);
  buffer[i]='\0'; 
	
  printf("%*s\t%d\n", 12, buffer,2);
}

int keywordNum(char *buffer) {
	//mapping all of the values to the reserve words they represent
  if (strcmp(buffer, "const") == 0)
    return keyword_const;

  else if (strcmp(buffer, "var") == 0)
    return keyword_var;

  else if (strcmp(buffer, "procedure") == 0)
    return keyword_procedure;

  else if (strcmp(buffer, "call") == 0)
    return keyword_call;

  else if (strcmp(buffer, "begin") == 0)
    return keyword_begin;

  else if (strcmp(buffer, "end") == 0)
    return keyword_end;

  else if (strcmp(buffer, "if") == 0)
    return keyword_if;

  else if (strcmp(buffer, "then") == 0)
    return keyword_then;

  else if (strcmp(buffer, "else") == 0)
    return keyword_else;

  else if (strcmp(buffer, "while") == 0)
    return keyword_while;

  else if (strcmp(buffer, "do") == 0)
    return keyword_do;

  else if (strcmp(buffer, "read") == 0)
    return keyword_read;

  else if (strcmp(buffer, "write") == 0)
    return keyword_write;

  else if (strcmp(buffer, "def") == 0)
    return keyword_def;

  else
    return identifier;
}
void parseWord(char *c, char *buffer) {
  int i = 0;
  //this makes sure char c is either a char or a number
  while (isalpha(*c) || isdigit(*c)) {
		if(i == 0 && isdigit(*c)){
			printf("%s\n",digitIdentifierError);
			while (isalpha(*c) || isdigit(*c)) {
       *c = fgetc(ifp);
      }
			return;
		}
    //checks if any identifiers is greater than 11 which breaks the constraint 
		if (i > 11) {
			printf("%s\n",identiferLengthError);
			while (isalpha(*c) || isdigit(*c)) {
       *c = fgetc(ifp);
      }
      return;
    }	
    buffer[i] = *c;
    *c = fgetc(ifp);
    i++;			
  }
	buffer[i] = '\0';
	temp = keywordNum(buffer);
	if((strcmp(buffer, "main") == 0) || (strcmp(buffer, "null") == 0)){
		printf("%s\n",invalidIdentifierError);
		return;
	}
  //this switch checks for the reserve words and prints out the corresponding number associated with them
	switch(temp){
		
		case 3:
			printf("%*s\t%d\n", 12, buffer, 3);
			return;
    case 4:
			printf("%*s\t%d\n", 12, buffer, 4);
			return;
    case 5:
			printf("%*s\t%d\n", 12, buffer, 5);
			return;
    case 6:
			printf("%*s\t%d\n", 12, buffer, 6);
			return;
    case 7:
			printf("%*s\t%d\n", 12, buffer, 7);
			return;
    case 8:
			printf("%*s\t%d\n", 12, buffer, 8);
			return;
    case 9:
			printf("%*s\t%d\n", 12, buffer, 9);
			return;
    case 10:
			printf("%*s\t%d\n", 12, buffer, 10);
			return;
    case 11:
			printf("%*s\t%d\n", 12, buffer, 11);
			return;
    case 12:
			printf("%*s\t%d\n", 12, buffer, 12);
			return;
    case 13:
			printf("%*s\t%d\n", 12, buffer, 13);
			return;
    case 14:
			printf("%*s\t%d\n", 12, buffer, 14);
			return;
    case 15:
			printf("%*s\t%d\n", 12, buffer, 15);
			return;
    case 16:
			printf("%*s\t%d\n", 12, buffer, 16);
			return;
		default:
			printf("%*s\t%d\n", 12, buffer, 1);
      return;		
	}
}
//this function looks at all of the symbols and prints out the symbols and the number the represent	
void parseSymbol(char c){
  switch(c)
  {
    case'.':
      printf("%*c\t%d\n",12, '.',17);
			return;
    case':':
			if((fgetc(ifp) == '=')){
      	printf("%*s\t%d\n",12,  ":=",18);
        return;
				}
			else {
				printf("%s\n", invalidSymbolError);
				fseek(ifp, -1L, SEEK_CUR);
				return;
			}
    case'-':
        printf("%*c\t%d\n",12, '-',19);
        return;
    case';':
        printf("%*c\t%d\n",12, ';',20);
        return;
    case'{':
        printf("%*c\t%d\n",12, '{',21);
        return;
    case'}':
        printf("%*c\t%d\n",12, '}',22);
        return;
    case'=':
        if((fgetc(ifp) == '=')){
        	printf("%*s\t%d\n",12, "==",23);
					return;
        }
				else{
					fseek(ifp, -1L, SEEK_CUR);
					return;
				}
    case'<':
      c = fgetc(ifp);
        if(c == '>'){
        	printf("%*s\t%d\n",12, "<>",24);
          return;
        }
      	else if(c == '='){
          printf("%*s\t%d\n",12, "<=",26);
					return;
        }
				else{
					fseek(ifp, -1L, SEEK_CUR);
					printf("%*s\t%d\n",12, "<",25);
					return;
				}		
		case'>':
			if((fgetc(ifp) == '=')){
        	printf("%*s\t%d\n",12, ">=",28);
        	return;
        }
      else {
				fseek(ifp, -1, SEEK_CUR);
        	printf("%*s\t%d\n",12, ">",27);
          return;
      }
    case'+':
      printf("%*s\t%d\n",12, "+",29);
        return;
    case'*':
      printf("%*s\t%d\n",12, "*",30);
      return;
    case'/':
      printf("%*s\t%d\n",12, "=",31);
      return;
    case'(':
      printf("%*s\t%d\n",12, "=",32);
      return;
    case')':
      printf("%*s\t%d\n",12, "=",33);
      return;
		case' ': 
			return;
		case'#':
			while((fgetc(ifp)!=('\n')) && (fgetc(ifp)!=EOF)){
				;
			}
			return;
		case'\n':
			return;
		case'\t':
			return;
		case'\0':
			return;
		case EOF:
			return;
		default:
			printf("%s\n",invalidSymbolError);
			return;
  }
}
int main(int argc, char const *argv[]) {
  ifp = fopen(argv[1], "r");
  int i = 0;
	//declare buffer in while loop below and reset it?
  char buffer[12];
  char c;
  if (ifp == NULL) {
    printf("Error Reading File\n");
    return 1;
  }
	while ((c = fgetc(ifp)) != EOF) {
    //check for only chars in the file
    if (isalpha(c)) {
      parseWord(&c, buffer);
      }
    //check for digits in the file
    if(isdigit(c)){
      parseNum(c,buffer);
    }
      //check for symbols in the file
		if(!isdigit(c) && !isalpha(c)){
			parseSymbol(c);
		} 
  }	
  //printf("%c", c);// debugging print
	return 0;
}

