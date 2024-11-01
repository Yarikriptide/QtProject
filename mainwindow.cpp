#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QElapsedTimer>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupConnections();

    // Установка начальной даты в DateEdit
    ui->birthDateEdit->setDate(QDate::currentDate());

    // Установка валидатора для поля ID
    QIntValidator *validator = new QIntValidator(1, 999999, this);
    ui->idLineEdit->setValidator(validator);
    ui->searchIdLineEdit->setValidator(validator);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupConnections()
{
    // Подключение сигналов к слотам
    connect(ui->addPatientButton, &QPushButton::clicked,
            this, &MainWindow::addPatientClicked);
    connect(ui->searchButton, &QPushButton::clicked,
            this, &MainWindow::searchButtonClicked);
    connect(ui->compareSearchButton, &QPushButton::clicked,
            this, &MainWindow::compareSearchClicked);
}

void MainWindow::addPatientClicked()
{
    // Проверка ID
    bool ok;
    int id = ui->idLineEdit->text().toInt(&ok);
    if (!ok || id <= 0) {
        QMessageBox::warning(this, "Ошибка", "Введите корректный ID (положительное число)");
        return;
    }

    // Проверка имени
    QString name = ui->nameLineEdit->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите имя пациента");
        return;
    }

    // Проверка даты
    QDate birthDate = ui->birthDateEdit->date();
    if (birthDate > QDate::currentDate()) {
        QMessageBox::warning(this, "Ошибка", "Дата рождения не может быть в будущем");
        return;
    }

    // Проверка на существующий ID
    Patient searchPatient;
    searchPatient.setId(id);
    if (bst.search(searchPatient)) {
        QMessageBox::warning(this, "Ошибка", "Пациент с таким ID уже существует");
        return;
    }

    // Создание и добавление нового пациента
    Patient newPatient(id, name, birthDate);
    bst.insert(newPatient);
    patientArray.append(newPatient);

    // Сортировка массива для корректного сравнения
    std::sort(patientArray.begin(), patientArray.end(),
              [](const Patient& a, const Patient& b) { return a.getId() < b.getId(); });

    // Обновление интерфейса
    updatePatientList();

    // Очистка полей ввода
    ui->idLineEdit->clear();
    ui->nameLineEdit->clear();
    ui->birthDateEdit->setDate(QDate::currentDate());

    QMessageBox::information(this, "Успех", "Пациент успешно добавлен");
}

void MainWindow::searchButtonClicked()
{
    if (patientArray.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "База данных пуста");
        return;
    }

    bool ok;
    int searchId = ui->searchIdLineEdit->text().toInt(&ok);
    if (!ok || searchId <= 0) {
        QMessageBox::warning(this, "Ошибка", "Введите корректный ID для поиска");
        return;
    }

    Patient searchPatient;
    searchPatient.setId(searchId);

    // Поиск в бинарном дереве
    QElapsedTimer timer;
    timer.start();
    bool found = bst.search(searchPatient);
    qint64 searchTime = timer.nsecsElapsed();

    QString message = found ?
                          QString("Пациент найден! Время поиска: %1 нс").arg(searchTime) :
                          QString("Пациент не найден. Время поиска: %1 нс").arg(searchTime);

    QMessageBox::information(this, "Результат поиска", message);
}

void MainWindow::compareSearchClicked()
{
    if (patientArray.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "База данных пуста");
        return;
    }

    bool ok;
    int searchId = ui->searchIdLineEdit->text().toInt(&ok);
    if (!ok || searchId <= 0) {
        QMessageBox::warning(this, "Ошибка", "Введите корректный ID для поиска");
        return;
    }

    compareSearchAlgorithms(searchId);
}

int MainWindow::linearSearch(const QVector<Patient>& arr, int id) const
{
    for (int i = 0; i < arr.size(); ++i) {
        if (arr[i].getId() == id) {
            return i;
        }
    }
    return -1;
}

void MainWindow::updatePatientList()
{
    ui->patientListWidget->clear();
    for (const Patient& patient : patientArray) {
        QString item = QString("ID: %1 | %2 | %3")
        .arg(patient.getId(), 6, 10, QChar('0'))
            .arg(patient.getName(), -20)
            .arg(patient.getBirthDate().toString("dd.MM.yyyy"));
        ui->patientListWidget->addItem(item);
    }

    // Обновление статистики - исправленная версия
    statusBar()->showMessage(
        QString("Всего пациентов: %1").arg(patientArray.size())
        );
}

void MainWindow::compareSearchAlgorithms(int searchId)
{
    const int ITERATIONS = 1000; // Количество итераций для усреднения

    QElapsedTimer timer;
    qint64 totalLinearTime = 0;
    qint64 totalBstTime = 0;
    int linearResult = -1;
    bool bstResult = false;

    // Многократное выполнение линейного поиска
    for (int i = 0; i < ITERATIONS; ++i) {
        timer.start();
        linearResult = linearSearch(patientArray, searchId);
        totalLinearTime += timer.nsecsElapsed();
    }

    // Многократное выполнение поиска в BST
    Patient searchPatient;
    searchPatient.setId(searchId);
    for (int i = 0; i < ITERATIONS; ++i) {
        timer.start();
        bstResult = bst.search(searchPatient);
        totalBstTime += timer.nsecsElapsed();
    }

    // Вычисление среднего времени
    qint64 avgLinearTime = totalLinearTime / ITERATIONS;
    qint64 avgBstTime = totalBstTime / ITERATIONS;

    // Форматирование результатов
    QString result = QString(
                         "Результаты поиска ID %1 (%2 итераций):\n\n"
                         "Линейный поиск:\n"
                         "- Среднее время: %3 нс\n"
                         "- Сложность: O(n)\n\n"
                         "Бинарное дерево:\n"
                         "- Среднее время: %4 нс\n"
                         "- Сложность: O(log n)\n\n"
                         "Результат: пациент %5\n\n"
                         "Размер базы данных: %6 записей")
                         .arg(searchId)
                         .arg(ITERATIONS)
                         .arg(avgLinearTime)
                         .arg(avgBstTime)
                         .arg(linearResult >= 0 || bstResult ? "найден" : "не найден")
                         .arg(patientArray.size());

    ui->resultsTextEdit->setText(result);
}
