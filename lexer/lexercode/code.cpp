#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <unordered_map>

// ---------- Token Types ----------
enum class TokenType
{
    Let,
    Identifier,
    Number,
    Plus,
    Equal,
    Semicolon,
    EndOfFile,
    Unknown
};

// ---------- Token Structure ----------
struct Token
{
    TokenType type;
    std::string value;
    int line;
    int column;

    Token(TokenType t, std::string v, int l, int c)
        : type(t), value(std::move(v)), line(l), column(c) {}
};

// ---------- Lexer Class ----------
class Lexer
{
private:
    std::string source;
    size_t pos = 0;
    int line = 1;
    int column = 1;

public:
    Lexer(const std::string &input) : source(input) {}

    Token nextToken()
    {
        skipWhitespace();

        if (isAtEnd())
            return Token(TokenType::EndOfFile, "", line, column);

        char c = peek();

        if (std::isalpha(c))
            return identifier();
        if (std::isdigit(c))
            return number();

        int currentCol = column;

        switch (c)
        {
        case '+':
            advance();
            return Token(TokenType::Plus, "+", line, currentCol);
        case '=':
            advance();
            return Token(TokenType::Equal, "=", line, currentCol);
        case ';':
            advance();
            return Token(TokenType::Semicolon, ";", line, currentCol);
        default:
            advance();
            return Token(TokenType::Unknown, std::string(1, c), line, currentCol);
        }
    }

private:
    bool isAtEnd() const
    {
        return pos >= source.size();
    }

    char peek() const
    {
        return isAtEnd() ? '\0' : source[pos];
    }

    char advance()
    {
        char c = peek();
        pos++;
        if (c == '\n')
        {
            line++;
            column = 1;
        }
        else
        {
            column++;
        }
        return c;
    }

    void skipWhitespace()
    {
        while (!isAtEnd() && std::isspace(peek()))
        {
            advance();
        }
    }

    Token identifier()
    {
        int startCol = column;
        std::string value;

        while (!isAtEnd() && (std::isalnum(peek()) || peek() == '_'))
        {
            value += advance();
        }

        if (value == "let")
        {
            return Token(TokenType::Let, value, line, startCol);
        }

        return Token(TokenType::Identifier, value, line, startCol);
    }

    Token number()
    {
        int startCol = column;
        std::string value;

        while (!isAtEnd() && std::isdigit(peek()))
        {
            value += advance();
        }

        return Token(TokenType::Number, value, line, startCol);
    }
};

// ---------- Main Function ----------
int main()
{
    std::ifstream file("example.stv");
    if (!file)
    {
        std::cerr << "Could not open example.stv\n";
        return 1;
    }

    std::string code((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    Lexer lexer(code);

    Token tok = lexer.nextToken();
    while (tok.type != TokenType::EndOfFile)
    {
        std::cout << "Token: " << static_cast<int>(tok.type)
                  << " Value: \"" << tok.value
                  << "\" Line: " << tok.line
                  << " Column: " << tok.column << "\n";
        tok = lexer.nextToken();
    }

    std::cout << "Lexing complete.\n";
    return 0;
}
