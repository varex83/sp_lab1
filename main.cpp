#include <iostream>
#include <utility>

class ToString {
public:
    virtual std::string to_string() const = 0;
};

class Word : public ToString {
private:
    std::string word;
public:
    explicit Word(std::string word) : word(std::move(word)) {}
    [[nodiscard]] std::string to_string() const { return word; }

    bool operator==(const Word& other) const {
        return word == other.word;
    }

    static bool is_letter(char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    }

    size_t size() const {
        return word.size();
    }
};

class Separator : public ToString {
private:
    std::string separator;
public:
    explicit Separator(std::string separator) : separator(std::move(separator)) {}
    std::string to_string() const { return separator; }
};

class Terminal : public ToString {
public:
    static const char TERMINAL = '\0';

    std::string to_string() const {
        return {1, TERMINAL};
    }
};

using Token = std::variant<Word, Separator, Terminal>;

class Tokenizer {
private:
    std::string str;
    int index = 0;
public:

    explicit Tokenizer(std::string str) : str(std::move(str)) {}

    Token next_token() {
        std::string buff;

        while (index < str.size() && Word::is_letter(str[index])) {
            buff.push_back(str[index]);
            index++;
        }

        if (!buff.empty()) {
            return Word(buff);
        }

        if (index < str.size()) {
            if (str[index] == Terminal::TERMINAL) {
                return Terminal();
            }
            buff.push_back(str[index]);
            index++;
        } else {
            return Terminal();
        }

        return Separator(buff);
    }
};

int main() {
    std::string str;

    std::getline(std::cin, str);

    Tokenizer tokenizer(str);

    std::vector<Word> words;

    Token token = tokenizer.next_token();

    while(true) {
        if (std::holds_alternative<Word>(token)) {
            words.push_back(std::get<Word>(token));
        } else if (std::holds_alternative<Terminal>(token)) {
            break;
        }
        token = tokenizer.next_token();
    }

    std::sort(words.begin(), words.end(), [](const Word& a, const Word& b) {
        return a.size() < b.size();
    });

    std::cout << "Words sorted by size:" << std::endl;
    for (const auto& word : words) {
        std::cout << word.to_string() << std::endl;
    }

    return 0;
}
