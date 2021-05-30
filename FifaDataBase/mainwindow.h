#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSql>
#include <iostream>
#include <QPixmap>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QSqlRecord>
#include <QSqlDriver>
#include <QWindow>
#include <QComboBox>
#include <QThread>
#include <QBitmap>
#include <QPainter>

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
    void on_Player1Pick_currentIndexChanged(const QString &arg1);

    void on_Player2Pick_currentIndexChanged(const QString &arg1);

    void on_Team1Pick_currentIndexChanged(const QString &arg1);

    void on_Team2Pick_currentIndexChanged(const QString &arg1);

    void on_findButton_clicked();                               /////////////////////////////////////////

    void on_restoreButton1_clicked();

    void on_restoreButton2_clicked();

    void on_restoreButton3_clicked();

    void on_restoreButton4_clicked();

    void on_restoreAllButton_clicked();

    void on_actionInfo_triggered();

    void on_actionAuthors_triggered();

    //void scroll_model(const QModelIndex &parent, int start, int end);

    void on_player1PicButton_clicked();

    void on_player2PicButton_clicked();

    void on_team1PicButton_clicked();

    void on_team2PicButton_clicked();

    void on_Player1Pick_activated(const QString &arg1);

    void on_Player2Pick_activated(const QString &arg1);

    void on_Team1Pick_activated(const QString &arg1);

    void on_Team2Pick_activated(const QString &arg1);

private:
    Ui::MainWindow *ui;
    QSqlDatabase* matches_db;
    QStringList players, teams;
    QSqlQuery *active_query;
    QSqlQueryModel *active_model;
    int current_size;
    bool player1_active = 0;
    bool player2_active = 0;
    bool team1_active = 0;
    bool team2_active = 0;
    bool player1ComboBox_active = 0;
    bool player2ComboBox_active = 0;
    bool team1ComboBox_active = 0;
    bool team2ComboBox_active = 0;

    bool connect_db();
    void disconnect_db();

    bool initialize_players();
    bool initialize_teams();

    bool initialize_restoreButtons();
    bool initialize_restoreButton(QPushButton *in);

    void restore_qlabel(QLabel* in, int pic);
    void restore_qcombobox(QComboBox* in);
    void restore_stat_labels();
    void restore_table_model();

    void show_messagebox(QString header, QString message);
    void show_message(QString header, QString message, bool about);

    bool valid_players_teams();
    QString prepare_query_text(QString p1, QString p2, QString t1, QString t2);

    int get_current_size();
    void update_current_size();

    void swap_polar_players();

    void fill_matches_stat();

    void brush_menu();

    void set_round_pic(QLabel* picholder, QString path);
    void set_round_pic(QLabel* picholder, QPixmap pic);
};

#endif // MAINWINDOW_H
