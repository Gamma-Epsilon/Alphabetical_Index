#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <variant>

#include "AlphabeticalIndex.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onSplitButtonClicked();

private:
    template<class Index>
    struct CaseIndexes {
        Index depend;
        Index notDepend;
    };

    void showPages();
    void invalidatePagination();
    void buildIndexes();
    void onIndexTypeChanged();
    void invalidateIndexes();
    void showIndex();
    void findWord();
    void onCaseModeChanged();
    void updateActionAvailability();

    Ui::MainWindow *ui;
    PaginationResult paginationResult;
    std::variant<std::monostate, CaseIndexes<ArrayIndex>, CaseIndexes<HashIndex>> indexes;
};
#endif // MAINWINDOW_H
