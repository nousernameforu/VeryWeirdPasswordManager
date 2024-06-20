#ifndef ConnectionPopup_H
#define ConnectionPopup_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QIntValidator>

class ConnectionPopup : public QDialog {
    Q_OBJECT

public:
    explicit ConnectionPopup(QWidget *parent = nullptr);

signals:
    void connectToServer(const QString& ip, int port);

private slots:
    void connectClicked();

private:
    QLineEdit *ipEdit;
    QLineEdit *portEdit;
    QPushButton *connectButton;
    QPushButton *cancelButton;
};

#endif // ConnectionPopup_H
