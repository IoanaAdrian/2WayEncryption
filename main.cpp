#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <cstring>

using namespace std;

char wrapDown(char leftLimit, char rightLimit, char c, int numToInt) {
    return leftLimit + (c + numToInt - rightLimit - 1);
}

char wrapUp(char leftLimit, char rightLimit, char c, int numToInt) {
    return rightLimit - numToInt + c - leftLimit + 1;
}

void encryptDigit(int i, char c, char num, char *a, char leftLimit, char rightLimit, bool positive) {
    int numToInt = num - '0';
    bool ok = i % 2 == 0;
    if (!positive) { ok = !ok; }
    if (ok) { //odd position, so we try adding
        if (isalpha(c + numToInt)) {
            a[i] = c + numToInt;
        } else {
            a[i] = wrapDown(leftLimit, rightLimit, c, numToInt);
        }
    } else { //even position, so we try subtracting
        if (isalpha(c - numToInt)) {
            a[i] = c - numToInt;
        } else {
            a[i] = wrapUp(leftLimit, rightLimit, c, numToInt);
        }
    }
}

char *encrypt(char *c) {
    int n = strlen(c);
    char *a = (char *) calloc(n + 1, sizeof(char));
    char *num = (char *) calloc(n + 1, sizeof(char));
    for (int i = 0; i < n; i++) {
        num[i] = rand() % 9 + '0';
    }
    for (int i = 0; i < n; i++) {
        if (islower(c[i])) {
            encryptDigit(i, c[i], num[i], a, 'a', 'z', true);
        } else if (isupper(c[i])) {
            encryptDigit(i, c[i], num[i], a, 'A', 'Z', true);
        } else {
            a[i] = c[i];
        }
    }
    char *result = (char *) calloc(2 * n + 3, sizeof(char));
    strcat(result, a);
    strcat(result, " ");
    strcat(result, num);
    free(a);
    free(num);
    return result;
}

char *decrypt(char *c, char *num) {
    int n = strlen(c);
    char *a = (char *) calloc(n + 1, sizeof(char));
    for (int i = 0; i < n; i++) {
        if (islower(c[i])) {
            encryptDigit(i, c[i], num[i], a, 'a', 'z', false);
        } else if (isupper(c[i])) {
            encryptDigit(i, c[i], num[i], a, 'A', 'Z', false);
        } else {
            a[i] = c[i];
        }
    }
    return a;
}

void consoleRun() {
    int n;
    cout << "Decrypt:1; Encrypt:2 \n";
    cin >> n;
    if (n == 2) {
        ifstream f("sdet.txt");
        ofstream g("out.txt");
        char c[205] = "";
        while (f.getline(c, 205) && strcmp(c, "\0") != 0) {
            char *r = encrypt(c);
            g << r << "\n";
        }
    } else if (n == 1) {
        ifstream decryptIn("out.txt");
        ofstream decryptOut("decrypted.txt");
        char c[205], num[205];
        while (decryptIn.getline(c, 100, ' ') && decryptIn.getline(num, 100, '\n') && strcmp(c, "\0") != 0) {
            char *r = decrypt(c, num);
            decryptOut << r << "\n";
            cout << r << "\n";
        }
    }
}

void argRun(int argv, char *argc[]) {
    if (strcmp(argc[1], "-e") == 0) {
        cout << encrypt(argc[2]);
    } else if (strcmp(argc[1], "-d") == 0) {
        cout << decrypt(argc[2], argc[3]);
    }else{
        cout << "Unknown command";
    }
}

int main(int argv, char *argc[]) {
    srand(time(nullptr));

    argRun(argv,argc);

    return 0;
}
