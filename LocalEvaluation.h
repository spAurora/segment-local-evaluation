//////////////////////////////////////////////////////////////////////////
//	code by wHy
//  Aerospace Information Research Institute, Chinese Academy of Sciences
//	751984964@qq.com
//////////////////////////////////////////////////////////////////////////

using namespace cv;

class CRegion
{
public:
	int id;   //区域编号
	vector<int> poxelLocation;   //区域的各个像素位置

	int pixelnum;   //像素数（面积）

	CRegion()  //无参构造
	{
		id = -1;
		pixelnum = 0;
	}
protected:
private:
};

class GraphNode
{
	public:
	int ID; //*初始化
	GraphNode(int mID)
	{
		ID = mID;
	}
};

class ArrayHeadGraphNode  //头结点数组
{
public:
	forward_list<GraphNode> pGraphNodeList;		//邻接拓扑点
protected:
private:
};

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