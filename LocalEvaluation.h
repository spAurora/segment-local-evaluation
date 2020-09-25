//////////////////////////////////////////////////////////////////////////
//	code by wHy
//  Aerospace Information Research Institute, Chinese Academy of Sciences
//	751984964@qq.com
//////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace cv;

class CRegion
{
public:
	int id;   //区域编号
	vector<int> poxelLocation;   //区域的各个像素位置

	int pixelnum;   //像素数（面积）
	int borderLength; //边长

	CRegion()  //无参构造
	{
		id = -1;

		pixelnum = 0;
		borderLength = 0;
	}
protected:
private:
};

class GraphNode
{
	public:
	int ID; //*初始化
	GraphNode()
	{
		ID = -1;
	}
	GraphNode(int mID)
	{
		ID = mID;
	}
};

class ArrayHeadGraphNode  //头结点数组
{
public:
	vector<GraphNode> pGraphNodeList;		//邻接拓扑点
protected:
private:
};

int CalculateRegionNum(int* labels, int width, int height)
{
	int regionNum = 0;
	for(int i = 0; i<height; i++)
		for(int j = 0; j<width; j++)
			if (labels[i*width + j] > regionNum)
				regionNum = labels[i*width + j];
	regionNum++;   //区域0开始计数,所以总数要+1
	return regionNum;
}

void CreateRegionSet(int* labels, Mat &srimg, CRegion* cRegion, int regionNum, int width, int height)
{
	/*
	*创建区域集合
	*初步统计区域信息
	*/
	for (int i = 0;i<height;i++)
		for (int j = 0;j<width;j++)
		{
			cRegion[labels[i*width + j]].poxelLocation.push_back(i*width+j);
		}

	for (int i = 0; i<regionNum; i++)
	{
		cRegion[i].pixelnum = cRegion[i].poxelLocation.size();
	}
}

void CreateToplogicalGraph(int* labels, ArrayHeadGraphNode* head, int regionNum, int width, int height)
{
	/*
	*创建区域的拓扑图
	*/
	for (int i = 0; i<height-1; i++)
		for (int j = 0; j<width-1; j++)
		{
			if (labels[i*width + j] != labels[i*width + j + 1])    //横向
			{
				vector<GraphNode>::iterator it;
				int check = 0; //0为不存在
				for (it = head[labels[i*width + j]].pGraphNodeList.begin(); it != head[labels[i*width + j]].pGraphNodeList.end(); it++)
					if (it->ID == labels[i*width+j+1])
					{
						check = 1;
						break;
					}
				if (check == 0)
				{
					head[labels[i*width + j]].pGraphNodeList.push_back(labels[i*width + j + 1]);
					head[labels[i*width + j + 1]].pGraphNodeList.push_back(labels[i*width + j]);
				}
			}

			if (labels[i*width + j] != labels[(i+1)*width+j])  //纵向
			{
				vector<GraphNode>::iterator it;
				int check = 0; //0为不存在
				for (it = head[labels[i*width + j]].pGraphNodeList.begin(); it != head[labels[i*width + j]].pGraphNodeList.end(); it++)
					if (it->ID == labels[(i+1)*width+j])
					{
						check = 1;
						break;
					}
				if (check == 0)
				{
					head[labels[i*width + j]].pGraphNodeList.push_back(labels[(i+1)*width + j]);
					head[labels[(i+1)*width + j]].pGraphNodeList.push_back(labels[i*width + j]);
				}
			}
		}
}