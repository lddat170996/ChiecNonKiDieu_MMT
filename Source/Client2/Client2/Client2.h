#pragma once

#include "resource.h"
#include "afxSock.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <conio.h>
using namespace std;

class KNClient
{
private:
	CSocket  Client;
public:
	void RegisterName(); //đăng kí tên cho user
	void InitChat(); //thiết lập kết nối với server
	bool recieve(); //nhận tất cả chuoi của server và in ra màn hình
	void Answer(); // trl câu hỏi
};