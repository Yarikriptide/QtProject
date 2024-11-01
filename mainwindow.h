#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "patient.h"
#include "BinarySearchTree.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addPatientClicked();
    void searchButtonClicked();
    void compareSearchClicked();

private:
    Ui::MainWindow *ui;
    BinarySearchTree<Patient> bst; // Бинарное дерево поиска
    void setupConnections(); // Новый метод для установки соединений
    QVector<Patient> patientArray; // Массив для линейного поиска

    // Вспомогательные методы
    int linearSearch(const QVector<Patient>& arr, int id) const;
    void updatePatientList();
    void compareSearchAlgorithms(int searchId);
};

#endif // MAINWINDOW_H
