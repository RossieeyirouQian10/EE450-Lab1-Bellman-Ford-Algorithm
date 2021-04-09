//
//  BellmanFord.cpp
//  LAB1
//
//  Created by 钱依柔 on 5/30/19.
//  Copyright © 2019 钱依柔. All rights reserved.
//

#include "BellmanFord.hpp"


#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>
#include <climits>
#include <regex>
#include <algorithm>
#include <stdio.h>


using namespace std;

const int INF = INT_MAX;
string srcFileName;

// read in file
vector<vector<int>> readFile(const string fileName){
    vector<vector<int>> graph;
    ifstream inFile(fileName,ios::in);
    if(!inFile.is_open()){
        cout << "File not found: " << fileName << endl;
        exit(0);
    }
    
    //get line in file to string
    string line;
    while (getline(inFile, line)){
        if (line.length() < 2){
            continue;
        }
        stringstream ss(line);
        string strIn;
        ss >> strIn;
        vector<int> vertex;
        while (strIn.length() > 0){
            transform(strIn.begin(), strIn.end(), strIn.begin(), ::toupper);
            
            // store distance value
            if (strIn.compare("INF") == 0) {
                vertex.push_back(INF);
            }else{
                
                // multi-cost case
                if (strIn[0] == '['){
                    int n1, n2, n3;
                    sscanf(strIn.c_str(), "[%d,%d,%d]", &n1, &n2, &n3);
                    vertex.push_back(n1 + n2 + n3);
                }else{
                    vertex.push_back(stoi(strIn.c_str()));
                }
            }
            strIn.clear();
            ss >> strIn;
        }
        
        // store values into graph
        graph.push_back(vertex);
    }
    return graph;
}

// do Bellman-Ford algorithm
void bellmanFord(vector<vector<int>> graph){
    int numNode = (int)graph.size();
    int count = 0;
    vector<int> distance;
    distance.push_back(0);
    
    //initialize the distance
    for (int i = 1; i < numNode; i++){
        distance.push_back(INT_MAX);
    }
    
    //last node having done relax calcution
    vector<int> preNode(numNode, -1);
    bool distUpdate;
    
    //do relax and save the shortest distance value
    for (int i = 0; i < numNode - 1; i++){
        distUpdate = false;
        count++;
        for (int j = 0; j < numNode; j++){
            for (int k = 0; k < graph[j].size(); k ++){
                if (graph[j][k] != INT_MAX && distance[j] != INT_MAX && distance[j] + graph[j][k] < distance[k]){
                    distance[k] = distance[j] + graph[j][k];
                    preNode[k] = j;
                    distUpdate = true;
                }
            }
        }
        if (!distUpdate){
            break;
        }
    }
    
    //detect negative loop
    count++;
    bool bFlag = false;
    for (int j = 0; j < numNode; j++){
        if(bFlag){
            break;
        }
        for (int k = 0; k < graph[j].size(); k++){
            if (graph[j][k] != INT_MAX && distance[j] != INT_MAX && distance[j] + graph[j][k] < distance[k]){
                bFlag = true;
                break;
            }
        }
    }
    // output file with result
    ofstream outFile;
    outFile.open("output-" + srcFileName, ios::out);
    if (bFlag){
        outFile << "Negative Loop Detected" << endl;
        
        //show negative iteration path
        int nPre;
        string strOut;
        stringstream ssOut;
        nPre = preNode[0];
        ssOut << nPre << "->" << 0;
        strOut = ssOut.str();
        while (nPre){
            nPre = preNode[nPre];
            stringstream ssOut2;
            ssOut2 << nPre << "->" << strOut;
            strOut = ssOut2.str();
        }
        outFile << strOut << endl;
    }else{
        
        //show distance to each node
        outFile << distance[0];
        for (int i = 1; i < numNode; i++){
            outFile << "," << distance[i];
        }
        outFile << endl << 0 << endl;
        
        //show non-negative iteration path
        int nPre;
        string strOut;
        for (int i = 1; i < numNode; i++){
            stringstream ssOut;
            nPre = preNode[i];
            ssOut << nPre << "->" << i;
            strOut = ssOut.str();
            while (nPre){
                nPre = preNode[nPre];
                stringstream ssOut2;
                ssOut2 << nPre << "->" << strOut;
                strOut = ssOut2.str();
            }
            outFile << strOut << endl;
        }
        
        // times of iteration
        outFile << "Iteration: " << count << endl;
    }
    outFile.close();
}

int main(int argc, char **argv){
    if (argc != 2){
        return 0;
    }
    srcFileName = argv[1];
    vector<vector<int>> newGraph = readFile(argv[1]);
    bellmanFord(newGraph);
    return 0;
}

