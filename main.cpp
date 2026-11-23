#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <queue>
#include <random>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

struct Question { string text; vector<string> options; char correct; string category, difficulty; };
struct Player { string name; int score = 0; };

string trim(const string& s) {
    size_t a=s.find_first_not_of(" \t\r\n");
    if(a==string::npos) return "";
    size_t b=s.find_last_not_of(" \t\r\n");
    return s.substr(a,b-a+1);
}
string lower(string s) {
    transform(s.begin(),s.end(),s.begin(),[](unsigned char c){return char(tolower(c));});
    return s;
}
int readInt(const string& prompt,int lo,int hi) {
    while(true) {
        cout<<prompt;
        string line; getline(cin,line); stringstream ss(line); int x; char extra;
        if((ss>>x) && !(ss>>extra) && x>=lo && x<=hi) return x;
        cout<<"Enter a number from "<<lo<<" to "<<hi<<".\n";
    }
}
char readOption(const vector<char>& allowed) {
    while(true) {
        cout<<"Your answer (";
        for(size_t i=0;i<allowed.size();++i) cout<<(i?", ":"")<<allowed[i];
        cout<<"): ";
        string s; getline(cin,s); s=lower(trim(s));
        if(s.size()==1) {
            char c=char(toupper(s[0]));
            if(find(allowed.begin(),allowed.end(),c)!=allowed.end()) return c;
        }
        cout<<"Invalid choice. Select one of the displayed options.\n";
    }
}
vector<Question> defaultQuestions() {
    return {
      {"Which data structure follows FIFO order?",{"Stack","Queue","Tree","Graph"},'B',"DSA","Easy"},
      {"Average time complexity of binary search on a sorted array?",{"O(n)","O(log n)","O(n log n)","O(1)"},'B',"Searching","Easy"},
      {"Which data structure follows LIFO order?",{"Queue","Array","Stack","Linked list"},'C',"DSA","Easy"},
      {"Which sorting algorithm repeatedly selects the smallest remaining element?",{"Merge sort","Selection sort","Quick sort","Heap sort"},'B',"Sorting","Medium"},
      {"Which traversal of a binary search tree gives sorted order?",{"Preorder","Postorder","Inorder","Level order"},'C',"Trees","Medium"},
      {"Which of these is a linear data structure?",{"Graph","Tree","Queue","Heap"},'C',"DSA","Easy"},
      {"Worst-case time complexity of linear search?",{"O(1)","O(log n)","O(n)","O(n log n)"},'C',"Searching","Easy"},
      {"Which technique does merge sort use?",{"Greedy","Divide and conquer","Backtracking","Dynamic programming"},'B',"Sorting","Medium"},
      {"Which STL container is commonly used for FIFO?",{"stack","queue","set","map"},'B',"C++ STL","Medium"},
      {"In a max-heap, which element is at the top?",{"Smallest","Largest","Oldest","Newest"},'B',"Heap","Medium"},
      {"Which graph traversal typically uses a queue?",{"DFS","BFS","Inorder","Binary search"},'B',"Graphs","Medium"},
      {"Average lookup complexity of unordered_map?",{"O(n)","O(log n)","O(1)","O(n log n)"},'C',"C++ STL","Medium"},
      {"Which pointer marks the end of a singly linked list?",{"Head","Null next pointer","Middle","Root"},'B',"Linked list","Easy"},
      {"Which operation removes the top item from a stack?",{"Enqueue","Dequeue","Pop","Front"},'C',"Stack","Easy"},
      {"Time complexity of comparison-based merge sort?",{"O(n)","O(log n)","O(n log n)","O(n^2)"},'C',"Sorting","Medium"},
      {"Which structure is suited to a priority-based leaderboard?",{"Simple queue","Priority queue","Stack","Array only"},'B',"C++ STL","Medium"},
      {"Which is not a standard asymptotic notation?",{"Big-O","Big-Omega","Big-Theta","Big-Delta"},'D',"Analysis","Medium"},
      {"In an undirected graph, an edge connects:",{"A vertex to itself only","Two vertices","Only the root","Two arrays"},'B',"Graphs","Easy"},
      {"Which C++ feature groups data and member functions?",{"Namespace","Class","Macro","Header"},'B',"C++","Easy"},
      {"Which data structure manages function calls?",{"Queue","Stack","Graph","Hash table"},'B',"DSA","Medium"}
    };
}
bool saveQuestions(const vector<Question>& qs) {
    ofstream out("questions.txt");
    if(!out) return false;
    for(const auto& q:qs)
        out<<q.text<<'|'<<q.options[0]<<'|'<<q.options[1]<<'|'<<q.options[2]<<'|'<<q.options[3]<<'|'<<q.correct<<'|'<<q.category<<'|'<<q.difficulty<<'\n';
    return bool(out);
}
vector<Question> loadQuestions() {
    ifstream in("questions.txt"); vector<Question> qs; string line;
    while(getline(in,line)) {
        stringstream ss(line); vector<string> f; string item;
        while(getline(ss,item,'|')) f.push_back(trim(item));
        if(f.size()!=8 || f[0].empty() || f[5].empty()) continue;
        char ans=char(toupper(f[5][0]));
        if(ans<'A'||ans>'D') continue;
        qs.push_back({f[0],{f[1],f[2],f[3],f[4]},ans,f[6],f[7]});
    }
    return qs;
}
void instructions() {
    cout<<"\nHOW TO PLAY\n2-4 players share one computer. No account or internet required.\n"
        <<"Players take turns answering questions. Correct = +10; wrong = 0.\n"
        <<"Each player gets one 50-50 lifeline. Highest score wins.\n\n";
}
vector<Player> setupPlayers() {
    int n=readInt("Number of players (2-4): ",2,4); vector<Player> ps;
    for(int i=0;i<n;++i) {
        while(true) {
            cout<<"Player "<<i+1<<" name: "; string name; getline(cin,name); name=trim(name);
            bool duplicate=false;
            for(const auto& p:ps) if(lower(p.name)==lower(name)) duplicate=true;
            if(name.empty()||duplicate) { cout<<(name.empty()?"Name cannot be empty.\n":"Name already used.\n"); continue; }
            Player player; player.name=name; player.score=0; ps.push_back(player); break;
        }
    }
    return ps;
}
void askQuestion(const Question& q,Player& p,int& lifeline,int number) {
    cout<<"\n----------------------------------------\nQuestion "<<number<<" | "<<q.category<<" | "<<q.difficulty<<"\n"<<q.text<<"\n";
    for(int i=0;i<4;++i) cout<<char('A'+i)<<". "<<q.options[i]<<"\n";
    vector<char> allowed={'A','B','C','D'};
    cout<<"Use your 50-50 lifeline? (y/n): ";
    string response; getline(cin,response);
    if(lower(trim(response))=="y") {
        if(lifeline) cout<<"You have already used your lifeline.\n";
        else {
            lifeline=1;
            vector<char> wrong;
            for(char c='A';c<='D';++c) if(c!=q.correct) wrong.push_back(c);
            random_device rd; mt19937 gen(rd()); shuffle(wrong.begin(),wrong.end(),gen);
            allowed={q.correct,wrong[0],wrong[1]};
            sort(allowed.begin(),allowed.end());
            cout<<"50-50: remaining options are ";
            for(char c:allowed) cout<<c<<" ";
            cout<<"\n";
        }
    }
    char ans=readOption(allowed);
    if(ans==q.correct) { p.score+=10; cout<<"Correct! +10 points.\n"; }
    else cout<<"Incorrect. Correct answer: "<<q.correct<<". "<<q.options[q.correct-'A']<<"\n";
    cout<<p.name<<"'s score: "<<p.score<<"\n";
}
void showLeaderboard(vector<Player> ps) {
    stable_sort(ps.begin(),ps.end(),[](const Player&a,const Player&b){return a.score>b.score;});
    cout<<"\n=========== FINAL LEADERBOARD ===========\n";
    int rank=1;
    for(size_t i=0;i<ps.size();++i) {
        if(i && ps[i].score!=ps[i-1].score) rank=int(i)+1;
        cout<<rank<<". "<<left<<setw(20)<<ps[i].name<<right<<ps[i].score<<" points\n";
    }
    cout<<"=========================================\n";
}
void saveResult(const vector<Player>& ps) {
    ofstream out("results.txt",ios::app);
    if(!out) { cout<<"Could not save results.\n"; return; }
    out<<"GAME";
    for(const auto&p:ps) out<<" | "<<p.name<<": "<<p.score;
    out<<"\n";
    cout<<"Game result saved in results.txt\n";
}
void previousResults() {
    ifstream in("results.txt");
    if(!in) { cout<<"No previous results yet.\n"; return; }
    cout<<"\nPREVIOUS RESULTS\n"; string line;
    while(getline(in,line)) cout<<line<<"\n";
}
void startGame(vector<Question> qs) {
    if(qs.size()<2) { cout<<"At least two valid questions are needed.\n"; return; }
    vector<Player> players=setupPlayers();
    int maxQ=int(qs.size()/players.size());
    int each=readInt("Questions per player (1-"+to_string(maxQ)+"): ",1,maxQ);
    random_device rd; mt19937 gen(rd()); shuffle(qs.begin(),qs.end(),gen);
    queue<Question> questionQueue;
    for(const auto&q:qs) questionQueue.push(q);
    unordered_map<string,int> scoreLookup;
    for(const auto&p:players) scoreLookup[p.name]=0;
    vector<int> lifelineUsed(players.size(),0);
    int total=each*int(players.size());
    for(int t=0;t<total && !questionQueue.empty();++t) {
        int idx=t%int(players.size());
        cout<<"\n>>> "<<players[idx].name<<"'s turn <<<\n";
        Question q=questionQueue.front(); questionQueue.pop();
        askQuestion(q,players[idx],lifelineUsed[idx],t/int(players.size())+1);
        scoreLookup[players[idx].name]=players[idx].score;
    }
    showLeaderboard(players);
    saveResult(players);
}
int main() {
    vector<Question> questions=loadQuestions();
    if(questions.empty()) {
        questions=defaultQuestions();
        if(saveQuestions(questions)) cout<<"Created default question bank: questions.txt\n";
    }
    while(true) {
        cout<<"\n====================================\n QUIZ BATTLE ARENA\n Offline C++ Multiplayer Quiz\n====================================\n"
            <<"1. Start New Game\n2. Instructions\n3. Previous Results\n4. Exit\n";
        int choice=readInt("Choose (1-4): ",1,4);
        if(choice==1) startGame(questions);
        else if(choice==2) instructions();
        else if(choice==3) previousResults();
        else { cout<<"Thanks for playing!\n"; break; }
        cout<<"\nPress Enter to continue..."; string pause; getline(cin,pause);
    }
    return 0;
}