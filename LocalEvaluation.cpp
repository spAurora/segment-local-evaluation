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
#include <algorithm>
#include <forward_list>
#include <cmath>
#include <time.h>
#include <string.h>
#include "highgui.h"
#include "cv.h"

#include "LocalEvaluation.h"
#include "Check.h"
#include "OutputData.h"

using namespace std;
using namespace cv;

#pragma comment(linker, "/STACK:102400000,102400000")    //防止栈溢出

int main()
{
	int width, height;
	Mat srimg, RG_img;

	srimg = imread("./test_data/test_img.png",1);
	if (srimg.empty())
	{
		printf("Can not open Image\n");
		system("pause");
		exit(0);
	}
	width = srimg.cols;
	height = srimg.rows;

	RG_img = imread("./test_data/RG.bmp",0);
	if (RG_img.empty())
	{
		printf("Can not open Image\n");
		system("pause");
		exit(0);
	}
	int *rg_labels = new int[height*width]; //参考地物对象图层
	for(int i = 0; i<height; i++)
		for(int j = 0; j<width; j++)
			rg_labels[i*width+j] = (int)RG_img.data[i*width+j];

	int *labels = new int[height*width];    //以labels方式储存的分割结果
	FILE* fp; 
	if((fp = fopen("./test_data/RS.txt", "r+")) == NULL)
	{
		printf("无法打开分割结果文件\n");
		exit(-1);
	}
	for(int i = 0; i<height; i++)
		for(int j = 0; j<width; j++)
			fscanf(fp, "%d", &labels[i*width+j]);
    
	printf("calculating...\n");

	/*统计区域总数*/
	int regionNum = CalculateRegionNum(labels, width, height);
	/*建立区域集合、统计区域信息、建立区域拓扑图*/
	CRegion* cRegion = new CRegion[regionNum]; 
	ArrayHeadGraphNode *head = new ArrayHeadGraphNode[regionNum];
	CreateRegionSet(labels, srimg, cRegion, regionNum, width, height);
	CreateToplogicalGraph(labels, head, regionNum, width, height);
	/*建立参考地物对象集合*/
	vector<CGeoObject> cGeoObject;
	CreateGeoObjectSet(rg_labels, cGeoObject, width, height);
	/*像素排序*/
	SortPixel(cGeoObject, cRegion, regionNum);
	/*匹配区域和参考地物对象*/
	MatchRegionAndGeoObject(cGeoObject, cRegion, regionNum);
	/*计算区域和地物对象的信息 包括OSE、USE*/
	SetRegionAndGeoObjectInfo(cGeoObject, cRegion, regionNum, 0.75);
	/*计算GOSE和GUSE*/
	double GOSE = 0, GUSE = 0;
	CalcualteGOSEAndGUSE(cGeoObject, GOSE, GUSE);
	/*输出OSE和USE*/
	OutputDataToMatlabVisualization(cGeoObject);
	OutputOSEUSEVisualization(srimg, cGeoObject, width, height);

	/*检查结果*/
	CheckRG(rg_labels, width, height);
	CheckRegionNum(regionNum);
	CheckRegionSet(cRegion);
	CheckGplot(head);
	CheckGeoObject(cGeoObject);
	CheckSort(cGeoObject, cRegion);
	CheckMatchRegion(cGeoObject);
	CheckES(cGeoObject);
	CheckOSEUSE(cGeoObject);
	CheckGOSEAndGUSE(GOSE, GUSE);

	/*释放内存*/
	delete[] rg_labels;
	delete[] labels;
	delete[] cRegion;
	delete[] head;

	system("pause");
	return 0;
}