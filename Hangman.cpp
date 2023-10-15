#include "Hangman.h"
#include <iostream>
#include <fstream>
#include <random>
#include <cctype>
#include <map>
#include <set>

bool playAnotherGame() {
    char choice;
    std::cout << "Do you want to play another game? (Y/N): ";
    std::cin >> choice;
    return (choice == 'Y' || choice == 'y');
}

Hangman::Hangman() {
    numberOfGuesses = 0;
    numberOfAllowedGuesses = 6; // You can change this to set your desired number of allowed guesses.
    guessedLetters = "";
    initDictionary();
    setSecretWord();
    setBoard();
}

void Hangman::initDictionary() {
    std::ifstream file("dictionary.txt"); // You can change the file name.
    std::string word;

    while (file >> word) {
        words.push_back(word);
    }
    file.close();
}

void Hangman::setSecretWord() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, words.size() - 1);
    secretWord = words[dist(gen)];
}

void Hangman::setBoard() {
    board = std::string(secretWord.length(), '_');
}

void Hangman::updateBoard(char c) {
    for (size_t i = 0; i < secretWord.length(); i++) {
        if (secretWord[i] == c) {
            board[i] = c;
        }
    }
}

void Hangman::displayWord() {
    std::cout << "Word: " << board << "\n";
    std::cout << "Guessed Letters: " << guessedLetters << "\n";
}

std::string Hangman::takeGuess() {
    std::string guess;
    while (true) {
        std::cout << "Enter a letter to guess: ";
        std::cin >> guess;
        char c = std::toupper(guess[0]);
        if (guess.length() == 1 && isalpha(c) && guessedLetters.find(c) == std::string::npos && isupper(guess[0])) {
            guessedLetters += c;
            return guess;
        } else {
            std::cout << "Invalid input. Please enter a single letter you haven't guessed before, and make sure its a capital letter.\n";
        }
    }
}

std::string Hangman::addGuessedLetter(std::string str) {
    guessedLetters += str;
    return guessedLetters;
}

void Hangman::play() {
    while (numberOfGuesses < numberOfAllowedGuesses) {
        displayWord();
        std::string guess = takeGuess();
        if (secretWord.find(guess[0]) != std::string::npos) {
            updateBoard(guess[0]);
        } else {
            numberOfGuesses++;
            std::cout << "Incorrect guess. You have " << numberOfAllowedGuesses - numberOfGuesses << " tries left.\n";
        }

        if (board == secretWord) {
            std::cout << "Congratulations! You've guessed the word: " << secretWord << "\n";
            return;  // Add a return statement to exit the function
        }
    }

    std::cout << "You've run out of guesses! The word was: " << secretWord << "\n";
}


void evilHangman::playEvil() {
    bool playAgain = true;
    while (playAgain) {
        guessedLetters = "";
        wordlistSizePrompt();
        setsizeOfWord();
        setGuesses();
        setSecretWord();
        secretWord = "";
        if (flag == 1) {
            std::cout << "The number of words left in the list is: " << wordlist.size() << "\n";
        }

        if (numberOfGuesses == numberOfAllowedGuesses) {
            secretWord = wordlist[0];
            play();
            playAgain = playAnotherGame();
            if (!playAgain) {
                break;
            }
        }

        while (secretWord == "") {
            std::string guess = takeGuess();
            char guessedLetter = guess[0];
            partitionWords(guessedLetter);
            if (flag == 1) {
                std::cout << "The number of words left in the list is: " << wordlist.size() << "\n";
            }
            numberOfGuesses++;
            std::cout << "Number of guesses left: " << numberOfAllowedGuesses - numberOfGuesses << "\n";
            if (numberOfAllowedGuesses == numberOfGuesses) {
                secretWord = wordlist[0];
                break;
            }
            if (board==secretWord){
                playAnotherGame();
            }
        }

        for (int i = 0; i < guessedLetters.size(); i++) {
            updateBoard(guessedLetters[i]);
        }

        play();
        playAgain = playAnotherGame();
    }
}


void evilHangman::setSecretWord() {
    initDictionary();
    int getArrayLength = words.size(); // Use words.size() to get the number of words.

    for (int i = 0; i < getArrayLength; i++) {
        if (words[i].length() == sizeOfWord) { // Check the length of each word.
            wordlist.push_back(words[i]); // Use push_back to add words to wordlist.
        }
    }
        if (wordlist.size()==1){
        secretWord = wordlist[0];
        setBoard();

    }
}

void evilHangman::setsizeOfWord() {
        sizeOfWord = 0;
        std::cout << "Please input the size of word that you want: " << "\n";
        std::cin >> sizeOfWord; // Use std::cin to read the input value.

}
void evilHangman::setGuesses(){
    std::cout<<"How many guesses do you want?"<<"\n";
    std::cin>>numberOfAllowedGuesses;

}

void evilHangman::partitionWords(char c) {
    std::map<std::string, std::vector<std::string>> wordFamilies;
    std::vector<std::string> updatedWordlist;

    // Create word families based on the guessed letter
    for (const std::string& word : wordlist) {
        std::string pattern = word; // Initialize with the word itself

        // Replace characters that are not equal to the guessed letter with placeholders
        for (char& letter : pattern) {
            if (letter != c) {
                letter = '_';
            }
        }

        // Add the word to the corresponding word family
        wordFamilies[pattern].push_back(word);
    }

    // Select the largest word family
    std::string largestFamilyPattern;
    size_t largestFamilySize = 0;

    for (const auto& entry : wordFamilies) {
        if (entry.second.size() > largestFamilySize) {
            largestFamilySize = entry.second.size();
            largestFamilyPattern = entry.first;
        }
    }

    // Create a set to store unique words from the largest family
    std::set<std::string> uniqueWords;
    for (const std::string& word : wordFamilies[largestFamilyPattern]) {
        uniqueWords.insert(word);
    }

    // Update the wordlist with unique words from the largest family
    updatedWordlist.assign(uniqueWords.begin(), uniqueWords.end());

    // Update the original wordlist with the updatedWordlist
    wordlist = updatedWordlist;

    // Display the largest word family
    std::cout << "(Pattern: " << largestFamilyPattern << "):" <<"\n";
    for (const std::string& word : updatedWordlist) {
    }
    if (flag){
        std::cout << "\nSize of Current Word Family: " << largestFamilySize << "\n";
    }


    if (wordlist.size()==1){
        secretWord=wordlist[0];
    }

    std::cout << std::endl;
    
}

void evilHangman::wordlistSizePrompt(){
    bool flag = true;
    char c;
    std::cout<<"Would you like to track the size of your word list?(enter Y or N)"<<"\n";
    std::cin>>c;
    if (c == 'Y'){
        flag = true;
    }
    

}
