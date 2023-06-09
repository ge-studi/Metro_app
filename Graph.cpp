// C++ code to represent metro map system using graph data structure and find the shortest path.
#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>

using namespace std;

class Graph_M
{
public:
    class Vertex {
    public:
        unordered_map<string, pair<int, int>> nbrs; // Updated to store both distance and stoppages
    };

    unordered_map<string, Vertex> vtces;

    // Function to get the number of vertices in the graph
    int numVertices() {
        return vtces.size();
    }

    // Function to check if the graph contains a specific vertex
    bool containsVertex(string vname) {
        return vtces.count(vname) > 0;
    }

    // Function to add a vertex to the graph
    void addVertex(string vname) {
        Vertex vtx;
        vtces[vname] = vtx;
    }

    // Function to remove a vertex from the graph
    void removeVertex(string vname) {
        Vertex vtx = vtces[vname];
        vector<string> keys;
        for (auto entry : vtx.nbrs) {
            keys.push_back(entry.first);
        }

        for (string key : keys) {
            Vertex nbrVtx = vtces[key];
            nbrVtx.nbrs.erase(vname);
        }

        vtces.erase(vname);
    }

    // Function to get the number of edges in the graph
    int numEdges() {
        int count = 0;
        for (auto entry : vtces) {
            Vertex vtx = entry.second;
            count += vtx.nbrs.size();
        }
        return count / 2;
    }

    // Function to check if an edge exists between two vertices
    bool containsEdge(string vname1, string vname2) {
        if (vtces.count(vname1) == 0 || vtces.count(vname2) == 0 || vtces[vname1].nbrs.count(vname2) == 0) {
            return false;
        }
        return true;
    }

    // Function to add an edge between two vertices
    void addEdge(string vname1, string vname2, int distance, int stoppages) {
        if (vtces.count(vname1) == 0 || vtces.count(vname2) == 0 || vtces[vname1].nbrs.count(vname2) > 0) {
            return;
        }
        vtces[vname1].nbrs[vname2] = make_pair(distance, stoppages);
        vtces[vname2].nbrs[vname1] = make_pair(distance, stoppages);
    }

    // Function to remove an edge between two vertices
    void removeEdge(string vname1, string vname2) {
        if (vtces.count(vname1) == 0 || vtces.count(vname2) == 0 || vtces[vname1].nbrs.count(vname2) == 0) {
            return;
        }
        vtces[vname1].nbrs.erase(vname2);
        vtces[vname2].nbrs.erase(vname1);
    }

    // Function to display the metro map
    void displayMap() {
        cout << "\t Lucknow Metro Map" << endl;
        cout << "\t------------------" << endl;
        cout << "----------------------------------------------------" << endl;
        cout << endl;
        for (auto entry : vtces) {
            string key = entry.first;
            string str = key + " =>" + "\n";
            Vertex vtx = entry.second;
            for (auto nbr : vtx.nbrs) {
                str += "\t" + nbr.first + "\t";
                if (nbr.first.length() < 16)
                    str += "\t";
                if (nbr.first.length() < 8)
                    str += "\t";
                str += "Distance: " + to_string(nbr.second.first) + " km\t";
                str += "Stoppages: " + to_string(nbr.second.second) + "\n";
            }
            cout << str << endl;
        }
        cout << "\t------------------" << endl;
        cout << "---------------------------------------------------" << endl;
    }

    // Function to display the list of stations
    void displayStations() {
        cout << endl;
        cout << "***********************************************************************" << endl;
        int i = 1;
        for (auto entry : vtces) {
            cout << i << ". " << entry.first << endl;
            i++;
        }
        cout << "***********************************************************************" << endl;
        cout << endl;
    }

    // Function to find the shortest path between two stations
    void shortestPath(string src, string dest) {
        unordered_map<string, vector<pair<int, int>>> routes; // Stores all possible routes
        unordered_map<string, int> distance;
        unordered_map<string, string> parent;
        unordered_map<string, int> stoppages; // Stores the number of stoppages
        for (auto entry : vtces) {
            distance[entry.first] = INT_MAX;
            stoppages[entry.first] = INT_MAX;
        }

        distance[src] = 0;
        stoppages[src] = 0;
        parent[src] = "";

        priority_queue<pair<pair<int, int>, string>, vector<pair<pair<int, int>, string>>, greater<pair<pair<int, int>, string>>> pq;
        pq.push(make_pair(make_pair(0, 0), src)); // Updated to include both distance and stoppages

        while (!pq.empty()) {
            pair<pair<int, int>, string> curr = pq.top();
            pq.pop();
            string currVtx = curr.second;
            int currDist = curr.first.first;
            int currStoppages = curr.first.second;

            for (auto nbr : vtces[currVtx].nbrs) {
                string nbrVtx = nbr.first;
                int edgeWeight = nbr.second.first;
                int edgeStoppages = nbr.second.second;

                int newDistance = currDist + edgeWeight;
                int newStoppages = currStoppages + edgeStoppages;

                if (newDistance < distance[nbrVtx] || (newDistance == distance[nbrVtx] && newStoppages < stoppages[nbrVtx])) {
                    distance[nbrVtx] = newDistance;
                    stoppages[nbrVtx] = newStoppages;
                    parent[nbrVtx] = currVtx;
                    pq.push(make_pair(make_pair(newDistance, newStoppages), nbrVtx));
                }
            }
        }

        vector<vector<string>> paths; // Stores all possible paths
        vector<string> path;
        string temp = dest;
        path.push_back(temp);
        while (parent[temp] != "") {
            path.push_back(parent[temp]);
            temp = parent[temp];
        }
        reverse(path.begin(), path.end());

        if (path[0] != src) {
            cout << "There is no direct metro route available from " << src << " to " << dest << "." << endl;
        } else {
            cout << "The shortest metro route(s) from " << src << " to " << dest << " is/are: " << endl;

            for (int i = 0; i < path.size(); i++) {
                string station = path[i];
                if (station != dest && i != 0) {
                    cout << station << " -> ";
                    continue;
                }

                if (i != 0) {
                    cout << station << endl;
                    cout << "Total distance: " << distance[dest] << " km" << endl;
                    cout << "Total stoppages: " << stoppages[dest] << endl;
                    cout << "Estimated time: " << stoppages[dest] * 5 << " minutes" << endl; // Assuming an average of 5 minutes per stoppage
                    cout << "Fare: Rs. " << calculateFare(stoppages[dest]) << endl;
                    cout << endl;
                }

                vector<string> new_path;
                for (int j = 0; j <= i; j++) {
                    new_path.push_back(path[j]);
                }
                paths.push_back(new_path);
            }

            cout << "Other possible routes: " << endl;
            for (int i = 0; i < paths.size(); i++) {
                vector<string> route = paths[i];
                cout << "Route " << i + 1 << ": ";
                for (int j = 0; j < route.size(); j++) {
                    cout << route[j];
                    if (j != route.size() - 1)
                        cout << " -> ";
                }
                cout << endl;
                cout << "Total distance: " << distance[dest] << " km" << endl;
                cout << "Total stoppages: " << stoppages[dest] << endl;
                cout << "Estimated time: " << stoppages[dest] * 5 << " minutes" << endl; // Assuming an average of 5 minutes per stoppage
                cout << "Fare: Rs. " << calculateFare(stoppages[dest]) << endl;
                cout << endl;
            }
        }
    }

    // Function to calculate the fare based on the number of stoppages
    int calculateFare(int stoppages) {
        if (stoppages <= 4) {
            return 10;
        } else if (stoppages <= 8) {
            return 15;
        } else {
            return 20;
        }
    }
};

int main() {
    Graph_M metroMap;

    metroMap.addVertex("Amausi");
    metroMap.addVertex("Charbagh");
    metroMap.addVertex("Hussainganj");
    metroMap.addVertex("Sadar");
    metroMap.addVertex("Hazratganj");
    metroMap.addVertex("IndiraNagar");
    metroMap.addVertex("GomtiNagar");
    metroMap.addVertex("Alambagh");
    metroMap.addVertex("TransportNagar");

    metroMap.addEdge("Amausi", "Charbagh", 13, 0);
    metroMap.addEdge("Charbagh", "Hussainganj", 3, 1);
    metroMap.addEdge("Charbagh", "Sadar", 2, 1);
    metroMap.addEdge("Hussainganj", "Sadar", 4, 0);
    metroMap.addEdge("Sadar", "Hazratganj", 1, 0);
    metroMap.addEdge("Hazratganj", "IndiraNagar", 6, 1);
    metroMap.addEdge("IndiraNagar", "GomtiNagar", 7, 0);
    metroMap.addEdge("GomtiNagar", "Alambagh", 8, 1);
    metroMap.addEdge("Alambagh", "TransportNagar", 5, 0);

    metroMap.displayMap();

    string source, destination;
    cout << "Enter the source station: ";
    cin >> source;
    cout << "Enter the destination station: ";
    cin >> destination;

    metroMap.shortestPath(source, destination);

    return 0;
}
