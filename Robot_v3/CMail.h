#define _CRT_SECURE_NO_WARNINGS

#pragma once

#include <iostream>
#include <string.h>
#include <Windows.h>
#include <io.h>

using namespace std;


//���~�B�z�`��
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
					return "�нT�{ SwithMail.exe �bSwithMail_v2150��Ƨ���";
				case Login_NOT_EXIST:
					return "Login.ini���s�b";
				case Data_NOT_EXIST:
					return "Data�ɮפ��s�b";
				case SMTP_Server:
					return "SMTP Server �����w";
				case Username:
					return "Username ���]�w";
				case Password:
					return "Password ���]�w";
				case From_Address:
					return "From Address ���]�w";
				case To_Address:
					return "To Address ���]�w";
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
		#pragma region �ܼƫŧi
		
		//SwithMail.exe ���|
		char SwithMail_dir[200];
		char port_buf[10];

		string Cmd_Line; //�̲פU�F�����O
		
		//Login ���
		string SMTP_Server; //�H�cServer��m
		string SMTP_Port; //�H�cServer port
		bool SMTP_Server_flag;

		string Username; //�ϥΪ̦W��
		bool Username_flag;
		
		string Password; //�ϥΪ̱K�X
		bool Password_flag;

		bool SSL; // �w�]�i�� SSL/TLS �[�K�ǿ�
		
		string From_Address; //�H��H�H�c
		bool From_Address_flag;

		string From_Name; //�H��H�H�c����
		bool From_Name_flag;

		string To_Address; //����H�H�c
		int To_Address_count;

		string ReplyTo_Address; //�^�_�H�c
		bool ReplyTo_Address_flag;

		string CC_Address; //�ƥ��H�c
		int CC_Address_count;

		string BCC_Address; //�K��ƥ��H�c
		int BCC_Address_count;

		//�H����
		string Mail_Subject; //�H��D��
		bool Mail_Subject_flag;

		string Mail_Data; //�H�󤺮e
		int Mail_Data_count;
		
		string Mail_Attachment; //�H����[�ɮ�
		int Mail_Attachment_count;


		#pragma endregion

		//�ܼƪ�l��
		void Initial()
		{	
			SMTP_Server_flag = false;
			Username_flag = false;
			Password_flag = false;
			SSL = true; // �w�]�i�� SSL/TLS �[�K�ǿ�
			From_Address_flag = false;
			From_Name_flag = false;
			To_Address_count = 0;
			ReplyTo_Address_flag = false;
			CC_Address_count = 0;
			BCC_Address_count = 0;
			
			//�H����
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

			//�M�䥪�k '"' ����m
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
			
			//�M��data
			data.clear();

			for(int i=left+1; i<=(right-1); i++)
			{
				data += Src[i];
			}

			return true;
		}

	public:
		//�غc�l ��l��
		CMail()
		{
			Initial();

			memset(SwithMail_dir, '\0', sizeof(SwithMail_dir));
			
			//��o��e�����ɦ�m
			GetCurrentDirectory(sizeof(SwithMail_dir), SwithMail_dir);

			strcat(SwithMail_dir, "\\SwithMail_v2150\\SwithMail.exe");


			//�ɮ��v���ˬd(�˴��O�_���Ӱ�����)
			//mode 00:�s�b�B02:�߼g�B04:��Ū�B06:�iŪ�g
			if(_access(SwithMail_dir, 0) == 1)//�ɮפ��s�b
				throw ECMail(ECMail::SwithMail_NOT_EXIST);
			
		}

		//�Ѻc�l
		~CMail()
		{
			Cmd_Line.~basic_string();
		}

		//�]�w�H�cServer��m�PPort
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

		//�]�w�ϥΪ̱b��
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

		//�]�w�ϥΪ̱K�X
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

		//�]�w SSL/TLS �[�K�ǿ�A�w�]�}��
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


		//�]�w�H��H�H�c
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

		//�]�w�H��H�H�c����
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

		//�]�w����H�H�c�A�h�H�ɥi���ƩI�s���禡
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

		//�]�w�^�_�H�c�A�Y�L���w�h���H��H�H�c
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

		//�]�w�ƥ��H�c�A�h�H�ɥi���ƩI�s���禡
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

		//�]�w�K��ƥ��H�c�A�h�H�ɥi���ƩI�s���禡
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

		//�]�w�H��D��
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

		//�]�w�H�󤺮e�A�h��i���ƩI�s���禡
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
				//Mail_Data += "\n";//���U��
				Mail_Data += msg;
			}
			Mail_Data_count++;
		}

		//�]�w�H����[�ɮסA�h�ɮץi���ƩI�s���禡
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

		//���JLogin.ini�]�w��
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

		//���Jtxt���H�󤺮e
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

		//�H�X�H��
		void Send()
		{
			//�ˬd�H�H�һݪ�����O�_����
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
			//�I�s�~���{�� �����]�w
			SHELLEXECUTEINFO ShExecInfo = {0};

			//�]�w�}�ҥ~�������ɸ�T 
			ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO); 
			ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;//�^�� Process �� Handle 
			ShExecInfo.hwnd = NULL; //�@��]�w��NULL
			ShExecInfo.lpVerb = TEXT("open");//�~���{���ʧ@���w��open
			ShExecInfo.lpFile = TEXT(SwithMail_dir); //�����ɸ��|
			ShExecInfo.lpParameters = TEXT(Cmd_Line.c_str());//�ǤJ�Ѽ�
			ShExecInfo.lpDirectory = NULL;  //�L���w lpFile �ɮץؿ�

			ShExecInfo.nShow = SW_NORMAL; //�{�������@���
			//��L�i�ΰѼơG[ShowWindow() Commands�Ѧ�]

			ShExecInfo.hInstApp = NULL; 

			ShellExecuteEx(&ShExecInfo);//�}�l����

			
			WaitForSingleObject(ShExecInfo.hProcess, INFINITE);//���ݫ��wHandle���~���{�������AĲ�o�ƥ�
			TerminateProcess(ShExecInfo.hProcess, 0);//�j�������~���{��
			CloseHandle(ShExecInfo.hProcess);//�����~���{����Handle

		}

};
