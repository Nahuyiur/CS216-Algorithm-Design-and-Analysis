#include <deque> // 新增头文件
#include <iostream>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <algorithm>
#include <unordered_set> // 新增头文件
#include <queue> // 新增头文件
using namespace std;
struct element {
    long key;
    long value;
    long freq;
    long lasttime;
    element(long k, long v, long f, long l) : key(k), value(v), freq(f), lasttime(l) {}

    friend ostream &operator<<(ostream &os, const element &e) {
        os << "[key:" << e.key << ", value:" << e.value 
           << ", freq:" << e.freq << ", lasttime:" << e.lasttime << "]";
        return os;
    }
};

struct compare {
    bool operator()(const element &a, const element &b) const {
        if (a.freq == b.freq)
            return a.lasttime < b.lasttime;
        return a.freq < b.freq; 
    }
};
class CustomeHeap {
    public:
        vector<element> minheap;
        unordered_map<long, long> pos;
        unordered_set<long> active_keys; // 新增：跟踪堆中的键

        void siftUp(long i) {
            compare comp;
            while (i > 0) {
                long parent = (i - 1) / 2;
                if (comp(minheap[i], minheap[parent])) {
                    swap(minheap[parent], minheap[i]);
                    pos[minheap[parent].key] = parent;
                    pos[minheap[i].key] = i;
                    i = parent;
                } else {
                    break;
                }
            }
        }
    
        void siftDown(long i) {
            compare comp;
            long n = minheap.size();
            while (true) {
                long left = 2 * i + 1;
                long right = 2 * i + 2;
                long smallest = i;
                if (left < n && comp(minheap[left], minheap[smallest])) {
                    smallest = left;
                }
                if (right < n && comp(minheap[right], minheap[smallest])) {
                    smallest = right;
                }
                if (smallest != i) {
                    swap(minheap[i], minheap[smallest]);
                    pos[minheap[i].key] = i;
                    pos[minheap[smallest].key] = smallest;
                    i = smallest;
                } else {
                    break;
                }
            }
        }
        
        void insert(const element &e) {
            minheap.push_back(e);
            long index = minheap.size() - 1;
            pos[e.key] = index;
            active_keys.insert(e.key); // 插入时添加到 active_keys
            siftUp(index);
        }
        void update(long key,long value,long time) {
            // 查找元素是否存在
            if (pos.find(key) != pos.end()) {
                long i = pos[key];  // 获取该元素的索引
                minheap[i].value = value;  // 更新值
                minheap[i].freq += 1;   // 将freq加1
                minheap[i].lasttime = time;  // 更新lasttime为给定的time参数
                long parent = (i - 1) / 2;
                if (i > 0 && compare()(minheap[i], minheap[parent])) {
                    siftUp(i);  // 如果需要上浮，就调用siftUp
                } else {
                    siftDown(i);  // 否则调用siftDown
                }
            }else{
                //创建这个元素
                insert(element(key,value,1,time));
            }
        }
        void remove(long key) {
            if (pos.find(key) != pos.end()) {
                long i = pos[key];
                pos.erase(key);
                active_keys.erase(key); // 移除时从 active_keys 删除
                long last = minheap.size() - 1;
                if (i != last) {
                    minheap[i] = minheap[last];
                    pos[minheap[i].key] = i;
                }
                minheap.pop_back();
                if (i < minheap.size()) {
                    long parent = (i - 1) / 2;
                    if (i > 0 && compare()(minheap[i], minheap[parent])) {
                        siftUp(i);
                    } else {
                        siftDown(i);
                    }
                }
            }
        }
        void pop() {
            if (!minheap.empty()) {
                remove(minheap[0].key);
            }
        }    
        
    
        long getKey(long key,long time){
            if(pos.find(key)!=pos.end()){
                long i=pos[key];
                long ans=minheap[i].value;
                minheap[i].freq += 1; 
                minheap[i].lasttime = time; 
                long parent = (i - 1) / 2;
                if (i > 0 && compare()(minheap[i], minheap[parent])) {
                    siftUp(i);
                } else {
                    siftDown(i);
                }
    
                return ans;
            } else {
                return -1;
            }
        }
        long findLastAccess(long key){
            if(pos.find(key)==pos.end())return -1;
            return minheap[pos[key]].lasttime;
        }
        bool findKey(long key){
            return pos.find(key)!=pos.end(); //只是为了找到这个元素
        }
        element top() {
            if (!minheap.empty())
                return minheap.front();
            throw runtime_error("Heap is empty!");
        }
        
        // 判断堆是否为空
        bool empty() {
            return minheap.empty();
        }
        bool full(long n){
            return minheap.size()>=n;
        }
    
        void elements() {
            cout<<"最小堆中还有的元素： ";
            for (const auto &e : minheap) {
                cout << e << endl;
            }
        }
    };
int main() {
    long n, m, t0;
    cin >> n >> m >> t0;

    CustomeHeap heap;
    deque<pair<long, long>> time_queue; // 新增：时间队列

    for (long t = 0; t < m; t++) {
        // 1. 检查过期元素
        while (!time_queue.empty()) {
            auto [t_old, key_old] = time_queue.front();
            if (t - t_old > t0) { // 超过过期时间
                if (heap.active_keys.count(key_old) &&  //双重检查，看是不是在heap中
                    heap.findLastAccess(key_old) == t_old) {
                    heap.remove(key_old); // 移除过期键
                }
                time_queue.pop_front(); // 弹出已检查的记录
            } else {
                break; // 后续记录未过期，退出循环
            }
        }

        // 2. 处理操作
        long op;
        cin >> op;

        if (op == 1) { // get 操作
            long key;
            cin >> key;
            long value = heap.getKey(key, t);
            cout << value << endl;
            if (value != -1) { // 如果键存在，记录访问
                time_queue.push_back({t, key});
            }
        } else if (op == 2) { // put 操作
            long key, value;
            cin >> key >> value;
            if (heap.findKey(key)) {
                heap.update(key, value, t);
            } else {
                if (heap.full(n)) {
                    long topKey = heap.top().key;
                    heap.remove(topKey); // 移除堆顶
                }
                heap.insert(element(key, value, 1, t));
            }
            time_queue.push_back({t, key}); // 记录访问
        }
    }

    // 3. 输出结果
    if (heap.empty()) {
        cout << "empty" << endl;
    } else {
        vector<element> sorted_elements = heap.minheap;
        sort(sorted_elements.begin(), sorted_elements.end(), 
             [](const element &a, const element &b) {
                 return a.lasttime < b.lasttime;
             });
        for (size_t i = 0; i < sorted_elements.size(); ++i) {
            cout << sorted_elements[i].value << " ";
        }
        cout << endl;
    }
    return 0;
}