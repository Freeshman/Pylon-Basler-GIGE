# Pylon-Basler-GIGE
A light pylon basler gige camera capture function (for Qt or others)
# Usage
1.Read the camera_vx.h and add the necessary infomation into the Qt pro file
2.Add the .h and .cpp files to your project
3.Inite the camera by calling the Camera_Init() function
4.Capture the Image by calling the Capture(QString filename) function
***Note:***Version_0 will print the detail information of the camera and pixel in the console
           Version_1 and Version_2 ignore the detail information. But Version_2 support the filename with the directory in    the Capture function. 
           Ex.For Version_1:Capture("xxxx");
              For Version_2:Capture("test/xxxx") 
