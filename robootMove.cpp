#include <iostream>
#include <memory.h>
using namespace std;
#define maxSize 110
char map[maxSize][maxSize];
int n, m;
char buffer[1100];

int curI, curJ;
bool ok(int x, int y) {
    return 0 <= x && x < n && 0 <= y && y < m && map[x][y] != 'x';
}
void move(char c) {
    switch ( c)
    {
    case 'U':
        if (ok(curI - 1, curJ)) {
            curI = curI - 1;
        }
        break;
    case 'D':
        if (ok(curI + 1, curJ)) {
            curI = curI + 1;
        }
        break;
    case 'L':
        if (ok(curI, curJ - 1)) {
            curJ--;
        }
        break;
    case 'R':
        if (ok(curI, curJ + 1)) {
            curJ++;
        }
        break;
    default:
        break;
    }
}
int main() {
    int t;
    cin >> t;
    while (t--) {
        curI = 0;
        curJ = 0;
        cin >> n >> m;
        for (int i = 0; i < n; i++) {
            cin >> map[i];
        }
        cin >> buffer;
        int j = 0;
        while (buffer[j] != '\0') {
            move(buffer[j]);
            j++;
        }
        cout << curI + 1 << " " << curJ + 1 << endl;
    }
    return 0;
}                                 