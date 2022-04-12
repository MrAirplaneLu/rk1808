/*
 * @Description: 
 * @Version: 
 * @Autor: guanzhou
 * @email: guanzhou.cn@gmail.com
 * @Date: 2022-04-08 21:08:53
 * @LastEditors: guanzhou
 * @LastEditTime: 2022-04-10 20:57:01
 */
/****************************************************/
/*                                                  */
/*                      v4lgrab.h                   */
/*                                                  */
/****************************************************/


#ifndef __V4LGRAB_H__
#define __V4LGRAB_H__

#include <iostream>
#include "opencv2/opencv.hpp"
#include <stdio.h>
#include <vector>
using namespace std;

int testFunc(int a)
{


    cv::Mat test(cv::Size(400,600),CV_8UC3,cv::Scalar(255,0,0));
	if (test.data != NULL)

	{
		cout << "size:\t" << test.cols << endl;
        cv::imwrite("./test111.jpg", test);
	}
	else
	{
		cout << "can`t open the file!" << endl;
	}

     return a + 10;
}


#endif //__V4LGRAB_H___
