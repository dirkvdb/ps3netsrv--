//    Copyright (C) 2012 Dirk Vanden Boer <dirk.vdb@gmail.com>
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#ifndef UTILS_SOCKET_H
#define UTILS_SOCKET_H

#include <string>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>

namespace utils
{

class Socket
{
public:
	Socket()
	: m_Socket(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))
	{
		if (m_Socket < 0)
		{
			throw std::logic_error("Failed to create socket");	
		}

		memset(&m_Address.sin_zero, 0, sizeof(m_Address.sin_zero));
	}

	Socket(const Socket&) = delete;
	
	Socket(Socket&& socket)
	: m_Socket(std::move(socket.m_Socket))
	{
		socket.m_Socket = -1;
		memcpy(&m_Address, &socket.m_Address, sizeof(sockaddr_in));
		memset(&socket.m_Address, 0, sizeof(sockaddr_in));

#if defined __APPLE__ || defined __FreeBSD__
        setSocketOption(SOL_SOCKET, SO_NOSIGPIPE, 1);
#endif
	}

	~Socket()
	{
		close();
	}

	Socket& operator=(const Socket&) = delete;

	operator bool() const
	{
		return m_Socket >= 0;
	}

	std::string getAddress() const
	{
		return std::string(inet_ntoa(m_Address.sin_addr));
	}

    void setNoDelayOption()
	{
        setSocketOption(IPPROTO_TCP, TCP_NODELAY, 1);
	}
    
	void setReuseAddressOption()
	{
        setSocketOption(SOL_SOCKET, SO_REUSEADDR, 1);
	}

	void startListening(uint32_t port)
	{
		sockaddr* sock{reinterpret_cast<sockaddr*>(&m_Address)};

		sock->sa_family = AF_INET;
		m_Address.sin_port = htons(static_cast<uint16_t>(port));
		m_Address.sin_addr.s_addr = htonl(INADDR_ANY);
		if (bind(m_Socket, reinterpret_cast<sockaddr*>(&m_Address), sizeof(sockaddr)) < 0)
		{
			std::stringstream ss;
			ss << "Failed to bind socket on port: " << port;
			throw std::logic_error(ss.str().c_str());
		}

		if (listen(m_Socket, 1) < 0)
		{
			throw std::logic_error("Failed to listen on socket");
		}
	}

	Socket accept()
	{
		Socket clientSocket;
		socklen_t size = sizeof(m_Address);
		clientSocket.m_Socket = ::accept(m_Socket, reinterpret_cast<sockaddr*>(&clientSocket.m_Address), &size);

		if (!clientSocket)
		{
			throw std::logic_error("Failed to accept incoming connection");
		}

		return clientSocket;
	}

	void close()
	{
		shutdown(m_Socket, SHUT_RDWR);
		::close(m_Socket);
	}

	void write(const void* data, size_t bytes)
	{
		if (send(m_Socket, data, bytes, 0) < bytes)
		{
			throw std::logic_error("Failed to write data to socket");		
		}
	}

	size_t read(void* data, size_t bytes)
	{
		return recv(m_Socket, data, bytes, MSG_WAITALL);
	}

	std::string readString(size_t characters)
	{
		std::vector<char> buf(characters + 1, '\0');
		
		auto received = recv(m_Socket, buf.data(), characters, MSG_WAITALL);
		if (received < characters)
		{
			throw std::logic_error("Failed to read string from socket");	
		}

		return &buf.front();
	}

	std::vector<uint8_t> read(size_t bytes)
	{
		std::vector<uint8_t> data(bytes);
		auto received = recv(m_Socket, data.data(), bytes, MSG_WAITALL);
		if (received < bytes)
		{
			data.resize(received);
		}

		return data;
	}

private:
    void setSocketOption(int32_t level, int32_t option, uint32_t value)
    {
        socklen_t size = sizeof(value);
        if (setsockopt(m_Socket, level, option, &value, size) < 0)
        {
            throw std::logic_error("Failed to set socket option");
        }
    }
    
	int32_t			m_Socket;
	sockaddr_in		m_Address;
};

}

#endif
