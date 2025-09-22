#include<string>
#include<map>
using namespace std; 
class Solution {
    public:
        /**
         * Note: 类名、方法名、参数名已经指定，请勿修改
         *
         * 
         * true 表示类型相同，false表示不相同
         * @param input_a string字符串  
         * @param input_b string字符串  
         * @return bool布尔型
         */
        bool CheckPatern(string input_a, string input_b) {
            // write code here
            int n = input_a.size(), m = input_b.size(); 
            if(n != m) return false; 
            // check mapping 
            map<char,int> mp1,mp2; 
            vector<int> pattern1(n), pattern2(n); 
            int idx1 = 0, idx2 = 0;
            for(int i = 0; i< n; ++i){
                if(mp1.find(input_a[i]) != mp1.end() ) mp1[input_a[i]] ++; 
                pattern1[i] = mp1[input_a[i]]; 
                if(mp2.find(input_a[i]) != mp2.end() ) mp2[input_b[i]] ++; 
                pattern2[i] = mp2[input_b[i]]; 
                if(pattern1[i] != pattern2[i]) return false; 
            }

            // checing relative order
            for(int i  = 0; i< n;++i){
                for(int j = i+1; j < n; ++j){
                    if((input_a[i] < input_a[j] && input_b[i] >= input_b[j]) ||
                        (input_a[i] > input_a[j] && input_b[i] <= input_b[j]) ||
                        (input_a[i] != input_a[j] && input_b[i] == input_b[j])
                    ) return false; 
                }
            }

            return true; 
    
        }
    };
    