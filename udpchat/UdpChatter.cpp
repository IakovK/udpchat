#include "UdpChatter.h"

UdpChatter::UdpChatter(int localPort, int remotePort)
	:m_localPort(localPort)
	,m_remotePort(remotePort)
{
	m_socket = new QUdpSocket(this);
	m_socket->bind(QHostAddress::LocalHost, localPort);
	connect(m_socket, &QUdpSocket::readyRead,
		this, &UdpChatter::readIncomingData);
}

void UdpChatter::readIncomingData()
{
	// when data comes in
	QByteArray buffer;
	buffer.resize(m_socket->pendingDatagramSize());

	QHostAddress sender;
	quint16 senderPort;

	m_socket->readDatagram(buffer.data(), buffer.size(),
		&sender, &senderPort);

	emit MessageReceived(senderPort, QString(buffer));
}

void UdpChatter::sendText(QString msg)
{
	QByteArray Data;
	Data.append(msg);
	m_socket->writeDatagram(Data, QHostAddress::LocalHost, m_remotePort);
}
