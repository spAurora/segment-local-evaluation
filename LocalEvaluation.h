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
	vector<int> pixelLocation;   //����ĸ�������λ��

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

class CGeoObject
{
public:
	int id;
	vector<int> pixelLocation;
	int corePixelLocation;     //��������λ��
	
	vector<int> cmpRegion;     //��Ӧ������
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
	/*
	*������������
	*/
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
			cRegion[labels[i*width + j]].pixelLocation.push_back(i*width+j);
		}

	for (int i = 0; i<regionNum; i++)
	{
		cRegion[i].pixelnum = cRegion[i].pixelLocation.size();
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


void FillPixel(CGeoObject* temp_GeoObject, int* rg_labels_cpy, int width, int height, int i, int j)
{
	/*
	*�ݹ���ο�����������������
	*�ο��������Ϊ����ͨ
	*/
	rg_labels_cpy[i*width+j] = 0;                        //�ѵ�ǰ����ֵ��Ϊ����ֵ
	temp_GeoObject->pixelLocation.push_back(i*width+j);  //�������
	if (i-1 >= 0)
		if (rg_labels_cpy[(i-1)*width+j] == 255)
			FillPixel(temp_GeoObject,rg_labels_cpy, width, height, i-1, j);
	if (j-1 >= 0)
		if (rg_labels_cpy[i*width+j-1] == 255)
			FillPixel(temp_GeoObject, rg_labels_cpy, width, height, i, j-1);
	if (i+1 <= height)
		if (rg_labels_cpy[(i+1)*width+j] == 255)
			FillPixel(temp_GeoObject, rg_labels_cpy, width, height, i+1, j);
	if (j+1 <= width)
		if (rg_labels_cpy[i*width+j+1] == 255)
			FillPixel(temp_GeoObject, rg_labels_cpy, width, height, i, j+1);
}

void createGeoObjectSet(int* rg_labels, vector<CGeoObject> & cGeoObject, int width, int height)
{
	/*
	*�����ο�������󼯺�
	*/
	int* rg_labels_cpy = new int[width*height];
	for (int i = 0; i<height; i++)
		for (int j = 0; j<width; j++)
			rg_labels_cpy[i*width+j] = rg_labels[i*width+j];

	bool over = false;
	int RG_id = -1;
	do 
	{
		for (int i = 0; i<height; i++)
			for (int j = 0; j<width; j++)
			{
				if (rg_labels_cpy[i*width+j] == 255)
				{
					RG_id++;			//����һ���ο��������
					CGeoObject temp_GeoObject;
					temp_GeoObject.id = RG_id;    //��ʱ�������ID
					FillPixel(&temp_GeoObject, rg_labels_cpy, width, height, i, j);   //�������ڽ�����װ�ؽ��òο����������
					cGeoObject.push_back(temp_GeoObject);//��ʱ�����������
					goto here;          //����˫��ѭ��
				}
			}
		over = true;   //�޶���ο�����
		here:
			{
				
			}
	} while (over == false);
}