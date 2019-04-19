#include <stdio.h>

void shell_sort(int numbers[], int start, int end) {
	int increment = end - start + 1;    //初始化划分增量
	int temp, i, j;
	do {    //每次减小增量，直到increment = 1
		increment = increment / 3 + 1;
		for (i = start + increment; i <= end; ++i) {    //对每个划分进行直接插入排序
			if (numbers[i - increment] > numbers[i]) {
				temp = numbers[i];
				j = i - increment;
				do {    //移动元素并寻找位置
					numbers[j + increment] = numbers[j];
					j -= increment;
				} while (j >= start && numbers[j] > temp);
				numbers[j + increment] = temp;  //插入元素
			}
		}
	} while (increment > 1);
}

int main(void)
{
	int i, a[] = {10, 18, 4, 3, 6, 12, 1, 9, 15, 8};
	int len = sizeof(a) / sizeof(a[0]);
	shell_sort(a, 0, len - 1);
	for (i = 0; i < len; i++) {
		printf("%d ", a[i]);
	}
}
