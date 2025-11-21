#include <vector>
#include <random>
#include "strategies.h"


using namespace std;

const int seed = 6456367;

mt19937 gen(seed);


int alex(const vector<int>& opp) {
    return 1;
}

int bob(const vector<int>& opp) {
    return 0;
}

int clara(const vector<int>& opp) {
    return (opp.size() == 0 ? 0 : opp.back());
}

int denis(const vector<int>& opp) {
    return (opp.size() == 0 ? 0 : 1 - opp.back());
}

int emma(const vector<int>& opp) {
    return ((opp.size() + 1) % 20 == 0 ? 1 : 0);
}

int frida(const vector<int>& opp) {
    if (opp.size() == 0)
        return 0;
    for (int x : opp)
        if (x == 1)
            return 1;
    return 0;
}

int george(const vector<int>& opp) {
    if (opp.size() < 3)
        return 0;
    if (opp[opp.size() - 1] + opp[opp.size() - 2] + opp[opp.size() - 3] == 3)
        return 1;
    else
        return 0;
}

int hank(const vector<int>& opp) {
    bernoulli_distribution dist(0.5);
    return (int)dist(gen);
}

int ivan(const vector<int>& opp) {
    bernoulli_distribution dist(0.1);
    return (int)dist(gen);
}

int jack(const vector<int>& opp) {
    bernoulli_distribution dist(0.75);
    if (opp.size() == 0)
        return 0;
    if (opp.back() == 0)
        return 0;
    else
        return (int)dist(gen);
}

int kevin(const vector<int>& opp) {
    bernoulli_distribution dist(0.75);
    if (opp.size() == 0)
        return 0;
    if (dist(gen))
        return opp.back();
    else
        return 1 - opp.back();
}

int lucas(const std::vector<int>& opp) {
    static int limit = -1;
    if (limit == -1) {
        uniform_int_distribution<int> dist(1, 50);
        limit = dist(gen);
    }
    return ((opp.size() + 1) <= limit ? 1 : 0);
}

int max(const std::vector<int>& opp) {
    static int rem = 0;
    static int curr = 0;
    if (rem == 0) {
        uniform_int_distribution<int> dist(0, 20);
        rem = dist(gen);
        curr = 1 - curr;
    }
    rem--;
    return curr;
}

int natan(const vector<int>& opp) {
    bernoulli_distribution dist(0.2);
    if (opp.size() < 2)
        return 0;
    if (opp[opp.size() - 1] + opp[opp.size() - 2] == 2)
        return (int)dist(gen);
    else
        return 0;
}