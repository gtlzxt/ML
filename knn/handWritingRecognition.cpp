#include <fstream>
#include <iostream>
#include "EasyMat.h"
#include "EasyVec.h"
#include "knn.h"
using namespace std;

//file name is like digit_number;
void load(em::EasyMat& mat, const string& folder)
{
	char fileName[30];
	for (int i = 0; i <= 9; i++)
	{
		int j = 0;
		em::EasyVec row(32*32 + 1);
		while (true)
		{
			snprintf(fileName, 30, "%s/%d_%d.txt", folder.c_str(),i, j++);
			cout << "reading file " << fileName << endl;
			ifstream in(fileName);
			if (!in.good())
				break;
			unsigned long z = 0;
			while (!in.eof() && z < 32 * 32)
			{
				double d = in.get() - '0';
				if (d >= 0)
					row[z++] = d;
			}
			row[z] = i;
			mat.append(row);
		}
	}
}

void hwr()
{
	string trainDataFolder = "trainingDigits";
	string testDataFolder = "testDigits";
	em::EasyMat mat(32 * 32 + 1);
	em::EasyMat testMat(32 * 32 + 1);
	load(mat, trainDataFolder);
	load(testMat, testDataFolder);
	
	em::EasyVec result = trainData(10, mat, testMat);
	unsigned long correctCounts = 0;
	for (unsigned long i = 0; i < result.size();i++)
	{
		if (result[i] == testMat(i, 32 * 32))
			correctCounts++;
	}
	cout << "accuracy is :" << (double)correctCounts / result.size() << endl;
}