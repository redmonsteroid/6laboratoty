#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <random>
#include <locale> 
#include <iomanip> // Добавляем заголовочный файл для использования setw, setfill и hex
using namespace std;

void GENERATE_MASTER_KEY(vector<unsigned char>& MASTER_KEY, mt19937_64& mt) {
    uniform_int_distribution<int> letters('a', 'z'); //кодировка ASCII
    uniform_int_distribution<int> numbers(0, 9); //кодировка ASCII
    for (int i = 1; i <= 8; i++) {
        MASTER_KEY.push_back(letters(mt));
        MASTER_KEY.push_back('0' + numbers(mt));
    }
}

void ShiftRows(std::vector<unsigned char>& line) {
    vector<unsigned char> v(line.size());
    for (int i = 1; i < line.size(); i++) {
        v[i - 1] = line[i]; //сдвиг элементов влево
    }
    v[line.size() - 1] = line[0]; //последний элемент становится первым

    line = v;
}

void SubBytes(vector<unsigned char>& line) { //замена на соответствующие байты из S-BOX

    vector<unsigned char> Sbox = {
       0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
       0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
       0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
       0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
       0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
       0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
       0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
       0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
       0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
       0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
       0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
       0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
       0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
       0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
       0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
       0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
    };

    vector<unsigned char> v;
    for (auto i : line) {
        v.push_back(Sbox[i]);
    }
    line = v;
}


vector<unsigned char> AddRoundKey(vector<unsigned char>& FIRST, vector<unsigned char>& SECOND) {
    vector<unsigned char> v;
    for (int i = 0; i < 4; ++i) {
        v.push_back(FIRST[i] ^ SECOND[i]);
    }
    return v;
}


void KeyExpansion(vector<unsigned char>& MASTER_KEY, vector<vector<unsigned char>>& ROUND_KEYS) {

    const vector<unsigned char> Rcon = {
        0x00, 0x00, 0x00, 0x00,
        0x01, 0x00, 0x00, 0x00,
        0x02, 0x00, 0x00, 0x00,
        0x04, 0x00, 0x00, 0x00,
        0x08, 0x00, 0x00, 0x00,
        0x10, 0x00, 0x00, 0x00,
        0x20, 0x00, 0x00, 0x00,
        0x40, 0x00, 0x00, 0x00,
        0x80, 0x00, 0x00, 0x00,
        0x1b, 0x00, 0x00, 0x00,
        0x36, 0x00, 0x00, 0x00
    };


    vector<unsigned char> v;
    int i = 0;
    int Nk = 4;
    int Nb = 4;
    int Nr = 10;

    ROUND_KEYS.resize(Nb * (Nr + 1), vector<unsigned char>(4)); //размер 44 по 4 строчки

    while (i < Nk) {
        v = { MASTER_KEY[4 * i], MASTER_KEY[4 * i + 1], MASTER_KEY[4 * i + 2], MASTER_KEY[4 * i + 3] }; //генерация раундового ключа по алгоритму

        ROUND_KEYS[i] = v;
        i++;
    }
    i = Nk;
    while (i < (Nb * (Nr + 1))) { //обработка раундовых ключей по алгоритму
        v = ROUND_KEYS[i - 1];
        if (i % Nk == 4) {
            ShiftRows(v);
            SubBytes(v);

            for (int k = 0; k < v.size(); ++k) {
                v[k] = v[k] ^ Rcon[i / Nk];
            }
        }
        else if (Nk > 6 && (i % Nk == 4)) {
            SubBytes(v);
        }

        for (int j = 0; j < v.size(); ++j) {
            ROUND_KEYS[i][j] = ROUND_KEYS[i - Nk][j] ^ v[j];
        }
        i++;
    }
}

unsigned char galois_multiply(unsigned char a, unsigned char b) {
    unsigned char result = 0; // Инициализация переменной для хранения результата умножения
    unsigned char carry; // Переменная для хранения переноса

    for (int i = 0; i < 8; ++i) { // Цикл по битам переменной b (предполагается, что char содержит 8 бит)
        if (b & 1) { // Если младший бит b равен 1
            result ^= a; // Выполняем XOR текущего результата с переменной a
        }

        carry = a & 0x80; // Проверяем старший бит переменной a перед сдвигом
        a <<= 1; // Сдвигаем переменную a влево на 1 бит

        if (carry) { // Если старший бит a был 1
            a ^= 0x1b; // Выполняем XOR переменной a с модулем AES
        }

        b >>= 1; // Сдвигаем переменную b вправо на 1 бит
    }

    return result; // Возвращаем результат умножения
}
vector<unsigned char> generate_mult_by_2() {
    vector<unsigned char> mult_by_2(256);
    for (int i = 0; i < 256; ++i) {
        mult_by_2[i] = galois_multiply(i, 2);
    }
    return mult_by_2;
}

// Генерация таблицы mult_by_3
vector<unsigned char> generate_mult_by_3() {
    vector<unsigned char> mult_by_3(256);
    for (int i = 0; i < 256; ++i) {
        mult_by_3[i] = galois_multiply(i, 3);
    }
    return mult_by_3;

}
void mix_column(vector<unsigned char>& line) {
    vector<unsigned char> mult_by_2 = generate_mult_by_2(); //операции умножения на 2 и 3 к столбцам матрицы данных

    vector<unsigned char> mult_by_3 = generate_mult_by_3();

    vector<unsigned char> v;
    v.push_back(mult_by_2[line[0]] ^ mult_by_3[line[1]] ^ line[2] ^ line[3]);
    v.push_back(mult_by_2[line[1]] ^ mult_by_3[line[2]] ^ line[0] ^ line[3]);
    v.push_back(mult_by_2[line[2]] ^ mult_by_3[line[3]] ^ line[0] ^ line[1]);
    v.push_back(mult_by_2[line[3]] ^ mult_by_3[line[0]] ^ line[1] ^ line[2]);
    line = v;
}


vector<vector<unsigned char>> XOR_blocks(const vector<vector<unsigned char>>& a, const vector<vector<unsigned char>>& b) {
    vector<vector<unsigned char>> res(4, vector<unsigned char>(4, 0));
    for (int i = 0; i < 4; ++i) {
        //res.push_back(matrixRow);
        for (int j = 0; j < 4; ++j) {
            res[i][j] = (a[i][j] ^ b[i][j]);
        }
    }
    return res;
}

void BLOCK_GENERATE(string& text, vector<vector<vector<unsigned char>>>& BLOCK) {
    while (text.size() % 16 != 0) { // добавление пробелов
        text += ' ';
    }

    vector<vector<vector<unsigned char>>> v; // временный BLOCK

    int count = 0;
    vector<vector<unsigned char>> sixteen(4, vector<unsigned char>(4)); // матрица 4

    for (int i = 0; i < text.size(); ++i) {
        int a = (i % 16) % 4; // по блокам выбирается строка
        int b = (i % 16) / 4;// по блокам выбирается столбец
        sixteen[a][b] = text[i];

        if ((i + 1) % 16 == 0) { //если блок заполнился
            v.push_back(sixteen);
            sixteen = vector<vector<unsigned char>>(4, vector<unsigned char>(4));
        }
    }

    BLOCK = v;
}


vector<vector<unsigned char>> CIPHER(vector<vector<unsigned char>>& BLOCK, vector<vector<unsigned char>>& ROUND_KEYS, vector<unsigned char> MASTER_KEY) {


    vector<vector<unsigned char>> v(4, vector<unsigned char>(4, 0)); //первый addroundkey
    for (int i = 0; i < 4; ++i) {
        v[i] = AddRoundKey(BLOCK[i], ROUND_KEYS[i]);
    }
    //ROUNDS_KEYS[0] - забрали
    for (int i = 1; i <= 9; i++) { // REPEAT 10 - 1
        for (int j = 0; j <= 3; j++) { //обрабатываем каждую строку BLOCK
            SubBytes(v[j]);
            ShiftRows(v[j]);
            mix_column(v[j]);
            v[j] = AddRoundKey(v[j], ROUND_KEYS[i]);
        }
    }
    //ROUNDS_KEYS[0] - ROUNDS_KEYS[9] - забрали
    for (int j = 0; j <= 3; j++) {
        SubBytes(v[j]);
        ShiftRows(v[j]);
        v[j] = AddRoundKey(v[j], ROUND_KEYS[10]);
    }//ROUNDS_KEYS[10] - забрали

    return v;
}


int main()
{   
    
    mt19937_64 mt(random_device{}());
    vector<vector<unsigned char>> ROUND_KEYS;
    vector<unsigned char> MASTER_KEY;
    vector<vector<vector<unsigned char>>> BLOCK;
    string text;
    cout << "enter text for encryption ==>> ";
    getline(cin, text);
    BLOCK_GENERATE(text, BLOCK);
    cout << "----------------------------------------------";
    cout << "\nencryption block looks like this: \n";
    for (auto i : BLOCK) {
        for (auto j : i) {
            for (auto k : j) {
                cout << setw(4) << k << " ";
            }
            cout << endl;
        }
        cout << "\n------";

    }
    cout << "\n----------------------------------------------" << endl;
    GENERATE_MASTER_KEY(MASTER_KEY, mt);
    cout << "\nmaster key 128 bit: ";
    for (auto i : MASTER_KEY) {
        cout << i;
    }
    cout << "\n\n----------------------------------------------\n";



    KeyExpansion(MASTER_KEY, ROUND_KEYS); // ГЕНЕРАЦИЯ КЛЮЧЕЙ
    cout << "generated keys \n----------------------------------------------\n";
    for (auto i : ROUND_KEYS) {
        for (auto j : i) {
            cout << hex << setw(4) << static_cast<int>(j) << " ";

        }
        cout << endl;
    }
    cout << "\n----------------------------------------------\n" << endl;


    vector<vector<unsigned char>> PREV(4, vector<unsigned char>(4, 0));
    vector<vector<unsigned char>> DEFOLT = PREV;
    vector<vector<unsigned char>> TEK;
    vector<vector<vector<unsigned char>>> ECRYPT;

    for (int i = 0; i < BLOCK.size(); i++) {

        vector<vector<unsigned char>> res(4, vector<unsigned char>(4, 0));
        vector<vector<unsigned char>> B = BLOCK[i];
        TEK = CIPHER(PREV, ROUND_KEYS, MASTER_KEY);

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                res[i][j] = (TEK[i][j] ^ B[i][j]);

            }
        }
        ECRYPT.push_back(res);

        PREV = TEK;
    }

    cout << "final cipher after encryption:\n" << endl;
    for (auto t : ECRYPT) {
        for (auto i : t) {
            for (auto j : i) {
                cout << setw(4) << j << " ";
            }
            cout << endl;
        }
    }
    cout << "\n\n----------------------------------------------\n" << endl;

    PREV = DEFOLT;
    vector<vector<vector<unsigned char>>> DECRYPT;

    for (int i = 0; i < ECRYPT.size(); ++i) {
        vector<vector<unsigned char>> res(4, vector<unsigned char>(4, 0));
        vector<vector<unsigned char>> B = ECRYPT[i];

        TEK = CIPHER(PREV, ROUND_KEYS, MASTER_KEY);

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                res[i][j] = (B[i][j] ^ TEK[i][j]);
            }
        }
        DECRYPT.push_back(res);
        PREV = TEK;
    }

    cout << "received message after decryption:\n" << endl;
    for (int i = 0; i < DECRYPT.size(); i++) {

        vector<vector<unsigned char>>& decrypt = DECRYPT[i];
        for (int j = 0; j < decrypt.size(); j++) {
            for (int k = 0; k < decrypt[j].size(); k++) {
                cout << decrypt[k][j];
            }

        }
    }
    cout << "\n\n----------------------------------------------\n" << endl;



}
