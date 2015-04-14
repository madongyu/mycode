/**
random shuffle

**/

void random(string a[], int n)
{
	int index, i;
	string tmp;
	srand(time(NULL));
	for (i = 0; i<n; i++)
	{
		index = rand() % (n - i) + i;
		if (index != i)
		{
			tmp = a[i];
			a[i] = a[index];
			a[index] = tmp;
		}

	}

}
