# 本实验为物联网安全第一次实验

部分要求如下：

加密方法格式为：
Void encrypt（String file,String destFile,String Key）{…}
解密方法格式为：
Void decode（ String file,String destFile,String Key）{…}
密钥生成方法格式为：
String genKey(int length) {…}

参数说明：
file 待加密(解密)的文件路径
destFile 加密(解密)后文件的存放路径
Key 密钥 length 密钥长度

现缺少功能：
+ 现在只能处理A-Z（小写字母会转化为大写），后面需要可以处理数字，特殊字符（如换行符）、甚至汉字
+ 现在只能支持加密文件`./a.out <inputfile> -o <outputfile>`,虽然已有解密算法，但是还没落实
+ 上面加密解密格式还没落实



