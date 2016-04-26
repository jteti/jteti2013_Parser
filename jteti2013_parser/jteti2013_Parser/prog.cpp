/*
Name: John Teti
Email: jteti2013@fau.edu

	prog        ->  begin stmt_list end

	stmt_list   ->  stmt ; stmt_list
				|  stmt
				|	stmt;

	stmt        ->  var = expr

	var         ->  A | B | C

	expr        ->  var + expr
				|  var - expr
				|  var

*/


#include    <iostream>
#include    <fstream>
#include    <cctype>

#include    "token.h"
#include    "functions.h"

using namespace std;

ifstream ifs;                   // input file stream used by lexan
SymTab   symtab;                // global symbol table
Token    token;                 // global token
int      lookahead = 0;         // no look ahead token yet

int      dbg = 1;               // debut is ON


int main( )
{
    ifs = get_ifs( );           // open an input file stream w/ the program
    init_kws( );                // initialize keywords in the symtab
    match( lookahead );         // get the first input token
    prog( );

    return 0;
}


// your methods...

void prog() {
	while (lookahead != DONE) {
		//determines begining of statement by checking for 'begin'
		if (token.tokstr() == "begin") {
			emit(lookahead);
			emit('\n');
			match(lookahead);
			stmt_list();

			//determines if at end of statement by checking for 'end'
			if (token.tokstr() == "end") {
				emit(lookahead);
				emit('\n');
				match(lookahead);
				
				//looks for tokens at end of statement
				if (lookahead != DONE) {
					
					//if token exists gives an error
					error(lookahead);
				}
			}
			else error(lookahead, KW, "end");
		}
		else error(lookahead, KW, "begin");
	}
}

void stmt_list() {
	while (1) {
		stmt();

		//determines if at end of statement by checking for 'end'
		if (token.tokstr() == "end") {

			//permits statement without a semicolon
			emit('\n');
			return;
		}
		match(';');
		emit(';');

		//determines if at end of statement by checking for 'end'
		if (token.tokstr() == "end") {

			//permits statement with a semicolon
			return;
		}
	}
}

void stmt() {

	//looks for assignment variable
	if (lookahead == ID) {
		emit(lookahead);
		match(lookahead);
		}
		else error(lookahead, ID);
		int lh;

		//looks for assignement operator
		if (lookahead == '=') {
			lh = lookahead;
			match(lookahead);
		}
		else error(lookahead, '=');
		
		//expression called after assignment
		expr();
		
		//postfix notation
		emit(lh);
}

void var() {

	//looks to determine if identifier is valid
	if(lookahead == ID) {
		emit(lookahead);
		match(lookahead);
	}
	else error(lookahead, ID);
}

void expr() {

	//calls var to look for first variable in expression
	var();
	while (1) {
		int lh;
		//looks for '+' or '-' operators
		if (lookahead == '+' || lookahead == '-') {
			lh = lookahead;

			//consumes current token, then gets the next one
			match(lookahead);

			//calls var so that variable follows operator
			var();

			//postfix notation
			emit(lh);

			//continues to next possible erpression following term 
			continue;
		}
		else return;
	}
}



// utility methods

void emit( int t )
{
    switch( t )
    {
        case '+': case '-': case '=':
            cout << char( t ) << ' ';
            break;

        case ';':
            cout << ";\n";
            break;

        case '\n':
            cout << "\n";
            break;

        case ID:
        case KW:
        case UID:
            cout << symtab.tokstr( token.tokvalue( ) ) << ' ';
            break;

        default:
            cout << "'token " << t << ", tokvalue "
                 << token.tokvalue( ) << "' ";
            break;
    }
}

void error( int t, int expt, const string &str )
{
    cerr << "\nunexpected token '";
    if( lookahead == DONE )
    {
        cerr << "EOF";
    }
    else
    {
        cerr << token.tokstr( );
    }
    cerr << "' of type " << lookahead;

    switch( expt )
    {
        case 0:         // default value; nothing to do
            break;

        case ID:
            cout << " while looking for an ID";
            break;

        case KW:
            cout << " while looking for KW '" << str << "'";
            break;

        default:
            cout << " while looking for '" << char( expt ) << "'";
            break;
    }

    cerr << "\nsyntax error.\n";

    exit( 1 );
}


void match( int t )
{
    if( lookahead == t )
    {
        token = lexan( );               // get next token
        lookahead = token.toktype( );   // lookahed contains the tok type
    }
    else
    {
        error( t );
    }
}

