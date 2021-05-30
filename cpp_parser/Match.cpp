//
// Created by vetal on 25/06/2020.
//

#include "Match.h"

Match::Match(std::string in) {

    std::string brick;

    size_t i = 0;
    size_t start_pos = 0;
    size_t str_length = 0;

    char temp = in[i];
    while(isdigit(temp) || temp == ':'){
        if(!isdigit(temp) && temp != ':' &&temp != ' '){
            std::cout << "error with time parsing" << std::endl;
            return;
        }
        i++;
        temp = in[i];
    }

    if(i > in.size()){
        //std::cout << "111" << std::endl;
        return;
    }

    this->time = in.substr(start_pos, i);
    i++;
    start_pos = i;
    temp = in[i];
    while(temp !='('){
        i++;
        temp = in[i];
    }
    this->team1 = in.substr(start_pos, i - start_pos);
    i++;
    start_pos = i;
    temp = in[i];

    if(i > in.size()){
        //std::cout << "aaa" << std::endl;
        return;
    }

    while(temp !=')'){
        i++;
        temp = in[i];
    }
    this->player1 = in.substr(start_pos, i - start_pos);
    i = i + 4;
    start_pos = i;
    temp = in[i];

    while(temp !='('){
        i++;
        temp = in[i];
    }
    this->team2 = in.substr(start_pos, i - start_pos);
    i++;
    start_pos = i;
    temp = in[i];

    if(i > in.size()){
        //std::cout << "bbb" << std::endl;
        return;
    }

    while(temp !=')'){
        i++;
        temp = in[i];
    }
    this->player2 = in.substr(start_pos, i - start_pos);
    i++;
    start_pos = i;
    temp = in[i];

    if(i > in.size()){
        //std::cout << "ccc" << std::endl;
        return;

    }


    while(!isdigit(temp)){
        i++;
        temp = in[i];
    }
    if(i > in.size()){
        //std::cout << "ddd" << std::endl;
        return;

    }
    this->score1 = temp;
    this->score2 = in[i + 2];


}
