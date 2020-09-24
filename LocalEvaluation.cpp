//////////////////////////////////////////////////////////////////////////
//	code by wHy
//  Aerospace Information Research Institute, Chinese Academy of Sciences
//	751984964@qq.com
//////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <list>
#include <vector>
#include <cstdlib>
#include <forward_list>
#include <cmath>
#include <time.h>
#include <string.h>
#include "highgui.h"
#include "cv.h"

#include "LocalEvaluation.h"

using namespace cv;

#pragma comment(linker, "/STACK:102400000,102400000")    //防止栈溢出

int main()
{
	int width, height;
	Mat srimg;

	srimg = imread("./test_data/test_img.png",1);
	if (srimg.empty())
	{
		printf("Can not open Image\n");
		system("pause");
		exit(0);
	}
	width = srimg.cols;
	height = srimg.rows;

	int *labels = new int[height*width];    //以labels方式储存的分割结果
	
	FILE* fp; 
	if((fp = fopen("./test_data/RS.txt", "r+")) == NULL)
	{
		printf("无法打开已知阈值序列\n");
		exit(-1);
	}
	for(int i = 0; i<height; i++)
		for(int j = 0; j<width; j++)
			fscanf(fp, "%d", &labels[i*width+j]);

	int regionNum = 0;
	for(int i = 0; i<height; i++)
		for(int j = 0; j<width; j++)
			if (labels[i*width + j] > regionNum)
				regionNum = labels[i*width + j];
	regionNum++;   //区域0开始计数
	printf("check regionNum:%d\n", regionNum);

	CRegion* cRegion = new CRegion[regionNum]; 

	return 0;
}