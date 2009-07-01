#include "Parser.h"

void Parser::newtoken() {
	GYuchar c;
	
	if (reusetoken) {
		reusetoken=false;
		return;
	}
	otoken=token;
	token="";
	while (!oss.eof()) {
		c=oss.get();
		switch(c) {
			case '\n':
				linenum++;
			case '\r': case '\t': case ' ':
				if (token!="") return;
				break;
			case '(': case ')': case '[': case ']':
			case ';': case '.':
				if (token=="") token.append(1, c);
				else oss.seekg(-1, std::ios_base::cur);
				return;
			case '{': case '}':
				if (token=="") {
					token.append(1, c);
					if (oss.get()==c) token.append(1, c);
					else oss.seekg(-1, std::ios_base::cur);
				} else oss.seekg(-1, std::ios_base::cur);
				return;
			case '#':
				if (token=="") {
					GYuchar c;

					c=oss.get();
					if (c=='#') {
						c=oss.get();
						if (c=='#') {
							// 여러 줄 주석
							while (!oss.eof()) {
								c=oss.get();
								if (c=='#')
									if (oss.get()=='#' && oss.get()=='#') break;
							}
						}
						// 그냥 아무 것도 아니게 된 주석
					} else {
						// 한 줄(혹은 부분) 주석
						while (!oss.eof() && c!='\n' && c!='#') c=oss.get();
					}
				} else {
					oss.seekg(-1, std::ios_base::cur);
					return;
				}
				break;
			case '\'': case '\"':
				if (token!="") oss.seekg(-1, std::ios_base::cur);
				else {
					GYuchar cc=c;
					token.append(1, c);
					do {
						c=oss.get();
						if (c=='\\') {
							c=oss.get();
							switch (c) {
							case 'n': c='\n'; break;
							case 't': c='\t'; break;
							}
							token.append(1, c); c='\\';
						} else token.append(1, c);
					} while (c!=cc);
				}
				return;
			case '~': case '`': case '!': case '@': case '$': case '%': case '^': case '&': case ',':
			case '+': case '*': case '-': case '/': case '?': case '<': case '>': case '=': case '|': case ':':
				if (token!="") {
					oss.seekg(-1, std::ios_base::cur);
				} else {
					token.append(1, c);
					while(true) {
						c=oss.get();
						switch (c) {
							case '~': case '`': case '!': case '@': case '$': case '%': case '^': case '&': case ',':
							case '+': case '*': case '-': case '/': case '?': case '<': case '>': case '=': case '|': case ':':
								token.append(1, c);
								break;
							default:
								oss.seekg(-1, std::ios_base::cur);
								return;
						}
					}
				}
				return;
			case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
				if (token=="") {
					while (isdigit(c)) {
						token.append(1, c);
						c=oss.get();
					}
					if (c=='.') {
						c=oss.get();
						if (isdigit(c)) {
							token.append(1, '.');
							do {
								token.append(1, c);
								c=oss.get();
							} while (isdigit(c));
						} else oss.seekg(-1, std::ios_base::cur);
					}
					oss.seekg(-1, std::ios_base::cur);
					return;
				}
			default:
				token.append(1, c);
		}
	}
}

TokenType Parser::whattoken() {
/*
	assign			T_ASSIGN
	class			T_CLASS
	elif			T_ELIF
	else			T_ELSE
	false			T_FALSE
	fn				T_FN
	for				T_FOR
	if				T_IF
	infix|infixl	T_INFIXL
	infixr			T_INFIXR
	in				T_IN
	operator		T_OPERATOR
	prefix			T_PREFIX
	return			T_RETURN
	true			T_TRUE
	use				T_USE
	var				T_VAR
	while			T_WHILE
	.				T_DOT
	;				T_ENDELEMENT
	(				T_OPENCALLR
	)				T_CLOSECALLR
	[				T_OPENCALLS
	]				T_CLOSECALLS
	{				T_STARTBLOCK
	}				T_ENDBLOCK
					T_NOTKEYWORD
*/
	GYstring t;
	if (reusetoken) t=otoken; else t=token;
	switch(t[0]) {
		case 'a':
			if (t=="assign") return T_ASSIGN;
			break;
		case 'c':
			if (t=="class") return T_CLASS;
			break;
		case 'e':
			if (t=="elif") return T_ELIF;
			if (t=="else") return T_ELSE;
			break;
		case 'f':
			if (t=="false") return T_FALSE;
			if (t=="for") return T_FOR;
			if (t=="fn") return T_FN;
			break;
		case 'i':
			if (t=="if") return T_IF;
			if (t=="in") return T_IN;
			if (t=="infix" || token=="infixl") return T_INFIXL;
			if (t=="infixr") return T_INFIXR;
			break;
		case 'o':
			if (t=="operator") return T_OPERATOR;
			break;
		case 'p':
			if (t=="prefix") return T_PREFIX;
			break;
		case 'r':
			if (t=="return") return T_RETURN;
			break;
		case 't':
			if (t=="true") return T_TRUE;
			break;
		case 'u':
			if (t=="use") return T_USE;
			break;
		case 'v':
			if (t=="var") return T_VAR;
			break;
		case 'w':
			if (t=="while") return T_WHILE;
			break;
		case '{':
			if (t=="{") return T_STARTBLOCK;
			if (t=="{{") return T_STARTSBLOCK;
			break;
		case '}':
			if (t=="}") return T_ENDBLOCK;
			if (t=="}}") return T_ENDSBLOCK;
			break;
		case '.': return T_DOT;
		case ';': return T_ENDELEMENT;
		case '(': return T_OPENCALLR;
		case ')': return T_CLOSECALLR;
		case '[': return T_OPENCALLS;
		case ']': return T_CLOSECALLS;
	}
	return T_NOTKEYWORD;
}

bool Parser::tokenisidentifier() {
	if (reusetoken) return whattoken()==T_NOTKEYWORD && !isdigit(otoken[0]);
	return whattoken()==T_NOTKEYWORD && !isdigit(token[0]);
}
