


#include"comm.h"



void do_admin_query(int sockfd,MSG * msg)
{
	int n = 0,nrow;
	
	int i = 0;
	char * p= NULL;

	while(1)
	{
		msg->msgtype = ADMIN_QUERY;
		printf("\n");
		printf("*****************************************************************************\n");
		printf("*****************1.按人名查询  2.查找所有 3.退出*****************************\n");
		printf("*****************************************************************************\n");	
		printf("请输入你的选择>>:");
		scanf("%d",&n);
		getchar();

		switch(n)
		{
			case 1:
				msg->flags = 1;
				printf("请输入你要查找的人名：");
				memset(msg->info.name,0,NAMELEN);
				fgets(msg->info.name,NAMELEN,stdin);
				msg->info.name[strlen(msg->info.name)-1] ='\0';
				printf("name : %s\n",msg->info.name);
				send(sockfd,msg,sizeof(MSG),0);
				recv(sockfd,msg,sizeof(MSG),0);
				nrow = *((int *)(msg->recvmsg));
				printf("nrow : %d\n",nrow);
				if(nrow == 0)
				{
					printf("name is error\n");
					break;
				}
				printf("staffno	usertype name passwd age phone addr work data level salary \n");
				printf("*******************************************************************\n");
				i = 0;
				while(i < nrow)
				{
					n = 0;
				//	printf("&&&&&&&&&&&&&&&&&&&&&&&&&\n");
					memset(msg->recvmsg,0,DATALEN);
					recv(sockfd,msg,sizeof(MSG),0);

				//	printf("%s\n",msg->recvmsg);

					p = msg->recvmsg;

					while(*p != '\0') 
					{
						if(*p == '\n')
						{
							*p = '\0';
						}
						p++;
					}

					p  = msg->recvmsg;
					while(n < 11)
					{
		
					
						if(strcmp(p,"##") == 0)
						{
							*p =' ';
							*(++p) = ' ';
						}
						printf("%s  ",p);
			
						n++;
						while(*p != '\0')
						{
							p++;
						}
							p++;
					}
					printf("\n");
					i++;
				}
				break;
				case 2:
					{
						msg->flags = 2;
						send(sockfd,msg,sizeof(MSG),0);
						recv(sockfd,msg,sizeof(MSG),0);
						nrow = *((int *)(msg->recvmsg));
						printf("nrow ; %d\n",nrow);
						if(nrow == 0)
						{
							printf("name is error\n");
							break;
						}
						printf("staffno usertype name passwd age phone addr work data level salary \n");
						printf("*******************************************************************\n");
						i=0;
						while(i < nrow)
						{
							n = 0;
							memset(msg->recvmsg,0,DATALEN);
							recv(sockfd,msg,sizeof(MSG),0);
					
						//	printf("%s\n",msg->recvmsg);
					
							p = msg->recvmsg;
					
							while(*p != '\0') 
							{
								//printf("222222\n");
								if(*p == '\n')
								{
									*p = '\0';
								}
								p++;
							}
					
							p  = msg->recvmsg;
							while(n < 11)
							{
					
							
								if(strcmp(p,"##") == 0)
								{
									*p =' ';
									*(++p) = ' ';
								}
								printf("%s	",p);
					
								n++;
								while(*p != '\0')
								{
									p++;
								}
								p++;
							}
							printf("\n");
							i++;
						}
						break;
					}
				case 3:
					msg->flags = 3;
					send(sockfd,msg,sizeof(MSG),0);
					goto lite1;
		}
		n = 0;
	}
lite1:
	printf("\n");

}


void do_admin_modif(int sockfd,MSG * msg)
{
		int n = 0;
		msg->msgtype = ADMIN_MODIFY;
		while(1)
		{

			printf("*****************************************************************************\n");
			printf("*************1.姓名 2.年龄  3.家庭住址 4.电话  5.职位************************\n");
			printf("*************6.工资 7.评级  8.入职时间 9.密码  10.退出***********************\n");
			printf("*****************************************************************************\n");
			printf("请输入你的选择>> ");
			scanf("%d",&n);
			getchar();
			
			switch(n)
			{
				case 1:
					printf("请输入需要修改人的供工号：");
					scanf("%d",&msg->info.no);
					getchar();
					printf("请输入名字：");
					fgets(msg->info.name,NAMELEN,stdin);
					msg->flags = 1;
					msg->info.name[strlen(msg->info.name)-1] = '\0';
					
					send(sockfd,msg,sizeof(MSG),0);
					recv(sockfd,msg,sizeof(MSG),0);
					printf("%s\n",msg->recvmsg);
					break;
				case 2:
					printf("请输入需要修改人的供工号：");
					scanf("%d",&msg->info.no);
					getchar();
					printf("请输入年龄：");
					scanf("%d",&msg->info.age);
					getchar();
					msg->flags = 2;
					
					send(sockfd,msg,sizeof(MSG),0);
					recv(sockfd,msg,sizeof(MSG),0);
					printf("%s\n",msg->recvmsg);
					break;
				case 3:
					printf("请输入需要修改人的供工号：");
					scanf("%d",&msg->info.no);
					getchar();
					printf("请输入家庭地址：");
					fgets(msg->info.addr,DATALEN,stdin);
					msg->flags = 3;
					msg->info.addr[strlen(msg->info.addr)-1] = '\0';
					
					send(sockfd,msg,sizeof(MSG),0);
					recv(sockfd,msg,sizeof(MSG),0);
					printf("%s\n",msg->recvmsg);
					break;
				case 4:
					printf("请输入需要修改人的供工号：");
					scanf("%d",&msg->info.no);
					getchar();
					printf("请输入电话：");
					fgets(msg->info.phone,DATALEN,stdin);
					msg->flags = 4;
					msg->info.phone[strlen(msg->info.phone)-1] = '\0';
					
					send(sockfd,msg,sizeof(MSG),0);
					recv(sockfd,msg,sizeof(MSG),0);
					printf("%s\n",msg->recvmsg);
					break;

				case 5:
					printf("请输入需要修改人的供工号：");
					scanf("%d",&msg->info.no);
					getchar();
					printf("请输入职位：");
					fgets(msg->info.work,DATALEN,stdin);
					msg->flags = 5;
					msg->info.work[strlen(msg->info.work)-1] = '\0';
					
					send(sockfd,msg,sizeof(MSG),0);
					recv(sockfd,msg,sizeof(MSG),0);
					printf("%s\n",msg->recvmsg);
					break;
				case 6:
					printf("请输入需要修改人的供工号：");
					scanf("%d",&msg->info.no);
					getchar();
					printf("请输入工资：");
					scanf("%lf",&msg->info.salary);
					getchar();
					msg->flags = 6;
					
					send(sockfd,msg,sizeof(MSG),0);
					recv(sockfd,msg,sizeof(MSG),0);
					printf("%s\n",msg->recvmsg);
					break;
				case 8:
					printf("请输入需要修改人的供工号：");
					scanf("%d",&msg->info.no);
					getchar();
					printf("请输入入职时间：");
					fgets(msg->info.date,DATALEN,stdin);
					msg->flags = 8;
					msg->info.date[strlen(msg->info.date)-1] = '\0';
					
					send(sockfd,msg,sizeof(MSG),0);
					recv(sockfd,msg,sizeof(MSG),0);
					printf("%s\n",msg->recvmsg);
					break;
				case 7:
					printf("请输入需要修改人的供工号：");
					scanf("%d",&msg->info.no);
					getchar();
					printf("请输入年龄：");
					scanf("%d",&msg->info.level);
					getchar();
					msg->flags = 7;
					send(sockfd,msg,sizeof(MSG),0);
					recv(sockfd,msg,sizeof(MSG),0);
					printf("%s\n",msg->recvmsg);
					break;
				case 9:
					printf("请输入需要修改人的供工号：");
					scanf("%d",&msg->info.no);
					getchar();
					printf("请输入新密码：");
					fgets(msg->info.passwd,DATALEN,stdin);
					msg->flags = 9;
					msg->info.passwd[strlen(msg->info.passwd)-1] = '\0';
					send(sockfd,msg,sizeof(MSG),0);
					recv(sockfd,msg,sizeof(MSG),0);
					printf("%s\n",msg->recvmsg);
					break;
				case 10:
					goto table3;
				default:
					printf("输入错误，请重新输入！\n");
					
			}
			n = 0;
			
		}
	table3:
		printf("\n");

}


void do_admin_adduser(int sockfd,MSG * msg)
{
	int n = 0;
	do{
		msg->msgtype=ADMIN_ADDUSER;
		printf("*********************热烈欢迎新员工***********************\n");
		printf("请输入用户名：");
		fgets(msg->info.name,NAMELEN,stdin);
		msg->info.name[strlen(msg->info.name) -1] = '\0';
		printf("请输入用户密码：");
		fgets(msg->info.passwd,8,stdin);
		msg->info.passwd[strlen(msg->info.passwd) -1] = '\0';
		printf("请输入年龄：");
		scanf("%d",&msg->info.age);
		getchar();
		printf("请输入电话：");
		fgets(msg->info.phone,NAMELEN,stdin);
		msg->info.phone[strlen(msg->info.phone)-1] ='\0';
		printf("请输入家庭住址：");
		fgets(msg->info.addr,DATALEN,stdin);
		msg->info.addr[strlen(msg->info.addr)-1] ='\0';
		printf("请输入职位：");
		fgets(msg->info.work,DATALEN,stdin);
		msg->info.work[strlen(msg->info.work)-1] = '\0';
		printf("请输入入职日期(格式：xxxx:xx:xx)：");
		fgets(msg->info.date,DATALEN,stdin);
		msg->info.date[strlen(msg->info.date)] = '\0';
		printf("请输入评职(1~5.5为最高，新员工为1)：");
		scanf("%d",&msg->info.level);
		getchar();
		printf("请输入工资：");
		scanf("%lf",&msg->info.salary);
		getchar();
		printf("是否为管理员(Y/N,y/n)：");
		scanf("%c",(char*)&msg->info.usertype);
		getchar();
		send(sockfd,msg,sizeof(MSG),0);

		memset(msg->recvmsg,0,DATALEN);
		recv(sockfd,msg,sizeof(MSG),0);
		printf("msg->info.userype: %d\n",msg->info.usertype);
		printf("新员工的工号是：%d\n",msg->info.no);
		printf("%s\n",msg->recvmsg);
		printf("是否继续添加(1:0)：");
		scanf("%d",&n);
		getchar();
		
	}while(n == 1);
	
	
	
	
}


void do_admin_deluser(int sockfd,MSG * msg)
{
		int n = 0;
		do{
			msg->msgtype=ADMIN_DELUSER;
			printf("*********************删除员工信息***********************\n");
			printf("请输入要删除的工号：");
			scanf("%d",&msg->info.no);
			getchar();
			printf("请输入要删除的用户名：");
			fgets(msg->info.name,NAMELEN,stdin);
			msg->info.name[strlen(msg->info.name) -1] = '\0';
			send(sockfd,msg,sizeof(MSG),0);

			memset(msg->recvmsg,0,DATALEN);
			recv(sockfd,msg,sizeof(MSG),0);
			printf("%s\n",msg->recvmsg);
			printf("是否继续添加(1:0)：");
			scanf("%d",&n);
			getchar();
		
		}while(n == 1);
}


int  do_admin_history(int sockfd, MSG * msg)
{
	int nrow = 0;
	int n = 0;
	int i = 0;
	char *p = NULL;
	msg->msgtype = ADMIN_HISTORY;
	send(sockfd,msg,sizeof(MSG),0);
	memset(msg->recvmsg,0,DATALEN);
	recv(sockfd,msg,sizeof(MSG),0);
	nrow = *((int *)msg->recvmsg);
	//printf("nrow = %d\n",nrow);
	if(nrow == 0)
	{
		recv(sockfd,msg,sizeof(MSG),0);
		printf("%s\n",msg->recvmsg);
		
	}
	else
	{

		while(i<nrow)
		{
			i++;
			n = 0;
			memset(msg->recvmsg,0,DATALEN);
			recv(sockfd,msg,sizeof(MSG),0);
		//	printf("%s\n",msg->recvmsg);

			p = msg->recvmsg;

			while(*p != '\0') 
			{
				if(*p == '\n')
				{
					*p = '\0';
				}
				p++;
			}

			p  = msg->recvmsg;
			while(n < 11)
			{

				{
					if(strcmp(p,"##") == 0)
					{
						*p =' ';
						*(++p) = ' ';
					}
					printf("%s  ",p);
				}
				n++;
				while(*p != '\0')
				{
						p++;
				}
				p++;
			}
			printf("\n");
			}
		}
}


void admin_menu(int sockfd,MSG* msg)
{
	int n  =0 ;
	while(1)
	{
		printf("*****************************************************************************\n");
		printf("********1.查询  2.修改 3.添加用户 4. 删除用户 5.查询历史记录  6退出 *********\n");
		printf("*****************************************************************************\n");	

		printf("请输入你的选择>>:");
		scanf("%d",&n);
		getchar();
		switch(n)
		{
			case 1:
				do_admin_query(sockfd,msg);
				break;
			case 2:
				do_admin_modif(sockfd,msg);
				break;
			case 3:
				do_admin_adduser(sockfd,msg);
				break;
			case 4:
				do_admin_deluser(sockfd,msg);
				break;
			case 5:
				do_admin_history(sockfd,msg);
				break;
			case 6:
				msg->msgtype = ADMIN_QUERY;
				send(sockfd,msg,sizeof(MSG),0);
				close(sockfd);
				exit(0);
				break;
			default:
				printf("输入有误，请重新输入！");
		}		
		n = 0;
	}

}





void do_user_modif(int sockfd,MSG*msg)
{
	int n = 0;
	msg->msgtype = USER_MODIFY;
	while(1)
	{
		printf("*****************************************************************************\n");
		printf("*************1.家庭地址  2.电话    3.密码   4.退出***************************\n");
		printf("*****************************************************************************\n");
		printf("请输入你的选择>> ");
		scanf("%d",&n);
		getchar();
		
		switch(n)
		{
			case 1:
				printf("请输入家庭地址：");
				fgets(msg->info.addr,DATALEN,stdin);
				msg->flags = 1;
				msg->info.addr[strlen(msg->info.addr)-1] = '\0';
				send(sockfd,msg,sizeof(MSG),0);
				recv(sockfd,msg,sizeof(MSG),0);
				printf("%s\n",msg->recvmsg);
				break;
			case 2:
				printf("请输入电话：");
				fgets(msg->info.phone,DATALEN,stdin);
				msg->flags = 2;
				msg->info.phone[strlen(msg->info.phone)-1] = '\0';
				send(sockfd,msg,sizeof(MSG),0);
				recv(sockfd,msg,sizeof(MSG),0);
				printf("%s\n",msg->recvmsg);
				break;
			case 3:
				printf("请输入新密码：");
				fgets(msg->info.passwd,DATALEN,stdin);
				msg->flags = 3;
				msg->info.passwd[strlen(msg->info.passwd)-1] = '\0';
				send(sockfd,msg,sizeof(MSG),0);
				recv(sockfd,msg,sizeof(MSG),0);
				printf("%s\n",msg->recvmsg);
				break;
			case 4:
				goto table3;
			default:
				printf("输入错误，请重新输入！\n");
				
		}
		n = 0;
		
	}
table3:
	printf("\n");
	
}




void do_user_query(int sockfd,MSG*msg)
{

	char * p ;
	int n = 0;
	printf("----------------%s---------%d\n",__func__,__LINE__);
	msg->msgtype = USER_QUERY;
	msg->usertype = USER;
	memset(msg->recvmsg,0,DATALEN);
	send(sockfd,msg,sizeof(MSG),0);

	
	recv(sockfd,msg,sizeof(MSG),0);
	
	printf("staffno	 usertype  name  passwd  age  phone  addr  work  data   level  salary\n");
	printf("*****************************************************************************\n");

	//printf("%s\n",msg->recvmsg);

	p = msg->recvmsg;

	while(*p != '\0') 
	{
		if(*p == '\n')
		{
			*p = '\0';
		}
		p++;
	}

	p  = msg->recvmsg;
	while(n < 11)
	{
//		if((n==0)||(n==1)||(n==4)||(n==9))
//		{
//			printf("%-7d",*((int *)p));
//		}
//		else
		{
			if(strcmp(p,"##") == 0)
			{
				*p =' ';
				*(++p) = ' ';
			}
			printf("%-7s",p);
		}
		n++;
		while(*p != '\0')
		{
				p++;
		}
		p++;
	}
	printf("\n");

	
}






void  user_menu(int sockfd, MSG*msg)
{
	printf("----------------%s---------%d\n",__func__,__LINE__);
	int n = 0;
	while(1)
	{
		printf("*****************************************************************************\n");
		printf("********************1.查询  2.修改   3.退出 *********************************\n");
		printf("*****************************************************************************\n");
		printf("请输入你的选择>> ");
		scanf("%d",&n);
		getchar();
	    
		switch(n)
		{

			case 1:
				do_user_query(sockfd,msg);
				break;
			case 2:
				do_user_modif(sockfd,msg);
				break;
			case 3:
				msg->msgtype = QUIT;
				send(sockfd,msg,sizeof(MSG),0);

				close(sockfd);
				exit(0);
			default:
				printf("输入有误，请重新输入！\n");

		}
	
		n = 0;
		

	}
}







int admin_or_user_login(int sockfd,MSG * msg)
{
	printf("---------%s-------%d .\n",__func__,__LINE__);

//	memset(msg->username,0,NAMELEN);

	printf("请输入工号:");
	scanf("%d",&msg->staffno);
	getchar();
//	msg->username[strlen(msg->username) - 1]  = '\0';
	memset(msg->passwd,0,DATALEN);
	printf("请输入密码(6):");
	fgets(msg->passwd,DATALEN,stdin);
	msg->passwd[strlen(msg->passwd) -1] = '\0';

//	printf("%d\n",msg->staffno);
//	printf("%s\n",msg->passwd);

	send(sockfd,msg,sizeof(MSG),0);

	recv(sockfd,msg,sizeof(MSG),0);


//	printf("msg->recvmsg:%s\n",msg->recvmsg);


	if(strncmp(msg->recvmsg,"OK", 2) == 0)
	{
		if(msg->usertype == ADMIN)
		{
			printf("欢迎工号 %d 管理员登录：\n",msg->staffno);
			admin_menu(sockfd,msg);		
		}
		else if(msg->usertype == USER)
		{
			printf("欢迎工号 %d 普通用户登录：\n",msg->staffno);
			user_menu(sockfd,msg);
			
		}
		
	}else
	{
		printf("登录失败！%s\n",msg->recvmsg);
		return -1;
	}
	return 0;

	

}






//do_login 登录

int do_login(int sockfd)
{
	int n =0;
	MSG msg ;


	while(1)
	{
		printf("*****************************************************************************\n");
		printf("****************1.管理员登录  2.普通用户登录   3.退出 ***********************\n");
		printf("*****************************************************************************\n");
tabel1:

		printf("请输入你的选择：");
		scanf("%d",&n);
		getchar();

		switch(n)
		{
			case 1:
				msg.msgtype = ADMIN_LOGIN;
				msg.usertype = ADMIN;
				break;
			case 2:
				msg.msgtype = USER_LOGIN;
				msg.usertype = USER;
				break;

			case 3:
				msg.msgtype = QUIT;
				if(send(sockfd,&msg,sizeof(MSG),0)< 0)
				{
					perror("do_login send error\n");
					return -1;
				}
				close(sockfd);
				exit(0);

			default:
				printf("你的输入有误，请重新输入：\n");
				goto tabel1;
				
		}	
		admin_or_user_login(sockfd,&msg);
		n = 0 ;
		

		
	}
}







int main(int argc,char * argv[])
{

	int sockfd ;
	int acceptfd;
	ssize_t recv_szie,send_size;
	struct sockaddr_in serviceaddr;
	struct sockaddr_in clientaddr;
	socklen_t addr_len = sizeof(serviceaddr);
	socklen_t cli_len = sizeof(clientaddr);

	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd == -1)
	{
		perror("socket failed \n");
		exit(-1);

	}
	printf("sockfd:%d\n",sockfd);


	memset(&serviceaddr,0,sizeof(serviceaddr));
	memset(&clientaddr,0,sizeof(clientaddr));

	serviceaddr.sin_family = AF_INET;
	serviceaddr.sin_port = htons(6000);
	serviceaddr.sin_addr.s_addr = inet_addr("192.168.8.3");
	//serviceaddr.sin_addr.s_addr = inet_addr("192.168.3.125");

	
	if(connect(sockfd,(const struct sockaddr*)&serviceaddr,addr_len) == -1)
	{
		perror("connect failed.\n");
		exit(-1);
	}

	
	do_login(sockfd);

	close(sockfd);

	return 0;



}







