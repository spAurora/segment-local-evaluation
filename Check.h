//////////////////////////////////////////////////////////////////////////
//	code by wHy
//  Aerospace Information Research Institute, Chinese Academy of Sciences
//	751984964@qq.com
//////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace cv;

void CheckRegionNum(int regionNum)
{
	printf("-----------\n");
	printf("check regionNum:%d\n", regionNum);
}

void CheckRG(int* rg_labels, int width, int height)
{
	int count_0 = 0, count_255 = 0;
	for (int i = 0; i<height; i++)
		for (int j = 0; j<width;j++)
		{
			if (rg_labels[i*width+j] == 0)
				count_0++;
			else if (rg_labels[i*width+j] == 255)
				count_255++;
		}
	printf("-----------\n");
	printf("check RG:\n");
	printf("count 0 = %d\n", count_0);
	printf("count 255 = %d\n", count_255);
}

void CheckRegionSet(CRegion* cRegion)
{
	printf("-----------\n");
	printf("check region set:\nregion_0 pixelNum:%d\n", cRegion[0].pixelNum);
}

void CheckGplot(ArrayHeadGraphNode* head)
{
	printf("-----------\n");
	printf("check gplot:\n");
	vector<GraphNode>::iterator it;
	for (it = head[0].pGraphNodeList.begin(); it!= head[0].pGraphNodeList.end(); it++)
		printf("%d -> ", it->ID);
	printf("\n");
	for (it = head[1].pGraphNodeList.begin(); it!= head[1].pGraphNodeList.end(); it++)
		printf("%d -> ", it->ID);
	printf("\n");
	for (it = head[2].pGraphNodeList.begin(); it!= head[2].pGraphNodeList.end(); it++)
		printf("%d -> ", it->ID);
	printf("\n");
}

void CheckGeoObject(vector<CGeoObject> & cGeoObject)
{
	printf("-----------\n");
	printf("RG num:%d\n", cGeoObject.size());
	printf("cGeoObject[0] size:%d\n", cGeoObject[0].pixelLocation.size());
}

void CheckSort(vector<CGeoObject> & cGeoObject, CRegion* cRegion, int checkNum = 5)
{
	printf("-----------\n");
	printf("check sort result:\n");
	printf("cGeoObject[0] pixel location:");
	if (!cGeoObject.empty())
	{
		vector<int>::iterator it;
		int ctl = 0;
		for (it = cGeoObject[0].pixelLocation.begin(); it!= cGeoObject[0].pixelLocation.end(); it++)
		{
			ctl++;
			printf("%d->", *it);
			if(ctl == checkNum)
				break;
		}
		printf("\n");
	}
	printf("cRegion[0] pixel location:");
	vector<int>::iterator it;
	int ctl = 0;
	for (it = cRegion[0].pixelLocation.begin(); it != cRegion[0].pixelLocation.end(); it++)
	{
		ctl++;
		printf("%d->", *it);
		if(ctl == checkNum)
			break;
	}
	printf("\n");
}

void CheckMatchRegion(vector<CGeoObject> & cGeoObject, int checkNum = 1024)
{
	printf("-----------\n");
	printf("check match region:\n");
	vector<int>::iterator it;
	int ctl = 0;
	for (it = cGeoObject[0].cmpRegion.begin(); it != cGeoObject[0].cmpRegion.end(); it++)
	{
		ctl++;
		printf("%d->", *it);
		if(ctl == checkNum)
			break;
	}
	printf("\n");
}

void CheckES(vector<CGeoObject> & cGeoObject, int checkNum = 1024)
{
	printf("-----------\n");
	printf("check ES:\n");
	int ctl = 0;
	for (int j = 0; j<cGeoObject.size(); j++)
	{
		for (int i = 0; i<cGeoObject[j].iFEG.size(); i++)
			if (cGeoObject[j].iFEG[i] == true)
				printf("true->");
		else
			printf("false->");
		printf("\n");
		ctl++;
		if(ctl == checkNum)
			break;
	}
}

void CheckOSEUSE(vector<CGeoObject> & cGeoObject, int checkNum = 1024)
{
	printf("-----------\n");
	printf("check OSE:\n");
	int ctl = 0;
	for (int j = 0; j<cGeoObject.size(); j++)
	{
		printf("%.3lf\n", cGeoObject[j].OSE);
		ctl++;
		if(ctl == checkNum)
			break;
	}
	printf("\n");
	ctl = 0;
	printf("check USE:\n");
	for (int j = 0; j<cGeoObject.size(); j++)
	{
		printf("%.3lf\n", cGeoObject[j].USE);
		ctl++;
		if(ctl == checkNum)
			break;
	}
}

void CheckGOSEAndGUSE(double GOSE, double GUSE)
{
	printf("-----------\n");
	printf("GOSE = %.3lf, GUSE = %.3lf\n", GOSE, GUSE);
}

void CheckPrecisionRecall(double Precision, double Recall, double F1)
{
	printf("-----------\n");
	printf("PRECISION = %.3lf, RECALL = %.3lf F1 = %.3lf\n", Precision, Recall, F1);
}