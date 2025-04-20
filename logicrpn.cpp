#include<bits/stdc++.h>

using namespace std; 

string N, RPN; 
set<char> sto;
string str; 
map<char, bool> anhxa;
int n;
bool Dung = true, Sai = true;

void chuyenDoi() { 
        // ! p -> r <=> !p | r; 
        stack<char> st;
        queue<char> qu;
        int cnt = 0;
        map<char, int> prio; 
        prio['!'] = 3;
        prio['&'] = prio['|'] = 2;
        prio['-'] = 2; 
        prio['('] = 0;
        vector<int> bu(2e5+1);
        for (int i = 0; i < N.length(); i++) {
                if (N[i] >= 'a' && N[i] <= 'z' || N[i] == '1' || N[i] == '0') {
                        qu.push(N[i]); 
                        while (bu[cnt]) { 
                                qu.push('!'); 
                                bu[cnt]--;
                        }
                        if (N[i] >= 'a' && N[i] <= 'z') sto.insert(N[i]); 
                }
                else { 
                        if (N[i] != ')') {
                                if (N[i] == '!') { 
                                        bu[cnt]++;
                                }
                                else if (st.empty() || N[i] == '(') {
                                        st.push(N[i]);
                                }
                                else {
                                        while(!st.empty() && prio[st.top()] >= prio[N[i]]) {
                                                qu.push(st.top()); 
                                                st.pop();
                                        }
                                        st.push(N[i]);
                                }
                        }
                        else { 
                                while (st.top() != '(') { 
                                        qu.push(st.top()); 
                                        st.pop();
                                }
                                cnt--;
                                while (bu[cnt]) { 
                                        qu.push('!'); 
                                        bu[cnt]--;
                                }
                                st.pop();
                        }
                        if (N[i] == '(') cnt++;
                }
        }
        while (!st.empty()) { 
                qu.push(st.top()); 
                st.pop();
        }
        while (!qu.empty()) { 
                RPN += qu.front(); 
                qu.pop();
        }
        for (char c : sto) str.push_back(c);
        n = str.length();
}

bool f() { 
        stack<bool> st;
        for (char c : RPN) { 
                if (c >= 'a' && c <= 'z') {
                        st.push(anhxa[c]); 
                }
                else if (c == '1' || c == '0') {
                        st.push(c - 48);
                }
                else { 
                        if (c == '&') { 
                                bool a = st.top(); 
                                st.pop(); 
                                bool b = st.top(); 
                                st.pop(); 
                                bool res = (b & a);
                                st.push(res);
                        }
                        if (c == '|') { 
                                bool a = st.top(); 
                                st.pop(); 
                                bool b = st.top(); 
                                st.pop(); 
                                bool res = a | b;
                                st.push(res);
                        }
                        if (c == '-') {
                                bool a = st.top(); 
                                st.pop(); 
                                bool b = st.top(); 
                                st.pop(); 
                                bool res = !b | a;
                                st.push(res);
                        }
                        if (c == '!') { 
                                bool a = !(st.top());
                                st.pop(); 
                                st.push(a);
                        }
                }
        }
        return st.top();
}

void backtrack(int ix, string permu) { 
        if (ix >= n-1) {
                for (int i = 0; i < n; i++) {
                        anhxa[str[i]] = (permu[i] - '0'); 
                }
                for (int i = 0; i < n; i++) { 
                        cout << anhxa[str[i]] << " | ";
                }
                bool res = f();
                if (res == 1) Sai = false; 
                else Dung = false;
                cout << res;
                cout << '\n';
                return;
        }
        backtrack(ix+1, permu + "0"); 
        backtrack(ix+1, permu + "1"); 
}

void lapBang() { 
        for (char c : str) { 
                cout << c << " | ";
        }
        cout << 'F' << '\n';
        for (int i = 0; i < 5*n; i++) cout << "-"; 
        cout << '\n';
        backtrack(-1, ""); 
}

int main() { 
        cin >> N; 
        chuyenDoi();
        lapBang();
        if (Dung) { 
                cout << "HANG DUNG";
        }
        if (Sai) { 
                cout << "HANG SAI";
        }
}