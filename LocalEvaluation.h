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
	int id;   //������
	vector<int> poxelLocation;   //����ĸ�������λ��

	int pixelnum;   //�������������
	int borderLength; //�߳�

	CRegion()  //�޲ι���
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
	int ID; //*��ʼ��
	GraphNode()
	{
		ID = -1;
	}
	GraphNode(int mID)
	{
		ID = mID;
	}
};

class ArrayHeadGraphNode  //ͷ�������
{
public:
	vector<GraphNode> pGraphNodeList;		//�ڽ����˵�
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
	regionNum++;   //����0��ʼ����,��������Ҫ+1
	return regionNum;
}

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

void CreateToplogicalGraph(int* labels, ArrayHeadGraphNode* head, int regionNum, int width, int height)
{
	/*
	*�������������ͼ
	*/
	for (int i = 0; i<height-1; i++)
		for (int j = 0; j<width-1; j++)
		{
			if (labels[i*width + j] != labels[i*width + j + 1])    //����
			{
				vector<GraphNode>::iterator it;
				int check = 0; //0Ϊ������
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

			if (labels[i*width + j] != labels[(i+1)*width+j])  //����
			{
				vector<GraphNode>::iterator it;
				int check = 0; //0Ϊ������
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