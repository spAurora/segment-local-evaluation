//////////////////////////////////////////////////////////////////////////
//	code by wHy
//  Aerospace Information Research Institute, Chinese Academy of Sciences
//	751984964@qq.com
//////////////////////////////////////////////////////////////////////////

#define min(a, b) ((a)<(b)?(a):(b))

using namespace std;
using namespace cv;

class CRegion
{
public:
	int id;   //区域编号
	vector<int> pixelLocation;   //区域的各个像素位置

	int pixelNum;   //像素数（面积）
	int borderLength; //边长

	int maxIntersectionPixelNum;   //用于计算Precision

	CRegion()  //无参构造
	{
		id = -1;

		pixelNum = 0;
		borderLength = 0;

		maxIntersectionPixelNum = 0;
	}
protected:
private:
};

class CGeoObject
{
public:
	int id;
	vector<int> pixelLocation;
	
	int anchor_x;     //锚定该区域
	int anchor_y;     
	int pixelNum;              //像素数
	vector<int> cmpRegion;     //对应的区域
	vector<bool> iFEG;         //是否为有效分割
	vector<int> matchPixel;    //对应区域匹配的像素数 等价于取交集
	
	vector<int> intersectionSet;   //与区域的交集像素数

	double OSE;
	double A;                   //等价于面积
	double f;                   
	int MAX_intersection;       //最大交集数  
	
	int lost;
	int extra; 
	double USE;



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
	/*
	*计算区域总数
	*/
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


void FillPixel(CGeoObject* temp_GeoObject, int* rg_labels_cpy, int width, int height, int i, int j)
{
	/*
	*递归向参考地物对象中填充像素
	*参考地物对象为四连通
	*/
	rg_labels_cpy[i*width+j] = 0;                        //把当前像素值改为背景值
	temp_GeoObject->pixelLocation.push_back(i*width+j);  //像素填充
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

void CreateGeoObjectSet(int* rg_labels, vector<CGeoObject> & cGeoObject, int width, int height)
{
	/*
	*建立参考地物对象集合
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
					RG_id++;			//增加一个参考地物对象
					CGeoObject temp_GeoObject;
					temp_GeoObject.anchor_x = i;
					temp_GeoObject.anchor_y = j;
					temp_GeoObject.id = RG_id;    //临时对象填充ID
					FillPixel(&temp_GeoObject, rg_labels_cpy, width, height, i, j);   //把所有邻接像素装载进该参考地物对象中
					cGeoObject.push_back(temp_GeoObject);//临时对象加入容器
					goto here;          //跳出双重循环
				}
			}
		over = true;   //无额外参考地物
		here:
			{
				
			}
	} while (over == false);
}

void SortPixel(vector<CGeoObject> & cGeoObject, CRegion* cRegion, int regionNum)
{
	/*
	*将区域和参考地物包含的像素进行排序
	*降低取并集的时间复杂度
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
	*匹配区域和参考地物对象
	*/
	vector<CGeoObject>::iterator it;
	for (it = cGeoObject.begin(); it!= cGeoObject.end(); it++)
	{
		for (int i = 0; i<regionNum; i++)
		{
			if (*(cRegion[i].pixelLocation.end() - 1) < *(it->pixelLocation.begin()) || *(it->pixelLocation.end() - 1) < *(cRegion[i].pixelLocation.begin()))
				continue;    //无交集直接跳过
			vector<int>::iterator it_1 = it->pixelLocation.begin();
			vector<int>::iterator it_2 = cRegion[i].pixelLocation.begin();
			while(it_1 != it->pixelLocation.end() && it_2 != cRegion[i].pixelLocation.end())  //寻找相同的像素
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
				if (*it_1 == *it_2)   //完成匹配 填充并退出
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
	*设置区域和地物对象的信息
	*完成区域和地物对象构建后统一计算
	*计算ES
	*计算OSE和USE
	*/
	for (int i = 0; i<regionNum; i++)
		cRegion[i].pixelNum = cRegion[i].pixelLocation.size();
	vector<CGeoObject>::iterator it;
	for (it = cGeoObject.begin(); it!= cGeoObject.end(); it++)
		it->pixelNum = it->pixelLocation.size();

	 
	for (int i = 0; i<cGeoObject.size(); i++)
	{
		//vector<int>::iterator it_1 = cGeoObject[i].pixelLocation.begin();   //错误的位置
		for (int j = 0; j<cGeoObject[i].cmpRegion.size(); j++)
		{
			cGeoObject[i].matchPixel.push_back(0);

			vector<int>::iterator it_1 = cGeoObject[i].pixelLocation.begin();
			vector<int>::iterator it_2 = cRegion[cGeoObject[i].cmpRegion[j]].pixelLocation.begin();	
			

			while(it_1 != cGeoObject[i].pixelLocation.end() && it_2 != cRegion[cGeoObject[i].cmpRegion[j]].pixelLocation.end()) //cGeoObject[i].cmpRegion[j]为下标
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
				if (*it_1 == *it_2)   //统计
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
	
	/*OSE*/
	for (int i = 0; i<cGeoObject.size(); i++)
	{
		cGeoObject[i].A = cGeoObject[i].pixelNum;
		cGeoObject[i].f = cGeoObject[i].A/(cGeoObject[i].A - 1);
		cGeoObject[i].MAX_intersection = 0;
		for (int j =0; j<cGeoObject[i].cmpRegion.size(); j++)
		{
			cGeoObject[i].intersectionSet.push_back(cGeoObject[i].matchPixel[j]);
			if (cGeoObject[i].iFEG[j] == true && cGeoObject[i].MAX_intersection < cGeoObject[i].matchPixel[j])
				cGeoObject[i].MAX_intersection = cGeoObject[i].matchPixel[j];
		}
		if (cGeoObject[i].MAX_intersection == 0)        //默认无错误分割
			cGeoObject[i].OSE = 0;
		else
			cGeoObject[i].OSE = cGeoObject[i].f * (1 - cGeoObject[i].MAX_intersection/cGeoObject[i].A);
	}
	
	/*USE*/
	for (int i = 0; i<cGeoObject.size(); i++)
	{
		int total_intersection = 0;
		for (int j = 0; j<cGeoObject[i].cmpRegion.size(); j++)
			if (cGeoObject[i].iFEG[j] == true)
				total_intersection += cGeoObject[i].intersectionSet[j];
		cGeoObject[i].lost = cGeoObject[i].A - total_intersection;

		cGeoObject[i].extra = 0;
		for (int j = 0; j<cGeoObject[i].cmpRegion.size(); j++)
			if (cGeoObject[i].iFEG[j] == true)
				cGeoObject[i].extra += cRegion[cGeoObject[i].cmpRegion[j]].pixelNum - cGeoObject[i].intersectionSet[j];

		cGeoObject[i].USE = (double)min(cGeoObject[i].lost + cGeoObject[i].extra, cGeoObject[i].A) / cGeoObject[i].A;
	}



}

void CalcualteGOSEAndGUSE(vector<CGeoObject> & cGeoObject, double & GOSE, double & GUSE)
{
	/*
	*计算GOSE和GUSE
	*/
	GOSE = 0;
	GUSE = 0;
	int At = 0;
	for (int i = 0; i<cGeoObject.size(); i++)
	{	
		At += cGeoObject[i].A;
		GOSE += cGeoObject[i].A * cGeoObject[i].OSE; 
		GUSE += cGeoObject[i].A * cGeoObject[i].USE;
	}
	GOSE /= At;
	GUSE /= At;
}

void CalculatePrecisionRecall(CRegion* cRegion, vector<CGeoObject> & cGeoObject,int regionNum, double & Precision, double & Recall, double & F1)
{
	/*
	*计算Precision And Recall
	*/
	/*Precision*/
	int sum_Pixel = 0, sum_MaxIntersectionPixel = 0;
	for (int i = 0; i < cGeoObject.size(); i++)   //计算每个区域与各个参考地物最大的交集面积
		for (int j = 0; j < cGeoObject[i].cmpRegion.size(); j++)
			if (cRegion[cGeoObject[i].cmpRegion[j]].maxIntersectionPixelNum < cGeoObject[i].matchPixel[j])
				cRegion[cGeoObject[i].cmpRegion[j]].maxIntersectionPixelNum = cGeoObject[i].matchPixel[j];
	for (int i = 0; i<regionNum; i++)
		if (cRegion[i].maxIntersectionPixelNum != 0)
		{
			sum_Pixel += cRegion[i].pixelNum;
			sum_MaxIntersectionPixel += cRegion[i].maxIntersectionPixelNum;
		}
	Precision = (double)sum_MaxIntersectionPixel/(double)sum_Pixel;

	/*Recall*/
	int sum_R_Pixel = 0, sum_R_MaxIntersectionPixel = 0;
	for (int i = 0; i<cGeoObject.size(); i++)
	{
		sum_R_Pixel += cGeoObject[i].pixelNum;
		sum_R_MaxIntersectionPixel += cGeoObject[i].MAX_intersection;
	}
	Recall = (double)sum_R_MaxIntersectionPixel / (double)sum_R_Pixel;

	F1 = 2*Precision*Recall/(Precision+Recall);
}