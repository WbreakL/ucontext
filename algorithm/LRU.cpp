#include<bits/stdc++.h>


using namespace std;

/**
 * lru design
 * @param operators int整型vector<vector<>> the ops
 * @param k int整型 the k
 * @return int整型vector
 */
bool find_key(vector<pair<int, int>>& tmp, int num) {
    int len = tmp.size();
    for (int i = 0;i < len;i++) {
        if (tmp[i].first == num)
            return true;
    }
    return false;
}
int find_value(vector<pair<int, int>>& tmp, int num) {
    int len = tmp.size();
    for (int i = 0;i < len;i++) {
        if (tmp[i].first == num)
            return tmp[i].second;
    }
    return -1;
}
void change_second(vector<pair<int, int>>& tmp, int num, int des) {
    int len = tmp.size();
    for (int i = 0;i < len;i++) {
        if (tmp[i].first == num)
            tmp[i].second = des;
    }
}
vector<int> LRU(vector<vector<int> >& operators, int k) {
    // write code here
    vector<int> ans;
    vector<pair<int, int>> tmp;
    int len1 = operators.size();
    int len2 = operators[0].size();
    for (int i = 0;i < len1;i++) {
        /*for(int j=0;j<len2;j++){

        }*/
        if (operators[i][0] == 1) {
            if (!find_key(tmp, operators[i][1])) {
                if (tmp.size() < k)
                    tmp.push_back(pair<int, int>{operators[i][1], operators[i][2]});
                else {
                    tmp.erase(tmp.begin());
                    tmp.push_back(pair<int, int>{operators[i][1], operators[i][2]});
                }
            }
            else {
                //update
                change_second(tmp, operators[i][1], operators[i][2]);
            }
        }
        else {
            ans.push_back(find_value(tmp, operators[i][1]));
            if (find_key(tmp, operators[i][1])) {
                //update
                tmp.erase(find(tmp.begin(), tmp.end(), pair<int, int>{operators[i][1], find_value(tmp, operators[i][1])}));
                tmp.push_back(pair<int, int>{operators[i][1], find_value(tmp, operators[i][1])});
            }


        }
    }
    return ans;
}

void display(vector<int>& a) {
    int len = a.size();
    for (int i = 0;i < len;i++) {
        cout << a[i] << endl;
    }
}

int main(int argc, char* argv[]) {

    vector<vector<int>> res{ {1,1,1},{1,2,2},{1,1,3},{2,1},{2,1} };
    vector<int> target = LRU(res, 3);
    display(target);
    return 0;
}