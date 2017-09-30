#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>
#include <asm/types.h>
#include <asm/byteorder.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <fcntl.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <asm/types.h>
#include <netinet/in.h>

#define SUCCESS			0
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
bool getUrl(char *filename,char *str)
{
    CURL *curl;
    CURLcode res;
    FILE *fp;
    char buffer[32]={0};
    strcat(buffer,"http://");
    strcat(buffer,str);
    if ((fp = fopen(filename, "w")) == NULL)  // 返回结果用文件存储
        return false;
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: Agent-007");
    curl = curl_easy_init();    // 初始化
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);// 改协议头
        //curl_easy_setopt(curl, CURLOPT_URL,"http://192.168.7.10");
        curl_easy_setopt(curl, CURLOPT_URL,buffer);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp); //将返回的http头输出到fp指向的文件
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, fp); //将返回的html主体数据输出到fp指向的文件
        res = curl_easy_perform(curl);   // 执行
        if (res != 0) {

            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);
        }
        fclose(fp);
        return true;
    }
    return true;
}
/** 功能：检测字符串 str1中是否包含字符串str2，如果有将str2后面的内容" "之jian,存入str3**/

int check_change_line(char *str1, char *str2, char *str3)
{
    char *p1=NULL;
	  char *ch = NULL;
    char *p2 = NULL;
    char *p3 = NULL;
	  unsigned int len;
	  unsigned int offset;
	  unsigned int i=0;
    unsigned int j=0;
	  ch = strstr( str1, str2 );
	  if (ch == NULL)
	  return 1;
	  offset = ( unsigned int ) ( ch - str1 );
    len = strlen( str2 );
    p1=str1+offset+len;
    int count = 0;
    int a=strlen(p1);
    while(a>0)
    {
    	if(count==2)
    		break;
    	if(*(p1+i)==34)
    		count++;
    	else
        {
            *(str3+j)=*(p1+i);
            j++;
         }
       a--;
       i++;
     }
	   return 0;
}
int change_html_para(char * str,char *str1)
{
	char tmp[100];
	char buffer_name[64]={0};
	char buffer_value[64]={0};
	char *ch = NULL;
	  ch = strstr( str, "<tr><td class=board1>" );
	  if (ch != NULL)
	{
	    check_change_line(str,"name=",buffer_name);
	    check_change_line(str,"value=",buffer_value);
		strcat(str1,"\"");
		strcat(str1,buffer_name);
		strcat(str1,"\"");
		strcat(str1,":");
	    strcat(str1,"\"");
	    strcat(str1,buffer_value);
	    strcat(str1,"\"");
	    strcat(str1,",");
		return 0;
	}


	return -1;
}
off_t LLS_Get_file_size(char *neme_file)
{
	struct stat msg_file;
	bzero( &msg_file, sizeof( msg_file ) );
	stat( neme_file, &msg_file );
	return msg_file.st_size;
}
#define LEN_BUF_READ_FILE_ADD 1024 //申请比文件大小大这么多字节的缓冲
#define LEN_BUF_ONE_LINE 1024 //一行文件的最大字节数
#define PATH_HTML "./get.html"
int get_param(char *str1)
{
	int res;
	unsigned int size;
//	char *query = NULL;
	char *buf = NULL;
	char *ch; //字符型指针.
	char temp[LEN_BUF_ONE_LINE]; //用于将文件逐行读入的缓存 .        //1024
	FILE *fp=NULL;
	char filename[256]; //保存文件名 or文件路径
	off_t size_file; //用于文件大小和偏移量。

	bzero( filename, sizeof( filename ) );
	sprintf( filename, "%s", PATH_HTML); //

	// /*检测文件是否存在 */
	res = access( filename, F_OK ); // F_OK只判断文件是否存在 ,R_OK只判断是否有读权限 ,W_OK只判断是否有写权限 ,X_OK判断是否有执行权限
	if (res == -1)
		return -1; //如果指定的存取方式有效，返回 0，反之返回 -1.

	// /*获得文件大小 */
	size_file = LLS_Get_file_size( filename );
	if (size_file <= 1)
		return -1;
	fp = fopen( filename, "r" );//打开文件，文件顺利打开后，返回指向该流的文件指针。反之返回 NULL，并把错误代码存在 error 中。"r"表示以只读方式打开文件 .
	if (fp == NULL)
		return -1;

	while (1)
	{
		bzero( temp, LEN_BUF_ONE_LINE );
		ch = fgets( temp, LEN_BUF_ONE_LINE, fp ); //从文件结构体指针fp中读取数据，到temp，每次读取一行。
	//读取的数据保存在 buf指向的字符数组中，每次最多读取 LEN_BUF_ONE_LINE-1个字符（第LEN_BUF_ONE_LINE个字符赋 '\0'），
	//如果文件中的该行，不足LEN_BUF_ONE_LINE个字符，则读完该行就结束。
	//如果函数读取成功，则返回指针temp，失败则返回 NULL。                            //fgetc能够实现fp自加
		if (ch == NULL)
			break;
		change_html_para(temp,str1);//将参数写入缓存中的bc_para_set.htlm
	}
	fclose( fp );
   return 0;
}
int main(int argc,char * argv[])
{
	char json_param[256]={0};
	char json_param1[256]={0};
	if(argc != 2)
	{
		printf("please put like this:'./get 192.168.7.10'\n");
		return false;
	}
    getUrl("./get.html",argv[1]);
    get_param(json_param);
    int a = strlen(json_param);
    *( json_param+a-1) = 125;
    strcat(json_param1,"{");
    strcat(json_param1,json_param);
    printf("json_param1=%s\n",json_param1);
}
