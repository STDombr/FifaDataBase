#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect_db();

    //QPixmap picx1(":/rec/pics/players/Player");
    //QPixmap picx1(":/rec/pics/players/Arcos.JPG");
    //QPixmap picx2(":/rec/pics/teams/Team");

    QString defPlayerPic = ":/rec/pics/players/Player";
    QString defTeamPic = ":/rec/pics/teams/Team";

    QPixmap circle_red(":/rec/pics/icons/circle_red.png");
    QPixmap circle_blue(":/rec/pics/icons/circle_blue.png");

    //ui->Player1Pic->setPixmap(picx1);
    //ui->Player2Pic->setPixmap(picx1);
    //ui->Team1Pic->setPixmap(picx2);
    //ui->Team2Pic->setPixmap(picx2);

    set_round_pic(ui->Player1Pic, defPlayerPic);
    set_round_pic(ui->Player2Pic, defPlayerPic);
    set_round_pic(ui->Team1Pic, defTeamPic);
    set_round_pic(ui->Team2Pic, defTeamPic);

    ui->Player1Pick->addItem(" ");
    ui->Player2Pick->addItem(" ");
    ui->Team1Pick->addItem(" ");
    ui->Team2Pick->addItem(" ");

    if(initialize_players() == 1){
        ui->Player1Pick->addItems(players);
        ui->Player2Pick->addItems(players);
    }

    if(initialize_teams() == 1){
        ui->Team1Pick->addItems(teams);
        ui->Team2Pick->addItems(teams);
    }

    active_query = nullptr;
    active_model = nullptr;

    QHeaderView *verticalHeader = this->ui->matches->verticalHeader();
    verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(50);

    initialize_restoreButtons();

    ui->lineLeft->hide();
    ui->lineRight->hide();

    brush_menu();

    ui->redCirclePlayerLabel->setScaledContents(true);
    ui->redCirclePlayerLabel->setPixmap(circle_red);
    ui->redCirclePlayerLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->blueCirclePlayerLabel->setScaledContents(true);
    ui->blueCirclePlayerLabel->setPixmap(circle_blue);
    ui->blueCirclePlayerLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    //set_round_pic(ui->label, ":/rec/pics/players/Arcos.JPG");


}

MainWindow::~MainWindow()
{
    disconnect_db();
    delete ui;
}

bool MainWindow::connect_db()
{
    matches_db = new QSqlDatabase();
    *matches_db = QSqlDatabase::addDatabase("QSQLITE");
    //matches_db->setDatabaseName("D:/FifaDataBase/DB_files/matches.db");
    matches_db->setDatabaseName("DB/matches.db");

    if(!matches_db->open())
        return 0;
    return 1;
}

void MainWindow::disconnect_db()
{

    matches_db->close();
    delete matches_db;
    matches_db->removeDatabase(QSqlDatabase::defaultConnection);
}

bool MainWindow::initialize_players()
{
    QSqlQuery *qry = new QSqlQuery(*matches_db);

    if(qry->prepare("SELECT DISTINCT PLAYER2 FROM MATCHES")){
        qry->exec();
        players.clear();

        while(qry->next()){
            QSqlRecord record = qry->record();
            players.append(record.value(0).toString());
        }

        players.sort(Qt::CaseInsensitive);
        return 1;
    } else {
        std::cout << "query error while initializing teams";
        return 0;
    }
}

bool MainWindow::initialize_teams()
{
    QSqlQuery *qry = new QSqlQuery(*matches_db);

    if(qry->prepare("SELECT DISTINCT TEAM1 FROM MATCHES")){
        qry->exec();
        teams.clear();

        while(qry->next()){
            QSqlRecord record = qry->record();
            teams.append(record.value(0).toString());
        }

        teams.sort(Qt::CaseInsensitive);
        return 1;
    } else {
        std::cout << "query error while initializing teams";
        return 0;
    }
}



bool MainWindow::initialize_restoreButton(QPushButton *in)
{
    QPixmap pixmap(":/rec/pics/icons/cross.png");
    if(pixmap.isNull())
        return 0;
    QIcon ButtonIcon(pixmap);
    in->setIcon(ButtonIcon);
    in->setIconSize(QSize(21,21));
    return 1;
}

bool MainWindow::initialize_restoreButtons()
{
    int correct = 0;
    correct += initialize_restoreButton(this->ui->restoreButton1);
    correct += initialize_restoreButton(this->ui->restoreButton2);
    correct += initialize_restoreButton(this->ui->restoreButton3);
    correct += initialize_restoreButton(this->ui->restoreButton4);
    if(correct == 4)
        return 1;
    else
        return 0;
}

void MainWindow::show_messagebox(QString header, QString message)
{
    QMessageBox messageBox;
    messageBox.setFixedSize(500,200);
    messageBox.information(nullptr,header,message);
}

void MainWindow::show_message(QString header, QString message, bool about)
{
    QMessageBox messageBox;
    messageBox.setFixedSize(700,500);
    if(about == 1){
        messageBox.about(nullptr, header, message);
        return;
    }
    messageBox.setIcon(QMessageBox::NoIcon);
    messageBox.setText(message);
    messageBox.setWindowTitle(header);
    messageBox.exec();
}

bool MainWindow::valid_players_teams()
{
    if(ui->Player1Pick->currentText() == " " &&
       ui->Player2Pick->currentText() == " " &&
       ui->Team1Pick->currentText() == " " &&
       ui->Team2Pick->currentText() == " "){
        show_messagebox("Hey", "Choose at least one team or player!");
        return 0;
    }

    if((ui->Player1Pick->currentText() == ui->Player2Pick->currentText() && ui->Player1Pick->currentText() != " ") ||
       (ui->Team1Pick->currentText() == ui->Team2Pick->currentText() && ui->Team1Pick->currentText() != " ")){
        show_messagebox("Hey", "You can't choose two identical players or teams!");
        return 0;
    }

    return 1;
}

void MainWindow::on_Player1Pick_currentIndexChanged(const QString &arg1)
{
    QString path = ":/rec/pics/players/";
    path = path + arg1;
    path = path + ".JPG";
    QPixmap picx(path);
    if(picx.isNull())
        picx.load(":/rec/pics/players/Player");
    set_round_pic(ui->Player1Pic, picx);

    if(arg1 != " ")
        player1_active = 1;
    else
        player1_active = 0;



}

void MainWindow::on_Player2Pick_currentIndexChanged(const QString &arg1)
{
    QString path = ":/rec/pics/players/";
    path = path + arg1;
    path = path + ".JPG";
    QPixmap picx(path);
    if(picx.isNull())
        picx.load(":/rec/pics/players/Player");
    set_round_pic(ui->Player2Pic, picx);

    if(arg1 != " ")
        player2_active = 1;
    else
        player2_active = 0;
}

void MainWindow::on_Team1Pick_currentIndexChanged(const QString &arg1)
{
    QString path = ":/rec/pics/teams/";
    path = path + arg1;
    path = path + ".png";
    QPixmap picx(path);
    if(picx.isNull())
        picx.load(":/rec/pics/teams/Team");
    if(arg1 == "Brazil" || arg1 == "England" || arg1 == "Belgium" || arg1 == "France" || arg1 == "Germany" || arg1 == "Italy" || arg1 == "Portugal" || arg1 == "Spain" || arg1 == "Netherlands")
        set_round_pic(ui->Team1Pic, picx);
    else
        ui->Team1Pic->setPixmap(picx);

    if(arg1 != " ")
        team1_active = 1;
    else
        team1_active = 0;
}

void MainWindow::on_Team2Pick_currentIndexChanged(const QString &arg1)
{
    QString path = ":/rec/pics/teams/";
    path = path + arg1;
    path = path + ".png";
    QPixmap picx(path);
    if(picx.isNull())
        picx.load(":/rec/pics/teams/Team");
    if(arg1 == "Brazil" || arg1 == "England" || arg1 == "Belgium" || arg1 == "France" || arg1 == "Germany" || arg1 == "Italy" || arg1 == "Portugal" || arg1 == "Spain"|| arg1 == "Netherlands")
        set_round_pic(ui->Team2Pic, picx);
    else
        ui->Team2Pic->setPixmap(picx);

    if(arg1 != " ")
        team2_active = 1;
    else
        team2_active = 0;
}

QString  MainWindow::prepare_query_text(QString p1, QString p2, QString t1, QString t2)
{
    int last_active_pick = 0;
    if(t2 != " ")
        last_active_pick = 3;
    else if(t1 != " ")
        last_active_pick = 2;
    else if (p2 != " ")
        last_active_pick = 1;

    QString query_text = "SELECT TEAM1, PLAYER1, SCORE1, SCORE2, PLAYER2, TEAM2 FROM MATCHES WHERE ";
    if(p1 != " "){
        query_text = query_text + "PLAYER1 = '";
        query_text = query_text +p1 + "'";
        if(last_active_pick == 0)
            return query_text;
        else
            query_text = query_text + " AND ";
    }
    if(p2 != " "){
        query_text = query_text + "PLAYER2 = '";
        query_text = query_text + p2 + "'";
        if(last_active_pick == 1)
            return query_text;
        else
            query_text = query_text + " AND ";
    }
    if(t1 != " "){
        query_text = query_text + "TEAM1 = '";
        query_text = query_text + t1 + "'";
        if(last_active_pick == 2)
            return query_text;
        else
            query_text = query_text + " AND ";
    }
    if(t2 != " "){
        query_text = query_text + "TEAM2 = '";
        query_text = query_text + t2 + "'";
    }
    return query_text;
}

int MainWindow::get_current_size()
{
    return current_size;
}

void MainWindow::update_current_size()
{
    if (matches_db->driver()->hasFeature(QSqlDriver::QuerySize)) {
            current_size = active_query->size();
        } else {
            active_query->last();
            current_size = active_query->at() + 1;
    }
}

void MainWindow::swap_polar_players()
{
    if(player1_active == 1){
        for(int i = 0 ; i < current_size; i++){
            if(ui->matches->model()->data(ui->matches->model()->index(i,1)).toString() != ui->Player1Pick->currentText()){
                QString temp_team, temp_player, temp_score;
                temp_team = ui->matches->model()->data(ui->matches->model()->index(i,0)).toString();
                temp_player = ui->matches->model()->data(ui->matches->model()->index(i,1)).toString();
                temp_score = ui->matches->model()->data(ui->matches->model()->index(i,2)).toString();

                ui->matches->model()->setData(ui->matches->model()->index(i,0), ui->matches->model()->data(ui->matches->model()->index(i,5)).toString());
                ui->matches->model()->setData(ui->matches->model()->index(i,1), ui->matches->model()->data(ui->matches->model()->index(i,4)).toString());
                ui->matches->model()->setData(ui->matches->model()->index(i,2), ui->matches->model()->data(ui->matches->model()->index(i,3)).toString());

                ui->matches->model()->setData(ui->matches->model()->index(i,3), temp_score);
                ui->matches->model()->setData(ui->matches->model()->index(i,4), temp_player);
                ui->matches->model()->setData(ui->matches->model()->index(i,5), temp_team);

            }
        }
        return;
    }

}

void MainWindow::on_findButton_clicked()
{
    if(valid_players_teams()){

        QSqlQuery *qry = new QSqlQuery(*matches_db);
        QSqlQueryModel *model = new QSqlQueryModel();
        QString select1 = prepare_query_text(ui->Player1Pick->currentText(),ui->Player2Pick->currentText(),ui->Team1Pick->currentText(),ui->Team2Pick->currentText());
        QString select2 = prepare_query_text(ui->Player2Pick->currentText(),ui->Player1Pick->currentText(),ui->Team2Pick->currentText(),ui->Team1Pick->currentText());
        QString query_text = select1 + " UNION " + select2;
        qry->prepare(query_text);
        qry->exec();
        model->setQuery(*qry);
        ui->matches->setModel(model);

        //connect(this->ui->matches->model(), SIGNAL(rowsInserted(const QModelIndex &, int, int)), this, SLOT(scroll_model(const QModelIndex &, int, int)));

        active_model = model;
        active_query = qry;
        update_current_size();

        //swap_polar_players();
        fill_matches_stat();
    }


}

void MainWindow::fill_matches_stat()
{
    if(current_size <= 0){
        ui->MatchesLabel->setText("No matches found");
        restore_stat_labels();
        restore_table_model();
        return;
    }

    ui->lineLeft->show();
    ui->lineRight->show();

    ui->matches->scrollToBottom();
    ui->matches->scrollToBottom();
    ui->matches->scrollToBottom();
    ui->matches->scrollToBottom();
    ui->matches->scrollToBottom();

    //QThread::msleep(500);

    ui->MatchesLabel->setText("Matches played: " + QString::number(current_size));

    int p1w, p2w, d, d0, d1, d2, bts, ohs, thb; //player1 won; player2 won; draws; draws 0-0; draws 1-1; draws 2-2; both to score; 1.5<; 2.5>;
    double  all_goals, goals_p1, goals_p2;

    p1w = p2w = d = d1 = d2 = d0 = bts = ohs = thb = 0;
    all_goals = goals_p1 = goals_p2 = 0;

    int score1, score2;
    bool swapped = 0;
    for(int i = 0; i < current_size; i++){

        score1 = ui->matches->model()->data(ui->matches->model()->index(i,2)).toInt();
        score2 = ui->matches->model()->data(ui->matches->model()->index(i,3)).toInt();

        //swap players stat if the player is on the other slot
        if (ui->Player1Pick->currentIndex() != 0){
            if (ui->matches->model()->data(ui->matches->model()->index(i,1)) != ui->Player1Pick->currentText()){
                int temp = score1;
                score1 = score2;
                score2 = temp;
                swapped = 1;
            }
        }
        //else
        if (ui->Player2Pick->currentIndex() != 0){
            if (ui->matches->model()->data(ui->matches->model()->index(i,4)) != ui->Player2Pick->currentText() && swapped == 0){
                int temp = score1;
                score1 = score2;
                score2 = temp;
                swapped = 1;
            }
        }

        if(ui->Player1Pick->currentIndex() == 0 && ui->Player2Pick->currentIndex() == 0){
            if(ui->Team1Pick->currentIndex() != 0){
                if (ui->matches->model()->data(ui->matches->model()->index(i,0)) != ui->Team1Pick->currentText()){
                    int temp = score1;
                    score1 = score2;
                    score2 = temp;
                    swapped = 1;
                }
            }
            if(ui->Team2Pick->currentIndex() != 0){
                if (ui->matches->model()->data(ui->matches->model()->index(i,5)) != ui->Team2Pick->currentText() && swapped == 0){
                    int temp = score1;
                    score1 = score2;
                    score2 = temp;
                }
            }
        }

        if(score1 > score2){
            p1w++;
        } else if(score1 < score2){
            p2w++;
        } else {
            d++;
            if(score1 == 0){
                d0++;
            }
            else if (score1 == 1)
                d1++;
            else if(score1 == 2)
                d2++;
        }

        if(score1 > 0 && score2 > 0)
            bts++;

    if(score1 + score2 < 2)
        ohs++;
    if(score1 + score2 > 2)
        thb++;

    all_goals = all_goals + score1 + score2;
    goals_p1 = goals_p1 + score1;
    goals_p2 = goals_p2 + score2;

    swapped = 0;
    }

    if(ui->Player1Pick->currentText() != " "){
        ui->P1WLabel->setText(ui->Player1Pick->currentText() + " won: " + QString::number(p1w));
        ui->IndTotP1Label->setText("Individual total " + ui->Player1Pick->currentText() + ": " + QString::number(goals_p1 / current_size));
    }
    else{
        if(ui->Team1Pick->currentText() != " "){
            ui->P1WLabel->setText(ui->Team1Pick->currentText() + " won: " + QString::number(p1w));
            ui->IndTotP1Label->setText("Individual total " + ui->Team1Pick->currentText() + ": " + QString::number(goals_p1 / current_size));
        }
        else {
            ui->P1WLabel->setText("Red player won: " + QString::number(p1w));
            ui->IndTotP1Label->setText("Individual total Red player: " + QString::number(goals_p1 / current_size));
        }
    }
    if(ui->Player2Pick->currentText() != " "){
        ui->P2WLabel->setText(ui->Player2Pick->currentText() + " won: " + QString::number(p2w));
        ui->IndTotP2Label->setText("Individual total " + ui->Player2Pick->currentText() + ": " + QString::number(goals_p2 / current_size));
    }
    else{
        if(ui->Team2Pick->currentText() != " "){
            ui->P2WLabel->setText(ui->Team2Pick->currentText() + " won: " + QString::number(p2w));
            ui->IndTotP2Label->setText("Individual total " + ui->Team2Pick->currentText() + ": " + QString::number(goals_p2 / current_size));
        }
        else {
            ui->P2WLabel->setText("Blue player won: " + QString::number(p2w));
            ui->IndTotP2Label->setText("Individual total Blue player: " + QString::number(goals_p2 / current_size));
        }
    }
    ui->DrawLabel->setText("Draws: " + QString::number(d));
    ui->Draw0Label->setText("0-0: " + QString::number(d0));
    ui->Draw1Label->setText("1-1: " + QString::number(d1));
    ui->Draw2Label->setText("2-2: " + QString::number(d2));

    ui->P1WPerLabel->setText(QString::number(p1w * 100 / current_size) + "%");
    ui->P2WPerLabel->setText(QString::number(p2w * 100 / current_size) + "%");
    ui->DrawPerLabel->setText(QString::number(100 - (p1w * 100 / current_size) - (p2w * 100 / current_size)) + "%");

    ui->MatchTotLabel->setText("Average match total: " + QString::number(all_goals / current_size));

    ui->MatchesLabel->setText(ui->MatchesLabel->text() + " (" + QString::number(p1w) + "-" + QString::number(d) + "-" + QString::number(p2w) + ")");

    ui->bothToScoreNameLabel->setText("Both to score:");

    int bts_percentage = bts * 100 / current_size;
    ui->bothToScoreLabel->setText(QString::number(bts_percentage) + "%");

    int ohs_percentage, thb_percentage;
    ohs_percentage = ohs * 100 / current_size;
    thb_percentage = thb * 100 / current_size;

    ui->oneHalfSmallerNameLabel->setText("1.5<:");
    ui->oneHalfSmallerLabel->setText(QString::number(ohs_percentage) + "%");
    ui->twoHalfBiggerNameLabel->setText("2.5>:");
    ui->twoHalfBiggerLabel->setText(QString::number(thb_percentage) + "%");
}

void MainWindow::brush_menu()
{
    QString colorStr = QString("background-color:") + QString("gainsboro") + ";";

    ui->menuBar->setStyleSheet(colorStr);
}

void MainWindow::restore_qlabel(QLabel *in, int pic)
{
    if(pic == 0)
        set_round_pic(in, ":/rec/pics/players/Player");
    else
        set_round_pic(in, ":/rec/pics/teams/Team.png");

}

void MainWindow::restore_qcombobox(QComboBox *in)
{
    in->setCurrentIndex(0);
}



void MainWindow::on_restoreButton1_clicked()
{
    restore_qlabel(ui->Player1Pic, 0);
    restore_qcombobox(ui->Player1Pick);
    player1_active = 0;
}


void MainWindow::on_restoreButton2_clicked()
{
    restore_qlabel(ui->Player2Pic, 0);
    restore_qcombobox(ui->Player2Pick);
    player2_active = 0;
}

void MainWindow::on_restoreButton3_clicked()
{
    restore_qlabel(ui->Team1Pic, 1);
    restore_qcombobox(ui->Team1Pick);
    team1_active = 0;
}

void MainWindow::on_restoreButton4_clicked()
{
    restore_qlabel(ui->Team2Pic, 1);
    restore_qcombobox(ui->Team2Pick);
    team2_active = 0;
}

void MainWindow::on_restoreAllButton_clicked()
{
    on_restoreButton1_clicked();
    on_restoreButton2_clicked();
    on_restoreButton3_clicked();
    on_restoreButton4_clicked();

    restore_table_model();

    restore_stat_labels();
    ui->MatchesLabel->clear();
}


void MainWindow::restore_stat_labels()
{
    ui->P1WLabel->clear();
    ui->P2WLabel->clear();
    ui->Draw0Label->clear();
    ui->Draw1Label->clear();
    ui->Draw2Label->clear();
    ui->DrawLabel->clear();
    ui->P1WPerLabel->clear();
    ui->P2WPerLabel->clear();
    ui->DrawPerLabel->clear();
    //ui->MatchesLabel->clear();
    ui->MatchTotLabel->clear();
    ui->IndTotP1Label->clear();
    ui->IndTotP2Label->clear();
    ui->twoHalfBiggerLabel->clear();
    ui->twoHalfBiggerNameLabel->clear();
    ui->oneHalfSmallerLabel->clear();
    ui->oneHalfSmallerNameLabel->clear();
    ui->bothToScoreLabel->clear();
    ui->bothToScoreNameLabel->clear();

    ui->lineLeft->hide();
    ui->lineRight->hide();
}

void MainWindow::restore_table_model()
{
    if(active_model)
        active_model->clear();
}

void MainWindow::on_actionInfo_triggered()
{
    show_message("How to use?", "This app was created in order to get better understanding in ESportsBattle betting. U just choose the player and seek through our data base in order to find some useful information that could help u win more bets. We don't take responsibility on your bets or anything. The idea is: \n 1)Choose the players and the teams(or only players or only teams. Or just only 1 player. whatever.) \n 2)Press find button and get the result of searching \n 3)Use the received information and your deduction in order to find some weird dependencies. \n 4)Become a successful person! \n\n Good Luck in your bets!", 0);
}

void MainWindow::on_actionAuthors_triggered()
{
    show_message("Authors", "This project was invented, worked and performed by Kyiv National University students: \n Vitaliy Datsiuk \n Stanislav Dombrovskyi", 1);
}

//void MainWindow::scroll_model(const QModelIndex &parent, int start, int end)
//{
//    ui->matches->scrollTo(ui->matches->model()->index(end,0));
//}



void MainWindow::on_player1PicButton_clicked()
{
    if(player1ComboBox_active){
        ui->Player1Pick->hidePopup();
        player1ComboBox_active = 0;
    }
    else{
        ui->Player1Pick->showPopup();
        player1ComboBox_active = 1;
    }

}

void MainWindow::on_player2PicButton_clicked()
{
    if(player2ComboBox_active){
        ui->Player2Pick->hidePopup();
        player2ComboBox_active = 0;
    }
    else{
        ui->Player2Pick->showPopup();
        player2ComboBox_active = 1;
    }
}

void MainWindow::on_team1PicButton_clicked()
{
    if(team1ComboBox_active){
        ui->Player1Pick->hidePopup();
        team1ComboBox_active = 0;
    }
    else{
        ui->Player1Pick->showPopup();
        team1ComboBox_active = 1;
    }
}

void MainWindow::on_team2PicButton_clicked()
{
    if(team2ComboBox_active){
        ui->Team2Pick->hidePopup();
        team2ComboBox_active = 0;
    }
    else{
        ui->Team2Pick->showPopup();
        team2ComboBox_active = 1;
    }
}


void MainWindow::on_Player1Pick_activated(const QString &arg1)
{
    player1ComboBox_active = 0;
}

void MainWindow::on_Player2Pick_activated(const QString &arg1)
{
    player2ComboBox_active = 0;
}

void MainWindow::on_Team1Pick_activated(const QString &arg1)
{
    team1ComboBox_active = 0;
}

void MainWindow::on_Team2Pick_activated(const QString &arg1)
{
   team2ComboBox_active = 0;
}

void MainWindow::set_round_pic(QLabel* picholder, QString path)
{
    QPixmap picx(path);

    QBitmap temporaryBitmap(150, 150);
    temporaryBitmap.fill(Qt::color0);

    QPainter painter(&temporaryBitmap);
    painter.setBrush(Qt::color1);
    painter.drawRoundedRect(0, 0, 150, 150, 800, 800);

    picx.setMask(temporaryBitmap);
    picholder->setPixmap(picx);
}

void MainWindow::set_round_pic(QLabel *picholder, QPixmap pic)
{
    QBitmap temporaryBitmap(150, 150);
    temporaryBitmap.fill(Qt::color0);

    QPainter painter(&temporaryBitmap);
    painter.setBrush(Qt::color1);
    painter.drawRoundedRect(0, 0, 150, 150, 800, 800);

    pic.setMask(temporaryBitmap);
    picholder->setPixmap(pic);
}

