#pragma once
#ifndef _KNN_H_
#define _KNN_H_

void hwr();
em::EasyVec trainData(unsigned long k, const em::EasyMat& trainData, const em::EasyMat testData);

#endif // !_KNN_H_
