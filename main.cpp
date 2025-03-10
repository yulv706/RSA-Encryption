#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <iterator>

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

void printUsage(const char* programName) {
    cerr << "Usage: " << programName << " <inputfile> -o <outputfile>\n";
    cerr << "Example: " << programName << " message.txt -o encrypted.txt\n";
}

int main(int argc, char* argv[]) {
    // 命令行参数验证
    if (argc != 4 || string(argv[2]) != "-o") {
        printUsage(argv[0]);
        return 1;
    }

    const string inputPath = argv[1];
    const string outputPath = argv[3];

    // 读取输入文件
    ifstream inFile(inputPath);
    if (!inFile) {
        cerr << "Error: Cannot open input file: " << inputPath << "\n";
        return 1;
    }
    
    // 读取文件内容并转换为大写
    string message((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
    inFile.close();
    transform(message.begin(), message.end(), message.begin(), ::toupper);

    // 验证字符并转换为数字表示
    vector<int> numericMsg;
    for (char c : message) {
        if (c == ' ') {
            numericMsg.push_back(26);
        } else if (c >= 'A' && c <= 'Z') {
            numericMsg.push_back(c - 'A');
        } else {
            cerr << "Error: Invalid character '" << c 
                 << "'. Only A-Z and space allowed.\n";
            return 1;
        }
    }

    // 生成RSA参数
    unsigned p, q;
    do {
        int rand1 = 500 + rand() % 500;
        int rand2 = 500 + rand() % 500;
        p = nthPrime(rand1);
        q = nthPrime(rand2);
    } while (p == q);

    const unsigned long long n_val = p * q;
    const unsigned long long phi = (p-1) * (q-1);

    // 选择加密指数e
    unsigned long long e;
    int x, y;
    do {
        e = 65537; // 常见选择
        if (e < phi && gcdExtended(e, phi, &x, &y) == 1)
            break;
        e = 3;     // 备用选择
    } while (gcdExtended(e, phi, &x, &y) != 1);

    // 计算解密指数d
    const long long d = modInverse(e, phi);
    if (d == -1) {
        cerr << "Error: Failed to find modular inverse.\n";
        return 1;
    }

    // 分块处理消息（每两个字符一个块）
    vector<unsigned long long> blocks;
    for (size_t i=0; i<numericMsg.size(); i+=2) {
        long block = numericMsg[i] * 100;
        if (i+1 < numericMsg.size())
            block += numericMsg[i+1];
        blocks.push_back(block);
    }

    // 加密消息
    const auto encrypted = crypto(blocks, e, n_val);

    // 写入加密结果到输出文件
    ofstream outFile(outputPath);
    if (!outFile) {
        cerr << "Error: Cannot open output file: " << outputPath << "\n";
        return 1;
    }
    
    for (const auto& num : encrypted) {
        outFile << num << " ";
    }
    outFile.close();

    // 在控制台显示关键信息
    cout << "Successfully encrypted " << inputPath << "\n";
    cout << "Public Key (e, n): (" << e << ", " << n_val << ")\n";
    cout << "Private Key (d, n): (" << d << ", " << n_val << ")\n";
    cout << "Encrypted data saved to: " << outputPath << "\n";

    return 0;
}