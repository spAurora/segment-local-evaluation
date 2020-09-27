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

using namespace std;
using namespace cv;

#pragma comment(linker, "/STACK:102400000,102400000")    //��ֹջ���

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
	int *rg_labels = new int[height*width]; //�ο��������ͼ��
	for(int i = 0; i<height; i++)
		for(int j = 0; j<width; j++)
			rg_labels[i*width+j] = (int)RG_img.data[i*width+j];
	CheckRG(rg_labels, width, height);

	int *labels = new int[height*width];    //��labels��ʽ����ķָ���
	FILE* fp; 
	if((fp = fopen("./test_data/RS.txt", "r+")) == NULL)
	{
		printf("�޷��򿪷ָ����ļ�\n");
		exit(-1);
	}
	for(int i = 0; i<height; i++)
		for(int j = 0; j<width; j++)
			fscanf(fp, "%d", &labels[i*width+j]);
    
	/*ͳ����������*/
	int regionNum = CalculateRegionNum(labels, width, height);
	CheckRegionNum(regionNum);

	/*�������򼯺ϡ�ͳ��������Ϣ��������������ͼ*/
	CRegion* cRegion = new CRegion[regionNum]; 
	ArrayHeadGraphNode *head = new ArrayHeadGraphNode[regionNum];

	CreateRegionSet(labels, srimg, cRegion, regionNum, width, height);
	CreateToplogicalGraph(labels, head, regionNum, width, height);
	CheckRegionSet(cRegion);
	CheckGplot(head);

	/*�����ο�������󼯺�*/
	vector<CGeoObject> cGeoObject;
	createGeoObjectSet(rg_labels, cGeoObject, width, height);
	CheckGeoObject(cGeoObject);
	/*��������*/
	SortPixel(cGeoObject, cRegion, regionNum);
	CheckSort(cGeoObject, cRegion);
	/*ƥ������Ͳο��������*/
	MatchRegionAndGeoObject(cGeoObject, cRegion, regionNum);
	CheckMatchRegion(cGeoObject);
	/*��������͵���������Ϣ*/
	SetRegionAndGeoObjectInfo(cGeoObject, cRegion, regionNum, 0.75);
	CheckES(cGeoObject);
	CheckOSEUSE(cGeoObject);

	system("pause");
	return 0;
}