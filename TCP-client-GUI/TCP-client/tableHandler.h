#ifndef TABLEHANDLER_H
#define TABLEHANDLER_H

#include <QFile>
#include <QTextStream>
#include <QTableView>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>

class tableHandler;

class table_SingletonDestroyer {
private:
    tableHandler* p_instance;

public:
    ~table_SingletonDestroyer();
    void initialize(tableHandler* p);
};

class tableHandler {
private:
    static tableHandler* p_instance;
    static table_SingletonDestroyer destroyer;

public:
    static tableHandler* getInstance();
    void createCsvTable(const QString& filename);
    void displayCSVTable(const QString &filename, QTableWidget *tableWidget);
    void commitTableToCSV(const QString &filename, QTableWidget *tableWidget);
};

#endif // TABLEHANDLER_H
