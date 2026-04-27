#include <QCoreApplication>
#include <QLocalServer>
#include <QLocalSocket>
#include <QTextStream>
#include <QDebug>
#include <iostream>

/**
 * SERVER FUNCTION
 * Listens for incoming "pipe" connections and prints received data.
 */
void runServer(const QString &pipeName) {
    std::cout << "";
    QLocalServer *server = new QLocalServer(QCoreApplication::instance());

    // On Linux/Arch, if the app crashed previously, the socket file might still exist.
    // We clear it out to ensure we can listen.
    QLocalServer::removeServer(pipeName);

    if (!server->listen(pipeName)) {
        qCritical() << "Server could not start:" << server->errorString();
        return;
    }

    qDebug() << "Server listening on:" << pipeName;

    QObject::connect(server, &QLocalServer::newConnection, [server]() {
        QLocalSocket *clientSocket = server->nextPendingConnection();
        
        QObject::connect(clientSocket, &QLocalSocket::readyRead, [clientSocket]() {
            QByteArray data = clientSocket->readAll();
            qDebug() << "Server Received:" << data;
        });

        // Cleanup when client disconnects
        QObject::connect(clientSocket, &QLocalSocket::disconnected, 
                         clientSocket, &QLocalSocket::deleteLater);
    });
}

/**
 * CLIENT FUNCTION
 * Connects to the server and sends a message.
 */
void runClient(const QString &pipeName, const QString &message) {
    QLocalSocket *socket = new QLocalSocket(QCoreApplication::instance());

    socket->connectToServer(pipeName);

    if (socket->waitForConnected(3000)) {
        qDebug() << "Client connected. Sending message...";
        socket->write(message.toUtf8());
        socket->flush(); // Ensure data leaves the buffer
        socket->disconnectFromServer();
    } else {
        qCritical() << "Client Error:" << socket->errorString();
    }
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    QString pipeName = "qt_ipc_pipe";

    // Simple CLI logic to test both roles
    QStringList args = a.arguments();
    
    if (args.contains("--server")) {
        runServer(pipeName);
    } else if (args.contains("--client")) {
        QString msg = args.size() > 2 ? args[2] : "Hello from the pipe!";
        runClient(pipeName, msg);
        // Exit client after sending
        return 0; 
    } else {
        qDebug() << "Usage:";
        qDebug() << "  Server mode: ./QtIpcExample --server";
        qDebug() << "  Client mode: ./QtIpcExample --client \"your message\"";
        return 1;
    }

    return a.exec();
}
