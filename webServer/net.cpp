#include "net.h"
#include <string.h>
#include <unistd.h>

NetServer::NetServer()
{
    m_sockfd = 0;
    m_clientfd = 0;
    m_socketLen = 0;
}
NetServer::~NetServer()
{
    if(m_sockfd > 0)
        close(m_sockfd);
}
bool NetServer::Init()
{
    m_sockfd = socket(AF_INET,SOCK_STREAM,0);
    m_socketLen = sizeof(m_server_addr);
    memset(&m_server_addr, 0 , m_socketLen);
    m_server_addr.sin_family = AF_INET;
    m_server_addr.sin_addr.s_addr=INADDR_ANY;
    m_server_addr.sin_port=htons(80);
    int bReuseaddr=1;
    setsockopt(m_sockfd,SOL_SOCKET ,SO_REUSEADDR,(const char*)&bReuseaddr,sizeof(int));
    bind(m_sockfd,(struct sockaddr *)&m_server_addr,m_socketLen);
    listen(m_sockfd,5);
    return true;
}
void sendHtml(int sockfd)
{
    char buf[LINE] = {};
    FILE *fp = NULL;
    fp = fopen("mp4.html","r");
    while(fgets(buf, LINE, fp))
    {
        if(send(sockfd, buf, strlen(buf), MSG_NOSIGNAL) < 0)
            break;
    }
    CLog::LOG()<<"Send info success"<<endl;
    fclose(fp);
}
void sendpic(int sockfd)
{
    char buf[MAXLINE] = {};
    memset(buf,0, MAXLINE);
    sprintf(buf, "HTTP/1.1 200 OK\r\n");
    sprintf(buf,"%sContent-Type: image/jpeg\r\n",buf);
    sprintf(buf,"%sConnection: close\r\n\r\n",buf);
    send(sockfd,buf,strlen(buf),0);

    FILE *fp = NULL;
    fp = fopen("id.jpg","rb");
    int i = 0;
    while((i = fread(buf, 1, LINE-1, fp)) > 0) {
        if(send(sockfd, buf, i, MSG_NOSIGNAL) < 0)
            break;
    }
    fclose(fp);
}

void sendmp3(int sockfd)
{
    char buf[MAXLINE] = {};
    memset(buf,0, MAXLINE);
    sprintf(buf, "HTTP/1.1 200 OK\r\n");
    sprintf(buf,"%sContent-Type: audio/mp3\r\n",buf);
    sprintf(buf,"%sConnection: close\r\n\r\n",buf);
    send(sockfd,buf,strlen(buf),0);

    FILE *fp = NULL;
    fp = fopen("waiting.mp3","rb");
    int i = 0;
    while((i = fread(buf, 1, LINE-1, fp)) > 0) {
        if(send(sockfd, buf, i, MSG_NOSIGNAL) < 0)
            break;
    }
    fclose(fp);
}
void sendmp4(int sockfd)
{
    char buf[MAXLINE] = {};
    memset(buf,0, MAXLINE);
    sprintf(buf, "HTTP/1.1 200 OK\r\n");
    sprintf(buf,"%sContent-Type: video/mpeg4\r\n",buf);
    sprintf(buf,"%sConnection: close\r\n\r\n",buf);
    send(sockfd,buf,strlen(buf),0);

    FILE *fp = NULL;
    fp = fopen("test.mp4","rb");
    int i = 0;
    while((i = fread(buf, 1, MAXLINE-1, fp)) > 0) {
        if(send(sockfd, buf, i, MSG_NOSIGNAL) < 0)
            break;
    }
    fclose(fp);
}
void HttpHandler(int sockfd, const char *buff)
{
    if(strstr(buff, "favicon.ico"))
        return;
    std::cout<<buff<<std::endl;
    std::cout<<"finish"<<std::endl;
    if(strstr(buff,".jpg"))
    {
        return sendpic(sockfd);
    }
    if(strstr(buff,".mp3"))
    {
        return sendmp3(sockfd);
    }
    if(strstr(buff,".mp4"))
    {
        return sendmp4(sockfd);
    }
    char buf[MAXLINE] = {};
    CLog::LOG()<<buff<<endl;
    memset(buf,0, MAXLINE);
    sprintf(buf, "HTTP/1.1 200 OK\r\n");
    sprintf(buf,"%sContent-Type: text/html\r\n",buf);
    sprintf(buf,"%sConnection: close\r\n\r\n",buf);
    send(sockfd,buf,strlen(buf),0);
    CLog::LOG()<<buf<<endl;
    sendHtml(sockfd);
}
bool NetServer::Start()
{
    Init();
    while(1) {
        char buf[MAXLINE] = {};
        //char tmp[MAXLINE] = {};
        int ret = 0;
        m_clientfd = accept(m_sockfd,(struct sockaddr *)&m_client_addr, &m_socketLen);
        ret = recv(m_clientfd, buf, MAXLINE, 0);
        if(ret <= 0)
            continue;
        HttpHandler(m_clientfd, buf);
        close(m_clientfd);
    }
    return true;
}
