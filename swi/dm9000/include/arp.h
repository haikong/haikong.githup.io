#ifndef _ARP_H__
#define _ARP_H__

//以太网头部结构
typedef struct _eth_hdr {
	unsigned char 	d_mac[6]; 	//目的地址
	unsigned char 	s_mac[6]; 	//源地址
	unsigned short 	type; 		//协议类型
}t_ETH_HDR;

//ARP 首部结构
typedef struct _arp{
	 unsigned short hwtype; 	//硬件类型(1 表示传输的是以太网 MAC 地址)
	 unsigned short protocol; 	//协议类型(0x0800 表示传输的是 IP 地址)
	 unsigned char  hwlen; 		//硬件地址长度(6)
	 unsigned char  protolen; 	//协议地址长度(4)
	 unsigned short opcode; 	//操作(1 表示 ARP 请求,2 表示 ARP 应答)
	 unsigned char  smac[6]; 	//发送端 MAC 地址
	 unsigned char  sipaddr[4]; //发送端 IP 地址
	 unsigned char  dmac[6]; 	//目的端 MAC 地址
	 unsigned char  dipaddr[4]; //目的端 IP 地址
}t_ARP;

//IP 首部结构
typedef struct _ip {
	 unsigned char  vhl; //4 			//4位版本号 4 位首部长度(0x45)
	 unsigned char  tos; 			//服务类型(0)
	 unsigned short len; 			//整个 IP 数据报总字节长度
	 unsigned short ipid; 			//IP 标识
	 unsigned short ipoffset; //3 		//3位标识 13 位偏移
	 unsigned char  ttl; 			//生存时间(32 或 64)
	 unsigned char  proto; 			//协议(1 表示 ICMP,2 表示 IGMP,6 表示 TCP,17 表示UDP)
	 unsigned short ipchksum; 		//首部校验和
	 unsigned char  srcipaddr[4]; 	//源 IP
	 unsigned short destipaddr[4]; 	//目的 IP
}t_IP;
//TCP 首部结构
typedef struct _tcp {
	 unsigned short	sport; 			//源端口号
	 unsigned short dport; 			//目的端口号
	 unsigned int 	sequencenum; 	//顺序号
	 unsigned int 	acknowledgenum; //确认号
	 unsigned short tcplength; 		//低四位为 TCP 报头字（32 位）的个数，高四位必须为 0，是保留位
	 unsigned short flags; 			//低两位为保留位，高 6 位为标志位[2:7]依次为
									//URG:紧急指针。用到的时候值为 1，用来处理避免TCP 数据流中断
									//ACK:置 1 时表示确认号为合法，为 0 的时候表示数据段不包含确认信息，确认号被忽略。
									//PSH:置 1 时请求的数据段在接收方得到后就可直接送到应用程序，而不必等到缓冲区满时才传送。
									//RST:用于复位因某种原因引起出现的错误连接，也用来拒绝非法数据和请求。
									//SYN:在连接请求中，SYN=1，ACK=0，连接响应时，SYN=1，ACK=1。
									//FIN:用来释放连接，表明发送方已经没有数据发送了。
	 unsigned short window; 		//指定关于发送端能传输的下一段的大小的指令，表示想收到的每个 TCP 数据段的大小。
	 unsigned short tcpchksum; 		//TCP 校验和
	 unsigned short urgentpoint; 	//紧急指针 16 位，紧急指针指出在本报文段中的紧急数据的位置，在 URG 标志设置了时才有效。
}t_TCP;
 
//UDP 首部结构
typedef struct _udp{
	 unsigned short sport; 		//源端口号 （34 35）
	 unsigned short dport; 		//目的端口号 （36 37）
	 unsigned short length; 	//UDP 数据包报总长度 （38 39）
	 unsigned short udpchksum; 	//UDP 校验和(可选项) （40 41）
}t_UDP;

#define htons(x) ({\
	typeof(x) _x = x;	\
	_x = (((unsigned short)x >> 8) & 0xff) | ((x & 0xff) << 8);\
	x = _x;\
})

#define htonl(x) ({\
	typeof(x) _x = x;	\
	_x = ((unsigned int)x & 0xff) << 24;\
	_x |= (((unsigned int)x >> 8) & 0xff) << 16;\
	_x |= (((unsigned int)x >> 16) & 0xff) << 8;\
	_x |= ((unsigned int)x >> 24) & 0xff;\
	x = _x;\
})
#endif
