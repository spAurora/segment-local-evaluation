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
	*�������matlab����OSEUSEɢ��ͼ������
	*/
	FILE *fp;
	if((fp = fopen("./output/output_to_matlab.txt", "w+")) == NULL)
	{
		printf("���������ʧ��\n");
		exit(-1);
	}
	for (int i = 0; i<cGeoObject.size(); i++)
		fprintf(fp, "%d %d %lf %lf %lf\n", cGeoObject[i].anchor_x+1, cGeoObject[i].anchor_y+1, cGeoObject[i].A, cGeoObject[i].OSE, cGeoObject[i].USE);
}

void OutputOSEUSEVisualization(Mat & srimg, vector<CGeoObject> & cGeoObject, int width, int height)
{
	/*
	*���OSE��USE�Ŀ��ӻ�ͼ��
	*/
	Mat img_OSE = srimg.clone();
	Mat img_USE = srimg.clone();
	for (int i = 0; i<height; i++)         //��ȫ�����óɺ�ɫ
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
		uchar R = 255;  //��ɫ����255����
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