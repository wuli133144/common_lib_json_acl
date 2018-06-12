/*
 * =====================================================================================
 * 
 *        Filename:  std_socket.h
 * 
 *     Description:  ����Socket�����Ļ���.
 * 
 *         Version:  1.0
 *         Created:  2006��05��30�� 15ʱ56��56�� CST
 *        Revision:  none
 *        Compiler:  g++
 * 
 *          Author:  ������ ()
 *         Company:  �����Ƽ�
 *           Email:  minghui.zhang@yeelion.com
 * 
 * =====================================================================================
 */
/** @addtogroup net
 *  
 *  
 *  @{
 */

#ifndef STD_SOCKET_H_ZMH_2006_05_30
#define STD_SOCKET_H_ZMH_2006_05_30

#include <iostream>
#include <sstream>
#include <string>

#include <sys/errno.h>
#include "SocketHeader.h"
#include "SocketException.h"
#include <poll.h>

using namespace std;

typedef enum  {
	SOCKET_BCONNECTTING,
	SOCKET_BCONNECTTED,
	SOCKET_BBOUND,
	SOCKET_BLISTEN,
	SOCKET_BCLOSED
} ESocketStat;
/*!
 *\brief   ����Socket�����Ļ���.
*! \class _Std_Socket Std_Socket.h net/Std_Socket.h   
 */
class _Std_Socket
{
public :
	//! Constructor.
	_Std_Socket();

	//! Destructor.
	virtual ~_Std_Socket();

	//! close this socket.
	void close() throw(SocketException);

	//void shutdown();

	/**
	 * @brief    Returns the local port to which this socket is bound.
	 *
	 * @return   the local port number to which this socket is bound 
	 *           or -1 if the socket is not bound yet.
	 */
	int getLocalPort() const;

	/**
	 * @brief    Returns the remote port to which this socket is connected. 
	 *           
	 * @return   the remote port number to which this socket is connected, 
	 *           or 0 if the socket is not connected yet.
	 */
	int getRemotePort() const;
	string getRemoteIP();

	const sockaddr_in *getLocalSockAddr() const;

	const sockaddr_in *getRemoteSockAddr() const;

	//! general interface.
	void setSockOpt(int level, int optname, const void *optval, socklen_t optlen) throw(SocketException);
	//! general interface.
	void getSockOpt(int level, int optname, void *optval, socklen_t *optlen) throw(SocketException);
	//! general interface.
	bool getNoDelay() throw(SocketException);
	//! general interface.
	void setNoDelay(bool on = true) throw(SocketException);
	//! general interface.
	bool getKeepAlive() throw(SocketException);
	//! general interface.
	void setKeepAlive(bool on) throw(SocketException);
	//! general interface.
	bool getReuseAddress() throw(SocketException);
	//! general interface.
	void setReuseAddress(bool on = true) throw(SocketException);
	//! general interface.
	int getReceiveBufferSize() throw(SocketException);
	//! general interface.
	void setReceiveBufferSize(int size) throw(SocketException);
	//! general interface.
	int getSendBufferSize() throw(SocketException);
	//! general interface.
	void  setSendBufferSize(int size) throw(SocketException);
	//! general interface.
	int getSoTimeout() throw(SocketException);
	//! general interface.
	void setSoTimeout(int timeout) throw(SocketException);

	//! get the string<ip:port>.
	string toString() const;

	//! ��ȡ����ʱʱ��,��getsockopt�е�SO_RCVTIMEO.
	int getReadTimeout() const;
	//! setReadTimeout
	void setReadTimeout(int timeo) {
		mReadTimeout = timeo;
		return;
	}

	int getWriteTimeout() const;
	//! setWriteTimeout
	void setWriteTimeout(int timeo) {
		mWriteTimeout = timeo;
		return;
	}

	//! read and write interface.
	/**
	 * @brief    ��socket�ж�ȡһ�����ֽ�.
	 *
	 * @param    buf    ������.
	 * @param    count  ����ȡ���ֽ���.
	 * @return   ʵ�ʶ�ȡ���ֽ���.
	 */
	ssize_t read(char *buf, ssize_t count);
	/**
	 * @brief    ��socket�ж�ȡһ�����ֽ�.
	 *
	 * @param    buf    ������.
	 * @param    count  ����ȡ���ֽ���.
	 * @return   ʵ�ʶ�ȡ���ֽ���.
	 */
	ssize_t read(char *buf, ssize_t count, int to) throw(SocketException);

	/**
	 * @brief    ֱ����socket�ж�ȡһ�����ֽ����ŷ���.
	 *
	 * @param    buf    ������.
	 * @param    count  ����ȡ���ֽ���.
	 * @return   ����count.
	 */
	ssize_t safeRead(char *buf, ssize_t count,bool isPoll=false) throw(SocketException);
       ssize_t safeReadHttpRequest(char *buf,ssize_t count,bool isPoll) throw(SocketException);
       
	/**
	 * @brief    ��socket��д��һ�����ֽ���.
	 *
	 * @param    buf    ������.
	 * @param    count  ��д����ֽ���.
	 * @return   ʵ��д����ֽ���.
	 */
	ssize_t write(char *buf, ssize_t count);

	/**
	 * @brief    ֱ��д��socket��һ�����ֽ����ŷ���.
	 *\exception    д����̳�����׳��쳣.
	 * @param    buf    ������.
	 * @param    count  ��д����ֽ���.
	 * @return   ����count.
	 * 
	 */
	ssize_t safeWrite(char *buf, ssize_t count,bool isPoll=false) throw(SocketException);

	bool isConnect() const;

	//! ͨ��select�жϵ�ǰ�����Ƿ����.
	bool good() const;

	bool tcp_establish() const;

	int getSocketFd() const {
		return mSocket;
	}
public :
	//! stat.
	bool		mBeInitialized;		//!< has get a socket.
	bool		mBeConnected;
	bool		mBeBound;
	bool		mBeClosed;

	int		mReadTimeout;
	int		mWriteTimeout;	

	//! data.
	int		mSocket;
	sockaddr_in	mLocalSockAddr;
	sockaddr_in	mRemoteSockAddr;
private :
	int _select_fd(int sockfd, double timeout, bool write);
	int _poll_fd(int sockfd, double timeout, bool writep);
	
};
#endif //STD_SOCKET_H_ZMH_2006_05_30
/** @} */ // end of 
