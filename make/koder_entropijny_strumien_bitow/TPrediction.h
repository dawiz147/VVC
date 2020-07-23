#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
using namespace std;
#include <iostream>
class TPrediction
{
private:
  int** m_piImageMatrix;
  int** m_piPredictionMatrix;
  int** m_piLeftToRight;
  int** m_piTopToBottom;
  int** m_piLeftCorner;
  int m_iWidth;
  int m_iHeight;
  int m_iBlockSize;
public:
  TPrediction(int** piImageMatrix, int iWidth, int iHeight, int iBlockSize);
  void PredictionLeftToRight();
  void PredictionTopToBottom();
  void PredicitionLeftCorner();
};
