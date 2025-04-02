#ifndef cyarg_scanner_h
#define cyarg_scanner_h

typedef enum {
    // Single-character tokens.
    TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
    TOKEN_LEFT_SQUARE_BRACKET, TOKEN_RIGHT_SQUARE_BRACKET,
    TOKEN_COMMA, TOKEN_DOT, TOKEN_MINUS, TOKEN_PLUS,
    TOKEN_SEMICOLON, TOKEN_SLASH, TOKEN_STAR,
    TOKEN_BAR, TOKEN_AMP, TOKEN_CARET, TOKEN_PERCENT,
    // One or two character tokens.
    TOKEN_BANG, TOKEN_BANG_EQUAL,
    TOKEN_EQUAL, TOKEN_EQUAL_EQUAL,
    TOKEN_GREATER, TOKEN_GREATER_EQUAL, TOKEN_RIGHT_SHIFT,
    TOKEN_LESS, TOKEN_LESS_EQUAL, TOKEN_LEFT_SHIFT,
    // Literals.
    TOKEN_IDENTIFIER, TOKEN_STRING, TOKEN_NUMBER,
    // Keywords.
    TOKEN_AND, TOKEN_CLASS, TOKEN_ELSE, TOKEN_FALSE,
    TOKEN_FOR, TOKEN_FUN, TOKEN_IF, TOKEN_IMPORT,
    TOKEN_LEN,
    TOKEN_MAKE_ARRAY, TOKEN_MAKE_CHANNEL, TOKEN_MAKE_ROUTINE,
    TOKEN_NIL, TOKEN_OR,
    TOKEN_PEEK, TOKEN_PRINT, 
    TOKEN_RECEIVE, TOKEN_RESUME, TOKEN_RETURN, 
    TOKEN_RPEEK, TOKEN_RPOKE,
    TOKEN_SEND, TOKEN_SET_ELEMENT, TOKEN_SHARE, TOKEN_START, TOKEN_SUPER, 
    TOKEN_THIS, TOKEN_TRUE, 
    TOKEN_VAR, TOKEN_WHILE, TOKEN_YIELD,

    TOKEN_ERROR, TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    const char* start;
    int length;
    int line;
} Token;

void initScanner(const char* source);
Token scanToken();

#endif