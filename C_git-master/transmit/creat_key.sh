#!/bin/bash

echo "生成2048位的私钥文件"
openssl genrsa -out privkey.pem 2048

echo "以数字证书文件(包含私钥文件)签发私钥文件"
openssl req -new -x509 -key privkey.pem -out cacert.pem -days 1095

echo "完成"



