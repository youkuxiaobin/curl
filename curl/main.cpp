/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  libcurl的使用文件
 *
 *        Version:  1.0
 *        Created:  2014年03月01日 17时10分19秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liyankun (), liyankun@baidu.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <string>
#include <curl/curl.h>
#include <string.h>
#include <errno.h>

using namespace std;
#define LOGDBG(fmt, argv...) do {\
	printf("DBG[%s:%d]	%s	"fmt"\n", __FILE__, __LINE__, __FUNCTION__, ##argv);\
}while(0)

//ERRMSG
#define LOGERR(fmt, argv...) do {\
	printf("ERR[%s:%d]	%s	"fmt"(%s)\n", __FILE__, __LINE__, __FUNCTION__, ##argv, strerror(errno));\
}while(0)	
#if 0
static char errorBuffer[CURL_ERROR_SIZE];
static int writer(char *, size_t, size_t, string *);
static bool init(CURL *&, char *,string *);

int main(int argc, char** argv)
{
	CURL *conn = NULL;
	CURLcode code;
	string buffer;

	//初始化
	curl_global_init(CURL_GLOBAL_DEFAULT);
	char* url=argv[1];
	if (!init(conn,url,&buffer ))
	{
		fprintf(stderr, "Connection initializion failed\n");
		exit(EXIT_FAILURE);
	}
	code = curl_easy_perform(conn);

	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to get '%s' [%s]\n", url, errorBuffer);
		exit(EXIT_FAILURE);
	}
	curl_easy_cleanup(conn);
	printf("%s\n",buffer.c_str());
	return 0;

}

static bool init(CURL *&conn, char *url,string *p_buffer)
{
	CURLcode code;
	conn = curl_easy_init();
	if (conn == NULL)
	{
		fprintf(stderr, "Failed to create CURL connection\n");
		exit(EXIT_FAILURE);
	}
	code = curl_easy_setopt(conn, CURLOPT_ERRORBUFFER, errorBuffer);
	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to set error buffer [%d]\n", code);
		return false;
	}
	code = curl_easy_setopt(conn, CURLOPT_URL, url);
	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to set URL [%s]\n", errorBuffer);
		return false;
	}
	code = curl_easy_setopt(conn, CURLOPT_FOLLOWLOCATION, 1);
	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to set redirect option [%s]\n", errorBuffer);
		return false;
	}
	code = curl_easy_setopt(conn, CURLOPT_WRITEFUNCTION, writer);
	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to set writer [%s]\n", errorBuffer);
		return false;
	}
	code = curl_easy_setopt(conn, CURLOPT_WRITEDATA, p_buffer);
	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to set write data [%s]\n", errorBuffer);
		return false;
	}
	return true;
}

static int writer(char *data, size_t size, size_t nmemb, string *writerData)
{
	unsigned long sizes = size * nmemb;
	if (writerData == NULL) return 0;
	writerData->append(data, sizes);
	return sizes;
}
#endif
size_t read_callback(void* ptr, size_t size, size_t nmemb, void* stream)
{
	uint32_t len = size* nmemb;

	char* buf = (char*)calloc(1, len+1);
	if (buf == NULL)
	{
		LOGERR("calloc failed");
		return 0;
	}
	memcpy(buf, (char*)ptr, len);
	LOGDBG("#########################\n %s", buf);
	free(buf);
	return len;

}
int main(int argc, char** argv)
{
	CURLcode ret;
	CURL* conn;
	ret = curl_global_init(CURL_GLOBAL_ALL);
	if (ret != CURLE_OK)
	{
		LOGERR("curl_global_init failed");
		return -1;		
	}
	
	conn = curl_easy_init();
	if (NULL == conn)
	{
		LOGERR("curl_easy_init failed");
		return -1;
	}	

	ret = curl_easy_setopt(conn, CURLOPT_URL, argv[1]);
	if (CURLE_OK !=	ret)
	{
		LOGERR("curl_easy_setopt faild");
		return -1;
	}

	ret = curl_easy_setopt(conn, CURLOPT_WRITEFUNCTION, read_callback);
	if (CURLE_OK != ret)
	{
		LOGERR("curl_easy_setopt failed");
		return -1;
	}
	ret = curl_easy_setopt(conn, CURLOPT_FOLLOWLOCATION, 1);
	if (CURLE_OK != ret)
	{
		LOGERR("curl_easy_setopt failed");
		return -1;
	}
	
	ret = curl_easy_perform(conn);
	if (CURLE_OK != ret)
	{
		LOGERR("curl_easy_perform failed");
		return -1;
	}

	curl_easy_cleanup(conn);
	return 0;
}
