#include <iostream>
using namespace std;

void S(int n)
{
	for (int i = 1; i < (1<<n); i++)
		cout << ' ';
}

void B(int n)
{
	n--;
	if (!n) {
		cout << 'X' << endl;
		return;
	}

	B(n); S(n); B(n); cout << endl;
	S(n); B(n); cout << endl;
	B(n); S(n); B(n); cout << endl;
}

int main(void)
{
}
