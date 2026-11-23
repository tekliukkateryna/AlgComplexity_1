#include <iostream>
#include <queue>
#include <vector>
#include <string>

using namespace std;

/*Варіант 3
Реалізуйте структуру даних Префіксне дерево (Trie) для збереження
словника унікальних слів. Дерево має підтримувати операції додавання
слова, видалення слова та пошуку. Реалізуйте функцію автодоповнення
(autocomplete): за заданим префіксом (наприклад, "про") програма
має за оптимальний час знайти та вивести на екран топ-5 найкоротших
слів з дерева, що починаються на цей префікс.*/

class Node{
public:
    char value = '\0';
    Node* parent = nullptr;
    vector<Node*> children;
    bool is_terminal = false;

    Node(char value, Node* parent, bool is_terminal) {
        this->value = value;
        this->parent = parent;
        this->is_terminal = is_terminal;
    }

    ~Node() {
        for (Node* child : children) {
            delete child;
        }
    }
};

class Trie{
public:
    Node* root = nullptr;

    Trie() {
        root = new Node('\0', nullptr, false);
    }

    void add (string word) {
        Node* curr = root;

        for (char letter : word){

            Node* found = nullptr;

            for (Node* child : curr->children){
                if (child->value == letter){
                    found = child;
                    break;
                }
            }

            if (found != nullptr){
                curr = found;
            } else {
                Node* new_node = new Node(letter, curr, false);
                curr->children.push_back(new_node);
                curr = new_node;
            }
        }

        curr->is_terminal = true;

        cout<<"Add completed \n";
    }

    void create (const vector<string>& list){

        for (const string& word : list){
            this->add(word);
        }
    }

    void remove (string str) {
        Node* curr = root;

        for (char letter : str) {
            bool found = false;
            for (Node* child : curr->children) {
                if (child->value == letter) {
                    curr = child;
                    found = true;
                    break;
                }
            }
            if (!found) {
                cout<<"Not found \n";
                return;
            }
        }

        if (!curr->is_terminal) {
            cout<<"Not found \n";
            return;
        }
        curr->is_terminal = false;

        if (!curr->children.empty()) {
            cout<<"Is included in other words \n";
            return;
        }


        while (!curr->is_terminal && curr!=root && curr->children.empty()) {
            Node* prev = curr->parent;

            for (int i = 0; i < prev->children.size(); i++) {
                if (curr->value == prev->children[i]->value) {
                    prev->children.erase(prev->children.begin() + i);
                    delete curr;
                    curr = prev;
                    break;
                }
            }
        }
        cout<<"Deletion completed \n";
    }

    bool find (string str) {
        Node* curr = root;
        for (char letter : str) {
            bool is_found = false;
            for (Node* child : curr->children) {
                if (child->value == letter) {
                    curr = child;
                    is_found = true;
                    break;
                }
            }
            if (!is_found) {
                return false;
            }
        }
        if (!curr->is_terminal) {
            return false;
        }

        return true;
    }

    vector<string> autocomplete (string prefix, int n) {
        vector<string> res = {};
        queue<pair<Node*, string>> q;

        // Проходимо префікс
        Node* curr = this->root;

        for (char letter : prefix) {
            bool is_found = false;
            for (Node* child : curr->children) {
                if (child->value == letter) {
                    curr = child;
                    is_found = true;
                    break;
                }
            }
            if (!is_found) {
                cout<<"Not found \n";
                return {};
            }
        }

        // BFS, щоб знайти спочатку найкоротші
        q.push({curr, prefix});

        while (n>0 && !q.empty()) {
            pair<Node*, string> curr_pair= q.front();
            q.pop();
            curr = curr_pair.first;
            string path = curr_pair.second;

            if (curr->is_terminal) {
                res.push_back(path);
                n--;
            }

            for (Node* child : curr->children) {
                q.push({child, path+child->value});
            }
        }

        return res;
    }

    void printHelper(Node* node, const string& prefix, bool isLast, const string& currentWord) const {
        if (node == nullptr) return;

        if (node == root) {
            cout << "[ROOT]" << endl;
        } else {
            cout << prefix;
            cout << (isLast ? "+-- " : "|-- ");
            cout << node->value;

            if (node->is_terminal) {
                cout << "  * (" << currentWord << ")";
            }
            cout << endl;
        }

        string childPrefix = prefix;
        if (node != root) {
            childPrefix += (isLast ? "    " : "|   ");
        }

        for (size_t i = 0; i < node->children.size(); ++i) {
            bool lastChild = (i == node->children.size() - 1);
            Node* child = node->children[i];
            printHelper(child, childPrefix, lastChild, currentWord + child->value);
        }
    }

    void print() const {
        printHelper(root, "", true, "");
    }

    ~Trie() {
        delete root;
    }
};

int main() {

    Trie trie;
    vector<string> words = {
        "const", "continue", "concept", "consteval", "constexpr",
        "constinit", "const_cast", "co_await", "co_return", "co_yield",

        "struct", "static", "static_cast", "static_assert",

        "return", "register", "requires", "reinterpret_cast",

        "char", "case", "catch", "class", "char8_t", "char16_t", "char32_t",

        "do", "delete", "double", "default", "decltype", "dynamic_cast",

        "try", "this", "true", "throw", "typeid", "typedef", "typename", "template", "thread_local",

        "auto", "bool", "break", "else", "enum", "explicit", "export",
        "extern", "false", "float", "for", "friend", "goto", "if",
        "inline", "int", "long", "namespace", "new", "nullptr",
        "operator", "private", "protected", "public", "short",
        "signed", "sizeof", "switch", "union", "unsigned",
        "using", "virtual", "void", "volatile", "while"
    };

    trie.create(words);
    cout << "--------------------Initial--------------------------------\n";
    trie.print();

    cout << "------------------Аdd constant-----------------------------\n";
    trie.add("constant");
    trie.print();

    cout << "----------------Remove continue----------------------------\n";
    trie.remove("continue");
    trie.print();

    cout<< "----------------Find 'constexpr' (must be true)-------------\n";
    cout << trie.find("constexpr") << '\n';
    cout << "--------------Find 'con' (must be false, as it is just a prefix)--------------\n";
    cout << trie.find("con") << '\n';

    cout << "-------------Autocomplete top-5----------------------------\n";
    vector<string> res = trie.autocomplete("d", 5);
    for (string word : res) {
        cout<< word << "\n";
    }
    cout << "-------------Autocomplete top-5 after deleting first word ('do')---------\n";
    trie.remove("do");
    res = trie.autocomplete("d", 5);
    for (string word : res) {
        cout<< word << "\n";
    }

    /*int choice = -1;
    while (choice != 0) {
        cout << "\n--- MENU ---\n";
        cout << "1. Add a word\n";
        cout << "2. Find a word\n";
        cout << "3. Remove a word\n";
        cout << "4. Autocomplete (top-5 shortest words)\n";
        cout << "5. Print Trie structure\n";
        cout << "0. Exit\n";
        cout << "Choose an option: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter a number from 0 to 5.\n";
            choice = -1;
            continue;
        }

        if (choice == 1) {
            string word;
            cout << "Enter a single word to add: ";
            cin >> word;
            cin.ignore(10000, '\n');
            trie.add(word);
            cout << "Word \"" << word << "\" added successfully.\n";
        }
        else if (choice == 2) {
            string word;
            cout << "Enter a word to find: ";
            cin >> word;
            cin.ignore(10000, '\n');
            if (trie.find(word)) {
                cout << "Result: Word \"" << word << "\" EXISTS in the dictionary.\n";
            }
        }
        else if (choice == 3) {
            string word;
            cout << "Enter a word to remove: ";
            cin >> word;
            cin.ignore(10000, '\n');
            trie.remove(word);
        }
        else if (choice == 4) {
            string prefix;
            cout << "Enter a prefix for autocomplete: ";
            cin >> prefix;
            cin.ignore(10000, '\n');

            vector<string> suggestions = trie.autocomplete(prefix, 5);
            if (!suggestions.empty()) {
                cout << "Top-" << suggestions.size() << " shortest words for \"" << prefix << "\":\n";
                for (size_t i = 0; i < suggestions.size(); ++i) {
                    cout << "  " << i + 1 << ". " << suggestions[i]
                         << " (length: " << suggestions[i].length() << ")\n";
                }
            }
        }
        else if (choice == 5) {
            cout << "\nCurrent Trie structure:\n";
            trie.print();
        }
        else if (choice == 0) {
            cout << "Exiting program. \n";
        }
        else {
            cout << "Unknown option. Please choose between 0 and 5.\n";
        }
    }*/

    return(0);
}