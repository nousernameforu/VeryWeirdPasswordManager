#include "tableHandler.h"

//Singleton section

// Initialize static members
tableHandler* tableHandler::p_instance = nullptr;
table_SingletonDestroyer tableHandler::destroyer;

// Implementation of SingletonDestroyer destructor
table_SingletonDestroyer::~table_SingletonDestroyer() {
    delete p_instance;
}

// Implementation of SingletonDestroyer initialize method
void table_SingletonDestroyer::initialize(tableHandler* p) {
    p_instance = p;
}

// Implementation of FileHandeling getInstance method
tableHandler* tableHandler::getInstance() {
    if (!p_instance) {
        p_instance = new tableHandler();
        destroyer.initialize(p_instance);
    }
    return p_instance;
}


void tableHandler::createCsvTable(const QString& filename)
{
    QFile file(filename);

    // Attempt to open the file in write-only mode
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning("Could not open file for writing");
        return;
    }

    QTextStream out(&file);

    // Write the header row
    QStringList header;
    header << "URL" << "Login" << "Password";
    out << header.join(",") << "\n";

    // Write some sample data rows
    QStringList row1;
    row1 << "https://www.google.com" << "bob@gmail.com" << "bob";
    out << row1.join(",") << "\n";

    QStringList row2;
    row2 << "https://www.github.com" << "bob@gmail.com" << "bob2";
    out << row2.join(",") << "\n";

    QStringList row3;
    row3 << "https://www.reddit.com" << "other_bob" << "bob3";
    out << row3.join(",") << "\n";

    // Close the file
    file.close();
    qDebug("CSV file created successfully");
}


void tableHandler::displayCSVTable(const QString &filename, QTableWidget *tableWidget) {
    QFile file(filename);

    // Attempt to open the file in read-only mode
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Could not open file for reading");
        return;
    }

    QTextStream in(&file);
    QStringList lines;

    // Read all lines from the file
    while (!in.atEnd()) {
        lines << in.readLine();
    }
    file.close();

    if (lines.isEmpty()) {
        qWarning("CSV file is empty");
        return;
    }

    // Split the first line to get the headers
    QStringList headers = lines.at(0).split(",");
    tableWidget->setColumnCount(headers.size());
    tableWidget->setHorizontalHeaderLabels(headers);

    // Populate the table with the rest of the lines
    tableWidget->setRowCount(lines.size() - 1);

    for (int i = 1; i < lines.size(); ++i) {
        QStringList fields = lines.at(i).split(",");
        for (int j = 0; j < fields.size(); ++j) {
            QTableWidgetItem *item = new QTableWidgetItem(fields.at(j));
            tableWidget->setItem(i - 1, j, item);
        }
    }

    // Adjust the headers
     tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void tableHandler::commitTableToCSV(const QString &filename, QTableWidget *tableWidget) {
    QFile file(filename);

    // Attempt to open the file in write-only mode
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning("Could not open file for writing");
        return;
    }

    QTextStream out(&file);

    // Write the header row
    QStringList header;
    for (int col = 0; col < tableWidget->columnCount(); ++col) {
        header << tableWidget->horizontalHeaderItem(col)->text();
    }
    out << header.join(",") << "\n";

    // Write the data rows
    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        QStringList rowData;
        for (int col = 0; col < tableWidget->columnCount(); ++col) {
            QTableWidgetItem *item = tableWidget->item(row, col);
            rowData << (item ? item->text() : "");
        }
        out << rowData.join(",") << "\n";
    }

    // Close the file
    file.close();
    qDebug("CSV file updated successfully");
}

