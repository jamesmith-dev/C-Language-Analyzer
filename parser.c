#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ARRAY_SIZE 500
//HW3
//11/15/22
// to test in eustis: 
//										1) gcc parser.c
//										2) ./a.out token_basics.txt

// IGNORE BELOW IF instructions on 6 AND 7 WORK
// have tokens_basic.txt in directory
//./test_case_input_creator pl0_basic.txt
// if denied: chmod +x test_case_input_creator
// output to output file: ./test_case_input_creator sample_tokens > output.txt

typedef enum token_type { // token types
  identifier = 1,
  number,
  keyword_const,
  keyword_var,
  keyword_procedure,
  keyword_call,
  keyword_begin,
  keyword_end,
  keyword_if,
  keyword_then,
  keyword_else,
  keyword_while,
  keyword_do,
  keyword_read,
  keyword_write,
  keyword_def,
  period,
  assignment_symbol,
  minus,
  semicolon,
  left_curly_brace,
  right_curly_brace,
  equal_to,
  not_equal_to,
  less_than,
  less_than_or_equal_to,
  greater_than,
  greater_than_or_equal_to,
  plus,
  times,
  division,
  left_parenthesis,
  right_parenthesis
} token_type;

typedef enum opcode_name { // op codes
  LIT = 1,
  OPR = 2,
  LOD = 3,
  STO = 4,
  CAL = 5,
  INC = 6,
  JMP = 7,
  JPC = 8,
  SYS = 9,
  WRT = 1,
  RED = 2,
  HLT = 3,
  RTN = 0,
  ADD = 1,
  SUB = 2,
  MUL = 3,
  DIV = 4,
  EQL = 5,
  NEQ = 6,
  LSS = 7,
  LEQ = 8,
  GTR = 9,
  GEQ = 10
} opcode_name;

typedef struct lexeme { // lexeme codes
  token_type type;
  char identifier_name[12];
  int number_value;
  int error_type;
} lexeme;

typedef struct instruction { // instruction codes
  int op;
  int l;
  int m;
} instruction;

typedef struct symbol { // symbol struct for accessing characteristics
  int kind;
  char name[12];
  int value;
  int level;
  int address;
  int mark;
} symbol;

// indexes and other variables necessary to parse through input
lexeme *tokens;
int token_index = 0;
symbol *table;
int table_index = 0;
instruction *code;
int code_index = 0;
int error = 0;
int level;

// worker functions imperative to the functioning of this program

void emit(int op, int l, int m);
void add_symbol(int kind, char name[], int value, int level, int address);
void mark();
int multiple_declaration_check(char name[]);
int find_symbol(char name[], int kind);
void print_parser_error(int error_code, int case_code);
void print_assembly_code();
void print_symbol_table();
void program();
void block();
void constants();
int declarations();
void variables(int numVars);
void statement();
void procedures();
void factor();

int main(int argc, char *argv[]) {

  // variable setup
  int i;
  tokens = calloc(ARRAY_SIZE, sizeof(lexeme));
  table = calloc(ARRAY_SIZE, sizeof(symbol));
  code = calloc(ARRAY_SIZE, sizeof(instruction));
  FILE *ifp;
  int buffer;
  // read in input
  // assume no input file if less than two arguments and program makes it this
  // far
  if (argc < 2) {
    printf("Error : please include the file name\nExiting Program\n");
    return 1;
  }

  ifp = fopen(argv[1], "r"); // open file, save to ifp
  if (ifp == NULL) {         // check if input file is null
    printf("Invalid Input File\nExiting Program\n");
    return 1;
  }
  while (fscanf(ifp, "%d", &buffer) != EOF) // read in input
  {

    tokens[token_index].type = buffer;
    if (buffer == identifier)
      fscanf(ifp, "%s", tokens[token_index].identifier_name);
    else if (buffer == number)
      fscanf(ifp, "%d", &(tokens[token_index].number_value));
    token_index++;
  }
  fclose(ifp);
  token_index = 0;
  program(); // begin execution of program
  free(tokens);
  free(table);
  free(code);
  return 0;
}
// done, check after
void program() { // this single function call begins the execution of the
                 // program
  add_symbol(3, "main", 0, 0, 0);
  level = -1;
  // check parameters
  emit(JMP, 0, 0);
  block();
  if (error == -1) {
    return;
  }

  if (tokens[token_index].type != period) {
    print_parser_error(1, 0);
    error = -1;
    return;
  }

  for (int i = 0; i < code_index; i++) {
    if (code[i].op == 5) {
      code[i].m = table[code[i].m].address;
    }
  }

  code[0].m = table[0].address;
  // What is m supposed to be 0?
  emit(SYS, 0, HLT);
  print_assembly_code();
  print_symbol_table();
}

// DONE
void block() { // missing a lot?
  level++;
  int index = table_index - 1;
  int inc_m_value = declarations();
  if (error == -1) {
    return;
  }
  procedures();
  if (error == -1) {
    return;
  }
  // im not sure what gets multiplied by 3
  table[index].address = code_index * 3;
  emit(INC, 0, inc_m_value);
  statement();
  if (error == -1) {
    return;
  }
  mark();
  level--;
}

// DONE
int declarations() { // tracks declarations of items in input file
  int number_of_variables_declared = 0;
  while (tokens[token_index].type == keyword_const ||
         tokens[token_index].type == keyword_var) {
    if (tokens[token_index].type == keyword_const) {
      constants();
      if (error == -1) {
        return number_of_variables_declared;
      }
    } else if (tokens[token_index].type == keyword_var) {
      variables(number_of_variables_declared);
      if (error == -1) {
        return number_of_variables_declared;
      }
      number_of_variables_declared++;
    }
  }
  return number_of_variables_declared + 3;
}
// DONE
void constants() {
  int minus_flag = 0;
  token_index++;
  if (tokens[token_index].type != identifier) {
    print_parser_error(2, 1);
    error = -1;
    return;
  }
  if (multiple_declaration_check(tokens[token_index].identifier_name) != -1) {
    print_parser_error(3, 0);
    error = -1;
    return;
  }
  char identifierName[12];
  strcpy(identifierName, tokens[token_index].identifier_name);
  token_index++;
  if (tokens[token_index].type != assignment_symbol) {
    print_parser_error(4, 1);
    error = -1;
    return;
  }
  token_index++;
  if (tokens[token_index].type == minus) {
    minus_flag = 1;
    token_index++;
  }
  if (tokens[token_index].type != number) {
    print_parser_error(5, 0);
    error = -1;
    return;
  }
  int identNum = tokens[token_index].number_value;
  token_index++;
  if (minus_flag) {
    identNum *= -1;
  }
  add_symbol(1, identifierName, identNum, level, 0);
  if (tokens[token_index].type != semicolon) {
    print_parser_error(6, 1);
    error = -1;
    return;
  }
  token_index++;
}
// DONE
void variables(int numVars) { // tracks variables from input file and performs
                              // necessary operations
  token_index++;
  if (tokens[token_index].type != identifier) {
    print_parser_error(2, 2);
    error = -1;
    return;
  }
  if (multiple_declaration_check(tokens[token_index].identifier_name) != -1) {
    print_parser_error(3, 0);
    error = -1;
    return;
  }
  char identifierName[12];
  strcpy(identifierName, tokens[token_index].identifier_name);
  token_index++;
  add_symbol(2, identifierName, 0, level, numVars + 3);
  // check for semicolon, 6-2
  if (tokens[token_index].type != semicolon) {
    print_parser_error(6, 2);
    error = -1;
    return;
  }
  token_index++;
}
// DONE
void statement() { // checks syntax of input file
  int symbol_index_in_table = 0;

  if (tokens[token_index].type == keyword_def) {
    token_index++;

    if (tokens[token_index].type != identifier) {
      print_parser_error(2, 6);
      error = -1;
      return;
    }
    symbol_index_in_table = find_symbol(tokens[token_index].identifier_name, 2);
    if (symbol_index_in_table == -1) {
      if (find_symbol(tokens[token_index].identifier_name, 1) ==
          find_symbol(tokens[token_index].identifier_name, 3)) {
        print_parser_error(8, 1);
        error = -1;
        return;
      } else {
        print_parser_error(7, 0);
        error = -1;
        return;
      }
    }
    token_index++;
    if (tokens[token_index].type != assignment_symbol) {
      print_parser_error(4, 2);
      error = -1;
      return;
    }
    token_index++;
    factor();
    if (error == -1) {
      return;
    }
    // make sure is STO
    emit(STO, level - table[symbol_index_in_table].level,
         table[symbol_index_in_table].address);
  } else if (tokens[token_index].type == keyword_call) {
    token_index++;

    if (tokens[token_index].type != identifier) {
      print_parser_error(2, 4);
      error = -1;
      return;
    }
    symbol_index_in_table = find_symbol(tokens[token_index].identifier_name, 3);
    if (symbol_index_in_table == -1) {
      if (find_symbol(tokens[token_index].identifier_name, 1) ==
          find_symbol(tokens[token_index].identifier_name, 2)) {
        print_parser_error(8, 2);
        error = -1;
        return;
      } else {
        print_parser_error(9, 0);
        error = -1;
        return;
      }
    }
    token_index++;
    // make sure this is cal
    emit(CAL, level - table[symbol_index_in_table].level,
         symbol_index_in_table);
  } else if (tokens[token_index].type == keyword_begin) {
    do {
      token_index++;
      statement();
      if (error == -1) {
        return;
      }
    } while (tokens[token_index].type == semicolon);

    if (tokens[token_index].type != keyword_end) {
      if (tokens[token_index].type == identifier ||
          tokens[token_index].type == keyword_call ||
          tokens[token_index].type == keyword_begin ||
          tokens[token_index].type == keyword_read ||
          tokens[token_index].type == keyword_def) {
        print_parser_error(6, 3);
        error = -1;
        return;
      } else {
        print_parser_error(10, 0);
        error = -1;
        return;
      }
    }
    token_index++;
  } else if (tokens[token_index].type == keyword_read) {
    token_index++;
    if (tokens[token_index].type != identifier) {
      print_parser_error(2, 5);
      error = -1;
      return;
    }
    symbol_index_in_table = find_symbol(tokens[token_index].identifier_name, 2);
    if (symbol_index_in_table == -1) {
      if (find_symbol(tokens[token_index].identifier_name, 1) ==
          find_symbol(tokens[token_index].identifier_name, 3)) {
        print_parser_error(8, 3);
        error = -1;
        return;
      } else {
        print_parser_error(13, 0);
        error = -1;
        return;
      }
    }
    token_index++;
    // make sure this is red
    emit(SYS, 0, RED);
    emit(STO, level - table[symbol_index_in_table].level,
         table[symbol_index_in_table].address);
  }
}

// DONE
void factor() {
  int constant_index;
  int variable_index;

  if (tokens[token_index].type == identifier) {
    constant_index = find_symbol(tokens[token_index].identifier_name, 1);
    variable_index = find_symbol(tokens[token_index].identifier_name, 2);

    if (constant_index == variable_index) {
      if (find_symbol(tokens[token_index].identifier_name, 3) != -1) {
        print_parser_error(17, 0);
        error = -1;
        return;
      } else {
        print_parser_error(8, 4);
        error = -1;
        return;
      }
    }
    if (constant_index == -1) {
      // LOD
      emit(LOD, level - table[table_index].level, table[table_index].address);
    } else if (variable_index == -1) {
      // LIT
      emit(LIT, 0, table[constant_index].value);
    } else if (table[constant_index].level > table[variable_index].level) {
      emit(LIT, 0, table[constant_index].value);
    } else {
      // LOD
      emit(LOD, level - table[variable_index].level,
           table[variable_index].address);
    }
    token_index++;
  } else if (tokens[token_index].type == number) {
    emit(LIT, 0, tokens[token_index].number_value);
    token_index++;
  } else {
    print_parser_error(19, 0);
    error = -1;
    return;
  }
}
// DONE
void procedures() { //
  char identifierName[12];
  while (tokens[token_index].type == keyword_procedure) {
    token_index++;
    if (tokens[token_index].type != identifier) {
      print_parser_error(2, 3);
      error = -1;
      return;
    }
    if (multiple_declaration_check(tokens[token_index].identifier_name) != -1) {
      print_parser_error(3, 0);
      error = -1;
      return;
    }

    strcpy(identifierName, tokens[token_index].identifier_name);
    token_index++;

    add_symbol(3, identifierName, 0, level, 0);
    if (tokens[token_index].type != left_curly_brace) {
      print_parser_error(14, 0);
      error = -1;
      return;
    }
    token_index++;
    block();
    if (error == -1) {
      return;
    }
    // im not sure what the parameters are
    emit(OPR, 0, RTN);
    if (tokens[token_index].type != right_curly_brace) {
      print_parser_error(15, 0);
      error = -1;
      return;
    }
    token_index++;
  }
}
// adds a new instruction to the end of the code
void emit(int op, int l, int m) {
  code[code_index].op = op;
  code[code_index].l = l;
  code[code_index].m = m;
  code_index++;
}
// adds a new symbol to the end of the table
void add_symbol(int kind, char name[], int value, int level, int address) {
  table[table_index].kind = kind;
  strcpy(table[table_index].name, name);
  table[table_index].value = value;
  table[table_index].level = level;
  table[table_index].address = address;
  table[table_index].mark = 0;
  table_index++;
}
// marks all of the current procedure's symbols
void mark() {
  int i;
  for (i = table_index - 1; i >= 0; i--) {
    if (table[i].mark == 1)
      continue;
    if (table[i].level < level)
      return;
    table[i].mark = 1;
  }
}
// returns -1 if there are no other symbols with the same name within this
// procedure
int multiple_declaration_check(
    char name[]) // checks for repeats/redundancies and incorrect syntax under
                 // certain conditions
{
  int i;
  for (i = 0; i < table_index; i++)
    if (table[i].mark == 0 && table[i].level == level &&
        strcmp(name, table[i].name) == 0)
      return i;
  return -1;
}
// returns the index of the symbol with the desired name and kind, prioritizing
//  symbols with level closer to the current level
int find_symbol(char name[], int kind) {
  int i;
  int max_idx = -1;
  int max_lvl = -1;
  for (i = 0; i < table_index; i++) {
    if (table[i].mark == 0 && table[i].kind == kind &&
        strcmp(name, table[i].name) == 0) {
      if (max_idx == -1 || table[i].level > max_lvl) {
        max_idx = i;
        max_lvl = table[i].level;
      }
    }
  }
  return max_idx;
}
void print_parser_error(int error_code,
                        int case_code) // parses for errors, if one is caught,
                                       // program will state which one is caught
{
  switch (error_code) // case switches for error code start here, given type of
                      // error code, will switch to respective switch
  {
  case 1:
    printf("Parser Error 1: missing . \n");
    break;
  case 2:
    switch (case_code) // case switches for case code, given type of case code,
                       // will switch to respective one
    {
    case 1:
      printf("Parser Error 2: missing identifier after keyword const\n");
      break;
    case 2:
      printf("Parser Error 2: missing identifier after keyword var\n");
      break;
    case 3:
      printf("Parser Error 2: missing identifier after keyword procedure\n");
      break;
    case 4:
      printf("Parser Error 2: missing identifier after keyword call\n");
      break;
    case 5:
      printf("Parser Error 2: missing identifier after keyword read\n");
      break;
    case 6:
      printf("Parser Error 2: missing identifier after keyword def\n");
      break;
    default:
      printf("Implementation Error: unrecognized error code\n");
    }
    break;
  case 3:
    printf("Parser Error 3: identifier is declared multiple times by a "
           "procedure\n");
    break;
  case 4:
    switch (case_code) {
    case 1:
      printf("Parser Error 4: missing := in constant declaration\n");
      break;
    case 2:
      printf("Parser Error 4: missing := in assignment statement\n");
      break;
    default:
      printf("Implementation Error: unrecognized error code\n");
    }
    break;
  case 5:
    printf("Parser Error 5: missing number in constant declaration\n");
    break;
  case 6:
    switch (case_code) {
    case 1:
      printf("Parser Error 6: missing ; after constant declaration\n");
      break;
    case 2:
      printf("Parser Error 6: missing ; after variable declaration\n");
      break;
    case 3:
      printf("Parser Error 6: missing ; after statement in begin-end\n");
      break;
    default:
      printf("Implementation Error: unrecognized error code\n");
    }
    break;
  case 7:
    printf("Parser Error 7: procedures and constants cannot be assigned to\n");
    break;
  case 8:
    switch (case_code) {
    case 1:
      printf("Parser Error 8: undeclared identifier used in assignment "
             "statement\n");
      break;
    case 2:
      printf("Parser Error 8: undeclared identifier used in call statement\n");
      break;
    case 3:
      printf("Parser Error 8: undeclared identifier used in read statement\n");
      break;
    case 4:
      printf("Parser Error 8: undeclared identifier used in arithmetic "
             "expression\n");
      break;
    default:
      printf("Implementation Error: unrecognized error code\n");
    }
    break;
  case 9:
    printf("Parser Error 9: variables and constants cannot be called\n");
    break;
  case 10:
    printf("Parser Error 10: begin must be followed by end\n");
    break;
  case 11:
    printf("Parser Error 11: if must be followed by then\n");
    break;
  case 12:
    printf("Parser Error 12: while must be followed by do\n");
    break;
  case 13:
    printf("Parser Error 13: procedures and constants cannot be read\n");
    break;
  case 14:
    printf("Parser Error 14: missing {\n");
    break;
  case 15:
    printf("Parser Error 15: { must be followed by }\n");
    break;
  case 16:
    printf("Parser Error 16: missing relational operator\n");
    break;
  case 17:
    printf("Parser Error 17: procedures cannot be used in arithmetic\n");
    break;
  case 18:
    printf("Parser Error 18: ( must be followed by )\n");
    break;
  case 19:
    printf("Parser Error 19: invalid expression\n");
    break;
  default:
    printf("Implementation Error: unrecognized error code\n");
  }
}
void print_assembly_code() // prints out assembly code for respective input,
                           // given certain parameters
{
  int i;
  printf("Assembly Code:\n");
  printf("Line\tOP Code\tOP Name\tL\tM\n");
  for (i = 0; i < code_index; i++) {
    printf("%d\t%d\t", i, code[i].op);
    switch (code[i].op) {
    case LIT:
      printf("LIT\t");
      break;
    case OPR:
      switch (code[i].m) {
      case RTN:
        printf("RTN\t");
        break;
      case ADD: // DO NOT ATTEMPT TO IMPLEMENT THIS, YOU
        // WILL GET A ZERO IF YOU DO
        printf("ADD\t");
        break;
      case SUB: // DO NOT ATTEMPT TO IMPLEMENT THIS, YOU
        // WILL GET A ZERO IF YOU DO
        printf("SUB\t");
        break;
      case MUL: // DO NOT ATTEMPT TO IMPLEMENT THIS, YOU
        // WILL GET A ZERO IF YOU DO
        printf("MUL\t");
        break;
      case DIV: // DO NOT ATTEMPT TO IMPLEMENT THIS, YOU
        // WILL GET A ZERO IF YOU DO
        printf("DIV\t");
        break;
      case EQL: // DO NOT ATTEMPT TO IMPLEMENT THIS, YOU
        // WILL GET A ZERO IF YOU DO
        printf("EQL\t");
        break;
      case NEQ: // DO NOT ATTEMPT TO IMPLEMENT THIS, YOU
        // WILL GET A ZERO IF YOU DO
        printf("NEQ\t");
        break;
      case LSS: // DO NOT ATTEMPT TO IMPLEMENT THIS, YOU
        // WILL GET A ZERO IF YOU DO
        printf("LSS\t");
        break;
      case LEQ: // DO NOT ATTEMPT TO IMPLEMENT THIS, YOU
        // WILL GET A ZERO IF YOU DO
        printf("LEQ\t");
        break;
      case GTR: // DO NOT ATTEMPT TO IMPLEMENT THIS, YOU
        // WILL GET A ZERO IF YOU DO
        printf("GTR\t");
        break;
      case GEQ: // DO NOT ATTEMPT TO IMPLEMENT THIS, YOU
        // WILL GET A ZERO IF YOU DO
        printf("GEQ\t");
        break;
      default:
        printf("err\t");
        break;
      }
      break;
    case LOD:
      printf("LOD\t");
      break;
    case STO:
      printf("STO\t");
      break;
    case CAL:
      printf("CAL\t");
      break;
    case INC:
      printf("INC\t");
      break;
    case JMP:
      printf("JMP\t");
      break;
    case JPC: // DO NOT ATTEMPT TO IMPLEMENT THIS, YOU WILL GET A
              // ZERO IF YOU DO
      printf("JPC\t");
      break;
    case SYS:
      switch (code[i].m) {
      case WRT: // DO NOT ATTEMPT TO IMPLEMENT THIS, YOU
                // WILL GET A ZERO IF YOU DO
        printf("WRT\t");
        break;
      case RED:
        printf("RED\t");
        break;
      case HLT:
        printf("HLT\t");
        break;
      default:
        printf("err\t");
        break;
      }
      break;
    default:
      printf("err\t");
      break;
    }
    printf("%d\t%d\n", code[i].l, code[i].m);
  }
  printf("\n");
}
void print_symbol_table() // prints symbol table when called by program
{
  int i;
  printf("Symbol Table:\n");
  printf("Kind | Name        | Value | Level | Address | Mark\n");
  printf("---------------------------------------------------\n");
  for (i = 0; i < table_index; i++)
    printf("%4d | %11s | %5d | %5d | %5d | %5d\n", table[i].kind, table[i].name,
           table[i].value, table[i].level, table[i].address, table[i].mark);
  printf("\n");
}