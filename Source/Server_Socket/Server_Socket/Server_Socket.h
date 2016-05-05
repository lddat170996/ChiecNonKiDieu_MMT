#pragma once
#include "afxsock.h"
#include "resource.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <time.h>
using namespace std;
//#define N_USER 2
struct User
{
	char *name;
	int score;
};
struct Question
{
	string ques, hint;
	bool asked;
	int turn;
};
class KNServer
{
private:
	CSocket Server, *Client;
	vector<User> userArr;
	vector<Question> quesArr;
	int N_USER;
public :
	void Chat();
	bool RegsisterUser(int i); // đăng ki ten client thu i

	void InitUsersAccount();//Server mơ kết nối cho user( yêu cầu đăng kí)
	void LoadData(char *patch);// Đọc data câu hỏi, save vào struct(Question)
	bool SendQues2AllUser(int i); // gửi câu hỏi cho tất cả user
	bool Turning_User(int i,int ques_j,char*&cl); //điều phối hoạt động send& receive trong 1 lượt đoán
	int Check_Answer(char * as,int ques_i,int D, char*& cl);// kiểm tra câu trl
	int find_maxScore(int &pos); //tìm người chơi số điểm cao nhất
	void Send2AllUser(char * string,int n); //gửi 1 chuỗi đến toán bộ user
	void ManageGame();// chương trình chính điều phối tát cả tro chơi
	int countLeft(char *answer); // đếm số ô chữ còn lại ( chưa giải)
	void ExitAll();



};
