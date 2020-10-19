#include <iostream>
#include <string>
#include <fstream>
#include <ctype.h>
using namespace std;

/*The first pass constructs a symbol table in which every line number, 
variable name, and constant of the Plain program is stored with its type
and corresponding location in the final IML code*/

/*
Symbol table receives: line numbers (only if it is first token in a statement), variables, constants
Symbole table does not receive: operators, commands
*/

/*
TableEntry members: 
symbol->indicates the line number, literal value, or name of a variable
type->indicates the symbol's type (C->constant literal val, L->line #, V->variable)
location->contains memory address for which symbol occurs
*/

/*
note: the table entry has a string to hold the symbol, but the symbol can be a numerical value or a string, if needed we can use the type
to parse it into an integer for later usage using an if probably
*/

/*
global variables 
RW
*/
int nextToken;
int token;
int characterClass;
int lexemeLength; //not sure if this is needed bc of using a mutable string
char nextCharacter;
ifstream codeFile; //idk if this is needed anymore
string lexeme;
int lineNumberAddr = 0;
int constVarAddr = 99;
int symbolTableIndex = 0;
FILE *fp;
    
    


/*
token codes 
RW
*/
#define INT_LIT 5
#define IDENT 10
#define INPUT_OP 15
#define OUTPUT_OP 16
#define IF 20
#define GOTO_OP 21
#define LINE_REF 22
#define ASSIGN_KW 25
#define ASSIGN_OP 29
#define EQUAL 30
#define LT 31
#define GTE 32
#define ADD_OP 35
#define SUB_OP 36
#define MULT_OP 37
#define DIV_OP 38
#define LEFT_PAREN 41
#define RIGHT_PAREN 42 
#define LINE_NUM 45
#define COMMENT 46
#define PROG_END_OP 99

/*
character classes 
RW
*/
#define LETTER 0
#define UNDERSCORE 1
#define DIGIT 2
#define COMP 3
#define NEWLINE 4 //probably not needed
#define UNKNOWN 5

/*
function declarations
RW
*/
void getCharacter();
void getNonWhiteSpace();
void addCharacter();
void clearLexeme();
int characterLookup(char character);
bool containsInteger(); //could be gotten rid of if the lex function is modified
int lex();
void updateSymbolTable();

/*
class declarations/definitions
INITIALS
*/
class TableEntry{
    private:
        string symbol; //not sure how this can be a string or int right now or if we should save line number as a string
        char type;
        int location;
    public:
        void setSymbol(string symb){
            symbol = symb;
        }
        void setType(char ty){
            type = ty;
        }
        void setLocation(int loc){
            location = loc;
        }
        int getLocation(){
            return location;
        }
        char getType(){
            return type;
        }
        string getSymbol(){
            return symbol;
        }
};
TableEntry symbolTable[200]; //creates an array of 200 TableEntry objects, we will need more though, consider changing to an array of pointers

/*
main driver class
RW + BF
*/
int main() {
    int wait; //int to take as input before closing out

    
    //file streams for inputting code and outputting result
    //ifstream codeFile("PlainCode.txt"); //hopefully this is global
    fp = fopen("PlainCode.txt","r");
    

    //try catch or equivalent to open the code file maybe

    //somehow need to figure out how to know if a number is the line number (at the start of a line) 
    //or an integer literal or if its a line number with a goto (first part of lex function)
    
    

    //get line number before loop since it will always be the first thing encountered
    //getNonWhiteSpace();
    //fix how line numbers are handled and symbol table
    
    //gets initial line number
    do{
        getCharacter();
        addCharacter();
    } while(nextCharacter != ' ');
    nextToken = LINE_NUM;
    cout << "Next token is: " << nextToken << " Next lexeme is: " << lexeme << "\n";
    updateSymbolTable();
    //somehow add it as a token and update the symbol table by calling function

    //loops over entire IML code file
    getCharacter();
    do{
        lex(); //not finished

        /*
        If a token is an operator or a Plain command, it does not go into the symbol table.  
        If a token is a line number, a variable, or a constant, it is placed in the symbol table.  
        */
        //add some sort of check to see if it is one of the things that belongs and throw some if statement in
        //to call the update symbol table function
        if(nextToken == INT_LIT || nextToken == IDENT || nextToken == LINE_NUM){
            updateSymbolTable();
        }

        //updateSymbolTable(); //not finished
    } while(nextToken != -1);//unsure of condition
    int loc0 = symbolTable[1].getLocation();
    char char0 = symbolTable[1].getType();
    string symbol0 = symbolTable[1].getSymbol();
    //shows symbol table
    cout << symbolTableIndex << "\n";
    int i;
    for(i = 0; i < symbolTableIndex; i++){
        cout << "Symbol: " << symbolTable[i].getSymbol() << "\n\t";
        cout << "Type: " << symbolTable[i].getType() << "\n\t";
        cout << "Location: " << symbolTable[i].getLocation() << "\n";
    }



    cin >> wait; //accepts the final input and ends program
}

////////////////////////////////////////////////////////////////////////////////////////////////
//function definitions//
////////////////////////////////////////////////////////////////////////////////////////////////

/*
gets the next character from the input file and determines its class
RW + BF
*/
void getCharacter(){
    //codeFile.get(nextCharacter);
    //nextCharacter = getc(codeFile);
    nextCharacter = getc(fp);

    //cout << nextCharacter;
    if(nextCharacter != EOF){
        //if the character is not the end of file signifier, identify the type

        if(isalpha(nextCharacter)){
            characterClass = LETTER;
        } else if(isdigit((unsigned char)nextCharacter)){
            characterClass = DIGIT;
        } else if(nextCharacter == '=' || nextCharacter == '<' || nextCharacter == '<'){
            characterClass = COMP;
        } else if(nextCharacter == '_'){
             characterClass = UNDERSCORE;
        } else{
            characterClass = UNKNOWN;
            //cout << "unkown";
        }
    } else{
        characterClass = EOF;
    }
}

/*
gets non whitespace character
RW
*/
void getNonWhiteSpace(){
    while(isspace(nextCharacter)){
        getCharacter();
    }
}

/*
adds character to the lexeme
RW
*/
void addCharacter(){
    lexeme += nextCharacter;
}

/*
clears the lexeme string when moving on to a new lexeme
RW
*/
void clearLexeme(){
    lexeme.clear();
}

/*
looks up a character of an unknown class, adds it to the lexeme, and updates the token
RW
*/
int characterLookup(char character){
    switch(character){
        //addCharacter();
        case '(':
            nextToken = LEFT_PAREN;
            break;
        case ')':
            nextToken = RIGHT_PAREN;
            break;
        case '+':
            nextToken = ADD_OP;
            break;
        case '-':
            nextToken = SUB_OP;
            break;
        case '*':
            nextToken = MULT_OP;
            break;
        case '/':
            nextToken = DIV_OP;
            break;
        default:
            nextToken = EOF;
    }
    return nextToken;
}

/*
checks if the lexeme contains any intergers or just letters
could probably be removed with correct modifications to lex
RW
*/
bool containsInteger(){
    string one = "1";
    string two = "2";
    string three = "3";
    string four = "4";
    string five = "5";
    string six = "6";
    string seven = "7";
    string eight = "8";
    string nine = "9";
    string zero = "0";
    
    size_t found1 = lexeme.find(one);
    size_t found2 = lexeme.find(two);
    size_t found3 = lexeme.find(three);
    size_t found4 = lexeme.find(four);
    size_t found5 = lexeme.find(five);
    size_t found6 = lexeme.find(six);
    size_t found7 = lexeme.find(seven);
    size_t found8 = lexeme.find(eight);
    size_t found9 = lexeme.find(nine);
    size_t found0 = lexeme.find(zero);

    if(found1 != string::npos || found2 != string::npos || found3 != string::npos || found4 != string::npos ||
        found5 != string::npos || found6 != string::npos || found7 != string::npos || found8 != string::npos ||
        found9 != string::npos || found0 != string::npos){
            //there was an integer found in the string, so it is an identifier
            return true;
        } else{
            return false;
        }
}

/*
lexical analyzer
RW + BF
*/
int lex(){
    //somehow check here if a newline is encountered and get the line numbers immediately and end lex
    //if(nextCharacter == 0x0a) then do something
    
    if(nextCharacter == 0x0a){
        getNonWhiteSpace();
        nextToken = LINE_NUM;
        clearLexeme();
        do{
            addCharacter();
            getCharacter();
        } while(isdigit(nextCharacter));
        
        
        //updates symbol table for the newline
        //updateSymbolTable();



    } else{
        clearLexeme();
        
        getNonWhiteSpace();
        
        switch(characterClass){
            /*parses identifiers, operations, and if statements*/
            case LETTER:
               do{
                    //continuosuly adds characters to the lexeme, which is an identifier or operation since it began with a letter
                    addCharacter();
                    getCharacter();
                }while(characterClass == LETTER || characterClass == DIGIT || characterClass == UNDERSCORE);
                //if to determine if it is a function or identifier or if statement
                if(containsInteger()){
                    nextToken = IDENT; //could probably be eliminated and just check for predetermined function first, then eliminate contains int function
                } else{
                    //check if it is a predefined function
                    if(lexeme.compare("comment") == 0){
                        //somehow skip the line
                        do{
                            getCharacter();
                        } while(nextCharacter != 0x0a);
                        nextToken = COMMENT;
                        break;

                    } else if(lexeme.compare("input") == 0){
                        //the lexeme is the input function
                        nextToken = INPUT_OP;

                    } else if(lexeme.compare("output") == 0){
                        //the lexeme is the output function
                        nextToken = OUTPUT_OP;

                    } else if(lexeme.compare("if") == 0){
                        //the lexeme is the if statement
                        nextToken = IF;

                    } else if(lexeme.compare("goto") == 0){
                        //the lexeme is the goto funtion
                        nextToken = GOTO_OP;
                        cout << "Next token is: " << nextToken << " Next lexeme is: " << lexeme << "\n";
                        clearLexeme();
                        //also go to the end of the line
                        //line number always follows goto and that line number is not needed for symbol table
                        getCharacter();
                        do{
                            addCharacter();
                            getCharacter();
                        } while(nextCharacter != 0x0a);
                        nextToken = LINE_REF;

                    } else if(lexeme.compare("prog_end") == 0){
                        //the lexeme is the program end statement
                        nextToken = PROG_END_OP;

                    } else if(lexeme.compare("assign") == 0){
                        //the lexeme is the assignment operation
                        nextToken = ASSIGN_KW;

                    } else{
                        //if it starts with a letter, has no numbers, and isnt a predefined function it is an identifier
                        nextToken = IDENT;
                    }

                }
            
                break;
            /*parses integer literals*/
            case DIGIT:
                do{
                    //continuously adds digits to the lexeme, which is an integer literal
                    addCharacter();
                    getCharacter();
                } while(characterClass == DIGIT);
                nextToken = INT_LIT;
                break;
        
            /*parses assignment and comparison operators*/
            case COMP:
                do{
                    addCharacter();
                    getCharacter();
                }while(characterClass == COMP);
                
                //if to determine which is is
                if(lexeme.compare("==") == 0){
                    //the lexeme is the equal to comparison
                    nextToken = EQUAL;
                    break;

                } else if(lexeme.compare("=") == 0){
                    //the lexeme is the assignment operator
                    nextToken = ASSIGN_OP;
                    break;

                } else if(lexeme.compare("<") == 0){
                    //the lexeme is the less than comparison
                    nextToken = LT;
                    break;

                } else{
                    //the lexeme is the greater than or equal to comparison
                    nextToken = GTE;
                    break;

                }


            /*parses arithemetic operators and parentheses*/
            case UNKNOWN:
                //this may need to be able to recognie a string of parentheses like (( or )) but it may already do that idk

                characterLookup(nextCharacter); //this function adds the character to the lexeme and updates nextToken
                addCharacter();
                getCharacter();
                if(nextToken == -1){
                    lexeme = "EOF";
                }
                break;
            /*End of File*/
            case EOF:
                //not quite sure of why this is needed if there isnt a character class called eof, but the packet code has it so here it is
                //additionally characterLookup has a case for EOF
                nextToken = EOF;
                clearLexeme();
                lexeme = "EOF";
                break;
            
        } //end switch
    }
    
    //output will need to be modified
    cout << "Next token is: " << nextToken << " Next lexeme is: " << lexeme << "\n";

    return nextToken;
}

/*
updates the symbol table
RW
*/
void updateSymbolTable(){
    if((nextToken == INT_LIT || nextToken == IDENT || nextToken == LINE_NUM) && constVarAddr > lineNumberAddr){
        TableEntry curr = symbolTable[symbolTableIndex];
        switch(nextToken){
            case INT_LIT:
                curr.setType('C');
                curr.setLocation(constVarAddr);
                curr.setSymbol(lexeme);
                constVarAddr--;
                break;
            case IDENT:
                curr.setType('V');
                curr.setLocation(constVarAddr);
                curr.setSymbol(lexeme);
                constVarAddr--;
                break;
            case LINE_NUM:
                curr.setType('L');
                curr.setLocation(lineNumberAddr);
                curr.setSymbol(lexeme);
                lineNumberAddr++;
                break;
        } //end switch
        symbolTable[symbolTableIndex] = curr;
    } else{
        cout << "memory is full\n";
    }
    
    symbolTableIndex++;
}



