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
	printf("check region set:\nregion_0 pixelnum:%d\n", cRegion[0].pixelnum);
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