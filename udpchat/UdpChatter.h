#pragma once

#include <QUdpSocket>

class UdpChatter : public QObject
{
	Q_OBJECT

	int m_localPort;
	int m_remotePort;

	QUdpSocket *m_socket;

public:
	UdpChatter(int localPort, int remotePort);

public slots:
	void readIncomingData();
	void sendText(QString msg);
signals:
	void MessageReceived(quint16 from, QString msg);
};

