#include <QtCore>

class InstallCacic : public QObject {
    Q_OBJECT

public:
    InstallCacic(QObject *parent = 0): QObject(parent) {}

public slots:
    void run() {
        // Processa e finaliza
        emit finished();
    }

signals:
    void finished();
};
