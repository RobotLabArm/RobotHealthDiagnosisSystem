#define _CRT_SECURE_NO_WARNINGS

#pragma once

#include <iostream>
#include <string.h>
#include <Windows.h>
#include <io.h>

using namespace std;


//錯誤處理常式
class ECMail
{
	public:
		enum CMailError
		{
			SwithMail_NOT_EXIST = 1,
			Login_NOT_EXIST,
			Data_NOT_EXIST,
			SMTP_Server,
			Username,
			Password,
			From_Address,
			To_Address,

		};

		ECMail(CMailError err_) : ErrorCode(err_) {}
		
		CMailError GetErrorNum(void) const {return ErrorCode;}
		
		char* GetErrorText(void) const
		{
			switch(ErrorCode)
			{
				case SwithMail_NOT_EXIST:
					return "請確認 SwithMail.exe 在SwithMail_v2150資料夾中";
				case Login_NOT_EXIST:
					return "Login.ini不存在";
				case Data_NOT_EXIST:
					return "Data檔案不存在";
				case SMTP_Server:
					return "SMTP Server 未指定";
				case Username:
					return "Username 未設定";
				case Password:
					return "Password 未設定";
				case From_Address:
					return "From Address 未設定";
				case To_Address:
					return "To Address 未設定";
				default:
					return "";
			}
		};


	private:
		CMailError ErrorCode;
};


class CMail
{
	private:
		#pragma region 變數宣告
		
		//SwithMail.exe 路徑
		char SwithMail_dir[200];
		char port_buf[10];

		string Cmd_Line; //最終下達的指令
		
		//Login 資料
		string SMTP_Server; //信箱Server位置
		string SMTP_Port; //信箱Server port
		bool SMTP_Server_flag;

		string Username; //使用者名稱
		bool Username_flag;
		
		string Password; //使用者密碼
		bool Password_flag;

		bool SSL; // 預設進行 SSL/TLS 加密傳輸
		
		string From_Address; //寄件人信箱
		bool From_Address_flag;

		string From_Name; //寄件人信箱標籤
		bool From_Name_flag;

		string To_Address; //收件人信箱
		int To_Address_count;

		string ReplyTo_Address; //回復信箱
		bool ReplyTo_Address_flag;

		string CC_Address; //副本信箱
		int CC_Address_count;

		string BCC_Address; //密件副本信箱
		int BCC_Address_count;

		//信件資料
		string Mail_Subject; //信件主旨
		bool Mail_Subject_flag;

		string Mail_Data; //信件內容
		int Mail_Data_count;
		
		string Mail_Attachment; //信件附加檔案
		int Mail_Attachment_count;


		#pragma endregion

		//變數初始化
		void Initial()
		{	
			SMTP_Server_flag = false;
			Username_flag = false;
			Password_flag = false;
			SSL = true; // 預設進行 SSL/TLS 加密傳輸
			From_Address_flag = false;
			From_Name_flag = false;
			To_Address_count = 0;
			ReplyTo_Address_flag = false;
			CC_Address_count = 0;
			BCC_Address_count = 0;
			
			//信件資料
			Mail_Subject_flag = false;
			Mail_Data_count = 0;
			Mail_Attachment_count = 0;
		}

		bool Find_and_Get_str(string& data, const char *Src, const char *Substr)
		{
			int len = strlen(Substr);

			for(int i=0; i<len; i++)
			{
				if(Src[i] != Substr[i])
					return false;
			}

			//尋找左右 '"' 的位置
			int left = -1, right = -1;
			int count = 0;

			while(Src[len] != '\0')
			{
				if(Src[len] == '"')
				{	
					count++;
					
					if(count == 1)
						left = len;
					else
						right = len;
				}
				
				len++;
			}

			if(count != 2 || left < 0 || right < 0)
				return false;
			
			//清空data
			data.clear();

			for(int i=left+1; i<=(right-1); i++)
			{
				data += Src[i];
			}

			return true;
		}

	public:
		//建構子 初始化
		CMail()
		{
			Initial();

			memset(SwithMail_dir, '\0', sizeof(SwithMail_dir));
			
			//獲得當前執行檔位置
			GetCurrentDirectory(sizeof(SwithMail_dir), SwithMail_dir);

			strcat(SwithMail_dir, "\\SwithMail_v2150\\SwithMail.exe");


			//檔案權限檢查(檢測是否有該執行檔)
			//mode 00:存在、02:唯寫、04:唯讀、06:可讀寫
			if(_access(SwithMail_dir, 0) == 1)//檔案不存在
				throw ECMail(ECMail::SwithMail_NOT_EXIST);
			
		}

		//解構子
		~CMail()
		{
			Cmd_Line.~basic_string();
		}

		//設定信箱Server位置與Port
		void Set_SMTPServer(const char *smtp, int port)
		{
			if(strlen(smtp) == 0)
				return;

			memset(port_buf, '\0', sizeof(port_buf));

			sprintf(port_buf, "%d", port);

			// "smtp"
			SMTP_Server = '"';
			SMTP_Server += smtp;
			SMTP_Server += '"';
			
			// "port"
			SMTP_Port = '"';
			SMTP_Port += port_buf;
			SMTP_Port += '"';

			SMTP_Server_flag = true;
		};
		void Set_SMTPServer(const char *smtp, const char *port)
		{
			if(strlen(smtp) == 0 || strlen(port) == 0)
				return;
			
			// "smtp"
			SMTP_Server = '"';
			SMTP_Server += smtp;
			SMTP_Server += '"';
			
			// "port"
			SMTP_Port = '"';
			SMTP_Port += port;
			SMTP_Port += '"';

			SMTP_Server_flag = true;
		};

		//設定使用者帳號
		void Set_Username(const char *user_name)
		{
			if(strlen(user_name) == 0)
				return;
			
			// "user_name"
			Username = '"';
			Username += user_name;
			Username += '"';
			Username_flag = true;
		}

		//設定使用者密碼
		void Set_Password(const char *password)
		{
			if(strlen(password) == 0)
				return;
			
			// "password"
			Password = '"';
			Password += password;
			Password += '"';
			Password_flag = true;
		}

		//設定 SSL/TLS 加密傳輸，預設開啟
		void Set_SSL(bool open)
		{
			SSL = open;
		}
		void Set_SSL(const char *open)
		{
			if(strlen(open) == 0)
				return;

			if(open[0] == 't' || open[0] == 'T')
				SSL = true;
			else if(open[0] == 'f' || open[0] == 'F')
				SSL = false;
		}


		//設定寄件人信箱
		void Set_FromAddress(const char *address)
		{
			if(strlen(address) == 0)
				return;
			
			// "address"
			From_Address = '"';
			From_Address += address;
			From_Address += '"';
			From_Address_flag = true;
		}

		//設定寄件人信箱標籤
		void Set_FromName(const char *name)
		{
			if(strlen(name) == 0)
				return;

			// "name"
			From_Name = '"';
			From_Name += name;
			From_Name += '"';
			From_Name_flag = true;
		}

		//設定收件人信箱，多人時可重複呼叫此函式
		void Set_ToAddress(const char *address)
		{
			if(strlen(address) == 0)
				return;
			
			if(To_Address_count == 0)
			{
				To_Address = address;
			}
			else
			{
				To_Address += ";";
				To_Address += address;
			}
			To_Address_count++;
		}

		//設定回復信箱，若無指定則為寄件人信箱
		void Set_ReplyToAddress(const char *address)
		{
			if(strlen(address) == 0)
				return;
			
			// "address"
			ReplyTo_Address = '"';
			ReplyTo_Address += address;
			ReplyTo_Address += '"';
			ReplyTo_Address_flag = true;
		}

		//設定副本信箱，多人時可重複呼叫此函式
		void Set_CC_Address(const char *address)
		{
			if(strlen(address) == 0)
				return;
			
			if(CC_Address_count == 0)
			{
				CC_Address = address;
			}
			else
			{
				CC_Address += ";";
				CC_Address += address;
			}
			CC_Address_count++;
		}

		//設定密件副本信箱，多人時可重複呼叫此函式
		void Set_BCC_Address(const char *address)
		{
			if(strlen(address) == 0)
				return;
			
			if(BCC_Address_count == 0)
			{
				BCC_Address = address;
			}
			else
			{
				BCC_Address += ";";
				BCC_Address += address;
			}
			BCC_Address_count++;
		}

		//設定信件主旨
		void Set_Mail_Subject(const char *subject)
		{
			if(strlen(subject) == 0)
				return;
			
			// "subject"
			Mail_Subject = '"';
			Mail_Subject += subject;
			Mail_Subject += '"';
			Mail_Subject_flag = true;
		}

		//設定信件內容，多行可重複呼叫此函式
		void Set_Mail_Data(const char *msg)
		{
			if(strlen(msg) == 0)
				return;
			
			if(Mail_Data_count == 0)
			{
				Mail_Data = msg;
			}
			else
			{
				//Mail_Data += "\n";//換下行
				Mail_Data += msg;
			}
			Mail_Data_count++;
		}

		//設定信件附加檔案，多檔案可重複呼叫此函式
		void Set_Mail_Attachment(const char *adress)
		{
			if(strlen(adress) == 0)
				return;
			
			if(Mail_Attachment_count == 0)
			{
				Mail_Attachment = adress;
			}
			else
			{
				Mail_Attachment += "|";
				Mail_Attachment += adress;
			}
			Mail_Attachment_count++;
		}

		//載入Login.ini設定檔
		void Load_Login(const char *address)
		{
			FILE *Login_ptr = fopen(address, "r");

			if(Login_ptr == NULL)
				throw ECMail(ECMail::Login_NOT_EXIST);

			char buf[512] = {'\0'};
			string data;
			string smtp, port;

			while (fgets(buf, sizeof(buf), Login_ptr) != NULL)
			{
				//printf("%s", buf);
				
				if(Find_and_Get_str(data, buf, "/Server"))
					smtp = data;
				else if(Find_and_Get_str(data, buf, "/Port"))
					port = data;
				else if(Find_and_Get_str(data, buf, "/Username"))
					Set_Username(data.c_str());
				else if(Find_and_Get_str(data, buf, "/Password"))
					Set_Password(data.c_str());
				else if(Find_and_Get_str(data, buf, "/SSL"))
					Set_SSL(data.c_str());
				else if(Find_and_Get_str(data, buf, "/FromAddress"))
					Set_FromAddress(data.c_str());
				else if(Find_and_Get_str(data, buf, "/FromName"))
					Set_FromName(data.c_str());
				else if(Find_and_Get_str(data, buf, "/ToAddress"))
					Set_ToAddress(data.c_str());
				else if(Find_and_Get_str(data, buf, "/ReplyTo"))
					Set_ReplyToAddress(data.c_str());
				else if(Find_and_Get_str(data, buf, "/CC"))
					Set_CC_Address(data.c_str());
				else if(Find_and_Get_str(data, buf, "/BCC"))
					Set_BCC_Address(data.c_str());
				else if(Find_and_Get_str(data, buf, "/Subject"))
					Set_Mail_Subject(data.c_str());
				else if(Find_and_Get_str(data, buf, "/Attachment"))
					Set_Mail_Attachment(data.c_str());

				memset(buf, '\0', sizeof(buf));
			}

			Set_SMTPServer(smtp.c_str(), port.c_str());

			fclose(Login_ptr);

		}

		//載入txt的信件內容
		void Load_Data(const char *address)
		{
			FILE *Data_ptr = fopen(address, "r");

			if(Data_ptr == NULL)
				throw ECMail(ECMail::Data_NOT_EXIST);

			char data[512] = {'\0'};

			while (fgets(data, sizeof(data), Data_ptr) != NULL)
			{
				//printf("%s", data);
				Set_Mail_Data(data);
				memset(data, '\0', sizeof(data));
			}

			fclose(Data_ptr);
		}

		//寄出信件
		void Send()
		{
			//檢查寄信所需的條件是否滿足
			if(!(SMTP_Server_flag))
				throw ECMail(ECMail::SMTP_Server);
			if(!(Username_flag))
				throw ECMail(ECMail::Username);
			if(!(Password_flag))
				throw ECMail(ECMail::Password);
			if(!(From_Address_flag))
				throw ECMail(ECMail::From_Address);
			if(To_Address_count == 0)
				throw ECMail(ECMail::To_Address);

			//Cmd_Line += SwithMail_dir;
			Cmd_Line += "/s";
			
			Cmd_Line += " /Server ";
			Cmd_Line += SMTP_Server;
			
			Cmd_Line += " /Port ";
			Cmd_Line += SMTP_Port;
			
			Cmd_Line += " /Username ";
			Cmd_Line += Username;

			Cmd_Line += " /Password ";
			Cmd_Line += Password;

			Cmd_Line += " /SSL ";
			if(SSL)
				Cmd_Line += "\"true\"";
			else
				Cmd_Line += "\"false\"";

			Cmd_Line += " /FromAddress ";
			Cmd_Line += From_Address;

			
			if(From_Name_flag)
			{
				Cmd_Line += " /FromName ";
				Cmd_Line += From_Name;
			}


			Cmd_Line += " /ToAddress ";
			Cmd_Line += '"';
			Cmd_Line += To_Address;
			Cmd_Line += '"';


			Cmd_Line += " /ReplyTo ";
			if(ReplyTo_Address_flag)
				Cmd_Line += ReplyTo_Address;
			else
				Cmd_Line += From_Address;

			if(CC_Address_count >= 1)
			{	
				Cmd_Line += " /CC ";
				Cmd_Line += '"';
				Cmd_Line += CC_Address;
				Cmd_Line += '"';
			}

			if(BCC_Address_count >= 1)
			{	
				Cmd_Line += " /BCC ";
				Cmd_Line += '"';
				Cmd_Line += BCC_Address;
				Cmd_Line += '"';
			}


			Cmd_Line += " /Subject ";
			Cmd_Line += Mail_Subject;

			if(Mail_Attachment_count >= 1)
			{
				Cmd_Line += " /Attachment ";
				Cmd_Line += '"';
				Cmd_Line += Mail_Attachment;
				Cmd_Line += '"';
			}

			if(Mail_Data_count >= 1)
			{
				Cmd_Line += " /Body ";
				Cmd_Line += '"';
				Cmd_Line += Mail_Data;
				Cmd_Line += '"';
			}



			/********************************************/
			//呼叫外部程式 相關設定
			SHELLEXECUTEINFO ShExecInfo = {0};

			//設定開啟外部執行檔資訊 
			ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO); 
			ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;//回傳 Process 的 Handle 
			ShExecInfo.hwnd = NULL; //一般設定為NULL
			ShExecInfo.lpVerb = TEXT("open");//外部程式動作指定為open
			ShExecInfo.lpFile = TEXT(SwithMail_dir); //執行檔路徑
			ShExecInfo.lpParameters = TEXT(Cmd_Line.c_str());//傳入參數
			ShExecInfo.lpDirectory = NULL;  //無指定 lpFile 檔案目錄

			ShExecInfo.nShow = SW_NORMAL; //程式視窗一般化
			//其他可用參數：[ShowWindow() Commands參考]

			ShExecInfo.hInstApp = NULL; 

			ShellExecuteEx(&ShExecInfo);//開始執行

			
			WaitForSingleObject(ShExecInfo.hProcess, INFINITE);//等待指定Handle的外部程式結束，觸發事件
			TerminateProcess(ShExecInfo.hProcess, 0);//強制關閉外部程式
			CloseHandle(ShExecInfo.hProcess);//關閉外部程式的Handle

		}

};
