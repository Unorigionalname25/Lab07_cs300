#ifndef HANGMAN_H
#define HANGMAN_H

#include <string>
#include <vector>

class Hangman {
public:
    Hangman();

    void play();

private:
    int numberOfGuesses;
    int numberOfAllowedGuesses;
    std::string guessedLetters;
    std::vector<std::string> words;
    std::string secretWord;
    std::string board;

    void setSecretWord();
    void initDictionary();
    void setBoard();
    void updateBoard(char c);
    void displayWord();
    std::string takeGuess();
    std::string addGuessedLetter(std::string str);
    friend class evilHangman;

};

class evilHangman : public Hangman {
    public:

    evilHangman() : Hangman(){

    }
    std::vector<std::string> wordlist;
    void setSecretWord();
    void setsizeOfWord();
    void playEvil();
    void setGuesses();
    void partitionWords(char c);
    void wordlistSizePrompt();
private:
    int sizeOfWord;
    int flag;


};

#endif
