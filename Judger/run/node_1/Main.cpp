#include <cstdio>
#include <iostream>
#include <iomanip>
#include <cstring>
 
using namespace std;
int n;
int main() {
    char n1[1024], n2[1024];
    char n3[1024];
    cin >> n;
    for (int s = 1 ; s <= n; s++) {
        cin >> n1 >> n2;
        int i = strlen(n1) - 1;
        int j = strlen(n2) - 1;
        int k = 0, w = 0;
        if ( i > 1 )
        {
            cout << endl << endl;
        }
        while( i >= 0 || j >= 0) {
            int a = 0, b = 0;
            if ( i >= 0) {
                a = n1[i] - '0';
                i--;
            }
            if ( j >= 0) {
                b = n2[j] - '0';
                j--;
            }
            n3[k++] = (a + b + w) % 10 + '0';
            w = (a + b + w) / 10;
        }
        if (w != 0) {
            n3[k++] = w + '0';
        }
        n3[k] = 0;
        cout << "Case " << s << ":" << endl;
        cout << n1 << " + " << n2 << " = " ;
        for (int m = strlen(n3)-1; m >= 0; m--) {
            cout << n3[m] ;
        }
    }
    return 0;
}