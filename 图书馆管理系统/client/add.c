

void add_operator(TcpSocket* ts)
{
    char buf[4096];
    sprintf(buf,"add_operator");
    // 发送字符串 
    send_str_tcp(ts,buf);
}