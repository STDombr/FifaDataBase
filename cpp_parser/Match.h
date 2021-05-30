//
// Created by vetal on 25/06/2020.
//

#ifndef CPP_MATCH_H
#define CPP_MATCH_H

#include <string>
#include <iostream>

class Match {
public:
    explicit Match(std::string in);

    std::string time;
    std::string team1;
    std::string team2;
    std::string player1;
    std::string player2;
    std::string score1;
    std::string score2;
};


#endif //CPP_MATCH_H
