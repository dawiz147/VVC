﻿#include "stdint.h"
#include <inttypes.h>
#include <iostream>
using namespace std;
#include <ctime>
#include <vector>
#include "TOutputBitstream.h"
#include "TInputBitstream.h"
#include "UnitTestAns.cpp"
#include "TFileReader.h"
#include "TPrediction.h"
int main()
{
  //srand(time(NULL));
  TOutputBitstream* bit_stream = new TOutputBitstream();
  TInputBitstream* load_bit_stream = new TInputBitstream();
  int iSizeBeforeEncoding = 0;
  int iSizeAfterEncoding = 0;
  /* Implementacja testów */
  TestAns();  // Testowanie poprawności działania ANS (UnitTestAns.cpp)
  //-------------------------------------------------------------------
  int iZero = 0;
  int iOne = 0;
  
  TFileReader* pFileReader = new TFileReader(176, 144, "foreman_176x144_qcif.yuv");
  TImage* pImage = pFileReader->ReadFrame();
  pImage->DCT8x8();
  pImage->ZigZag4x4();
  TPrediction* prediction = new TPrediction(pImage->GetImageMatrix(), pImage->GetWidth(), pImage->GetHeight(), 8);

  prediction->PredictionLeftToRight();





  unsigned SizeZigZag = pImage->GetSizeZigZag();
  cerr << "size Zigzag" << SizeZigZag << endl;
  TAns* ans2 = new TAns(256, 2, 16);
  vector<int>* Probability = pImage->CalculateProbability(256);
  cerr << "prawd" << endl;
  for (int i = 0; i < Probability->size(); i++)
  {
    cerr << Probability->at(i) << " ";
  }
  cerr << "prawd" << endl;
  int uiPixelValue;
  int uiP1 = 3;
  std::fstream file;
  file.open("test.bin", ios::in | ios::out | ios::binary);
  bool negative = false;
  cerr << endl;
  for (unsigned y = 0; y < SizeZigZag; y++)
    {
      uiPixelValue = pImage->GetValueZigZag();
      if (uiPixelValue<0)
      {
        uiPixelValue = abs(uiPixelValue);
        negative = true;
      }

      for (uint32_t i = 0; i < 12; i++)
      {
        iSizeBeforeEncoding++;
        uint8_t uiBit = uiPixelValue & 0x01;
        //cerr << int(uiBit) << " " ;
        //ans2->Code(Probability->at(i), uiBit);
        ans2->Code(Probability->at(i), uiBit);
        uiPixelValue = uiPixelValue >> 1;
      }
      if (negative == true)
      {
        ans2->Code(Probability->at(12), 1);
        negative = false;
      }
      else ans2->Code(Probability->at(12), 0);
    }
  file.close();
  ans2->Save();
  //cerr << "Zero: " << iZero << "One: " << iOne << endl;
  TImage* pDecodeImage = new TImage(nullptr, pImage->GetWidth(), pImage->GetHeight());
  iSizeAfterEncoding = ans2->GetBitAmout();
  cerr << "Rozmiary: " << endl;
  cerr << "Przed kodowaniem: "<< pFileReader->GetAll() <<endl;
  cerr << "Po kodowaniu: "<< iSizeAfterEncoding <<endl;
  bool Negative = false;
  for (unsigned y = 0; y < SizeZigZag; y++)
    {

       uiPixelValue = 0;
      for(uint32_t i = 0; i < 13; i++)
      {
        if (i == 0)
        {
          if (0 != ans2->Decode(Probability->at(12-i)))
          {
            Negative = true;
          }
        }
        else {
          uint8_t uiBit = ans2->Decode(Probability->at(12-i));
          //cerr << 11 - i << " ";
          //cerr << int(uiBit) << " ";
          uiPixelValue = (uiPixelValue << 1) | uiBit;
        }
      } 
      if (Negative == true)
      {
        uiPixelValue = uiPixelValue*(-1);
        Negative = false;
      }
     // cerr << uiPixelValue<<" ";

    }
 
}


