#include <iostream>
#include <vector>
#include <unordered_set>
#include <set>
#include <stack>
#include <utility>

using namespace std;

struct Point {
    int x, y;
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
    bool operator<(const Point& other) const {
        return tie(x, y) < tie(other.x, other.y);
    }
};

namespace std {
    template <>
    struct hash<Point> {
        std::size_t operator()(const Point& k) const {
            return std::hash<int>()(k.x) ^ (std::hash<int>()(k.y) << 1);
        }
    };
}

// Check if the next position is an empty cell
bool canMoveForward(Point cur, Point next, const unordered_set<Point>& soldiers) {
    if (cur.x == next.x) {
        for (int j = cur.y + 1; j < next.y; ++j) {
            if (soldiers.find({cur.x, j}) != soldiers.end()) {
                return false;
            }
        }
        return true;
    }
    return false;
}

// Check if the path is clear for a left turn (decreasing y value)
bool canMoveLeft(Point cur, Point next, const unordered_set<Point>& soldiers) {
    if (cur.y == next.y) {
        for (int i = cur.x - 1; i > next.x; --i) {
            if (soldiers.find({i, cur.y}) != soldiers.end()) {
                return false;
            }
        }
        return true;
    }
    return false;
}

void findPaths(Point cur, Point start, unordered_set<Point>& soldiers, vector<Point>& path, vector<vector<Point>>& allPaths, bool isReturning) {
    if (soldiers.empty() && cur == start) {
        allPaths.push_back(path);
        return;
    }

    if (!isReturning) { // moving forward
        for (auto soldier : soldiers) {
            if (cur.y < soldier.y && canMoveForward(cur, soldier, soldiers)) {
                soldiers.erase(soldier);
                path.push_back(soldier);
                findPaths({soldier.x, soldier.y}, start, soldiers, path, allPaths, false);
                path.pop_back();
                soldiers.insert(soldier);
            }
        }
        // Start returning home after moving forward is done
        findPaths(cur, start, soldiers, path, allPaths, true);
    } else { // returning home
        for (auto soldier : soldiers) {
            if (cur.y > soldier.y && canMoveLeft(cur, soldier, soldiers)) {
                soldiers.erase(soldier);
                path.push_back(soldier);
                findPaths({soldier.x, soldier.y}, start, soldiers, path, allPaths, true);
                path.pop_back();
                soldiers.insert(soldier);
            }
        }
    }
}

int main() {
    int n;
    cout << "Enter the number of soldiers: ";
    cin >> n;

    unordered_set<Point> soldiers;
    for (int i = 0; i < n; ++i) {
        Point soldier;
        cout << "Enter coordinates for soldier " << i + 1 << " (x y): ";
        cin >> soldier.x >> soldier.y;
        soldiers.insert(soldier);
    }

    Point castle;
    cout << "Enter the coordinates for your special castle (x y): ";
    cin >> castle.x >> castle.y;

    vector<vector<Point>> allPaths;
    vector<Point> path = {castle};

    // Initially, it is not returning home
    findPaths(castle, castle, soldiers, path, allPaths, false);

    cout << "There are " << allPaths.size() << " unique paths for your special castle." << endl;
    for (size_t i = 0; i < allPaths.size(); ++i) {
        cout << "Path " << i + 1 << ":\nStart: (" << castle.x << ", " << castle.y << ")\n";
        for (auto& point : allPaths[i]) {
            cout << "Kill: (" << point.x << ", " << point.y << "). Turn Left.\n";
        }
         
    }
}