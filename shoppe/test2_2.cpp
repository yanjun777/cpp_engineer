#include<string>
#include<map>
#include<unordered_map>
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
            unordered_map<char,char> mp1,mp2; 
            for(int i = 0; i < n; ++i){
                char char_a = input_a[i];
                char char_b = input_b[i]; 

                if(mp1.count(char_a)){
                    if(mp1[char_a] != char_b){
                        return false; 
                    }
                }else{
                    mp1[char_a] = char_b; 
                }

                if(mp2.count(char_b)){
                    if(mp2[char_b] != char_a){
                        return false; 
                    }
                }else{
                    mp2[char_b] = char_a; 
                }

               // checing relative order
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
    