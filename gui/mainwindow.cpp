#include <QAction>
#include <QCheckBox>
#include <QCollator>
#include <QComboBox>
#include <QHeaderView>
#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <QLocale>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QStatusBar>
#include <QStringList>
#include <QTableWidget>

#include <algorithm>
#include <limits>
#include <new>
#include <sstream>
#include <vector>

#include "mainwindow.h"
#include "ui_mainwindow.h"

/* Форматирует номера страниц для таблицы и результата поиска */
static QString formatPageNumbers(const DynamicArray<int>& numbers)
{
    QStringList formatted;
    for(int i = 0; i < numbers.GetLength(); ++i) {
        formatted.append(QString::number(numbers[i]));
    }
    return formatted.join(QStringLiteral(", "));
}

/* Создаёт элементы окна, настраивает ввод и подключает обработчики */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->bucketCountInput->setValidator(new QIntValidator(1, std::numeric_limits<int>::max(), ui->bucketCountInput));
    connect(ui->splitButton, &QPushButton::clicked, this, &MainWindow::onSplitButtonClicked);
    connect(ui->actionEnterText, &QAction::triggered, this, [this]() {
        ui->inputText->setFocus();
        statusBar()->showMessage(QStringLiteral("Введите исходный текст."), 3000);
    });
    connect(ui->actionSplitPages, &QAction::triggered, this, &MainWindow::onSplitButtonClicked);
    connect(ui->actionShowPages, &QAction::triggered, this, &MainWindow::showPages);
    connect(ui->inputText, &QPlainTextEdit::textChanged, this, &MainWindow::invalidatePagination);
    connect(ui->pageSizeSpinBox, &QSpinBox::valueChanged, this, &MainWindow::invalidatePagination);
    connect(ui->pageUnitComboBox, &QComboBox::currentIndexChanged, this, &MainWindow::invalidatePagination);
    connect(ui->actionBuildIndexes, &QAction::triggered, this, &MainWindow::buildIndexes);
    connect(ui->indexTypeComboBox, &QComboBox::currentIndexChanged, this, &MainWindow::onIndexTypeChanged);
    connect(ui->bucketCountInput, &QLineEdit::textChanged, this, &MainWindow::invalidateIndexes);
    connect(ui->actionShowIndex, &QAction::triggered, this, &MainWindow::showIndex);
    connect(ui->actionFindWord, &QAction::triggered, this, &MainWindow::findWord);
    connect(ui->searchWordInput, &QLineEdit::textChanged, ui->searchOutput, &QPlainTextEdit::clear);
    connect(ui->caseSensitiveCheckBox, &QCheckBox::toggled, this, &MainWindow::onCaseModeChanged);
    ui->indexTable->horizontalHeader()->setStretchLastSection(true);
    onIndexTypeChanged();
}

/* Освобождает объект интерфейса окна */
MainWindow::~MainWindow()
{
    delete ui;
}

/* Разбивает введённый текст по выбранным настройкам, сохраняет и показывает страницы */
void MainWindow::onSplitButtonClicked()
{
    QString text = ui->inputText->toPlainText();

    if(text.trimmed().isEmpty()) {
        ui->pagesOutput->setPlainText(QStringLiteral("Введите текст."));
        return;
    }

    const int pageSize = ui->pageSizeSpinBox->value();
    const PageUnit unit = ui->pageUnitComboBox->currentIndex() == 0 ? PageUnit::Words : PageUnit::Characters;
    paginationResult = SplitIntoPages(text.toStdString(), pageSize, unit);
    updateActionAvailability();
    showPages();
}

/* Сбрасывает страницы и указатели при изменении текста или параметров разбиения */
void MainWindow::invalidatePagination()
{
    paginationResult = PaginationResult{};
    invalidateIndexes();
    ui->pagesOutput->clear();
}

/* Строит два указателя выбранной структуры для разных режимов учёта регистра */
void MainWindow::buildIndexes()
{
    if(paginationResult.pages.GetLength() == 0) {
        statusBar()->showMessage(QStringLiteral("Сначала разбейте текст на страницы."), 4000);
        return;
    }

    const auto& wordPages = paginationResult.wordPages;
    try {
        if(ui->indexTypeComboBox->currentIndex() == 0) {
            indexes = CaseIndexes<ArrayIndex>{
                BuildArrayIndex(wordPages, CaseMode::Depend),
                BuildArrayIndex(wordPages, CaseMode::NotDepend)
            };
        } else {
            if(!ui->bucketCountInput->hasAcceptableInput()) {
                statusBar()->showMessage(QStringLiteral("Введите положительное целое число корзин."), 4000);
                return;
            }

            const auto locale = ui->bucketCountInput->validator()->locale();
            const int bucketCount = locale.toInt(ui->bucketCountInput->text());
            indexes = CaseIndexes<HashIndex>{
                BuildHashIndex(wordPages, bucketCount, CaseMode::Depend),
                BuildHashIndex(wordPages, bucketCount, CaseMode::NotDepend)
            };
        }
    } catch(const std::bad_alloc&) {
        statusBar()->showMessage(QStringLiteral("Недостаточно памяти для построения указателей."), 5000);
        return;
    }

    updateActionAvailability();
    statusBar()->showMessage(QStringLiteral("Построены указатели с учётом регистра и без него."));
}

/* Обновляет поле числа корзин и сбрасывает указатели при смене структуры */
void MainWindow::onIndexTypeChanged()
{
    const bool hashSelected = ui->indexTypeComboBox->currentIndex() == 1;
    ui->bucketCountLabel->setEnabled(hashSelected);
    ui->bucketCountInput->setEnabled(hashSelected);
    invalidateIndexes();
}

/* Сбрасывает оба указателя, таблицу и результат поиска */
void MainWindow::invalidateIndexes()
{
    if(!std::holds_alternative<std::monostate>(indexes)) {
        indexes = std::monostate{};
        ui->indexTable->setRowCount(0);
        ui->searchOutput->clear();
        statusBar()->clearMessage();
    }

    updateActionAvailability();
}

/* Обновляет доступность команд с учётом текста, страниц, указателей и числа корзин */
void MainWindow::updateActionAvailability()
{
    const bool hasText = !ui->inputText->toPlainText().trimmed().isEmpty();
    const bool hasPages = paginationResult.pages.GetLength() > 0;
    const bool hasIndexes = !std::holds_alternative<std::monostate>(indexes);
    const bool canBuild = hasPages && (ui->indexTypeComboBox->currentIndex() == 0 || ui->bucketCountInput->hasAcceptableInput());

    ui->actionSplitPages->setEnabled(hasText);
    ui->actionShowPages->setEnabled(hasPages);
    ui->actionBuildIndexes->setEnabled(canBuild);
    ui->actionShowIndex->setEnabled(hasIndexes);
    ui->actionFindWord->setEnabled(hasIndexes);
}

/* Выводит слова и номера страниц из указателя выбранного режима */
void MainWindow::showIndex()
{
    struct Row {
        QString word;
        const DynamicArray<int>* pages;
    };

    const bool caseSensitive = ui->caseSensitiveCheckBox->isChecked();
    std::vector<Row> rows;
    if(const auto* arrayIndexes = std::get_if<CaseIndexes<ArrayIndex>>(&indexes)) {
        const auto& index = caseSensitive ? arrayIndexes->depend : arrayIndexes->notDepend;
        rows.reserve(index.GetLength());
        for(int i = 0; i < index.GetLength(); ++i) {
            const auto& entry = index[i];
            rows.push_back({QString::fromStdString(entry.key), &entry.value});
        }
    } else if(const auto* hashIndexes = std::get_if<CaseIndexes<HashIndex>>(&indexes)) {
        const auto& index = caseSensitive ? hashIndexes->depend : hashIndexes->notDepend;
        rows.reserve(index.GetCount());
        auto iterator = index.GetIterator();
        while(iterator->HasNext()) {
            const auto& entry = iterator->Next();
            rows.push_back({QString::fromStdString(entry.key), &entry.value});
        }
    } else {
        statusBar()->showMessage(QStringLiteral("Сначала постройте указатели."), 4000);
        return;
    }

    QCollator collator;
    collator.setCaseSensitivity(Qt::CaseInsensitive);
    // Ставит варианты одного слова рядом и упорядочивает их по написанию
    std::sort(rows.begin(), rows.end(), [&collator](const Row& left, const Row& right) {
        const int order = collator.compare(left.word, right.word);
        return order == 0 ? left.word < right.word : order < 0;
    });

    const int rowCount = static_cast<int>(rows.size());
    ui->indexTable->setRowCount(rowCount);
    for(int i = 0; i < rowCount; ++i) {
        ui->indexTable->setItem(i, 0, new QTableWidgetItem(rows[i].word));
        ui->indexTable->setItem(i, 1, new QTableWidgetItem(formatPageNumbers(*rows[i].pages)));
    }

    statusBar()->showMessage(QStringLiteral("Слов в указателе: %1").arg(rowCount), 4000);
}

/* Нормализует запрос и ищет страницы слова в готовом указателе выбранного режима */
void MainWindow::findWord()
{
    if(std::holds_alternative<std::monostate>(indexes)) {
        statusBar()->showMessage(QStringLiteral("Сначала постройте указатели."), 4000);
        return;
    }

    std::istringstream input(ui->searchWordInput->text().toStdString());
    std::string word;
    std::string extra;
    if(!(input >> word) || (input >> extra)) {
        ui->searchOutput->setPlainText(QStringLiteral("Введите одно слово."));
        return;
    }

    const bool caseSensitive = ui->caseSensitiveCheckBox->isChecked();
    const CaseMode mode = caseSensitive ? CaseMode::Depend : CaseMode::NotDepend;
    const std::string key = NormalizeWord(word, mode);
    if(key.empty()) {
        ui->searchOutput->setPlainText(QStringLiteral("Введите одно слово."));
        return;
    }

    const DynamicArray<int>* pages = nullptr;
    if(const auto* arrayIndexes = std::get_if<CaseIndexes<ArrayIndex>>(&indexes)) {
        const auto& index = caseSensitive ? arrayIndexes->depend : arrayIndexes->notDepend;
        const KeyValue<std::string, DynamicArray<int>> entry{key, {}};
        const int position = index.Index(entry, SearchMode::Half);
        if(position >= 0) pages = &index[position].value;
    } else if(const auto* hashIndexes = std::get_if<CaseIndexes<HashIndex>>(&indexes)) {
        const auto& index = caseSensitive ? hashIndexes->depend : hashIndexes->notDepend;
        if(index.ContainsKey(key)) pages = &index.Get(key);
    }

    if(pages == nullptr) {
        ui->searchOutput->setPlainText(QStringLiteral("Слово не найдено"));
        return;
    }

    ui->searchOutput->setPlainText(QStringLiteral("Страницы: %1").arg(formatPageNumbers(*pages)));
}

/* Сбрасывает результат поиска и обновляет открытую таблицу при смене режима регистра */
void MainWindow::onCaseModeChanged()
{
    ui->searchOutput->clear();
    if(ui->indexTable->rowCount() > 0) showIndex();
}

/* Выводит сохранённые страницы в порядке их номеров */
void MainWindow::showPages()
{
    const auto& pages = paginationResult.pages;
    if(pages.GetLength() == 0) {
        ui->pagesOutput->setPlainText(QStringLiteral("Сначала разбейте текст на страницы."));
        return;
    }

    QString output;
    for(int i = 0; i < pages.GetLength(); ++i) {
        if(i > 0) output += '\n';
        output += QStringLiteral("Страница %1:").arg(i + 1);

        for(int j = 0; j < pages[i].GetLength(); ++j) {
            output += ' ';
            output += QString::fromStdString(pages[i][j]);
        }
    }

    ui->pagesOutput->setPlainText(output);
}

