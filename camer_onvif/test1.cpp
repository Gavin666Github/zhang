#include <iostream>    
#include <opencv2/highgui/highgui.hpp>    
#include <opencv2/objdetect/objdetect.hpp>    
#include <opencv2/ml/ml.hpp>    
#include <Windows.h>  
  

#pragma comment( lib, "opencv_highgui249d.lib")
#pragma comment(lib,"opencv_imgproc249d.lib")  
#pragma comment(lib, "opencv_core249.lib")
#pragma comment(lib,"opencv_contrib249d.lib")
#pragma comment(lib,"opencv_objdetect249d.lib")   
#pragma comment(lib,"opencv_photo249d.lib")   
#pragma comment(lib,"opencv_nonfree249d.lib")   
#pragma comment(lib,"opencv_features2d249d.lib")   
using namespace std;    
using namespace cv;    
  
int main()    
{    
    Mat src = imread("1.jpg",1);    
    vector<Rect> personSVM, personCasc,personListSVM, personListCacs;//检测结果矩形框向量  
    DWORD SVMTimeBegin,SVMTimeEnd,CascTimeBegin,CascTimeEnd;  //耗时统计      
  
    //方法1，Hog+svm    
    //SVMTimeBegin = GetTickCount();    
    //HOGDescriptor hog;//HOG特征检测器    
    //hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());//设置SVM分类器为默认参数       
    //hog.detectMultiScale(src, personSVM, 0, Size(2,2), Size(0,0), 1.05, 2);//对图像进行多尺度检测  
    //SVMTimeEnd=GetTickCount();  
    //cout<<"HOG+SVM行人检测耗时：\n"<<(SVMTimeEnd-SVMTimeBegin)<<endl;  
  
    //方法2.Hog+cascade    
    CascTimeBegin=GetTickCount();    
    CascadeClassifier *cascade = new CascadeClassifier;    
    cascade->load("C://opencv//sources//data//hogcascades//hogcascade_pedestrians.xml");    
    cascade->detectMultiScale(src, personCasc);    
    CascTimeEnd=GetTickCount();    
    cout<<"HOG+Cascade行人检测耗时：\n"<<(CascTimeEnd-CascTimeBegin)<<endl;  
  
    //不重合的直接放入List，重合的选取最外侧结果    
    for(int i=0; i < personSVM.size(); i++)    
    {    
        Rect r = personSVM[i];    
        int j=0;    
        for(; j < personSVM.size(); j++)    
            if(j != i && (r & personSVM[j]) == r)    
                break;    
        if( j == personSVM.size())    
            personListSVM.push_back(r);    
    }    
    for(int i=0; i < personCasc.size(); i++)    
    {    
        Rect r = personCasc[i];    
        int j=0;    
        for(; j < personCasc.size(); j++)    
            if(j != i && (r & personCasc[j]) == r)    
                break;    
        if( j == personCasc.size())    
            personListCacs.push_back(r);    
    }    
  
    //画矩形框，缩放检测到的矩形框   
    for(int i=0; i<personListSVM.size(); i++)    
    {    
        Rect r = personListSVM[i];    
        r.x += cvRound(r.width*0.1);    
        r.width = cvRound(r.width*0.8);    
        r.y += cvRound(r.height*0.07);    
        r.height = cvRound(r.height*0.8);    
        rectangle(src, r.tl(), r.br(), Scalar(0,0,255), 2);    
    }    
    for(int i=0; i<personListCacs.size(); i++)    
    {    
        Rect r = personListCacs[i];    
        r.x += cvRound(r.width*0.1);    
        r.width = cvRound(r.width*0.8);    
        r.y += cvRound(r.height*0.07);    
        r.height = cvRound(r.height*0.8);    
        rectangle(src, r.tl(), r.br(), Scalar(0,255,0), 2);    
    }    
    imshow("HOG特征+SVM/Cascade行人检测",src);    
    waitKey();   
    return 0;    
}    
