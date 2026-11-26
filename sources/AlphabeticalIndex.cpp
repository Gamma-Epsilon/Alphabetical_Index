#ifndef ALPHABETICAL_INDEX_CPP
#define ALPHABETICAL_INDEX_CPP

#include <sstream> // Для обработки слов

#include "../headers/AlphabeticalIndex.hpp"

static const std::string punctuation[] = {".", ",", "!", "?", ";", ":", 
    "\"", "'", "(", ")", "[", "]",
    "«", "»", "—", "–", "…"}; 

static const std::string lower_rus = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
static const std::string UPPER_RUS = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";

int GetPageLimit(int pageNumber, int pageSize) {
    if(pageSize <= 0) throw SizeIsNegative("AlphabeticalIndex - Page size should be positive");
    
    if(pageNumber == 1)      return pageSize / 2;
    if(pageNumber % 10 == 0) return pageSize * 3/4;
    
    return pageSize;
}


std::string Lower(const std::string& word) {
    /* Приведение к нижнему регистру */
    std::string result; 

    size_t i = 0;
    while(i < word.size()) {
        char c = word[i];

        if(c >= 'A' && c <= 'Z') {  // Если буква английская
            char end_lower = c + ('a' - 'A');

            result += end_lower;
            i += 1;
        } else if(i + 1 < word.size()) { // Если есть еще один байт (буква, возможно, русская)
            std::string rus  = word.substr(i, 2);
            size_t rus_upper = UPPER_RUS.find(rus);
            
            if(rus_upper != std::string::npos) {
                std::string rus_lower = lower_rus.substr(rus_upper, 2);
                result += rus_lower;
                i += 2;
            } else {
                result += c; 
                i += 1;
            }
        } else {
            result += c;
            i += 1;
        }
    }

    return result; 
}


std::string NormalizeWord(const std::string& word, CaseMode mode) {
    /* Удаление крайних знаков препинания */
    size_t left  = 0; 
    size_t right = word.size(); // word[right - 1] - последний байт

    
    while(left < right) {
        bool found = false; 
        for(const std::string& symbol : punctuation) {

            if(symbol.size() > right - left) continue; // Помещается ли символ в принципе? 

            if(word.compare(left, symbol.size(), symbol) == 0) {
                left += symbol.size(); 
                found = true;
                break;
            }
        }

        if(!found) break;
    }

    while(right > left) {
        bool found = false;
        for(const std::string& symbol : punctuation) {

            if(symbol.size() > right - left) continue; 

            if(word.compare(right - symbol.size(), symbol.size(), symbol) == 0) {
                right -= symbol.size(); 
                found = true;
                break;
            }
        }

        if(!found) break;
    }

    std::string normalizedWord = word.substr(left, right - left);

    /* Приведение к нижнему регистру в зависимости от флага */
    if(mode == CaseMode::Depend) return normalizedWord;
    return Lower(normalizedWord);
}


PaginationResult SplitIntoPages(const std::string& text, int pageSize, PageUnit unit) {
    /* Минимальный pageSize = 2 */
    
    if(pageSize <= 0) throw SizeIsNegative("AlphabeticalIndex - Page size should be positive");

    PaginationResult result;
    DynamicArray<std::string> wordsOnCurrentPage;

    int pageNumber  = 1;
    int currentUsed = 0;

    std::istringstream stream(text);
    std::string word;

    // Читаем из текста по одному целому слову
    while(stream >> word) {
        KeyValue<std::string, DynamicArray<int>> wordOnPages;
        wordOnPages.key = word;

        // Очищенное слово нужно для указателя. На страницы кладём исходный текст
        std::string cleanedWord = NormalizeWord(word, CaseMode::Depend);

        if(unit == PageUnit::Words) {

            // Сохраняем текущую страницу, когда очередное слово туда уже не помещается
            if(currentUsed >= GetPageLimit(pageNumber, pageSize)) {
                result.pages.Append(wordsOnCurrentPage);

                wordsOnCurrentPage = DynamicArray<std::string>();
                currentUsed = 0;
                ++pageNumber;
            }

            wordsOnCurrentPage.Append(word);
            ++currentUsed;

            if(!cleanedWord.empty()) wordOnPages.value.Append(pageNumber);

        } else {
            // Находим в исходном слове участок без краевой пунктуации
            size_t wordStart = word.find(cleanedWord);
            size_t wordEnd   = wordStart + cleanedWord.size();
            size_t bytePosition = 0;

            // Пока не разместим все символы слова на странице
            while(bytePosition < word.size()) {

                // На непустой странице между словами нужно место для пробела
                int separator = (currentUsed > 0) ? 1 : 0;
                int freeSpace = GetPageLimit(pageNumber, pageSize) - currentUsed - separator;

                if(freeSpace <= 0) {
                    result.pages.Append(wordsOnCurrentPage);

                    wordsOnCurrentPage = DynamicArray<std::string>();
                    currentUsed = 0;
                    ++pageNumber;

                    // Пробуем разместить остаток слова на новой странице
                    continue;
                }

                size_t partStart = bytePosition;
                std::string part;
                int partLength = 0;

                // Берём символы, пока на странице есть место
                while(partLength < freeSpace && bytePosition < word.size()) {
                    part += GetSymbol(word, bytePosition);
                    ++partLength;
                }

                wordsOnCurrentPage.Append(part);
                currentUsed += separator + partLength;

                // Запоминаем страницу, если на ней есть часть очищенного слова
                if(partStart < wordEnd && wordStart < bytePosition) wordOnPages.value.Append(pageNumber);
            }
        } /* else */

        // Сохраняем для построителя целое слово и номера его страниц
        if(wordOnPages.value.GetLength() > 0) result.wordPages.Append(wordOnPages);
    } /* first while */

    // Сохраняем последнюю страницу, оставшуюся после цикла
    if(wordsOnCurrentPage.GetLength() > 0) result.pages.Append(wordsOnCurrentPage);

    return result;
}


int CountChars(const std::string& word) {
    int length = 0; 

    /* В UTF-8 правило: дополнительные байты одного символа начинаются с 10 (а русские буквы двухбайтовые) */
    /* Можем пропустить такие байты, чтобы их не считать за новую букву */
    for(unsigned char c : word) {
        if((c & 0xc0) != 0x80) length++; 
    }

    /* 0xc0 = 11000000 */
    /* 0x80 = 10000000 */ 
    /* => С помощью маски отсекаем все биты, кроме первых двух */

    return length;
}

/* Возврат одного символа */
std::string GetSymbol(std::string& word, size_t& bytePosition) {
    if (bytePosition >= word.size()) return "";

    const size_t symbolStart = bytePosition;
    ++bytePosition;

    while(bytePosition < word.size() &&
           (static_cast<unsigned char>(word[bytePosition]) & 0xC0) == 0x80) ++bytePosition;


    return word.substr(symbolStart, bytePosition - symbolStart);
}


/* Создание указателя на основе массива */
ArrayIndex BuildArrayIndex(const DynamicArray<KeyValue<std::string, DynamicArray<int>>>& pages, CaseMode mode) {
    ArrayIndex array; 

    for(int i = 0; i < pages.GetLength(); ++i) {
        KeyValue<std::string, DynamicArray<int>> word;
        word.key = NormalizeWord(pages[i].key, mode);

        if(word.key.empty()) continue;

        int index = array.Index(word, SearchMode::Half);

        // Слово встретилось впервые. Добавляем сразу все его страницы
        if(index == -1) {
            word.value = pages[i].value;
            array.Add(word);
            continue;
        }

        // Слово уже есть. Дополняем список страниц в существующей записи
        DynamicArray<int>& pageNumbers = array[index].value;

        for(int j = 0; j < pages[i].value.GetLength(); ++j) {
            int pageNumber = pages[i].value[j];
    
            if(pageNumbers.GetLast() != pageNumber) pageNumbers.Append(pageNumber);
        }
    }

    return array;
}


/* Создание указателя на основе хеш-таблицы */
HashIndex BuildHashIndex(const DynamicArray<KeyValue<std::string, DynamicArray<int>>>& pages, int bucketCount, CaseMode mode) {
    HashIndex dictionary(bucketCount); 

    for(int i = 0; i < pages.GetLength(); ++i) {
        std::string word = NormalizeWord(pages[i].key, mode);
        if(word.empty()) continue;

        // Слово встретилось впервые. Добавляем сразу все его страницы
        if(!dictionary.ContainsKey(word)) {
            dictionary.Add(word, pages[i].value);
            continue;
        }

        // Слово уже есть. Дополняем список страниц по его ключу
        DynamicArray<int>& pageNumbers = dictionary.Get(word);

        for(int j = 0; j < pages[i].value.GetLength(); ++j) {
            int pageNumber = pages[i].value[j];
            
            if(pageNumbers.GetLast() != pageNumber) pageNumbers.Append(pageNumber);
        }
    }

    return dictionary;
}


#endif /* ALPHABETICAL_INDEX_CPP */