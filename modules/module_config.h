/*
Author: QIN Shuo
Organization: RC-MIC (CUHK)
Date: 2017/6/5

Description:
	Module configuration flags

*/

#ifndef _MODULE_CONFIG_H_
#define _MODULE_CONFIG_H_



#define DEBUG_MODE

#define Seed_Update_Event 9999

#define STD_COUT(X) std::cout<<X<<std::endl;

#define PI 3.14159265358979323846
#define ANG2RAD(ang) 180.0/PI


enum View_Direction
{
	Frontal,
	Profile
};



#endif // !_MODULE_CONFIG_H_

