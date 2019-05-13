

#include<sqlite3.h>

#include "comm.h"

sqlite3 * db;
int staffno = 0;


void itoa(int staffno,char* s)
{
	int i = 0;
	int temp1= staffno;
	int temp2 = 0;
	while((temp1 = temp1 / 10) != 0)
	{
		i++;
	}
	//i++;
	temp1 = staffno;
	while(i >= 0)
	{
		temp2 =temp1%10;
		temp1 = temp1/10;
		
		*(s + i) = '0' + temp2;
		i--;
	}
	printf("****:s : %s\n",s);
}

void do_date(MSG* msg ,char * s)
{
	char no[10]={0};
	char text[DATALEN] = {0};
	char who[NAMELEN] ={0};
	char whos[NAMELEN] ={0};
	char times[DATALEN] = {0};
	char sql[DATALEN] = {0};
	time_t ftime;
	struct tm * pm = NULL;
    char * errmsg = NULL;
	time(&ftime);
	pm = localtime(&ftime);
	sprintf(times,"%04d-%2d-%02d %02d-%02d-%02d",pm->tm_year+1900,pm->tm_mon,pm->tm_mday,pm->tm_hour,pm->tm_min,pm->tm_sec);
	sprintf(who,"--%s--",(msg->usertype == 0?"admin":"user"));
	
	sprintf(whos,"%s ",(msg->usertype == 0?"admin":"user"));
	strncat(text,whos,strlen(whos));
	strcat(text,"(");
	//itoa(staffno,no,10);

//	strncat(text,(char*)&staffno,4);
	//strncat(text,no,4);
//	strncpy(no,(char*)&staffno,4);
//	strcpy(text,no);

	itoa(staffno,no);
	strcat(text,no);
	strcat(text,")");
	printf("----------:%s\n",s);
	
	strncat(text,s,strlen(s));
	printf("****:%s\n",text);
	

	sprintf(sql,"insert into historyinfo values('%s','%s','%s');",times,who,text);
	printf("sql:%s\n",sql);
	
	if(sqlite3_exec(db,sql,NULL,NULL,&errmsg) != 0)
	{
	
		printf("222222222222222222222222222222222222222222222222211111\n");
		printf("%s\n",errmsg);
		
	}else
	{
		printf("11111111111111111111111111111111111111111111111111111\n");
		//printf("%s %s %s",times,who,text );
	}
}



int process_user_or_admin_login(int acceptfd,MSG* msg)
{
	
	printf("---------%s-------%d .\n",__func__,__LINE__);
	char sql[DATALEN] = {0};
	char * errmsg ;
	char ** result;
	int nrow, ncolumn;
	char s[DATALEN] = {0};
	staffno = 0;
	msg->info.usertype = msg->usertype;
//	strcpy(msg->info.name,msg->username);
	msg->info.no = msg->staffno;
	staffno = msg->staffno;
	printf("staffno:%d\n",staffno);
	strcpy(msg->info.passwd,msg->passwd);


	printf("usertype: %#x \n staffno: %d\n passwd: %s\n",msg->info.usertype,msg->info.no,msg->info.passwd);
	sprintf(sql,"select * from userinfo where usertype=%d and staffno = '%d' and passwd = '%s';",msg->info.usertype,msg->info.no,msg->info.passwd);
	printf("%s\n",sql);

	if(sqlite3_get_table(db,sql,&result,&nrow,&ncolumn,&errmsg)!=0)
	{
		printf("%s\n",errmsg);
	}
	else
	{
		memset(msg->recvmsg,0,DATALEN);
		if(nrow == 0)
		{
			strcpy(msg->recvmsg,"staffno or passwd failed\n");
			send(acceptfd,msg,sizeof(MSG),0);
		}
		else
		{
			strcpy(msg->recvmsg,"OK");
			send(acceptfd,msg,sizeof(MSG),0);
			sprintf(s," login in !");
			do_date(msg,s);
		}
	}
	

	return 0;
}
	
int process_user_modif(int acceptfd,MSG * msg)
{
	char sql[DATALEN] = {0};
	char * errmsg;
	char s[NAMELEN] = {0};
	switch(msg->flags)
	{
		case 1:
			sprintf(sql,"update userinfo set addr='%s' where staffno = %d;",msg->info.addr,msg->info.no);
			printf("%s\n",sql);
			if(sqlite3_exec(db,sql,NULL,NULL,&errmsg) != 0)
			{
				printf("%s\n",errmsg);
			}
			else
			{
				memset(msg->recvmsg,0,DATALEN);
				strcpy(msg->recvmsg,"addr modification success!");
				send(acceptfd,msg,sizeof(MSG),0);
				sprintf(s,"修改工号为%d的用户的住址为:%s",msg->info.no,msg->info.addr);
				do_date(msg,s);
			}
			break;
		case 2:
			sprintf(sql,"update userinfo set phone='%s' where staffno = %d;",msg->info.phone,msg->info.no);
			printf("%s\n",sql);
			if(sqlite3_exec(db,sql,NULL,NULL,&errmsg) != 0)
			{
				printf("%s\n",errmsg);
			}
			else
			{
				memset(msg->recvmsg,0,DATALEN);
				strcpy(msg->recvmsg,"phone modification success!");
				send(acceptfd,msg,sizeof(MSG),0);
				sprintf(s," 修改工号为%d的用户的电话为:%s ",msg->info.no,msg->info.phone);
				do_date(msg,s);
			}
			break;
		case 3:
			sprintf(sql,"update userinfo set passwd='%s' where staffno = %d;",msg->info.passwd,msg->info.no);
			printf("%s\n",sql);
			if(sqlite3_exec(db,sql,NULL,NULL,&errmsg) != 0)
			{
				printf("%s\n",errmsg);
			}
			else
			{
				memset(msg->recvmsg,0,DATALEN);
				strcpy(msg->recvmsg,"passwd modification success!");
				send(acceptfd,msg,sizeof(MSG),0);
				sprintf(s," 修改工号为%d的用户的密码为:%s ",msg->info.no,msg->info.passwd);
				do_date(msg,s);
			}
			break;		
			
	}

	return 0;
}
int process_user_query(int acceptfd,MSG * msg)
{
	char sql[DATALEN] = {0};
	char * errmsg;
	char ** result = NULL;
	int nrow,ncolumn;
	char s[NAMELEN] = {0};
	//msg->info.no = msg->staffno;
	int i = 0,j =0 ;
	
	sprintf(sql,"select * from userinfo where staffno = %d ;",msg->info.no);

	printf("%s\n",sql);
	
	if(sqlite3_get_table(db,sql,&result,&nrow,&ncolumn,&errmsg)!=0)
	{
		printf("%s\n",errmsg);
	}
	else
	{
		sprintf(s," 查询自己的信息 工号为:%d ",msg->info.no);
		do_date(msg,s);
		memset(msg->recvmsg,0,DATALEN);
		int ps = ncolumn;
		printf("%d   %d \n",nrow,ncolumn);
		for(i=1;i<=nrow;i++)
		{
			for(j=0;j<ncolumn;j++)
			{
				if(result [ps] == NULL)
				{
					result[ps] ="##";
				}
				strcat(msg->recvmsg,result[ps]);
				ps++;
				strcat(msg->recvmsg,"\n");
			}
		}
		send(acceptfd,msg,sizeof(MSG),0);
	}
	return 0;

}
int process_admin_modif(int acceptfd,MSG * msg)
{
		char sql[DATALEN] = {0};
		char * errmsg;
		char ** result = NULL;
		int nrow,ncolumn;
		char s[NAMELEN] = {0};
		switch(msg->flags)
		{


			case 1:
				
				sprintf(sql,"select * from userinfo where staffno = %d;",msg->info.no);
				printf("%s\n",sql);
				if(sqlite3_get_table(db,sql,&result,&nrow,&ncolumn,&errmsg)!=0)
				{
					printf("%s\n",errmsg);
				//	printf("111111113333333333333333333333333311111111111\n");
					break;
				}
				else
				{
				//	printf("1111166666666666666666666666666666666666666666666111\n");
					if(nrow == 0)
					{
						memset(msg->recvmsg,0,DATALEN);
						strcpy(msg->recvmsg,"staffno no exist!");
						send(acceptfd,msg,sizeof(MSG),0);
					}
					else
					{
						memset(sql,0,DATALEN);
				
						sprintf(sql,"update userinfo set name='%s' where staffno = %d;",msg->info.name,msg->info.no);
						printf("%s\n",sql);

						if(sqlite3_exec(db,sql,NULL,NULL,&errmsg) != 0)
						{
							printf("%s\n",errmsg);
						}
						else
						{
							memset(msg->recvmsg,0,DATALEN);
							strcpy(msg->recvmsg,"name modification success!");
							send(acceptfd,msg,sizeof(MSG),0);
							sprintf(s,"修改工号为%d的员工的名字为:%s ",msg->info.no,msg->info.name);
							printf("-----:%s:\n",s);
							do_date(msg,s);
						}
					}
				}
				break;

			case 2:

				sprintf(sql,"select * from userinfo where staffno = %d;",msg->info.no);
				printf("%s\n",sql);
				
				if(sqlite3_get_table(db,sql,&result,&nrow,&ncolumn,&errmsg)!=0)
				{
					printf("%s\n",errmsg);
					break;
				}
				else
				{
					if(nrow == 0)
					{
						memset(msg->recvmsg,0,DATALEN);
						strcpy(msg->recvmsg,"staffno no exist!");
						send(acceptfd,msg,sizeof(MSG),0);
					}
					else
					{
						memset(sql,0,DATALEN);
				
						sprintf(sql,"update userinfo set age='%d' where staffno = %d;",msg->info.age,msg->info.no);
						printf("%s\n",sql);
						if(sqlite3_exec(db,sql,NULL,NULL,&errmsg) != 0)
						{
							printf("%s\n",errmsg);
						}
						else
						{
							memset(msg->recvmsg,0,DATALEN);
							strcpy(msg->recvmsg,"age modification success!");
							send(acceptfd,msg,sizeof(MSG),0);
							sprintf(s,"修改工号为 %d 的员工的年龄为:%d",msg->info.no,msg->info.age);
							do_date(msg,s);
						}
					}
				}
				break;
				
			
			case 3:
				sprintf(sql,"select * from userinfo where staffno = %d;",msg->info.no);
				printf("%s\n",sql);
				
				if(sqlite3_get_table(db,sql,&result,&nrow,&ncolumn,&errmsg)!=0)
				{
					printf("%s\n",errmsg);
					break;
				}
				else
				{
					if(nrow == 0)
					{
						memset(msg->recvmsg,0,DATALEN);
						strcpy(msg->recvmsg,"staffno no exist!");
						send(acceptfd,msg,sizeof(MSG),0);
					}
					else
					{
						memset(sql,0,DATALEN);
				
						sprintf(sql,"update userinfo set addr='%s' where staffno = %d;",msg->info.addr,msg->info.no);
						printf("%s\n",sql);
						if(sqlite3_exec(db,sql,NULL,NULL,&errmsg) != 0)
						{
							printf("%s\n",errmsg);
						}
						else
						{
							memset(msg->recvmsg,0,DATALEN);
							strcpy(msg->recvmsg,"addr modification success!");
							send(acceptfd,msg,sizeof(MSG),0);
							sprintf(s,"修改工号为%d的员工的组织为:%s",msg->info.no,msg->info.addr);
							do_date(msg,s);
						}
					}
				}
				
				break;
			case 4:
				sprintf(sql,"select * from userinfo where staffno = %d;",msg->info.no);
				printf("%s\n",sql);
				
				if(sqlite3_get_table(db,sql,&result,&nrow,&ncolumn,&errmsg)!=0)
				{
					printf("%s\n",errmsg);
					break;
				}
				else
				{
					if(nrow == 0)
					{
						memset(msg->recvmsg,0,DATALEN);
						strcpy(msg->recvmsg,"staffno no exist!");
						send(acceptfd,msg,sizeof(MSG),0);
					}
					else
					{
						memset(sql,0,DATALEN);
				
						sprintf(sql,"update userinfo set phone='%s' where staffno = %d;",msg->info.phone,msg->info.no);
						printf("%s\n",sql);
						if(sqlite3_exec(db,sql,NULL,NULL,&errmsg) != 0)
						{
							printf("%s\n",errmsg);
						}
						else
						{
							memset(msg->recvmsg,0,DATALEN);
							strcpy(msg->recvmsg,"phone modification success!");
							send(acceptfd,msg,sizeof(MSG),0);
							sprintf(s,"修改工号为%d的员工的电话为:%s",msg->info.no,msg->info.phone);
							do_date(msg,s);
							
						}
					}
				}
				
				break;
			case 5:
				sprintf(sql,"select * from userinfo where staffno = %d;",msg->info.no);
				printf("%s\n",sql);
				
				if(sqlite3_get_table(db,sql,&result,&nrow,&ncolumn,&errmsg)!=0)
				{
					printf("%s\n",errmsg);
					break;
				}
				else
				{
					if(nrow == 0)
					{
						memset(msg->recvmsg,0,DATALEN);
						strcpy(msg->recvmsg,"staffno no exist!");
						send(acceptfd,msg,sizeof(MSG),0);
					}
					else
					{
						memset(sql,0,DATALEN);
				
						sprintf(sql,"update userinfo set work='%s' where staffno = %d;",msg->info.work,msg->info.no);
						printf("%s\n",sql);
						if(sqlite3_exec(db,sql,NULL,NULL,&errmsg) != 0)
						{
							printf("%s\n",errmsg);
						}
						else
						{
							memset(msg->recvmsg,0,DATALEN);
							strcpy(msg->recvmsg,"work modification success!");
							send(acceptfd,msg,sizeof(MSG),0);
							sprintf(s,"修改工号为%d的员工的职位为:%s",msg->info.no,msg->info.work);
							do_date(msg,s);
						}
					}
				}
				
				break;
			case 6:
				sprintf(sql,"select * from userinfo where staffno = %d;",msg->info.no);
				printf("%s\n",sql);
				
				if(sqlite3_get_table(db,sql,&result,&nrow,&ncolumn,&errmsg)!=0)
				{
					printf("%s\n",errmsg);
					break;
				}
				else
				{
					if(nrow == 0)
					{
						memset(msg->recvmsg,0,DATALEN);
						strcpy(msg->recvmsg,"staffno no exist!");
						send(acceptfd,msg,sizeof(MSG),0);
					}
					else
					{
						memset(sql,0,DATALEN);
				
						sprintf(sql,"update userinfo set salary='%lf' where staffno = %d;",msg->info.salary,msg->info.no);
						printf("%s\n",sql);
						if(sqlite3_exec(db,sql,NULL,NULL,&errmsg) != 0)
						{
							printf("%s\n",errmsg);
						}
						else
						{
							memset(msg->recvmsg,0,DATALEN);
							strcpy(msg->recvmsg,"salary modification success!");
							send(acceptfd,msg,sizeof(MSG),0);
							sprintf(s,"修改工号为%d的员工的工资为:%lf",msg->info.no,msg->info.salary);
							do_date(msg,s);
						}
					}
				}

				
				break;
			case 8:
				sprintf(sql,"select * from userinfo where staffno = %d;",msg->info.no);
				printf("%s\n",sql);
				
				if(sqlite3_get_table(db,sql,&result,&nrow,&ncolumn,&errmsg)!=0)
				{
					printf("%s\n",errmsg);
					break;
				}
				else
				{
					if(nrow == 0)
					{
						memset(msg->recvmsg,0,DATALEN);
						strcpy(msg->recvmsg,"staffno no exist!");
						send(acceptfd,msg,sizeof(MSG),0);
					}
					else
					{
						memset(sql,0,DATALEN);
				
						sprintf(sql,"update userinfo set date='%s' where staffno = %d;",msg->info.date,msg->info.no);
						printf("%s\n",sql);
						if(sqlite3_exec(db,sql,NULL,NULL,&errmsg) != 0)
						{
							printf("%s\n",errmsg);
						}
						else
						{
							memset(msg->recvmsg,0,DATALEN);
							strcpy(msg->recvmsg,"date modification success!");
							send(acceptfd,msg,sizeof(MSG),0);
							sprintf(s,"修改工号为%d的员工的入职日期为:%s",msg->info.no,msg->info.date);
							do_date(msg,s);
						}
					}
				}

				
				break;
			case 7:
				sprintf(sql,"select * from userinfo where staffno = %d;",msg->info.no);
				printf("%s\n",sql);
				
				if(sqlite3_get_table(db,sql,&result,&nrow,&ncolumn,&errmsg)!=0)
				{
					printf("%s\n",errmsg);
					break;
				}
				else
				{
					if(nrow == 0)
					{
						memset(msg->recvmsg,0,DATALEN);
						strcpy(msg->recvmsg,"staffno no exist!");
						send(acceptfd,msg,sizeof(MSG),0);
					}
					else
					{
						memset(sql,0,DATALEN);
				
						sprintf(sql,"update userinfo set level='%d' where staffno = %d;",msg->info.level,msg->info.no);
						printf("%s\n",sql);
						if(sqlite3_exec(db,sql,NULL,NULL,&errmsg) != 0)
						{
							printf("%s\n",errmsg);
						}
						else
						{
							memset(msg->recvmsg,0,DATALEN);
							strcpy(msg->recvmsg,"level modification success!");
							send(acceptfd,msg,sizeof(MSG),0);
							sprintf(s,"修改工号为%d的员工的评职为:%d",msg->info.no,msg->info.level);
							do_date(msg,s);
						}
					}
				}
				
				break;
			case 9:
				sprintf(sql,"select * from userinfo where staffno = %d;",msg->info.no);
				printf("%s\n",sql);
				
				if(sqlite3_get_table(db,sql,&result,&nrow,&ncolumn,&errmsg)!=0)
				{
					printf("%s\n",errmsg);
					break;
				}
				else
				{
					if(nrow == 0)
					{
						memset(msg->recvmsg,0,DATALEN);
						strcpy(msg->recvmsg,"staffno no exist!");
						send(acceptfd,msg,sizeof(MSG),0);
					}
					else
					{
						memset(sql,0,DATALEN);
				
						sprintf(sql,"update userinfo set passwd='%s' where staffno = %d;",msg->info.passwd,msg->info.no);
						printf("%s\n",sql);
						if(sqlite3_exec(db,sql,NULL,NULL,&errmsg) != 0)
						{
							printf("%s\n",errmsg);
						}
						else
						{
							memset(msg->recvmsg,0,DATALEN);
							strcpy(msg->recvmsg,"passwd modification success!");
							send(acceptfd,msg,sizeof(MSG),0);
							sprintf(s,"修改工号为%d的员工的密码为:%s",msg->info.no,msg->info.passwd);
							do_date(msg,s);
						}
					}
				}
				
				break;	
			case 10:
				break;
				
		}


	return 0;

}
int process_admin_adduser(int acceptfd,MSG * msg)
{

	char sql[DATALEN] = {0};
	char * errmsg =NULL;
	char ** result = NULL;
	int nrow = 0,ncolumn =0;
	int i =0,j = 0;
	int maxno = 0;
	char s[DATALEN] = {0};
	sprintf(sql,"select staffno from userinfo;");
	if(sqlite3_get_table(db,sql,&result,&nrow,&ncolumn,&errmsg) != 0)
	{
		printf("%s\n",errmsg);
		return -1;
	}
	else
	{
		memset(msg->recvmsg,0,DATALEN);
		if(nrow == 0)
		{
			msg->info.no = 1000;
			if( (*((char*)&msg->info.usertype) == 'Y') ||(*((char*)&msg->info.usertype) =='y') )
			{
				msg->info.usertype = 0;
			}
			else if((*((char*)&msg->info.usertype) == 'N') ||(*((char*)&msg->info.usertype) =='n'))
			{
				msg->info.usertype = 1;
			}
			else
			{
				strcpy(msg->recvmsg,"usertype error!");
				send(acceptfd,msg,sizeof(MSG),0);
				return -1;
			}
			memset(sql,0,DATALEN);
			sprintf(sql,"insert into userinfo(staffno integer primary key,usertype integer,name text,passwd text,age integer,phone text,addr text,work text,date text,level integer,salary REAL) values(%d,%d,'%s','%s',%d,'%s','%s','%s','%s',%d,%lf);",msg->info.no,msg->info.usertype,msg->info.name,msg->info.passwd,msg->info.age,msg->info.phone,msg->info.addr,msg->info.work,msg->info.date,msg->info.level,msg->info.salary);
			if(sqlite3_exec(db,sql,NULL,NULL,&errmsg) != 0)
			{
				printf("%s\n",errmsg);
				return -1;
			}
			memset(msg->recvmsg,0,DATALEN);
			strcpy(msg->recvmsg,"add success!");
			send(acceptfd,msg,sizeof(MSG),0);
			sprintf(s," 添加了工号为:%d 用户名为:%s 的用户 ",msg->info.no,msg->info.name);
			do_date(msg,s);
		}
		else
		{
			maxno = 0;
			for(i = 1 ;i <= nrow;i++)
			{
				if((atoi(result[i*ncolumn])) > maxno)
				{
					//	sprintf(&maxno,(result[i*ncolumn]));
					//	printf("::::%d\n",atoi(result[i*ncolumn]));
						maxno = atoi(result[i*ncolumn]);
				}
			}
			
			msg->info.no = maxno + 1;
			printf("msg->info.no = %d \n",maxno);
			if( (*((char*)&msg->info.usertype) == 'Y') ||(*((char*)&msg->info.usertype) =='y') )
			{
				msg->info.usertype = 0;
			}
			else if((*((char*)&msg->info.usertype) == 'N') ||(*((char*)&msg->info.usertype) =='n'))
			{
				msg->info.usertype = 1;
			}
			else
			{
				strcpy(msg->recvmsg,"usertype error!");
				send(acceptfd,msg,sizeof(MSG),0);
				return -1;
			}
			memset(sql,0,DATALEN);
			sprintf(sql,"insert into userinfo values(%d,%d,'%s','%s',%d,'%s','%s','%s','%s',%d,%lf);",msg->info.no,msg->info.usertype,msg->info.name,msg->info.passwd,msg->info.age,msg->info.phone,msg->info.addr,msg->info.work,msg->info.date,msg->info.level,msg->info.salary);
			if(sqlite3_exec(db,sql,NULL,NULL,&errmsg) != 0)
			{
				printf("%s\n",errmsg);
				printf("********************************\n");
				return -1;
			}
			memset(msg->recvmsg,0,DATALEN);
			strcpy(msg->recvmsg,"add success!");
			send(acceptfd,msg,sizeof(MSG),0);	
			sprintf(s," 添加了工号为:%d 用户名为:%s 的用户 ",msg->info.no,msg->info.name);
			do_date(msg,s);
		}
	}
	return 0;

}
int process_admin_deluser(int acceptfd,MSG * msg)
{
	char sql[DATALEN] = {0};
	char * errmsg =NULL;
	char ** result = NULL;
	int nrow = 0,ncolumn =0;
	int i =0,j = 0;
	char s[NAMELEN] = {0};
	sprintf(sql,"select * from userinfo where staffno = %d and name = '%s';",msg->info.no,msg->info.name);
	if(sqlite3_get_table(db,sql,&result,&nrow,&ncolumn,&errmsg) != 0)
	{
		printf("%s\n",errmsg);
		return -1;
	}
	memset(msg->recvmsg,0,DATALEN);
	if(nrow == 0)
	{
		sprintf(msg->recvmsg,"staffno or name error!");
		send(acceptfd,msg,sizeof(MSG),0);
		return -1;
	}
	else
	{
		memset(sql,0,DATALEN);
		sprintf(sql,"delete from userinfo where staffno = %d and name = '%s';",msg->info.no,msg->info.name);
		if(sqlite3_exec(db,sql,NULL,NULL,&errmsg) != 0)
		{
			printf("%s\n",errmsg);
			return -1;
		}

		sprintf(msg->recvmsg,"staffno:%d, name:%s delele success!",msg->info.no,msg->info.name);
		send(acceptfd,msg,sizeof(MSG),0);
		sprintf(s," 删除了工号为:%d 用户名为:%s 的用户 ",msg->info.no,msg->info.name);
		do_date(msg,s);
		//printf("……………………%s\n",msg->recvmsg);
	}

	
	return 0;

}
int process_admin_query(int acceptfd,MSG * msg)
{
	char sql[DATALEN] = {0};
	char * errmsg =NULL;
	char ** result = NULL;
	int nrow = 0,ncolumn =0;
	int i =0,j = 0;
	int mm = 0;
	char s[DATALEN] = {0};
	switch(msg->flags)
	{
		case 1:
			{
				memset(sql,0,DATALEN);
				sprintf(sql,"select * from userinfo where name = '%s';",msg->info.name);
				printf("%s\n",sql);
				sprintf(s," 查找用户名为:%s 的员工信息 ",msg->info.name);
				do_date(msg,s);
				//printf("888888888888888888888888888888888888\n");
				if(sqlite3_get_table(db,sql,&result,&nrow,&ncolumn,&errmsg) != 0)
				{
					printf("%s\n",errmsg);
				}
				else
				{
					memset(msg->recvmsg,0,DATALEN);
					if(nrow == 0)
					{
						strncpy(msg->recvmsg,(char*)&nrow,4);
						send(acceptfd,msg,sizeof(MSG),0);
					}
					else
					{
						printf("%d   %d \n",nrow,ncolumn);
						mm = ncolumn;
						strncpy(msg->recvmsg,(char*)&nrow,4);
						send(acceptfd,msg,sizeof(MSG),0);
					//	printf("##########################33333\n");
						for(i=1;i<=nrow;i++)
						{
					//		printf("^^^^^^^^^^^^^^^^^\n");
							memset(msg->recvmsg,0,DATALEN);
							for(j=0;j<ncolumn;j++)
							{
					//			printf("3333333333333333333333\n");
								if(result[mm] == NULL)
								{
										result[mm] ="##";
								}
								strcat(msg->recvmsg,result[mm]);
								mm++;
								strcat(msg->recvmsg,"\n");
							}
					//		printf("2222222222\n");
							send(acceptfd,msg,sizeof(MSG),0);
					//		printf("msg->recv : %s",msg->recvmsg);
						}
						
					}
			}
			break;
			}
			case 2:
				{
					memset(sql,0,DATALEN);
					sprintf(sql,"select * from userinfo;");
					printf("%s\n",sql);
					sprintf(s," 查找所有用员工的信息 ");
					do_date(msg,s);
				//	printf("888888888888888888888888888888888888\n");
					if(sqlite3_get_table(db,sql,&result,&nrow,&ncolumn,&errmsg) != 0)
					{
						printf("%s\n",errmsg);
					}
					else
					{
						memset(msg->recvmsg,0,DATALEN);
						if(nrow == 0)
						{
							strncpy(msg->recvmsg,(char*)&nrow,4);
							send(acceptfd,msg,sizeof(MSG),0);
						}
						else
						{
							strncpy(msg->recvmsg,(char*)&nrow,4);
							send(acceptfd,msg,sizeof(MSG),0);
							mm = ncolumn;
							printf("%d   %d %d \n",nrow,ncolumn,mm);
							for(i=1;i<=nrow;i++)
							{
								printf("##########################33333\n");
								memset(msg->recvmsg,0,DATALEN);
								for(j=0;j<ncolumn;j++)
								{
									if(result[mm] == NULL)
									{
											result[mm] ="##";
									}
									strcat(msg->recvmsg,result[mm]);
									mm++;
									strcat(msg->recvmsg,"\n");
								}
								send(acceptfd,msg,sizeof(MSG),0);
								printf("msg->recv : %s \n",msg->recvmsg);
							}
							
						}
					}
					break;
				}
			case 3:
				break;
	}
	return 0;

}
int process_admin_history(int acceptfd,MSG * msg)
{

	char sql[DATALEN] = {0};
	char * errmsg = NULL;
	char ** result = NULL;
	int nrow = 0, ncolumn = 0;
	int i = 0,j = 0;
//	char s[NAMELEN] = {0};
	sprintf(sql,"select *  from historyinfo;");
	if(sqlite3_get_table(db,sql,&result,&nrow,&ncolumn,&errmsg) != 0)
	{
			printf("%s\n",errmsg);
			return -1;
	}
	
	else
	{
		printf("nrow = %d\n",nrow);
		strncpy(msg->recvmsg,(char*)&nrow,4);
		//printf("msg->recvmsg:%s \n",(msg->recvmsg));
		send(acceptfd,msg,sizeof(MSG),0);
		memset(msg->recvmsg,0,DATALEN);
		
		if(nrow == 0)
		{
			strcpy(msg->recvmsg,"no historyinfo!");
			send(acceptfd,msg,sizeof(MSG),0);
//			sprintf(s," 查看所有历史操作 ");
//			do_date(msg,s);
		}
		else
		{

			for(i=1;i<=nrow;i++)
			{
				memset(msg->recvmsg,0,DATALEN);
				for(j=0;j<ncolumn;j++)
				{
					strcat(msg->recvmsg,result[i*ncolumn+j]);
					strcat(msg->recvmsg,"\n");
				}
				send(acceptfd,msg,sizeof(MSG),0);
//				sprintf(s," 查看所有历史操作 ");
//				do_date(msg,s);
			}
		}
	}
	
	return 0;

}
int process_client_quit(int acceptfd,MSG * msg)
{
	recv(acceptfd,msg,sizeof(MSG),0);
	printf("admin out!\n");
	return 0;

}


	




int process_client_request(int acceptfd,MSG *msg)
{
	printf("---------%s-------%d .\n",__func__,__LINE__);
	switch(msg->msgtype)
	{
		case USER_LOGIN:
		case ADMIN_LOGIN:
			process_user_or_admin_login(acceptfd,msg);
			break;
		case USER_MODIFY:
			process_user_modif(acceptfd,msg);
			break;
		case USER_QUERY:
			process_user_query(acceptfd,msg);
			break;
		case ADMIN_MODIFY:
			process_admin_modif(acceptfd,msg);
			break;

		case ADMIN_ADDUSER:
			process_admin_adduser(acceptfd,msg);
			break;

		case ADMIN_DELUSER:
			process_admin_deluser(acceptfd,msg);
			break;
		case ADMIN_QUERY:
			process_admin_query(acceptfd,msg);
			break;
		case ADMIN_HISTORY:
			process_admin_history(acceptfd,msg);
			break;
		case QUIT:
			process_client_quit(acceptfd,msg);
			break;
		default:
			break;
			
	}
}

int main(int argc,char * argv[])
{
	int sockfd;
	int acceptfd;
	ssize_t recv_size;
	struct sockaddr_in serviceaddr;
	struct sockaddr_in clientaddr;

	socklen_t addrlen = sizeof(serviceaddr);
	socklen_t cli_len = sizeof(clientaddr);



	MSG msg;

	char * errmsg;

	if(sqlite3_open(DATABASE,&db) != 0 )
	{
		printf("%s\n",sqlite3_errmsg(db));
	}else
	{
		printf("database opene success\n");
	}

	if(sqlite3_exec(db,"create table userinfo(staffno integer primary key,usertype integer,name text,passwd text,age integer,phone text,addr text,work text,date text,level integer,salary REAL);",NULL,NULL,&errmsg) != 0)
	{
			printf("%s\n",errmsg);
		
	}
	else
	{
		printf("create userinfo table success\n");
	}


	if(sqlite3_exec(db,"create table historyinfo(time text,name text,words text);",NULL,NULL,&errmsg) != 0)
	{
			printf("%s\n",errmsg);
		
	}
	else
	{
		printf("create historyinfo table success\n");
	}

	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd == -1)
	{
		perror("sockfd failed\n");
		exit(-1);
	}

	printf("sockfd : %d \n",sockfd);

	int b_reuse = 1;

	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &b_reuse, sizeof(int));

	memset(&serviceaddr,0,sizeof(serviceaddr));
	memset(&clientaddr, 0, sizeof(clientaddr));

	serviceaddr.sin_family = AF_INET;
	serviceaddr.sin_port = htons(6000);
	serviceaddr.sin_addr.s_addr = inet_addr("192.168.8.3");
	//serviceaddr.sin_addr.s_addr = inet_addr("192.168.3.125");


	if(bind(sockfd,(const struct sockaddr *)&serviceaddr,addrlen) == -1)
	{
		printf("bind failed\n");
		exit(-1);
	}


	if(listen(sockfd,10) == -1)
	{
		printf("listen failed\n");
		exit(-1);
	}
	fd_set readfds,tempfds;

	FD_ZERO(&readfds);
	FD_ZERO(&tempfds);

	FD_SET(sockfd,&readfds);

	int nfds = sockfd;

	int retval;

	int i = 0;
	
	while(1)
	{
		tempfds = readfds;
		retval = select(nfds + 1,&tempfds,NULL,NULL,NULL);
		for(i=0 ;i <nfds + 1;  i++)
		{
			if(FD_ISSET(i,&tempfds))
			{
				if(i == sockfd)
				{
					acceptfd = accept(sockfd,(struct sockaddr *)&clientaddr,&cli_len);
					if(acceptfd == -1)
					{
							printf("acceptfd failed\n");
							exit(-1);
					}
					printf("ip: %s \n",inet_ntoa(clientaddr.sin_addr));
					FD_SET(acceptfd,&readfds);
					nfds = nfds > acceptfd?nfds:acceptfd;
				}
				else
				{
					recv_size = recv(i,&msg,sizeof(msg),0);
					printf("msg.type:%#x\n",msg.msgtype);
					if(recv_size == -1)
					{
						printf("recv failed\n");
						continue;
					}
					else if(recv_size == 0)
					{
						printf("client shutdown \n");
						close(i);
						FD_CLR(i,&readfds);
					}
					else
					{
						process_client_request(i,&msg);
					}
				}
				
			}


		}

	}

	
	close(sockfd);
	return 0;



	

}







