#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <algorithm>

using namespace std;

// 优化的质数判断函数
bool isPrime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n%2 == 0 || n%3 == 0) return false;
    for(int i=5; i*i<=n; i+=6) {
        if(n%i == 0 || n%(i+2) == 0)
            return false;
    }
    return true;
}

// 扩展欧几里得算法
int gcdExtended(int a, int b, int *x, int *y) {
    if (a == 0) {
        *x = 0, *y = 1;
        return b;
    }
    int x1, y1;
    int gcd = gcdExtended(b%a, a, &x1, &y1);
    *x = y1 - (b/a) * x1;
    *y = x1;
    return gcd;
}

// 模逆计算
int modInverse(int a, int m) {
    int x, y;
    int g = gcdExtended(a, m, &x, &y);
    if (g != 1) return -1;
    return (x%m + m) % m;
}

// 优化的质数生成函数
int nthPrime(int n) {
    if (n <= 0) return -1;
    int count = 0, candidate = 2;
    while(count < n) {
        if (isPrime(candidate)) ++count;
        ++candidate;
    }
    return candidate - 1;
}

// 迭代实现的模幂运算
unsigned long long modExp(unsigned long long base, unsigned long long exponent, unsigned long long mod) {
    unsigned long long result = 1;
    base %= mod;
    while (exponent > 0) {
        if (exponent % 2 == 1)
            result = (result * base) % mod;
        exponent >>= 1;
        base = (base * base) % mod;
    }
    return result;
}

// 加密/解密函数
vector<unsigned long long> crypto(const vector<unsigned long long>& msg, unsigned long long key, unsigned long long n) {
    vector<unsigned long long> result;
    for (auto num : msg)
        result.push_back(modExp(num, key, n));
    return result;
}

// 显示向量内容
template<typename T>
void displayVector(const vector<T>& vec, const string& title) {
    cout << title << ":\n";
    for (const auto& elem : vec)
        cout << elem << " ";
    cout << "\n\n";
}

int main() {
    srand(time(0));

    // 输入处理
    string message;
    cout << "Enter message (A-Z, space only): ";
    getline(cin, message);
    transform(message.begin(), message.end(), message.begin(), ::toupper);

    // 转换为数字表示
    vector<int> numericMsg;
    for (char c : message) {
        if (c == ' ') 
            numericMsg.push_back(26);
        else if (c >= 'A' && c <= 'Z')
            numericMsg.push_back(c - 'A');
        else {
            cerr << "Invalid character detected. Only A-Z and space allowed.\n";
            return 1;
        }
    }

    // 生成质数p和q
    unsigned p, q;
    do {
        int rand1 = 500 + rand() % 500;
        int rand2 = 500 + rand() % 500;
        p = nthPrime(rand1);
        q = nthPrime(rand2);
    } while (p == q);

    // 计算RSA参数
    unsigned long long n_val = p * q;
    unsigned long long phi = (p-1) * (q-1);
    
    // 选择e并确保与phi互质
    unsigned long long e;
    int x, y;
    do {
        e = 65537; // 常见选择
        if (e < phi && gcdExtended(e, phi, &x, &y) == 1)
            break;
        e = 3;     // 备用选择
    } while (gcdExtended(e, phi, &x, &y) != 1);

    // 计算d
    long long d = modInverse(e, phi);
    if (d == -1) {
        cerr << "Error: Failed to find modular inverse.\n";
        return 1;
    }

    // 分块处理
    vector<unsigned long long> blocks;
    for (size_t i=0; i<numericMsg.size(); i+=2) {
        long block = numericMsg[i] * 100;
        if (i+1 < numericMsg.size())
            block += numericMsg[i+1];
        blocks.push_back(block);
    }

    // 加密解密过程
    auto encrypted = crypto(blocks, e, n_val);
    auto decrypted = crypto(encrypted, d, n_val);

    // 解密恢复消息
    vector<int> decoded;
    for (auto num : decrypted) {
        decoded.push_back(num / 100);
        if (num % 100 != 0)
            decoded.push_back(num % 100);
    }

    // 转换回字符
    string recovered;
    for (int num : decoded) {
        if (num == 26)
            recovered += ' ';
        else if (num >= 0 && num <= 25)
            recovered += char(num + 'A');
    }

    // 显示结果
    cout << "\nOriginal Message: " << message
         << "\nNumeric Values:   "; for (auto n : numericMsg) cout << n << " ";
    cout << "\nBlocks:           "; for (auto b : blocks) cout << b << " ";
    cout << "\n\nPublic Key (e, n): (" << e << ", " << n_val << ")"
         << "\nPrivate Key (d, n): (" << d << ", " << n_val << ")"
         << "\n\nEncrypted Blocks:  "; for (auto e : encrypted) cout << e << " ";
    cout << "\nDecrypted Blocks:  "; for (auto d : decrypted) cout << d << " ";
    cout << "\n\nRecovered Message: " << recovered << "\n";

    return 0;
}