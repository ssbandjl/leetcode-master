#include <stdio.h>
#include <stdlib.h>
#include <mxml.h>

int main()
{
    mxml_node_t *tree;   // XML 树的根节点
    mxml_node_t *data;   // 数据节点
    mxml_node_t *value;  // 值节点
    mxml_node_t *node;   //

    // 创建 XML 树的根节点
    tree = mxmlNewXML("1.0");

    // 创建数据节点
    data = mxmlNewElement(tree, "lists");

    // 创建值节点并设置属性
    value = mxmlNewElement(data, "list");
    mxmlElementSetAttr(value, "device", "100010350000000001");
    mxmlElementSetAttr(value, "type", "10001d");
    
    node = mxmlNewElement(value, "CPU_PERCENT"); 
    mxmlNewText(node, 0, "0.03");

    node = mxmlNewElement(value, "MEM_PERCENT"); 
    mxmlNewText(node, 0, "0.13");

    // 将 XML 树保存到文件中
    FILE *fp = fopen("output.mxml", "w");
    mxmlSaveFile(tree, fp, MXML_NO_CALLBACK);
    fclose(fp);
    
    // 输出到屏幕
    mxmlSaveFile(tree, stdout, MXML_NO_CALLBACK);

    // 输出到字符串数组
    char buffer[16384] = {0};
    mxmlSaveString(tree, buffer, sizeof(buffer), MXML_NO_CALLBACK);
    
    printf("buffer: %s", buffer); 

    // 释放 XML 树内存
    mxmlDelete(tree);
    return 0;
}

// gcc `pkg-config --cflags --libs mxml` main.c -o main
