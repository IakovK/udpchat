#include "udpchat.h"
#include <QIntValidator>

udpchat::udpchat(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	ui.localPort->setValidator(new QIntValidator(0, 10000, this));
	ui.remotePort->setValidator(new QIntValidator(0, 10000, this));
	connect(ui.btnConnect, &QPushButton::clicked, this, &udpchat::handleConnect);
	connect(ui.btnDisconnect, &QPushButton::clicked, this, &udpchat::handleDisconnect);
	connect(ui.btnSend, &QPushButton::clicked, this, &udpchat::handleSend);
	connect(ui.localPort, &QLineEdit::textChanged, this, &udpchat::enableButtons);
	connect(ui.remotePort, &QLineEdit::textChanged, this, &udpchat::enableButtons);
	enableButtons();
}

void udpchat::handleConnect()
{
	CreateChatter();
	enableButtons();
}

void udpchat::CreateChatter()
{
	int localPort = ui.localPort->text().toInt();
	int remotePort = ui.remotePort->text().toInt();

	m_chatter = new UdpChatter(localPort, remotePort);
	m_workerThread = new QThread();
	m_chatter->moveToThread(m_workerThread);

	connect(m_workerThread, &QThread::finished, m_chatter, &QObject::deleteLater);
	connect(m_workerThread, &QThread::finished, this, &udpchat::handleDisconnect2);
	connect(this, &udpchat::sendText, m_chatter, &UdpChatter::sendText);
	connect(m_chatter, &UdpChatter::MessageReceived, this, &udpchat::MessageReceived);
	m_workerThread->start();
}

void udpchat::handleDisconnect()
{
	m_workerThread->quit();
}

void udpchat::handleDisconnect2()
{
	m_chatter.clear();
	m_workerThread.clear();
	enableButtons();
}

void udpchat::handleSend()
{
	auto s = ui.msgText->text();
	DisplayMessage("you", s);
	emit sendText(s);
}

void udpchat::DisplayMessage(QString prefix, QString message)
{
	ui.chatArea->append(QString("%1: %2").arg(prefix).arg(message));
}

void udpchat::MessageReceived(quint16 from, QString msg)
{
	DisplayMessage(QString("from %1").arg(from), msg);
}

void udpchat::enableButtons()
{
	if (!ui.localPort->text().isEmpty() &&
		!ui.remotePort->text().isEmpty() &&
		m_chatter == nullptr)
	{
		ui.btnConnect->setEnabled(true);
		ui.btnDisconnect->setEnabled(false);
		ui.btnSend->setEnabled(false);
	}
	else if (m_chatter != nullptr)
	{
		ui.btnConnect->setEnabled(false);
		ui.btnDisconnect->setEnabled(true);
		ui.btnSend->setEnabled(true);
	}
	else
	{
		ui.btnConnect->setEnabled(false);
		ui.btnDisconnect->setEnabled(false);
		ui.btnSend->setEnabled(false);
	}
}

