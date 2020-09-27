//////////////////////////////////////////////////////////////////////////
//	code by wHy
//  Aerospace Information Research Institute, Chinese Academy of Sciences
//	751984964@qq.com
//////////////////////////////////////////////////////////////////////////

//using namespace std;
using namespace cv;

class CRegion
{
public:
	int id;   //������
	vector<int> pixelLocation;   //����ĸ�������λ��

	int pixelNum;   //�������������
	int borderLength; //�߳�

	CRegion()  //�޲ι���
	{
		id = -1;

		pixelNum = 0;
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
	int pixelNum;              //������
	vector<int> cmpRegion;     //��Ӧ������
	vector<bool> iFEG;         //�Ƿ�Ϊ��Ч�ָ�
	vector<int> matchPixel;    //��Ӧ����ƥ���������
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
	*/
	for (int i = 0; i<regionNum; i++)
		cRegion[i].id = i;
	for (int i = 0;i<height;i++)
		for (int j = 0;j<width;j++)
		{
			cRegion[labels[i*width + j]].pixelLocation.push_back(i*width+j);
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

void SortPixel(vector<CGeoObject> & cGeoObject, CRegion* cRegion, int regionNum)
{
	/*
	*������Ͳο�������������ؽ�������
	*����ȡ������ʱ�临�Ӷ�
	*/
	for (int i = 0; i<regionNum; i++)
		sort(cRegion[i].pixelLocation.begin(), cRegion[i].pixelLocation.end());
	vector<CGeoObject>::iterator it;
	for (it = cGeoObject.begin(); it!= cGeoObject.end(); it++)
		sort(it->pixelLocation.begin(), it->pixelLocation.end());
}

void MatchRegionAndGeoObject(vector<CGeoObject> & cGeoObject, CRegion* cRegion, int regionNum)
{
	/*
	*ƥ������Ͳο��������
	*/
	vector<CGeoObject>::iterator it;
	for (it = cGeoObject.begin(); it!= cGeoObject.end(); it++)
	{
		for (int i = 0; i<regionNum; i++)
		{
			if (*(cRegion[i].pixelLocation.end() - 1) < *(it->pixelLocation.begin()) || *(it->pixelLocation.end() - 1) < *(cRegion[i].pixelLocation.begin()))
				continue;    //�޽���ֱ������
			vector<int>::iterator it_1 = it->pixelLocation.begin();
			vector<int>::iterator it_2 = cRegion[i].pixelLocation.begin();
			while(it_1 != it->pixelLocation.end() && it_2 != cRegion[i].pixelLocation.end())  //Ѱ����ͬ������
			{
				if (*it_1 > *it_2)
				{
					it_2++;
					continue;
				}
				if (*it_2 > *it_1)
				{
					it_1++;
					continue;
				}
				if (*it_1 == *it_2)   //���ƥ�� ��䲢�˳�
				{
					it->cmpRegion.push_back(cRegion[i].id);
					break;
				}
			}
		}
	}
}


void SetRegionAndGeoObjectInfo(vector<CGeoObject> & cGeoObject, CRegion* cRegion, int regionNum, double TES)
{
	/*
	*��������͵���������Ϣ
	*�������͵�����󹹽���ͳһ����
	*����ES
	*/
	for (int i = 0; i<regionNum; i++)
		cRegion[i].pixelNum = cRegion[i].pixelLocation.size();
	vector<CGeoObject>::iterator it;
	for (it = cGeoObject.begin(); it!= cGeoObject.end(); it++)
		it->pixelNum = it->pixelLocation.size();

	 
	for (int i = 0; i<cGeoObject.size(); i++)
	{
		//vector<int>::iterator it_1 = cGeoObject[i].pixelLocation.begin();   //�����λ��
		for (int j = 0; j<cGeoObject[i].cmpRegion.size(); j++)
		{
			cGeoObject[i].matchPixel.push_back(0);

			vector<int>::iterator it_1 = cGeoObject[i].pixelLocation.begin();
			vector<int>::iterator it_2 = cRegion[cGeoObject[i].cmpRegion[j]].pixelLocation.begin();	
			

			while(it_1 != cGeoObject[i].pixelLocation.end() && it_2 != cRegion[cGeoObject[i].cmpRegion[j]].pixelLocation.end()) //cGeoObject[i].cmpRegion[j]Ϊ�±�
			{
				if (*it_1 > *it_2)
				{
					it_2++;
					continue;
				}
				if (*it_2 > *it_1)
				{
					it_1++;
					continue;
				}
				if (*it_1 == *it_2)   //ͳ��
				{
					cGeoObject[i].matchPixel[j]++;   
					it_1++;
					it_2++;
				}
			}
		}
	}
	for (int i = 0; i<cGeoObject.size(); i++)
		for (int j = 0; j<cGeoObject[i].cmpRegion.size(); j++)
		{
			if ((double)cGeoObject[i].matchPixel[j] /(double) cRegion[cGeoObject[i].cmpRegion[j]].pixelNum >= TES)
				cGeoObject[i].iFEG.push_back(true);
			else
				cGeoObject[i].iFEG.push_back(false);
		}
}