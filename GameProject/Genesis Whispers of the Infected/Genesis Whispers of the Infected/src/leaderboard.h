#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <string>
#include <vector>

struct ScoreEntry {
    char name[32];
    int score;
};

class Leaderboard {
private:
    std::vector<ScoreEntry> entries;
    const std::string filename = "scores.dat";

public:
    Leaderboard();
    void LoadScores();
    void SaveScores();
    void AddScore(const std::string& name, int score);
    const std::vector<ScoreEntry>& GetEntries() const { return entries; }
};

#endif
