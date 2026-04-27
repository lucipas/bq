#include <QApplication>
#include <QMainWindow>
#include <QWebEngineView>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QProgressBar>
#include <QLocalServer>
#include <QLocalSocket>
#include <QUrl>
#include <iostream>

class SimpleBrowser : public QMainWindow {
    Q_OBJECT;

public:
    SimpleBrowser() {
        // --- 1. UI LAYOUT SETUP ---
        QWidget *centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);

        QVBoxLayout *layout = new QVBoxLayout(centralWidget);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);

        // URL Bar with basic styling
        urlBar = new QLineEdit(this);
        urlBar->setPlaceholderText("Enter URL and press Enter...");
        urlBar->setStyleSheet("QLineEdit { padding: 8px; border: none; background: #000000; }");
        layout->addWidget(urlBar);

        // Slim Progress Bar (YouTube style)
        progressBar = new QProgressBar(this);
        progressBar->setMaximumHeight(2);
        progressBar->setTextVisible(false);
        progressBar->setStyleSheet("QProgressBar { border: 0px; background: transparent; } "
                                   "QProgressBar::chunk { background: #2ecc71; }");
        layout->addWidget(progressBar);

        // The Web Engine
        view = new QWebEngineView(this);
        layout->addWidget(view);

        // --- 2. IPC SERVER SETUP ---
        QString serverName = "browser_remote";

        std::cout << "IPC name: browser_remote \n";
        QLocalServer::removeServer(serverName); // Clear stale socket files on Arch
        ipcServer = new QLocalServer(this);
        if (ipcServer->listen(serverName)) {
            connect(ipcServer, &QLocalServer::newConnection, this, &SimpleBrowser::handleIpc);
        }

        // --- 3. SIGNAL CONNECTIONS ---
        connect(urlBar, &QLineEdit::returnPressed, this, &SimpleBrowser::navigateToUrl);
        
        connect(view, &QWebEngineView::urlChanged, [this](const QUrl &url) {
            urlBar->setText(url.toString());
        });

        connect(view, &QWebEngineView::loadProgress, progressBar, &QProgressBar::setValue);
        
        connect(view, &QWebEngineView::loadFinished, [this](bool ok) {
            progressBar->setValue(ok ? 0 : 100);
        });

        // Load Homepage
        view->load(QUrl("https://www.archlinux.org"));
        resize(1024, 768);
    }

private slots:
    void navigateToUrl() {
        QString input = urlBar->text();
        if (!input.startsWith("http")) {
            input.prepend("https://");
        }
        view->load(QUrl(input));
    }

    void handleIpc() {
        QLocalSocket *client = ipcServer->nextPendingConnection();
        connect(client, &QLocalSocket::readyRead, [this, client]() {
            QString remoteUrl = QString::fromUtf8(client->readAll()).trimmed();
            if (!remoteUrl.isEmpty()) {
                urlBar->setText(remoteUrl);
                navigateToUrl();
            client->write("SUCCESS: Navigating...");
            } else {
                client -> write("Something happened.");
            }
            client->disconnectFromServer();
        });
    }

private:
    QWebEngineView *view;
    QLineEdit *urlBar;
    QProgressBar *progressBar;
    QLocalServer *ipcServer;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("QtBrowser_IPC");

    SimpleBrowser browser;
    browser.show();

    return app.exec();
}

#include "main.moc"
