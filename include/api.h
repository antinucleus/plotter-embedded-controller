#ifndef _API_H
#define _API_H
#include <string>

std::string getData(std::string path);
std::string postData(std::string path, std::string body);
void movePen(std::string direction);
void sendCoordinates(double x, double y);

#endif