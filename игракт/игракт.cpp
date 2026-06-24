#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <windows.h>

using namespace std;

// ============================================================
// СЧЕТЧИК РЕШЕННЫХ ГОЛОВОЛОМОК
// ============================================================
int puzzlesSolved = 0;        // Количество решенных головоломок

// ========== ГОЛОВОЛОМКИ ==========

// Математическая головоломка
bool mathPuzzle() {
    srand(time(0));
    int a = rand() % 20 + 1;
    int b = rand() % 20 + 1;
    int correct = a + b;

    cout << "\n=== ГОЛОВОЛОМКА ===\n";
    cout << "Решите пример: " << a << " + " << b << " = ?\n";
    cout << "Ваш ответ: ";

    int answer;
    cin >> answer;
    cin.ignore();

    return answer == correct;
}

// Словесная головоломка
bool wordPuzzle() {
    cout << "\n=== ГОЛОВОЛОМКА ===\n";
    cout << "Угадайте слово по подсказке:\n";
    cout << "Подсказка: подземное сооружение с запутанными ходами\n";
    cout << "Введите слово: ";

    string word;
    getline(cin, word);

    // Убираем пробелы в начале
    while (!word.empty() && word[0] == ' ') {
        word.erase(0, 1);
    }

    // Убираем пробелы в конце
    while (!word.empty() && word[word.length() - 1] == ' ') {
        word.pop_back();
    }

    // Приводим к нижнему регистру
    for (int i = 0; i < (int)word.length(); i++) {
        word[i] = tolower(word[i]);
    }

    return word == "labirint" || word == "labyrinth";
}

// Головоломка с выбором пути
bool pathPuzzle() {
    cout << "\n=== ГОЛОВОЛОМКА ===\n";
    cout << "Выберите правильный путь:\n";
    cout << "1. Налево\n";
    cout << "2. Направо\n";
    cout << "3. Прямо\n";
    cout << "Ваш выбор: ";

    int choice;
    cin >> choice;
    cin.ignore();

    return choice == 2;
}

// ========== СТРУКТУРА ЛОКАЦИИ ==========

struct Location {
    int id;
    string name;
    string description;
    vector<string> art;
    vector<int> exits;
    bool isOpen;
    int puzzleType; // 0 - нет, 1 - математика, 2 - слово, 3 - путь
    string hint;
};

// ============================================================
// ФУНКЦИЯ ПОКАЗА КОЛИЧЕСТВА РЕШЕННЫХ ГОЛОВОЛОМОК
// ============================================================
void showPuzzlesSolved() {
    cout << "\n[ГОЛОВОЛОМКИ]\n";
    cout << "========================================\n";
    cout << "  Решено головоломок: " << puzzlesSolved << "\n";
    cout << "========================================\n";
}

// ========== РИСОВАНИЕ ЛОКАЦИИ ==========
void drawLocation(const Location& loc) {
    cout << "\n========================================\n";
    cout << "[ЛОКАЦИЯ] " << loc.name << "\n";
    cout << "Описание: " << loc.description << "\n";
    if (!loc.isOpen && !loc.hint.empty()) {
        cout << "Подсказка: " << loc.hint << "\n";
    }
    cout << "========================================\n";

    for (int i = 0; i < (int)loc.art.size(); i++) {
        cout << loc.art[i] << "\n";
    }

    cout << "========================================\n";
    cout << "Статус: " << (loc.isOpen ? "ОТКРЫТА" : "ЗАКРЫТА") << "\n";

    if (!loc.exits.empty()) {
        cout << "Выходы: ";
        for (int i = 0; i < (int)loc.exits.size(); i++) {
            cout << loc.exits[i];
            if (i < (int)loc.exits.size() - 1) {
                cout << ", ";
            }
        }
        cout << "\n";
    }

    if (!loc.isOpen && loc.puzzleType != 0) {
        cout << "Чтобы открыть, введите 'open'\n";
    }
    cout << "========================================\n";
}

// ========== РЕШЕНИЕ ГОЛОВОЛОМКИ ==========
bool solvePuzzle(int type) {
    bool result = false;
    if (type == 1) {
        result = mathPuzzle();
    }
    else if (type == 2) {
        result = wordPuzzle();
    }
    else if (type == 3) {
        result = pathPuzzle();
    }

    // Увеличиваем счетчик решенных головоломок
    if (result) {
        puzzlesSolved++;
    }

    return result;
}

// ========== ПОКАЗ ПОМОЩИ ==========
void showHelp() {
    cout << "\n[ПОМОЩЬ] Команды:\n";
    cout << "  go <номер>   - перейти в локацию\n";
    cout << "  open         - решить головоломку\n";
    cout << "  look         - посмотреть локацию\n";
    cout << "  map          - показать карту\n";
    cout << "  puzzles/p    - показать количество решенных головоломок\n";
    cout << "  help         - показать помощь\n";
    cout << "  exit         - выйти\n";
    cout << "\n[ПОДСКАЗКА] Как пройти:\n";
    cout << "  1. go 1 - зайти в Зал рун\n";
    cout << "  2. Решить математический пример\n";
    cout << "  3. go 2 - зайти в Библиотеку\n";
    cout << "  4. Ввести слово 'labirint'\n";
    cout << "  5. go 3 - зайти к Выходу\n";
    cout << "  6. Выбрать 2 (направо)\n";
}

// ========== ПОКАЗ КАРТЫ ==========
void showMap(const map<int, Location>& locations, int currentId) {
    cout << "\n[КАРТА] Лабиринт:\n";

    map<int, Location>::const_iterator it;
    for (it = locations.begin(); it != locations.end(); it++) {
        int id = it->first;
        const Location& loc = it->second;
        cout << "  " << id << ". " << loc.name
            << " [" << (loc.isOpen ? "ОТКРЫТА" : "ЗАКРЫТА") << "]";

        if (!loc.exits.empty()) {
            cout << " -> ";
            for (int i = 0; i < (int)loc.exits.size(); i++) {
                cout << loc.exits[i];
                if (i < (int)loc.exits.size() - 1) {
                    cout << ", ";
                }
            }
        }
        cout << "\n";
    }
    cout << "\nТекущая локация: " << currentId << "\n";
}

// ========== ПОКАЗ ИНСТРУКЦИИ ==========
void showInstructions() {
    cout << "\n========================================\n";
    cout << "        ИНСТРУКЦИЯ ПО УПРАВЛЕНИЮ        \n";
    cout << "========================================\n";
    cout << "\n";
    cout << "  ЦЕЛЬ ИГРЫ:\n";
    cout << "  ----------\n";
    cout << "  Вы находитесь в подземном лабиринте.\n";
    cout << "  Ваша задача - найти выход!\n";
    cout << "\n";
    cout << "  КАК ИГРАТЬ:\n";
    cout << "  -----------\n";
    cout << "  1. Вводите команды в строке после символа '>'\n";
    cout << "  2. Для перехода в другую комнату используйте 'go <номер>'\n";
    cout << "  3. Если комната закрыта, решите головоломку командой 'open'\n";
    cout << "  4. Изучайте комнаты с помощью 'look'\n";
    cout << "  5. Смотрите карту командой 'map'\n";
    cout << "  6. Смотрите количество решенных головоломок командой 'puzzles'\n";
    cout << "\n";
    cout << "  ДОСТУПНЫЕ КОМАНДЫ:\n";
    cout << "  -------------------\n";
    cout << "  go <номер>   - перейти в локацию\n";
    cout << "  open         - решить головоломку\n";
    cout << "  look         - посмотреть локацию\n";
    cout << "  map          - показать карту\n";
    cout << "  puzzles/p    - показать количество решенных головоломок\n";
    cout << "  help         - показать помощь\n";
    cout << "  exit         - выйти\n";
    cout << "\n";
    cout << "  НОМЕРА ЛОКАЦИЙ:\n";
    cout << "  ---------------\n";
    cout << "  0 - Вход в лабиринт (стартовая)\n";
    cout << "  1 - Зал древних рун\n";
    cout << "  2 - Библиотека теней\n";
    cout << "  3 - Выход из лабиринта (финальная)\n";
    cout << "\n";
    cout << "========================================\n";
}

// ========== ГЛАВНАЯ ФУНКЦИЯ ==========
int main() {
    // Настройка кодировки для Windows
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    // Создаём локации
    map<int, Location> locations;

    // Локация 0 - Вход
    Location entrance;
    entrance.id = 0;
    entrance.name = "Вход в лабиринт";
    entrance.description = "Вы стоите у входа в тёмный подземный лабиринт.";
    entrance.art.push_back("    +---------------------------------------+");
    entrance.art.push_back("    |           ВХОД В ЛАБИРИНТ             |");
    entrance.art.push_back("    |                                       |");
    entrance.art.push_back("    |     ##########################        |");
    entrance.art.push_back("    |     #                        #        |");
    entrance.art.push_back("    |     #    [ВЫ ЗДЕСЬ]          #        |");
    entrance.art.push_back("    |     #                        #        |");
    entrance.art.push_back("    |     ##########################        |");
    entrance.art.push_back("    |                                       |");
    entrance.art.push_back("    |   Двери: 1 (Зал рун)  2 (Библиотека) |");
    entrance.art.push_back("    +---------------------------------------+");
    entrance.exits.push_back(1);
    entrance.exits.push_back(2);
    entrance.isOpen = true;
    entrance.puzzleType = 0;
    entrance.hint = "";
    locations[0] = entrance;

    // Локация 1 - Зал рун
    Location runeHall;
    runeHall.id = 1;
    runeHall.name = "Зал древних рун";
    runeHall.description = "Вы в зале, стены которого покрыты древними письменами.";
    runeHall.art.push_back("    +---------------------------------------+");
    runeHall.art.push_back("    |           ЗАЛ ДРЕВНИХ РУН             |");
    runeHall.art.push_back("    |                                       |");
    runeHall.art.push_back("    |     ~~~~~~~~~~~~~~~~~~~~~~~~~~        |");
    runeHall.art.push_back("    |     ~   [ДРЕВНИЕ РУНЫ]       ~        |");
    runeHall.art.push_back("    |     ~    ~~~~~~~~~~~~~~~      ~        |");
    runeHall.art.push_back("    |     ~    ~               ~     ~        |");
    runeHall.art.push_back("    |     ~    ~   [КРИСТАЛЛ]   ~     ~      |");
    runeHall.art.push_back("    |     ~~~~~~~~~~~~~~~~~~~~~~~~~~        |");
    runeHall.art.push_back("    |                                       |");
    runeHall.art.push_back("    |   Выход: 2 (Библиотека)              |");
    runeHall.art.push_back("    +---------------------------------------+");
    runeHall.exits.push_back(2);
    runeHall.isOpen = false;
    runeHall.puzzleType = 1; // математика
    runeHall.hint = "На стене высечены числа...";
    locations[1] = runeHall;

    // Локация 2 - Библиотека
    Location library;
    library.id = 2;
    library.name = "Библиотека теней";
    library.description = "Огромная библиотека с древними книгами.";
    library.art.push_back("    +---------------------------------------+");
    library.art.push_back("    |           БИБЛИОТЕКА ТЕНЕЙ            |");
    library.art.push_back("    |                                       |");
    library.art.push_back("    |     +------------------------+        |");
    library.art.push_back("    |     | [КНИГИ]                |        |");
    library.art.push_back("    |     |  [][][]  [][][]       |        |");
    library.art.push_back("    |     |  [][][]  [][][]       |        |");
    library.art.push_back("    |     +------------------------+        |");
    library.art.push_back("    |        [ГОРЯТ СВЕЧИ]                  |");
    library.art.push_back("    |                                       |");
    library.art.push_back("    |   Выход: 3 (Выход из лабиринта)      |");
    library.art.push_back("    +---------------------------------------+");
    library.exits.push_back(3);
    library.isOpen = false;
    library.puzzleType = 2; // слово
    library.hint = "Одна книга шепчет загадку...";
    locations[2] = library;

    // Локация 3 - Выход
    Location exit;
    exit.id = 3;
    exit.name = "Выход!";
    exit.description = "Вы видите свет в конце тоннеля! Это выход!";
    exit.art.push_back("    +---------------------------------------+");
    exit.art.push_back("    |           ВЫХОД ИЗ ЛАБИРИНТА          |");
    exit.art.push_back("    |                                       |");
    exit.art.push_back("    |     **************************        |");
    exit.art.push_back("    |     *    СВЕТ ВПЕРЕДИ         *        |");
    exit.art.push_back("    |     *    [СОЛНЦЕ] [СОЛНЦЕ]    *        |");
    exit.art.push_back("    |     *    [СОЛНЦЕ] [СОЛНЦЕ]    *        |");
    exit.art.push_back("    |     **************************        |");
    exit.art.push_back("    |                                       |");
    exit.art.push_back("    |   [ВЫ ПОБЕДИЛИ!]                     |");
    exit.art.push_back("    +---------------------------------------+");
    exit.exits = {};
    exit.isOpen = false;
    exit.puzzleType = 3; // путь
    exit.hint = "Выберите правильный путь к свободе";
    locations[3] = exit;

    // Игровые переменные
    int current = 0;
    bool gameWon = false;
    bool firstLaunch = true;

    // Приветствие
    cout << "\n[ИГРА] ДОБРО ПОЖАЛОВАТЬ В ПОДЗЕМНЫЙ ЛАБИРИНТ!\n";
    cout << "=========================================\n";
    cout << "Ваша цель - найти выход из лабиринта.\n";

    // Показываем инструкцию при первом запуске
    if (firstLaunch) {
        showInstructions();
        firstLaunch = false;
    }

    cout << "\nВведите 'help' для вызова справки.\n";

    // Основной цикл игры
    while (!gameWon) {
        // Показываем текущую локацию
        drawLocation(locations[current]);

        // Проверка победы
        if (current == 3 && locations[3].isOpen) {
            cout << "\n[ПОБЕДА] ПОЗДРАВЛЯЮ! ВЫ ВЫБРАЛИСЬ ИЗ ЛАБИРИНТА!\n";
            cout << "=========================================\n";
            cout << "  Вы прошли игру! Отличная работа!  \n";
            cout << "=========================================\n";
            gameWon = true;
            break;
        }

        // Ввод команды
        cout << "\n> ";
        string command;
        cin >> command;
        cin.ignore();

        if (command == "help") {
            showHelp();
        }
        else if (command == "look") {
            // уже отрисовано
        }
        else if (command == "map") {
            showMap(locations, current);
        }
        else if (command == "puzzles" || command == "p") {
            showPuzzlesSolved();
        }
        else if (command == "open") {
            if (!locations[current].isOpen) {
                if (solvePuzzle(locations[current].puzzleType)) {
                    cout << "\n[УСПЕХ] Правильно! Локация открыта.\n";
                    locations[current].isOpen = true;
                }
                else {
                    cout << "\n[ОШИБКА] Неправильно. Попробуйте снова.\n";
                }
            }
            else {
                cout << "[ИНФО] Эта локация уже открыта.\n";
            }
        }
        else if (command == "go") {
            string num;
            getline(cin, num);

            // Убираем пробелы в начале
            while (!num.empty() && num[0] == ' ') {
                num.erase(0, 1);
            }

            // Убираем пробелы в конце
            while (!num.empty() && num[num.length() - 1] == ' ') {
                num.pop_back();
            }

            if (num.empty()) {
                cout << "[ОШИБКА] Укажите номер локации.\n";
                continue;
            }

            int target = atoi(num.c_str());

            // Проверяем существует ли локация
            if (locations.find(target) == locations.end()) {
                cout << "[ОШИБКА] Локации " << target << " не существует.\n";
                continue;
            }

            // Проверяем есть ли путь
            bool hasPath = false;
            for (int i = 0; i < (int)locations[current].exits.size(); i++) {
                if (locations[current].exits[i] == target) {
                    hasPath = true;
                    break;
                }
            }

            if (!hasPath) {
                cout << "[ОШИБКА] Нет пути в локацию " << target << ".\n";
                continue;
            }

            // Проверяем открыта ли локация
            if (!locations[target].isOpen) {
                cout << "\n[ЗАКРЫТО] Локация закрыта! Решите головоломку.\n";
                if (solvePuzzle(locations[target].puzzleType)) {
                    cout << "\n[УСПЕХ] Правильно! Локация открыта.\n";
                    locations[target].isOpen = true;
                }
                else {
                    cout << "\n[ОШИБКА] Неправильно. Попробуйте снова.\n";
                    continue;
                }
            }

            // Переходим
            current = target;
            cout << "[ПЕРЕХОД] Перемещение в локацию " << target << "...\n";

            // Проверка победы
            if (current == 3 && locations[3].isOpen) {
                cout << "\n[ПОБЕДА] ПОЗДРАВЛЯЮ! ВЫ ВЫБРАЛИСЬ ИЗ ЛАБИРИНТА!\n";
                cout << "=========================================\n";
                cout << "  Вы прошли игру! Отличная работа!  \n";
                cout << "=========================================\n";
                gameWon = true;
                break;
            }
        }
        else if (command == "exit") {
            cout << "[ВЫХОД] До свидания!\n";
            return 0;
        }
        else if (command == "instructions") {
            showInstructions();
        }
        else {
            cout << "[ОШИБКА] Неизвестная команда. Введите 'help'.\n";
        }
    }

    return 0;
}