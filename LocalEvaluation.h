//////////////////////////////////////////////////////////////////////////
//	code by wHy
//  Aerospace Information Research Institute, Chinese Academy of Sciences
//	751984964@qq.com
//////////////////////////////////////////////////////////////////////////

using namespace cv;

class CRegion
{
public:
	int id;   //������
	vector<int> poxelLocation;   //����ĸ�������λ��

	int pixelnum;   //�������������

	CRegion()  //�޲ι���
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
	int ID; //*��ʼ��
	GraphNode(int mID)
	{
		ID = mID;
	}
};

class ArrayHeadGraphNode  //ͷ�������
{
public:
	forward_list<GraphNode> pGraphNodeList;		//�ڽ����˵�
protected:
private:
};

void CreateRegionSet(int* labels, Mat &srimg, CRegion* cRegion, int regionNum, int width, int height)
{
	/*
	*�������򼯺�
	*����ͳ��������Ϣ
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