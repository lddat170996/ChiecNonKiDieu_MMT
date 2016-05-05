// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Server_Socket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: change error code to suit your needs
			_tprintf(_T("Fatal Error: MFC initialization failed\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: code your application's behavior here.
			KNServer x;

			cout << "\t\t\t#################  << CHIEC NON KY DIEU >>  ################# " << endl << endl;
			x.Chat();
			x.InitUsersAccount(); // dang ky ten 
			srand(time(NULL));
			x.LoadData("input.txt");
			x.ManageGame();
			x.ExitAll();


		}
	}
	else
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		nRetCode = 1;
	}

	return nRetCode;
}


void KNServer::Chat()
{
	AfxSocketInit(NULL);
	Server.Create(1234);
	cout << "Xin nhap so nguoi choi: ";
	cin >> this->N_USER;
	Client = new CSocket[N_USER];

	Server.Listen(N_USER);
	cout << "Dang cho 1 ket noi" << endl;

}



bool KNServer::RegsisterUser(int i)
{
	bool flag = true;
	char *str_r;
	char a[100];
	User tmp;
	str_r = new char[100];
	
		int len = Client[i].Receive(str_r, 100, 0); // nhận chuỗi đăng kí tên từ người chơi
		str_r[len] = 0;
	cout << "\tuser [" << i << "] : " << str_r << endl;

	//kiểm tra xem có trùng tên hay không 
	for (int j = 0; j < userArr.size(); j++)
	{
		if (strcmp(str_r, userArr[j].name) == 0)
			flag = false;
	}

	if (flag == true)
	{
		tmp.score = 0;
		tmp.name = str_r;
		userArr.push_back(tmp);
		Client[i].Send("Dang ky ten thanh cong", 100, 0);
		return true;
	}
	else
	{
		Client[i].Send("Dang ky lai", 100, 0);
	}
	return false;
}

void  KNServer::InitUsersAccount()
{
	vector<char*> nameArr;
	system("cls");
	cout << "\t\t\t#################  << CHIEC NON KY DIEU >>  ################# " << endl << endl;
	cout << "\n << Danh sach nguoi choi >> " << endl;
	for (int i = 0; i < N_USER; i++)
	{
		//phải đăng kí thành công tất cả các user name
		if (Server.Accept(Client[i]))
		{

			cout << "Chap nhan user thu:" << i+1 << endl;
		}
	}
	//dang ky
	for (int i = 0; i < N_USER; i++)
	{
		while (1)
		{
			if (this->RegsisterUser(i) == true)
				break;
		}
	}
	cout << "-----------------------------" << endl;
}

void KNServer::LoadData(char *path)
{
	ifstream reader;
	string line;
	int n;

	reader.open("input.txt");

	if (!reader.is_open())
	{
		cout << "Cant open file \n";
		return;
	}
	reader >> n;
	string ques, hint, a;
	int count;
	Question tmp;
	getline(reader, a,'\n');
	//lưu data câu hỏi vào struc Question
	for (int i = 0; i < n; i++)
	{
		getline(reader, ques, '\t');
		getline(reader, hint,'\t');	
		reader >> count;
		getline(reader, a, '\n');
		tmp.asked = false;
		tmp.hint = hint;
		tmp.ques = ques;
		tmp.turn = count;
		quesArr.push_back(tmp);
	}
	reader.close();
}



bool KNServer::SendQues2AllUser(int j)
{
	string str_s, str_r;
	if (j < 0 || j >quesArr.size())
	{
		cout << "khong co cau hoi thu" << j << endl;
		return false;
	}
	for (int i = 0; i < N_USER; i++)
	{
		//send het cho N_USER
		str_r = new char[300];
		str_s = new char[300];

		str_s = "So ki tu : ";

		string tmp = to_string(quesArr[j].ques.length());
		str_s += tmp;
		str_s += "          Goi y: ";
		str_s += quesArr[j].hint;

		//cout << str_s << endl;
		const char* chr = str_s.c_str();
		Client[i].Send(chr, 200, 0);
	}
	return true;
}
void  KNServer::Send2AllUser(char * stringi, int n)
{
	for (int i = 0; i < N_USER; i++)
	{
		Client[i].Send(stringi, 200, 0);
	}
}
int KNServer::Check_Answer(char * as, int ques_i, int D, char *&cl)
{
	int count = 0;
	if (strlen(as) == 1) // TH trl 1 kí tự
	{
		char trl = as[0];
		for (int i = 0; i < quesArr[ques_i].ques.length(); i++)
		{
			if (trl == quesArr[ques_i].ques[i] && cl[i] == '*')
			{
				cl[i] = trl;
				count++;
			}
		}
		return count*D;
	}
	else //TH trl cả câu hỏi
	{
		if (strcmp(as, quesArr[ques_i].ques.c_str()) == 0)
		{
			cl = as;
			return 3 * D;
		}
		else
			return 0;
	}

}
bool  KNServer::Turning_User(int i, int ques_j, char * &cl)
{
	char *str_r, *str_cl;

	Client[i].Send("-->Toi luot ban tra loi (Xin random so): ", 100, 0);
	
	do
	{
		str_r = new char[200];
		Client[i].Receive(str_r, 200, 0);
		str_r[strlen(str_r)] = 0;
		if (strstr(str_r, "Quay duoc: ")) //chờ Client tới khi nhận được điểm quay random
			break;
	} while (1);

	string temp(str_r);
	temp.erase(0, 11);
	char* r = new char[temp.length() + 1];
	strcpy(r, temp.c_str()); //save điểm random vào "r"
	
	
	cout << " User [" << userArr[i].name << "] quay duoc : " << atoi(r) << " diem" << endl;
	if (atoi(r)==0)
	{
		Client[i].Send("\nBan bi mat luot", 200, 0); //TH random điểm == 0
		return false;
	}
	else
	{
		int D = atoi(r); //D là điểm người chơi random
		
		str_r = new char[200];
		Client[i].Send("Nhap cau tra loi: ", 100, 0);
		do{
			Client[i].Receive(str_r, 200, 0);
			str_r[strlen(str_r)] = 0;
			if (strstr(str_r, "Tra loi: "))
				break;
		} while (1);
		string temp(str_r);
		temp.erase(0, 9);
		
		cout << "- > Cau tra loi cua User[" << userArr[i].name << "]: " << temp<<endl;
		quesArr[ques_j].turn--; //down turn of ques
		char* answer = new char[temp.length() + 1];
		strcpy(answer, temp.c_str());
		int Score = this->Check_Answer(answer, ques_j, D, cl);	 // check_Answer trl về score
		if (Score == 0)
		{
			Client[i].Send("\nCau tra loi sai", 200, 0);
			return false;
		}

		cout << "User < " << userArr[i].name << " > vua doan dung => so diem la : " << Score << endl;
		string send = "Nguoi choi <";
		send += userArr[i].name;
		send += "> vua duoc so diem la : ";
		send += to_string(Score);		
		
		for (int i = 0; i < N_USER; i++)
		{
			Client[i].Send(send.c_str(), 200, 0);
		}
		userArr[i].score += Score;
	}
	return true;
}

int KNServer::countLeft(char *answer)
{
	int count = 0;
	for (int i = 0; i < strlen(answer); i++)
	{
		if (answer[i] == '*')
		{
			return -1;
		}
	}
	return 0;
}
void KNServer::ManageGame()
{
	int user_i = 0;
	for (int ques_i = 0; ques_i < quesArr.size(); ques_i++)
	{
		int n = 0;
		bool over_turn = false; //ktr có quá lượt đoán không
		string a;
		char *tmp = new char[200];
		cout << "\n\n------------------ O CHU THU <" << ques_i + 1 << "> ------------------\n";
		cout << "Cau hoi <" << ques_i + 1 << "> : " << quesArr[ques_i].ques << endl;
		a = "\n------------------ O CHU THU <";
		a += to_string(ques_i + 1);
		a += ">------------------\n";
		strcpy(tmp, a.c_str());
		Send2AllUser(tmp, 200);

		SendQues2AllUser(ques_i);
		delete tmp;
		tmp = new char[200];
		
		for (n; n < quesArr[ques_i].ques.length(); n++)
			tmp[n] = '*';
		tmp[n++] = 0;

		//random user moi vong choi
		srand(time(NULL));
		user_i = rand() % N_USER;
		cout << user_i;
		do{
			string a;
			a = "Con lai: ";
			a += to_string(quesArr[ques_i].turn); // số lượt đoán còn lại
			a += " (luot doan)\n";
			char *turn = new char[200];
			strcpy(turn, a.c_str());
			Send2AllUser(turn, 200);
			if (quesArr[ques_i].turn <= 0)
			{
				over_turn = true; 
				break;
			}
			Send2AllUser(tmp, n);
			if (Turning_User(user_i, ques_i, tmp))
				user_i--;
			user_i++;
			if (user_i >= N_USER)
				user_i = 0;
		} while (countLeft(tmp) != 0);


		Send2AllUser("<KET THUC LUOT CHOI>", 200);
		for (int i = 0; i < N_USER; i++)
		{
			string a = "Tong diem Ban dat duoc la :";
			char *tmp = new char[a.length() + 10];
			a += to_string(userArr[i].score);
			strcpy(tmp, a.c_str());
			Client[i].Send(tmp, 200, 0);

		}
		// tim nguoi thang cuoc
	
		if (over_turn)
		{
			Send2AllUser("O Chu truoc khong co nguoi chien thang\n", 200);
		}
		else
		{
			int pos = 0;
			int max = find_maxScore(pos);
			a = a + "Nguoi choi <" + userArr[pos].name + "> Vua chien thang(" + to_string(userArr[pos].score) + ") diem\n";
			char *tmp = new char[a.length() + 1];
			strcpy(tmp, a.c_str());
			Send2AllUser(tmp, 200);
		}

	} 

	
	this->Send2AllUser("\nExit", 200);
}
int KNServer::find_maxScore(int &pos)
{
	int max = 0;
	for (int i = 0; i < userArr.size(); i++)
	{
		if (userArr[i].score > max)
		{
			max = userArr[i].score;
			pos = i;
		}
	}
	return max;
}
void KNServer::ExitAll()
{
	for (int i = 0; i < N_USER; i++)
	{
		Client[i].Close();
	}
	Server.Close();
}