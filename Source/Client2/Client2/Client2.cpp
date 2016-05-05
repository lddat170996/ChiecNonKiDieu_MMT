// Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Client2.h"


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
			cout << "\t\t\t#################  << CHIEC NON KY DIEU >>  ################# " << endl << endl;
			KNClient x;
			x.InitChat();
			x.RegisterName();

			cout << "\n\n\t\t ***************** < Luat Choi > *****************\n";
			cout << "1. Nguoi choi phai trai qua cac vong choi tuong duong voi so cau hoi cua chuong trinh.\n";
			cout << "2. Khi den luot choi :\n \tdoan dung 1 chu cai -> nhan duoc so diem tuong duong so diem quay duoc.\n ";
			cout << "\tdoan dung o chu bi mat -> nhan duoc so diem x3.\n";
			cout << "\n\t -----> nhan phim bat ky de bat dau tro choi : ";
			char k;
			cin >> k;
			fflush(stdin);
			system("cls");
			cout << "\t\t\t#################  << CHIEC NON KY DIEU >>  ################# " << endl << endl;


			CSocket Client;
			char *str_r = new char[200];
			Client.Receive(str_r, 200, 0);
			str_r[strlen(str_r)] = 0;
			if (strstr(str_r, "Exit"))
			{
				Client.Close();
				return 0;
			}
			else
			{

				do
				{
	
					if (x.recieve()) //nếu trả về true mới được trl
						x.Answer();
					else
						break;
				} while (1);
			}

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


void KNClient::InitChat()
{
	AfxSocketInit(NULL);
	Client.Create();
	if (Client.Connect(_T("127.0.0.2"), 1234))
	{
		cout << "Da ket noi thanh cong voi server" << endl;
		char s_str[1000], r_str[1000];
	}
}
void KNClient::RegisterName()
{
	char name[100];
	int len;
	char str_r[200];
	bool flag = true;
	do{
		do{
			cout << "Moi ban nhap ten: ";
			gets(name);
			string a(name);
			for (int i = 0; i < a.length(); i++)
			{
				if (!((a[i] <'a' || a[i] >'z') && (a[i] <'A' || a[i] > 'Z') && (a[i] <'0' || a[i] > '9')))
				{
					flag = false;
					break;
				}
			}

		} while (flag);
		Client.Send(name, strlen(name), 0);
		len = Client.Receive(str_r, 100, 0);
		str_r[len] = 0;
		if (len != 0)
			cout << "\t----> " << str_r << endl;
	} while (strcmp(str_r, "Dang ky ten thanh cong") != 0);


}

bool KNClient::recieve()
{
	char *str_r, *str_r1, *str_s;
	do{
		str_r = new char[200];
		Client.Receive(str_r, 200, 0);
		str_r[strlen(str_r)] = 0;
		cout << str_r << endl;
		if (strstr(str_r, "-->Toi luot ban tra loi (Xin random so):")) //quay so
		{
			cout << "\t----> Nhan phim bat ki de quay so: ";
			char x;
			x = getch();
			fflush(stdin);
			break;
		}
		if (strstr(str_r, "Exit")) //kết thúc trò choi
			return false;
		if (strstr(str_r, "<KET THUC LUOT CHOI")) //ket thuc luot choi
		{
			system("cls");
			cout << "\t\t\t#################  << CHIEC NON KY DIEU >>  ################# " << endl << endl;
		}
	} while (1);

	//send diem random	
	srand(time(NULL));
	int num = rand() % 500;
	char s[100];
	itoa(num, s, 10);
	string a, b;
	a = "Quay duoc: ";
	ostringstream convert;
	convert << num;
	b = convert.str();
	a += b;
	Client.Send(a.c_str(), 200, 0);
	cout << "\t----> Ban quay duoc:  " << num << endl;
	return true;

}

void KNClient::Answer()
{
	char *str_s, *str_r;
	do
	{
		str_r = new char[100];
		Client.Receive(str_r, 100, 0);
		str_r[strlen(str_r)] = 0;
		cout << str_r << endl;
		if (strstr(str_r, "Nhap cau tra loi: "))
		{
			break;
		}
	} while (1);
	string a, b;
	a = "Tra loi: ";
	getline(cin, b);
	a += b;
	Client.Send(a.c_str(), 200, 0);

	/*do
	{
	str_r = new char[200];
	Client.Receive(str_r, 200, 0);
	str_r[strlen(str_r)] = 0;
	cout << "Server: " << str_r << endl;
	if (strstr(str_r, "Nguoi choi <"))
	{
	break;
	}
	} while (1);*/
}