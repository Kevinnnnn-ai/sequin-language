#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef enum {
    KEYWORD,
    IDENTIFIER,
    FLAG,
    STRING,
    NUMBER,
    TIMESTAMP,
    DURATION,
    DELIMITER,
    NEWLINE,
    END_OF_FILE,
} TokenType;

typedef struct {
    TokenType token_type;
    const char *lexeme;
    int length;
    int line;
    int column;
} Token;

typedef struct {
    Token *token_list;
    int token_count;
    int capacity;
} TokenList;

typedef struct {
    const char *source_string;
    int source_length;
    int current_position;
    int line;
    int column;
} Lexer;

void AppendToken(TokenList *token_list, Token *token);
void CreateToken(TokenList *token_list, TokenType token_type, const char *lexeme, int length, int line, int column);
bool IsAtEndOfFile(const Lexer *lexer);
char Advance(Lexer *lexer);
char Peek(const Lexer *lexer);

TokenList* Tokenize(const char *source_string, int source_length);
char* ToString(const char *source_path, long int *string_length);
TokenList* RunLexer(const char* source_path);

void AppendToken(TokenList *token_list, Token *token) {
    if ((*token_list).token_count == (*token_list).capacity) {
        int capacity = ((*token_list).capacity == 0) ? 8 : (*token_list).capacity * 2;

        Token *temp = realloc((*token_list).token_list, capacity * sizeof(Token));
        if (temp == NULL) {
            return; // TODO: add error handling here.
        }

        (*token_list).capacity = capacity;
        (*token_list).token_list = temp;
    }

    (*token_list).token_list[(*token_list).token_count] = *token;
    (*token_list).token_count++;
}

void CreateToken(TokenList *token_list, TokenType token_type, const char *lexeme, int length, int line, int column) {
    Token token;
    token.token_type = token_type;
    token.lexeme = lexeme;
    token.length = length;
    token.line = line;
    token.column = column;
    AppendToken(token_list, &token);
}

bool IsAtEndOfFile(const Lexer *lexer) {
    return (*lexer).current_position >= (*lexer).source_length;
}

char Advance(Lexer *lexer) {
    (*lexer).column++;
    return (*lexer).source_string[(*lexer).current_position++];
}

char Peek(const Lexer *lexer) {
    return (*lexer).source_string[(*lexer).current_position];
}

TokenList* Tokenize(const char *source_string, int source_length) {
    TokenList *token_list = calloc(1, sizeof(TokenList));
    if (token_list == NULL) {
        return NULL; // TODO: add error handling here.
    }
    
    Lexer lexer;
    lexer.source_string = source_string;
    lexer.source_length = source_length;
    lexer.current_position = 0;
    lexer.line = 1;
    lexer.column = 1;

    while (!(IsAtEndOfFile(&lexer))) {
        int lexeme_position = lexer.current_position;
        int line = lexer.line;
        int column = lexer.column;
        char character = Advance(&lexer);

        switch (character) { // TODO: add a default return management.
            case ' ':
            case '\t':
                break;

            case '\r':
                if (Peek(&lexer) == '\n') {
                    Advance(&lexer);
                    CreateToken(token_list, NEWLINE, &source_string[lexeme_position], lexer.current_position - lexeme_position, line, column);
                    lexer.column = 1;
                    lexer.line++;
                } else {
                    return NULL; // TODO: add error handling here. Free token list struct and tokens themselves via a helper.
                }
                break;

            case '\n':
                CreateToken(token_list, NEWLINE, &source_string[lexeme_position], lexer.current_position - lexeme_position, line, column);
                lexer.column = 1;
                lexer.line++;
                break;

            case '#':
                while (!(IsAtEndOfFile(&lexer)) && Peek(&lexer) != '\r' && Peek(&lexer) != '\n') {
                    Advance(&lexer);
                }
                break;
        }
    }
}

char* ToString(const char *source_path, long int *string_length) {
    FILE *source_file = fopen(source_path, "rb");
    if (source_file == NULL) {
        return NULL; // TODO: add error handling here.
    }

    if (fseek(source_file, 0, SEEK_END) != 0) {
        fclose(source_file); // TODO: add error handling here.
        return NULL;
    }

    long int source_length = ftell(source_file); // NOTE: fseek and SEEK_END with ftell is not a guaranteed size in ISO C
    if (source_length == -1) {
        fclose(source_file); // TODO: add error handling here.
        return NULL;
    }

    rewind(source_file);

    char *source_string = malloc(source_length + 1);
    if (source_string == NULL) {
        fclose(source_file); // TODO: add error handling here.
        return NULL;
    }

    size_t bytes_read = fread(source_string, sizeof(char), source_length, source_file); // FIXME: ferror is unchecked
    source_string[bytes_read] = '\0';
    fclose(source_file);

    if (string_length != NULL) {
        *string_length = source_length; // FIXME: The '\0' above already admits they can differ, which number describes the contents?
    }
    
    return source_string;
}

TokenList* RunLexer(const char* source_path) {
    long int source_length;
    const char *source_string = ToString(source_path, &source_length); // FIXME: this variable owns mutable malloc'd memory.
    if (source_string == NULL) {
        return NULL; // TODO: add error handling here.
    }

    free(source_string); // TODO: decide who owns the source string and for how long.
    return NULL; // TODO: add return here.
}

int main() { // TODO: remove main, integrate lexer into compiling process.
    const char *source_path = "C:\\Users\\Admin\\files\\projects\\sequin-lang\\examples\\example_1.sqn";
    RunLexer(source_path);
}