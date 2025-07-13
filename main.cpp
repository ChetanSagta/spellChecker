#include <codecvt>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <uchar.h>
#include <locale.h>
class WordNode {
public:
  void insertWord(std::wstring);
  void print(std::wstring);
  bool isValid();
  std::wstring isValidString();
  wchar_t getWchar_T();
  std::map<wchar_t, WordNode *> getChildren();
  WordNode();
  ~WordNode();

private:
  wchar_t value;
  bool valid;
  std::map<wchar_t, WordNode *> children;
};

wchar_t WordNode::getWchar_T() { return this->value; }
std::map<wchar_t, WordNode *> WordNode::getChildren() { return this->children; }
WordNode::WordNode() {
  value = ' ';
  valid = false;
}

WordNode::~WordNode() {}

bool WordNode::isValid() { return this->valid; }

std::wstring WordNode::isValidString() {
  if (this->valid) {
    return L"true";
  }
  return L"false";
}

void WordNode::insertWord(std::wstring word) {
  wchar_t firstChar = word[0];
  try {
    if (children.at(firstChar) != nullptr) {
      std::wstring restWord = word.substr(1, word.size());
      if (!restWord.empty()) {
        this->children[firstChar]->insertWord(restWord);
      } else {
        this->children[firstChar]->valid = true;
      }
    }
  } catch (const std::out_of_range &ex) {
    children[firstChar] = new WordNode();
    children[firstChar]->value = firstChar;
    std::wstring restWord = word.substr(1, word.size());
    if (!restWord.empty()) {
      this->children[firstChar]->insertWord(restWord);
    } else {
      this->children[firstChar]->valid = true;
    }
  }
}

void WordNode::print(std::wstring prefix) {
  if (this->children.size() == 0)
    return;
  for (auto it = children.cbegin(); it != children.cend(); ++it) {
    std::wcout << prefix << it->first << L"(" << it->second->isValidString()
               << it->second->children.size() << L")\n";
    ((WordNode *)it->second)->print(prefix + L" ");
  }
}

bool verifySpelling(WordNode *wordNode, std::wstring word) {
  wchar_t firstChar = word[0];
  std::wstring restWord = word.substr(1, word.size());
  if (restWord.length() > 0) {
    try {
      return verifySpelling(wordNode->getChildren().at(firstChar), restWord);
    } catch (const std::out_of_range &ex) {
      return false;
    }
  }
  try {
    return wordNode->getChildren().at(firstChar)->isValid();
  } catch (const std::out_of_range &ex) {
    return false;
  }
}

void prepareWordNode(WordNode *wordNode) {
  std::wifstream file("wl2.txt", std::ios::binary);
  file.imbue(std::locale(file.getloc(), new std::codecvt_utf8<wchar_t>));

  if (!file) {
    std::wcerr << L"Failed to open file.\n";
    return;
  }
  std::wstring line;
  while (std::getline(file, line)) {
    wordNode->insertWord(line);
  }
}

void checkSpelling(WordNode *wordNode, std::wstring text) {
  bool valid = verifySpelling(wordNode, text);
  if (valid) {
    std::wcout << L"Valid Wstring\n";
  } else {
    std::wcout << L"Invalid Wstring\n";
  }
}

int main() {
  std::locale::global(std::locale(""));

  WordNode wordNode;
  std::wstring valid = L"content";
  std::wstring invalid = L"ica";
  std::wstring wideString = L"études"; 
  std::wstring wideString2 = L"Ångström"; 
  prepareWordNode(&wordNode);
  wordNode.print(L"");
  checkSpelling(&wordNode, valid);
  checkSpelling(&wordNode, invalid);
  checkSpelling(&wordNode, wideString);
  checkSpelling(&wordNode, wideString2);
  return 0;
}
