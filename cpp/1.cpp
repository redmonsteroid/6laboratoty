#include <iostream>
#include <random>
#include <vector>
#include <map>
#include <algorithm>
#include <iomanip>
#include <set>
using namespace std;

bool isPrime (int num){
    if (num < 2) return false;
    if (num == 2) return true;
    if (num % 2 == 0) return false;
    for (int i = 3; i * i <= num; i += 2){
        if (num % i == 0) return false;
    }
    return true;
}

void task1() {

    srand(time(0));
    cout << "enter size of matrix" << endl;
    int m,n;
    cin >> m >> n;
    vector<vector<int>> matrix(m, vector<int>(n));

    for (int i = 0; i < m; i++){
        for (int j = 0; j < n; j++){
            matrix[i][j] = rand() % 51;
        }
    }

    cout << "matrix is" << endl;

    for (int i = 0; i < m; i++){
        for (int j = 0; j < n; j++){
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }

    map<int, int> primeCount; // первое - простое число, второе - количество вхождений

    for (const auto& row : matrix) {
        for (int num : row) {
            if (isPrime(num)) {
                primeCount[num]++;
            }
        }
    }

    // Поиск простого числа с наибольшим количеством вхождений
    auto maxElement = max_element(primeCount.begin(), primeCount.end(), 
        [](const pair<int, int>& p1, const pair<int, int>& p2) {
            return p1.second < p2.second;
        });

    // Вывод результата
    if (maxElement != primeCount.end()) {
        cout << "The prime number that occurs the most times: " << maxElement->first << " (occurs " << maxElement->second << " times)" << endl;
    } else {
        cout << "There are no prime numbers in the matrix." << endl;
    }
}
void task2() {
    int N;
    cout << "Enter size of matrix: ";
    cin >> N;
    int M=N;

    // Инициализация матрицы
    vector<vector<int>> matrix(N, vector<int>(N));

  for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (i == j) {
                matrix[i][j] = 000;
            }
            else if (i > j) {
                matrix[i][j] = 100 + j;
            }
            else {
                matrix[i][j] = 100 + N - j - 1;
            }
        }
    }
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (matrix[i][j] == 0) {
                cout << "000" << " ";
            }
            else {
                cout << matrix[i][j] << " ";
            }
        }
        cout << endl;
    }
}

// Функция для генерации уникальных случайных чисел

vector<int> generateUniqueRandomNumbers(int count, int min, int max) {
    set<int> uniqueNumbers;
    while (uniqueNumbers.size() < count) {
        int num = min + rand() % (max - min + 1);
        uniqueNumbers.insert(num);
    }
    return vector<int>(uniqueNumbers.begin(), uniqueNumbers.end());
}

// Функция для поиска элементов, минимальных в строке и максимальных в столбце
vector<int> findSpecialElements(const vector<vector<int>>& matrix) {
    vector<int> specialElements;
    int rows = matrix.size();
    int cols = matrix[0].size();

    for (int i = 0; i < rows; ++i) {
        // Находим минимальный элемент в строке i
        int minElement = matrix[i][0];
        int minIndex = 0;
        for (int j = 1; j < cols; ++j) {
            if (matrix[i][j] < minElement) {
                minElement = matrix[i][j];
                minIndex = j;
            }
        }

        // Проверяем, является ли этот элемент максимальным в своем столбце
        bool isMaxInColumn = true;
        for (int k = 0; k < rows; ++k) {
            if (matrix[k][minIndex] > minElement) {
                isMaxInColumn = false;
                break;
            }
        }

        // Выводим отладочную информацию
        cout << "Row " << i << ": min element = " << minElement 
             << " at column " << minIndex 
             << " is " << (isMaxInColumn ? "" : "not ") 
             << "max in column" << endl;

        if (isMaxInColumn) {
            specialElements.push_back(minElement);
        }
    }

    return specialElements;
}

void task3() {
    srand(static_cast<unsigned int>(time(0)));

    int M = 5; // Количество строк
    int N = 5; // Количество столбцов

    // Генерируем уникальные случайные числа для заполнения матрицы
    vector<int> randomNumbers = generateUniqueRandomNumbers(M * N, 1, 100);

    random_shuffle(randomNumbers.begin(), randomNumbers.end()); // случайно тасует вектор
    
    // Заполняем двумерный массив
    vector<vector<int>> matrix(M, vector<int>(N));
    int index = 0;
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            matrix[i][j] = randomNumbers[index++];
        }
    }

    // Выводим матрицу
    cout << "Matrix:" << endl;
    for (const auto& row : matrix) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }

    // Ищем и выводим специальные элементы
    vector<int> specialElements = findSpecialElements(matrix);

    cout << "Special elements (minimal in row, maximal in column):" << endl;
    for (int val : specialElements) {
        cout << val << " ";
    }
    cout << endl;
}

int main () {
    cout << "enter task number" << endl;
    int task; cin >> task;
    cout << endl;
    switch (task){
        case 1: {task1(); break;}
        case 2: {task2(); break;}
        case 3: {task3();break;}
        default: {cout << "incorrect task number" << endl; break;}
    }
}