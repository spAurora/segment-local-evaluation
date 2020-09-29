//////////////////////////////////////////////////////////////////////////
//	code by wHy
//  Aerospace Information Research Institute, Chinese Academy of Sciences
//	751984964@qq.com
//////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace cv;

void OutputDataToMatlabVisualization(vector<CGeoObject> & cGeoObject)
{
	/*
	*输出用于matlab绘制OSEUSE散点图的数据
	*/
	FILE *fp;
	if((fp = fopen("./output/output_to_matlab.txt", "w+")) == NULL)
	{
		printf("打开数据输出失败\n");
		exit(-1);
	}
	for (int i = 0; i<cGeoObject.size(); i++)
		fprintf(fp, "%d %d %lf %lf %lf\n", cGeoObject[i].anchor_x+1, cGeoObject[i].anchor_y+1, cGeoObject[i].A, cGeoObject[i].OSE, cGeoObject[i].USE);
}

void OutputOSEUSEVisualization(Mat & srimg, vector<CGeoObject> & cGeoObject, int width, int height)
{
	/*
	*输出OSE和USE的可视化图像
	*/
	Mat img_OSE = srimg.clone();
	Mat img_USE = srimg.clone();
	for (int i = 0; i<height; i++)         //先全部设置成黑色
		for (int j = 0;j<width; j++)
		{
			img_OSE.data[(i*width + j)*3] = 0;
			img_OSE.data[(i*width + j)*3 + 1] = 0;
			img_OSE.data[(i*width + j)*3 + 2] = 0;

			img_USE.data[(i*width + j)*3] = 0;
			img_USE.data[(i*width + j)*3 + 1] = 0;
			img_USE.data[(i*width + j)*3 + 2] = 0;
		}
	for (int i = 0; i<cGeoObject.size(); i++)
	{
		uchar B_OSE = (uchar) ((1-cGeoObject[i].OSE)*255);
		uchar G_OSE = (uchar) ((1-cGeoObject[i].OSE)*255);
		uchar B_USE = (uchar) ((1-cGeoObject[i].USE)*255);
		uchar G_USE = (uchar) ((1-cGeoObject[i].USE)*255);
		uchar R = 255;  //红色保持255不变
		for (int j = 0; j<cGeoObject[i].pixelLocation.size(); j++)
		{
			img_OSE.data[cGeoObject[i].pixelLocation[j]*3] = B_OSE;
			img_OSE.data[cGeoObject[i].pixelLocation[j]*3 + 1] = G_OSE;
			img_OSE.data[cGeoObject[i].pixelLocation[j]*3 + 2] = R;

			img_USE.data[cGeoObject[i].pixelLocation[j]*3] = B_USE;
			img_USE.data[cGeoObject[i].pixelLocation[j]*3 + 1] = G_USE;
			img_USE.data[cGeoObject[i].pixelLocation[j]*3 + 2] = R;
		}
	}
	imwrite("./output/OSE.bmp", img_OSE);
	imwrite("./output/USE.bmp", img_USE);
}