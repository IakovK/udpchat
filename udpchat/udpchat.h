#pragma once

#include <QtWidgets/QMainWindow>
#include <QThread>
#include <QPointer>

#include "ui_udpchat.h"

#include "UdpChatter.h"

class udpchat : public QMainWindow
{
    Q_OBJECT

public:
    udpchat(QWidget *parent = Q_NULLPTR);

private:
    Ui::udpchatClass ui;
	QPointer<UdpChatter> m_chatter;
	QPointer<QThread> m_workerThread;

	void CreateChatter();
	void DisplayMessage(QString prefix, QString message);

public slots:
	void handleConnect();
	void handleDisconnect();
	void handleSend();
	void enableButtons();
	void handleDisconnect2();
	void MessageReceived(quint16 from, QString msg);

signals:
	void sendText(QString msg);
};
