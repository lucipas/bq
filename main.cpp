#include <QApplication>
#include <QWebEngineView>
#include <QWebEngineSettings>
#include <QCommandLineParser>
#include <QUrl>

int main(int argc, char *argv[]) {
    qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--disable-gpu --no-sandbox");

    QApplication app(argc, argv);
    app.setApplicationName("MyArchBrowser");

    QCommandLineParser parser;
    parser.setApplicationDescription("Lightweight Custom Browser");
    parser.addHelpOption();
    parser.addPositionalArgument("url", "The URL to open.");
    parser.process(app);

    const QStringList args = parser.positionalArguments();
    QString target = args.isEmpty() ? "https://archlinux.org" : args.first();

    QWebEngineView view;
    
    view.settings()->setAttribute(QWebEngineSettings::AutoLoadImages, true);
    view.settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
    view.settings()->setAttribute(QWebEngineSettings::PlaybackRequiresUserGesture, true); // No auto-playing videos
    view.settings()->setAttribute(QWebEngineSettings::HyperlinkAuditingEnabled, false);   // Disable "ping" tracking
    view.settings()->setAttribute(QWebEngineSettings::ErrorPageEnabled, true);            // Show a simple error page instead of hanging

    // SECURITY
    view.settings()->setAttribute(QWebEngineSettings::AllowRunningInsecureContent, false); // Block HTTP content on HTTPS sites
    view.settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, false); // Sandbox local files
                                                                                 // /:
    view.setUrl(QUrl::fromUserInput(target));
    view.resize(1024, 768);
    view.show();

    return app.exec();
}
