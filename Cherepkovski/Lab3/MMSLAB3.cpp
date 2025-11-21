#include <iostream>
#include <vector>
#include <iomanip>
#include <random>
#include <algorithm>
#include <cmath>
#include "strategies.h"

using namespace std;

const int rounds = 200;

vector<vector<int>> W = { { 3 , 0 },
                          { 5 , 1 } 
};


struct GameResult{
    string name;
    int points = 0;
    int maxSeries = 0;

    bool operator<(const GameResult& other) const {
        return points < other.points;
    }
};

enum StrategyType {
    Alex,
    Bob,
    Clara,
    Denis,
    Emma,
    Frida,
    George,
    Hank,
    Ivan,
    Jack,
    Kevin,
    Lucas,
    Max,
    Natan
};

string StrategyTypeToString(StrategyType a) {
    switch (a) {
    case Alex:  return "Alex";
    case Bob:   return "Bob";
    case Clara: return "Clara";
    case Denis: return "Denis";
    case Emma:  return "Emma";
    case Frida: return "Frida";
    case George:return "George";
    case Hank:  return "Hank";
    case Ivan:  return "Ivan";
    case Jack:  return "Jack";
    case Kevin: return "Kevin";
    case Lucas: return "Lucas";
    case Max:   return "Max";
    case Natan: return "Natan";
    }
}

int move(StrategyType a, const vector<int>& opp) {
    switch (a) {
    case Alex:   return alex(opp);
    case Bob:    return bob(opp);
    case Clara:  return clara(opp);
    case Denis:  return denis(opp);
    case Emma:   return emma(opp);
    case Frida:  return frida(opp);
    case George: return george(opp);
    case Hank:   return hank(opp);
    case Ivan:   return ivan(opp);
    case Jack:   return jack(opp);
    case Kevin:  return kevin(opp);
    case Lucas:  return lucas(opp);
    case Max:    return max(opp);
    case Natan:  return natan(opp);
    }
}


GameResult play(StrategyType a, StrategyType b) {         
    GameResult res;
    res.name = StrategyTypeToString(a);
    vector<int> moves;
    vector<int> movesP2;
    int domSer = 0;
    for (int i = 1; i <= rounds; i++) {
        int x = move(a, movesP2);
        int y = move(b, moves);

        moves.push_back(x);
        movesP2.push_back(y);

        int score = W[x][y];
        int scoreP2 = W[y][x];
        res.points += score;

        if (score == 5 && scoreP2 == 0)
            domSer++;
        else
            domSer = 0;
        res.maxSeries = max(res.maxSeries, domSer);
    }
    return res;
}
                                  
int main()
{
    setlocale(LC_ALL, "RU");
    int gamesAmount = 1000;
    vector<StrategyType> players{
        Alex, Bob, Clara, Denis, Emma, Frida, George,
        Hank, Ivan, Jack, Kevin, Lucas, Max, Natan
    };
    vector<vector<vector<GameResult>>> results(players.size(), vector<vector<GameResult>>(players.size()));
    vector<vector<int>> average(players.size(), vector<int>(players.size()));
    vector<vector<int>> median(players.size(), vector<int>(players.size()));
    vector<vector<int>> moda(players.size(), vector<int>(players.size()));
    vector<vector<int>> dispertion(players.size(), vector<int>(players.size()));
    double totalPoints = 0;
    for (int i = 0; i < players.size(); i++) {                                                               
        for (int j = 0; j < players.size(); j++) {
            if (i < 7 && j < 7) 
                results[i][j].push_back(play(players[i], players[j]));
            else {
                for (int k = 0; k < gamesAmount; k++) {
                    results[i][j].push_back(play(players[i], players[j]));
                }
            }
        }
    }

    for (int i = 0; i < players.size(); i++) {                                                                //среднее
        for (int j = 0; j < players.size(); j++) {
            if (i < 7 && j < 7)
                average[i][j] = results[i][j][0].points;
            else {
                double sum = 0;
                for (int k = 0; k < gamesAmount; k++) {
                    sum += results[i][j][k].points;
                }
                sum /= gamesAmount;
                average[i][j] = sum;
            }
        }
    }

    for (int i = 0; i < players.size(); i++) {                                                                //медиана
        for (int j = 0; j < players.size(); j++) {
            if (i < 7 && j < 7)
                median[i][j] = results[i][j][0].points;
            else {
                sort(results[i][j].begin(), results[i][j].end());
                int index = gamesAmount / 2;
                if (gamesAmount & 1) 
                    median[i][j] =  results[i][j][index].points;                          
                else {
                    int med = (results[i][j][index].points + results[i][j][index - 1].points) / 2;
                    median[i][j] = med;

                } 
            }
        }
    }

    for (int i = 0; i < players.size(); i++) {                                                                //дипрессия
        for (int j = 0; j < players.size(); j++) {
            if (i < 7 && j < 7)
                dispertion[i][j] = results[i][j][0].points;
            else {
                double disp = 0;
                for (int k = 0; k < gamesAmount; k++) {
                    disp += (results[i][j][k].points - average[i][j]) * (results[i][j][k].points - average[i][j]);
                }
                disp /= gamesAmount;
                dispertion[i][j] = disp;
            }
        }
    }

    int k = ceil(log2(gamesAmount + 1));
    for (int i = 0; i < players.size(); i++) {                                                                //мода
        for (int j = 0; j < players.size(); j++) {
            if (i < 7 && j < 7)
                moda[i][j] = results[i][j][0].points;
            else {
                int minP = results[i][j][0].points, maxP = results[i][j][gamesAmount-1].points;;
                double h = (double)(maxP - minP) / k;
                if (h == 0) 
                    moda[i][j] = results[i][j][0].points;
                else {
                    int maxCount = -1;
                    double bestL = 0, bestR = 0;
                    for (int с = 0; с < k; с++) {
                        double L = minP + с * h;
                        double R = (с == k - 1) ? maxP : minP + (с + 1) * h;
                        int count = 0;
                        for (int w = 0; w < gamesAmount; w++) {
                            if (results[i][j][w].points >= L && results[i][j][w].points <= R) count++;
                        }
                        if (count > maxCount) {
                            maxCount = count;
                            bestL = L;
                            bestR = R;
                        }
                    }
                    double sum = 0.0;
                    for (int q = 0; q < gamesAmount; q++) {
                        if (results[i][j][q].points >= bestL && results[i][j][q].points <= bestR) {
                            sum += results[i][j][q].points;
                        }
                    }
                    moda[i][j] = (int)sum / maxCount;
                }
            }
        }
    }

    size_t maxLen = 0;
    for (auto p : players)
        maxLen = max(maxLen, StrategyTypeToString(p).size());

    int nameColWidth = max(maxLen + 4, size_t(10)); 
    int cellWidth = 8;                             
    cout << "Среднее\n";
    cout << left << setw(nameColWidth) << "Player";                                    //среднее
    for (auto p : players)
        cout << right << setw(cellWidth) << StrategyTypeToString(p);
    cout << right << setw(cellWidth + 7) << "Total points";
    cout << endl;

    cout << string(nameColWidth + cellWidth * players.size() + 15, '-') << endl;

    for (int i = 0; i < players.size(); i++) {
        cout << left << setw(nameColWidth) << StrategyTypeToString(players[i]);
        totalPoints = 0;
        for (int j = 0; j < players.size(); j++) {
            cout << right << setw(cellWidth) << average[i][j];
            if(i != j)
            totalPoints += average[i][j];
        }
        cout << right << setw(cellWidth+3) << totalPoints << endl;
    }

    cout << string(nameColWidth + cellWidth * players.size() + 15, '-') << endl << endl;

    cout << "Медиана\n";

    cout << left << setw(nameColWidth) << "Player";                                          //медиана
    for (auto p : players)
        cout << right << setw(cellWidth) << StrategyTypeToString(p);
    cout << right << setw(cellWidth + 7) << "Total points";

    cout << endl;

    cout << string(nameColWidth + cellWidth * players.size() + 15, '-') << endl;

    for (int i = 0; i < players.size(); i++) {
        cout << left << setw(nameColWidth) << StrategyTypeToString(players[i]);
        totalPoints = 0;
        for (int j = 0; j < players.size(); j++) {
            cout << right << setw(cellWidth) << median[i][j];
            if (i != j)
                totalPoints += median[i][j];
        }
        cout << right << setw(cellWidth + 3) << totalPoints << endl;
    }


    cout << string(nameColWidth + cellWidth * players.size() + 15, '-') << endl << endl;
    cout << "Дисперсия\n";

    cout << left << setw(nameColWidth) << "Player";                                    //Дипрессия
    for (auto p : players)
        cout << right << setw(cellWidth) << StrategyTypeToString(p);
    cout << right << setw(cellWidth + 7) << "Total points";

    cout << endl;

    cout << string(nameColWidth + cellWidth * players.size() + 15, '-') << endl;

    for (int i = 0; i < players.size(); i++) {
        cout << left << setw(nameColWidth) << StrategyTypeToString(players[i]);
        totalPoints = 0;
        for (int j = 0; j < players.size(); j++) {
            cout << right << setw(cellWidth) << dispertion[i][j];
            if (i != j)
                totalPoints += dispertion[i][j];
        }
        cout << right << setw(cellWidth + 3) << totalPoints << endl;
    }

    cout << string(nameColWidth + cellWidth * players.size() + 15, '-') << endl << endl;
    cout << "Мода\n";

    cout << left << setw(nameColWidth) << "Player";                                    //мода
    for (auto p : players)
        cout << right << setw(cellWidth) << StrategyTypeToString(p);
    cout << right << setw(cellWidth + 7) << "Total points";

    cout << endl;

    cout << string(nameColWidth + cellWidth * players.size() + 15, '-') << endl;

    for (int i = 0; i < players.size(); i++) {
        cout << left << setw(nameColWidth) << StrategyTypeToString(players[i]);
        totalPoints = 0;
        for (int j = 0; j < players.size(); j++) {
            cout << right << setw(cellWidth) << moda[i][j];
            if (i != j)
                totalPoints += moda[i][j];
        }
        cout << right << setw(cellWidth + 3) << totalPoints << endl;
    }
    return 0;
}


