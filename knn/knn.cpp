#include <iostream>
#include <fstream>
#include "EasyMat.h"
#include "EasyVec.h"
#include "knn.h"
using namespace std;

void loadData(const string& file, unsigned long col, 
	em::EasyMat& normData)
{
	normData = em::EasyMat(4);
	normData.reserve(col * 500);
	em::EasyVec vec(col);
	ifstream in(file);
	if (!in.good())
		throw "file not found";
	while (!in.eof())
	{
		for (unsigned long i = 0; i < col;i++)
		{
			if (in.eof())
				return;
			in >> vec[i];
		}
		normData.append(vec);
	}
}

void getMetaData(em::EasyMat& normData, em::EasyVec& minVec, em::EasyVec& rangeVec)
{
	unsigned long col = normData.cols();
	em::EasyVec maxVec;
	minVec = normData.min();
	maxVec = normData.max();
	rangeVec = maxVec - minVec;

	minVec[col - 1] = 0;
	rangeVec[col - 1] = 1;
	normData.sub(minVec);
	normData.div(rangeVec);
}

void normData(em::EasyMat& normData, const em::EasyVec& minVec, const em::EasyVec& rangeVec)
{
	normData.sub(minVec);
	normData.div(rangeVec);
}

em::EasyVec trainData(unsigned long k, const em::EasyMat& trainData, const em::EasyMat testData)
{
	unsigned long row = testData.rows();
	em::EasyVec result(row, em::EM_BY_COL);
	for (unsigned long i = 0; i < row;i++)
	{
		cout << "caculating the " << i << " data..." << endl;
		em::EasyMat distanceData(trainData.rows(), 2);
		distanceData.set(1, trainData.col(trainData.cols() - 1), em::EM_BY_COL);
		em::EasyMat data(trainData);
		data.set(data.cols() - 1, 0, em::EM_BY_COL);	
		em::EasyVec sample = testData.row(i);
		sample[sample.size() - 1] = 0;
		data.sub(sample);
		data.pow(2);
		distanceData.set(0, data.sum(em::EM_BY_ROW), em::EM_BY_COL);
		distanceData.sortByRowAsc(0);
		em::EasyMat tmpMat(2);
		em::EasyVec tmpVec(2);
		for (unsigned long i = 0; i < k; i++)
		{
			bool found = false;
			for (unsigned long j = 0; j < tmpMat.rows();j++)
			{
				if (tmpMat(j, 0) == distanceData(i, 1))
				{
					tmpMat(j, 1)++;
					found = true;
					break;
				}
			}
			if (!found)
			{
				tmpVec[0] = distanceData(i, 1);
				tmpVec[1] = 1;
				tmpMat.append(tmpVec);
			}
		}
		tmpMat.sortByRowDesc(1);
		result[i] = tmpMat(0, 0);
	}
	return result;
}

void knn(unsigned long k, unsigned long col, 
	const string& trainFile, const string& testFile)
{
	em::EasyMat normTrainData;
	em::EasyVec minTrainVec;
	em::EasyVec rangeTrainVec;
	loadData(trainFile, col, normTrainData);
	getMetaData(normTrainData, minTrainVec, rangeTrainVec);
	em::EasyMat testData;
	loadData(testFile, col, testData);
	normData(testData, minTrainVec, rangeTrainVec);
	em::EasyVec result = trainData(k, normTrainData, testData);
	result.show();

	unsigned long correctCounts = 0;
	for (unsigned long i = 0; i < result.size();i++)
	{
		if (result[i] == testData(i, col - 1))
			correctCounts++;
	}
	cout << "accuracy is :" << (double)correctCounts / result.size() << endl;
}

inline void fun(double* d, int i,int size)
{
	//if (i > 0 && i < size)
		d[i] = i;
}

int main()
{
	hwr();
	return 0;
}