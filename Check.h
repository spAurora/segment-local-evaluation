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

void CheckRegionSet(CRegion* cRegion)
{
	printf("-----------\n");
	printf("check region set:\n region_0 pixelnum:%d\n", cRegion[0].pixelnum);
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